#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/MssmHbb/interface/MssmHbb.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;


// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   MssmHbb analysis(inputList);
   
   std::string atype = "Jet";
   analysis.addTree<Jet>    ("Jets"    , "MssmHbb/Events/slimmedJetsPuppi"              );
   analysis.addTree<Muon>   ("Muons"   , "MssmHbb/Events/slimmedMuons"                  );
   analysis.addTree<MET>    ("METs"    , "MssmHbb/Events/slimmedMETsPuppi"              ); 
   analysis.addTree<Vertex> ("Vertices", "MssmHbb/Events/offlineSlimmedPrimaryVertices" ); 
   analysis.addTree<GenParticle> ("GenParticles", "MssmHbb/Events/prunedGenParticles"   ); 
   analysis.addTree<Candidate>   ("MuonCands"   , "MssmHbb/Events/slimmedMuons"                  );
   
   TH1F * h_njets_closeToLeading = new TH1F("h_njets_closeToLeading","",10,0,10);
   TH1F * h_ptjet_closeToLeading = new TH1F("h_ptjet_closeToLeading","",200,0,1000);
   
   TH1F * h_m12  = new TH1F("h_m12","",300,0,1500);
   TH1F * h_dR12  = new TH1F("h_dR12","",10,0,10);
   TH1F * h_dR13  = new TH1F("h_dR13","",10,0,10);
   TH1F * h_dR23  = new TH1F("h_dR23","",10,0,10);
   TH1F * h_m12X = new TH1F("h_m12X","",300,0,1500);
   
   TH1F * h_njets = new TH1F("h_njets","",10,0,10);
   
   TH1F * h_m12_partons  = new TH1F("h_m12_partons","",300,0,1500);
   TH1F * h_dR12_partons  = new TH1F("h_dR12_partons","",10,0,10);
   
   analysis.defaultCollection<GenParticle>("GenParticles");
   if ( !analysis.defaultCollection<GenParticle>().empty() )
      std::cout << "oioi  " <<  analysis.defaultCollection<GenParticle>() << std::endl;
   
   // Analysis of events
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // Muon Candidate
      auto muonCands = analysis.collection<Candidate>("MuonCands");
      
      // GenParticles
      auto genParticles = analysis.collection<GenParticle>("GenParticles");
      int ngp = genParticles->size();
      std::vector<GenParticle> gps;
      std::vector<GenParticle> finalStateHard;
      bool removeEvent = false;
      for ( int g = 0; g < ngp ; ++g )
      {
         GenParticle gp = genParticles->at(g);
         
         
         if ( gp.pdgId() == 36 && gp.status() == 62 )
         {
            if ( gp.m() < 1100*0.8 )
            {
               removeEvent = true;
               break;
            }
            else
            {
               finalStateHard.push_back(gp);
            }
         }
         if ( gp.status() == 23 ) finalStateHard.push_back(gp);
         if ( gp.pdgId() == 36 )
         {
//            higgsFound = true;
//            std::cout << "Higgs         : px, py, pz, E, m = " << gp.px() << ", " << gp.py() << ", " << gp.pz() << ", " << gp.e() << ", " << gp.m() << ", " << gp.status() << std::endl;
         }
//          if ( gp.higgsDaughter() && !higgsFound )
//          {
//             std::cout << "Higgs daughter: px, py, pz, E, m = " << gp.px() << ", " << gp.py() << ", " << gp.pz() << ", " << gp.e() << ", " << gp.m() << ", " << gp.status() << std::endl;
//          }
         
         if ( gp.higgsDaughter() && gp.status() == 23 )
         {
            gps.push_back(gp);
            if ( abs(gp.pdgId()) != 5 ) std::cout << "Not a b quark from Higgs" << std::endl;
         }
         if ( gps.size() == 2 ) break;
      }
//      std::cout << "-----" << std::endl;
      if ( removeEvent ) continue;
      
      auto finalStateGen = analysis.addCollection<GenParticle>(finalStateHard,"FinalStateHard");
      if ( finalStateGen->size() > 6 )
      std::cout << finalStateGen->size() << std::endl;
      
      if ( gps.size() > 2 )
      {
         std::cout << "More than 2 daughters!? " << gps.size() << std::endl;
         for ( auto & gp : gps )
         {
            std::cout << gp.status() << std::endl;
         }
         std::cout << "====" << std::endl;
      }
      if ( gps.size() < 2 )
      {
//         std::cout << "Will skip the event, for the Higgs has not 2 daughters" << std::endl;
         continue;

      }
      
