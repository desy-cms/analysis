/*
 * JetAnalysisBase.cpp
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */

#include "JetAnalysisBase.h"

using namespace analysis;
using namespace analysis::mssmhbb;

JetAnalysisBase::JetAnalysisBase(const std::string & inputFilelist, const std::string & evtinfo, const bool lowM) :
								 Analysis(inputFilelist,evtinfo),
								 lowM_(lowM),
								 triggerLogicName_(){

	if(this->isMC()){
		//Add specific to MC trees
		this->addTree<GenParticle>("GenParticles","MssmHbb/Events/prunedGenParticles");
		this->addtree<Jet>("GenJets","MssmHbb/Events/slimmedGenJets");

		// Add MC information:
		this->crossSections("MssmHbb/Metadata/CrossSections");
		this->generatorFilter("MssmHbb/Metadata/GeneratorFilter");

		// Show MC information
		this->listCrossSections();
		this->listGeneratorFilter();
	}

	//Trigger trees
	this->triggerResults("MssmHbb/Events/TriggerResults");

	// Tree for Jets
	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppiReapplyJEC");

	// Tree for Vertices
	this->addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices");

}

JetAnalysisBase::~JetAnalysisBase() {
	// TODO Auto-generated destructor stub
}

JetAnalysisBase::SetupAnalysis(const std::string & json){

	this->processJsonFile(json);
	this->loadCorrections();
}

void JetAnalysisBase::addTriggerObjects(const std::vector<std::string> &triggerObjectName, const std::string & path)
{

	if(triggerObjectName.size() == 0)
	{
		std::cerr<<"Error: Empty vector of triggerObjectNames were sepcified. Interupt!"<<std::endl;
		exit(1);
	}

	//Add trees with different Trigger Objects and specify Trigger Logic name
	for(const auto & triggerObject : triggerObjectName)
	{
		this->addTree<TriggerObject>(triggerObject,(path + triggerObject).c_str());
	}
}

void JetAnalysisBase::loadCorrections(){

	if(lowM_){
		//Online BTag Trigger Efficiency produced by Ye Chen
		fCorrections_["fRelBTagEff"] = pTFile(new TFile("input_corrections/RelOnlineBTagCSV0p9Eff_PtEta.root","read") );
		hCorrections1D_["hRelBTagEff0p9"] 		=	pTH1D (fCorrections_["fRelBTagEff"] -> Get("heh4") );			// eta <0.9
		hCorrections1D_["hRelBTagEff0p9_1p4"] 	=	pTH1D (fCorrections_["fRelBTagEff"] -> Get("heh3") );		// 1.4 > eta >0.9
		hCorrections1D_["hRelBTagEff1p4_2p5"] 	= 	pTH1D (fCorrections_["fRelBTagEff"] -> Get("heh2") );		// 2.5 > eta > 1.4

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtLowMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"] = pTH2D ( fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] = pTH1D ( fCorrections_["fHtWeight"] -> Get("hRatio") );
	}
	else {

		// For high mass trigger only 2D efficiency were provided
		fCorrections_["fRelBTagEff"] = pTFile (new TFile("input_corrections/TwoDBTagCSV0p85_2D_PtEta.root","read") );
		hCorrections2D_["hRelBTagEff2D"] = pTH2D( fCorrections_["fRelBTagEff"] ->Get("h2ehn") );

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtHighMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"]  = pTH2D (fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] = pTH1D ( fCorrections_["fHtWeight"] -> Get("hRatio") );

	}

	BTagCalibrationLib_ = std::unique_ptr<BTagCalibration> (new BTagCalibration("csvv2", "input_corrections/SFbLib.csv") );
	SFb_["J12_central"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_,               // calibration instance
            													BTagEntry::OP_TIGHT,  // operating point
																"mujets",               // measurement type
																"central"));           // systematics type);
	SFb_["J12_up"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_, BTagEntry::OP_TIGHT, "mujets", "up") );
	SFb_["J12_down"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_, BTagEntry::OP_TIGHT, "mujets", "down") );

	SFb_["J3_central"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_, BTagEntry::OP_LOOSE, "mujets", "central") );
	SFb_["J3_up"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_, BTagEntry::OP_LOOSE, "mujets", "up") );
	SFb_["J3_down"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_, BTagEntry::OP_LOOSE, "mujets", "down") );

}



