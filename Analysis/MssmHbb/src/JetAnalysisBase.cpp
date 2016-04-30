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

const bool findStrings(const std::string & input, const std::string & needful);

JetAnalysisBase::JetAnalysisBase(const std::string & inputFilelist, const bool & lowM, const bool & test) :
								 Analysis(inputFilelist,"MssmHbb/Events/EventInfo"),
								 lowM_(lowM),
								 triggerLogicName_(""),
								 nJets_(1),
								 TEST(test){
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

		if(findStrings(inputFilelist,"susy")) signalMC_ = true;
		else signalMC_ = false;

		JESshift_ = 0;
	}

	//Trigger trees
	this->triggerResults("MssmHbb/Events/TriggerResults");

	// Tree for Jets
	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppiReapplyJEC");

	// Tree for Vertices
	this->addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices");

	triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
}

JetAnalysisBase::~JetAnalysisBase() {
	if(TEST) std::cout<<"I'm at ~JetAnalysisBase"<<std::endl;
	// TODO Auto-generated destructor stub
}

void JetAnalysisBase::setupAnalysis(const std::string & json){
	if(!isMC()) this->processJsonFile(json);
	this->loadCorrections();
	this->addTriggerObjects(triggerObjectName_);
	if(TEST) std::cout<<"I'm in setupAnalysis"<<std::endl;
}

