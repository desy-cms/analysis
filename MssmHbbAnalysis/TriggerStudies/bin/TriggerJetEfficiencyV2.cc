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

TString outFilename;


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
   TH1F * h_JetPtRef = new TH1F("h_JetPtRef","", 40, 0, 400.);
   TH1F * h_JetPtNom = new TH1F("h_JetPtNom","", 40, 0, 400.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainJets[4];
   chainJets[0]  = new TChain("TriggerStudies/hltL1extraParticles_MergedJets");
   chainJets[1]  = new TChain("TriggerStudies/hltAK4CaloJetsCorrectedIDPassed");
   chainJets[2]  = new TChain("TriggerStudies/hltAK4PFJetsCorrectedLooseID");
   chainJets[3]  = new TChain("TriggerStudies/ak4GenJets");
   
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
   for ( int i = 0 ; i < branches->GetEntries() ; ++i )
      std::cout << branches->At(i)->GetName() << std::endl;
   
   for ( int l = 0 ; l < 4 ; ++l )
   {
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chainJets[l]);
      chainJets[l]->AddFileInfoList((TCollection*) fc.GetList());
      std::cout << "Chain name = " << chainJets[l]->GetName() << std::endl;
      branches = chainJets[l]->GetListOfBranches();
      for ( int i = 0 ; i < branches->GetEntries() ; ++i )
         std::cout << branches->At(i)->GetName() << std::endl;
      std::cout << "---" << std::endl;
      chainJets[l]->SetBranchAddress( "n"   , &jetN_  [l]);
      chainJets[l]->SetBranchAddress( "pt"  ,  jetPt_ [l]);
      chainJets[l]->SetBranchAddress( "eta" ,  jetEta_[l]);
      chainJets[l]->SetBranchAddress( "phi" ,  jetPhi_[l]);
      chainJets[l]->SetBranchAddress( "e"   ,  jetE_  [l]);
   }
   
   
   std::vector<int> levels;
   levels.clear();
   // The highest level, ie the level with respect to which the efficiency will be evaluated
   int highestLevel = -1;
   bool doMatchL1 = (ncut[0] > 0);
   for ( int i = 0 ; i < 4 ; ++i )
      if ( ncut[i] > 0 ) levels.push_back(i);
   highestLevel = levels.back();
   
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
      for ( size_t il = 0 ; il < levels.size() ; ++il )
      {
         int l = levels[il];
         jets[l].clear();
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
      
      // The efficiency is wrt highestLevel, therefore the event must pass the highestLevel selection
      if ( ! fired[highestLevel] ) continue;
      
      // highest level (reference) histogram; leading jet only
      TLorentzVector probejet = jets[highestLevel].at(0);
      h_JetPtRef -> Fill(probejet.Pt());
      
      
      // check if the chain fired
      bool chainFired = true;
      for ( size_t il = 0 ; il < levels.size() ; ++il )
      {
         int l = levels[il];
         chainFired = chainFired && fired[l];
      }
      if ( ! chainFired ) continue;
      
      // Matching between the objects at different levels; the top level only the first object will be probed
      
      // Matching non L1 objects
      std::bitset<4> match;
      match.set();
      float dRcut = 0.5;
      for ( size_t il = 0 ; il < levels.size() ; ++il )
      {
         int l = levels[il];
         if ( l == 0 || l == highestLevel ) continue;  // improve this, too lazy today
         
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
               match[l] = false;
         }
      }
      
      // Matching  probejet with L1 objects
      if ( doMatchL1 )
      {
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
      }
      
     if ( ! match.all() ) continue;
     
     // Fill histograms
     h_JetPtNom -> Fill(jets[highestLevel].at(0).Pt());
 
     
   }
   
   TGraphAsymmErrors * g_JetPtEff = new TGraphAsymmErrors( h_JetPtNom, h_JetPtRef, "cl=0.683 b(1,1) mode" );
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
   ptcut[0] = 100.;
   etacut[0] = 3.0;
   // L2 calo jet
   ncut[1] = 1;
   ptcut[1] = 100.;
   etacut[1] = 2.6;
   // L3 PF jets
   ncut[2] = 1;
   ptcut[2] = 20.;
   etacut[2] = 2.5;
   // Offline jets
   ncut[3] = 0;
   ptcut[3] = 20.;
   etacut[3] = 2.4;
   
   outFilename = "trigger_jet_efficiencies_qcd_L1Jet100_L2Jet100_L1L2wrtL3";
   
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   
   infile.close();
}
