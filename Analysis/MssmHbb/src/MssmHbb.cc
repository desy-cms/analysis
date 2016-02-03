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
	if(this->isMC()){
		// Add MC information:
		this->crossSections("MssmHbb/Metadata/CrossSections");
		this->generatorFilter("MssmHbb/Metadata/GeneratorFilter");
		this->ShowMCInformation();
	}

	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
	this->triggerResults("MssmHbb/Events/TriggerResults");


}

MssmHbb::~MssmHbb()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

void MssmHbb::SetupStandardOutputFile(const std::string & outputFileName){

	if(this->isMC()){
		// This add to the file name name of Pt-hat bin
		outPutName_ = this->tree<Jet>("Jets")->PhysicsObjectTree<Jet>::PhysicsObjectTreeBase::TreeBase::tree()->GetFile()->GetName();
		outPutName_ = outputFileName + boost::filesystem::basename(outPutName_);
	}
	else{
		outPutName_ = outputFileName;
	}

	outPutName_ = outPutName_ + this->get_date()  + ".root";

	//Create output TFile
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

	//Add trees with different Trigger Objects and specify Trigger Logic name
	for(const auto & triggerObject : triggerObjectName_)
	{
		this->addTree<TriggerObject>(triggerObject,(path + triggerObject).c_str());

		if(triggerObject.find("MaxDeta1p6") != std::string::npos) {
			triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
			lowMSelection_ = true;
		}

	}

	if(!lowMSelection_){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
	}

}

//For double BTag Selection
bool MssmHbb::OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,
							  const analysis::tools::Jet &sLeadOfflineJet){

	for(const auto & triggerObject : triggerObjectName_){

		const Candidate *onlineJet1 = fLeadOfflineJet.matched(triggerObject);
		const Candidate *onlineJet2 = sLeadOfflineJet.matched(triggerObject);

		if(!onlineJet1 || !onlineJet2 || onlineJet1 == onlineJet2) return false;
		//Check dEta condition
		if(triggerObject.find("MaxDeta1p6") != std::string::npos){
			if(std::abs(onlineJet1->eta() - onlineJet2-> eta()) > 1.6) return false;
		}
	}

	return true;
}

//For triple BTag Selection
bool MssmHbb::OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,
							  const analysis::tools::Jet &sLeadOfflineJet,
							  const analysis::tools::Jet &thLeadOfflineJet){

	for(const auto & triggerObject : triggerObjectName_){

		const Candidate *onlineJet1 = fLeadOfflineJet.matched(triggerObject);
		const Candidate *onlineJet2 = sLeadOfflineJet.matched(triggerObject);
		const Candidate *onlineJet3 = thLeadOfflineJet.matched(triggerObject);

		if(!onlineJet1 || !onlineJet2 || !onlineJet3 || onlineJet1 == onlineJet2 ||
														onlineJet1 == onlineJet3 ||
														onlineJet2 == onlineJet3) return false;
		//Check dEta condition
		if(triggerObject.find("MaxDeta1p6") != std::string::npos){
			if(std::abs(onlineJet1->eta() - onlineJet2->eta()) > 1.6 &&
			   std::abs(onlineJet1->eta() - onlineJet3->eta()) > 1.6 &&
			   std::abs(onlineJet2->eta() - onlineJet3->eta()) > 1.6) return false;
		}
	}

	return true;
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

void MssmHbb::ShowMCInformation(){

	//Show MC information
	if(this->isMC()){
		this->listCrossSections();
		this->listGeneratorFilter();
	}
	else {
		std::cout<<"This is not MC"<<std::endl;
	}
}

std::string MssmHbb::get_date(){

	time_t now;
	char the_date[12];
	the_date[0] = '\0';

	now = time(NULL);

   if (now != -1)
   {
      std::strftime(the_date, 12, "%d_%m_%Y", gmtime(&now));
   }

   return std::string(the_date);
}


//
// member functions
//
// ------------ method called for each event  ------------

