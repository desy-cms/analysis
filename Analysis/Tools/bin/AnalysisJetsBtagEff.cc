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
   
   // Physics Objects Collections
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // GenParticles (when needed)
   analysis.addTree<GenParticle> ("GenParticles","MssmHbb/Events/prunedGenParticles");

   // Use external btagging efficiencies
   analysis.addBtagEfficiencies("/afs/desy.de/user/w/walsh/cms/analysis/cmssw/dev/CMSSW_7_6_5/src/Analysis/Objects/bin/BtagEfficiencies_btag_csvivf_0.935.root");
   analysis.btagEfficienciesAlgo("btag_csvivf");  // only needed above a certain commit where this information should be available in the title of the root file above
   analysis.btagEfficienciesFlavour("Extended");  // only needed above a certain commit where this information should be available in the title of the root file above
   
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i << std::endl;
      
      // GenParticles
      auto particles = analysis.collection<GenParticle>("GenParticles");
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      jets->btagAlgo("btag_csvivf");
      
      // Needed to use the extended flavour
      jets->associatePartons(particles,0.4,5);
      
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         std::cout << "    Jet #" << j << ": ";
         std::cout << "pT  = "           << jet.pt()      << ", ";
         std::cout << "eta = "           << jet.eta()     << ", ";
         std::cout << "phi = "           << jet.phi()     << ", ";
         std::cout << "id loose = "      << jet.idLoose() << ", ";
         std::cout << "flavour = "       << jet.flavour() << ", ";
         std::cout << "extFlavour = "    << jet.extendedFlavour() << ", ";
         std::cout << "btag = "    << jet.btag()    << std::endl;
         std::cout << "efficiency = "  << analysis.btagEfficiency(jet) << std::endl;
         
      }
      
   }
   
//    
}

