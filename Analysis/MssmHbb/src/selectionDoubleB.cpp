/*
 * doubleB.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/selectionDoubleB.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

selectionDoubleB::selectionDoubleB(const std::string & inputFilelist, const bool & lowM, const bool & test) :
JetAnalysisBase(inputFilelist,lowM,test) {
	nJets_ = 2;
	if(lowM){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
        pt1_ = 100.; pt2_ = 100.;
        btag1_ = 0.935; btag2_ = 0.935;
	}
	else {
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
        pt1_ = 160.; pt2_ = 160.;
        btag1_ = 0.8; btag2_ = 0.8;
        btagOP1_ = 1; btagOP2_ = 1;
	}

}

selectionDoubleB::~selectionDoubleB() {
	// TODO Auto-generated destructor stub
}

const bool selectionDoubleB::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){
//	if(TEST) std::cout<<"I'm in doubleB::leadingJetSelection"<<std::endl;
	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	//Pt requirements
	if (jet1.pt() < pt1_) return false;
	if (jet2.pt() < pt2_) return false;

	//Eta requirements
	if (std::abs(jet1.eta()) > eta1_) return false;
	if (std::abs(jet2.eta()) > eta2_) return false;

	//BTag requirements
	if (jet1.btag() < btag1_) return false;
	if (jet2.btag() < btag2_) return false;

	//dEta in case of LowM selection
	if(lowM_){
		if(std::abs(jet1.eta() - jet2.eta()) > dEta_) return false;
	}

	//deltaR requirements
	if (jet1.deltaR(jet2) <= dR_) return false;

	return true;
}

void selectionDoubleB::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){
//	if(TEST) std::cout<<"I'm in doubleB::fillHistograms"<<std::endl;

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

}

const double selectionDoubleB::assignWeight(){
	double weight = 1;
	if(isMC()) {
		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PU_central"];
	}
	return weight;
}
