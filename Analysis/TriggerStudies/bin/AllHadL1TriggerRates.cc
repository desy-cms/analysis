#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 
#include "TH2.h" 
#include "TGraphAsymmErrors.h"

#include "Analysis/Tools/interface/Analysis.h"

#include "lumis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;

bool TriggerObjectSelection(const std::shared_ptr< Collection<TriggerObject> > objects, const unsigned int & nmin, const double & ptmin, const double & etamax, const double & detamax = -1);

// =============================================================================================   
int main(int argc, char * argv[])
{
   std::cout << "Starting ...." << std::endl;
   
   brilcalc bc = readLumisCsv("lumis.csv");
   std::map<int, std::map<int,float> > & lumiByLS = bc.lumiByLS;
   std::map<int, std::map<int,float> > & pileupByLS = bc.pileupByLS;
   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   std::cout << "Reading root files..." << std::endl; 
   Analysis analysis(inputList,"MssmHbbTrigger/Events/EventInfo");
   
   std::cout << "Setting up physics objects to be used in the analysis..." << std::endl;
   // Physics Objects Collections
   // Trigger results
   analysis.triggerResults("MssmHbbTrigger/Events/TriggerResults");
   std::string hltPath[20];
   hltPath[0] = "HLT_ZeroBias_v";
   hltPath[1] = "HLT_L1DoubleJetC100_v";
   
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sDoubleJetC100");
   
   std::string trgobj_path = "MssmHbbTrigger/Events/hltTriggerSummaryAOD/";
   for ( auto & obj : triggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Certified lumis
//   analysis.processJsonFile("json.txt");
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   
   int nRefTrg = 0;
   int nTrg = 0;
   
   TH1F * h_nRefTrg = new TH1F("h_nRefTrg","",30,20,50);
   TH1F * h_nTrg    = new TH1F("h_nTrg","",30,20,50);
   
   TH1F * h_lumiRefTrg = new TH1F("h_lumiRefTrg","",60,5000,15000);
   TH1F * h_lumiTrg    = new TH1F("h_lumiTrg","",60,5000,15000);
   
   TH1F * h_pu = new TH1F("h_pu","",100,0,100);
   TH1F * h_zb   = new TH1F("h_zb","",100,5000,15000);
   TH1F * h_lumi = new TH1F("h_lumi","",100,5000,15000);
   
   TH2F * h_pu_lumi = new TH2F("h_pu_lumi","",30,20,50,100,5000,15000);
   
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // skip runs
//      if ( analysis.run() == 282730 ) continue;
//      if ( analysis.run() == 283306 ) continue;
//      if ( analysis.run() == 283408 ) continue;
//      if ( analysis.run() == 283453 ) continue;
//      if ( analysis.run() == 283865 ) continue;
            
//      float pileup = analysis.lumiPileup();
//      float instLumi = analysis.instantLumi();

      float instLumi = lumiByLS[analysis.run()][analysis.lumiSection()]/23.4;
      float pileup   = pileupByLS[analysis.run()][analysis.lumiSection()];
      
//      instLumi = lumiByLS[analysis.run()][analysis.lumiSection()]/23.4*1.e6;
      
      float fbx = 2200./3564. * 40800000.;
      float sigMB = 80000;
      pileup = instLumi*sigMB/fbx;
      
//      std::cout << "Run= " << analysis.run() << "  LS = " << analysis.lumiSection() << "    Pile up = " << pileup << "   inst Lumi = " << instLumi << std::endl;

      h_pu -> Fill(pileup);
      h_lumi -> Fill(instLumi);
      
      h_pu_lumi -> Fill(pileup,instLumi);
      
      
//      if ( pileup < 20 ) continue;
      
      // Trigger results
      // Reference trigger
      if ( ! analysis.triggerResult(hltPath[0]) ) continue;
      ++nRefTrg;
      
      h_nRefTrg -> Fill(pileup);
      h_lumiRefTrg -> Fill(instLumi);
      h_zb -> Fill(instLumi);
      
      
      // Trigger basis
      if ( ! analysis.triggerResult(hltPath[1]) ) continue;
      
      
//       // New trigger emulation
//       auto l1Jet20    = analysis.collection<TriggerObject>("hltL1sSingleJet20_hltTriggerSummaryAOD");
//       bool l1New      = TriggerObjectSelection(l1Jet20,1,90,10);
//       
//       if ( ! l1New ) continue;
      
      ++nTrg;
      h_nTrg -> Fill(pileup);
      h_lumiTrg -> Fill(instLumi);
      
   }
   
   std::cout << "Num reference trigger fired = " << nRefTrg << std::endl;
   std::cout << "Num trigger fired           = " << nTrg << std::endl;
   std::cout << "Efficiency of the trigger   = " << (double) nTrg/nRefTrg << std::endl;
   
   TGraphAsymmErrors * g_effTrg = new TGraphAsymmErrors(h_nTrg,h_nRefTrg,"cl=0.683 b(1,1) mode");
   TGraphAsymmErrors * g_effLumiTrg = new TGraphAsymmErrors(h_lumiTrg,h_lumiRefTrg,"cl=0.683 b(1,1) mode");
   
   
   TFile * fout = new TFile("output.root","recreate");
   h_nRefTrg -> Write();
   h_nTrg -> Write();
   h_lumiRefTrg -> Write();
   h_lumiTrg -> Write();
   g_effTrg -> Write();
   g_effLumiTrg -> Write();
   
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

