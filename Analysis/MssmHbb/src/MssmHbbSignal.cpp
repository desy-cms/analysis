/*
 * MssmHbbSignal.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

std::string constructTemplateName(const std::string & name);

MssmHbbSignal::MssmHbbSignal(const std::string & inputFilelist,const double & dataLumi, const bool & lowM, const bool & test) :
								selectionDoubleB(inputFilelist,dataLumi,lowM,test)
{
	nJets_ = 3;
	if(lowM){
        btag3_ = 0.8;
        btagOP3_ = 1;
	}
	else {
        btag3_ = 0.46;
	}
	baseOutputName_ = "MssmHbbSignal";
}

MssmHbbSignal::~MssmHbbSignal() {
}

//Member methods

const bool MssmHbbSignal::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	//To avoid code duplication frind class were used
	if(!selectionDoubleB::leadingJetSelection(offlineJets)) return false;

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::leadingJetSelection"<<std::endl;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);

	//Trigger Selection
	if(!this->triggerResult(triggerLogicName_)) return false;

	//Online selection:
	if(!this->OnlineSelection(jet1,jet2)) return false;

	//Pt requirements
	if (jet3.pt() < pt3_) return false;

	//Eta requirements
	if (std::abs(jet3.eta()) > eta3_) return false;

	//BTag requirements
	if (jet3.btag() < btag3_) return false;

	//deltaR requirements
	if (jet2.deltaR(jet3) <= dR_) return false;
	if (jet1.deltaR(jet3) <= dR_) return false;

	return true;
}

void MssmHbbSignal::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::fillHistograms"<<std::endl;

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);
	TLorentzVector obj12;
	obj12 = jet1.p4() + jet2.p4();

	if(JESshift_ == 0 && JERshift_ == 0){
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSf()<<" pt: "<<jet1.pt()<<std::endl;
		selectionDoubleB::fillHistograms(offlineJets,weight);

		(histo_.getHisto())["data_Mbb_230"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["data_Mbb_220"]->Fill(obj12.M(),weight);

		(histo_.getHisto())["jet_pt3"]->Fill(jet3.pt(),weight);
		(histo_.getHisto())["jet_eta3"]->Fill(jet3.eta(),weight);
		(histo_.getHisto())["jet_phi3"]->Fill(jet3.phi(),weight);
		(histo_.getHisto())["jet_btag_csv3"]->Fill(jet3.btag(),weight);
		(histo_.getHisto())["jet_btag_cmva3"]->Fill(jet3.btag("btag_csvmva"),weight);
		(histo_.getHisto())["jet_dR13"]->Fill(jet1.deltaR(jet3),weight);
		(histo_.getHisto())["jet_dR23"]->Fill(jet2.deltaR(jet3),weight);

		if(signalMC_){
			(histo_.getHisto())["template_Mbb"]->Fill(obj12.M(),weight);
			(histo_.getHisto())["template_SFb_up"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_up"]);
			(histo_.getHisto())["template_SFb_down"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_down"]);

			(histo_.getHisto())["template_SFl_up"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_up"]);
			(histo_.getHisto())["template_SFl_down"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_down"]);

			(histo_.getHisto())["template_PU_up"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_up"]);
			(histo_.getHisto())["template_PU_down"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_down"]);

			(histo_.getHisto())["template_PtEff_up"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_up"]);
			(histo_.getHisto())["template_PtEff_down"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_down"]);
		}
	}
	else if (JESshift_ < 0 && JERshift_ == 0){
		(histo_.getHisto())["template_JES_down"]->Fill(obj12.M(),weight);
	}
	else if (JESshift_ > 0 && JERshift_ == 0){
		(histo_.getHisto())["template_JES_up"]->Fill(obj12.M(),weight);
	}
	else if (JERshift_ > 0 && JESshift_ == 0){
		(histo_.getHisto())["template_JER_up"]->Fill(obj12.M(),weight);
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSfUp()<<" pt: "<<jet1.pt()<<std::endl;
	}
	else if (JERshift_ < 0 && JESshift_ == 0){
		(histo_.getHisto())["template_JER_down"]->Fill(obj12.M(),weight);
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSfDown()<<" pt: "<<jet1.pt()<<std::endl;
	}
//	if(TEST) std::cout<<"I'm out of MssmHbbSignal::fillHistograms"<<std::endl;
}

void MssmHbbSignal::writeHistograms(){
	std::string full_name;

	outputFile_->mkdir("distributions","general distributions of Jets and di-Jet objects");
	for(const auto & h: (histo_.getHisto())){
		if(h.second->GetEntries() == 0) continue; 			//skip empty histograms
		if(h.first.find("template") != std::string::npos){
			full_name = constructTemplateName(h.first);
			h.second->Write(full_name.c_str());
		}
		else {
			outputFile_->cd("distributions");
			h.second->Write();
			outputFile_->cd("");
		}
	}
	outputFile_->Close();
}

const double MssmHbbSignal::assignWeight(){
double weight = 1;
	if(isMC()) {
		weight = weight_["Lumi"] * weight_["PtEff_central"] * weight_["PU_central"] * weight_["SFb_central"] * weight_["SFl_central"];
	}
	//std::cout<<" weight = "<<weight<<" "<<weight_["dEta"]<<" "<<weight_["Lumi"]<<" "<<weight_["2DPt"]<<" "<<weight_["BTag"]<<" "<<weight_["PU_central"]<<" "<<weight_["SFb_central"]<<" "<<weight_["SFl_central"]<<std::endl;
	return weight;
}

std::string constructTemplateName(const std::string & name){
	std::string process = "bbH";
	std::string channel	= "Mbb";
	std::string energy	= "13TeV";
	std::string collaboration = "CMS";
	if(name.find("template_") == std::string::npos) {
		std::cerr<<"Wrong input at constructTemplateName finction. Interrupt"<<std::endl;
		exit(1);
	}
	std::string full_name = name.std::string::substr(name.find("template_") + std::string("template_").std::string::length());
	if(name.find("_up") != std::string::npos){
		full_name = full_name.substr(0,full_name.size() - 3);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Up";
	}
	else if(name.find("_down") != std::string::npos){
		full_name = full_name.substr(0,full_name.size() - 5);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Down";
	}
	else if(name.find("Mbb") != std::string::npos){
		full_name = process + "_" + full_name;
	}

	return full_name;
}

std::shared_ptr<tools::Collection<tools::Jet> > MssmHbbSignal::modifyJetCollection(tools::Jet & jet,
						std::shared_ptr<tools::Collection<tools::Jet> > & initialJets
						){
	Jet shiftedJet = jet;
	TLorentzVector p4Shift = jet.p4() * (1 + 2.* JESshift_ * jet.jecUncert());
	jet.p4(p4Shift);
	initialJets->add(jet);
	return initialJets;
}

void MssmHbbSignal::runAnalysis(const std::string &json, const std::string &output, const int &size){

	this->setupAnalysis(json);
	std::cout<<"Total number of events: "<<this->size()<<std::endl;
	this->makeHistograms(size);
	this->SetupStandardOutputFile(output);
	if(signalMC_){
		for(int i = 0; i < 3 ; ++i){
			JERshift_ = 0;
			if(i==0) JESshift_ = 0;
			else if (i == 1){
				JESshift_ = 1;
			}
			else {
				JESshift_ = - 1;
			}

			this->applySelection();
		}

		for(int i = 1; i < 3; ++i){
			JESshift_ = 0;
			if (i == 1){
				JERshift_ = 2;
			}
			else {
				JERshift_ = -2;
			}
				this->applySelection();

		}
	}
	else{
		JERshift_ = 0;
		JESshift_ = 0;
		this->applySelection();
	}
	this->writeHistograms();


}



