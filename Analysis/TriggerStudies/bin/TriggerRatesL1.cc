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
   TChain * chainEvent   = new TChain("L1TriggerStudies/EventInfo");
   TChain * chainL1Jets  = new TChain("L1TriggerStudies/hltL1extraJetParticles_MergedJets");
   
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
   
//    
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   std::cout << nentries << std::endl;
   
   float l1EtaBin[12] = { 0. , 0.348, 0.696, 1.044, 1.392, 1.74, 2.172, 3., 3.5, 4., 4.5, 5.  }; 

   
   std::vector<float> ptcut;
   std::vector<float> etacut;
   std::vector<int>   detacut;
   
   int ncut = 2;
   
   ptcut.push_back(84.);
   ptcut.push_back(88.);
   ptcut.push_back(92.);
   ptcut.push_back(96.);
   ptcut.push_back(100.);
   
   etacut.push_back(l1EtaBin[7]);
   etacut.push_back(l1EtaBin[6]);
   etacut.push_back(l1EtaBin[5]);
   
   detacut.push_back(6);
   detacut.push_back(5);
   detacut.push_back(4);
   
   std::vector<int> c_l1pteta;
   c_l1pteta.resize(ptcut.size()*etacut.size(),0);
   std::cout << "size = " << c_l1pteta.size() << std::endl;
   std::vector<int> c_l1deta;
   c_l1deta.resize(ptcut.size()*etacut.size()*detacut.size(),0);
   
   int ipteta = 0;
   int iptetadeta = 0;
      
   
   for ( int i = 0 ; i < nentries ; ++i )
   {
//      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      
      // define which pt cut
      for ( size_t ipt = 0; ipt < ptcut.size(); ++ipt )
      {
         // define which eta cut
         for ( size_t ieta = 0; ieta < etacut.size(); ++ieta )
         {
            ipteta = ieta + ipt*etacut.size();
            bool l1pteta = false;
            
            // selected jets
            std::vector<TLorentzVector> jets;
            jets.clear();
            
            for ( int j = 0 ; j < jetN_[0] ; ++j )
            {
               if ( jetPt_[0][j] >= ptcut[ipt] && fabs(jetEta_[0][j]) <= etacut[ieta]  )
               {
                  TLorentzVector jet;
                  jet.SetPtEtaPhiE(jetPt_[0][j], jetEta_[0][j], jetPhi_[0][j], jetE_[0][j]); 
                  jets.push_back(jet);
               }
            }
            // min number of jets required
            l1pteta = int(jets.size()) >= ncut;
            if (!l1pteta) continue;
            
            // count events fired
            ++c_l1pteta[ipteta];
            
            for ( size_t ideta = 0; ideta < detacut.size(); ++ideta)
            {
               iptetadeta = ideta + ipteta*detacut.size();
               int dEtaBinCut = detacut[ideta];
               bool l1deta = false;
            
               // delta_eta and delta_R
               for ( size_t j1 = 0 ; j1 < jets.size()-1 ; ++j1 )
               {
                  TLorentzVector jet1 = jets[j1];
                  int j1EtaBin = GetEtaBin(jet1.Eta());
         //         int j1PhiBin = GetPhiBin(jet1.Phi());
                  for ( size_t j2 = j1+1 ; j2 < jets.size() ; ++j2 )
                  {
                     TLorentzVector jet2 = jets[j2];
                     int j2EtaBin = GetEtaBin(jet2.Eta());
         //            int j2PhiBin = GetPhiBin(jet2.Phi());
                     // delta_Eta cut
                     if ( abs(j1EtaBin - j2EtaBin) <= dEtaBinCut ) l1deta = true;
                  }
               }
//               std::cout << ipteta << "  " << iptetadeta << std::endl;
               if ( l1deta ) ++c_l1deta[iptetadeta];
            } // detacut loop
            
         } // etacut loop
         
      } // ptcut loop
      
   } // events loop
            
   // print for each set of cuts
   // define which pt cut
   for ( size_t ipt = 0; ipt < ptcut.size(); ++ipt )
   {
      std::cout << "pt>= " << ptcut[ipt] << std::endl;
      // define which eta cut
      for ( size_t ieta = 0; ieta < etacut.size(); ++ieta )
      {
         ipteta = ieta + ipt*etacut.size();
         std::cout << "    |eta| <= " << etacut[ieta] << ": " << c_l1pteta[ipteta] <<std::endl;
         for ( size_t ideta = 0; ideta < detacut.size(); ++ideta)
         {
            iptetadeta = ideta + ipteta*detacut.size();
            std::cout << "        dEta <= " << detacut[ideta] << " bins: " << c_l1deta[iptetadeta] <<std::endl;
         }
      }
   }
            
   
    
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

