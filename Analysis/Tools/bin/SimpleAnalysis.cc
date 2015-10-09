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
   analysis.addTree<Jet> ("Jets2","MssmHbb/Events/slimmedJets");
   analysis.addTree<Muon>("Muons","MssmHbb/Events/slimmedMuons");
   analysis.addTree<MET> ("METs","MssmHbb/Events/slimmedMETsPuppi");
   analysis.addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices");
   analysis.addTree<Trigger> ("HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v1","MssmHbb/Events/selectedPatTrigger/hltDoubleBTagCSV0p9")

   // Analysis of events
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      Collection<Jet> jets = analysis.collection<Jet>("Jets");
      Collection<Jet> jets2 = analysis.collection<Jet>("Jets2");
      Collection<Vertex> vtxs = analysis.collection<Vertex>("Vertices");
      Collection<Trigger> triggers = analysis.collection<Trigger>("HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v1");
      std::cout << "jets  " << jets.size() << "    "  << jets2.size() << std::endl;
      for ( int i = 0 ; i < jets.size() ; ++i )
      {
         //Jet jet = jets.at(i);
         //jet.matchTo(vtxs.vectorCandidates(),"Vertices");
         Trigger trigger = triggers.at(i);
         std::cout << "Trigger n" <<trigger.size()<<" "<<trigger.pt()<< std::endl;
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

   // cross sections
   analysis.crossSections("MssmHbb/Metadata/CrossSections");
   analysis.listCrossSections();

   //generator filter
   analysis.generatorFilter("MssmHbb/Metadata/GeneratorFilter");
   analysis.listGeneratorFilter();


//
}
