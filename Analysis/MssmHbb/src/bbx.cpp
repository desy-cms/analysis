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
	nJets_ = 3;
	if(lowM){
        btag3_ = 0.46;
        btagOP3_ = 0;
	}
	else {
        btag3_ = 0.46;
        btagOP3_ = 0;
	}
	baseOutputName_ = "bbx";
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

	if(TEST){
		std::cout<<"Jet1: pt = "<<jet1.pt()<<" eta = "<<jet1.eta()<<std::endl;
		std::cout<<"Jet2: pt = "<<jet2.pt()<<" eta = "<<jet2.eta()<<std::endl;
		std::cout<<"Jet3: pt = "<<jet3.pt()<<" eta = "<<jet3.eta()<<std::endl;
	}

	return true;
}
