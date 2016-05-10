/*
 * TriggerEfficiency.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/TriggerEfficiency.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

std::string constructTemplateName(const std::string & name);

TriggerEfficiency::TriggerEfficiency(const std::string & inputFilelist,const double & dataLumi, const bool & lowM, const bool & test) :
								selectionDoubleB(inputFilelist,dataLumi,lowM,test)
{
	nJets_ = 2;
	pt1_ = 90;
	pt2_ = 90;
	eta1_ = 2.4;
	eta2_ = 2.4;
	triggerLogicName_ = "HLT_PFJet60_v";
	triggerObjectName_ = {"hltL1sL1SingleJet36","hltSingleCaloJet40","hltSinglePFJet60"};
}

TriggerEfficiency::~TriggerEfficiency() {
}

//Member methods

const bool TriggerEfficiency::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	if(TEST) std::cout<<"I'm in TriggerEfficiency::leadingJetSelection"<<std::endl;

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	//Pt requirements
	if (jet1.pt() < pt1_) return false;
	if (jet2.pt() < pt2_) return false;

	//Eta requirements
	if (std::abs(jet1.eta()) > eta1_) return false;
	if (std::abs(jet2.eta()) > eta2_) return false;

	return true;
}

void TriggerEfficiency::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){

	if(TEST) std::cout<<"I'm in TriggerEfficiency::fillHistograms"<<std::endl;

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	selectionDoubleB::fillHistograms(offlineJets,weight);

	if(lowM_){
		if(matchToPF100_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF100_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF100_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF100_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF100_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF100_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF100dEta1p6_PF60(jet1,jet2)) 								(histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);
		if(matchToPF100L3_PF60(jet1) && matchToPF100L3_PF60(jet2)) 				(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);

		if(matchToPF100dEta1p6_PF80(jet1,jet2)) 								(histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);
		if(matchToPF100L3_PF80(jet1) && matchToPF100L3_PF60(jet2)) 				(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);

		if(matchToPF100_PF60(jet1) && matchToPF100_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_Pf60_Pf100"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf60_Pf100"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF100_PF80(jet1) && matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_Pf80_Pf100"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf80_Pf100"]->Fill(jet1.pt(),jet2.pt(),weight);
	}
	else{

		if(matchToPF160_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF160_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF160_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF160_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF160_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF160_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF160_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF160_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF160_PF60(jet1) && matchToPF160_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_Pf60_Pf160"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf60_Pf160"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF160_PF80(jet1) && matchToPF160_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_Pf80_Pf160"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf80_Pf160"]->Fill(jet1.pt(),jet2.pt(),weight);

	}

}

void TriggerEfficiency::writeHistograms(){

	//Divide Num and Den to get Efficiency:
	if(lowM_){
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf100_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_pt1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf100_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_pt2"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf100_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_eta1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf100_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_eta2"],1,1,"b");

		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf100_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_pt1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf100_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_pt2"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf100_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_eta1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf100_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_eta2"],1,1,"b");

		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf100_dEta"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf100_dEta"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf100_dEta"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf100_dEta"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf100_dEta"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf100_dEta"],1,1,"b");

		(histo_.getHisto2D())["KinTrigEff_2D_Pf60_Pf100"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_Pf60_Pf100"],(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf60_Pf100"],1,1,"b");
		(histo_.getHisto2D())["KinTrigEff_2D_Pf80_Pf100"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_Pf80_Pf100"],(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf80_Pf100"],1,1,"b");
	}
	else{

		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf160_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_pt1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf160_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_pt2"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf160_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_eta1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf60_Pf160_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf60_Pf160_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf60_Pf160_eta2"],1,1,"b");

		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf160_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_pt1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf160_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_pt2"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf160_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_eta1"],1,1,"b");
		(histo_.getHisto())["KinTrigEff_1D_Pf80_Pf160_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_Pf80_Pf160_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_Pf80_Pf160_eta2"],1,1,"b");

		(histo_.getHisto2D())["KinTrigEff_2D_Pf60_Pf160"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_Pf60_Pf160"],(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf60_Pf160"],1,1,"b");
		(histo_.getHisto2D())["KinTrigEff_2D_Pf80_Pf160"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_Pf80_Pf160"],(histo_.getHisto2D())["KinTrigEff_Den_2D_Pf80_Pf160"],1,1,"b");
	}

	std::string full_name;

	outputFile_->mkdir("distributions","general distributions of Jets and di-Jet objects");
	outputFile_->mkdir("TriggerEfficiencies","different trigger efficiencies");
	for(const auto & h: (histo_.getHisto())){
		if(h.second->GetEntries() == 0) continue; 			//skip empty histograms
		if(h.first.find("TrigEff_Din") != std::string::npos || h.first.find("TrigEff_Num") != std::string::npos) continue;	// skip Numerators and Den
		if(h.first.find("TrigEff") != std::string::npos){
			outputFile_->cd("TriggerEfficiencies");
			h.second->Write();
			outputFile_->cd("");
		}
		else {
			outputFile_->cd("distributions");
			h.second->Write();
			outputFile_->cd("");
		}
	}
	outputFile_->Close();
}

const double TriggerEfficiency::assignWeight(){
double weight = 1;
	if(isMC()) {
//		weight = weight_["dEta"] * weight_["Lumi"] * weight_["2DPt"] * weight_["BTag"] * weight_["PU_central"] * weight_["SFb_central"] * weight_["SFl_central"];
		weight = weight_["Lumi"] * weight_["PU_central"];
	}
	//std::cout<<" weight = "<<weight<<" "<<weight_["dEta"]<<" "<<weight_["Lumi"]<<" "<<weight_["2DPt"]<<" "<<weight_["BTag"]<<" "<<weight_["PU_central"]<<" "<<weight_["SFb_central"]<<" "<<weight_["SFl_central"]<<std::endl;
	return weight;
}

void TriggerEfficiency::runAnalysis(const std::string &json, const std::string &output, const int &size){

	this->setupAnalysis(json);
	std::cout<<"Total number of events: "<<this->size()<<std::endl;
	this->makeHistograms(size);
	this->SetupStandardOutputFile(output);
	this->applySelection();
	this->writeHistograms();

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

bool TriggerEfficiency::matchToPF100_PF60(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet36");
	const Candidate * l2 = jet.matched("hltSingleCaloJet40");
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 100  ) matched = true;

	delete l1;
	delete l2;
	delete l3;
	return matched;
}

bool TriggerEfficiency::matchToPF100_PF80(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 100  ) matched = true;

	delete l1;
	delete l2;
	delete l3;
	return matched;
}

bool TriggerEfficiency::matchToPF100dEta1p6_PF60(const Jet &jet1 , const Jet &jet2){
	bool matched = false;
	const Candidate * J1l3 = jet1.matched("hltSinglePFJet60");
	const Candidate * J2l3 = jet2.matched("hltSinglePFJet60");
	if( J1l3 && J2l3 && J1l3->pt() >= 100 && J2l3->pt() >= 100 && std::abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;

	delete J1l3;
	delete J2l3;
	return matched;
}

bool TriggerEfficiency::matchToPF100dEta1p6_PF80(const Jet &jet1 , const Jet &jet2){
	bool matched = false;
	const Candidate * J1l3 = jet1.matched("hltSinglePFJet80");
	const Candidate * J2l3 = jet2.matched("hltSinglePFJet80");
	if( J1l3 && J2l3 && J1l3->pt() >= 100 && J2l3->pt() >= 100 && std::abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;

	delete J1l3;
	delete J2l3;
	return matched;
}

bool TriggerEfficiency::matchToPF160_PF60(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet36");
	const Candidate * l2 = jet.matched("hltSingleCaloJet40");
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 160  ) matched = true;

	delete l1;
	delete l2;
	delete l3;
	return matched;
}

bool TriggerEfficiency::matchToPF160_PF80(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 160  ) matched = true;

	delete l1;
	delete l2;
	delete l3;
	return matched;
}


bool TriggerEfficiency::matchToPF100L3_PF60(const analysis::tools::Jet & jet){
	bool matched = false;
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if(l3 && l3->pt() >= 100) matched = true;

	delete l3;
	return matched;
}

bool TriggerEfficiency::matchToPF100L3_PF80(const analysis::tools::Jet & jet){
	bool matched = false;
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if(l3 && l3->pt() >= 100) matched = true;

	delete l3;
	return matched;
}
