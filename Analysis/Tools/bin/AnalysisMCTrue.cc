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
   analysis.addTree<GenParticle> ("GenParticles","MssmHbb/Events/prunedGenParticles");
   analysis.addTree<GenJet> ("GenJets","MssmHbb/Events/slimmedGenJets");

   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevents = analysis.size();
   nevents = 5;
   for ( int i = 0 ; i < nevents ; ++i )
   {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i;
      std::cout << std::endl;
      
      // Generated Particles
      auto genParticles = analysis.collection<GenParticle>("GenParticles");
      for ( int j = 0 ; j < genParticles->size() ; ++j )
      {
         GenParticle gp = genParticles->at(j);
         std::cout << "    Generated Particle #" << j << ": ";
         std::cout << "pT  = "     << gp.pt()      << ", ";
         std::cout << "eta = "     << gp.eta()     << ", ";
         std::cout << "phi = "     << gp.phi()     << ", ";
         std::cout << "pdgId = " << gp.pdgId() << ", ";
         std::cout << "status = " << gp.status() << ", ";
         if ( fabs(gp.pdgId()) == 5 ) 
         {
            std::cout << "is higgs daughter = "    << gp.higgsDaughter()    << std::endl;
         }
         else
         {
            std::cout << std::endl;
         }
      }
      
      // Generated jets
      auto genJets = analysis.collection<GenJet>("GenJets");
      for ( int j = 0 ; j < genJets->size() ; ++j )
      {
         GenJet gjet = genJets->at(j);
         std::cout << "    Generated Jet #" << j << ": ";
         std::cout << "pT  = "     << gjet.pt()      << ", ";
         std::cout << "eta = "     << gjet.eta()     << ", ";
         std::cout << "phi = "     << gjet.phi()     << ", " << std::endl;
      }
      
      
   }
   
//    
}

