/**\class MssmHbb MssmHbb.cc Analysis/Tools/src/MssmHbb.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include <iostream>
// 
// user include files
#include "Analysis/MssmHbb/interface/MssmHbb.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

//
// constructors and destructor
//
MssmHbb::MssmHbb(const std::string & inputFilelist, const std::string & evtinfo) : Analysis(inputFilelist,evtinfo)
{
	//Show MC information
	if(this->isMC()){
		this->crossSections("MssmHbb/Metadata/CrossSections");
		this->listCrossSections();

		this->generatorFilter("MssmHbb/Metadata/GeneratorFilter");
		this->listGeneratorFilter();
	}

}

MssmHbb::~MssmHbb()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void MssmHbb::setupDoubleBTagStudy(const std::string & outputFileName){

	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
	this->triggerResults("MssmHbb/Events/TriggerResults");

	if(this->isMC()){
			outPutName_ = this->tree<Jet>("Jets")->PhysicsObjectTree<Jet>::PhysicsObjectTreeBase::TreeBase::tree()->GetFile()->GetName();
			outPutName_ = outputFileName + boost::filesystem::basename(outPutName_) + ".root";
		}
	else{
		outPutName_ = outputFileName + ".root";
	}

	this->createOutputFile(outPutName_);
}

void MssmHbb::addTriggerObjects(const std::vector<std::string> &triggerObjectName, const std::string & path)
{

	triggerObjectName_ = triggerObjectName;
	if(triggerObjectName_.size() == 0)
	{
		std::cerr<<"Error: Empty vector of triggerObjectNames were sepcified. Interupt!"<<std::endl;
		exit(0);
	}

	//TODO: implement switcher from LowM to HighM triggers!!
	triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v"; lowMSelection_ = true;
	for(const auto & triggerObject : triggerObjectName_)
	{
		this->addTree<TriggerObject>(triggerObject,(path + triggerObject).c_str());
		/*
		if(triggerObject.find("MaxDeta1p6") != std::string::npos) {
			triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v"; lowMSelection_ = true;
		}
		else {
			triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
			lowMSelection_ = false;
		}
		*/
	}

}




bool MssmHbb::lowMOnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,const analysis::tools::Jet &sLeadOfflineJet)
{
	//This is for the doubleBTagSelection
	bool status = true;
	
	//Step by step:
	// 1) First Level trigger matching
	if(!fLeadOfflineJet.matched("hltL1sL1DoubleJetC100") || !sLeadOfflineJet.matched("hltL1sL1DoubleJetC100") || fLeadOfflineJet.matched("hltL1sL1DoubleJetC100") == sLeadOfflineJet.matched("hltL1sL1DoubleJetC100")) return false; // check - if jets are mutched
	
	// 2) Second Level trigger matching:
	if(!fLeadOfflineJet.matched("hltDoubleJetsC100") || !sLeadOfflineJet.matched("hltDoubleJetsC100") || fLeadOfflineJet.matched("hltDoubleJetsC100") == sLeadOfflineJet.matched("hltDoubleJetsC100")) return false;
	
	// 3) 2p5 Level trigger matching:
	if(!fLeadOfflineJet.matched("hltDoublePFJetsC100") || !sLeadOfflineJet.matched("hltDoublePFJetsC100") || fLeadOfflineJet.matched("hltDoublePFJetsC100") == sLeadOfflineJet.matched("hltDoublePFJetsC100")) return false;
	
	// 4) b-tagging matching
	if(!fLeadOfflineJet.matched("hltDoubleBTagCSV0p9") || !sLeadOfflineJet.matched("hltDoubleBTagCSV0p9") || fLeadOfflineJet.matched("hltDoubleBTagCSV0p9") == sLeadOfflineJet.matched("hltDoubleBTagCSV0p9")) return false;
	
	// 5) Third Level matching 
	if(fLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6") && sLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6") && fLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6") != sLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6") )
	{
		const Candidate *onlineJet1 = fLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6");
		const Candidate *onlineJet2 = sLeadOfflineJet.matched("hltDoublePFJetsC100MaxDeta1p6");
		if( std::abs(onlineJet1->eta() - onlineJet2-> eta()) > 1.6) status = false; //Check Pt and dEta condition	else return false;
	}
	
	else return false;
	return status;
}



//
// member functions
//
// ------------ method called for each event  ------------

