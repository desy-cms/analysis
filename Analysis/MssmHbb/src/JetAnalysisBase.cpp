/*
 * JetAnalysisBase.cpp
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */
#include <stdlib.h>
#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

double signal_shape(const double & M12_gen){
	double result = 1;
	double par[6];
	par[0] = 7.08179e-01; par[1] = 7.74883e+00; par[2] = 1.10010e+03;
	par[3] = 4.48655e-02; par[4] = 8.82140e+02; par[5] = 2.12431e+02;
	double M  = M12_gen;
	double M2 = M*M;
	double G = par[1];
	double G2 = G*G;
	double E = par[2];
	double E2= E*E;
	double rel_BW = par[0] * 2 / TMath::Pi() * G2*E2 / ( (M2-E2)*(M2-E2)+ M2*M2*(G2/E2) );
	double gaus = par[3] * TMath::Gaus(M,par[4],par[5]);
	if(M >= 1000 && M <= 1200) result = rel_BW / gaus;
	else result = 0;
	return result;
}

JetAnalysisBase::JetAnalysisBase(const std::string & inputFilelist, const double & dataLumi, const bool & lowM, const bool & test) :
								 Analysis(inputFilelist,"MssmHbb/Events/EventInfo"),
								 dataLumi_(dataLumi),
								 lowM_(lowM),
								 triggerLogicName_(""),
								 nJets_(1),
								 TEST(test){
	if(this->isMC()){
		//Define MC type
		if(findStrings(inputFilelist,"susy")) signalMC_ = true;
		else signalMC_ = false;
		if(signalMC_) this->setupXSections();

		//Add specific to MC trees
		this->addTree<GenParticle>("GenParticles","MssmHbb/Events/prunedGenParticles");
		if(isMC()) this->addTree<Jet>("GenJets","MssmHbb/Events/slimmedGenJets");

		// Add MC information:
		this->crossSections("MssmHbb/Metadata/CrossSections");
		this->generatorFilter("MssmHbb/Metadata/GeneratorFilter");
		if(signalMC_) this->eventFilter("MssmHbb/Metadata/EventFilter");//,"MssmHbb/mHatFilter/EventFilter");
		else this->eventFilter("MssmHbb/Metadata/EventFilter");

		// Show MC information
		this->listCrossSections();
		this->listGeneratorFilter();

		if(isMC()){
			std::cout<<"***********************************"<<std::endl;
			if(signalMC_) std::cout<<"This is signal SUSY MC"<<std::endl;
			else std::cout<<" This is BG MC"<<std::endl;
			std::cout<<"***********************************"<<std::endl;
		}
		else {
			std::cout<<"***********************************"<<std::endl;
			std::cout<<"This is Real Data"<<std::endl;
			std::cout<<"***********************************"<<std::endl;
		}
	}
	JESshift_ = 0;
	JERshift_ = 0;
	//Muon tree
	this->addTree<Muon> ("Muons","MssmHbb/Events/slimmedMuons");

	//Trigger trees
	this->triggerResults("MssmHbb/Events/TriggerResults");

	// Tree for Jets
	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
	// Tree for Vertices
	this->addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices");

	triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
	baseOutputName_ = "JetAnalysisBase";

	std::string selection_type;
	if(lowM){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
        pt1_ = 100.; pt2_ = 100.;
        btag1_ = 0.935; btag2_ = 0.935;
        btagOP1_ = 2; btagOP2_ = 2;
        btag3_ = 0.8;
        btagOP3_ = 1;	//Mid OP
        selection_type = "Low Mass";
	}
	else {
        btag3_ = 0.46;
        btagOP3_ = 0; // Loose OP
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
        pt1_ = 160.; pt2_ = 160.;
        btag1_ = 0.8; btag2_ = 0.8;
        btagOP1_ = 1; btagOP2_ = 1;
        selection_type = "High Mass";
	}
//	cuts_ = CutFlow(baseOutputName_,selection_type);
	mHat_ = 0.7;
}

