#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 
#include "TH2.h" 
#include "TGraphAsymmErrors.h"

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;

bool TriggerObjectSelection(const std::shared_ptr< Collection<TriggerObject> > objects, const unsigned int & nmin, const double & ptmin, const double & etamax, const double & detamax = -1);

// =============================================================================================   
int main(int argc, char * argv[])
{
   std::cout << "Starting ...." << std::endl;
   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   std::cout << "Reading root files..." << std::endl; 
   Analysis analysis(inputList,"MssmHbbTrigger/Events/EventInfo");
   
   std::cout << "Setting up physics objects to be used in the analysis..." << std::endl;
   // Physics Objects Collections
   // Trigger results
   analysis.triggerResults("MssmHbbTrigger/Events/TriggerResults__HLT2");
   std::string hltPath[20];
   hltPath[0] = "HLT_ZeroBias_v";
   hltPath[1] = "HLT_L1SingleJet20_v";
   
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sSingleJet20_hltTriggerSummaryAOD__HLT2");
   
   std::string trgobj_path = "MssmHbbTrigger/Events/hltTriggerSummaryAOD__HLT2/";
   for ( auto & obj : triggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Certified lumis
//   analysis.processJsonFile("json.txt");
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   
   int nRefTrg = 0;
   int nTrg = 0;
   
   TH1F * h_nRefTrg = new TH1F("h_nRefTrg","",28,20,48);
   TH1F * h_nTrg    = new TH1F("h_nTrg","",28,20,48);
   
   TH1F * h_pu = new TH1F("h_pu","",100,0,100);
   TH1F * h_lumi = new TH1F("h_lumi","",100,0,30000);
   
   TH2F * h_pu_lumi = new TH2F("h_pu_lumi","",100,0,100,100,0,30000);
   
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      if ( analysis.run() != 283408 ) continue;
      if ( analysis.lumiSection() < 30 || analysis.lumiSection() > 101 ) continue; 
      
      float pileup = analysis.lumiPileup();
      float instLumi = analysis.instantLumi();
      
      h_pu -> Fill(pileup);
      h_lumi -> Fill(instLumi);
      
      h_pu_lumi -> Fill(pileup,instLumi);
      
      
//      if ( pileup < 20 ) continue;
      
      // Trigger results
      // Reference trigger
      if ( ! analysis.triggerResult(hltPath[0]) ) continue;
      ++nRefTrg;
      
      h_nRefTrg -> Fill(pileup);
      
      
      // Trigger basis
      if ( ! analysis.triggerResult(hltPath[1]) ) continue;
      
      
      // New trigger emulation
      auto l1Jet20    = analysis.collection<TriggerObject>("hltL1sSingleJet20_hltTriggerSummaryAOD__HLT2");
      bool l1New      = TriggerObjectSelection(l1Jet20,1,90,10);
      
      if ( ! l1New ) continue;
      
      ++nTrg;
      h_nTrg -> Fill(pileup);
      
   }
   
   std::cout << "Num reference trigger fired = " << nRefTrg << std::endl;
   std::cout << "Num trigger fired           = " << nTrg << std::endl;
   std::cout << "Efficiency of the trigger   = " << (double) nTrg/nRefTrg;
   
   TGraphAsymmErrors * g_effTrg = new TGraphAsymmErrors(h_nTrg,h_nRefTrg,"cl=0.683 b(1,1) mode");
   
   
   TFile * fout = new TFile("output.root","recreate");
   h_nRefTrg -> Write();
   h_nTrg -> Write();
   g_effTrg -> Write();
   
   h_pu -> Write();
   h_lumi -> Write();
   h_pu_lumi -> Write();

   
   fout -> Close();
   
}


bool TriggerObjectSelection(const std::shared_ptr< Collection<TriggerObject> > objects, const unsigned int & nmin, const double & ptmin, const double & etamax, const double & detamax)
{
   bool fired = false;
   bool objectSelection = false;
   bool deltaEtaSelection = false;
   std::vector<TriggerObject> selectedObjects;
   for ( int j = 0 ; j < objects->size() ; ++j )
   {
      TriggerObject object = objects->at(j);
      if ( object.pt() >= ptmin && fabs(object.eta()) <= etamax ) 
      {
         selectedObjects.push_back(object);
      }
   }
   if ( selectedObjects.size() >= nmin ) objectSelection = true;
   
   if ( detamax < 0 || ! objectSelection  ) return objectSelection;
   
   unsigned int pairs = 0;
   for ( size_t j1 = 0 ; j1 < selectedObjects.size()-1 ; ++j1 )
   {
      double eta1 = selectedObjects.at(j1).eta();
      for ( size_t j2 = j1+1 ; j2 < selectedObjects.size() ; ++j2 )
      {
         double eta2 = selectedObjects.at(j2).eta();
         if ( fabs(eta1-eta2) <= detamax ) 
         {
            ++pairs;
         }
      }
   }
   if ( pairs >= 1 ) deltaEtaSelection = true;
   
   fired = (objectSelection && deltaEtaSelection);
   
   return fired;
   
}
