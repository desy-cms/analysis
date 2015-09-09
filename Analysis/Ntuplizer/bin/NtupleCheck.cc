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

#include "Analysis/Ntuplizer/interface/Ntuple.h"

using namespace std;

int    nEvents    ;
string run        ;
string steer      ;
float  pi;

TString outFilename;

// bool myreplace(std::string& , const std::string& , const std::string& );

PatJet SetPatJet(const float & pt, 
              const float & eta, 
              const float & phi, 
              const float & e, 
              const float & btag, 
              const int   & flav, 
              const float & nHadFrac, 
              const float & nEmFrac, 
              const float & nMult, 
              const float & cHadFrac, 
              const float & cEmFrac, 
              const float & cMult, 
              const float & muonFrac ); 

int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
//   double lumi = 20.; // in fb-1

   // Input files
   std::string inputList = "rootFileList.txt"; 
   TFileCollection fc("dum","",inputList.c_str());
   
   
   // Cross Sections - print on the screen
   std::cout << "=======================================================" << std::endl;
   std::cout << "  CROSS SECTIONS" << std::endl;
   std::cout << "=======================================================" << std::endl;
   
   TChain * t_xsection  = new TChain("MonteCarloStudies/Metadata/CrossSections");
   t_xsection -> AddFileInfoList((TCollection*) fc.GetList());
   TObjArray * xsecBranches = t_xsection->GetListOfBranches();
   std::map<std::string, double> xsections;
   for ( int i = 0 ; i < xsecBranches->GetEntries() ; ++i )
   {
      std::string branch = xsecBranches->At(i)->GetName();
      if ( branch == "run" ) continue;
      xsections[branch] = 0;
      t_xsection -> SetBranchAddress(branch.c_str(), &xsections[branch]);
   }
   t_xsection -> GetEntry(0);
   for ( auto& xs : xsections )
   {
      std::cout << xs.first << " = " << xs.second << " pb " << std::endl;
   }
   
   // For the histograms normalisation
   xsection_ = xsections["myCrossSection"];
   if ( xsection_ < 0. ) xsection_ = 1.;
   
   
   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   
   // Generator filter - print events and efficiency on the screen
   std::cout << "=======================================================" << std::endl;
   std::cout << "  GENERATOR FILTER" << std::endl;
   std::cout << "=======================================================" << std::endl;
   
   TChain * t_genfilter  = new TChain("MonteCarloStudies/Metadata/GeneratorFilter");
   t_genfilter -> AddFileInfoList((TCollection*) fc.GetList());
   unsigned int nEvtTot;
   unsigned int nEvtFlt;
   unsigned int sEvtTot = 0;
   unsigned int sEvtFlt = 0;
      
   t_genfilter -> SetBranchAddress("nEventsTotal", &nEvtTot);
   t_genfilter -> SetBranchAddress("nEventsFiltered", &nEvtFlt);
   
   for ( int i = 0; i < t_genfilter->GetEntries(); ++i )
   {
      t_genfilter -> GetEntry(i);
      sEvtTot += nEvtTot;
      sEvtFlt += nEvtFlt;
   }
   float genFilterEff = double(sEvtFlt)/sEvtTot;
   
   std::cout << "Total generated events = " << sEvtTot << std::endl;
   std::cout << "Filtered generated events = " << sEvtFlt << std::endl;
   std::cout << "Generator Filter Efficiency = " << genFilterEff << std::endl;
   
   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
   
      
// =============================================================================================  
   
   // HISTOGRAMS
    
   std::map<std::string, TH1F*> h1;
   
   // JETS
   h1["h_patjet_N"]   = new TH1F("h_patjet_N" , "", 20, 0., 20.);
   h1["h_patjet_Pt"]   = new TH1F("h_patjet_Pt" , "", 200, 0., 1000.);
   h1["h_patjet_Eta"]  = new TH1F("h_patjet_Eta", "", 100, -5, 5.);
   h1["h_patjet_Btag"]  = new TH1F("h_patjet_Btag", "", 100, 0., 1.);
   h1["h_patjet_Flavour"]  = new TH1F("h_patjet_Flavour", "", 40, -10., 30.);
   h1["h_patjet_LooseId"]  = new TH1F("h_patjet_LooseId", "", 2, 0, 2);
   h1["h_patjet_TightId"]  = new TH1F("h_patjet_TightId", "", 2, 0, 2);

   // ranked jets
   for ( int i = 0 ; i < 4 ; ++i )
   {
      h1[Form("h_patjet_%i_Pt",i)]       = new TH1F(Form("h_patjet_%i_Pt",i) ,  "", 200, 0., 1000.);
      h1[Form("h_patjet_%i_Eta",i)]      = new TH1F(Form("h_patjet_%i_Eta",i),  "", 100, -5, 5.);
      h1[Form("h_patjet_%i_Btag",i)]     = new TH1F(Form("h_patjet_%i_Btag",i), "", 100, 0, 1.);
      h1[Form("h_patjet_%i_Flavour",i)]  = new TH1F(Form("h_patjet_%i_Flavour",i), "", 40, -10, 30);
   }
   // flavour jets
   std::string flavour[3] = {"udsg","c","b"};
