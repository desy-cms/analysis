#include <string>
#include <iostream>
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

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;

double weight(TH2F *, const float &, const float &, const float &, const float &);
double weight2(TH2F *, TH2F *, const float &, const float &, const float &, const float &);
double ptRewfunction(const float &);
double ptRew2D(TH2F *, const float &, const float &);
double dEtaRew(const float &);
double bTagRew(TH2F *, const float&, const float &);

// =============================================================================================
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   //For btag efficiency:
   TFile * fileBTagEff = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/bTagEfficiency2D_pteta_zomin.root");
   TH2F *btagEff = (TH2F*) fileBTagEff ->Get("h2ehn");
   //For Pt,eta efficiency:
   TFile * filePtEff = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsJetHT2015DPromptReco4.root");
   TH2F *PtEff = (TH2F*) filePtEff ->Get("h2_eff80");

   // Input files list MonteCarloStudies
   std::string inputList = "rootFileListBTagCSV.txt";
   MssmHbb analysis(inputList);

   if(analysis.isMC())
   {
     // cross sections
     analysis.crossSections("MssmHbb/Metadata/CrossSections");
     analysis.listCrossSections();

     //generator filter
     analysis.generatorFilter("MssmHbb/Metadata/GeneratorFilter");
     analysis.listGeneratorFilter();
   }

   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
   std::vector<std::string> jetTriggerObjects;
   if(!analysis.isMC()) {
   analysis.addTree<TriggerObject> ("hltL1sL1DoubleJetC100","MssmHbb/Events/selectedPatTrigger/hltL1sL1DoubleJetC100");
   analysis.addTree<TriggerObject> ("hltDoubleJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoubleJetsC100");
   analysis.addTree<TriggerObject> ("hltDoublePFJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoublePFJetsC100");
   analysis.addTree<TriggerObject> ("hltDoubleBTagCSV0p9","MssmHbb/Events/selectedPatTrigger/hltDoubleBTagCSV0p9");
   analysis.addTree<TriggerObject> ("hltDoublePFJetsC100MaxDeta1p6","MssmHbb/Events/selectedPatTrigger/hltDoublePFJetsC100MaxDeta1p6");
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   std::string triggerLogicName = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";

   jetTriggerObjects.push_back("hltL1sL1DoubleJetC100");
   jetTriggerObjects.push_back("hltDoubleJetsC100");
   jetTriggerObjects.push_back("hltDoubleBTagCSV0p9");
   jetTriggerObjects.push_back("hltDoublePFJetsC100");
   jetTriggerObjects.push_back("hltDoublePFJetsC100MaxDeta1p6");
   }

   if(!analysis.isMC()) analysis.processJsonFile("goodJson.txt");

   int nCand = 0;
   std::vector <int> leadJetsNumber;

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

	h1["h_obj_M"]  		  = new TH1F("h_obj_M12","",100,0.,1000.);
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

   std::cout<<" Lumi: "<<analysis.size()<<" "<<analysis.size()/analysis.crossSection()<<" "<<analysis.luminosity()<<std::endl;
   //std::cout<<"Lol: "<<1001.179266/analysis.luminosity(850.)<<std::endl;
   // Analysis of events
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {

      analysis.event(i);

      if (!analysis.isMC() && !analysis.selectJson()) continue;
      auto offlineJets = analysis.collection<Jet>("Jets");
      if (offlineJets -> size() < 2) continue;
      if(!analysis.isMC()) analysis.match<Jet,TriggerObject>("Jets",jetTriggerObjects);


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
      if( fLeadJet.pt() < 150 || sLeadJet.pt() < 150 ) continue;
      if( std::abs(fLeadJet.eta()) > 2.2 || std::abs(sLeadJet.eta()) > 2.2 ) continue;
      if( std::abs(fLeadJet.eta() - sLeadJet.eta()) > 1.6 ) continue;
      if( fLeadJet.deltaR(sLeadJet) <= 1) continue;
      if( fLeadJet.btag() < 0.941 || sLeadJet.btag() < 0.941 ) continue;

      h1["h_jet1_PtNoRew"] -> Fill(fLeadJet.pt());
      if(!analysis.isMC() && !analysis.lowMOnlineSelection(fLeadJet,sLeadJet) ) continue;

     double reweight;
     if(!analysis.isMC()) reweight = 1.;
     else reweight = weight(btagEff,fLeadJet.pt(),fLeadJet.eta(),sLeadJet.pt(),sLeadJet.eta());
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
     h1["h_obj_M"] -> Fill(obj.M(),reweight);

     h1["h_djet_Pt12"] -> Fill(fLeadJet.pt() - sLeadJet.pt(),reweight);
     h1["h_djet_Eta12"] -> Fill( fLeadJet.eta() - sLeadJet.eta(),reweight);
     h1["h_djet_Phi12"] -> Fill( fLeadJet.phi() - sLeadJet.phi(),reweight);
     h1["h_djet_PtTOT"] -> Fill( (fLeadJet.pt() - sLeadJet.pt() )/ (fLeadJet.pt() + sLeadJet.pt()) ,reweight);

     if(fLeadJet.pt() <= sLeadJet.pt() ) std::cout<<"WTF"<<std::endl;


     if(analysis.isMC()) {
     h1["h_jet1_Ptrew1"] -> Fill(fLeadJet.pt(),ptRewfunction(fLeadJet.pt())*ptRewfunction(sLeadJet.pt()));
     h1["h_jet1_Ptrew2"] -> Fill(fLeadJet.pt(),ptRew2D(PtEff,fLeadJet.pt(),fLeadJet.eta()) * ptRew2D(PtEff,sLeadJet.pt(),sLeadJet.eta()));
     h1["h_jet1_PtplusDEta"] -> Fill(fLeadJet.pt(),ptRewfunction(fLeadJet.pt())*ptRewfunction(sLeadJet.pt()) * dEtaRew(std::abs(fLeadJet.eta() - sLeadJet.eta())));
     h1["h_jet1_All"] -> Fill(fLeadJet.pt(),ptRewfunction(fLeadJet.pt())*ptRewfunction(sLeadJet.pt()) * dEtaRew(std::abs(fLeadJet.eta() - sLeadJet.eta())) * bTagRew(btagEff,fLeadJet.pt(),fLeadJet.eta()) * bTagRew(btagEff, sLeadJet.pt(),sLeadJet.eta()) );
     h1["h_jet1_All2"] -> Fill (fLeadJet.pt(),reweight);
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

   }
   std::cout<<"Number of Candidates: "<<nCand<<std::endl;
   std::string fileName = "output.root";
   fileName = analysis.tree<Jet>("Jets")->PhysicsObjectTree<Jet>::PhysicsObjectTreeBase::TreeBase::tree()->GetFile()->GetName();
   fileName = "outputFiles/Selection150GeV" + boost::filesystem::basename(fileName) + ".root";
   TFile * outFile = new TFile(fileName.c_str(),"RECREATE");

   //double lumiData2015DPrRec3 = 553.149951;
   double lumiData2015DPrRec4 = 1001.179266;
   for ( auto& ih1 : h1 )
   {
   	if(analysis.isMC() && fileName.find("QCD") != std::string::npos ) ih1.second->Scale(lumiData2015DPrRec4/analysis.luminosity());
   	else if (analysis.isMC() && fileName.find("TT") != std::string::npos) 	{ih1.second->Scale(lumiData2015DPrRec4/analysis.luminosity(850.));}
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