JetAnalysisBase::JetAnalysisBase(const std::string & inputFilelist, const bool & lowM) :
								 Analysis(inputFilelist,"MssmHbb/Events/EventInfo"),
								 dataLumi_(1),
								 lowM_(lowM),
								 triggerLogicName_(""),
								 nJets_(1),
								 TEST(false){
	if(this->isMC()){
		//Define MC type
		if(findStrings(inputFilelist,"susy")) signalMC_ = true;
		else signalMC_ = false;
		if(signalMC_) this->setupXSections();

		//Add specific to MC trees
		this->addTree<GenParticle>("GenParticles","MssmHbb/Events/prunedGenParticles");
		if(isMC()) this->addTree<Jet>("GenJets","MssmHbb/Events/slimmedGenJets");

		// Add MC information:
		this->crossSections("MssmHbb/Metadata/CrossSections");
		this->generatorFilter("MssmHbb/Metadata/GeneratorFilter");
		if(signalMC_) this->eventFilter("MssmHbb/Metadata/EventFilter");//,"MssmHbb/mHatFilter/EventFilter");
		else this->eventFilter("MssmHbb/Metadata/EventFilter");

		// Show MC information
		this->listCrossSections();
		this->listGeneratorFilter();

		if(isMC()){
			std::cout<<"***********************************"<<std::endl;
			if(signalMC_) std::cout<<"This is signal SUSY MC"<<std::endl;
			else std::cout<<" This is BG MC"<<std::endl;
			std::cout<<"***********************************"<<std::endl;
		}
		else {
			std::cout<<"***********************************"<<std::endl;
			std::cout<<"This is Real Data"<<std::endl;
			std::cout<<"***********************************"<<std::endl;
		}
	}
	JESshift_ = 0;
	JERshift_ = 0;
	//Muon tree
	this->addTree<Muon> ("Muons","MssmHbb/Events/slimmedMuons");

	//Trigger trees
	this->triggerResults("MssmHbb/Events/TriggerResults");

	// Tree for Jets
	this->addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
	// Tree for Vertices
	this->addTree<Vertex> ("Vertices","MssmHbb/Events/offlineSlimmedPrimaryVertices");

	triggerLogicName_ = "";
	triggerObjectName_ = {};
	pt1_ = -100; pt2_ = -100;
	btag1_ = -100; btag2_ = -100;
	btagOP1_ = -100; btagOP2_ = -100;
	btag3_ = -100;
	btagOP3_ = -100;	//Mid OP

	baseOutputName_ = "JetAnalysisBase";

	std::string selection_type;
	if(lowM_){
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
        selection_type = "Low Mass";
	}
	else {
		triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
        selection_type = "High Mass";
	}

	mHat_ = 0.7;
}

JetAnalysisBase::~JetAnalysisBase() {
	//if(TEST) std::cout<<"I'm at JetAnalysisBase::~JetAnalysisBase"<<std::endl;
	// TODO Auto-generated destructor stub
}

void JetAnalysisBase::setupAnalysis(const std::string & json){
	if(TEST) std::cout<<"I'm in setupAnalysis"<<std::endl;
	if(!isMC()) this->processJsonFile(json);
	if(isMC())  this->loadCorrections();
	this->addTriggerObjects(triggerObjectName_);
}

