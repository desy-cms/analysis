/**\class MssmHbb TriggerEfficiency.cc Analysis/Tools/src/TriggerEfficiency.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Rostyslav Shevchenko
//         Created:  Wed, 25 Nov 2015 14:24:08 GMT
//
//

// system include files
#include <iostream>
// 
// user include files
#include "Analysis/MssmHbb/interface/TriggerEfficiency.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

//
// constructors and destructor
//
TriggerEfficiency::TriggerEfficiency(const std::string & inputFilelist, const std::string & evtinfo) : MssmHbb(inputFilelist,evtinfo)
{

}

TriggerEfficiency::~TriggerEfficiency()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void TriggerEfficiency::setBranches(){

	//Set number of the trigger objects;
	int i = 0;
	for(const auto & triggerObject : triggerObjectName_){
		OutTree_->Branch(("N_"+triggerObject).c_str(),&NTrigObj_[i],("N_"+triggerObject+"/I").c_str());
		++i;
	}

	OutTree_->Branch("PFJet80",&PFJet80_,"PFJet80/I");
	OutTree_->Branch("PFJet60",&PFJet60_,"PFJet60/I");

	OutTree_->Branch("LeadMatch60",LeadMatch60_,"LeadMatch60[20]/I");
	OutTree_->Branch("LeadMatch80",LeadMatch80_,"LeadMatch80[20]/I");
	OutTree_->Branch("LeadMatch100",LeadMatch100_,"LeadMatch100[20]/I");
	OutTree_->Branch("LeadMatch160",LeadMatch160_,"LeadMatch160[20]/I");
	OutTree_->Branch("LeadMatch100dEta1p6",LeadMatch100dEta1p6_,"LeadMatch100dEta1p6[20]/I");
	OutTree_->Branch("doubleJetTopology",&doubleJetTolopogy_,"doubleJetTopology/I");

	BasicTree::setBranches();
}


void TriggerEfficiency::cleanVariables(){

	//Clean Basic variables
	BasicTree::cleanVariables();

	//Clean Specific for MssmHbb analysis variables
	std::fill_n(NTrigObj_,20,-100);
	std::fill_n(LeadMatch60_,20,-100);
	std::fill_n(LeadMatch80_,20,-100);
	std::fill_n(LeadMatch100_,20,-100);
	std::fill_n(LeadMatch160_,20,-100);
	std::fill_n(LeadMatch100dEta1p6_,20,-100);
	doubleJetTolopogy_ = -100;

	if(isMC()){

	}
}

void TriggerEfficiency::setTriggerObjectVars(){

	int i = 0;
	for(const auto & triggerObject : triggerObjectName_){
		NTrigObj_[i] = this->collection<TriggerObject>(triggerObject)->size();
		++i;
	}

	PFJet60_ = this->triggerResult("HLT_PFJet60_v");
	PFJet80_ = this->triggerResult("HLT_PFJet80_v");

}

bool TriggerEfficiency::matchToPF60(const Jet &jet){
	bool matched = false;
	if(jet.matched("hltL1sL1SingleJet36") && jet.matched("hltSingleCaloJet40") &&jet.matched("hltSinglePFJet60") ) matched = true;
	LeadMatch60_[jetCounter_] = matched;
	return matched;
}

bool TriggerEfficiency::matchToPF80(const Jet &jet){
	bool matched = false;
	if(jet.matched("hltL1sL1SingleJet52") && jet.matched("hltSingleCaloJet50") &&jet.matched("hltSinglePFJet80") ) matched = true;
	LeadMatch80_[jetCounter_] = matched;
	return matched;
}

bool TriggerEfficiency::matchToPF100(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() > 100 && l2->pt() > 100  && l3->pt() > 100) matched = true;
	LeadMatch100_[jetCounter_] = matched;
	return matched;
}

bool TriggerEfficiency::matchToPF160(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() > 100 && l2->pt() > 100  && l3->pt() > 160) matched = true;
	LeadMatch160_[jetCounter_] = matched;
	return matched;
}

bool TriggerEfficiency::matchToPF100dEta1p6(const Jet &jet1 , const Jet &jet2){
	bool matched = false;
	const Candidate * J1l1 = jet1.matched("hltL1sL1SingleJet52");
	const Candidate * J1l2 = jet1.matched("hltSingleCaloJet50");
	const Candidate * J1l3 = jet1.matched("hltSinglePFJet80");

	const Candidate * J2l1 = jet2.matched("hltL1sL1SingleJet52");
	const Candidate * J2l2 = jet2.matched("hltSingleCaloJet50");
	const Candidate * J2l3 = jet2.matched("hltSinglePFJet80");
	if( J1l1 && J1l2 && J1l3 && J2l1 && J2l2 && J2l3&& abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;
	LeadMatch100dEta1p6_[jetCounter_] = matched;
	return matched;
}
