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
   
   std::map<std::string, TH1F*> h1;
   
   std::string triggerPath = "HLT_BTagMu_DiJet20_Mu5";
   std::string dataset = "QCD_Pt_30to50_Mu_Pt5";
   
   if ( argc == 2 )
   {
      dataset = argv[1];
   }
   else if ( argc == 3 ) 
   {
      dataset = argv[1];
      triggerPath = argv[2];
   }
   
   h1["h_jetTagPt"]  = new TH1F("h_jetTagPt", "", 500, 0., 500.);
   h1["h_jetProbePt"]= new TH1F("h_jetProbePt", "", 500, 0., 500.);
   h1["h_jetTPdPhi"] = new TH1F("h_dPhiJetTP", "", 64, 0., 3.2);
   
   h1["h_jetTagFlavour"]  = new TH1F("h_jetTagFlavour", "", 35, -10, 25.);
   h1["h_jetProbeFlavour"]= new TH1F("h_jetProbeFlavour", "", 35, -10., 25.);
   
   h1["h_jetTagBtag"]  = new TH1F("h_jetTagBtag", "", 100, 0., 1.);
   h1["h_jetProbeBtag"]= new TH1F("h_jetProbeBtag", "", 100, 0., 1.);
   
   h1["h_SelChain"] = new TH1F("h_SelChain", "", 7, 0., 7.);
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(1,"no cut");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(2,"btagmu trigger");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(3,"online btag (emul)");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(4,"dijet selection");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(5,"muon in jet (tag jet)");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(6,"offline 2 btags");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(7,"delta phi(j1,j2)");
   
   
   // TTree chains and friendship?
   TChain * chainMeta  = new TChain("MonteCarloStudies/Metadata");
   
   // Events
   TChain * chainEvent   = new TChain("MonteCarloStudies/EventInfo");
   TChain * chainJets    = new TChain("MonteCarloStudies/slimmedJetsPuppi");
   TChain * chainMuons   = new TChain("MonteCarloStudies/slimmedMuons");
   TChain * chainTrigger = new TChain("MonteCarloStudies/TriggerResults_HLT");
   
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
   // branches
   chainMeta->SetBranchAddress( "xsection", &xsection_ );
   chainMeta->SetBranchAddress( "nGenEventsTotal", &nGenTotal_ );
   chainMeta->SetBranchAddress( "nGenEventsFiltered", &nGenFilt_ );
   
   // Number of metadata entries (should be one per file)
   int nMeta = chainMeta->GetEntries();
   int nGenTotal = 0;
   int nGenFilt  = 0;
   for ( int i = 0 ; i < nMeta ; ++i )
   {
      chainMeta->GetEntry(i);
      nGenTotal += nGenTotal_;
      nGenFilt  += nGenFilt_;
   }
   
   chainEvent->AddFileInfoList((TCollection*) fc.GetList());
   // Branches
   // Event Info
   chainEvent->SetBranchAddress( "run"         , &run_ );
   chainEvent->SetBranchAddress( "lumisection" , &lumi_ );
   chainEvent->SetBranchAddress( "event"       , &event_ );
