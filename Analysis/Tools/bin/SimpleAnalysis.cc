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
   
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
   analysis.addTree<TriggerObject> ("hltDoubleJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoubleJetsC100");
    analysis.addTree<TriggerObject> ("hltDoublePFJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoublePFJetsC100");
//    analysis.addTree<Muon>("Muons","MssmHbb/Events/slimmedMuons");
//    analysis.addTree<MET> ("METs","MssmHbb/Events/slimmedMETsPuppi"); 
//    analysis.addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices"); 
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
//   for ( int i = 0 ; i < analysis.size() ; ++i )
   for ( int i = 0 ; i < 1 ; ++i )
   {
      analysis.event(i);
      Collection<Jet> jets = analysis.collection<Jet>("Jets");
      Collection<TriggerObject> hltDoubleJetsC100 = analysis.collection<TriggerObject>("hltDoubleJetsC100");
      Collection<TriggerObject> hltDoublePFJetsC100 = analysis.collection<TriggerObject>("hltDoublePFJetsC100");
      
      jets.matchTo(hltDoubleJetsC100);
      jets.matchTo(hltDoublePFJetsC100);
      
//       Collection<Vertex> vtxs = analysis.collection<Vertex>("Vertices");
      for ( int j = 0 ; j < jets.size() ; ++j )
      {
         Jet jet = jets.at(j);
//          jet.matchTo(hltDoubleJetsC100.vectorCandidates(),"hltDoubleJetsC100");
//          jet.matchTo(hltDoublePFJetsC100.vectorCandidates(),"hltDoublePFJetsC100");
         const Candidate * to_calojet = jet.matched("hltDoubleJetsC100");
         const Candidate * to_pfjet = jet.matched("hltDoublePFJetsC100");
         std::cout << "Jet " << j << "   " << jet.eta() << "   " << jet.phi() << "   " << std::endl;
         if ( to_calojet )
          {
             std::cout << "SimpleAnalysis  TriggerObject CaloJet: " << to_calojet -> eta() << ", " << to_calojet -> phi() << "   " << to_calojet << std::endl;
          }
          if ( to_pfjet )
          {
             std::cout << "SimpleAnalysis  TriggerObject PFJet:   " << to_pfjet -> eta() << ", " << to_pfjet -> phi() << "   " << to_pfjet << std::endl;
          }
      }
      std::cout << "==============" << std::endl;
      
//       jets.matchTo(hltDoublePFJetsC100);
//       for ( int j = 0 ; j < jets.size() ; ++j )
//       {
//          Jet jet = jets.at(j);
// //          jet.matchTo(hltDoubleJetsC100.vectorCandidates(),"hltDoubleJetsC100");
// //          jet.matchTo(hltDoublePFJetsC100.vectorCandidates(),"hltDoublePFJetsC100");
//          const Candidate * to_calojet = jet.matched("hltDoubleJetsC100");
//          const Candidate * to_pfjet = jet.matched("hltDoublePFJetsC100");
//          std::cout << "Jet " << j << "   " << jet.eta() << "   " << jet.phi() << "   " << std::endl;
//          if ( to_calojet )
//           {
//              std::cout << "SimpleAnalysis  TriggerObject CaloJet: " << to_calojet -> eta() << ", " << to_calojet -> phi() << "   " << to_calojet << std::endl;
//           }
//           if ( to_pfjet )
//           {
//              std::cout << "SimpleAnalysis  TriggerObject PFJet:   " << to_pfjet -> eta() << ", " << to_pfjet -> phi() << "   " << to_pfjet << std::endl;
//           }
//       }
       
//       Collection<Muon> muons = analysis.collection<Muon>("Muons");
//       std::cout << "muons  " << muons.size() << std::endl;
//       for ( int i = 0 ; i < muons.size() ; ++i )
//       {
//          Muon muon = muons.at(i);
//          std::cout << muon.pt() << "  " << muon.eta() << "  " << muon.phi() << "  "  << muon.q() << std::endl;
//       }
//       
//       Collection<MET> mets = analysis.collection<MET>("METs");
//       std::cout << "Analysis  " << mets.size() << std::endl;
//       for ( int i = 0 ; i < mets.size() ; ++i )
//       {
//          MET met = mets.at(i);
//          TVector3 p3 = met.p3();
//          std::cout << p3.Px() << "  " << p3.Py() << "  " << p3.Phi() << "  "  << p3.Pt() << std::endl;
//       }
//       std::cout << "Analysis  ====== " << mets.size() << std::endl;
//      std::cout << "=====" << std::endl;
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