//    for ( int i = 0 ; i < 3 ; ++i )
//    {
//       h1[Form("h_patjet_%s_Pt",flavour[i])]       = new TH1F(Form("h_patjet_%s_Pt",flavour[i])  ,  "", 200, 0., 1000.);
//       h1[Form("h_patjet_%s_Eta",flavour[i])]      = new TH1F(Form("h_patjet_%s_Eta",flavour[i]) ,  "", 100, -5, 5.);
//       h1[Form("h_patjet_%s_Btag",flavour[i])]     = new TH1F(Form("h_patjet_%s_Btag",flavour[i]), "", 100, 0, 1.);
//       h1[Form("h_patjet_%s_Flavour",flavour[i])]  = new TH1F(Form("h_patjet_%s_Flav",flavour[i]), "", 40, -10, 30);
//    }
      
// =============================================================================================   
   
   // Events
   TChain * t_Event   = new TChain("MonteCarloStudies/Events/EventInfo");
   TChain * t_Jets    = new TChain("MonteCarloStudies/Events/slimmedJetsPuppi");
//   TChain * t_Muons   = new TChain("MonteCarloStudies/slimmedMuons");
//   TChain * t_Trigger = new TChain("MonteCarloStudies/TriggerResults");
   
   t_Event->AddFileInfoList((TCollection*) fc.GetList());
   // Branches
   // Event Info
   t_Event->SetBranchAddress( "run"         , &run_ );
   t_Event->SetBranchAddress( "lumisection" , &lumi_ );
   t_Event->SetBranchAddress( "event"       , &event_ );
//    
   // Don't forget that the trees must be friends!!!
   t_Event -> AddFriend(t_Jets);
   t_Jets  -> AddFileInfoList((TCollection*) fc.GetList());
   
   t_Jets  -> SetBranchAddress( "n"      ,    &jetN_     );
   t_Jets  -> SetBranchAddress( "pt"     ,     jetPt_    );
   t_Jets  -> SetBranchAddress( "eta"    ,     jetEta_   );
   t_Jets  -> SetBranchAddress( "phi"    ,     jetPhi_   );
   t_Jets  -> SetBranchAddress( "e"      ,     jetE_     );
   t_Jets  -> SetBranchAddress( "btag_csvivf", jetBtag_  );
//   t_Jets  -> SetBranchAddress( "flavour",     jetFlav_  );
   t_Jets  -> SetBranchAddress( "id_nHadFrac", jetNHadFrac_  );
   t_Jets  -> SetBranchAddress( "id_nEmFrac",  jetNEmFrac_   );
   t_Jets  -> SetBranchAddress( "id_nMult",    jetNMult_     );
   t_Jets  -> SetBranchAddress( "id_cHadFrac", jetCHadFrac_  );
   t_Jets  -> SetBranchAddress( "id_cEmFrac",  jetCEmFrac_   );
   t_Jets  -> SetBranchAddress( "id_cMult",    jetCMult_     );
   t_Jets  -> SetBranchAddress( "id_muonFrac", jetMuFrac_    );
   
   
   TObjArray * jetBranches = t_Jets->GetListOfBranches();
   for ( int i = 0 ; i < jetBranches->GetEntries() ; ++i )
   {
      std::string branch = jetBranches->At(i)->GetName();
      std::string title  = jetBranches->At(i)->GetTitle();
      if ( branch == "n" ) continue;
      bool isInt = (title.find("/I") != std::string::npos);
//      bool isFloat = (title.find("/F") != std::string::npos);
       if ( isInt )
       {
           std::cout << branch << ", " << title << ", " << iPatJets_[branch] << std::endl;
           t_Jets -> SetBranchAddress(branch.c_str(), iPatJets_[branch]);
       }
//          t_Jets -> SetBranchAddress(branch.c_str(), iPatJets_[branch]);
//       fPatJets[branch] = 0;
//       iPatJets[branch] = 0;
//       t_Jets -> SetBranchAddress(branch.c_str(), &xsections[branch]);
   }

   
   // Don't forget that the trees must be friends!!!
//    t_Event -> AddFriend(t_Muons);
//    t_Muons -> AddFileInfoList((TCollection*) fc.GetList());
//    t_Muons -> SetBranchAddress( "n"  , &muN_  );
//    t_Muons -> SetBranchAddress( "pt" ,  muPt_ );
//    t_Muons -> SetBranchAddress( "eta",  muEta_);
//    t_Muons -> SetBranchAddress( "phi",  muPhi_);
//    t_Muons -> SetBranchAddress( "e"  ,  muE_  );
//    t_Muons -> SetBranchAddress( "q"  ,  muQ_  );
   
   // Don't forget that the trees must be friends!!!
   /*
   Trigger paths
      "HLT_BTagMu_DiJet20_Mu5_v1"
      "HLT_BTagMu_DiJet40_Mu5_v1"
      "HLT_BTagMu_DiJet70_Mu5_v1"
      "HLT_BTagMu_DiJet110_Mu5_v1"
      "HLT_BTagMu_Jet300_Mu5_v1"
   */
