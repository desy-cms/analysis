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

void SetParameters();

int njets[2];
float ptcut[2];
float ptmax[2];
float etacut[2];

int nbtags[2];
float btagcut[2];

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
   
   // TTree chains
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainTrgResults  = new TChain("TriggerStudies/TriggerResults_HLT3PB");
   TChain * chainJets[2];
   chainJets[0]  = new TChain("TriggerStudies/hltAK4CaloJetsCorrectedIDPassed");
   chainJets[1]  = new TChain("TriggerStudies/hltAK4PFJetsCorrectedLooseID");
   TChain * chainBtags[2];
   chainBtags[0]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsCalo");
   chainBtags[1]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsPF");
   
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
   
   // Trigger results
   // Friendship
   chainTrgResults -> AddFileInfoList((TCollection*) fc.GetList());
   chainEvent -> AddFriend(chainTrgResults);
   TObjArray * trgBranches = chainTrgResults->GetListOfBranches();
   int nPaths = trgBranches -> GetEntries();
//   trgAccepts_ = new bool [nPaths];
//   trgAccepts_ = std::unique_ptr<bool[]>(new bool [nPaths]);
   std::map<std::string, int> trgCounter;
   for ( int i = 0 ; i < nPaths ; ++i )
   {
      std::string path = trgBranches->At(i)->GetName();
      chainTrgResults -> SetBranchAddress(path.c_str(), &trgAccepts_[path]);
      trgCounter[path] = 0;
   }
   
   for ( int l = 0 ; l < 2 ; ++l )
   {
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chainJets[l]);
      chainJets[l]->AddFileInfoList((TCollection*) fc.GetList());
      chainEvent -> AddFriend(chainBtags[l]);
      chainBtags[l]->AddFileInfoList((TCollection*) fc.GetList());
      
      chainJets[l]->SetBranchAddress( "n"   , &jetN_   [l]);
      chainJets[l]->SetBranchAddress( "pt"  ,  jetPt_  [l]);
      chainJets[l]->SetBranchAddress( "eta" ,  jetEta_ [l]);
      chainJets[l]->SetBranchAddress( "phi" ,  jetPhi_ [l]);
      chainJets[l]->SetBranchAddress( "e"   ,  jetE_   [l]);
      
      chainBtags[l]->SetBranchAddress( "n"   , &jetBtagN_[l]);
      chainBtags[l]->SetBranchAddress( "tag" , jetBtag_  [l]);
  }
   
   // The highest level, ie the level with respect to which the efficiency will be evaluated
//   int highestLevel = 1;
   
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   if ( nEvents < 0 )  nEvents = nentries;
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   
   
   int nGenerated = 0; // in the end must be equal to nEvents
   int nTriggered = 0;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      ++nGenerated;
      
      // openHLT loose trigger
      if ( ! trgAccepts_["HLT_PFJet_BTagCSV_Changed_CSVL3_0p0_v1"] ) continue;
      
      // trigger emulator
         // online calo jets
      int ncalo = 0;
      bool calotrigger = false;
      for ( int j = 0 ; j < jetN_[0] ; ++j )
      {
         if ( jetPt_[0][j] >= ptcut[0] && jetPt_[0][j] < ptmax[0] && fabs(jetEta_[0][j]) <= etacut[0] )
            ++ncalo;
      }
      
      if ( ncalo < njets[0] ) continue;
      calotrigger = true;
      
         // online calo btag
      int ncalobtag = 0;
      bool calobtagtrigger = false;
      for ( int j = 0 ; j < jetBtagN_[0] ; ++j )
      {
         if ( jetBtag_[0][j] < btagcut[0] )
            ++ncalobtag;
      }
      
      if ( ncalobtag < nbtags[0] ) continue;
      calobtagtrigger = true;
      
         // online calo jets
      int npf = 0;
      bool pftrigger = false;
      for ( int j = 0 ; j < jetN_[1] ; ++j )
      {
         if ( jetPt_[1][j] >= ptcut[1] && jetPt_[1][j] < ptmax[1] && fabs(jetEta_[1][j]) <= etacut[1] )
            ++npf;
      }
      
      if ( npf < njets[1] ) continue;
      pftrigger = true;
      
         // online pf btag
      int npfbtag = 0;
      bool pfbtagtrigger = false;
      for ( int j = 0 ; j < jetBtagN_[1] ; ++j )
      {
         if ( jetBtag_[1][j] < btagcut[1] )
            ++npfbtag;
      }
      
      if ( npfbtag < nbtags[1] ) continue;
      pfbtagtrigger = true;
      
      if ( calotrigger && calobtagtrigger && pftrigger && pfbtagtrigger )
         ++nTriggered;
   }
   
   std::cout << nTriggered << "  " << nGenerated << std::endl;
   
   std::cout << "Efficiency = " << float(nTriggered)/nGenerated << std::endl;
      
//    
}

// #############################################################################
void SetParameters()
{
   steer = "steer.txt";
      
   // These are ARBITRARY levels!!!
   // calo jets
   njets[0] = 2;
   ptcut[0] = 100.;
   ptmax[0] = 100000.;
   etacut[0] = 2.4;
   // pf jets
   njets[1] = 2;
   ptcut[1] = 100.;
   ptmax[1] = 100000.;
   etacut[1] = 2.4;
   
   // btagging
   nbtags[0] = 2;
   nbtags[1] = 0;
   btagcut[0] = 0.9;
   btagcut[1] = 0;
   
   
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   
   infile.close();
}
