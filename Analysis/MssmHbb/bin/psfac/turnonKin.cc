#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "stdlib.h"

#include "Analysis/MssmHbb/interface/MssmHbb.h"
#include "Analysis/MssmHbb/interface/json.h"
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"
#include "Analysis/MssmHbb/interface/Weights.h"

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;

double weight(TH2F *, const float &, const float &, const float &, const float &);
double weight2(TH2F *, TH2F *, const float &, const float &, const float &, const float &);
double ptRewfunction(const float &);
double ptRew2D(TH2F *, const float &, const float &);
double dEtaRew(const float &);
double bTagRew(TH2F *, const float&, const float &);

class TriggerEfficiency {
public:
  bool matchToPF80(const Jet &jet){
    bool matched = false;
    if(jet.matched("hltL1sL1SingleJet52") && jet.matched("hltSingleCaloJet50") &&jet.matched("hltSinglePFJet80") ) matched = true;
    return matched;
  }
  bool testMatchToPF100(const Jet &jet){
    bool matched = false;
    const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
    const Candidate * l2 = jet.matched("hltSingleCaloJet50");
    const Candidate * l3 = jet.matched("hltSinglePFJet80");
    if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() > 100  && l3->pt() > 100) matched = true;
    if (!matched)  std::cout << "l1=" << l1 << " l2=" << l2
			     << " l3=" << l3 << " ptl1=" << l1->pt()
			     << " ptl2=" << l2->pt()
			     << " ptl3=" << l3->pt() << std::endl;
    return matched;
  }
  bool matchToPF100(const Jet &jet){
    bool matched = false;
    const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
    const Candidate * l2 = jet.matched("hltSingleCaloJet50");
    const Candidate * l3 = jet.matched("hltSinglePFJet80");
    // if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() > 100  && l3->pt() > 100) matched = true;
    if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() > 100  && l3->pt() > 100) matched = true;
    return matched;
  }
  bool matchToPF160(const Jet &jet){
    bool matched = false;
    const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
    const Candidate * l2 = jet.matched("hltSingleCaloJet50");
    const Candidate * l3 = jet.matched("hltSinglePFJet80");
    if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() > 100  && l3->pt() > 160) matched = true;
    return matched;
  }
  bool matchToPF100dEta1p6(const Jet &jet1 , const Jet &jet2){
		bool matched = false;
		// const Candidate * J1l1 = jet1.matched("hltL1sL1SingleJet52");
		// const Candidate * J1l2 = jet1.matched("hltSingleCaloJet50");
		const Candidate * J1l3 = jet1.matched("hltSinglePFJet80");
	
		// const Candidate * J2l1 = jet2.matched("hltL1sL1SingleJet52");
		// const Candidate * J2l2 = jet2.matched("hltSingleCaloJet50");
		const Candidate * J2l3 = jet2.matched("hltSinglePFJet80");
		if( J1l3 && J2l3 && J1l3->pt()>100 && J2l3->pt()>100 && std::abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;
		return matched;
  }
  bool matchToDoubleJetsC100DoublePFJetsC100MaxDeta1p6(const Jet &jet1 , const Jet &jet2){
    bool matched = false;
    const Candidate * J1l1 = jet1.matched("hltL1sL1DoubleJetC100");
    const Candidate * J1l2 = jet1.matched("hltDoubleJetsC100");
    const Candidate * J1l3 = jet1.matched("hltDoublePFJetsC100MaxDeta1p6");
	
    const Candidate * J2l1 = jet2.matched("hltL1sL1DoubleJetC100");
    const Candidate * J2l2 = jet2.matched("hltDoubleJetsC100");
    const Candidate * J2l3 = jet2.matched("hltDoublePFJetsC100MaxDeta1p6");
    if( J1l1 && J1l2 && J1l3 && J2l1 && J2l2 && J2l3) matched = true;
    return matched;
  }
  bool matchToDoubleJetsC100DoublePFJetsC160(const Jet &jet1 , const Jet &jet2){
    bool matched = false;
    const Candidate * J1l1 = jet1.matched("hltL1sL1DoubleJetC100");
    const Candidate * J1l2 = jet1.matched("hltDoubleJetsC100");
    const Candidate * J1l3 = jet1.matched("hltDoublePFJetsC160");
	
    const Candidate * J2l1 = jet2.matched("hltL1sL1DoubleJetC100");
    const Candidate * J2l2 = jet2.matched("hltDoubleJetsC100");
    const Candidate * J2l3 = jet2.matched("hltDoublePFJetsC160");

    if( J1l1 && J1l2 && J1l3 && J2l1 && J2l2 && J2l3) matched = true;
    return matched;
  }
  bool matchToDoubleBTagCSV0p9(const Jet &jet1 , const Jet &jet2){
    bool matched = false;
    const Candidate * J1l3 = jet1.matched("hltDoubleBTagCSV0p9");
    const Candidate * J2l3 = jet2.matched("hltDoubleBTagCSV0p9");
    if ( J1l3 && J2l3 ) matched = true;
    return matched;
  }
  bool matchToDoubleBTagCSV0p85(const Jet &jet1 , const Jet &jet2){
    bool matched = false;
    const Candidate * J1l3 = jet1.matched("hltDoubleBTagCSV0p85");
    const Candidate * J2l3 = jet2.matched("hltDoubleBTagCSV0p85");
    if ( J1l3 && J2l3 ) matched = true;
    return matched;
  }
};



