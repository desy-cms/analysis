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

// bool myreplace(std::string& , const std::string& , const std::string& );

int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   lumi = 1.; // in fb-1
   
   float ptcut[3] = {110.,110.,30.};
   float etacut[3] = {2.4,2.4,2.4};
   float mw = -1;
   float dmw = -1;
   float detacut = -1.;
   
   if ( argc > 1 )
   {
      ptcut[0]  = atof(argv[1]);
      ptcut[1]  = atof(argv[2]);
      ptcut[2]  = atof(argv[3]);
      etacut[0] = atof(argv[4]);
      etacut[1] = atof(argv[5]);
      etacut[2] = atof(argv[6]);
   }
   if ( argc == 8 ) detacut = atof(argv[7]);
   if ( argc == 10 )
   {
      detacut = atof(argv[7]);
      mw = atof(argv[8]);
      dmw = atof(argv[9]);
   }
   
   TH1F * h_selection = new TH1F("h_selection", "", 2,0.,2.);
   
   TH1F * h_PtJet[4];
   TH1F * h_EtaJet[4];
   // Histograms
   for ( int i = 0 ; i < 4 ; ++i )
   {
      h_PtJet[i]  = new TH1F(Form("h_PtJet_%i",i), "", 800, 0, 4000.);
      h_EtaJet[i] = new TH1F(Form("h_EtaJet_%i",i),"", 60, -3, 3.);
   }
   TH1F * h_dEta01    = new TH1F("h_dEta01","", 60, 0., 6.);
   TH1F * h_dEta12    = new TH1F("h_dEta12","", 60, 0., 6.);
   TH1F * h_dEta02    = new TH1F("h_dEta02","", 60, 0., 6.);
   TH1F * h_dR01      = new TH1F("h_R01","", 60, 0., 6.);
   TH1F * h_dR12      = new TH1F("h_R12","", 60, 0., 6.);
   TH1F * h_dR02      = new TH1F("h_R02","", 60, 0., 6.);
   TH1F * h_MassJet01 = new TH1F("h_MassJet01","", 800, 0, 4000.);
   TH2F * h_Jet0Jet1_Pt  = new TH2F("h_Jet0Jet1_Pt","", 800, 0, 4000., 800, 0, 4000.);
   TH2F * h_Jet0Jet1_Eta = new TH2F("h_Jet0Jet1_Eta","", 30, 0, 3, 30, 0, 3);
   
   
   // TTree chains and friendship?
   TChain * chainMeta  = new TChain("MonteCarloStudies/Metadata");
   TChain * chainEvent = new TChain("MonteCarloStudies/EventInfo");
   TChain * chainJets  = new TChain("MonteCarloStudies/ak4GenJets");
   
   // Input files
   std::string inputList = "rootFileList.txt"; 
   ifstream inputFile (inputList.c_str());
   std::string filename;
   getline (inputFile,filename);
   boost::filesystem::path path(filename.c_str());
//   filename = boost::filesystem::basename(filename);
   std::string subfilename = path.parent_path().filename().native();
   
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
      
      h_selection -> Fill(0.5);
      
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
      
      TLorentzVector dijet = jets[0] + jets[1];
      
      float dEta01 = fabs(jets[0].Eta() - jets[1].Eta());
      float dEta12 = fabs(jets[1].Eta() - jets[2].Eta());
      float dEta02 = fabs(jets[0].Eta() - jets[2].Eta());
      
      float dR01 = jets[0].DeltaR(jets[1]);
      float dR12 = jets[1].DeltaR(jets[2]);
      float dR02 = jets[0].DeltaR(jets[2]);
      
      float m12 = dijet.M();
      
      if ( !(      jets[0].Pt()   > ptcut [0]) )   continue;
      if ( !( fabs(jets[0].Eta()) < etacut[0] ) )  continue;
      if ( !(      jets[1].Pt()   > ptcut [1]) )   continue;
      if ( !( fabs(jets[1].Eta()) < etacut[1] ) )  continue;
      if ( !(      jets[2].Pt()   > ptcut [2] ) )  continue;
      if ( !( fabs(jets[2].Eta()) < etacut[2] ) )  continue;
      if ( detacut > 0 )
         if ( dEta01 > detacut ) continue;
      if ( mw > 0 && dmw > 0 )
         if ( m12 < (mw-dmw) || m12 > (mw+dmw) ) continue;
      
      for ( size_t j = 0 ; j < jets.size() ; ++j )
      {
         if ( j > 3 ) break; // only up to 4 leading jets
         h_PtJet[j]  -> Fill(jets[j].Pt());
         h_EtaJet[j] -> Fill(jets[j].Eta());
      }
      h_Jet0Jet1_Pt  -> Fill(jets[0].Pt(),jets[1].Pt());
      h_Jet0Jet1_Eta -> Fill(fabs(jets[0].Eta()),fabs(jets[1].Eta()));
      h_MassJet01 -> Fill(m12);
      h_dEta01 -> Fill(dEta01);
      h_dEta12 -> Fill(dEta12);
      h_dEta02 -> Fill(dEta02);
      h_dR01 -> Fill(dR01);
      h_dR12 -> Fill(dR12);
      h_dR02 -> Fill(dR02);
      h_selection -> Fill(1.5);
   }
   /// end of loop event
   std::cout << "Processed  " << nentries << "  events" << "\n";
   
   
   
   std::ostringstream tmp;
   tmp << "genjets_jpt" << int(ptcut[0]) << "_jpt" << int(ptcut[1]) << "_jpt" << int(ptcut[2]);
   tmp << "_jeta" << etacut[0] << "_jeta" << etacut[1] << "_jeta" << etacut[2];
   if ( detacut > 0 ) tmp << "_deta" << detacut;
   if ( mw > 0 && dmw > 0  ) tmp << "_m12w" << mw << "pm" << dmw;
   outFilename = TString(tmp.str());
   outFilename.ReplaceAll(".","p");
   
//   outFilename = "genjets_"+subfilename+".jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
//   outFilename = "genjets_jpt110_jpt110_jpt30_jeta2p4_jeta2p4_jeta2p4";
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
   h_Jet0Jet1_Eta -> Scale(scale);
   h_MassJet01    -> Scale(scale);
   h_selection    -> Scale(scale);
   
   
   h_Jet0Jet1_Pt  -> Write();
   h_Jet0Jet1_Eta -> Write();
   h_MassJet01    -> Write();
   h_dEta01    -> Write();
   h_dEta12    -> Write();
   h_dEta02    -> Write();
   h_dR01    -> Write();
   h_dR12    -> Write();
   h_dR02    -> Write();
   
   h_selection    -> Write();
   
   outFile -> Close();
    
//    
}

// bool myreplace(std::string& str, const std::string& from, const std::string& to) {
//     if(from.empty()) return false;
//     size_t start_pos = 0;
//     while((start_pos = str.find(from, start_pos)) != std::string::npos) {
//         str.replace(start_pos, from.length(), to);
//         start_pos += to.length(); 
//     }
//     return true;
// }
