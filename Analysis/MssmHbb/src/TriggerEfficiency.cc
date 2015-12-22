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
TriggerEfficiency::TriggerEfficiency(const std::string & inputFilelist, const std::string & evtinfo) : Analysis(inputFilelist,evtinfo)
{
	OutTree = new TTree("OutTree","");
}

TriggerEfficiency::~TriggerEfficiency()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

//......Add your methods ......

void TriggerEfficiency::addTriggerObjects(const std::vector<std::string> &triggerObjectName, const std::string & path)
{
	if(triggerObjectName.size() == 0)
	{
		std::cerr<<"Error: Empty vector of triggerObjectNames were sepcified. Interupt!"<<std::endl;
		exit(0);
	}
	
	triggerObjectName_ = triggerObjectName;
	for(const auto & triggerObject : triggerObjectName)
	{
		this->addTree<TriggerObject>(triggerObject,(path + triggerObject).c_str());
	}
	
}

void TriggerEfficiency::fillTree(){
	OutTree ->Fill();
}

void TriggerEfficiency::writeTree(){
	OutTree ->Write();
}

bool TriggerEfficiency::matchToPF60(const Jet &jet){
	bool matched = false;
	if(jet.matched("hltL1sL1SingleJet36") && jet.matched("hltSingleCaloJet40") &&jet.matched("hltSinglePFJet60") ) matched = true;
	return matched;
}

bool TriggerEfficiency::matchToPF80(const Jet &jet){
	bool matched = false;
	if(jet.matched("hltL1sL1SingleJet52") && jet.matched("hltSingleCaloJet50") &&jet.matched("hltSinglePFJet80") ) matched = true;
	return matched;
}

bool TriggerEfficiency::matchToPF100(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() > 100 && l2->pt() > 100  && l3->pt() > 100) matched = true;
	return matched;
}

void TriggerEfficiency::setBranches(){

	OutTree->Branch("LeadPt", LeadPt,"LeadPt[4]/D");
	OutTree->Branch("LeadEta", LeadEta,"LeadEta[4]/D");
	OutTree->Branch("LeadPhi", LeadPhi,"LeadPhi[4]/D");
	OutTree->Branch("LeadBTag", LeadBTag,"LeadBTag[4]/D");
	OutTree->Branch("dPhiFS",&dPhiFS,"dPhiFS/D");
	OutTree->Branch("dEtaFS",&dEtaFS,"dEtaFS/D");
	OutTree->Branch("Njets",&Njets,"Njets/I");

	OutTree->Branch("Njets80",&Njets80,"Njets80/I");

	OutTree->Branch("hpf60for100_Num",&hpf60for100_Num,"hpf60for100_Num/D");
	OutTree->Branch("hpf60for100_Denum",&hpf60for100_Denum,"hpf60for100_Denum/D");
	OutTree->Branch("hpf60for80_Num",&hpf60for80_Num,"hpf60for80_Num/D");
	OutTree->Branch("hpf60for80_Denum",&hpf60for80_Denum,"hpf60for80_Denum/D");
	OutTree->Branch("hpf100_Num",hpf100_Num,"hpf100_Num[4]/D");
	OutTree->Branch("hpf100_Denum",hpf100_Denum,"hpf100_Denum[4]/D");

	OutTree->Branch("TnP40_tag",&TnP40_tag,"TnP40_tag/D");
	OutTree->Branch("TnP40_probe",&TnP40_probe,"TnP40_probe/D");
	OutTree->Branch("TnP60_tag",&TnP60_tag,"TnP60_tag/D");
	OutTree->Branch("TnP60_probe",&TnP60_probe,"TnP60_probe/D");
	OutTree->Branch("TnP80_tag",&TnP80_tag,"TnP80_tag/D");
	OutTree->Branch("TnP80_probe",&TnP80_probe,"TnP80_probe/D");
	OutTree->Branch("TnP100_tag",TnP100_tag,"TnP100_tag[4]/D");
	OutTree->Branch("TnP100_probe",TnP100_probe,"TnP100_probe[4]/D");

	OutTree->Branch("LeadMatch60",LeadMatch60,"LeadMatch60[4]/I");
	OutTree->Branch("LeadMatch80",LeadMatch80,"LeadMatch80[4]/I");
	OutTree->Branch("LeadMatch100",LeadMatch100,"LeadMatch100[4]/I");

	OutTree->Branch("NL1Object",&NL1Object,"NL1Object/I");
	OutTree->Branch("NL2Object",&NL2Object,"NL2Object/I");
	OutTree->Branch("NL3Object",&NL3Object,"NL3Object/I");

	OutTree->Branch("weightPtTnP",&weightPtTnP,"weightPtTnP/D");
	OutTree->Branch("weightPtRef",&weightPtRef,"weightPtRef/D");
	OutTree->Branch("weightPtComb",&weightPtComb,"weightPtComb/D");

	OutTree->Branch("ptVeto",&ptVeto,"ptVeto/D");
	OutTree->Branch("M12",&M12,"M12/D");
	if(this->isMC()){
		OutTree->Branch("mcModel_Num",mcModel_Num,"mcModel_Num[4]/D");
		OutTree->Branch("mcModel_Denum",mcModel_Denum,"mcModel_Denum[4]/D");
		OutTree->Branch("weightLumi",&weightLumi,"weightLumi/D");
	}
}

void TriggerEfficiency::zeroingBranches(){

	std::fill_n(LeadPt,4,-100.);
	std::fill_n(LeadEta,4,-100.);
	std::fill_n(LeadPhi,4,-100.);
	std::fill_n(LeadBTag,4,-100.);

    Njets = -100;
    Njets80 = -100;

    dPhiFS 	= -100;
    dEtaFS 	= -100;
    Njets = -100;

    // Reference efficiency variables:
    hpf60for100_Num = -100;
    hpf60for100_Denum = -100;
    hpf60for80_Num = -100;
    hpf60for80_Denum = -100;
    std::fill_n(hpf100_Num,4,-100);
    std::fill_n(hpf100_Denum,4,-100);

    // T&P efficiency variables:
    TnP60_tag = -100;
    TnP60_probe = -100;
    TnP80_tag = -100;
    TnP80_probe = -100;
    std::fill_n(TnP100_tag,4,-100.);
    std::fill_n(TnP100_probe,4,-100);

    //Trigger Matching variables:
    std::fill_n(LeadMatch60,4,0);
    std::fill_n(LeadMatch80,4,0);
    std::fill_n(LeadMatch100,4,0);

    //Trigger Object variables:
    	   //L1
    NL1Object = -100;
    	   //L2
    NL2Object = -100;
    	   //L3
    NL3Object = -100;
    //Other variables
    ptVeto = -100;
    M12 = -100;

    //Only MC variables:
    std::fill_n(mcModel_Num,4,-100);
    std::fill_n(mcModel_Denum,4,-100);
    weightPtTnP = -100;
    weightPtRef = -100;
    weightPtComb = -100;
    weightLumi = -100;
}