// =============================================================================================
int main(int argc, char * argv[])
{

  std::cout << "Starting..." << std::endl;
  TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

  // Input files list
  //std::string inputList = "rootFileListBTagCSV.txt";

  bool Data2016 = false; 

  std::string inputList;

  if (Data2016) {
    inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/2016/80X/BTagCSVData/Run2016B-PromptReco-v1.txt";
  } else {
    inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/2015/76X/76x_summer_conferences_2016_v1/BTagCSV/Run2015.txt";
  }
  bool ptCut = false;
  std::string pthat = "170to300";
  inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/2015/76X/76x_summer_conferences_2016_v1/Pythia8_QCD/QCD_Pt_"+pthat+"_TuneCUETP8M1_13TeV_pythia8.txt";
//  inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/Pythia8_QCD/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt";
//  inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/Pythia8_QCD/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.txt";
//  inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/Pythia8_QCD/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt";

  // std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/MSSMHbb/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.txt";
  // std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/Pythia8_QCD/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.txt";
  // std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/Pythia8_QCD/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt";
  // std::string inputList = "/afs/desy.de/group/cms/pool/mankel/HbbRun2v2/CMSSW_7_6_3/src/Analysis/MssmHbb/test/QCD_Pt_30to170.txt";
  // std::string inputList = "/afs/desy.de/group/cms/pool/mankel/HbbRun2v2/CMSSW_7_6_3/src/Analysis/MssmHbb/test/QCD_Pt_50to170.txt";
  // Initialisation of MssmHbb class

  std::cout<<"Input list: "<<inputList<<std::endl;

  MssmHbb analysis(inputList);
  // Process selected JSON file
  if (! Data2016) {
    if(!analysis.isMC()) analysis.processJsonFile("../Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_Silver_v2.txt");
  }

  cout << "After JSON" << endl;
  
  // Add std::vector<std::string> of the Trigger Objects that you would like to apply.
  // Also Trigger Results name will be stored, according to the trigger objects names
  // By default - LowM MssmHbb trigger objects is used
//  std::vector<std::string> triggerObjects = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};

  cout << "After triggerObjects (1)" << endl;

  //Select which sets of cuts should be applied:for Low mass selection = true; for High M = false.
  // if(analysis.isMC()) analysis.setLowMSelection(true);

  analysis.setLowMSelection(true);
  
  //analysis.addTriggerObjects();
  analysis.addTriggerObjects();

  cout << "After trigger objects" << endl;

  //Create Object to calculate the weights:
  Weights weightCalc(analysis.getLowMSelection());
  std::map<std::string, double> weight;

  std::string triggerLogicLowMass = "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v";
  std::string triggerLogicHighMass = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
  
  //.................................Input corrections.................................
  cout << "Before reweighting" << endl;

  // Add PileUp reweighting
  std::map<std::string, TFile*> fPileUpData;
  std::map<std::string, TH1F* > hPileUpData;
  fPileUpData["central"] = new TFile("../input_corrections/PileUp_2015Dec_central.root","read");
  hPileUpData["central"] = (TH1F*) fPileUpData["central"]->Get("pileup");
  fPileUpData["down"] = new TFile("../input_corrections/PileUp_2015Dec_up.root","read");
  hPileUpData["down"] = (TH1F*) fPileUpData["down"]->Get("pileup");
  fPileUpData["up"] = new TFile("../input_corrections/PileUp_2015Dec_down.root","read");
  hPileUpData["up"] = (TH1F*) fPileUpData["up"]->Get("pileup");
  TFile *fPileUpMC = new TFile("../input_corrections/MC_Fall15_PU25_V1.root","read");
  TH1F *hPileUpMC = (TH1F*) fPileUpMC->Get("pileup");

  int nCand = 0;
  int nTrigHighMass = 0;
  int nTrigMatchHbb = 0;
  int nTrigMatchOnlineHbb = 0;
  int nTrigLowMassMatchKin = 0;
  int nTrigLowMass = 0;
  int nOfflineSel = 0;
  std::vector <int> leadJetsNumber;

  cout << "Before hist booking" << endl;

  std::map < std::string, TH1F *> h1;
  h1["h_jet_N"]         = new TH1F("h_jet_N" , "", 20, 0., 20.);
  
  h1["h_jet1_Pt"]       = new TH1F("h_jet1_Pt" , "", 100, 0., 1000.);
  h1["h_jet1_Eta"]      = new TH1F("h_jet1_Eta", "", 30, -5, 5.);
  h1["h_jet1_Phi"]      = new TH1F("h_jet1_Phi", "", 100, -3.2, 3.2);
  h1["h_jet1_BTag"]     = new TH1F("h_jet1_BTag", "", 100, 0., 1.);
  h1["h_jet1_BTagZoom"] = new TH1F("h_jet1_BTagZoom", "", 100, 0.94, 1.);
   
  h1["h_jet2_Pt"]       = new TH1F("h_jet2_Pt" , "", 100, 0., 1000.);
  h1["h_jet2_Eta"]      = new TH1F("h_jet2_Eta", "", 30, -5, 5.);
  h1["h_jet2_Phi"]      = new TH1F("h_jet2_Phi", "", 100, -3.2, 3.2);
  h1["h_jet2_BTag"]     = new TH1F("h_jet2_BTag", "", 100, 0., 1.);
  h1["h_jet2_BTagZoom"] = new TH1F("h_jet2_BTagZoom", "", 100, 0.94, 1.);

  h1["h_jet1_hM_Pt"]       = new TH1F("h_jet1_hM_Pt" , "", 100, 0., 500.);
  h1["h_jet2_hM_Pt"]       = new TH1F("h_jet2_hM_Pt" , "", 100, 0., 500.);

  h1["h_obj_M12"]       = new TH1F("h_obj_M12","",100,0.,500.);
  h1["h_obj_Pt"]		  = new TH1F("h_obj_Pt","",100,0.,1000.);
  h1["h_obj_Eta"]		  = new TH1F("h_obj_Eta","",100,-5.,5.);
  h1["h_obj_Phi"]       = new TH1F("h_obj_Phi","",100,-3.2,3.2);
   
  h1["h_djet_Pt12"]     = new TH1F("h_djet_Pt12","",100,0.,200);
  h1["h_djet_Eta12"]    = new TH1F("h_djet_Eta12","",100,-5.,5.);
  h1["h_djet_Phi12"]    = new TH1F("h_djet_Phi12","",100,-3.2,3.2);
  h1["h_djet_PtTOT"]	  = new TH1F("h_djet_PtTOT","",100,0,1);
   
  h1["h_jet_dEta"]	  = new TH1F("h_jet_dEta","",30,0,3);

  //Plots to study reweighting:
  h1["h_jet1_PtNoRew"]      = new TH1F("h_jet1_PtNoRew", "", 100, 0., 1000.);
  h1["h_jet1_Ptrew1"]       = new TH1F("h_jet1_Ptrew1" , "", 100, 0., 1000.);
  h1["h_jet1_Ptrew2"]       = new TH1F("h_jet1_Ptrew2" , "", 100, 0., 1000.);
  h1["h_jet1_PtplusDEta"]   = new TH1F("h_jet1_PtplusDEta" , "", 100, 0., 1000.);
  h1["h_jet1_All"]          = new TH1F("h_jet1_All" , "", 100, 0., 1000.);
  h1["h_jet1_All2"]         = new TH1F("h_jet1_All2" , "", 100, 0., 1000.);

  h1["h_jet1_qq_Pt"]		  = new TH1F("h_jet1_qq_Pt", "", 100, 0., 1000.);
  h1["h_jet1_bb_Pt"]		  = new TH1F("h_jet1_bb_Pt", "", 100, 0., 1000.);
  h1["h_jet1_cc_Pt"]		  = new TH1F("h_jet1_cc_Pt", "", 100, 0., 1000.);
  h1["h_jet1_qb_Pt"]		  = new TH1F("h_jet1_qb_Pt", "", 100, 0., 1000.);
  h1["h_jet1_qc_Pt"]		  = new TH1F("h_jet1_qc_Pt", "", 100, 0., 1000.);
  h1["h_jet1_cb_Pt"]		  = new TH1F("h_jet1_cb_Pt", "", 100, 0., 1000.);

  cout << "after hist booking" << endl;

  // histograms for emulated trigger inefficiency
  std::vector<std::string> vtemu = {"Hmt", "Lmt"};
  for (std::vector<std::string>::iterator sit=vtemu.begin(); sit != vtemu.end(); ++sit) {
    
    h1[Form("h_jet1_%s_Pt",(*sit).c_str())] = new TH1F(Form("h_jet1_%s_Pt",(*sit).c_str()) , "", 100, 0., 1000.);
    h1[Form("h_jet1_%s_Eta",(*sit).c_str())] = new TH1F(Form("h_jet1_%s_Eta",(*sit).c_str()), "",100,-5.,5.);
    h1[Form("h_jet1_%s_Phi",(*sit).c_str())] = new TH1F(Form("h_jet1_%s_Phi",(*sit).c_str()), "", 100, -3.2, 3.2);
    h1[Form("h_jet1_%s_BTag",(*sit).c_str())] = new TH1F(Form("h_jet1_%s_BTag",(*sit).c_str()), "", 100, 0., 1.);
    h1[Form("h_jet2_%s_Pt",(*sit).c_str())] = new TH1F(Form("h_jet2_%s_Pt",(*sit).c_str()) , "", 100, 0., 1000.);
    h1[Form("h_jet2_%s_Eta",(*sit).c_str())] = new TH1F(Form("h_jet2_%s_Eta",(*sit).c_str()), "",100,-5.,5.);
    h1[Form("h_jet2_%s_Phi",(*sit).c_str())] = new TH1F(Form("h_jet2_%s_Phi",(*sit).c_str()), "", 100, -3.2, 3.2);
    h1[Form("h_jet2_%s_BTag",(*sit).c_str())] = new TH1F(Form("h_jet2_%s_BTag",(*sit).c_str()), "", 100, 0., 1.);
    h1[Form("h_obj_%s_MinBTag",(*sit).c_str())]   = new TH1F(Form("h_obj_%s_MinBTag",(*sit).c_str()),"",100, 0., 1.);
  }

  cout << "Trigger Logic: "<<analysis.getTriggerLogicName()<<endl;


   // Analysis of events
   for ( int i = 0 ; i < analysis.size(); ++i )
   {

     if ( i%100000 == 0 ) std::cout << "At event " << i << std::endl;
     // if ( i%100000 == 0 ) std::cout << "At event " << i << "\r" << std::flush;
//     if (i>500000) {
//       std::cout << "stop here" << std::endl;
//       break;
//     }

     analysis.event(i);

     //Select only good JSon runs
     if (!analysis.isMC() && !analysis.selectJson()) continue;

     //Trigger Selection
     if( !analysis.isMC() && !analysis.getTriggerResult()) continue;
//     std::cout<<"After Trigger Results"<<std::endl;

     //Match offline Jets to online Objects
     // if (!analysis.isMC()) analysis.match<Jet,TriggerObject>("Jets",analysis.getTriggerObjectNames());
     analysis.match<Jet,TriggerObject>("Jets",analysis.getTriggerObjectNames());

     //Define Jet Collection
     auto offlineJets = analysis.collection<Jet>("Jets");

     if (offlineJets -> size() < 2) continue;

//     std::cout<<"After NJets"<<std::endl;

     // Selection of double b-tag sample:

     leadJetsNumber.clear();
     for( int iJet = 0; iJet < offlineJets -> size(); ++iJet)
       {
	 Jet jet = offlineJets -> at(iJet);
	 if(jet.idLoose() ) leadJetsNumber.push_back(iJet);
	 
       }
     if( leadJetsNumber.size() < 2) continue;
     // Match Offline jet objects to trigger objects
     Jet fLeadJet = offlineJets -> at(leadJetsNumber[0]);
     Jet sLeadJet = offlineJets -> at(leadJetsNumber[1]);
     //std::cout<<"Flavours: "<<fLeadJet.flavour("Hadron")<<" "<<std::endl;

     if (analysis.getLowMSelection()) {
       if( ptCut && (fLeadJet.pt() < 100 || sLeadJet.pt() < 100) ) continue;
       if( std::abs(fLeadJet.eta()) > 2.2 || std::abs(sLeadJet.eta()) > 2.2 ) continue;
       if( std::abs(fLeadJet.eta() - sLeadJet.eta()) > 1.55 ) continue;
       if( fLeadJet.deltaR(sLeadJet) <= 1) continue;
       if( fLeadJet.btag() < 0.8 || sLeadJet.btag() < 0.8 ) continue;
     } else {
//       if( fLeadJet.pt() < 160 || sLeadJet.pt() < 160 ) continue;
       if( std::abs(fLeadJet.eta()) > 2.2 || std::abs(sLeadJet.eta()) > 2.2 ) continue;
       if( std::abs(fLeadJet.eta() - sLeadJet.eta()) > 1.55 ) continue;
       if( fLeadJet.deltaR(sLeadJet) <= 1) continue;
       if( fLeadJet.btag() < 0.8 || sLeadJet.btag() < 0.8 ) continue;
     }

     ++nOfflineSel;

     // std::cout << "pt1= " << fLeadJet.pt()
     // 	       << " pt2= " << sLeadJet.pt()
     // 	       << " Lmt= " << analysis.triggerResult(triggerLogicLowMass)
     // 	       << " Hmt= " << analysis.triggerResult(triggerLogicHighMass)
     // 	       << std::endl;

     h1["h_jet1_PtNoRew"] -> Fill(fLeadJet.pt());
     // if(!analysis.isMC() && !analysis.OnlineSelection(fLeadJet,sLeadJet) ) continue;
     // const Candidate *cand = matched(jet1,"DoubleJetC100");

     // select events that pass the low mass trigger
//     if (! analysis.triggerResult(triggerLogicLowMass)) continue;  // require low mass trigger
//     ++nTrigLowMass;

     // require matches of trigger objects to two leading jets
//     TriggerEfficiency te;
//     if (! (te.matchToDoubleJetsC100DoublePFJetsC100MaxDeta1p6(fLeadJet,sLeadJet) )) continue;
//     ++nTrigLowMassMatchKin;

     string smode("Lmt");
//     h1[Form("h_jet1_%s_Pt",smode.c_str())]->Fill( fLeadJet.pt() );
//     h1[Form("h_jet1_%s_Eta",smode.c_str())]->Fill( fLeadJet.eta() );
//     h1[Form("h_jet1_%s_Phi",smode.c_str())]->Fill( fLeadJet.phi() );
//     h1[Form("h_jet1_%s_BTag",smode.c_str())]->Fill( fLeadJet.btag() );
//     h1[Form("h_jet2_%s_Pt",smode.c_str())]->Fill( sLeadJet.pt() );
//     h1[Form("h_jet2_%s_Eta",smode.c_str())]->Fill( sLeadJet.eta() );
//     h1[Form("h_jet2_%s_Phi",smode.c_str())]->Fill( sLeadJet.phi() );
//     h1[Form("h_jet2_%s_BTag",smode.c_str())]->Fill( sLeadJet.btag() );
//     h1[Form("h_obj_%s_MinBTag",smode.c_str())]->Fill( min(fLeadJet.btag(),sLeadJet.btag()) );

     // this defines the reference set (denominator)

     // further checks for the selected set (numerator)
     // check if event survives the Hbb trigger
//     bool trigHighMass = analysis.triggerResult(triggerLogicHighMass);
//     if (! trigHighMass) continue;
//     ++nTrigHighMass;
//
//     // check for matching of kinematic trigger objects
//     bool trigMatchHbb = te.matchToDoubleJetsC100DoublePFJetsC160(fLeadJet,sLeadJet);
//     if (! trigMatchHbb) continue;
//     ++nTrigMatchHbb;


     // check for survival of the online btag match
     // bool trigMatchOnlineHbb = false;
     // if(analysis.getLowMSelection()){
     //   trigMatchOnlineHbb = te.matchToDoubleBTagCSV0p9(fLeadJet,sLeadJet);
     // } else {
     //   trigMatchOnlineHbb = te.matchToDoubleBTagCSV0p85(fLeadJet,sLeadJet);
     // }

     // // count events still surviving the online btag match
     // if (! trigMatchOnlineHbb) continue;
     // ++nTrigMatchOnlineHbb;
     
     // fill control histograms
     smode.assign("Hmt");
     h1[Form("h_jet1_%s_Pt",smode.c_str())]->Fill( fLeadJet.pt() );
     h1[Form("h_jet1_%s_Eta",smode.c_str())]->Fill( fLeadJet.eta() );
     h1[Form("h_jet1_%s_Phi",smode.c_str())]->Fill( fLeadJet.phi() );
     h1[Form("h_jet1_%s_BTag",smode.c_str())]->Fill( fLeadJet.btag() );
     h1[Form("h_jet2_%s_Pt",smode.c_str())]->Fill( sLeadJet.pt() );
     h1[Form("h_jet2_%s_Eta",smode.c_str())]->Fill( sLeadJet.eta() );
     h1[Form("h_jet2_%s_Phi",smode.c_str())]->Fill( sLeadJet.phi() );
     h1[Form("h_jet2_%s_BTag",smode.c_str())]->Fill( sLeadJet.btag() );
     h1[Form("h_obj_%s_MinBTag",smode.c_str())]->Fill( min(fLeadJet.btag(),sLeadJet.btag()) );

     if(analysis.isMC()) {

   	  //.........................Calculate weights...............
   	  // general weights which is independent of selection type OR already includes
   	  // selection depends criterias inside.
   	  weight["Lumi"]     = weightCalc.LumiWeight(2318.278306,analysis.luminosity());

   	  weight["PileUpCentral"] = weightCalc.PileUpWeight(hPileUpData["central"],hPileUpMC,analysis.nTruePileup());
   	  weight["PileUpDown"]    = weightCalc.PileUpWeight(hPileUpData["down"],hPileUpMC,analysis.nTruePileup());
   	  weight["PileUpUp"]      = weightCalc.PileUpWeight(hPileUpData["up"],hPileUpMC,analysis.nTruePileup());

     }

     double reweight;
     if(!analysis.isMC()) reweight = 1.;
     // else reweight = weight["2DPt"]*weight["dEta"]*weight["BTag"]*weight["PileUpCentral"]*weight["Lumi"];
     else reweight = weight["PileUpCentral"]*weight["Lumi"];
     //else reweight = weight2(PtEff,btagEff,fLeadJet.pt(),fLeadJet.eta(),sLeadJet.pt(),sLeadJet.eta());

     h1["h_jet1_Pt"] -> Fill(fLeadJet.pt(),reweight);
     h1["h_jet1_Eta"] -> Fill(fLeadJet.eta(),reweight);
     h1["h_jet1_Phi"] -> Fill(fLeadJet.phi(),reweight);
     h1["h_jet1_BTag"] -> Fill(fLeadJet.btag(),reweight);
     h1["h_jet1_BTagZoom"] -> Fill(fLeadJet.btag(),reweight);

     h1["h_jet2_Pt"] -> Fill(sLeadJet.pt(),reweight);
     h1["h_jet2_Eta"] -> Fill(sLeadJet.eta(),reweight);
     h1["h_jet2_Phi"] -> Fill(sLeadJet.phi(),reweight);
     h1["h_jet2_BTag"] -> Fill(sLeadJet.btag(),reweight);
     h1["h_jet2_BTagZoom"] -> Fill(sLeadJet.btag(),reweight);

     h1["h_jet_dEta"] -> Fill(std::abs(fLeadJet.eta() - sLeadJet.eta()),reweight);

     TLorentzVector obj;
     obj = fLeadJet.p4() + sLeadJet.p4();

     h1["h_obj_Pt"] -> Fill(obj.Pt(),reweight);
     h1["h_obj_Eta"] -> Fill(obj.Eta(),reweight);
     h1["h_obj_Phi"] -> Fill(obj.Phi(),reweight);
     h1["h_obj_M12"] -> Fill(obj.M(),reweight);

     h1["h_djet_Pt12"] -> Fill(fLeadJet.pt() - sLeadJet.pt(),reweight);
     h1["h_djet_Eta12"] -> Fill( fLeadJet.eta() - sLeadJet.eta(),reweight);
     h1["h_djet_Phi12"] -> Fill( fLeadJet.phi() - sLeadJet.phi(),reweight);
     h1["h_djet_PtTOT"] -> Fill( (fLeadJet.pt() - sLeadJet.pt() )/ (fLeadJet.pt() + sLeadJet.pt()) ,reweight);

     if(obj.M() > 500) {
    	 h1["h_jet1_hM_Pt"] -> Fill(fLeadJet.pt(),reweight);
    	 h1["h_jet2_hM_Pt"] -> Fill(sLeadJet.pt(),reweight);
//    	 std::cout<<"M12: "<<obj.M()<<" Jets: pt1 = "<<fLeadJet.pt()<<" pt2 = "<<sLeadJet.pt()<<std::endl;
     }

     if(fLeadJet.flavour("Hadron") == 0 && sLeadJet.flavour("Hadron") == 0 )
     {
     	h1["h_jet1_qq_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }
     else if (fLeadJet.flavour("Hadron") == 4 && sLeadJet.flavour("Hadron") == 4)
     {
     	h1["h_jet1_cc_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }
     else if (fLeadJet.flavour("Hadron") == 5 && sLeadJet.flavour("Hadron") == 5)
     {
     	h1["h_jet1_bb_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }
     else if ( (fLeadJet.flavour("Hadron") == 0 || sLeadJet.flavour("Hadron") == 0) && (sLeadJet.flavour("Hadron") == 5 || fLeadJet.flavour("Hadron") == 5 ) )
     {
     	h1["h_jet1_qb_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }
     else if ( (fLeadJet.flavour("Hadron") == 4 || sLeadJet.flavour("Hadron") == 4) && ( sLeadJet.flavour("Hadron") == 0 || fLeadJet.flavour("Hadron") == 0 ) )
     {
     	h1["h_jet1_qc_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }
     else if ( (fLeadJet.flavour("Hadron") == 5 || sLeadJet.flavour("Hadron") == 5) && (sLeadJet.flavour("Hadron") == 4 || fLeadJet.flavour("Hadron") == 4 ) )
     {
     	h1["h_jet1_cb_Pt"] -> Fill(fLeadJet.pt(),reweight);
     }

      ++nCand;
      // if (analysis.triggerResult(triggerLogicName)) ++nTrigHbb;
      
      


   }
   std::cout << std::endl;  // to compensate for the last \r
   std::cout << "Number of Candidates passing offline selection " << nOfflineSel << std::endl;
   std::cout << "Number of Candidates with low mass trigger " 
	     << nTrigLowMass << std::endl;
   std::cout << "Number of Candidates with low mass trigger and kin match " << nTrigLowMassMatchKin << std::endl;
   std::cout << "Number of Candidates with high mass trigger " 
	     << nTrigHighMass << std::endl;
   std::cout << "Number of Candidates with Hbb trigger and leading jets matched kinematically " 
	     << nTrigMatchHbb << std::endl;
   std::cout << "Number of Candidates with Hbb trigger, kin and online btag match "
	     << nTrigMatchOnlineHbb << std::endl;
   std::cout<<"Number of Candidates: "<<nCand<<std::endl;
   std::string fileName;
   if(analysis.isMC()) fileName = "MC";
   else fileName = "Data";
   if(Data2016) fileName += "16";
   else fileName += "15";
   if(ptCut) fileName +="_PtCut";
   else fileName += "_NoPtCut";
   fileName += "_output_"+pthat+".root";
   TFile * outFile = new TFile(fileName.c_str(),"RECREATE");

   //double lumiData2015DPrRec3 = 553.149951;
//   double lumiData2015DPrRec4 = 1001.179266;
   for ( auto& ih1 : h1 )
   {
//   	if(analysis.isMC() && fileName.find("QCD") != std::string::npos ) ih1.second->Scale(lumiData2015DPrRec4/analysis.luminosity());
//   	else if (analysis.isMC() && fileName.find("TT") != std::string::npos) 	{ih1.second->Scale(lumiData2015DPrRec4/analysis.luminosity(850.));}
   	ih1.second -> Write();
   }
   outFile -> Close();

   std::cout<<" Output File: "<<fileName<<" was stored!"<<std::endl;

   return 0;
//
}

double weight(TH2F * BTagH, const float &pt1, const float &eta1, const float & pt2, const float & eta2)
{
  double weightPt1 = ptRewfunction(pt1);
	double weightPt2 = ptRewfunction(pt2);
	double dEta = std::abs(eta1 - eta2);
	double weightdEta = dEtaRew(dEta);
	double weightBTag1 = bTagRew(BTagH,pt1,eta1);
	double weightBTag2 = bTagRew(BTagH,pt2,eta2);
	double result = weightPt1 * weightPt2 * weightdEta * weightBTag1 * weightBTag2;
	return result;
}

double weight2(TH2F * PtH, TH2F * BTagH, const float &pt1, const float &eta1, const float & pt2, const float & eta2)
{
	double weightPt1 = ptRew2D(PtH,pt1,eta1);
	double weightPt2 = ptRew2D(PtH,pt2,eta2);
	double dEta = std::abs(eta1 - eta2);
	double weightdEta = dEtaRew(dEta);
	double weightBTag1 = bTagRew(BTagH,pt1,eta1);
	double weightBTag2 = bTagRew(BTagH,pt2,eta2);
	double result = weightPt1 * weightPt2 * weightdEta * weightBTag1 * weightBTag2;
	//double result = weightBTag1 * weightBTag2;
	return result;
}

double ptRewfunction(const float &pt)
{
	return 1./(1.+exp( -8.87968e-02 * (pt - 1.03879e+02) )) * 1./ (1.+exp( -1.88620e-01 * (pt - 1.02891e+02) ));
}
double ptRew2D(TH2F *PtH, const float & pt,const float &eta)
{
	if(pt <500) return PtH -> Interpolate(pt,eta);
	else return 1;
}
double dEtaRew(const float & dEta)
{
//	return 1./(1.+exp( 1.15733e+02 * (dEta - 1.59862e+00) )) * 1./ (1.+exp( -2.06004e-02 * (dEta + 2.22684e+02) ));
	return 1./(1.+exp( 1.41132e+02 * (dEta - 1.60010e+00) )) * 1./ (1.+exp( -2.06352e-02 * (dEta + 2.27738e+02) ));
}
double bTagRew(TH2F *histo, const float &pt, const float &eta)
{
	if(pt <1000) return histo -> Interpolate(pt,eta);
	else return 1;
}