//    t_Event   -> AddFriend(t_Trigger);
//    t_Trigger -> AddFileInfoList((TCollection*) fc.GetList());
//    TObjArray * trgBranches = t_Trigger->GetListOfBranches();
//    std::map<std::string, int> trgCounter;
//    for ( int i = 0 ; i < trgBranches->GetEntries() ; ++i )
//    {
//       std::string path = trgBranches->At(i)->GetName();
//       t_Trigger -> SetBranchAddress(path.c_str(), &trgAccepts_[path]);
//       trgCounter[path] = 0;
//    }
//    
//    std::string triggerPath_v;
//    for ( auto& it : trgAccepts_ )
//    {
//       std::size_t found = it.first.find(triggerPath);
//       if ( found != std::string::npos ) 
//       {
//          triggerPath_v = it.first;
//          break;
//       }
//    }
   
   // Number of loop events
   int nEvents = t_Event->GetEntries();
   
   int eventCounter = 0;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
//      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      ++eventCounter;
      
      t_Event->GetEntry(i);
      
      h1["h_patjet_N"]  -> Fill(jetN_);
      
      std::vector<PatJet> jets;
      
      for ( int j = 0 ; j < jetN_; ++j )
      {
         int * flv = iPatJets_["flavour"][j];
         if ( i < 5 )
         std::cout << jetN_ << ", " <<  flv << std::endl;
         
         PatJet jet = SetPatJet(jetPt_[j],jetEta_[j],jetPhi_[j],jetE_[j],
                                jetBtag_[j],jetFlav_[j],
                                jetNHadFrac_[j], jetNEmFrac_[j], jetNMult_[j],
                                jetCHadFrac_[j], jetCEmFrac_[j], jetCMult_[j],
                                jetMuFrac_[j] );
         jets.push_back(jet);
         
         h1["h_patjet_Pt"] -> Fill(jet.p.Pt());
         h1["h_patjet_Eta"] -> Fill(jet.p.Eta());
         h1["h_patjet_Btag"] -> Fill(jet.btag);
         h1["h_patjet_Flavour"] -> Fill(jet.flavour);
         h1["h_patjet_LooseId"] -> Fill(jet.looseId);
         h1["h_patjet_TightId"] -> Fill(jet.tightId);
         
         if ( j > 3 ) continue;  // Meant for up to 4 leading jets
         h1[Form("h_patjet_%i_Pt",j)]->Fill(jet.p.Pt());
         h1[Form("h_patjet_%i_Eta",j)]->Fill(jet.p.Eta());
         h1[Form("h_patjet_%i_Btag",j)]->Fill(jet.btag);
         h1[Form("h_patjet_%i_Flavour",j)]->Fill(jet.flavour);
         
      }
      
      
            
   } // END LOOP EVENTS
   std::cout << "Processed  " << eventCounter << "  events" << "\n";
    
   TFile * outFile = new TFile("NtupleCheckOut.root","RECREATE");
   outFile -> mkdir("PatJets","PatJets");
   outFile -> mkdir("L1Jets","L1Jets");
   outFile -> mkdir("HltJets","HltJets");
   outFile -> mkdir("PatMuons","PatMuons");
   outFile -> mkdir("TriggerResults","TriggerResults");
   
   for ( auto& ih1 : h1 )
   {
      if ( ih1.first.find("h_patjet_") != std::string::npos )
         outFile -> cd("PatJets");
      ih1.second -> Write();
   }
   outFile -> Close();
//    
}

PatJet SetPatJet(const float & pt, 
                 const float & eta, 
                 const float & phi, 
                 const float & e, 
                 const float & btag, 
                 const int   & flav, 
                 const float & nHadFrac, 
                 const float & nEmFrac, 
                 const float & nMult, 
                 const float & cHadFrac, 
                 const float & cEmFrac, 
                 const float & cMult, 
                 const float & muonFrac )
{
   PatJet jet;
   jet.p.SetPtEtaPhiE(pt,eta,phi,e);
   jet.btag = btag;
   jet.flavour = flav;
   int nM = (int)round(nMult);
   int cM = (int)round(cMult);
   int numConst = nM + cM;
   if ( fabs(eta) <= 3. )
   {
      jet.looseId = ((nHadFrac<0.99 && nEmFrac<0.99 && numConst>1) && ((abs(eta)<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || abs(eta)>2.4) && abs(eta)<=3.0);
      jet.tightId = ((nHadFrac<0.90 && nEmFrac<0.90 && numConst>1) && ((abs(eta)<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || abs(eta)>2.4) && abs(eta)<=3.0);
   }
   else
   {
      jet.looseId = (nEmFrac<0.90 && nM>10 && abs(eta)>3.0);
      jet.tightId = (nEmFrac<0.90 && nM>10 && abs(eta)>3.0);
   }
   return jet;
   
} 

