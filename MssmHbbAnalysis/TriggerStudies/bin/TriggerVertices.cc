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

int    nEvents    ;
string run        ;
string steer      ;
float  pi;

int main(int argc, char * argv[])
{
   
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   // Histograms
   TH2F * h_Z_L3vsPF = new TH2F("h_Z_L3vsPF","", 1000, -10., 10., 1000, -10., 10.);
   TH2F * h_X_L3vsPF = new TH2F("h_X_L3vsPF","", 100, -1., 1., 100, -1., 1.);
   TH2F * h_Y_L3vsPF = new TH2F("h_Y_L3vsPF","", 100, -1., 1., 100, -1., 1.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chain[2];

   chain[0]  = new TChain("TriggerStudies/hltVerticesL3");
   chain[1]  = new TChain("TriggerStudies/hltVerticesPFFilter");
   
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
   
   for ( int l = 0 ; l < 2 ; ++l )
   {
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chain[l]);
      chain[l]->AddFileInfoList((TCollection*) fc.GetList());
      chain[l]->SetBranchAddress( "n"    , &pvN_   [l]);
      chain[l]->SetBranchAddress( "x"    ,  pvX_   [l]);
      chain[l]->SetBranchAddress( "y"    ,  pvY_   [l]);
      chain[l]->SetBranchAddress( "z"    ,  pvZ_   [l]);
      chain[l]->SetBranchAddress( "xe"   ,  pvXe_  [l]);
      chain[l]->SetBranchAddress( "ye"   ,  pvYe_  [l]);
      chain[l]->SetBranchAddress( "ze"   ,  pvZe_  [l]);
      chain[l]->SetBranchAddress( "fake" ,  pvFake_[l]);
      chain[l]->SetBranchAddress( "chi2" ,  pvChi2_[l]);
      chain[l]->SetBranchAddress( "ndof" ,  pvNdof_[l]);
      chain[l]->SetBranchAddress( "rho"  ,  pvRho_ [l]);
  }
   
   // The highest level, ie the level with respect to which the efficiency will be evaluated
//   int highestLevel = 1;
   
   // Number of loop events
   int nEvents = chainEvent->GetEntries();
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      if ( ! pvFake_[0][0] )
      {
         h_Z_L3vsPF -> Fill(pvZ_[0][0],pvZ_[1][0]);
         h_X_L3vsPF -> Fill(pvX_[0][0],pvX_[1][0]);
         h_Y_L3vsPF -> Fill(pvY_[0][0],pvY_[1][0]);
      }
    
   }
   
   TString outFilename = "vertices_histograms";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_Z_L3vsPF   -> Write();
   h_X_L3vsPF   -> Write();
   h_Y_L3vsPF   -> Write();
   outFile -> Close();
    
    
//    
}