//      if ( higgsFound ) continue;
//      std::cout << " ---------------" << std::endl;
      TLorentzVector p12_partons;
      
//      if ( gps[0].pt() < 200. || fabs(gps[0].eta()) > 2.2 ) continue;
//      if ( gps[1].pt() < 200. || fabs(gps[1].eta()) > 2.2 ) continue;  // BE CAREFUL it will bias the reco analysis below
      
      p12_partons = gps[0].p4()+gps[1].p4();
//      if ( p12_partons.M() < 800 || p12_partons.M() > 1400 ) continue;
      
      
      if ( gps[0].pt() > 200. && fabs(gps[0].eta()) < 2.2 && gps[1].pt() > 200. && fabs(gps[1].eta()) < 2.2)
      h_m12_partons -> Fill(p12_partons.M());
      h_dR12_partons -> Fill(gps[0].p4().DeltaR(gps[1].p4()));
      
      analysis.match<Jet,Muon>("Jets","Muons");
   
      // Jets
      auto offlineJets = analysis.collection<Jet>("Jets");
      int njets = offlineJets->size();
      
      if ( njets < 3 ) continue;
      
      Jet jet1 = offlineJets->at(0);
      Jet jet2 = offlineJets->at(1);
      Jet jet3 = offlineJets->at(2);
      
      if ( jet1.pt() < 200. || fabs(jet1.eta()) > 2.2 ) continue;
      if ( jet2.pt() < 200. || fabs(jet2.eta()) > 2.2 ) continue;
      if ( jet3.pt() < 30.  || fabs(jet3.eta()) > 2.2 ) continue;
      
      if ( jet1.p4().DeltaR(jet2.p4()) < 1. ) continue;
      if ( jet1.p4().DeltaR(jet3.p4()) < 1. ) continue;
      if ( jet2.p4().DeltaR(jet3.p4()) < 1. ) continue;
      
      int njets_closeToLeading = 0;
      int index_closeToLeading = -1;
      for ( int j = 3 ; j < njets; ++j )
      {
         Jet jetX = offlineJets->at(j);
         double deltaR1X = jet1.p4().DeltaR(jetX.p4());
         if ( deltaR1X < 1.1 )
         {
            ++njets_closeToLeading;
            h_ptjet_closeToLeading -> Fill(jetX.pt());
            index_closeToLeading = j;
            break;
         }
      }
      int index_closeToSubLeading = -1;
      for ( int j = 3 ; j < njets; ++j )
      {
         if ( j == index_closeToLeading ) continue;
         Jet jetX = offlineJets->at(j);
         double deltaR2X = jet2.p4().DeltaR(jetX.p4());
         if ( deltaR2X < 1.1 )
         {
            index_closeToSubLeading = j;
            break;
         }
      }
      
      h_njets_closeToLeading -> Fill(njets_closeToLeading);
           
      TLorentzVector p12 = jet1.p4()+jet2.p4();
      double dR12 = jet1.p4().DeltaR(jet2.p4());
      double dR13 = jet1.p4().DeltaR(jet3.p4());
      double dR23 = jet2.p4().DeltaR(jet3.p4());
      float m12 = p12.M();
      if ( index_closeToLeading > 0 )
      {
         Jet jetX = offlineJets->at(index_closeToLeading);
         if ( index_closeToSubLeading < 0 )
            p12 = jet1.p4()+jet2.p4()+jetX.p4();
         else
         {
            Jet jetY = offlineJets->at(index_closeToSubLeading);
            p12 = jet1.p4()+jet2.p4()+jetX.p4()+jetY.p4();
         }
      }
      
      float m12X = p12.M();
      
      h_njets -> Fill(njets);
      h_m12 -> Fill(m12);
      h_dR12 -> Fill(dR12);
      h_dR13 -> Fill(dR13);
      h_dR23 -> Fill(dR23);
      h_m12X -> Fill(m12X); 
      
   }
   
   TFile out("output_histograms.root","recreate");
   h_m12->Write();
   h_dR12->Write();
   h_dR13->Write();
   h_dR23->Write();
   h_m12X->Write();
   h_njets->Write();
   h_njets_closeToLeading->Write();
   h_ptjet_closeToLeading->Write();
   h_m12_partons->Write();
   h_dR12_partons->Write();
   out.Close();
   
//    // cross sections
//    analysis.crossSections("MssmHbb/Metadata/CrossSections");
//    analysis.listCrossSections();
//    
//    //generator filter
//    analysis.generatorFilter("MssmHbb/Metadata/GeneratorFilter");
//    analysis.listGeneratorFilter();
//    
      
//    
}

