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

int ncut[2];
float ptcut[2];
float etacut[2];
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
   
   // Histograms
   TH1F * h_BtagLogRef = new TH1F("h_BtagLogRef","", 50, 0, 1.);
   TH1F * h_BtagLogNom = new TH1F("h_BtagLogNom","", 50, 0, 1.);
   TH2F * h_BtagCaloPF = new TH2F("h_BtagCaloPF","", 100, 0., 1., 100, 0., 1.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainJets[2];

   chainJets[0]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsCalo");
   chainJets[1]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsPF");
   
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
//   int highestLevel = 1;
   
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   if ( nEvents < 0 )  nEvents = nentries;
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // The trigger objects after online selection, ie all objects that pass the selection
      std::vector<TLorentzVector> jets[2];
      std::vector<float> jettags[2];
      bool fired;    
      
       
      // trigger emulator
      for ( int l = 0 ; l < 2 ; ++l )
      {
         jets[l].clear();
         jettags[l].clear();
         for ( int j = 0 ; j < jetN_[l] ; ++j )
         {
            if ( jetPt_[l][j] >= ptcut[l] && fabs(jetEta_[l][j]) <= etacut[l] && jetBtag_[l][j] >= btagcut[l] )
            {
               TLorentzVector jet;
               jet.SetPtEtaPhiE(jetPt_[l][j], jetEta_[l][j], jetPhi_[l][j], jetE_[l][j]); 
               jets[l].push_back(jet);
//               jettags[l].push_back(-log(1-jetBtag_[l][j]));
               jettags[l].push_back(jetBtag_[l][j]);
            }
         }
      }
      
      // trigger fired
      fired = (int(jets[0].size()) >= ncut[0]); 
      
      // muss pass offline selection
      if ( jets[1].size() < 1 ) continue;
      
      // highest b-tag offline jet
      int j1 = -1;
      float maxtag = -999999;
      for ( size_t j = 0; j < jets[1].size(); ++j )
      {
         if ( jettags[1].at(j) > maxtag )
         {
            maxtag = jettags[1].at(j);
            j1 = j;
         }
      }
      h_BtagLogRef -> Fill(jettags[1].at(j1));
      
      // highest b-tag online jet
      int j0 = -1;
      maxtag = -999999;
      for ( size_t j = 0; j < jets[0].size(); ++j )
      {
         if ( jettags[0].at(j) > maxtag )
         {
            maxtag = jettags[0].at(j);
            j0 = j;
         }
      }
      
//       // Matching jets
//       float dRcut = 0.5;
//       bool match = false;
//       TLorentzVector jet1 = jets[1].at(j1);
//       h_BtagLogRef -> Fill(jettags[1].at(j1));
//       for ( size_t j0 = 0; j0 < jets[0].size(); ++j0 )
//       {
//          TLorentzVector jet0 = jets[0].at(j0);
//          float deltaR = jet1.DeltaR(jet0);
//          if ( deltaR < dRcut )
//          {
//             h_BtagCaloPF -> Fill(jettags[0].at(j0),jettags[1].at(j1));
//             match = true;
//             break;
//          }
//       }
      
      if ( j0 >= 0 && j1 >= 0 ) 
      h_BtagCaloPF -> Fill(jettags[0].at(j0),jettags[1].at(j1));
      


//       for ( int l = highestLevel-1; l >= 0 ; --l )
//       {
//          if ( ncut[l] <= 0 ) continue; 
//          
//          // Match first leading non L1 jets, then the matched with lower level jets
//          for ( int j = 0 ; j < int(jets[l].size()) ; ++j )
//          {
//             float deltaR = probejet.DeltaR(jets[l].at(j));
//             if ( deltaR < dRcut ) // Not necessarily the closest but a match
//             {
//                match[l] = true;
//                probejet = jets[l].at(j);
//                break; // go to the next level with the matched jet as the probe jet
//             }
//             else
//             {
//                match[l] = false;
//             }
//          }
//       }
      
      
      if ( fired )
         h_BtagLogNom -> Fill(jettags[1].at(j1));
      
//       
      
            
     // Fill histograms
//     if ( chainFired && match.all() )
//      {
//         h_BtagLogNom -> Fill(jettags[highestLevel].at(0));
//      }
//      
//      h_BtagLogRef -> Fill(jettags[highestLevel].at(0));

     
   }
   
   TH1F * h_fraction = (TH1F*) h_BtagLogRef->Clone("h_fraction");
   h_fraction -> Divide(h_BtagLogNom);
   
   TGraphAsymmErrors * g_BtagEff = new TGraphAsymmErrors( h_BtagLogNom, h_BtagLogRef, "cl=0.683 b(1,1) mode" );
   TString outFilename = "histograms";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_BtagLogRef      -> Write();
   h_BtagLogNom      -> Write();
   h_fraction -> Write();
   h_BtagCaloPF      -> Write();
   g_BtagEff      -> Write();
   outFile -> Close();
    
    
//    
}

// #############################################################################
void SetParameters()
{
   steer = "steer.txt";
      
   // These are ARBITRARY levels!!!
   // online btagging
   ncut[0] = 1;
   ptcut[0] = 80.;
   etacut[0] = 2.4;
   btagcut[0] = 0.;
   // offline btagging
   ncut[1] = 1;
   ptcut[1] = 80.;
   etacut[1] = 2.4;
   btagcut[1] = 0.;
   
   
   nEvents     = -1;
   run         = "ABCD";
   
   string title;
   ifstream infile;
   infile.open (steer.c_str());
   cout << "Input parameters" << endl;
   
   infile.close();
}
