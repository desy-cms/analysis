#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>

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

int    l1JetNCut  ;
float  l1JetPtCut ;
float  l1JetEtaCut;
int    l2JetNCut  ;
float  l2JetPtCut ;
float  l2JetEtaCut;
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
   TChain * chainEvent      = new TChain("TriggerObjects/EventInfo");
   TChain * chainL1Jets     = new TChain("TriggerObjects/hltL1extraParticles_MergedJets");
   TChain * chainL2CaloJets = new TChain("TriggerObjects/hltAK4CaloJetsCorrectedIDPassed");
   
   // Don't forget that the trees must be friends!!!
   chainEvent -> AddFriend(chainL2CaloJets);
   chainEvent -> AddFriend(chainL1Jets);
   
   // Input files
   std::string inputList = "rootFileList.txt";
   cout << "List of input files: " << inputList << endl;
   TFileCollection fc("dum","",inputList.c_str());
   chainEvent->AddFileInfoList((TCollection*) fc.GetList());
   chainL2CaloJets->AddFileInfoList((TCollection*) fc.GetList());
   chainL1Jets->AddFileInfoList((TCollection*) fc.GetList());
   
   // Branches
   // Event Info
   chainEvent->SetBranchAddress( "run"         , &run_ );
   chainEvent->SetBranchAddress( "lumisection" , &lumi_ );
   chainEvent->SetBranchAddress( "event"       , &event_ );
   
   // L1 Jets
   chainL1Jets->SetBranchAddress( "n"   , &l1JetN_  );
   chainL1Jets->SetBranchAddress( "pt"  ,  l1JetPt_  );
   chainL1Jets->SetBranchAddress( "e"   ,  l1JetE_   );
   chainL1Jets->SetBranchAddress( "eta" ,  l1JetEta_ );
   chainL1Jets->SetBranchAddress( "phi" ,  l1JetPhi_ );
   chainL1Jets->SetBranchAddress( "px"  ,  l1JetPx_  );
   chainL1Jets->SetBranchAddress( "py"  ,  l1JetPy_  );
   chainL1Jets->SetBranchAddress( "pz"  ,  l1JetPz_  );
   
   // L2 Jets
   chainL2CaloJets->SetBranchAddress( "n"   , &l2JetN_  );
   chainL2CaloJets->SetBranchAddress( "pt"  ,  l2JetPt_  );
   chainL2CaloJets->SetBranchAddress( "e"   ,  l2JetE_   );
   chainL2CaloJets->SetBranchAddress( "eta" ,  l2JetEta_ );
   chainL2CaloJets->SetBranchAddress( "phi" ,  l2JetPhi_ );
   chainL2CaloJets->SetBranchAddress( "px"  ,  l2JetPx_  );
   chainL2CaloJets->SetBranchAddress( "py"  ,  l2JetPy_  );
   chainL2CaloJets->SetBranchAddress( "pz"  ,  l2JetPz_  );
   
   int nentries = chainEvent->GetEntries();
   if ( nEvents < 0 )  nEvents = nentries;
   
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // L1 emulator
      bool l1Fired = false;
      std::vector<int> iL1;    // index of the L1 trigger objects for matching
      int nL1Obj = 0;
      if ( l1JetPtCut < 0 ) 
      {
         l1Fired = true;
      }
      else
      {
         for ( int j = 0 ; j < l1JetN_ ; ++j )
         {
            if ( l1JetPt_[j] >= l1JetPtCut && fabs(l1JetEta_[j]) <= l1JetEtaCut  )
            {
               iL1.push_back(j);
            }
         }
         nL1Obj = (int)iL1.size();
         if ( nL1Obj >= l1JetNCut ) 
         {
            l1Fired = true;
         }
      }

      // L2 emulator
      bool l2Fired = false;
      std::vector<int> iL2;    // index of the L2 trigger objects for matching
      int nL2Obj = 0;
      if ( l2JetPtCut < 0 ) 
      {
         l2Fired = true;
      }
      else
      {
         for ( int j = 0 ; j < l2JetN_ ; ++j )
         {
            if ( l2JetPt_[j] >= l2JetPtCut && fabs(l2JetEta_[j]) <= l2JetEtaCut  )
            {
               iL2.push_back(j);
            }
         }
         nL2Obj = (int)iL2.size();
         if ( nL2Obj >= l2JetNCut ) 
         {
            l2Fired = true;
         }
      }
      
      if ( ! l2Fired ) continue;
      
      // Emulated trigger chain
      bool fired = false;
      if ( l1Fired ) fired = true;  // L1 only, its efficiency with respect to L2

      
      // Matching jets
      bool isL1L2Matched = false;
      
      if  ( fired )
      {
         // Match L2 with L1
         int etaBinCut = 2;
         int phiBinCut = 3;
         if ( l2JetPtCut > 0. && l1JetPtCut > 0. )
         {
            int l2EtaBin = GetEtaBin(l2JetEta_[0]);
            int l2PhiBin = GetPhiBin(l2JetPhi_[0]);
//             std::cout << "L2 " << l2JetPt_[0] << ", " << l2JetEta_[0] << ", " << l2JetPhi_[0] << std::endl;
//             std::cout << "L2 " << l2EtaBin << ", " << l2PhiBin << std::endl;
            for ( int j1 = 0 ; j1 < nL1Obj ; ++j1 )
            {
               int l1EtaBin = GetEtaBin(l1JetEta_[iL1[j1]]);
               int l1PhiBin = GetPhiBin(l1JetPhi_[iL1[j1]]);
//                std::cout << "    L1 " << l1JetPt_[iL1[j1]] << ", " << l1JetEta_[iL1[j1]] << ", " << l1JetPhi_[iL1[j1]] << std::endl;
//                std::cout << "    L1 " << l1EtaBin << ", " << l1PhiBin << std::endl;
               bool etaMatch = false;
               bool phiMatch = false;
               if ( abs(l2EtaBin - l1EtaBin) <= etaBinCut ) etaMatch = true;
               if ( abs(l2PhiBin - l1PhiBin) <= phiBinCut  || abs(l2PhiBin - l1PhiBin) >= 18-phiBinCut ) phiMatch = true;
               if ( etaMatch && phiMatch )
               {
                  isL1L2Matched = true;
                  break;
               }
            }
         }
      }
      
