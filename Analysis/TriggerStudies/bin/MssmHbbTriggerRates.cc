#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 
#include "TGraph.h" 
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TVectorD.h"

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


std::map<std::string, TH1F*> h1_;
TGraphAsymmErrors * g_eff_;
TGraphErrors * g_rates_;
std::string inputList_;
std::string basename_;

std::map<std::string, double> xsections_;

std::string hltPath_[20];

std::map<std::string, double> CrossSections();
bool L1DoubleJetC100(const Analysis & );

// =============================================================================================   
int main(int argc, char * argv[])
{
   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

   xsections_ = CrossSections();

   h1_["nSelected"] = new TH1F("h_nSelected","",35,27.5,62.5);
   h1_["nTotal"]    = new TH1F("h_nTotal","",35,27.5,62.5);
   
   
   // Input files list
   inputList_ = "rootFileList.txt";
   if ( argc == 2 )
   {
      inputList_ = std::string(argv[1]);
      basename_ =  std::string(boost::filesystem::basename(inputList_));
      std::cout << basename_ << std::endl;
   }
   
   Analysis analysis(inputList_,"MssmHbbTrigger/Events/EventInfo");
   
   // Physics Objects Collections
   analysis.triggerResults("MssmHbbTrigger/Events/TriggerResults");
   hltPath_[0] = "HLT_ZeroBias_v";
   hltPath_[1] = "HLT_L1DoubleJetC100_v";
//   hltPath[1] = "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v";
//   hltPath[1] = "HLT_L1SingleJet20_v1";
   
   // Trigger objects
   std::vector<std::string> jetTriggerObjects;
   jetTriggerObjects.push_back("hltL1sDoubleJetC100");
//    jetTriggerObjects.push_back("hltDoubleJetsC100");
//    jetTriggerObjects.push_back("hltBTagCaloCSVp014DoubleWithMatching");
//    jetTriggerObjects.push_back("hltDoublePFJetsC100");
//    jetTriggerObjects.push_back("hltDoublePFJetsC100MaxDeta1p6");
   
//   jetTriggerObjects.push_back("hltL1sSingleJet20");
   
   std::string trgobj_path = "MssmHbbTrigger/Events/hltTriggerSummaryAOD/";
   for ( auto & obj : jetTriggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Get generator filter information
//   FilterResults genFilter = analysis.generatorFilter("MssmHbbTrigger/Metadata/GeneratorFilter");  
   
   // Get cross section
   analysis.crossSections("MssmHbbTrigger/Metadata/CrossSections");
   
   double crossSection = xsections_[basename_];
   
   std::cout << crossSection << std::endl;
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   
//   float genFilterEfficiency = genFilter.efficiency;
   for ( int i = 0 ; i < analysis.size() ; ++i )
//   for ( int i = 0 ; i < 300000 ; ++i )
   {
      analysis.event(i);
      
      if ( i % 100000 == 0 && i>0 )
      {
         std::cout << i << " events analysed" << std::endl;
      }
      
      // Trigger results: fired and prescales
      // hltPath0 - reference trigger
      if ( ! analysis.triggerResult(hltPath_[0]) ) continue;
      
      h1_["nTotal"] -> Fill(analysis.nTruePileup());
      
      int trg_fired = analysis.triggerResult(hltPath_[1]);
      
      if ( ! trg_fired ) continue;
      
      // FOR L1 TRIGGER
      auto l1dijet100 = analysis.collection<TriggerObject>("hltL1sDoubleJetC100");
      if ( l1dijet100->size() < 2 ) continue;
      
      std::vector<TriggerObject> l1dijetc100v2;
      for ( int j = 0 ; j < l1dijet100->size() ; ++j )
      {
         TriggerObject jet = l1dijet100->at(j);
         if ( jet.pt() < 100. || fabs(jet.eta()) > 2.3 ) continue;
         l1dijetc100v2.push_back(jet);
      }
      
      if ( l1dijetc100v2.size() < 2 ) continue;
      
      bool deta = false;
      for ( int j1 = 0 ; j1 < (int)l1dijetc100v2.size()-1 ; ++j1 )
      {
         for ( int j2 = j1+1 ; j2 < (int)l1dijetc100v2.size() ; ++j2 )
         {
            float eta1 = l1dijetc100v2.at(j1).eta();
            float eta2 = l1dijetc100v2.at(j2).eta();
            if ( fabs(eta1-eta2) <= 1.6 ) deta = true;
         }
      }
      
      if ( ! deta ) continue;
      
      
      // All hadronic path
      
//       // L1 
//       auto l1dijetc100 = analysis.collection<TriggerObject>("hltL1sDoubleJetC100");
//       std::vector<TriggerObject> l1dijet100eta2p3;
//       for ( int j = 0 ; j < l1dijetc100->size() ; ++j )
//       {
//          TriggerObject jet = l1dijetc100->at(j);
//          if ( fabs(jet.eta()) > 2.3 ) continue;
//          l1dijet100eta2p3.push_back(jet);
//       }
//       
//       if ( l1dijet100eta2p3.size() < 2 ) continue;
//       
//       bool deta = false;
//       for ( int j1 = 0 ; j1 < (int)l1dijet100eta2p3.size()-1 ; ++j1 )
//       {
//          for ( int j2 = j1+1 ; j2 < (int)l1dijet100eta2p3.size() ; ++j2 )
//          {
//             float eta1 = l1dijet100eta2p3.at(j1).eta();
//             float eta2 = l1dijet100eta2p3.at(j2).eta();
//             if ( fabs(eta1-eta2) <= 1.6 ) deta = true;
//          }
//       }
//       if ( ! deta ) continue;
//       
//       // online b-tagging trigger objects
//       // at most 6 jets with pT > 80 GeV
//       auto hlt8bJets30 = analysis.collection<TriggerObject>("hltBTagCaloCSVp014DoubleWithMatching");
//       std::vector<TriggerObject> hlt6bJets80;
//       for ( int j = 0 ; j < hlt8bJets30->size() ; ++j )
//       {
//          if ( j == 6 ) break; // at most 6 jets
//          TriggerObject jet = hlt8bJets30->at(j);
//          if ( jet.pt() < 80. ) continue;
//          hlt6bJets80.push_back(jet);
//       }
//       
//       if ( hlt6bJets80.size() < 2 ) continue;
//       
//       auto hltdipf100 = analysis.collection<TriggerObject>("hltDoublePFJetsC100");
//       std::vector<TriggerObject> hltdipf100eta2p3;
//       for ( int j = 0 ; j < hltdipf100->size() ; ++j )
//       {
//          TriggerObject jet = hltdipf100->at(j);
//          if ( fabs(jet.eta()) > 2.3 ) continue;
//          hltdipf100eta2p3.push_back(jet);
//       }
//       
//       if ( hltdipf100eta2p3.size() < 2 ) continue;
      
//       // muon selection at L1
//       auto l1Mu3s = analysis.collection<TriggerObject>("hltL1sSingleMu3");
//       
//       std::vector<TriggerObject> selL1Mu10s;
//       for ( int m = 0 ; m < l1Mu3s->size() ; ++m )
//       {
//          TriggerObject l1Mu3 = l1Mu3s->at(m);
//          if ( l1Mu3.pt() >= 10 && fabs(l1Mu3.eta()) <= 2.2 ) 
//          {
//             selL1Mu10s.push_back(l1Mu3);
// //         std::cout << l1Mu3.pt() << std::endl;
//          }
//          
//       }
//       if ( selL1Mu10s.size() < 1 ) continue;
//       
//       // jet selection at L1
//       auto l1Jet20s = analysis.collection<TriggerObject>("hltL1sSingleJet20");
//       std::vector<TriggerObject> selL1Jet30s;
//       for ( int j = 0 ; j < l1Jet20s->size() ; ++j )
//       {
//          TriggerObject l1Jet20 = l1Jet20s->at(j);
//          if ( l1Jet20.pt() >= 30 && fabs(l1Jet20.eta()) <= 2.2 ) 
//          {
//             selL1Jet30s.push_back(l1Jet20);
//          }
//       }
//       std::cout << selL1Jet30s.size() <<std::endl;
//       
//       if ( selL1Jet30s.size() < 2 ) continue;
      
      h1_["nSelected"] -> Fill(analysis.nTruePileup());

      
   }
   g_eff_ = new TGraphAsymmErrors(h1_["nSelected"],h1_["nTotal"],"cl=0.683 b(1,1) mode");
   g_eff_ -> SetName("TriggerEfficiency");
   
   double fbx = (2200./3564.)/25.E-9;
   double * pu      = g_eff_->GetX();
   double * eff     = g_eff_->GetY();
   double rate[35];
   double puErr[35];
   double effErr[35];
   double rateErr[35];
   for ( int i = 0; i < g_eff_ -> GetN() ; ++i )
   {
      double mu_i = crossSection*1E-9/80.*pu[i];
      puErr[i] = 0;
      effErr[i] = g_eff_->GetErrorYhigh(i);
      rate[i] = fbx*(1-exp(-mu_i*eff[i]));
      rateErr[i] = fbx*mu_i*exp(-mu_i*eff[i])*effErr[i];
      std::cout << "Pile up = " << pu[i] << "   rate = " << rate[i] << " +- " << rateErr[i] << std::endl;
   }
   
   g_rates_ = new TGraphErrors(35,pu,rate,puErr,rateErr);
   g_rates_ -> SetName("rates");
   
   TFile * f_out = new TFile(Form("xxxl1rates_mod_%s.root",basename_.c_str()),"RECREATE");
   for ( auto & h : h1_ )
   {
      std::cout << h.first << std::endl;
      h.second->Write();
   }
   g_rates_ -> Write();
   g_eff_ -> Write();
   
   TVectorD xsection(1);
   xsection[0] = crossSection;
   xsection.Write("xsection");
   
   TNamed sampleName("SampleName",basename_.c_str());
   sampleName.Write();
   
   f_out -> Close();
   
//    
}


bool L1DoubleJetC100(const Analysis & analysis)
{
   return true;
}

std::map<std::string, double> CrossSections()
{
   std::map<std::string, double> xsections;
   xsections["QCD_Pt_15to30"]   = 1.83741e+09;
   xsections["QCD_Pt_30to50"]   = 1.40932e+08;
   xsections["QCD_Pt_50to80"]   = 1.92043e+07;
   xsections["QCD_Pt_80to120"]  = 2.76253e+06;
   xsections["QCD_Pt_120to170"] = 471100;
   xsections["QCD_Pt_170to300"] = 117276;
   xsections["QCD_Pt_300to470"] = 7823.28;
   xsections["QCD_Pt_470to600"] = 648.174;
   return xsections;
}


// TRIGGER PATHS
//                                   'HLT_L1SingleJet20_v1',
//                                   'HLT_L1SingleMu3_v1',
//                                   'HLT_Mu5_v1',
//                                   'HLT_PFJet20_v1',
//                                   'HLT_PFJet40_v1',
//                                   'HLT_PFJet60_v1',
//                                   'HLT_DoublePFJet20_v1',
//                                   'HLT_DiPFJetAve40_v1',
//                                   'HLT_DiPFJetAve60_v1',
//                                   'HLT_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30_v1',
//                                   'HLT_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30MaxDeta1p6_v1',
//                                   'HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v1',
//                                   'HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v1',
//                                   'HLT_DoubleJetsC100_SingleBTagCSV_p014_v1',
//                                   'HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v1',
//                                   'HLT_BTagMu_DiJet20_Mu5_v1',
//                                   'HLT_BTagMu_DiJet20_Mu5_DoubleBTagCSV_p014_DoublePFJet20_v1',
//                                   'HLT_BTagMu_DiJet20_Mu5_DoubleBTagCSV_p014_DoublePFJet20MaxDeta1p6_v1',
//                                   'HLT_L1MuJet_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30_v1',
//                                   'HLT_L1MuJet_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30MaxDeta1p6_v1',



// HLT ConfDB table name = /users/rwalsh/dev/CMSSW_8_0_X/MssmHbb/V15
// GlobalTag = 80X_mcRun2_asymptotic_v20
// ========================================
// 
// L1RePack_step
// ----------------------------------------
// 
// HLTriggerFirstPath
// ----------------------------------------
// 
// HLT_L1SingleJet20_v1
//    L1T Seed: L1_SingleJet20,
//            Trigger Object:               'hltL1sSingleJet20',
// ----------------------------------------
// 
// HLT_L1SingleMu3_v1
//    L1T Seed: L1_SingleMu3,
//            Trigger Object:               'hltL1sSingleMu3',
//            Trigger Object:               'hltL1fL1sMu3L1Filtered0',
// ----------------------------------------
// 
// HLT_Mu5_v1
//    L1T Seed: L1_SingleMu3,
//            Trigger Object:               'hltL1sSingleMu3',
//            Trigger Object:               'hltL1fL1sMu3L1Filtered0',
//            Trigger Object:               'hltL2fL1sMu3L1f0L2Filtered3Q',
//            Trigger Object:               'hltL3fL1sMu3L1f0L2f3QL3Filtered5Q',
// ----------------------------------------
// 
// HLT_PFJet20_v1
//    L1T Seed: L1_SingleJet20,
//            Trigger Object:               'hltL1sSingleJet20',
//            Trigger Object:               'hltSingleCaloJet20',
//            Trigger Object:               'hltSinglePFJet20',
// ----------------------------------------
// 
// HLT_PFJet40_v1
//    L1T Seed: L1_ZeroBias,
//            Trigger Object:               'hltL1sZeroBias',
//            Trigger Object:               'hltSingleCaloJet10',
//            Trigger Object:               'hltSinglePFJet40',
// ----------------------------------------
// 
// HLT_PFJet60_v1
//    L1T Seed: L1_SingleJet35,
//            Trigger Object:               'hltL1sSingleJet35',
//            Trigger Object:               'hltSingleCaloJet40',
//            Trigger Object:               'hltSinglePFJet60',
// ----------------------------------------
// 
// HLT_DoublePFJet20_v1
//    L1T Seed: L1_SingleJet20,
//            Trigger Object:               'hltL1sSingleJet20',
//            Trigger Object:               'hltDoubleCaloJet20',
//            Trigger Object:               'hltDoublePFJet20',
// ----------------------------------------
// 
// HLT_DiPFJetAve40_v1
//    L1T Seed: L1_ZeroBias,
//            Trigger Object:               'hltL1sZeroBias',
//            Trigger Object:               'hltDiCaloJetAve30',
//            Trigger Object:               'hltDiPFJetAve40',
// ----------------------------------------
// 
// HLT_DiPFJetAve60_v1
//    L1T Seed: L1_ZeroBias,
//            Trigger Object:               'hltL1sZeroBias',
//            Trigger Object:               'hltDiCaloJetAve45',
//            Trigger Object:               'hltDiPFJetAve60',
// ----------------------------------------
// 
// HLT_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30_v1
//    L1T Seed: L1_SingleJet20,
//            Trigger Object:               'hltL1sSingleJet20',
//            Trigger Object:               'hltDoubleCaloJet30',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJet30',
// ----------------------------------------
// 
// HLT_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30MaxDeta1p6_v1
//    L1T Seed: L1_SingleJet20,
//            Trigger Object:               'hltL1sSingleJet20',
//            Trigger Object:               'hltDoubleCaloJet30',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJet30',
//            Trigger Object:               'hltDoublePFJetsC30MaxDeta1p6',
// ----------------------------------------
// 
// HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v1
//    L1T Seed: L1_DoubleJetC100,
//            Trigger Object:               'hltL1sDoubleJetC100',
//            Trigger Object:               'hltDoubleJetsC100',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJetsC100',
//            Trigger Object:               'hltDoublePFJetsC100MaxDeta1p6',
// ----------------------------------------
// 
// HLT_DoubleJetsC112_DoubleBTagCSV_p014_DoublePFJetsC112MaxDeta1p6_v1
//    L1T Seed: L1_DoubleJetC112,
//            Trigger Object:               'hltL1sDoubleJetC112',
//            Trigger Object:               'hltDoubleJetsC112',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJetsC112',
//            Trigger Object:               'hltDoublePFJetsC112MaxDeta1p6',
// ----------------------------------------
// 
// HLT_DoubleJetsC100_SingleBTagCSV_p014_v1
//    L1T Seed: L1_DoubleJetC100,
//            Trigger Object:               'hltL1sDoubleJetC100',
//            Trigger Object:               'hltDoubleJetsC100',
//            Trigger Object:               'hltSingleBTagCSV0p84',
// ----------------------------------------
// 
// HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v1
//    L1T Seed: L1_DoubleJetC100,
//            Trigger Object:               'hltL1sDoubleJetC100',
//            Trigger Object:               'hltDoubleJetsC100',
//            Trigger Object:               'hltSingleBTagCSV0p84',
//            Trigger Object:               'hltJetC350',
// ----------------------------------------
// 
// HLT_BTagMu_DiJet20_Mu5_v1
//    L1T Seed: L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4,
//            Trigger Object:               'hltL1sMu3JetC16dEtaMax0p4dPhiMax0p4',
//            Trigger Object:               'hltBDiJet20L1FastJetCentral',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetL25FilterByDR',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetMu5L3FilterByDR',
// ----------------------------------------
// 
// HLT_BTagMu_DiJet20_Mu5_DoubleBTagCSV_p014_DoublePFJet20_v1
//    L1T Seed: L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4,
//            Trigger Object:               'hltL1sMu3JetC16dEtaMax0p4dPhiMax0p4',
//            Trigger Object:               'hltBDiJet20L1FastJetCentral',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetL25FilterByDR',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetMu5L3FilterByDR',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFBJet20',
// ----------------------------------------
// 
// HLT_BTagMu_DiJet20_Mu5_DoubleBTagCSV_p014_DoublePFJet20MaxDeta1p6_v1
//    L1T Seed: L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4,
//            Trigger Object:               'hltL1sMu3JetC16dEtaMax0p4dPhiMax0p4',
//            Trigger Object:               'hltBDiJet20L1FastJetCentral',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetL25FilterByDR',
//            Trigger Object:               'hltBSoftMuonDiJet20L1FastJetMu5L3FilterByDR',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFBJet20',
//            Trigger Object:               'hltDoublePFJetsC20MaxDeta1p6',
// ----------------------------------------
// 
// HLT_L1MuJet_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30_v1
//    L1T Seed: L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4,
//            Trigger Object:               'hltL1sMu3JetC16dEtaMax0p4dPhiMax0p4',
//            Trigger Object:               'hltDoubleCaloJet30',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJet30',
// ----------------------------------------
// 
// HLT_L1MuJet_DoubleJet30_DoubleBTagCSV_p014_DoublePFJet30MaxDeta1p6_v1
//    L1T Seed: L1_Mu3_JetC16_dEta_Max0p4_dPhi_Max0p4,
//            Trigger Object:               'hltL1sMu3JetC16dEtaMax0p4dPhiMax0p4',
//            Trigger Object:               'hltDoubleCaloJet30',
//            Trigger Object:               'hltBTagCaloCSVp014DoubleWithMatching',
//            Trigger Object:               'hltDoublePFJet30',
//            Trigger Object:               'hltDoublePFJetsC30MaxDeta1p6',
// ----------------------------------------
// 
// HLTriggerFinalPath
// ----------------------------------------
// 
