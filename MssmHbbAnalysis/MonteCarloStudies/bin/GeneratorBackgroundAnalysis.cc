#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <bitset>
#include <boost/filesystem.hpp>

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
double lumi;

int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   lumi = 1.; // in fb-1
   
   TH1F * h_PtJet[4];
   TH1F * h_EtaJet[4];
   // Histograms
   for ( int i = 0 ; i < 4 ; ++i )
   {
      h_PtJet[i]  = new TH1F(Form("h_PtJet_%i",i), "", 600, 0, 3000.);
      h_EtaJet[i] = new TH1F(Form("h_EtaJet_%i",i),"", 30, -3, 3.);
   }
   TH1F * h_dEta01    = new TH1F("h_dEta01","", 30, 0., 6.);
   TH1F * h_MassJet01 = new TH1F("h_MassJet01","", 400, 0, 2000.);
   TH2F * h_Jet0Jet1_Pt = new TH2F("h_Jet0Jet1_Pt","", 600, 0, 3000., 600, 0, 3000.);
   
   
   // TTree chains and friendship?
   TChain * chainMeta  = new TChain("MonteCarloStudies/Metadata");
   TChain * chainEvent = new TChain("MonteCarloStudies/EventInfo");
   TChain * chainJets  = new TChain("MonteCarloStudies/ak4GenJets");
   
   // Input files
   std::string inputList = "rootFileList.txt"; // must be only one file!!!
   ifstream inputFile (inputList.c_str());
   std::string filename;
   getline (inputFile,filename);
   filename = boost::filesystem::basename(filename);
   std::string subfilename = filename.erase(0,filename.find("_")+1);
   subfilename = subfilename.substr(0, subfilename.find("_"));
   
   
   cout << "List of input files: " << inputList << endl;
   TFileCollection fc("dum","",inputList.c_str());
   
   chainMeta->AddFileInfoList((TCollection*) fc.GetList());
   chainMeta->SetBranchAddress( "xsection", &xsection_ );
   chainMeta->GetEntry(0);
   xsection_ *= 1000; // convert from pb to fb
   
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
   
   double effLumi = nentries/xsection_;
   double scale   = lumi/effLumi;
   
   std::cout << "Cross section   = " << xsection_ << std::endl;
   std::cout << "Effective lumi  = " << effLumi   << std::endl;
   std::cout << "Nmber of events = " << nentries*scale << std::endl;
   
   
   for ( int i = 0 ; i < nentries ; ++i )
   {
      if ( i%1000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // The trigger objects after online selection, ie all objects that pass the selection
      std::vector<TLorentzVector> jets;
      
      // leading jets and preselection
      jets.clear();
      bool ok = true;
      if ( jetN_[0] < 3 ) continue; // at least 3 jets in the event
      for ( int j = 0 ; j < jetN_[0] ; ++j )
      {
         if ( j < 3 && fabs(jetEta_[0][j]) > 2.4 ) ok = false;  // 3 leading jets within the tracker acceptance
         if ( jetPt_[0][2] < 30 ) ok = false;  // 3rd leading jet soft cut
         TLorentzVector jet;
         jet.SetPtEtaPhiE(jetPt_[0][j], jetEta_[0][j], jetPhi_[0][j], jetE_[0][j]); 
         jets.push_back(jet);
      }
      // must have at least 3 jets
      if ( jets.size() < 3 ) continue;
      
      // deltaR between 3 leading jets must be > 1
      if ( jets[0].DeltaR(jets[1]) < 1. || jets[1].DeltaR(jets[2]) < 1. || jets[2].DeltaR(jets[0]) < 1. ) ok = false;
      
      // preselection
      if ( ! ok ) continue;
      
      float dEta01 = fabs(jets[0].Eta() - jets[1].Eta());
      
      TLorentzVector dijet = jets[0] + jets[1];
      if ( !(      jets[0].Pt()   > 140.) )  continue;
      if ( !( fabs(jets[0].Eta()) < 1.6 ) )  continue;
      if ( !(      jets[1].Pt()   > 100.) )  continue;
      if ( !( fabs(jets[1].Eta()) < 1.6 ) )  continue;
      if ( !(      jets[2].Pt()   > 30. ) )  continue;
      if ( !( fabs(jets[2].Eta()) < 2.4 ) )  continue;
      
      for ( size_t j = 0 ; j < jets.size() ; ++j )
      {
         if ( j > 3 ) break; // only up to 4 leading jets
         h_PtJet[j]  -> Fill(jets[j].Pt());
         h_EtaJet[j] -> Fill(jets[j].Eta());
      }
      h_Jet0Jet1_Pt -> Fill(jets[0].Pt(),jets[1].Pt());
      h_MassJet01 -> Fill(dijet.M());
      h_dEta01 -> Fill(dEta01);
      
   }
   
   
   outFilename = "genjets_"+subfilename+".jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   // scale to luminosity; write to file
   for ( size_t j = 0 ; j < 4 ; ++j )
   {
      h_PtJet[j]  -> Scale(scale);
      h_EtaJet[j] -> Scale(scale);
      h_PtJet[j]  -> Write();
      h_EtaJet[j] -> Write();
   }
   h_Jet0Jet1_Pt  -> Scale(scale);
   h_MassJet01    -> Scale(scale);
   h_Jet0Jet1_Pt  -> Write();
   h_MassJet01    -> Write();
   h_dEta01    -> Write();
   
   outFile -> Close();
    
   std::cout << "Processed  " << nentries << "  events" << "\n";
//    
}