void JetAnalysisBase::applySelection(){

	if(TEST) std::cout<<"I'm in applySelection"<<std::endl;

	//different counters declaration
	int TotalNumberOfGenEvents = 0;						// Total number of generated events
	int NumberOfGenEvents_afterMHat = 0;				// Total number of generated events after Mhat cut
	double NumberOfGenEvents_afterMHat_rewPU = 0;		// Total number of generated events after Mhat cut and PU reweighting
	int NumberOfEventsAfterSelection = 0;				// Total number of the events that pass selection
	double NumberOfEventsAfterSelection_weighted = 0;	// Weighted number of the events that pass selection

	bool goodLeadingJets = false;
	double totWeight = 0;
	TLorentzVector diJetObject;
	double Ht = 0;
	//Event loop:



	int n_true_b = 0;

	std::string selection_type;
	if(lowM_)selection_type = "Low Mass";
	else selection_type = "High Mass";
	cuts_ = CutFlow(baseOutputName_,selection_type);


	if(TEST) TotalNumberOfGenEvents = 30000;
	else TotalNumberOfGenEvents = this->size();
	std::cout<<"Events to process: "<<TotalNumberOfGenEvents<<std::endl;

	for(auto i = 0; i < TotalNumberOfGenEvents; ++ i){
		this->event(i);

		cuts_.check("Total");

		if(isSignalMC() && !cuts_.check("mHat",(mHat() >= 0.7 * returnMassPoint()))) continue;
		++NumberOfGenEvents_afterMHat;

		//PU reweighting for MC
		if(isMC()) {
			weight_["PU_central"] 	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_central"],hCorrections1D_["hPileUpMC"],this->nTruePileup());
			NumberOfGenEvents_afterMHat_rewPU += weight_["PU_central"];
		}

		//Select only good JSon runs for Data
		if(!isMC()) if(!cuts_.check("json",this->selectJson() )) continue;

	    //Define Jet Collection
		auto offlineJets = this->collection<Jet>("Jets");
		auto shiftedJets = std::make_shared<Collection<Jet> >();

		//Define MC specific collections:
		if(isMC()){
			genJets_ = (std::shared_ptr<Collection<Jet> >) this->collection<Jet>("GenJets");
			//Apply JER smearing if this is MC:
		    offlineJets->matchTo(*genJets_,3,0.2);
		    offlineJets->smearTo(*genJets_,JERshift_);
		}

//		if(offlineJets->size() < nJets_ ) continue;
		if(!cuts_.check("nJets",offlineJets->size() >= nJets_)) continue;
		//Match offline Jets to online Objects

		this->match<Jet,TriggerObject>("Jets",this->getTriggerObjectNames());

	    //Define Vertex collection
	    auto offlinePrimaryVertices = this->collection<Vertex>("Vertices");

	    goodLeadingJets = false;
		Jet LeadJet[5];
		std::vector<std::pair<int,Jet> > TrueBJets;
		ScaleFactor sf[5];
	    Ht = 0.;
	    //Jet Selection
	    for( int iJet = 0; iJet < offlineJets -> size(); ++iJet){
	    	Jet jet = offlineJets->at(iJet);
	    	shiftedJets = this->modifyJetCollection(jet,shiftedJets);
	    	jet = shiftedJets->at(iJet);

	    	if(isMC() && jet.flavour() == 5) {
	    		TrueBJets.push_back( std::pair<int,Jet> (iJet,jet) );
	    		++n_true_b;
	    	}

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
	    				sf[iJet] = calculateBTagSF(jet,btagOP1_);
	    			}
	    			if(iJet == 2){
	    				sf[iJet] = calculateBTagSF(jet,btagOP3_);
	    			}
	    		}

	    		if(iJet == nJets_ - 1) goodLeadingJets = true;
	    	}
	    }

	    // STUDY OF THE FLAVOUR COMPOSITION
	    int j = 0;
	    if(isMC()){
	    	for(const auto & jpair : TrueBJets){
	    		std::string name = "bJet" + std::to_string(j) + "_pt";
	    		(histo_.getHisto())[name]->Fill(jpair.second.pt());
	    		name = "bJet" + std::to_string(j) + "_eta";
	    		(histo_.getHisto())[name]->Fill(jpair.second.eta());
	    		name = "bJet" + std::to_string(j) + "_btag";
	    		(histo_.getHisto())[name]->Fill(jpair.second.btag());
	    		name = "bJet" + std::to_string(j) + "_order";
	    		(histo_.getHisto())[name]->Fill(jpair.first);
	    		++j;
	    	}
	    }


	    (histo_.getHisto())["nTrue_b"]->Fill(n_true_b);
	    n_true_b = 0;
	    if(!cuts_.check("LoosID",goodLeadingJets)) continue;