void JetAnalysisBase::applySelection(){

	if(TEST) std::cout<<"I'm in applySelection"<<std::endl;
	if(TEST) std::cout<<"NJets: "<<nJets_<<std::endl;

	bool goodLeadingJets = false;
	double totWeight = 0;
	TLorentzVector diJetObject;
	double Ht;
	Jet LeadJet[5];
	ScaleFactor sf[5];
	//Event loop:
	auto nevents = 0;
	if(TEST) nevents = 50;
	else nevents = this->size();

	for(auto i = 0; i < nevents; ++ i){
		this->event(i);

		//Select only good JSon runs for Data
		if(!isMC() && !this->selectJson() ) continue;

		//Trigger Selection
		if(!isMC() && !this->triggerResult(triggerLogicName_)) continue;

	    //Define Jet Collection
		auto offlineJets = this->collection<Jet>("Jets");
		auto shiftedJets = std::make_shared<Collection<Jet> >();
//		auto shiftedJets = std::shared_ptr<Collection<Jet> >( new Collection<Jet>);

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
	    Ht = 0.;

	    //Jet Selection
	    for( int iJet = 0; iJet < offlineJets -> size(); ++iJet){
	    	Jet jet = offlineJets->at(iJet);
	    	shiftedJets = this->modifyJetCollection(jet,shiftedJets);

	    	//ht calculation
	    	if(jet.idLoose()) Ht +=jet.pt();

	    	if(iJet < nJets_){
	    		//Selection of good Leading Jets:
	    		if(!jet.idLoose()) break;
//	    		if(!this->leadingJetSelection(shiftedJets)) break;

	    	    //Clear SFb
	    	    sf[iJet].clear();

	    		LeadJet[iJet] = jet;

	    		//Calculate SFb
	    		if(isMC()){
	    			if(iJet < 2){
//	    				sf[iJet] = calculateBTagSF(jet, btagOP1_);
	    			}
	    			if(iJet == 2){
//	    				sf[iJet] = calculateBTagSF(jet,btagOP3_);
	    			}
	    		}

	    		if(iJet == nJets_ - 1) goodLeadingJets = true;
	    	}
	    }
	    if(!goodLeadingJets) continue;

	    //Should return true for trigger efficiency study
	    if(!isMC() && !OnlineSelection(shiftedJets->at(0),shiftedJets->at(1))) continue;
	    if(!this->leadingJetSelection(shiftedJets)) continue;
	    //Weights:
	    if(isMC()){

        	  //.........................Calculate weights...............
        	  // general weights which is independent of selection type OR already includes
        	  // selection depends criterias inside.
        	  weight_["FactorPt"] 		= pWeight_->FactorizationPtWeight(LeadJet[0].pt(), LeadJet[1].pt());
        	  weight_["dEta"]     		= pWeight_->dEtaWeight(abs(LeadJet[0].eta() - LeadJet[1].eta()));
        	  weight_["2DPt"]     		= pWeight_->TwoDPtWeight(hCorrections2D_["hPtTriggerEff"],LeadJet[0].pt(),LeadJet[1].pt());
        	  //TODO: Data luminosity!!!
        	  weight_["Lumi"] 			=pWeight_->LumiWeight(2318.278306,this->luminosity());
        	  weight_["Ht"]       		= pWeight_->HtWeight(hCorrections1D_["hHtWeight"],Ht);

        	  //2SIGMA variation!!!!!!
        	  weight_["PU_central"] 	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_central"],hCorrections1D_["hPileUpMC"],this->nTruePileup());
        	  weight_["PU_down"]    	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_down"],hCorrections1D_["hPileUpMC"],this->nTruePileup());
        	  weight_["PU_up"]      	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_up"],hCorrections1D_["hPileUpMC"],this->nTruePileup());

        	  weight_["PtEff_central"]	= weight_["2DPt"];
        	  weight_["PtEff_up"]		= weight_["2DPt"] + 2.*std::abs(weight_["2DPt"] - weight_["FactorPt"]);
        	  weight_["PtEff_down"]		= weight_["2DPt"] - 2.*std::abs(weight_["2DPt"] - weight_["FactorPt"]);

        	  weight_["SFl_central"] = 1;
        	  weight_["SFl_up"] = 1;
        	  weight_["SFl_down"] = 1;

        	  weight_["SFb_central"] = 1;
        	  weight_["SFb_up"] = 1;
        	  weight_["SFb_down"] = 1;

        	  //..........................SF weight...............
        	  for(int i = 0; i<nJets_;++i){
        		  if(sf[i].flavour != 0){ //only for b/c
        			  weight_["SFb_central"] 	*= sf[i].central;
        			  weight_["SFb_up"]			*= (2.*(sf[i].up - sf[i].central) + sf[i].central);
        			  weight_["SFb_down"]		*= (2.*(sf[i].down - sf[i].central) + sf[i].central);
        		  }
        		  else {
        			  weight_["SFl_central"] 	*= sf[i].central;
        			  weight_["SFl_up"]			*= (2.*(sf[i].up - sf[i].central) + sf[i].central);
        			  weight_["SFl_down"]		*= (2.*(sf[i].down - sf[i].central) + sf[i].central);
        		  }
        	  }

        	  //Selection depending weights
        	  if(lowM_){
            	  weight_["BTag"] = pWeight_->BTagWeight(hCorrections1D_["hRelBTagEff0p9"],hCorrections1D_["hRelBTagEff0p9_1p4"],hCorrections1D_["hRelBTagEff1p4_2p5"],LeadJet[0].pt(),LeadJet[0].eta()) *
            			  	  	   pWeight_->BTagWeight(hCorrections1D_["hRelBTagEff0p9"],hCorrections1D_["hRelBTagEff0p9_1p4"],hCorrections1D_["hRelBTagEff1p4_2p5"],LeadJet[1].pt(),LeadJet[1].eta());
        	  }
        	  else {
            	  weight_["BTag"] = pWeight_->BTagWeight(hCorrections2D_["hRelBTagEff2D"], LeadJet[0].pt(),LeadJet[0].eta())*
            			  	  	   pWeight_->BTagWeight(hCorrections2D_["hRelBTagEff2D"], LeadJet[1].pt(), LeadJet[1].eta());
        	  }

	    }

	    totWeight = this->assignWeight();
	    this->fillHistograms(shiftedJets,totWeight);

	}

//	this->writeHistograms();
//	outputFile_->Close();

}


const bool JetAnalysisBase::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){
	if(TEST) std::cout<<"I'm in JetAnalysisBase::leadingJetSelection::shared_ptr"<<std::endl;
	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	if(jet1.pt() < pt1_ || jet2.pt() < pt2_) return false;
	if(jet1.eta() < eta1_ || jet2.eta() < eta2_) return false;

	return true;
}