//    
   // Don't forget that the trees must be friends!!!
   chainEvent -> AddFriend(chainJets);
   chainJets  -> AddFileInfoList((TCollection*) fc.GetList());
   chainJets  -> SetBranchAddress( "n"      , &jetN_     );
   chainJets  -> SetBranchAddress( "pt"     ,  jetPt_    );
   chainJets  -> SetBranchAddress( "eta"    ,  jetEta_   );
   chainJets  -> SetBranchAddress( "phi"    ,  jetPhi_   );
   chainJets  -> SetBranchAddress( "e"      ,  jetE_     );
   chainJets  -> SetBranchAddress( "btag"   ,  jetBtag_  );
   chainJets  -> SetBranchAddress( "flavour",  jetFlav_  );
   
   // Don't forget that the trees must be friends!!!
   chainEvent -> AddFriend(chainMuons);
   chainMuons -> AddFileInfoList((TCollection*) fc.GetList());
   chainMuons -> SetBranchAddress( "n"  , &muN_  );
   chainMuons -> SetBranchAddress( "pt" ,  muPt_ );
   chainMuons -> SetBranchAddress( "eta",  muEta_);
   chainMuons -> SetBranchAddress( "phi",  muPhi_);
   chainMuons -> SetBranchAddress( "e"  ,  muE_  );
   chainMuons -> SetBranchAddress( "q"  ,  muQ_  );
   
   // Don't forget that the trees must be friends!!!
   /*
   Trigger paths
      "HLT_BTagMu_DiJet20_Mu5_v1"
      "HLT_BTagMu_DiJet40_Mu5_v1"
      "HLT_BTagMu_DiJet70_Mu5_v1"
      "HLT_BTagMu_DiJet110_Mu5_v1"
      "HLT_BTagMu_Jet300_Mu5_v1"
   */
   chainEvent   -> AddFriend(chainTrigger);
   chainTrigger -> AddFileInfoList((TCollection*) fc.GetList());
   TObjArray * trgBranches = chainTrigger->GetListOfBranches();
   std::map<std::string, int> trgCounter;
   for ( int i = 0 ; i < trgBranches->GetEntries() ; ++i )
   {
      std::string path = trgBranches->At(i)->GetName();
      chainTrigger -> SetBranchAddress(path.c_str(), &trgAccepts_[path]);
      trgCounter[path] = 0;
   }
   
   std::string triggerPath_v;
   for ( auto& it : trgAccepts_ )
   {
      std::size_t found = it.first.find(triggerPath);
      if ( found != std::string::npos ) 
      {
         triggerPath_v = it.first;
         break;
      }
   }
   
   std::cout << triggerPath_v << std::endl;

   // Number of loop events
   int nEvents = chainEvent->GetEntries();
   
   int eventCounter = 0;
   int btagEmulationCounter = 0;
   int triggerCounter = 0;
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      ++eventCounter;
      
      chainEvent->GetEntry(i);
      
      h1["h_SelChain"] -> Fill("no cut",1);
      
      // trigger selection
      if ( ! trgAccepts_[triggerPath_v] ) continue;
      ++triggerCounter;
      
      h1["h_SelChain"] -> Fill("btagmu trigger",1);
      
      // rough emulation  of the btag leg using offline variables (for trigger rate/xsection evaluation)
      // it should not have any impact in the final selection statistics
      bool hasBJet = false;
      for ( int j = 0 ; j < jetN_ ; ++j )
      {
         if ( j > 5 ) break;
         if ( jetPt_[j] < 30. || fabs(jetEta_[j]) > 2.4 ) continue;
         if ( jetBtag_[j] > 0.9 ) 
         {
            hasBJet = true;
            break;
         }
      }
      if ( ! hasBJet ) continue;
      
      ++btagEmulationCounter;
      h1["h_SelChain"] -> Fill("online btag (emul)",1);
      
      // At least 2 jets, and at least one muon
      if ( jetN_ < 2 ) continue;
      if ( muN_ < 1 ) continue;
      
      // Interested in the two leading jets only
      TLorentzVector jet[2];
      jet[0].SetPtEtaPhiE(jetPt_[0], jetEta_[0], jetPhi_[0], jetE_[0]); 
      jet[1].SetPtEtaPhiE(jetPt_[1], jetEta_[1], jetPhi_[1], jetE_[1]);
      
      // Jet kinematics selection
      if ( jet[0].Pt() < 30. || fabs(jet[0].Eta()) > 2.4  ) continue;
      if ( jet[1].Pt() < 30. || fabs(jet[1].Eta()) > 2.4  ) continue;
      
      h1["h_SelChain"] -> Fill("dijet selection",1);

      // Tag and probe      
      TLorentzVector tagJet;
      float tagJetBtag;
      int tagJetFlavour;
      TLorentzVector probeJet;
      float probeJetBtag;
      int probeJetFlavour;
       
      // muon in a jet
      bool muInJet = false;
      for ( int m = 0; m < muN_ ; ++m )
      {
         TLorentzVector mu;
         mu.SetPtEtaPhiE(muPt_[m], muEta_[m], muPhi_[m], muE_[m]);
         if ( mu.Pt() < 5. || fabs(mu.Eta()) > 2.4 ) continue;
         // still need to check which jet was online b-tagged to assign the tag and probe labels
         
         if ( mu.DeltaR(jet[0]) < 0.5 ) 
         {
            tagJet = jet[0];
            tagJetBtag = jetBtag_[0];
            tagJetFlavour = jetFlav_[0];
            probeJet = jet[1];
            probeJetBtag = jetBtag_[1];
            probeJetFlavour = jetFlav_[1];
            muInJet = true;
            break;
         }
         if ( mu.DeltaR(jet[1]) < 0.5 ) 
         {
            tagJet = jet[1];
            tagJetBtag = jetBtag_[1];
            tagJetFlavour = jetFlav_[1];
            probeJet = jet[0];
            probeJetBtag = jetBtag_[0];
            probeJetFlavour = jetFlav_[0];
            muInJet = true;
            break;
         }
      }
      
      if ( ! muInJet ) continue;
      h1["h_SelChain"] -> Fill("muon in jet (tag jet)",1);

      // offline btagging
      // for the probe jet it must be the nominal btag wp
      if( probeJetBtag < 0.9 ) continue;
      // for the tag jet it can be tigher
      if( tagJetBtag < 0.95 ) continue;
      
      h1["h_SelChain"] -> Fill("offline 2 btags",1);
      
      // delta_phi cut
      float jetTPdPhi = fabs(tagJet.DeltaPhi(probeJet));
      if ( jetTPdPhi < 2.5 ) continue;
      
      h1["h_SelChain"] -> Fill("delta phi(j1,j2)",1);
      
      // Histograms
      h1["h_jetTagPt"] -> Fill(tagJet.Pt());      
      h1["h_jetProbePt"] -> Fill(probeJet.Pt());      
      h1["h_jetTPdPhi"] -> Fill(jetTPdPhi);
      
      h1["h_jetTagFlavour"] -> Fill(float(tagJetFlavour));
      h1["h_jetProbeFlavour"] -> Fill(float(probeJetFlavour));
      h1["h_jetTagBtag"] -> Fill(tagJetBtag);
      h1["h_jetProbeBtag"] -> Fill(probeJetBtag);
      
            
