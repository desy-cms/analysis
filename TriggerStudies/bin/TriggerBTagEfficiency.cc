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

int ncut[4];
float ptcut[4];
float etacut[4];
float btagcut[4];

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
   TH1F * h_BtagRef = new TH1F("h_BtagRef","", 100, 0, 3.);
   TH1F * h_BtagNom = new TH1F("h_BtagNom","", 100, 0, 3.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainJets[4];
//   chainJets[0]  = new TChain("TriggerStudies/hltL3CombinedSecondaryVertexBJetTags"); // dummy
//   chainJets[1]  = new TChain("TriggerStudies/hltL3CombinedSecondaryVertexBJetTags"); // dummy
   chainJets[2]  = new TChain("TriggerStudies/hltL3CombinedSecondaryVertexBJetTags");
   chainJets[3]  = new TChain("TriggerStudies/hltL3CombinedSecondaryVertexBJetTagsCopy");
   
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
   
   for ( int l = 2 ; l < 4 ; ++l )
   {
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chainJets[l]);
      chainJets[l]->AddFileInfoList((TCollection*) fc.GetList());
      chainJets[l]->SetBranchAddress( "n"   , &jetN_   [l]);
      chainJets[l]->SetBranchAddress( "pt"  ,  jetPt_  [l]);
      chainJets[l]->SetBranchAddress( "eta" ,  jetEta_ [l]);
      chainJets[l]->SetBranchAddress( "phi" ,  jetPhi_ [l]);
      chainJets[l]->SetBranchAddress( "e"   ,  jetE_   [l]);
      chainJets[l]->SetBranchAddress( "tag" ,  jetBtag_[l]);
  }
   
   // The highest level, ie the level with respect to which the efficiency will be evaluated
   int highestLevel = -1;
   for ( int i = 2 ; i < 4 ; ++i )
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
      std::vector<float> jettags[4];
            
      // trigger emulators
      for ( int l = 2 ; l < 4 ; ++l )
      {
         jets[l].clear();
         jettags[l].clear();
         if ( ncut[l] > 0 )      // else same as no trigger selection, ie trigger always fired at this level
         {
            for ( int j = 0 ; j < jetN_[l] ; ++j )
            {
               if ( jetPt_[l][j] >= ptcut[l] && fabs(jetEta_[l][j]) <= etacut[l] && jetBtag_[l][j] >= btagcut[l] )
               {
                  TLorentzVector jet;
                  jet.SetPtEtaPhiE(jetPt_[l][j], jetEta_[l][j], jetPhi_[l][j], jetE_[l][j]); 
                  jets[l].push_back(jet);
                  jettags[l].push_back(-log(1-jetBtag_[l][j]));
               }
            }
            fired[l] = int(jets[l].size()) >= ncut[l];
         }      
      }
      // The efficiency is wrt highestLevel, therefore the event must pass the highestLevel selection
      if ( ! fired[highestLevel] ) continue;
      
      // check if the chain fired
      bool chainFired = true;
      for ( int l = 2 ; l < 4 ; ++l )
         chainFired = chainFired && fired[l];
      
      // Matching between the objects at different levels; the top level only the first object will be probed
      TLorentzVector probejet = jets[highestLevel].at(0);
      
      // Matching non L1 objects
      std::bitset<4> match;
      match.set();
      float dRcut = 1.0;
      for ( int l = highestLevel-1; l >= 0 ; --l )
      {
         if ( ncut[l] <= 0 ) continue; 
         
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
            
     // Fill histograms
     if ( chainFired && match.all() )
     {
        h_BtagNom -> Fill(jettags[highestLevel].at(0));
     }
     
     h_BtagRef -> Fill(jettags[highestLevel].at(0));

     
   }
   
   TGraphAsymmErrors * g_BtagEff = new TGraphAsymmErrors( h_BtagNom, h_BtagRef, "cl=0.683 b(1,1) mode" );
   TString outFilename = "histograms";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_BtagRef      -> Write();
   h_BtagNom      -> Write();
   g_BtagEff      -> Write();
   outFile -> Close();
    
    
//    
}

// #############################################################################
void SetParameters()
{
   steer = "steer.txt";
      
   // These are ARBITRARY levels!!!
   // online btagging L1   
   ncut[0] = 0;
   ptcut[0] = 20.;
   etacut[0] = 2.4;
   btagcut[0] = -100.;
   // online btagging L2
   ncut[1] = 0;
   ptcut[1] = 20.;
   etacut[1] = 2.4;
   btagcut[1] = -100.;
   // online btagging L3
   ncut[2] = 1;
   ptcut[2] = 20.;
   etacut[2] = 2.4;
   btagcut[2] = 1-exp(-0.678);
   // offline btagging
   ncut[3] = 1;
   ptcut[3] = 20.;
   etacut[3] = 2.4;
   btagcut[3] = 0.;
   
   
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   
   infile.close();
}