const bool JetAnalysisBase::OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,
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
	TH1::AddDirectory(0);
	if(lowM_){
		//Online BTag Trigger Efficiency produced by Ye Chen
		fCorrections_["fRelBTagEff"] = std::make_unique<TFile>("input_corrections/RelOnlineBTagCSV0p9Eff_PtEta.root","read");
		hCorrections1D_["hRelBTagEff0p9"] 		=	  (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh4") ;			// eta <0.9
		hCorrections1D_["hRelBTagEff0p9_1p4"] 	=	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh3") ;		// 1.4 > eta >0.9
		hCorrections1D_["hRelBTagEff1p4_2p5"] 	= 	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh2") ;		// 2.5 > eta > 1.4

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = std::make_unique<TFile>("input_corrections/TwoDPtLowMassEfficiency.root","read");
		hCorrections2D_["hPtTriggerEff"] = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ; // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = std::make_unique<TFile>("input_corrections/HtRatio.root","read");
		hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") ;

	}
	else {

		// For high mass trigger only 2D efficiency were provided
		fCorrections_["fRelBTagEff"] = std::make_unique<TFile>("input_corrections/TwoDBTagCSV0p85_2D_PtEta.root","read");
		hCorrections2D_["hRelBTagEff2D"] = (TH2D*) fCorrections_["fRelBTagEff"] ->Get("h2ehn") ;

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = std::make_unique<TFile>("input_corrections/TwoDPtHighMassEfficiency.root","read");
		hCorrections2D_["hPtTriggerEff"]  = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num"); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = std::make_unique<TFile>("input_corrections/HtRatio.root","read");
		hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") ;

	}
	fCorrections_["fPileUpData_central"] = std::make_unique<TFile>("input_corrections/PileUp_2015Dec_central.root","read");
	fCorrections_["fPileUpData_up"] = std::make_unique<TFile>("input_corrections/PileUp_2015Dec_up.root","read");
	fCorrections_["fPileUpData_down"] = std::make_unique<TFile>("input_corrections/PileUp_2015Dec_down.root","read");
	hCorrections1D_["hPileUpData_central"] = (TH1D*) fCorrections_["fPileUpData_central"]->Get("pileup");
	hCorrections1D_["hPileUpData_up"] = (TH1D*) fCorrections_["fPileUpData_up"]->Get("pileup");
	hCorrections1D_["hPileUpData_down"] = (TH1D*) fCorrections_["fPileUpData_down"]->Get("pileup");
	fCorrections_["fPileUpMC"] = std::make_unique<TFile>("input_corrections/MC_Fall15_PU25_V1.root","read");
	hCorrections1D_["hPileUpMC"] = (TH1D*) fCorrections_["fPileUpMC"] -> Get("pileup") ;



	BTagCalibrationLib_ = std::make_unique<BTagCalibration>("csvv2", "input_corrections/SFbLib.csv");

//	SFb_["J12_central"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),               // calibration instance
//            													BTagEntry::OP_TIGHT,  // operating point
//																"mujets",               // measurement type
//																"central");           // systematics type);
//	SFb_["J12_up"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(), BTagEntry::OP_TIGHT, "mujets", "up");
//	SFb_["J12_down"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(), BTagEntry::OP_TIGHT, "mujets", "down");
//
//	BTagEntry::OperatingPoint op;
//	if(lowM_) op = BTagEntry::OP_MEDIUM;
//	else op = BTagEntry::OP_LOOSE;
//	SFb_["J3_central"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(), op, "mujets", "central") ;
//	SFb_["J3_up"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(), op , "mujets", "up") ;
//	SFb_["J3_down"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(), op , "mujets", "down") ;

	//Declare weights:
	pWeight_ = std::unique_ptr<Weights>(new Weights(lowM_));

}

const JetAnalysisBase::ScaleFactor JetAnalysisBase::calculateBTagSF(const tools::Jet & jet,const int & op){

	std::string measurementType;
	BTagEntry::JetFlavor flav_b;
	double pt_limit = jet.pt();
	bool doubleUncertainty = false;
	if(jet.flavour() == 0){ flav_b = BTagEntry::FLAV_UDSG;	measurementType = "incl"; }
	else if(jet.flavour() == 5){ flav_b = BTagEntry::FLAV_B; measurementType = "mujets"; }
	else if(jet.flavour() == 4){ flav_b = BTagEntry::FLAV_C; measurementType = "mujets"; }
	else {std::cerr<<"non b/c/udsg flavour in JetAnalysisBase::calculateBTagSF. Exception."<<std::endl; exit(1); }

	if(measurementType == "incl" && pt_limit >= 1000){ pt_limit = 999; doubleUncertainty = true; }
	else if(measurementType == "mujets" && pt_limit >= 670. ){ pt_limit = 669; doubleUncertainty = true; }

	ScaleFactor sf;
	BTagCalibrationReader readerCentral(BTagCalibrationLib_.get(),(BTagEntry::OperatingPoint)op,measurementType,"central");
	BTagCalibrationReader readerUp(BTagCalibrationLib_.get(),(BTagEntry::OperatingPoint)op,measurementType,"up");
	BTagCalibrationReader readerDown(BTagCalibrationLib_.get(),(BTagEntry::OperatingPoint)op,measurementType,"down");

	sf.central 	= readerCentral	.eval(flav_b,jet.eta(), pt_limit);
	sf.up		= readerUp		.eval(flav_b,jet.eta(), pt_limit);
	sf.down		= readerDown	.eval(flav_b,jet.eta(), pt_limit);
	sf.flavour  = jet.flavour();

	if(doubleUncertainty){
		sf.up 	= 2*(sf.up - sf.central) + sf.central;
		sf.down = 2*(sf.down - sf.central) + sf.central;
	}



	return sf;
}

