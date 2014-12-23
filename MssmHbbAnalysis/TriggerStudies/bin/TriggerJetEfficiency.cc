#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <bitset>

#include "TFile.h" 
#include "TH1.h" 
#include "TH2.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"
#include "TRFIOFile.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TFileCollection.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TF1.h"

#include "NtupleVariables.h"

using namespace std;

int GetEtaBin( const float & );
int GetPhiBin( const float & );

void SetParameters();

int ncut[4];
float ptcut[4];
float etacut[4];

int    nEvents    ;
string run        ;
string steer      ;
float  pi;

int main(int argc, char * argv[])
{
   SetParameters();
   
   if ( argc == 2 )
   {
      steer = atof(argv[1]);
   }
   
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   // Histograms
   TH1F * h_JetPtRef = new TH1F("h_JetPtRef","", 48, 0, 240.);
   TH1F * h_JetPtNom = new TH1F("h_JetPtNom","", 48, 0, 240.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerObjects/EventInfo");
   TChain * chainJets[4];
   chainJets[0]  = new TChain("TriggerObjects/hltL1extraParticles_MergedJets");
   chainJets[1]  = new TChain("TriggerObjects/hltAK4CaloJetsCorrectedIDPassed");
   chainJets[2]  = new TChain("TriggerObjects/hltAK4PFJets");
   chainJets[3]  = new TChain("TriggerObjects/hltAK4PFJetsCorrected");
   
   // Input files
   std::string inputList = "rootFileList.txt";
   cout << "List of input files: " << inputList << endl;
   TFileCollection fc("dum","",inputList.c_str());
   chainEvent->AddFileInfoList((TCollection*) fc.GetList());
   // Branches
   // Event Info
   chainEvent->SetBranchAddress( "run"         , &run_ );
   chainEvent->SetBranchAddress( "lumisection" , &lumi_ );
   chainEvent->SetBranchAddress( "event"       , &event_ );
   
   for ( int l = 0 ; l < 4 ; ++l )
   {
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chainJets[l]);
      chainJets[l]->AddFileInfoList((TCollection*) fc.GetList());
      chainJets[l]->SetBranchAddress( "n"   , &jetN_  [l]);
      chainJets[l]->SetBranchAddress( "pt"  ,  jetPt_ [l]);
      chainJets[l]->SetBranchAddress( "eta" ,  jetEta_[l]);
      chainJets[l]->SetBranchAddress( "phi" ,  jetPhi_[l]);
      chainJets[l]->SetBranchAddress( "e"   ,  jetE_  [l]);
   }
   
   // The highest level, ie the level with respect to which the efficiency will be evaluated
   int highestLevel = -1;
   for ( int i = 0 ; i < 4 ; ++i )
      if ( ncut[i] > 0 ) highestLevel = i;
   
   
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   if ( nEvents < 0 )  nEvents = nentries;
   
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      std::bitset<4> fired;
      fired.set();  // set all bits to true
      
      // The trigger objects after online selection, ie all objects that pass the selection
      std::vector<TLorentzVector> jets[4];
      
      // trigger emulators
      for ( int l = 0 ; l < 4 ; ++l )
      {
         jets[l].clear();
         if ( ncut[l] > 0 )      // same as no trigger selection, ie trigger always fired at this level
         {
            for ( int j = 0 ; j < jetN_[l] ; ++j )
            {
               if ( jetPt_[l][j] >= ptcut[l] && fabs(jetEta_[l][j]) <= etacut[l]  )
               {
                  TLorentzVector jet;
                  jet.SetPtEtaPhiE(jetPt_[l][j], jetEta_[l][j], jetPhi_[l][j], jetE_[l][j]); 
                  jets[l].push_back(jet);
               }
            }
            fired[l] = int(jets[l].size()) >= ncut[l];
         }      
      }
      
      // The efficiency is wrt highestLevel, therefore the event must pass the highestLevel selection
      if ( ! fired[highestLevel] ) continue;
      
      // check if the chain fired
      bool chainFired = true;
      for ( int l = 0 ; l < 4 ; ++l )
         chainFired = chainFired && fired[l];
      
      // Matching between the objects at different levels; the top level only the first object will be probed
      TLorentzVector probejet = jets[highestLevel].at(0);
      
      // Matching non L1 objects
      std::bitset<4> match;
      match.set();
      float dRcut = 1.0;
      for ( int l = highestLevel-1; l > 0 ; --l )
      {
         if ( ncut[l] <= 0 ) continue;  // the level l is not triggering - no matching required
         
         // Match first leading non L1 jets, then the matched with lower level jets
         for ( int j = 0 ; j < int(jets[l].size()) ; ++j )
         {
            float deltaR = probejet.DeltaR(jets[l].at(j));
            if ( deltaR < dRcut ) // Not necessarily the closest but a match
            {
               match[l] = true;
               probejet = jets[l].at(j);
               break; // go to the next level with the matched jet as the probe jet
            }
            else
            {
               match[l] = false;
            }
         }
      }
      
      // Matching last probejet with L1 objects 
      int etaBinCut = 2;
      int phiBinCut = 3;
      int etaBin = GetEtaBin(probejet.Eta());
      int phiBin = GetPhiBin(probejet.Phi());
      for ( int j = 0 ; j < int(jets[0].size()) ; ++j )
      {
         int l1EtaBin = GetEtaBin(jets[0].at(j).Eta());
         int l1PhiBin = GetPhiBin(jets[0].at(j).Phi());
         bool etaMatch = false;
         bool phiMatch = false;
         if ( abs(etaBin - l1EtaBin) <= etaBinCut ) etaMatch = true;
         if ( abs(phiBin - l1PhiBin) <= phiBinCut  || abs(phiBin - l1PhiBin) >= 18-phiBinCut ) phiMatch = true;
         if ( etaMatch && phiMatch )
         {
            match[0] = true;
            break;
         }
         else
         {
            match[0] = false;
         }
      }
      
      
     // Fill histograms
     if ( chainFired && match.all() )
     {
        h_JetPtNom -> Fill(jets[highestLevel].at(0).Pt());
     }
     
     h_JetPtRef -> Fill(jets[highestLevel].at(0).Pt());

     
   }
   
   TGraphAsymmErrors * g_JetPtEff = new TGraphAsymmErrors( h_JetPtNom, h_JetPtRef, "cl=0.683 b(1,1) mode" );
   TString outFilename = "histograms";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_JetPtRef      -> Write();
   h_JetPtNom      -> Write();
   g_JetPtEff      -> Write();
   outFile -> Close();
    
    
