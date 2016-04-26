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
	TLorentzVector diJetObject;
	Jet LeadJet[5];
	//Event loop:
	auto nevents = 0;
	if(TEST) nevents = 5000;
	else nevents = this->size();

	for(auto i = 0; i < nevents; ++ i){
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
	    //Should return true for trigger efficiency study
	    if(!isMC() && !OnlineSelection(offlineJets->at(0),offlineJets->at(1))) continue;

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
	    		if(!jet.idLoose()) break;
	    		if(!this->leadingJetSelection(offlineJets)) break;
	    		LeadJet[iJet] = jet;

	    		//Calculate SFb
	    		if(isMC()){
	    			if(iJet < 2){
	    				sf[iJet] = calculateBTagSF(jet,false);
	    			}
	    			if(iJet == 2){
	    				sf[iJet] = calculateBTagSF(jet,true);
	    			}
	    		}

	    		if(iJet == nJets_ - 1) goodLeadingJets = true;
	    	}
	    }
	    if(!goodLeadingJets) continue;

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

        	  weight_["JES_central"]	= LeadJet[0].pt()*LeadJet[1].pt();
        	  weight_["JES_up"]			= LeadJet[0].pt()*(1.+ 2.*LeadJet[0].jecUncert()) *
        			  	  	  	  	  	  LeadJet[1].pt()*(1.+ 2.*LeadJet[1].jecUncert());
        	  weight_["JES_down"]		= LeadJet[0].pt()*(1.- 2.*LeadJet[0].jecUncert()) *
  	  	  	  	  	  	  	  	  	  	  LeadJet[1].pt()*(1.- 2.*LeadJet[1].jecUncert());

        	  weight_["SFb_central"]	= sf[0].central * sf[1].central;
        	  weight_["SFb_up"]			= (2.*(sf[0].up - sf[0].central) + sf[0].central) * (2.*(sf[1].up - sf[1].central) + sf[1].central);
        	  weight_["SFb_down"]		= (2.*(sf[0].down - sf[0].central) + sf[0].central) * (2.*(sf[1].down - sf[1].central) + sf[1].central);
        	  if(nJets_ == 3) {
        		  weight_["SFb_central"] 	*= sf[2].central;
        		  weight_["SFb_up"]			*= (2.*(sf[2].up - sf[2].central) + sf[2].central);
        		  weight_["SFb_down"]		*= (2.*(sf[2].down - sf[2].central) + sf[2].central);

        		  weight_["JES_up"]			*= LeadJet[2].pt()*(1.+ 2.*LeadJet[2].jecUncert());
        		  weight_["JES_down"]		*= LeadJet[2].pt()*(1.- 2.*LeadJet[2].jecUncert());
        		  weight_["JES_central"]	*= LeadJet[2].pt();
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

	    this->fillHistograms(offlineJets);

	}

	this->writeHistograms();
	outputFile_->Close();

}


const bool JetAnalysisBase::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){
	if(TEST) std::cout<<"I'm in JetAnalysisBase::leadingJetSelection::shared_ptr"<<std::endl;
	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	if(jet1.pt() < pt1_ || jet2.pt() < pt1_) return false;
	if(jet1.pt() < eta1_ || jet2.pt() < eta1_) return false;

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
		fCorrections_["fRelBTagEff"] = pTFile(new TFile("input_corrections/RelOnlineBTagCSV0p9Eff_PtEta.root","read") );
		hCorrections1D_["hRelBTagEff0p9"] 		=	  (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh4") ;			// eta <0.9
		hCorrections1D_["hRelBTagEff0p9_1p4"] 	=	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh3") ;		// 1.4 > eta >0.9
		hCorrections1D_["hRelBTagEff1p4_2p5"] 	= 	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh2") ;		// 2.5 > eta > 1.4

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtLowMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"] = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ; // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") ;

	}
	else {

		// For high mass trigger only 2D efficiency were provided
		fCorrections_["fRelBTagEff"] = pTFile (new TFile("input_corrections/TwoDBTagCSV0p85_2D_PtEta.root","read") );
		hCorrections2D_["hRelBTagEff2D"] = (TH2D*) fCorrections_["fRelBTagEff"] ->Get("h2ehn") ;

		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = pTFile (new TFile("input_corrections/TwoDPtHighMassEfficiency.root","read") );
		hCorrections2D_["hPtTriggerEff"]  = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num"); // 2D

		// Add Ht reweighting:
		fCorrections_["fHtWeight"] = pTFile (new TFile("input_corrections/HtRatio.root","read") );
		hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("hRatio") ;

	}
	fCorrections_["fPileUpData_central"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_central.root","read") );
	fCorrections_["fPileUpData_up"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_up.root","read") );
	fCorrections_["fPileUpData_down"] = pTFile(new TFile("input_corrections/PileUp_2015Dec_down.root","read") );
	hCorrections1D_["hPileUpData_central"] = (TH1D*) fCorrections_["fPileUpData_central"]->Get("pileup");
	hCorrections1D_["hPileUpData_up"] = (TH1D*) fCorrections_["fPileUpData_up"]->Get("pileup");
	hCorrections1D_["hPileUpData_down"] = (TH1D*) fCorrections_["fPileUpData_down"]->Get("pileup");
	fCorrections_["fPileUpMC"] = pTFile(new TFile("input_corrections/MC_Fall15_PU25_V1.root","read") );
	hCorrections1D_["hPileUpMC"] = (TH1D*) fCorrections_["fPileUpMC"] -> Get("pileup") ;



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

		outputName += fullName + ".root";
		this->createOutputFile(outputName);
	}
	else{
		this->createOutputFile(outputFileName);
	}

}