//       if ( i == 0 ) 
//       {
//          std::cout << "Triggers" << std::endl;
//          for ( auto& it : trgAccepts_ )
//          {
//             std::cout << it.first << "   " << it.second << std::endl;
//          }
//          std::cout << "Jets" << std::endl;
//          for ( int i = 0; i < jetN_ ; ++i )
//          {
//             std::cout << "    " << jetPt_[i] << ",  " <<  jetEta_[i] << ",  " <<jetPhi_[i] << " ,  " << jetFlav_[i] <<std::endl;
//          }
//          std::cout << "Muons" << std::endl;
//          for ( int i = 0; i < muN_ ; ++i )
//          {
//             std::cout << "    " << muPt_[i] << ",  " <<  muEta_[i] << ",  " << muPhi_[i] <<std::endl;
//          }
//       }
         
      
      
   } // END LOOP EVENTS
   std::cout << "Processed  " << eventCounter << "  events" << "\n";
    
   double genEff = nGenTotal > 0 ? (double) nGenFilt / (double) nGenTotal : 1.;
   xsection_ *= 1000; // convert from pb to fb
   double genXsection = xsection_ * genEff;
   
   double effLumi = eventCounter/genXsection;
   double scaleLumi   = lumi/effLumi;
   
   std::cout << "Cross section * generator efficiency  = " << xsection_ << " * " << genEff << " = " << genXsection << std::endl;
   std::cout << "Effective lumi  = " << effLumi   << std::endl;
   std::cout << "Number of events = " << nEvents << std::endl;
   std::cout << "Number of events filtered (gen) = " << nGenFilt << std::endl;
   std::cout << "Number of triggered events = " << triggerCounter << " events" << std::endl;
   std::cout << "Number of emulated btag events = " << btagEmulationCounter << " events" << std::endl;
    
   
      