//	    if(!goodLeadingJets) continue;
	    //Should return true for trigger efficiency study
//	    if(!this->leadingJetSelection(shiftedJets)) continue;
	    if(!cuts_.check("LeadingJetSelection",leadingJetSelection(shiftedJets))) continue;
	    this->Ht(Ht);

	    //Weights:
	    if(isMC()){
	    	//.........................Calculate weights...............
        	// general weights which is independent of selection type OR already includes
        	// selection depends criterias inside.
//          weight_["FactorPt"] 		= pWeight_->FactorizationPtWeight(LeadJet[0].pt(), LeadJet[1].pt());

	    	double f1 		= pWeight_->triggerCorrectionFunction(LeadJet[0].pt(), LeadJet[0].eta());
	    	double eff1		= pWeight_->PtTriggerEfficiency(LeadJet[0].pt(), LeadJet[0].eta());
	    	double f2		= pWeight_->triggerCorrectionFunction(LeadJet[1].pt(), LeadJet[1].eta());
	    	double eff2		= pWeight_->PtTriggerEfficiency(LeadJet[1].pt(), LeadJet[1].eta());

	    	double df1	 	= f1 * 0.1 * (1. - eff1);
	    	double df2	 	= f2 * 0.1 * (1. - eff2);

	    	double F 		= f1*f2;
 	    	double dF		= F * std::sqrt( pow(df1/f1,2) + pow(df2/f2,2) );

	    	//For systematics
	    	weight_["PtTriggerEfficiency"]= pWeight_->PtTriggerEfficiency(LeadJet[0].pt(), LeadJet[0].eta()) * pWeight_->PtTriggerEfficiency(LeadJet[1].pt(), LeadJet[1].eta());
	    	weight_["dEta"]     		= pWeight_->dEtaWeight(std::abs(LeadJet[0].eta() - LeadJet[1].eta()));
	    	//TODO: Data luminosity!!!
//	    	if(!signalMC_) {
//	    		weight_["Lumi"] 			=pWeight_->LumiWeight(dataLumi_,this->luminosity());
//	    	}
//	    	else weight_["Lumi"] 			=pWeight_->LumiWeight(dataLumi_,this->numberGenEvents()/xsection_[returnMassPoint()]);

//	    	else weight_["Lumi"] 			=pWeight_->LumiWeight(dataLumi_,weight_["TOT_GEN_EVENTS"]/xsection_[returnMassPoint()]);
//	    	else weight_["Lumi"] 			=pWeight_->LumiWeight(dataLumi_,this->numberFilteredGenEvents()/xsection_[returnMassPoint()]);

	    	std::string file_name = outputFile_->GetName();
	    	if(file_name.find("madgraphMLM") != std::string::npos && ( file_name.find("bEnriched") != std::string::npos || file_name.find("TuneCUETP8M1") != std::string::npos )){
	    		weight_["Ht"]  = pWeight_->HtWeight(hCorrections1D_["hHtWeight"],Ht);
	    	}
	    	else weight_["Ht"] = 1;

	    	//2SIGMA variation!!!!!!
	    	weight_["PU_down"]    	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_down"],hCorrections1D_["hPileUpMC"],this->nTruePileup());
	    	weight_["PU_up"]      	= pWeight_->PileUpWeight(hCorrections1D_["hPileUpData_up"],hCorrections1D_["hPileUpMC"],this->nTruePileup());

//	    	std::cout<<"wt: "<<weight_["PU_central"]<<" "<<weight_["PU_down"]<<" "<<weight_["PU_up"]<<std::endl;
//	    	std::cout<<"wt: "<<weight_["PU_central"]<<" "<<weight_["PU_down"]/weight_["PU_central"]<<" "<<weight_["PU_up"]/weight_["PU_central"]<<"\n"<<std::endl;

	    	weight_["PtEff_central"]	= F;
	    	weight_["PtEff_up"]       	= F + 2.*dF;
	    	weight_["PtEff_down"]		= F - 2.*dF;

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

	    ++NumberOfEventsAfterSelection;

	    totWeight = this->assignWeight();
	    this->fillHistograms(shiftedJets,totWeight);
		NumberOfEventsAfterSelection_weighted += totWeight;

	}

	double xsection = 0;
	if(isMC()){
		if(!signalMC_) {
			xsection = this->crossSection();
		}
		else {
			xsection = xsection_[returnMassPoint()];
		}
		weight_["Lumi"] 			=pWeight_->LumiWeight(dataLumi_,NumberOfGenEvents_afterMHat_rewPU / xsection);
	}


	(histo_.getHisto())["xsection"]->Fill(xsection);
	(histo_.getHisto())["lumi_weight"]->Fill(weight_["Lumi"]);
	(histo_.getHisto())["TotalNumberOfGenEvents"]->Fill(TotalNumberOfGenEvents) ;
	(histo_.getHisto())["NumberOfGenEvents_afterMHat"]->Fill(NumberOfGenEvents_afterMHat);
	(histo_.getHisto())["NumberOfGenEvents_afterMHat_rewPU"]->Fill(NumberOfGenEvents_afterMHat_rewPU);

	cuts_.Print();

	if(isMC()){
		std::cout<<"***********************************"<<std::endl;
		std::cout<<"# generated events: "<<TotalNumberOfGenEvents<<std::endl;
		if(isSignalMC()) std::cout<<"# generated events after MHat cut: "<<NumberOfGenEvents_afterMHat<<std::endl;
		std::cout<<"# generated events after MHat cut and PU rew : "<<NumberOfGenEvents_afterMHat_rewPU<<std::endl;
		std::cout<<"# selected events: "<<NumberOfEventsAfterSelection<<std::endl;
		std::cout<<"# selected weighted evetns: "<<NumberOfEventsAfterSelection_weighted<<std::endl;
		std::cout<<"Cross section: "<<xsection<<" Effective Lumi: "<<NumberOfGenEvents_afterMHat_rewPU / xsection<<std::endl;
		std::cout<<"***********************************"<<std::endl;
	}
	else{
		std::cout<<"***********************************"<<std::endl;
		std::cout<<"Total number of Events: "<<TotalNumberOfGenEvents<<std::endl;
		std::cout<<"Number of selected Events: "<<NumberOfEventsAfterSelection<<std::endl;
		std::cout<<"***********************************"<<std::endl;
	}