void JetAnalysisBase::createOutputFile(const std::string &name){

	std::string finale_name;
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	if(name.find(".root") == std::string::npos) finale_name = name + ".root";
	if(finale_name.find("/") != std::string::npos){
		outputFile_ = pTFile(new TFile(finale_name.c_str(),"RECREATE"));
	}
	else {
		finale_name = cmsswBase + "/src/Analysis/MssmHbb/output/" + name.c_str();
		outputFile_ = pTFile(new TFile(finale_name.c_str(),"RECREATE"));
	}
	std::cout<<"File: "<<name<<" was created"<<std::endl;
}

void JetAnalysisBase::writeHistograms(){

	for(const auto & h : histo_.getHisto()){
		if(h.second->GetEntries() != 0) h.second->Write();
	}

}

void JetAnalysisBase::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets){

	if(TEST) std::cout<<"I'm in JetAnalysisBase::fillHistograms"<<std::endl;
	auto weight = 1;
	if(isMC()) {
		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PU_central"];
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

}

void JetAnalysisBase::runAnalysis(const std::string &json, const std::string &output, const int &size){
	this->setupAnalysis(json);
	this->SetupStandardOutputFile(output);
	this->makeHistograms(size);
	this->applySelection();
}

const bool findStrings(const std::string & input, const std::string & needful){
	std::string input1 = input;
	std::string input2 = needful;
	std::transform(input1.begin(),input1.end(),input1.begin(),tolower);
	std::transform(input2.begin(),input2.end(),input2.begin(),tolower);
//	boost::algorithm::string::to_upper(input1);
//	boost::algorithm::string::to_upper(input2);
	if(input1.find(input2) != std::string::npos) return true;
	else return false;
}

/*
void JetAnalysisBase::fillHistograms(const tools::Jet &jet1, const tools::Jet &jet2){
	auto weight = 0;
	if(isMC()) weight = weight_["dEta"] * weight_["Lumi"];
	else weight = 1;
	(histo_.getHisto())["jet_pt1"]->Fill(jet1.pt(),weight);
	(histo_.getHisto())["jet_eta1"]->Fill(jet1.eta(),weight);
	(histo_.getHisto())["jet_pt2"]->Fill(jet2.pt(),weight);
	(histo_.getHisto())["jet_eta2"]->Fill(jet2.eta(),weight);
	(histo_.getHisto())["jet_deta12"]->Fill(jet1.eta() - jet2.eta(),weight);
	(histo_.getHisto())["jet_dR12"]->Fill(jet1.deltaR(jet2),weight);

	TLorentzVector obj12;
	obj12 = jet1.p4() + jet2.p4();
	(histo_.getHisto())["diJet_pt"]->Fill(obj12.Pt(),weight);
	(histo_.getHisto())["diJet_eta"]->Fill(obj12.Eta(),weight);

}

const bool JetAnalysisBase::leadingJetSelection(const int & iJet, const tools::Jet & Jet){
	bool flag = false;
	if(TEST) std::cout<<"I'm in JetAnalysisBase::leadingJetSelection"<<std::endl;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	if(!Jet.idLoose()) return false;
	if(Jet.pt() < pt1_) return false;
	if(Jet.eta() > eta1_) return false;

	flag = true;
	return flag;
}
*/
