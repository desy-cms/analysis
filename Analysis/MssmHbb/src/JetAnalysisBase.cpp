/*
 * JetAnalysisBase.cpp
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

JetAnalysisBase::JetAnalysisBase(const std::string & inputFilelist, const std::string & evtinfo, const bool & lowM) :
								 Analysis(inputFilelist,evtinfo),
								 lowM_(lowM),
								 triggerLogicName_(),
								 nJets_(1){

	if(this->isMC()){
		//Add specific to MC trees
		this->addTree<GenParticle>("GenParticles","MssmHbb/Events/prunedGenParticles");
		this->addTree<Jet>("GenJets","MssmHbb/Events/slimmedGenJets");

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

void JetAnalysisBase::setupAnalysis(const std::string & json){
	this->processJsonFile(json);
	this->loadCorrections();
}

void JetAnalysisBase::applySelection(){

	auto goodLeadingJets = false;
	TLorentzVector diJetObject;
	Jet LeadJet[5];
	//Event loop:
	for(int i = 0; i < this->size(); ++ i){
		this->event(i);

		//Select only good JSon runs for Data
		if(!isMC() && !this->selectJson() ) continue;

		//Trigger Selection
		if(!isMC() && !this->triggerResult(triggerLogicName_)) continue;

	    //Define Jet Collection
		auto offlineJets = this->collection<Jet>("Jets");

		//Define MC specific collections:
		if(isMC()){
			auto genJets = this->collection<Jet>("GenJets");
			auto genPart = this->collection<GenParticle>("GenParticles");
		}

		if(offlineJets->size() < nJets_ ) continue;

		//Match offline Jets to online Objects
		if(!isMC()) this->match<Jet,TriggerObject>("Jets",this->getTriggerObjectNames());

	    //Define Vertex collection
	    auto offlinePrimaryVertices = this->collection<Vertex>("Vertices");

	    goodLeadingJets = false;
	    auto Ht = 0.;
	    //Declare SFb
	    ScaleFactor sf[5];

	    //Jet Selection
	    for( int iJet = 0; iJet < offlineJets -> size(); ++iJet){
	    	Jet jet = offlineJets->at(iJet);

	    	//ht calculation
	    	if(jet.idLoose()) Ht +=jet.pt();

	    	if(iJet < nJets_){
	    		//Selection of good Leading Jets:
	    		if(!this->leadingJetSelection(iJet,jet)) break;
	    		LeadJet[iJet] = jet;
	    		goodLeadingJets = true;
	    		//Calculate SFb
	    		if(isMC()){
	    			if(iJet < 2){
	    				sf[iJet] = calculateBTagSF(jet,false);
	    			}
	    			if(iJet == 2){
	    				sf[iJet] = calculateBTagSF(jet,true);
	    			}
	    		}
	    	}
	    }
	    if(!goodLeadingJets) continue;

	    //Should return true for trigger efficiency study
	    if(!isMC() && !OnlineSelection(LeadJet[0],LeadJet[1])) continue;

	    //Weights:
	    if(isMC()){

        	  //.........................Calculate weights...............
        	  // general weights which is independent of selection type OR already includes
        	  // selection depends criterias inside.
        	  weight_["FactorPt"] 		= pWeight_->FactorizationPtWeight(LeadJet[0].pt(), LeadJet[1].pt());
        	  weight_["dEta"]     		= pWeight_->dEtaWeight(abs(LeadJet[0].eta() - LeadJet[1].eta()));
        	  weight_["2DPt"]     		= pWeight_->TwoDPtWeight(hCorrections2D_["hPtTriggerEff"].get(),LeadJet[0].pt(),LeadJet[1].pt());
        	  //TODO: Data luminosity!!!
        	  weight_["Lumi"] 			=pWeight_->LumiWeight(2318.278306,this->luminosity());
        	  weight_["Ht"]       		= pWeight_->HtWeight(hCorrections1D_["hHtWeight"].get(),Ht);
        	  weight_["PileUpCentral"] 	= pWeight_->PileUpWeight(hCorrections1D_["fPipeUpData_central"].get(),hCorrections1D_["fPipeUpMC"].get(),this->nTruePileup());
        	  weight_["PileUpDown"]    	= pWeight_->PileUpWeight(hCorrections1D_["fPipeUpData_down"].get(),hCorrections1D_["fPipeUpMC"].get(),this->nTruePileup());
        	  weight_["PileUpUp"]      	= pWeight_->PileUpWeight(hCorrections1D_["fPipeUpData_up"].get(),hCorrections1D_["fPipeUpMC"].get(),this->nTruePileup());

        	  //Selection depending weights
        	  if(lowM_){
            	  weight_["BTag"] = pWeight_->BTagWeight(hCorrections1D_["hRelBTagEff0p9"].get(),hCorrections1D_["hRelBTagEff0p9_1p4"].get(),hCorrections1D_["hRelBTagEff1p4_2p5"].get(),LeadJet[0].pt(),LeadJet[0].eta()) *
            			  	  	   pWeight_->BTagWeight(hCorrections1D_["hRelBTagEff0p9"].get(),hCorrections1D_["hRelBTagEff0p9_1p4"].get(),hCorrections1D_["hRelBTagEff1p4_2p5"].get(),LeadJet[1].pt(),LeadJet[1].eta());
        	  }
        	  else {
            	  weight_["BTag"] = pWeight_->BTagWeight(hCorrections2D_["hRelBTagEff2D"].get(), LeadJet[0].pt(),LeadJet[0].eta())*
            			  	  	   pWeight_->BTagWeight(hCorrections2D_["hRelBTagEff2D"].get(), LeadJet[1].pt(), LeadJet[1].eta());
        	  }

	    }

	    diJetObject = LeadJet[0].p4() + LeadJet[1].p4();

	}

}

bool JetAnalysisBase::leadingJetSelection(const int & iJet, const tools::Jet & Jet){
	bool flag = false;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	if(!Jet.idLoose()) return false;
	if(Jet.pt() < pt1_) return false;
	if(Jet.eta() > eta1_) return false;

	flag = true;
	return flag;
}

bool JetAnalysisBase::OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,
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

void JetAnalysisBase::addTriggerObjects(const std::vector<std::string> &triggerObjectName, const std::string & path)
{

	triggerObjectName_ = triggerObjectName;
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
		hCorrections1D_["hRelBTagEff0p9"] 		=	pTH1D ( (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh4") );			// eta <0.9
		hCorrections1D_["hRelBTagEff0p9_1p4"] 	=	pTH1D ((TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh3") );		// 1.4 > eta >0.9
		hCorrections1D_["hRelBTagEff1p4_2p5"] 	= 	pTH1D ((TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh2") );		// 2.5 > eta > 1.4

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtLowMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"] = pTH2D ((TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] = pTH1D ((TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") );

	}
	else {

		// For high mass trigger only 2D efficiency were provided
		fCorrections_["fRelBTagEff"] = pTFile (new TFile("input_corrections/TwoDBTagCSV0p85_2D_PtEta.root","read") );
		hCorrections2D_["hRelBTagEff2D"] = pTH2D((TH2D*) fCorrections_["fRelBTagEff"] ->Get("h2ehn") );

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtHighMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"]  = pTH2D ((TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] = pTH1D ((TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") );

	}
	fCorrections_["fPipeUpData_central"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_central.root","read") );
	fCorrections_["fPipeUpData_up"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_up.root","read") );
	fCorrections_["fPipeUpData_down"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_down.root","read") );
	hCorrections1D_["hPileUpData_central"] = pTH1D((TH1D*) fCorrections_["fPipeUpData_central"]->Get("pileup"));
	hCorrections1D_["hPileUpData_up"] = pTH1D((TH1D*) fCorrections_["fPipeUpData_up"]->Get("pileup"));
	hCorrections1D_["hPileUpData_down"] = pTH1D((TH1D*) fCorrections_["fPipeUpData_down"]->Get("pileup"));
	fCorrections_["fPipeUpMC"] = pTFile(new TFile("input_corrections/MC_Fall15_PU25_V1.root","read") );
	hCorrections1D_["hPileUpMC"] = pTH1D((TH1D*) fCorrections_["fPipeUpMC"] -> Get("pileup") );



	BTagCalibrationLib_ = std::unique_ptr<BTagCalibration> (new BTagCalibration("csvv2", "input_corrections/SFbLib.csv") );
	SFb_["J12_central"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(),               // calibration instance
            													BTagEntry::OP_TIGHT,  // operating point
																"mujets",               // measurement type
																"central"));           // systematics type);
	SFb_["J12_up"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(), BTagEntry::OP_TIGHT, "mujets", "up") );
	SFb_["J12_down"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(), BTagEntry::OP_TIGHT, "mujets", "down") );

	SFb_["J3_central"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(), BTagEntry::OP_LOOSE, "mujets", "central") );
	SFb_["J3_up"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(), BTagEntry::OP_LOOSE, "mujets", "up") );
	SFb_["J3_down"] = pSFReader (new BTagCalibrationReader(BTagCalibrationLib_.get(), BTagEntry::OP_LOOSE, "mujets", "down") );

	//Declare weights:
	pWeight_ = std::unique_ptr<Weights>(new Weights(lowM_));

}

const JetAnalysisBase::ScaleFactor JetAnalysisBase::calculateBTagSF(const tools::Jet & jet, const bool & thirdJ){

	double btagJetPt = 0;
	btagJetPt = jet.pt();
	bool DoubleUncertainty = false;
	if(btagJetPt >= 670.){
		btagJetPt = 669; //Doesn't work at the limit, so that I add "-1"
		DoubleUncertainty = true;
	}

	ScaleFactor sf;

	if(!thirdJ){
		sf.central 	= SFb_["J12_central"]->eval(BTagEntry::FLAV_B, jet.eta(), btagJetPt);
		sf.up		= SFb_["J12_up"]->eval(BTagEntry::FLAV_B,jet.eta(), btagJetPt);
		sf.down		= SFb_["J12_down"]->eval(BTagEntry::FLAV_B,jet.eta(), btagJetPt);
	}
	else {
		sf.central 	= SFb_["J3_central"]->eval(BTagEntry::FLAV_B, jet.eta(), btagJetPt);
		sf.up		= SFb_["J3_up"]->eval(BTagEntry::FLAV_B,jet.eta(), btagJetPt);
		sf.down		= SFb_["J3_down"]->eval(BTagEntry::FLAV_B,jet.eta(), btagJetPt);
	}

	// If Pt > Maximum - uncertinties should be doubled and calculated with the extremum value
	if(DoubleUncertainty){
		sf.up 	= 2*(sf.up - sf.central) + sf.central;
		sf.down = 2*(sf.down - sf.central) + sf.central;
	}

	return sf;
}

void JetAnalysisBase::makeHistograms(const int & size){
	histo_.Make(size);
}

