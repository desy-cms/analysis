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
float ptmax[4];
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
  TH1F * h_BtagLogRef = new TH1F("h_BtagLogRef","", 10, 0, 10.);
  TH1F * h_BtagLogNom = new TH1F("h_BtagLogNom","", 10, 0, 10.);
   
   
//    const int nbins = 100.;
//    float xbins[nbins];
//    float lox = 0.;
//    float hix = 1.;
//    
//    for ( int i = 0 ; i < nbins ; ++i )
//    {
//       xbins[i] = lox + (hix-lox)*i/nbins;
//       std::cout << xbins[i] << "  ";
//       xbins[i] = -log(1.- xbins[i]);
//       std::cout << xbins[i] << std::endl;
//    }
//    TH1F * h_BtagLogRef = new TH1F("h_BtagLogRef","", nbins-1,xbins);
//    TH1F * h_BtagLogNom = (TH1F*) h_BtagLogRef -> Clone("h_BtagLogNom");
   
   
//   TH2F * h_BtagCaloPF = new TH2F("h_BtagCaloPF","", 100, 0., 1., 100, 0., 1.);
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainJets[4];

   chainJets[0]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsCalo");
   chainJets[1]  = new TChain("TriggerStudies/hltCombinedSecondaryVertexBJetTagsPF");
   chainJets[2]  = new TChain("TriggerStudies/slimmedJetsPuppi");
   
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
   
   std::vector<int> levels;
   levels.clear();
   // The highest level, ie the level with respect to which the efficiency will be evaluated
   int highestLevel = -1;
   for ( int i = 0 ; i < 4 ; ++i )
      if ( ncut[i] > 0 ) levels.push_back(i);
   highestLevel = levels.back();
   
   
   for ( size_t il = 0 ; il < levels.size() ; ++il )
   {
      int l = levels[il];
      // Don't forget that the trees must be friends!!!
      chainEvent -> AddFriend(chainJets[l]);
      chainJets[l]->AddFileInfoList((TCollection*) fc.GetList());
      chainJets[l]->SetBranchAddress( "n"   , &jetN_   [l]);
      chainJets[l]->SetBranchAddress( "pt"  ,  jetPt_  [l]);
      chainJets[l]->SetBranchAddress( "eta" ,  jetEta_ [l]);
      chainJets[l]->SetBranchAddress( "phi" ,  jetPhi_ [l]);
      chainJets[l]->SetBranchAddress( "e"   ,  jetE_   [l]);
      chainJets[l]->SetBranchAddress( "btag",  jetBtag_[l]);
      if ( l == 2 )
         chainJets[l]->SetBranchAddress( "flavour",  jetFlavour_[l]);
   }
  
   
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   if ( nEvents < 0 )  nEvents = nentries;
   std::cout << "Running on  " << nEvents << "  events..." << std::endl;
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      // The trigger objects after online selection, ie all objects that pass the selection
      std::vector<TLorentzVector> jets[3];
      std::vector<float> jettags[3];
       
      std::bitset<4> fired;
      fired.set();  // set all bits to true
      
      bool kinPass = true;
      // trigger emulators
      for ( size_t il = 0 ; il < levels.size() ; ++il )
      {
         int l = levels[il];
         jets[l].clear();
         jettags[l].clear();
         for ( int j = 0 ; j < jetN_[l] ; ++j )
         {
            if ( jetPt_[l][j] >= ptcut[l] && jetPt_[l][j] < ptmax[l] && fabs(jetEta_[l][j]) <= etacut[l] )
            {
//               if ( l == highestLevel )
//               {
//                  if ( abs(jetFlavour_[l][j]) == 5 || abs(jetFlavour_[l][j]) == 4 ) continue; 
//               }
               TLorentzVector jet;
               jet.SetPtEtaPhiE(jetPt_[l][j], jetEta_[l][j], jetPhi_[l][j], jetE_[l][j]); 
               jets[l].push_back(jet);
               jettags[l].push_back(jetBtag_[l][j]);
            }
         }
//         fired[l] = int(jets[l].size()) >= ncut[l];
         kinPass = ( kinPass && jets[l].size() > 0 );
      }
      bool topology = true;
      // highest level jets, if more than one jet, must be separated
      for ( size_t j1 = 0 ; j1 < jets[highestLevel].size(); ++j1 )
      {
         TLorentzVector jet1 = jets[highestLevel].at(j1);
         for ( size_t j2 = 0 ; j2 < jets[highestLevel].size(); ++j2 )
         {
            if ( j2 <= j1 ) continue;
            TLorentzVector jet2 = jets[highestLevel].at(j2);
            topology = ( topology && jet1.DeltaR(jet2) > 1. );
         }
      }
      
      // must pass kinematic selection at all levels
      if ( ! ( kinPass && topology ) ) continue;
      
      // matching
      std::vector<TLorentzVector> jetsM[4];
      std::vector<float> jettagsM[4];
      int index[4];
      std::bitset<4> match;
      match.set();
      
      // highest level jets to be matched with lower level jets
      for ( size_t j = 0 ; j < jets[highestLevel].size(); ++j )
      {
         TLorentzVector jet = jets[highestLevel].at(j);
         float btag = jettags[highestLevel].at(j);
         for ( size_t il = 0 ; il < levels.size() ; ++il )
         {
            int l = levels[il];
            if ( l == highestLevel ) continue;  // highest level not to be matched with itself
            for ( size_t jl = 0 ; jl < jets[l].size(); ++jl )
            {
               TLorentzVector jetl = jets[l].at(jl);
               if ( jet.DeltaR(jetl) < 0.5 )
               {
                  index[l] = jl;
                  match[l] = true;
                  break;
               }
               else
                  match[l] = false;
            }
         }
         if ( match.all() )
         {
            jetsM[highestLevel].push_back(jet);
            jettagsM[highestLevel].push_back(btag);
            for ( size_t il = 0 ; il < levels.size() ; ++il )
            {
               int l = levels[il];
               if ( l == highestLevel ) continue;  // highest level not to be matched with itself
               jetsM[l].push_back(jets[l].at(index[l]));
               jettagsM[l].push_back(jettags[l].at(index[l]));
               
            }
         }
      }
      
      if ( jetsM[highestLevel].size() < 1 ) continue;
      
      
      //  trigger efficiency
      for ( size_t j = 0 ; j < jetsM[highestLevel].size(); ++j )
      {
         TLorentzVector jet = jetsM[highestLevel].at(j);
         float btag = jettagsM[highestLevel].at(j);
         if ( btag < btagcut[highestLevel] ) continue;
         float btaglog = -log(1-btag);
         h_BtagLogRef -> Fill(btaglog);
         
         bool btagPass = true;
         for ( size_t il = 0 ; il < levels.size() ; ++il )
         {
            int l = levels[il];
            if ( l == highestLevel ) continue;  // highest level not to be matched with itself
            float btagl = jettagsM[l].at(j);
            btagPass = ( btagPass && btagl > btagcut[l] );
       }
       if ( btagPass ) h_BtagLogNom -> Fill(btaglog);

     }
      
   }
   
   TH1F * h_fraction = (TH1F*) h_BtagLogRef->Clone("h_fraction");
   h_fraction -> Divide(h_BtagLogNom);
   
   TGraphAsymmErrors * g_BtagEff = new TGraphAsymmErrors( h_BtagLogNom, h_BtagLogRef, "cl=0.683 b(1,1) mode" );
   TString outFilename = "histograms_Calo0p4_PF";
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   h_BtagLogRef      -> Write();
   h_BtagLogNom      -> Write();
   h_fraction -> Write();
//   h_BtagCaloPF      -> Write();
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
   ptcut[0] = 30.;
   ptmax[0] = 100000.;
   etacut[0] = 2.4;
   btagcut[0] = 0.4;
   // online btagging
   ncut[1] = 1;
   ptcut[1] = 30.;
   ptmax[1] = 100000.;
   etacut[1] = 2.4;
   btagcut[1] = 0.;
   // offline btagging
   ncut[2] = 0;
   ptcut[2] = 30.;
   ptmax[2] = 100000.;
   etacut[2] = 2.4;
   btagcut[2] = 0.;
   // special
   ncut[3] = 0;
   ptcut[3] = 30.;
   ptmax[3] = 100000.;
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
