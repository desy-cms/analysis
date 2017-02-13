/*
 * bbx.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/bbx.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

bbx::bbx(const std::string & inputFilelist,const double & dataLumi, const bool & lowM, const bool & test) :
		DataMcComparison(inputFilelist,dataLumi,lowM,3,test)
{
	baseOutputName_ = "bbx";
	nJets_ = 3;
	JESshift_ = 0;
	JERshift_ = 0;
    if(lowM){
    	triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v";
    	triggerObjectName_ = {"hltL1sDoubleJetC100","hltDoubleJetsC100","hltBTagCaloCSVp014DoubleWithMatching","hltDoublePFJetsC100","hltDoublePFJetsC100MaxDeta1p6"};
    	pt1_ = 100.; pt2_ = 100.; pt3_ = 40;
    	eta1_ = 2.2; eta2_ = 2.2; eta3_ = 2.2;
    	btag1_ = 0.8484; btag2_ = 0.8484; btag3_ = 0.5426;
    	btagOP1_ = 1; btagOP2_ = 1; btagOP3_ = 1;
    	dR_ = 1; dEta_ = 1.55; mHat_ = 0.7;
    }
    else {
    	triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV_p026_DoublePFJetsC160_v";
    	triggerObjectName_ = {"hltL1sDoubleJetC100","hltDoubleJetsC100","hltBTagCaloCSVp026DoubleWithMatching","hltDoublePFJetsC160"};
    	pt1_ = 160.; pt2_ = 160.; pt3_ = 40;
    	eta1_ = 2.2; eta2_ = 2.2; eta3_ = 2.2;
    	btag1_ = 0.8484; btag2_ = 0.8484; btag3_ = 0.5426;
    	btagOP1_ = 1; btagOP2_ = 1; btagOP3_ = 1;
    	dR_ = 1; dEta_ = 100.; mHat_ = 0.7;
    }
}

bbx::~bbx() {
}

const bool bbx::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	//To avoid code duplication friend class were used
	if(!selectionDoubleB::leadingJetSelection(offlineJets)) return false;

	if(TEST) std::cout<<"I'm in bbx::leadingJetSelection"<<std::endl;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);

	//Pt requirements
//	if (jet3.pt() < pt3_) return false;
	if(!cuts_.check("pt3",jet3.pt() >= pt3_)) return false;

	//Eta requirements
//	if (std::abs(jet3.eta()) > eta3_) return false;
	if(!cuts_.check("eta3",std::abs(jet3.eta()) <= eta3_)) return false;

	//BTag requirements
//	if (jet3.btag() > btag3_) return false;
	if(!cuts_.check("btag3",jet3.btag() <= btag3_)) return false;

	//deltaR requirements
//	if(jet2.deltaR(jet3) <= dR_) return false;
	if(!cuts_.check("dR23",jet2.deltaR(jet3) > dR_)) return false;
//	if(jet1.deltaR(jet3) <= dR_) return false;
	if(!cuts_.check("dR13",jet1.deltaR(jet3) > dR_)) return false;

	return true;
}
