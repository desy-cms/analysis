#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;

bool TriggerFired(const std::shared_ptr< Collection<TriggerObject> > objects, const unsigned int & nmin, const double & ptmin, const double & etamax, const double & detamax);

// =============================================================================================   
int main(int argc, char * argv[])
{
   std::cout << "Starting ...." << std::endl;
   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   std::cout << "Reading root files..." << std::endl; 
   Analysis analysis(inputList);
   
   std::cout << "Setting up physics objects to be used in the analysis..." << std::endl;
   // Physics Objects Collections
   // Jets
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // Trigger results
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   std::string hltPath = "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v";
   
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sDoubleJetC100");
   triggerObjects.push_back("hltDoubleJetsC100");
   triggerObjects.push_back("hltBTagCaloCSVp014DoubleWithMatching");
   triggerObjects.push_back("hltDoublePFJetsC100");
   triggerObjects.push_back("hltDoublePFJetsC100MaxDeta1p6");
   
   std::string trgobj_path = "MssmHbb/Events/selectedPatTrigger/";
   for ( auto & obj : triggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Certified lumis
   analysis.processJsonFile("pu45_selrun_json.txt");
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nCertEvts = 0;
   int nHltPath = 0;
   int nHltPathNew = 0;
   int nOffline = 0;
   int nOfflineNew = 0;
   
   std::string prevFile = "";
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      // look for the files containing run 283933
      if ( analysis.run() == 283933 )
      {
         if ( prevFile == "" || prevFile != analysis.fileName() )
         {
            prevFile = analysis.fileName();
            std::cout << prevFile << std::endl;
         }
      }
      
      if (!analysis.selectJson()) continue; // To use only goodJSonFiles
      ++nCertEvts;
      
//       std::cout << "++++++    ENTRY  " << i;
//       std::cout << std::endl;
//      
      // Trigger results
      // hltPath
      int trgFired = analysis.triggerResult(hltPath);
      if ( ! trgFired ) continue;
      ++nHltPath;
      
      // New trigger emulation
      auto l1Dijet100 = analysis.collection<TriggerObject>("hltL1sDoubleJetC100");
      auto l2Dijet100 = analysis.collection<TriggerObject>("hltDoubleJetsC100");
      auto l3Dijet100 = analysis.collection<TriggerObject>("hltDoublePFJetsC100");
      bool l1FiredNew = TriggerFired(l1Dijet100,2,100,2.263,1.6);
      bool l2FiredNew = TriggerFired(l2Dijet100,2,100,2.4,-1);
      bool l3FiredNew = TriggerFired(l3Dijet100,2,100,2.4,1.6);
      bool trgFiredNew = ( l1FiredNew && l2FiredNew && l3FiredNew );
      
      if ( trgFiredNew ) ++nHltPathNew;
      
      // Offline selection double b-tag
      // Jets - std::shared_ptr< Collection<Jet> >
      auto jets = analysis.collection<Jet>("Jets");
      // select good jets - passed jet id loose
      std::vector<Jet*> selJets;
      for ( int j = 0 ; j < jets->size() ; ++j )
         if ( jets->at(j).idLoose() ) selJets.push_back(&jets->at(j));

      // need at least 2 good jets
      if ( selJets.size() < 2 ) continue;      
      bool goodEvent = true;
      
      // Kinematic selection - 2 leading jets
      double ptmin = 100;
      double etamax = 2.2;
      double btagmin = 0.8;
      for ( int j = 0; j < 2; ++j )
      {
         if ( selJets[j]->pt() < ptmin || fabs(selJets[j]->eta()) > etamax || selJets[j]->btag() < btagmin )
         {
            goodEvent = false;
            break;
         }
      }
      
      if ( ! goodEvent ) continue;
      
      // try to match the offline to the online objects
      analysis.match<Jet,TriggerObject>("Jets",triggerObjects,0.5);
      // do trigger objects match with offline?
      for ( size_t to = 0; to < triggerObjects.size() ; ++to )
      {
         if ( ! selJets[0]->matched(triggerObjects[to]) || !  selJets[1]->matched(triggerObjects[to]) )
         {
            goodEvent = false;
            break;
         }
      }
      if ( ! goodEvent ) continue;
      
      // Additional kinematic cuts
      // delta R between the leading jets
      double dRmin = 1.;
      if ( selJets[0]->deltaR(*selJets[1]) < dRmin ) continue;
      // delta eta between two leading jets
      double detamax = 1.40;
      float deta12 =  fabs(selJets[0]->eta() - selJets[1]->eta()) ;
      if ( deta12 > detamax ) continue;
      
      goodEvent = true;
      
      ++nOffline;
      
      if ( trgFiredNew && goodEvent ) ++nOfflineNew;
      
   }
   
   std::cout << "Num trigger fired = " << nHltPath << std::endl;
   std::cout << "Num trigger fired && offline = " << nOffline << std::endl;
   std::cout << "Num new trigger fired = " << nHltPathNew << std::endl;
   std::cout << "Num new trigger fired && offline = " << nOfflineNew << std::endl;
   
   
}

bool TriggerFired(const std::shared_ptr< Collection<TriggerObject> > objects, const unsigned int & nmin, const double & ptmin, const double & etamax, const double & detamax)
{
   bool fired = false;
   bool kinsel = false;
   bool detasel = false;
   std::vector<TriggerObject> selectedObjects;
   for ( int j = 0 ; j < objects->size() ; ++j )
   {
      TriggerObject object = objects->at(j);
      if ( object.pt() >= ptmin && fabs(object.eta()) <= etamax ) 
      {
         selectedObjects.push_back(object);
      }
   }
   if ( selectedObjects.size() >= nmin ) kinsel = true;
   
   // Debugging
//    if ( !kinsel )
//    {
//    for ( int j1 = 0 ; j1 < objects->size() ; ++j1 )
//    {
//       TriggerObject obj = objects->at(j1);
//       std::cout << "pt = " << obj.pt() << " ,  " << obj.eta() << " ,  " << obj.phi() << std::endl;
//    }
//    std::cout << "-----" << std::endl;
//    }
   
   if ( detamax < 0 || ! kinsel  ) return kinsel;
   
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
   if ( pairs >= 1 ) detasel = true;
   
   fired = (kinsel && detasel);
   
   return fired;
   
}
