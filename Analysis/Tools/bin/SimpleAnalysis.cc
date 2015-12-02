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


// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);
   
   std::string atype = "Jet";
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
   analysis.addTree<TriggerObject> ("hltL1sL1DoubleJetC100","MssmHbb/Events/selectedPatTrigger/hltL1sL1DoubleJetC100");
   analysis.addTree<TriggerObject> ("hltDoubleJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoubleJetsC100");
   analysis.addTree<TriggerObject> ("hltDoublePFJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoublePFJetsC100");
   analysis.addTree<TriggerObject> ("hltDoubleBTagCSV0p9","MssmHbb/Events/selectedPatTrigger/hltDoubleBTagCSV0p9");
   analysis.addTree<Muon>("Muons","MssmHbb/Events/slimmedMuons");
   analysis.addTree<MET> ("METs","MssmHbb/Events/slimmedMETsPuppi"); 
//    analysis.addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices"); 
   
   
   std::vector<std::string> jetTriggerObjects;
   jetTriggerObjects.push_back("hltL1sL1DoubleJetC100");
   jetTriggerObjects.push_back("hltDoubleJetsC100");
   jetTriggerObjects.push_back("hltDoubleBTagCSV0p9");
   jetTriggerObjects.push_back("hltDoublePFJetsC100");
   
   if(!analysis.isMC()) analysis.processJsonFile("goodJson.txt");
   
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
//   for ( int i = 0 ; i < analysis.size() ; ++i )
   for ( int i = 0 ; i < 10 ; ++i )
   {
      std::cout << "++++++    EVENT  " << i << std::endl;
      analysis.event(i);
      if (!analysis.isMC() && !analysis.selectJson()) continue; // To use only goodJSonFiles
      
      analysis.match<Jet,TriggerObject>("Jets",jetTriggerObjects,0.5);
      
      // Jets
      auto offlineJets         = analysis.collection<Jet>("Jets");
      int jetTriggerMatching = 0;
      int btagTriggerMatching = 0;
     for ( int j = 0 ; j < offlineJets->size() ; ++j )
      {
         Jet jet = offlineJets->at(j);
         std::cout << "Jet " << j << "   pt= " << jet.pt() << "  eta= " << jet.eta() << "   phi= " << jet.phi() << std::endl;
         
         // jet trigger matching
         if ( j < 2 &&
              jet.matched("hltL1sL1DoubleJetC100") &&
              jet.matched("hltDoubleJetsC100")     &&
              jet.matched("hltDoublePFJetsC100") )
            ++jetTriggerMatching;
         
         if ( j < 3 &&
              jet.matched("hltDoubleBTagCSV0p9") )
            ++btagTriggerMatching;
         
      }
      if ( jetTriggerMatching >= 2 ) std::cout << "Two leading jets have matching" << std::endl;
      if ( btagTriggerMatching >= 2 ) std::cout << "Two online btagged jets have matching" << std::endl;
      
      
      // MUONS
      auto offlineMuons         = analysis.collection<Muon>("Muons");
      for ( int m = 0 ; m < offlineMuons->size() ; ++m )
      {
         Muon muon = offlineMuons->at(m);
         std::cout << "Muon " << m << "   pt= " << muon.pt() << "  eta= " << muon.eta() << "   phi= " << muon.phi() << std::endl;
      }
       
      // MET
      auto offlineMETs         = analysis.collection<MET>("METs");
      if ( offlineMuons->size() > 0 )
      {
         MET met = offlineMETs->at(0);
         std::cout << "MET     px= " << met.px() << "  py= " << met.py() << "   phi= " << met.phi() << std::endl;
      }
       
      std::cout << std::endl;
   }
   
//    // cross sections
//    analysis.crossSections("MssmHbb/Metadata/CrossSections");
//    analysis.listCrossSections();
//    
//    //generator filter
//    analysis.generatorFilter("MssmHbb/Metadata/GeneratorFilter");
//    analysis.listGeneratorFilter();
   
      
//    
}