//       // The trigger objects after online selection, ie all objects that pass the selection
//       std::vector<TLorentzVector> jets;
//       
//       // leading jets and preselection
//       jets.clear();
//       bool ok = true;
//       if ( jetN_[0] < 3 ) continue; // at least 3 jets in the event
//       for ( int j = 0 ; j < jetN_[0] ; ++j )
//       {
//          if ( j < 3 && fabs(jetEta_[0][j]) > 2.4 ) ok = false;  // 3 leading jets within the tracker acceptance
//          if ( jetPt_[0][2] < 30 ) ok = false;  // 3rd leading jet soft cut
//          TLorentzVector jet;
//          jet.SetPtEtaPhiE(jetPt_[0][j], jetEta_[0][j], jetPhi_[0][j], jetE_[0][j]); 
//          jets.push_back(jet);
//       }
//       // must have at least 3 jets
//       if ( jets.size() < 3 ) continue;
//       
//       // deltaR between 3 leading jets must be > 1
//       if ( jets[0].DeltaR(jets[1]) < 1. || jets[1].DeltaR(jets[2]) < 1. || jets[2].DeltaR(jets[0]) < 1. ) ok = false;
//       
//       // preselection
//       if ( ! ok ) continue;
//       
//       TLorentzVector dijet = jets[0] + jets[1];
//       
//       float dEta01 = fabs(jets[0].Eta() - jets[1].Eta());
//       float dEta12 = fabs(jets[1].Eta() - jets[2].Eta());
//       float dEta02 = fabs(jets[0].Eta() - jets[2].Eta());
//       
//       float dR01 = jets[0].DeltaR(jets[1]);
//       float dR12 = jets[1].DeltaR(jets[2]);
//       float dR02 = jets[0].DeltaR(jets[2]);
//       
//       float m12 = dijet.M();
//       
//       if ( !(      jets[0].Pt()   > ptcut [0]) )   continue;
//       if ( !( fabs(jets[0].Eta()) < etacut[0] ) )  continue;
//       if ( !(      jets[1].Pt()   > ptcut [1]) )   continue;
//       if ( !( fabs(jets[1].Eta()) < etacut[1] ) )  continue;
//       if ( !(      jets[2].Pt()   > ptcut [2] ) )  continue;
//       if ( !( fabs(jets[2].Eta()) < etacut[2] ) )  continue;
//       if ( detacut > 0 )
//          if ( dEta01 > detacut ) continue;
//       if ( mw > 0 && dmw > 0 )
//          if ( m12 < (mw-dmw) || m12 > (mw+dmw) ) continue;
//       
//       for ( size_t j = 0 ; j < jets.size() ; ++j )
//       {
//          if ( j > 3 ) break; // only up to 4 leading jets
//          h_PtJet[j]  -> Fill(jets[j].Pt());
//          h_EtaJet[j] -> Fill(jets[j].Eta());
//       }
//       h_Jet0Jet1_Pt  -> Fill(jets[0].Pt(),jets[1].Pt());
//       h_Jet0Jet1_Eta -> Fill(fabs(jets[0].Eta()),fabs(jets[1].Eta()));
//       h_MassJet01 -> Fill(m12);
//       h_dEta01 -> Fill(dEta01);
//       h_dEta12 -> Fill(dEta12);
//       h_dEta02 -> Fill(dEta02);
//       h_dR01 -> Fill(dR01);
//       h_dR12 -> Fill(dR12);
//       h_dR02 -> Fill(dR02);
//       h_selection -> Fill(1.5);
//    }
//    /// end of loop event
//    
//    
//    
//    std::ostringstream tmp;
//    tmp << "genjets_jpt" << int(ptcut[0]) << "_jpt" << int(ptcut[1]) << "_jpt" << int(ptcut[2]);
//    tmp << "_jeta" << etacut[0] << "_jeta" << etacut[1] << "_jeta" << etacut[2];
//    if ( detacut > 0 ) tmp << "_deta" << detacut;
//    if ( mw > 0 && dmw > 0  ) tmp << "_m12w" << mw << "pm" << dmw;
//    outFilename = TString(tmp.str());
//    outFilename.ReplaceAll(".","p");
//    
//   outFilename = "genjets_"+subfilename+".jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   outFilename = dataset + "-" + triggerPath;
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   
   for ( auto& ih1 : h1 )
   {
      ih1.second -> Scale(scaleLumi);
      ih1.second -> Write();
   }
   
   outFile -> Close();
//    // scale to luminosity; write to file
//    for ( size_t j = 0 ; j < 4 ; ++j )
//    {
//       h_PtJet[j]  -> Scale(scale);
//       h_EtaJet[j] -> Scale(scale);
//       h_PtJet[j]  -> Write();
//       h_EtaJet[j] -> Write();
//    }
//    h_Jet0Jet1_Pt  -> Scale(scale);
//    h_Jet0Jet1_Eta -> Scale(scale);
//    h_MassJet01    -> Scale(scale);
//    h_selection    -> Scale(scale);
//    
//    
//    h_Jet0Jet1_Pt  -> Write();
//    h_Jet0Jet1_Eta -> Write();
//    h_MassJet01    -> Write();
//    h_dEta01    -> Write();
//    h_dEta12    -> Write();
//    h_dEta02    -> Write();
//    h_dR01    -> Write();
//    h_dR12    -> Write();
//    h_dR02    -> Write();
//    
//    h_selection    -> Write();
//    
//    outFile -> Close();
    
//    
}

