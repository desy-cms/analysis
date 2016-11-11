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
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");

   
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
      // The higgs (pdgId == 36) is also in such collection.
      // If higgs status=22, then hard-process intermediate (mass preserved)
      // If higgs status=62, then  particle with primordial kT included
      // The higgs daughters have initially status = 23, but they will undergo several steps 
      // until status=71, i.e. before hadronisation, which would be the best to use.
      // Thereofre, one can then try to follow the cascade down to the last b quarks
      // using delta_R matching. Maybe maybe some delta_pt/pt... but that's not done here.
      auto genParticles = analysis.collection<GenParticle>("GenParticles");
      GenParticle bquark;
      GenParticle bbarquark;
      for ( int j = 0 ; j < genParticles->size() ; ++j )
      {
         GenParticle gp = genParticles->at(j);
         if ( gp.pdgId() == 36 && gp.status() > 60 )
         {
            std::cout << "I am the higgs! "           << " : ";
            std::cout << "pT  = "     << gp.pt()      << ", ";
            std::cout << "eta = "     << gp.eta()     << ", ";
            std::cout << "phi = "     << gp.phi()     << ", ";
            std::cout << "pdgId = "   << gp.pdgId()   << ", ";
            std::cout << "status = "  << gp.status()  << std::endl;
         }
         if ( fabs(gp.pdgId()) == 5 && gp.higgsDaughter() ) 
         {
            if ( gp.pdgId() > 0 )  bquark = gp;
            if ( gp.pdgId() < 0 )  bbarquark = gp;
         }
      }

      std::cout << "   I am a higgs b quark daughter! " << " : ";
      std::cout << "pT  = "     << bquark.pt()      << ", ";
      std::cout << "eta = "     << bquark.eta()     << ", ";
      std::cout << "phi = "     << bquark.phi()     << ", ";
      std::cout << "pdgId = "   << bquark.pdgId()   << ", ";
      std::cout << "status = "  << bquark.status()  << std::endl;
      
      std::cout << "   I am a higgs bbar quark daughter! " << " : ";
      std::cout << "pT  = "     << bbarquark.pt()      << ", ";
      std::cout << "eta = "     << bbarquark.eta()     << ", ";
      std::cout << "phi = "     << bbarquark.phi()     << ", ";
      std::cout << "pdgId = "   << bbarquark.pdgId()   << ", ";
      std::cout << "status = "  << bbarquark.status()  << std::endl;
      
// ====================================================================      
      
      // Generated jets
      // Let's match with the Higgs daughters
      auto genJets = analysis.collection<GenJet>("GenJets");
      GenJet bgenjet;
      GenJet bbargenjet;
      int nb = 0;
      int nbbar = 0;
      for ( int j = 0 ; j < genJets->size() ; ++j )
      {
         GenJet gjet = genJets->at(j);
         if ( gjet.deltaR(bquark) < 0.3 && nb == 0)
         {
            bgenjet = gjet ;
            ++nb;
         }
         if ( gjet.deltaR(bbarquark) < 0.3 && nbbar == 0 )
         {
            bbargenjet = gjet ;
            std::cout << "       I am a gen jet matched to a higgs daughter bbar! " << " : ";
            std::cout << "pT  = "     << gjet.pt()      << ", ";
            std::cout << "eta = "     << gjet.eta()     << ", ";
            std::cout << "phi = "     << gjet.phi()     << ", " << std::endl;
            ++nbbar;
         }
         if ( nb > 0 && nbbar > 0 ) break;
      }
      
      std::cout << "       I am a gen jet matched to a higgs daughter b quark! " << " : ";
      std::cout << "pT  = "     << bgenjet.pt()      << ", ";
      std::cout << "eta = "     << bgenjet.eta()     << ", ";
      std::cout << "phi = "     << bgenjet.phi()     << ", " << std::endl;
      
      std::cout << "       I am a gen jet matched to a higgs daughter bbar quark! " << " : ";
      std::cout << "pT  = "     << bbargenjet.pt()      << ", ";
      std::cout << "eta = "     << bbargenjet.eta()     << ", ";
      std::cout << "phi = "     << bbargenjet.phi()     << ", " << std::endl;
      
// ====================================================================      
      
      
      nb = 0;
      nbbar = 0;
      // reconstructed jets
      // will now try to match the gen jets that are daughter of the higgs to the reconstructed jets
      auto jets = analysis.collection<Jet>("Jets");
      Jet bjet;
      Jet bbarjet;
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         if ( bgenjet.deltaR(jet) < 0.3 && nb == 0 )
         {
            bjet = jet ;
            ++nb;
         }
         if ( bbargenjet.deltaR(jet) < 0.3 && nbbar == 0 )
         {
            bbarjet = jet ;
            ++nbbar;
         }
         if ( nb > 0 && nbbar > 0 ) break;
         
      }
      std::cout << "            I am a reco jet matched to a higgs daughter b quark! " << " : ";
      std::cout << "pT  = "     << bjet.pt()      << ", ";
      std::cout << "eta = "     << bjet.eta()     << ", ";
      std::cout << "phi = "     << bjet.phi()     << ", " << std::endl;
      
      std::cout << "            I am a reco jet matched to a higgs daughter bbar quark! " << " : ";
      std::cout << "pT  = "     << bbarjet.pt()      << ", ";
      std::cout << "eta = "     << bbarjet.eta()     << ", ";
      std::cout << "phi = "     << bbarjet.phi()     << ", " << std::endl;
   }
   
//    
}

