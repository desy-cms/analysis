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
   
   lumi = 20.; // in fb-1
   
   std::map<std::string, TH1F*> h1;
   
   std::string triggerPath = "HLT_BTagMu_DiJet20_Mu5";
   std::string dataset = "30to50";
   std::string type = "data";
   std::string ptbin = "nobin";
   
   if ( argc == 2 )
   {
      dataset = argv[1];
   }
   else if ( argc == 3 ) 
   {
      dataset = argv[1];
      triggerPath = argv[2];
   }
   else if ( argc == 4 ) 
   {
      dataset = argv[1];
      triggerPath = argv[2];
      type = argv[3];
   }
   else if ( argc == 5 ) 
   {
      dataset = argv[1];
      triggerPath = argv[2];
      type = argv[3];
      ptbin = argv[4];
   }
   
   h1["h_jetTagPt"]  = new TH1F("h_jetTagPt", "", 100, 0., 500.);
   h1["h_jetProbePt"]= new TH1F("h_jetProbePt", "", 100, 0., 500.);
   h1["h_jetTPdPhi"] = new TH1F("h_dPhiJetTP", "", 64, 0., 3.2);
   
   h1["h_jetTagFlavour"]  = new TH1F("h_jetTagFlavour", "", 35, -10, 25.);
   h1["h_jetProbeFlavour"]= new TH1F("h_jetProbeFlavour", "", 35, -10., 25.);
   
   h1["h_jetTagBtag"]  = new TH1F("h_jetTagBtag", "", 100, 0., 1.);
   h1["h_jetProbeBtag"]= new TH1F("h_jetProbeBtag", "", 100, 0., 1.);
   
   h1["h_SelChain"] = new TH1F("h_SelChain", "", 8, 0., 8.);
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(1,"no cut");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(2,"btagmu trigger");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(3,"online btag (emul)");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(4,"dijet selection");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(5,"muon in jet (tag jet)");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(6,"offline 2 btags");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(7,"delta phi(j1,j2)");
   h1["h_SelChain"] -> GetXaxis() -> SetBinLabel(8,"prescale");
   
   
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
   
   std::map<std::string, float> ps; 
   ps["HLT_BTagMu_DiJet20_Mu5"]  = 400;
   ps["HLT_BTagMu_DiJet40_Mu5"]  = 200;
   ps["HLT_BTagMu_DiJet70_Mu5"]  = 40;
   ps["HLT_BTagMu_DiJet110_Mu5"] = 10;
   ps["HLT_BTagMu_Jet300_Mu5"]   = 1;
   if ( type == "mc" )
   {
      ps["HLT_BTagMu_DiJet20_Mu5"]  = 1;
      ps["HLT_BTagMu_DiJet40_Mu5"]  = 1;
      ps["HLT_BTagMu_DiJet70_Mu5"]  = 1;
      ps["HLT_BTagMu_DiJet110_Mu5"] = 1;
      ps["HLT_BTagMu_Jet300_Mu5"]   = 1;
   }

   std::map<std::string, float> jetptmin; 
   jetptmin["HLT_BTagMu_DiJet20_Mu5"]  = 30;
   jetptmin["HLT_BTagMu_DiJet40_Mu5"]  = 30;
   jetptmin["HLT_BTagMu_DiJet70_Mu5"]  = 30;
   jetptmin["HLT_BTagMu_DiJet110_Mu5"] = 30;
   jetptmin["HLT_BTagMu_Jet300_Mu5"]   = 30;

   std::map<std::string, float> jetptmax; 
   jetptmax["HLT_BTagMu_DiJet20_Mu5"]  = 999999.; 
   jetptmax["HLT_BTagMu_DiJet40_Mu5"]  = 999999.; 
   jetptmax["HLT_BTagMu_DiJet70_Mu5"]  = 999999.; 
   jetptmax["HLT_BTagMu_DiJet110_Mu5"] = 999999.; 
   jetptmax["HLT_BTagMu_Jet300_Mu5"]   = 999999.; 

   if ( ptbin != "nobin" )
   {
      jetptmin["HLT_BTagMu_DiJet20_Mu5"]  = 30;    
      jetptmin["HLT_BTagMu_DiJet40_Mu5"]  = 50;    
      jetptmin["HLT_BTagMu_DiJet70_Mu5"]  = 70;    
      jetptmin["HLT_BTagMu_DiJet110_Mu5"] = 120;   
      jetptmin["HLT_BTagMu_Jet300_Mu5"]   = 300;   
      jetptmax["HLT_BTagMu_DiJet20_Mu5"]  = 50.;   
      jetptmax["HLT_BTagMu_DiJet40_Mu5"]  = 70.;   
      jetptmax["HLT_BTagMu_DiJet70_Mu5"]  = 120.;  
      jetptmax["HLT_BTagMu_DiJet110_Mu5"] = 300.;  
      jetptmax["HLT_BTagMu_Jet300_Mu5"]   = 999999.;
   }
   
   std::cout << triggerPath_v << std::endl;

   // Number of loop events
   int nEvents = chainEvent->GetEntries();
   
   int eventCounter = 0;
   int btagEmulationCounter = 0;
   int triggerCounter = 0;
   float weight;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      ++eventCounter;
      
      chainEvent->GetEntry(i);
      
      weight = 1.;
      
      h1["h_SelChain"] -> Fill("no cut",weight);
      
      // trigger selection
      if ( ! trgAccepts_[triggerPath_v] ) continue;
      ++triggerCounter;
      
      h1["h_SelChain"] -> Fill("btagmu trigger",weight);
      
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
      weight *= 0.5;
      h1["h_SelChain"] -> Fill("online btag (emul)",weight);
      
      // At least 2 jets, and at least one muon
      if ( jetN_ < 2 ) continue;
      if ( muN_ < 1 ) continue;
      
      // Interested in the two leading jets only
      TLorentzVector jet[2];
      jet[0].SetPtEtaPhiE(jetPt_[0], jetEta_[0], jetPhi_[0], jetE_[0]); 
      jet[1].SetPtEtaPhiE(jetPt_[1], jetEta_[1], jetPhi_[1], jetE_[1]);
      
      // Jet kinematics selection
      if ( jet[0].Pt() < jetptmin[triggerPath] || jet[0].Pt() > jetptmax[triggerPath] || fabs(jet[0].Eta()) > 2.4 ) continue;
      if ( jet[1].Pt() < jetptmin[triggerPath] || jet[1].Pt() > jetptmax[triggerPath] || fabs(jet[1].Eta()) > 2.4 ) continue;
      
      h1["h_SelChain"] -> Fill("dijet selection",weight);

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
      h1["h_SelChain"] -> Fill("muon in jet (tag jet)",weight);

      // offline btagging
      // for the probe jet it must be the nominal btag wp
      if( probeJetBtag < 0.9 ) continue;
      // for the tag jet it can be tigher
      if( tagJetBtag < 0.95 ) continue;
      
      h1["h_SelChain"] -> Fill("offline 2 btags",weight);
      
      // delta_phi cut
      float jetTPdPhi = fabs(tagJet.DeltaPhi(probeJet));
      if ( jetTPdPhi < 2.5 ) continue;
      
      h1["h_SelChain"] -> Fill("delta phi(j1,j2)",weight);
      
      
      weight /= ps[triggerPath];
      h1["h_SelChain"] -> Fill("prescale",weight);
      
      
      
      // Histograms
      h1["h_jetTagPt"] -> Fill(tagJet.Pt());      
      h1["h_jetProbePt"] -> Fill(probeJet.Pt());      
      h1["h_jetTPdPhi"] -> Fill(jetTPdPhi);
      
      h1["h_jetTagFlavour"] -> Fill(float(tagJetFlavour));
      h1["h_jetProbeFlavour"] -> Fill(float(probeJetFlavour));
      h1["h_jetTagBtag"] -> Fill(tagJetBtag);
      h1["h_jetProbeBtag"] -> Fill(probeJetBtag);
      
            
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
    
   if ( type == "mc" )
      scaleLumi = 1.;
      
   outFilename = type + "/";
   if ( ptbin == "nobin" )
      outFilename += "ptMin/";
   else
      outFilename += "ptMinMax/";
   
   outFilename += "QCD_Pt_"+ dataset + "_Mu_Pt5" + "-" + triggerPath;
   TFile * outFile = new TFile(outFilename+".root","RECREATE");
   
   for ( auto& ih1 : h1 )
   {
      if ( ih1.first == "h_SelChain" )
         ih1.second -> Scale(scaleLumi);
      else
         ih1.second -> Scale(scaleLumi*0.5/ps[triggerPath]);
      ih1.second -> Write();
   }
   
   outFile -> Close();
//    
}