//	this->writeHistograms();
//	outputFile_->Close();
}
void JetAnalysisBase::addTriggerObjects(const std::vector<std::string> &triggerObjectName, const std::string & path)
{

	if(triggerObjectName.size() == 0)
	{
		std::cerr<<"Error: Empty vector of triggerObjectNames were sepcified. Interupt!"<<std::endl;
		exit(1);
	}

	triggerObjectName_ = triggerObjectName;
	//Add trees with different Trigger Objects and specify Trigger Logic name
	for(const auto & triggerObject : triggerObjectName)
	{
		this->addTree<TriggerObject>(triggerObject,(path + triggerObject).c_str());
	}
}

void JetAnalysisBase::loadCorrections(){
	if(TEST) std::cout<<"I'm in JetAnalysisBase::loadCorrections"<<std::endl;
	TH1::AddDirectory(0);
	if(lowM_){
		//Online BTag Trigger Efficiency produced by Ye Chen
		fCorrections_["fRelBTagEff"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/RelOnlineBTagCSV0p9Eff_PtEta.root").c_str(),"read");
		hCorrections1D_["hRelBTagEff0p9"] 		=	  (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh4") ;			// eta <0.9
		hCorrections1D_["hRelBTagEff0p9_1p4"] 	=	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh3") ;		// 1.4 > eta >0.9
		hCorrections1D_["hRelBTagEff1p4_2p5"] 	= 	 (TH1D*) fCorrections_["fRelBTagEff"] -> Get("heh2") ;		// 2.5 > eta > 1.4
		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/TwoDPtLowMassEfficiency.root").c_str(),"read");
		hCorrections2D_["hPtTriggerEff"] = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num") ; // 2D

		// Add Ht reweighting:
		std::string file_name = outputFile_->GetName();
//		if(file_name.find("BGenFilter") != std::string::npos ){
//			fCorrections_["fHtWeight"] = std::make_unique<TFile>("input_corrections/DataMC_HT_Correction_lowM_BGenFilter.root","read");
//			hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("h_clone") ;
//		}
		std::string file_ht_name;
		if(file_name.find("bEnriched") != std::string::npos || file_name.find("BGenFilter") != std::string::npos){
//			fCorrections_["fHtWeight"] = std::make_unique<TFile>("input_corrections/bbx_HT_Correction_lowM_QCD_b_Ht.root","read");
			if(nJets_ == 2) file_ht_name = "bin/input_corrections/QCD_HT_2b_Correction_lowM_QCD.root";
			else file_ht_name = "bin/input_corrections/bbx_HT_Correction_lowM_QCD.root";
			fCorrections_["fHtWeight"] = std::make_unique<TFile>((defaultInputDir_ + file_ht_name).c_str(),"read");
			hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("h_clone") ;
			std::cout<<"HT corrections for bEnriched + BGenFilter samples were loaded"<<std::endl;
		}
		else if (file_name.find("TuneCUETP8M1") != std::string::npos) {
			fCorrections_["fHtWeight"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/DataMC_HT_Correction_lowM_TuneCUETP8M1.root").c_str(),"read");
			hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("h_clone") ;
		}

	}
	else {
		// For high mass trigger only 2D efficiency were provided
		fCorrections_["fRelBTagEff"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/TwoDBTagCSV0p85_2D_PtEta.root").c_str(),"read");
		hCorrections2D_["hRelBTagEff2D"] = (TH2D*) fCorrections_["fRelBTagEff"] ->Get("h2ehn") ;
		//Online Pt trigger efficiency:
		fCorrections_["fPtTriggerEff"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/TwoDPtHighMassEfficiency.root").c_str(),"read");
		hCorrections2D_["hPtTriggerEff"]  = (TH2D*) fCorrections_["fPtTriggerEff"] ->Get("TwoDEff_Num"); // 2D
		// Add Ht reweighting:
		std::string file_name = outputFile_->GetName();
		if(file_name.find("BGenFilter") != std::string::npos ){
			fCorrections_["fHtWeight"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/DataMC_HT_Correction_highM_BGenFilter.root").c_str(),"read");
			hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("h_clone") ;
		}
		else if (file_name.find("TuneCUETP8M1") != std::string::npos) {
			fCorrections_["fHtWeight"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/DataMC_HT_Correction_highM_TuneCUETP8M1.root").c_str(),"read");
			hCorrections1D_["hHtWeight"] =  (TH1D*) fCorrections_["fHtWeight"] -> Get("h_clone") ;
		}
	}
	fCorrections_["fPileUpData_central"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/PileUp_2015Dec_central.root").c_str(),"read");
	fCorrections_["fPileUpData_up"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/PileUp_2015Dec_up.root").c_str(),"read");
	fCorrections_["fPileUpData_down"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/PileUp_2015Dec_down.root").c_str(),"read");
	hCorrections1D_["hPileUpData_central"] = (TH1D*) fCorrections_["fPileUpData_central"]->Get("pileup");
	hCorrections1D_["hPileUpData_up"] = (TH1D*) fCorrections_["fPileUpData_up"]->Get("pileup");
	hCorrections1D_["hPileUpData_down"] = (TH1D*) fCorrections_["fPileUpData_down"]->Get("pileup");
	fCorrections_["fPileUpMC"] = std::make_unique<TFile>((defaultInputDir_ + "bin/input_corrections/MC_Fall15_PU25_V1.root").c_str(),"read");
	hCorrections1D_["hPileUpMC"] = (TH1D*) fCorrections_["fPileUpMC"] -> Get("pileup") ;

	BTagCalibrationLib_ = std::make_unique<BTagCalibration>("csvv2", (defaultInputDir_ + "bin/input_corrections/SFbLib.csv").c_str());
	SFb_["OP0_FLAV_B_central"] = std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),             // calibration instance
																	BTagEntry::OperatingPoint::OP_LOOSE,  		// operating point
																	"mujets",               					// measurement type
																	"central");           						// systematics type);

	SFb_["OP0_FLAV_B_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"mujets","up");
	SFb_["OP0_FLAV_B_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"mujets","down");

	SFb_["OP0_FLAV_C_central"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"mujets","central");
	SFb_["OP0_FLAV_C_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"mujets","up");
	SFb_["OP0_FLAV_C_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"mujets","down");

	SFb_["OP0_FLAV_UDSG_central"] 	= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"incl","central");
	SFb_["OP0_FLAV_UDSG_up"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"incl","up");
	SFb_["OP0_FLAV_UDSG_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_LOOSE,"incl","down");

	SFb_["OP1_FLAV_B_central"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","central");
	SFb_["OP1_FLAV_B_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","up");
	SFb_["OP1_FLAV_B_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","down");

	SFb_["OP1_FLAV_C_central"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","central");
	SFb_["OP1_FLAV_C_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","up");
	SFb_["OP1_FLAV_C_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","down");

	SFb_["OP1_FLAV_UDSG_central"] 	= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"incl","central");
	SFb_["OP1_FLAV_UDSG_up"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"incl","up");
	SFb_["OP1_FLAV_UDSG_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_MEDIUM,"incl","down");

	SFb_["OP2_FLAV_B_central"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","central");
	SFb_["OP2_FLAV_B_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","up");
	SFb_["OP2_FLAV_B_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","down");

	SFb_["OP2_FLAV_C_central"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","central");
	SFb_["OP2_FLAV_C_up"] 			= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","up");
	SFb_["OP2_FLAV_C_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"mujets","down");

	SFb_["OP2_FLAV_UDSG_central"] 	= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"incl","central");
	SFb_["OP2_FLAV_UDSG_up"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"incl","up");
	SFb_["OP2_FLAV_UDSG_down"] 		= std::make_unique<BTagCalibrationReader>(BTagCalibrationLib_.get(),BTagEntry::OperatingPoint::OP_TIGHT,"incl","down");

	//Declare weights:
	pWeight_ = std::unique_ptr<Weights>(new Weights(lowM_));

}

const bool JetAnalysisBase::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){
//	if(TEST) std::cout<<"I'm in JetAnalysisBase::leadingJetSelection::shared_ptr"<<std::endl;
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

const JetAnalysisBase::ScaleFactor JetAnalysisBase::calculateBTagSF(const tools::Jet & jet,const int & op){

	std::string measurementType;
	BTagEntry::JetFlavor flav_b;
	double pt_limit = jet.pt();
	bool doubleUncertainty = false;
	if(jet.flavour() == 0)		{ flav_b = BTagEntry::FLAV_UDSG	; measurementType = "FLAV_UDSG"; }
	else if(jet.flavour() == 5)	{ flav_b = BTagEntry::FLAV_B	; measurementType = "FLAV_B"; }
	else if(jet.flavour() == 4)	{ flav_b = BTagEntry::FLAV_C	; measurementType = "FLAV_C"; }
	else {std::cerr<<"non b/c/udsg flavour in JetAnalysisBase::calculateBTagSF. Exception."<<std::endl; exit(1); }

	std::string operation_point;
	if(op == 0){ operation_point = "OP0";}
	else if( op == 1) {operation_point = "OP1";}
	else if( op == 2) {operation_point = "OP2";}
	else {std::cerr<<"wrong operation point was provided in JetAnalysisBase::calculateBTagSF. Exception."<<std::endl; exit(1);}

	if(jet.flavour() == 0 && pt_limit >= 1000)		{ pt_limit = 999; doubleUncertainty = true; }
	else if(jet.flavour() != 0 && pt_limit >= 670. ){ pt_limit = 669; doubleUncertainty = true; }

	ScaleFactor sf;

 	sf.central 	= SFb_[operation_point + "_" + measurementType + "_" + "central"]->eval(flav_b,jet.eta(),pt_limit);
	sf.up 		= SFb_[operation_point + "_" + measurementType + "_" + "up"]		->eval(flav_b,jet.eta(),pt_limit);
	sf.down		= SFb_[operation_point + "_" + measurementType + "_" + "down"]	->eval(flav_b,jet.eta(),pt_limit);
	sf.flavour  = jet.flavour();

	if(doubleUncertainty){
		sf.up 	= 2*(sf.up - sf.central) + sf.central;
		sf.down = 2*(sf.down - sf.central) + sf.central;
	}

	return sf;
}

void JetAnalysisBase::makeHistograms(const int & size){
	histo_.Make(size,lowM_);
}

void JetAnalysisBase::SetupStandardOutputFile(const std::string & outputFileName){

		//get the full file name and path from the Tree
		std::string fullName = this->tree<Jet>("Jets")->PhysicsObjectTree<Jet>::PhysicsObjectTreeBase::TreeBase::tree()->GetFile()->GetName();
		std::string outputName = baseOutputName_;
		if(outputFileName != "") outputName+="_"+outputFileName;
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

		if(is_mc_ && findStrings(fullName,"asymptotic")){
			fullName = fullName.substr(0,found);
			found = fullName.find_last_of("/");
			fullName = fullName.substr(found+1);
		}
		else{
			fullName = fullName.substr(found+1);
		}

		outputName += fullName;// + ".root";
		this->createOutputFile(outputName);
//	else{
//		this->createOutputFile(outputFileName);
//	}

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
//		finale_name = "/nfs/dust/cms/user/" + std::string(getenv("USERNAME")) + "/output/" + finale_name;
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

//	if(TEST) std::cout<<"I'm in JetAnalysisBase::fillHistograms"<<std::endl;

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
//	if(TEST) std::cout<<"I'm in JetAnalysisBase::modifyJetCollection"<<std::endl;

	initialJets->add(jet);
	return initialJets;
}

int JetAnalysisBase::returnMassPoint() const {
	int Mpoint = 0;
	if(!signalMC_){
		return 0;
	}
	std::string MassPos = "_M-";
	auto p1 = inputFilelist_.find(MassPos) + 3;
	if(p1 == std::string::npos) {
		std::cerr<<"FileNames were cahnged!!!!"<<std::endl;
		exit(1);
	}
	auto p2 = inputFilelist_.find("_",p1);
	std::string MpointString = inputFilelist_.substr(p1,size_t(p2-p1));
	Mpoint = std::stoi(MpointString);
	return Mpoint;
}

void JetAnalysisBase::setupXSections(){
    xsection_[100] = 1.;//5.23;	//5.22917222713
    xsection_[120] = 1.;//41.79;	//41.7887314999
    xsection_[160] = 1.;//75.32;	//75.3185132566
    xsection_[200] = 1.;//35.42;	//35.4180244078
    xsection_[250] = 1.;//15.55;	//15.5527782822
    xsection_[300] = 1.;//7.64;	//7.63976194847
    xsection_[350] = 1.;//4.1; 	//4.09571617045
    xsection_[400] = 1.;//2.34;	//2.34044663823
    xsection_[500] = 1.;//0.88;	//0.877385402091
    xsection_[600] = 1.;//0.377;	//0.376766971183
    xsection_[700] = 1.;//0.18;	//0.17940535717
    xsection_[900] = 1.;//0.051;	//0.0507664661841
    xsection_[1100] = 1.;//0.018;//0.0180548176636
    xsection_[1300] = 1.;//0.008;//0.00802737010696
}

const double JetAnalysisBase::mHat(){
	double p_prot = 13000. /2.;
	double p1 = this->pdf().x.first * p_prot;
	double p2 = this->pdf().x.second * p_prot;
	double mHat_ = std::sqrt((p1+p2)*(p1+p2) - (p1-p2)*(p1-p2));
	return mHat_;
}

