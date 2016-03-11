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
   analysis.addTree<GenParticle> ("GenParticles","MssmHbb/Events/prunedGenParticles");

   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
//      std::cout << "++++++    ENTRY  " << i;
//      std::cout << std::endl;
      
      analysis.event(i);
      
      // GenParticles
      auto particles = analysis.collection<GenParticle>("GenParticles");
      
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      jets->associatePartons(particles,0.4,1);
      jets->btagAlgo("btag_csvmva");
      
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         if ( jet.pt() < 30 ) continue;
//         std::cout << jet.btag() << "  " << jet.btag("btag_csvivf") << " pt = " <<  jet.pt() << std::endl;
         if ( ( jet.extendedFlavour() == "cc" || jet.extendedFlavour() == "bb" ) && jets->size() > 4 )
         {
//            std::cout << analysis.event() << "  " << analysis.lumiSection() << std::endl;
//            std::cout << "Jet # " << j << " with flavour = " << jet.flavour() << " and extended flavour = " << jet.extendedFlavour() << "  btag = " << jet.btag("btag_csvivf") << ", " << jet.btag("btag_csvmva") << std::endl;
         }
      }
      
   }
   
//    
}

