/*
 * ReverseB.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/ReverseB.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

ReverseB::ReverseB(const std::string & inputFilelist, const bool & lowM, const bool & test) :
JetAnalysisBase(inputFilelist,lowM,test) {
	nJets_ = 3;
	if(lowM){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
	}
	else {
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
	}

	doubleBanalysis = pDoubleB(new selectionDoubleB(inputFilelist,lowM,test));

}

ReverseB::~ReverseB() {
	// TODO Auto-generated destructor stub
}

const bool ReverseB::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	//To avoid code duplication frind class were used
	if(!doubleBanalysis->leadingJetSelection(offlineJets)) return false;

	if(TEST) std::cout<<"I'm in ReverseB::leadingJetSelection"<<std::endl;

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
	if (jet3.btag() > btag3_) return false;

	//deltaR requirements
	if (jet2.deltaR(jet3) <= dR_) return false;
	if (jet1.deltaR(jet3) <= dR_) return false;

	return true;
}

void ReverseB::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets){

	if(TEST) std::cout<<"I'm in ReverseB::fillHistograms"<<std::endl;
	//TODO: re-implement with selectionDoubleB friendness

	double weight = 1;
	if(isMC()) {
		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PileUpCentral"];
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

	Jet jet3 = offlineJets->at(2);
	(histo_.getHisto())["jet_pt3"]->Fill(jet3.pt(),weight);
	(histo_.getHisto())["jet_eta3"]->Fill(jet3.eta(),weight);
	(histo_.getHisto())["jet_phi3"]->Fill(jet3.phi(),weight);
	(histo_.getHisto())["jet_btag_csv3"]->Fill(jet3.btag(),weight);
	(histo_.getHisto())["jet_btag_cmva3"]->Fill(jet3.btag("btag_csvmva"),weight);

	if(TEST) std::cout<<"I'm out of ReverseB::fillHistograms"<<std::endl;
}
