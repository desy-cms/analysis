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

   // Input files list MonteCarloStudies
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList,"MonteCarloStudies/Events/EventInfo");

   analysis.addTree<Jet> ("Jets","MonteCarloStudies/Events/slimmedJetsPuppi");
   //analysis.addTree<Jet> ("Jets2","MonteCarloStudies/Events/slimmedJets");
   //analysis.addTree<Muon>("Muons","MonteCarloStudies/Events/slimmedMuons");
   //analysis.addTree<MET> ("METs","MonteCarloStudies/Events/slimmedMETsPuppi");
   //analysis.addTree<Vertex> ("Vertices","MonteCarloStudies/Events/offlineSlimmedPrimaryVertices");
   if(!analysis.isMC())
   {
     analysis.addTree<TriggerObject> ("hltDoubleBTagCSV0p9","MonteCarloStudies/Events/selectedPatTrigger/hltDoubleBTagCSV0p9");
     analysis.addTree<TriggerObject> ("hltDoubleJetsC100","MonteCarloStudies/Events/selectedPatTrigger/hltDoubleJetsC100");
     analysis.addTree<TriggerObject> ("hltDoublePFJetsC100","MonteCarloStudies/Events/selectedPatTrigger/hltDoublePFJetsC100");
   }
   analysis.addTriggerResultTree("TriggerResults","MonteCarloStudies/Events/TriggerResults");

   // Analysis of events
   for ( int i = 0 ; i < 1000 ; ++i )
   {
      analysis.event(i);

      if(analysis.HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160()) std::cout<< "WTD = "<< analysis.HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160()<< std::endl;

      Collection<Jet> jets = analysis.collection<Jet>("Jets");

      //Collection<Jet> jets2 = analysis.collection<Jet>("Jets2");
      //Collection<Vertex> vtxs = analysis.collection<Vertex>("Vertices");
      Collection<TriggerObject> triggers = analysis.collection<TriggerObject>("hltDoubleBTagCSV0p9");
      //std::cout << "Size " << triggers.size() << std::endl;
      //std::cout << "jets  " << jets.size() << "    "  << jets2.size() << std::endl;
      for ( int i = 0 ; i < triggers.size() ; ++i )
      {
         //Jet jet = jets.at(i);
         //jet.matchTo(vtxs.vectorCandidates(),"Vertices");
         TriggerObject trigger = triggers.at(i);
         std::cout << "Trigger = " << trigger.eta() << " " << trigger.pt() << " " << trigger.phi() << std::endl;
      }

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

   if(analysis.isMC())
   {
     // cross sections
     analysis.crossSections("MonteCarloStudies/Metadata/CrossSections");
     analysis.listCrossSections();

     //generator filter
     analysis.generatorFilter("MonteCarloStudies/Metadata/GeneratorFilter");
     analysis.listGeneratorFilter();
   }


//
}