//    
}


// #############################################################################
int GetEtaBin (const float & eta )
{
   int nBins = 22;
   float etaBin[23] = { -5., -4.5, -4., -3.5, -3., -2.172, -1.74, -1.392, -1.044, -0.696, -0.348, 0. , 0.348, 0.696, 1.044, 1.392, 1.74, 2.172, 3., 3.5, 4., 4.5, 5.  }; 
   
   int bin = -1; 
   for ( int i = 0 ; i < nBins ; ++i )
   {
      if ( eta > etaBin[i] && eta < etaBin[i+1] )
      {
         bin = i;
         break;
      }
   }
      
   return bin;
}

// #############################################################################
int GetPhiBin (const float & Phi )
{
   float pi = acos(-1.);
   float phi = Phi;
   
   if ( phi < 0 ) phi += 2.*pi;
   phi = phi*180./pi;
   
   int nBins = 18;
   float phiBin[19] = { 350, 10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350  }; 
   
   int bin = -1; 
   for ( int i = 0 ; i < nBins ; ++i )
   {
      if ( phi > phiBin[i] && phi < phiBin[i+1] )
      {
         bin = i;
      }
      else if ( phi > 350 || phi <= 10 )
      {
         bin = 0;
      }
   }
     
   return bin;
}

// #############################################################################
void SetParameters()
{
   steer = "steer.txt";
      
   // L1 jet
   ncut[0] = 1;
   ptcut[0] = 84.;
   etacut[0] = 5.0;
   // L2 calo jet
   ncut[1] = 1;
   ptcut[1] = 20.;
   etacut[1] = 2.6;
   // L3 PF jets
   ncut[2] = 0;
   ptcut[2] = 84.;
   etacut[2] = 2.6;
   // Offline jets
   ncut[3] = 0;
   ptcut[3] = 20.;
   etacut[3] = 2.6;
   
   
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   
   infile.close();
}
