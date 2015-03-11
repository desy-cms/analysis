#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <bitset>
#include <memory>

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

float  pi;

int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   // Histograms
   
   // TTree chains and friendship?
   TChain * chainEvent   = new TChain("TriggerStudies/EventInfo");
   TChain * chainL1Jets  = new TChain("TriggerStudies/hltL1extraParticles_MergedJets");
   
   // Friendship
   chainEvent -> AddFriend(chainL1Jets);
   
   // Input files
   std::string inputList = "rootFileList.txt";
   cout << "List of input files: " << inputList << endl;
   TFileCollection fc("dum","",inputList.c_str());
   
   // Read from file
   chainEvent  -> AddFileInfoList((TCollection*) fc.GetList());
   chainL1Jets -> AddFileInfoList((TCollection*) fc.GetList());
   
   // Branches
   // Event Info
   chainEvent->SetBranchAddress( "run"         , &run_ );
   chainEvent->SetBranchAddress( "lumisection" , &lumi_ );
   chainEvent->SetBranchAddress( "event"       , &event_ );
   // L1 Jets
   chainL1Jets->SetBranchAddress( "n"   , &jetN_  [0]);
   chainL1Jets->SetBranchAddress( "pt"  ,  jetPt_ [0]);
   chainL1Jets->SetBranchAddress( "eta" ,  jetEta_[0]);
   chainL1Jets->SetBranchAddress( "phi" ,  jetPhi_[0]);
   chainL1Jets->SetBranchAddress( "e"   ,  jetE_  [0]);
   chainL1Jets->SetBranchAddress( "et"  ,  jetEt_ [0]);
   
   
//    
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   std::cout << nentries << std::endl;
   
   int c_l1pteta = 0;
   int c_l1deta = 0;
   int c_l1dr = 0;
   int c_l1detadr = 0;
   for ( int i = 0 ; i < nentries ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // selected jets
      std::vector<TLorentzVector> jets;
      jets.clear();
      
      // event fired
      bool l1pteta = false;
      bool l1deta = false;
      bool l1dr = false;
      
      float ptcut = 84;
      float etacut = 2.172;
      int ncut = 2;
      
      for ( int j = 0 ; j < jetN_[0] ; ++j )
      {
         if ( jetPt_[0][j] >= ptcut && fabs(jetEta_[0][j]) <= etacut  )
         {
            TLorentzVector jet;
            jet.SetPtEtaPhiE(jetPt_[0][j], jetEta_[0][j], jetPhi_[0][j], jetE_[0][j]); 
            jets.push_back(jet);
         }
      }
      
      // min number of jets required
      l1pteta = int(jets.size()) >= ncut;
      
      if (!l1pteta) continue;
      
      int dRCut[2] = {1,1};
      int dEtaBinCut = 4;
      
      // delta_eta and delta_R
      for ( size_t j1 = 0 ; j1 < jets.size()-1 ; ++j1 )
      {
         TLorentzVector jet1 = jets[j1];
         int j1EtaBin = GetEtaBin(jet1.Eta());
         int j1PhiBin = GetPhiBin(jet1.Phi());
         for ( size_t j2 = j1+1 ; j2 < jets.size() ; ++j2 )
         {
            TLorentzVector jet2 = jets[j2];
            int j2EtaBin = GetEtaBin(jet2.Eta());
            int j2PhiBin = GetPhiBin(jet2.Phi());
            // delta_Eta cut
            if ( abs(j1EtaBin - j2EtaBin) <= dEtaBinCut ) l1deta = true;
            // delta_R cut
            if (( abs(j1EtaBin - j2EtaBin) > dRCut[0] ) &&
               ( abs(j1PhiBin - j2PhiBin) > dRCut[1]  && abs(j1PhiBin - j2PhiBin) < 18-dRCut[1] )) l1dr = true;
         }
      }
      
      if ( l1pteta )
      {
         ++c_l1pteta;
         if ( l1deta ) ++c_l1deta;
         if ( l1dr ) ++c_l1dr;
         if ( l1deta && l1dr ) ++c_l1detadr;
      }
      
         
   }
   
   std::cout << "Number of events fired pt-eta cuts: " << c_l1pteta <<std::endl;
   std::cout << "Number of events fired pt-eta cuts and delta_eta cut: " << c_l1deta <<std::endl;
   std::cout << "Number of events fired pt-eta cuts and delta_r cut: " << c_l1dr <<std::endl;
   std::cout << "Number of events fired pt-eta cuts and delta_eta and delta_r cuts: " << c_l1detadr <<std::endl;
   
    
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

