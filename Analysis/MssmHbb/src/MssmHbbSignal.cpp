/*
 * MssmHbbSignal.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

std::string constructTemplateName(const std::string & name);

MssmHbbSignal::MssmHbbSignal(const std::string & inputFilelist, const bool & lowM, const bool & test) :
								selectionDoubleB(inputFilelist,lowM,test)
{
	nJets_ = 3;
	if(lowM){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
        pt1_ = 100.; pt2_ = 100.; pt3_ = 40.;
        btag1_ = 0.935; btag2_ = 0.935; btag3_ = 0.8;
	}
	else {
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
        pt1_ = 160.; pt2_ = 160.; pt3_ = 40.;
        btag1_ = 0.8; btag2_ = 0.8; btag3_ = 0.46;
	}

	doubleBanalysis = pDoubleB(new selectionDoubleB(inputFilelist,lowM,test));
}

MssmHbbSignal::~MssmHbbSignal() {
}

//Member methods

const bool MssmHbbSignal::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	//To avoid code duplication frind class were used
	if(!doubleBanalysis->leadingJetSelection(offlineJets)) return false;

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::leadingJetSelection"<<std::endl;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);

	//Pt requirements
	if (jet3.pt() < pt3_) return false;

	//Eta requirements
	if (std::abs(jet3.eta()) > eta3_) return false;

	//BTag requirements
	if (jet3.btag() < btag3_) return false;

	//deltaR requirements
	if (jet2.deltaR(jet3) <= dR_) return false;
	if (jet1.deltaR(jet3) <= dR_) return false;

	return true;
}

void MssmHbbSignal::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets){

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::fillHistograms"<<std::endl;
	//TODO: re-implement with selectionDoubleB friendness

	double weight = 1;
	if(isMC()) {
		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PU_central"] * weight_["SFb_central"];
	}
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	(histo_.getHisto())["jet_pt1"]->Fill(jet1.pt(),weight);
	(histo_.getHisto())["jet_pt2"]->Fill(jet2.pt(),weight);

	(histo_.getHisto())["jet_eta1"]->Fill(jet1.eta(),weight);
	(histo_.getHisto())["jet_eta2"]->Fill(jet2.eta(),weight);

	(histo_.getHisto())["jet_phi1"]->Fill(jet1.phi(),weight);
	(histo_.getHisto())["jet_phi2"]->Fill(jet2.phi(),weight);

	(histo_.getHisto())["jet_btag_csv1"]->Fill(jet1.btag(),weight);
	(histo_.getHisto())["jet_btag_csv2"]->Fill(jet2.btag(),weight);

	(histo_.getHisto())["jet_btag_cmva1"]->Fill(jet1.btag("btag_csvmva"),weight);
	(histo_.getHisto())["jet_btag_cmva2"]->Fill(jet2.btag("btag_csvmva"),weight);

	(histo_.getHisto())["jet_deta12"]->Fill(jet1.eta() - jet2.eta(),weight);

	(histo_.getHisto())["jet_dR12"]->Fill(jet1.deltaR(jet2),weight);

	TLorentzVector obj12;
	obj12 = jet1.p4() + jet2.p4();
	(histo_.getHisto())["diJet_pt"]->Fill(obj12.Pt(),weight);
	(histo_.getHisto())["diJet_eta"]->Fill(obj12.Eta(),weight);
	(histo_.getHisto())["diJet_phi"]->Fill(obj12.Phi(),weight);
	if(isMC()) (histo_.getHisto())["diJet_m"]->Fill(obj12.M(),weight);

	Jet jet3 = offlineJets->at(2);
	(histo_.getHisto())["jet_pt3"]->Fill(jet3.pt(),weight);
	(histo_.getHisto())["jet_eta3"]->Fill(jet3.eta(),weight);
	(histo_.getHisto())["jet_phi3"]->Fill(jet3.phi(),weight);
	(histo_.getHisto())["jet_btag_csv3"]->Fill(jet3.btag(),weight);
	(histo_.getHisto())["jet_btag_cmva3"]->Fill(jet3.btag("btag_csvmva"),weight);
	(histo_.getHisto())["jet_dR13"]->Fill(jet1.deltaR(jet3),weight);
	(histo_.getHisto())["jet_dR23"]->Fill(jet2.deltaR(jet3),weight);

	if(signalMC_){
		(histo_.getHisto())["template_Mbb"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["template_SFb_up"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_up"]);
		(histo_.getHisto())["template_SFb_down"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_down"]);

		(histo_.getHisto())["template_PU_up"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_up"]);
		(histo_.getHisto())["template_PU_down"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_down"]);

		(histo_.getHisto())["template_JES_up"]->Fill(obj12.M(),weight/weight_["JES_central"] * weight_["JES_up"]);
		(histo_.getHisto())["template_JES_down"]->Fill(obj12.M(),weight/weight_["JES_central"] * weight_["JES_down"]);

		(histo_.getHisto())["template_PtEff_up"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_up"]);
		(histo_.getHisto())["template_PtEff_down"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_down"]);
	}

//	if(TEST) std::cout<<"I'm out of MssmHbbSignal::fillHistograms"<<std::endl;
}

void MssmHbbSignal::writeHistograms(){
	std::string full_name;

	outputFile_->mkdir("distributions","general distributions of Jets and di-Jet objects");
	for(const auto & h: (histo_.getHisto())){
		if(h.second->GetEntries() == 0) continue; 			//scip empty histograms
		if(h.first.find("template") != std::string::npos){
			full_name = constructTemplateName(h.first);
			h.second->Write(full_name.c_str());
		}
		else {
			outputFile_->cd("distributions");
			h.second->Write();
			outputFile_->cd("");
		}
	}
}

std::string constructTemplateName(const std::string & name){
	std::string process = "bbH";
	std::string channel	= "Mbb";
	std::string energy	= "13TeV";
	std::string collaboration = "CMS";
	if(name.find("template_") == std::string::npos) {
		std::cerr<<"Wrong input at constructTemplateName finction. Interrupt"<<std::endl;
		exit(1);
	}
	std::string full_name = name.std::string::substr(name.find("template_") + std::string("template_").std::string::length());
	if(name.find("_up") != std::string::npos){
		full_name.substr(0,full_name.size() - 3);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Up";
	}
	else if(name.find("_down") != std::string::npos){
		full_name.substr(0,full_name.size() - 5);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Down";
	}

	return full_name;
}
