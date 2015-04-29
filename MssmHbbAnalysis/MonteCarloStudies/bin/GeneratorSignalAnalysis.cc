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

TString outFilename;


int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   TH1F * h_PtJet[4];
   // Histograms
   for ( int i = 0 ; i < 4 ; ++i )
      h_PtJet[i] = new TH1F(Form("h_PtJet_%i",i),"", 200, 0, 1000.);
   TH1F * h_MassJet01 = new TH1F("h_MassJet01","", 200, 0, 1000.);
   TH2F * h_Jet0Jet1_Pt = new TH2F("h_Jet0Jet1_Pt","", 200, 0, 1000., 200, 0, 1000.);
   
   // TTree chains and friendship?
   TChain * chainEvent = new TChain("MonteCarloStudies/EventInfo");
   TChain * chainJets  = new TChain("MonteCarloStudies/ak4GenJets");
   
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
   TObjArray * branches = chainEvent->GetListOfBranches();
   
   // Don't forget that the trees must be friends!!!
   chainEvent -> AddFriend(chainJets);
   chainJets  -> AddFileInfoList((TCollection*) fc.GetList());
   branches = chainJets -> GetListOfBranches();
   for ( int i = 0 ; i < branches->GetEntries() ; ++i )
   {
      chainJets->SetBranchAddress( "n"   , &jetN_  [0]);
      chainJets->SetBranchAddress( "pt"  ,  jetPt_ [0]);
      chainJets->SetBranchAddress( "eta" ,  jetEta_[0]);
      chainJets->SetBranchAddress( "phi" ,  jetPhi_[0]);
      chainJets->SetBranchAddress( "e"   ,  jetE_  [0]);
   }
   
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   
   for ( int i = 0 ; i < nentries ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // The trigger objects after online selection, ie all objects that pass the selection
      std::vector<TLorentzVector> jets;
      
      // loop over jets
      jets.clear();
      bool ok = true;
      if ( jetN_[0] < 3 ) continue; // at least 3 jets in the event
      for ( int j = 0 ; j < jetN_[0] ; ++j )
      {
         if ( j < 3 && fabs(jetEta_[0][j]) > 2.4 ) ok = false;  // 3 leading jets within the tracker acceptance
         if ( jetPt_[0][2] < 20 ) ok = false;  // 3rd leading jet soft cut
         TLorentzVector jet;
         jet.SetPtEtaPhiE(jetPt_[0][j], jetEta_[0][j], jetPhi_[0][j], jetE_[0][j]); 
         jets.push_back(jet);
      }
      
      if ( jets.size() < 3 || ! ok ) continue;
      
      if ( jets[1].Pt() > 0 && jets[0].Pt() > 0)
      {
         h_PtJet[0] -> Fill(jets[0].Pt());
         h_PtJet[1] -> Fill(jets[1].Pt());
         h_Jet0Jet1_Pt -> Fill(jets[0].Pt(),jets[1].Pt());
         TLorentzVector higgs = jets[0] + jets[1];
         h_MassJet01 -> Fill(higgs.M());
      }
      
   }
   
   
   outFilename = "genjets_m300_noptcut";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_PtJet[0]      -> Write();
   h_PtJet[1]      -> Write();
   h_Jet0Jet1_Pt   -> Write();
   h_MassJet01     -> Write();
   outFile -> Close();
    
    
//    
}