void JetAnalysisBase::makeHistograms(const int & size){
	histo_.Make(size);
}

void JetAnalysisBase::SetupStandardOutputFile(const std::string & outputFileName){

	if(outputFileName == ""){
		//get the full file name and path from the Tree
		std::string fullName = this->tree<Jet>("Jets")->PhysicsObjectTree<Jet>::PhysicsObjectTreeBase::TreeBase::tree()->GetFile()->GetName();
		std::string outputName = "Selection";
		if(lowM_) outputName += "_lowM_";
		else {outputName += "_highM_";}

		//TODO: find better solution to get the file name
		size_t found;
		found=fullName.find_last_of("/");
		fullName = fullName.substr(0,found);
		found = fullName.find_last_of("/");
		fullName = fullName.substr(0,found);
		found = fullName.find_last_of("/");
		fullName = fullName.substr(0,found);
		found = fullName.find_last_of("/");

		if(is_mc_){
			fullName = fullName.substr(0,found);
			found = fullName.find_last_of("/");
			fullName = fullName.substr(found+1);
		}
		else{
			fullName = fullName.substr(found+1);
		}

		outputName += fullName;// + ".root";
		this->createOutputFile(outputName);
	}
	else{
		this->createOutputFile(outputFileName);
	}

}

void JetAnalysisBase::createOutputFile(const std::string &name){

	std::string finale_name = name;
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	if(name.find(".root") == std::string::npos) finale_name += ".root";
	if(finale_name.find("/") != std::string::npos){
		outputFile_ = std::make_unique<TFile>(finale_name.c_str(),"RECREATE");
	}
	else {
		finale_name = cmsswBase + "/src/Analysis/MssmHbb/output/" + finale_name;
		outputFile_ = std::make_unique<TFile>(finale_name.c_str(),"RECREATE");
	}
	std::cout<<"File: "<<finale_name<<" was created"<<std::endl;
}

void JetAnalysisBase::writeHistograms(){

	for(const auto & h : histo_.getHisto()){
		if(h.second->GetEntries() != 0) h.second->Write();
	}
	outputFile_->Close();
}

void JetAnalysisBase::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){

	if(TEST) std::cout<<"I'm in JetAnalysisBase::fillHistograms"<<std::endl;

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

}

void JetAnalysisBase::runAnalysis(const std::string &json, const std::string &output, const int &size){
	this->setupAnalysis(json);
	this->SetupStandardOutputFile(output);
	this->makeHistograms(size);
	this->applySelection();
	this->writeHistograms();
}

const bool findStrings(const std::string & input, const std::string & needful){
	std::string input1 = input;
	std::string input2 = needful;
	std::transform(input1.begin(),input1.end(),input1.begin(),tolower);
	std::transform(input2.begin(),input2.end(),input2.begin(),tolower);
	if(input1.find(input2) != std::string::npos) return true;
	else return false;
}

const double JetAnalysisBase::assignWeight(){
	double weight = 1;
	if(isMC()) {
		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PU_central"];
	}
	return weight;
}

std::shared_ptr<tools::Collection<tools::Jet> > JetAnalysisBase::modifyJetCollection(tools::Jet & jet,
						std::shared_ptr<tools::Collection<tools::Jet> > & initialJets
						){
	if(TEST) std::cout<<"I'm in JetAnalysisBase::modifyJetCollection"<<std::endl;
	return initialJets;
}