//       if ( fired && ! isL1L2Matched )
//          std::cout << "IS NOT MATCHED!!! ###################################################" << std::endl;
//       std::cout << "--------------------------------------------------------------------" << std::endl;
      
      // Fill histograms
      if ( fired && l2Fired && isL1L2Matched )
      {
         h_JetPtNom -> Fill(l2JetPt_[0]);
      }
      
      if ( l2Fired )
      {
         h_JetPtRef -> Fill(l2JetPt_[0]);
      }

      
   }
   
     TGraphAsymmErrors * g_JetPtEff = new TGraphAsymmErrors( h_JetPtNom, h_JetPtRef, "cl=0.683 b(1,1) mode" );
     TString outFilename = "histograms";
     TFile * outFile = new TFile(outFilename+".root","RECREATE");
     h_JetPtRef      -> Write();
     h_JetPtNom      -> Write();
     g_JetPtEff      -> Write();
     outFile -> Close();
//     
    
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
      
   
   l1JetNCut   = 1;
   l1JetPtCut  = 84.;
   l1JetEtaCut = 3.0;
   l2JetNCut   = 1;
   l2JetPtCut  = 30.;
   l2JetEtaCut = 2.6;
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   infile >> title >> run ;
   cout << title << " = " << run << endl;
   infile >> title >>  l1JetPtCut; 
   cout << title << " = " << l1JetPtCut << endl;
   infile >> title >>  l2JetPtCut; 
   cout << title << " = " << l2JetPtCut << endl;
   infile >> title >>  l1JetEtaCut; 
   cout << title << " = " << l1JetEtaCut << endl;
   infile >> title >>  l2JetEtaCut;
   cout << title << " = " << l2JetEtaCut << endl;
   
   infile.close();
}
