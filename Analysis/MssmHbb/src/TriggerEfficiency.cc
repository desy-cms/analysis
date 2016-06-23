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
	if(lowM_){
		pt1_ = 100;
		pt2_ = 100;
	}
	else{
		pt1_ = 160.;
		pt2_ = 160.;
	}
	eta1_ = 2.2;
	eta2_ = 2.2;
	dR_ = 1.;
	triggerLogicName_ = "HLT_PFJet60_v";
	triggerObjectName_ = {"hltL1sL1SingleJet36","hltSingleCaloJet40","hltSinglePFJet60","hltL1sL1SingleJet52","hltSingleCaloJet50","hltSinglePFJet80"};
	baseOutputName_ = "TriggerEff";
}

TriggerEfficiency::~TriggerEfficiency() {
}

//Member methods

const bool TriggerEfficiency::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

//	if(TEST) std::cout<<"I'm in TriggerEfficiency::leadingJetSelection"<<std::endl;

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	//Pt requirements
	if (jet1.pt() < pt1_) return false;
	if (jet2.pt() < pt2_) return false;

	//Eta requirements
	if (std::abs(jet1.eta()) > eta1_) return false;
	if (std::abs(jet2.eta()) > eta2_) return false;

	//deltaR requirements
	if (jet1.deltaR(jet2) <= dR_) return false;

	//true b-jets (FOR TEST only)
//	if( isMC() && jet1.flavour("Hadron") != 5) return false;
//	if( isMC() && jet2.flavour("Hadron") != 5) return false;

	return true;
}

void TriggerEfficiency::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){

//	if(TEST) std::cout<<"I'm in TriggerEfficiency::fillHistograms"<<std::endl;

//	std::cout<<"lol1"<<std::endl;

	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);

	selectionDoubleB::fillHistograms(offlineJets,weight);

	(histo_.getHisto())["jet_turnOn_pt1"]->Fill(jet1.pt(),weight);
	if(isMC() && genJets_->size() != 0) (histo_.getHisto())["Genjet_turnOn_pt1"]->Fill((genJets_->at(0)).pt(),weight);

	const Candidate * j1l1 = jet1.matched("hltL1sL1SingleJet52");
	const Candidate * j1l2 = jet1.matched("hltSingleCaloJet50");
	const Candidate * j1l3 = jet1.matched("hltSinglePFJet80");

	auto offlineMuons = this->collection<Muon>("Muons");
	if(matchToPF100_PF80(jet1)){

		(histo_.getHisto())["onlobject_L1_pt1"]->Fill(j1l1->pt(),weight);
		(histo_.getHisto())["onlobject_L2_pt1"]->Fill(j1l2->pt(),weight);
		(histo_.getHisto())["onlobject_L3_pt1"]->Fill(j1l3->pt(),weight);

		(histo_.getHisto())["onlobject_turnOn_L1_pt1"]->Fill(j1l1->pt(),weight);
		(histo_.getHisto())["onlobject_turnOn_L2_pt1"]->Fill(j1l2->pt(),weight);
		(histo_.getHisto())["onlobject_turnOn_L3_pt1"]->Fill(j1l3->pt(),weight);

		(histo_.getHisto())["onlobject_L1_eta1"]->Fill(j1l1->eta(),weight);
		(histo_.getHisto())["onlobject_L2_eta1"]->Fill(j1l2->eta(),weight);
		(histo_.getHisto())["onlobject_L3_eta1"]->Fill(j1l3->eta(),weight);

		(histo_.getHisto2D())["onlobject_L1_pt1_vs_off"]->Fill(j1l1->pt(),jet1.pt(),weight);
		(histo_.getHisto2D())["onlobject_L2_pt1_vs_off"]->Fill(j1l2->pt(),jet1.pt(),weight);
		(histo_.getHisto2D())["onlobject_L3_pt1_vs_off"]->Fill(j1l3->pt(),jet1.pt(),weight);

		(histo_.getHisto2D())["onlobject_turnOn_L1_pt1_vs_off"]->Fill(j1l1->pt(),jet1.pt(),weight);
		(histo_.getHisto2D())["onlobject_turnOn_L2_pt1_vs_off"]->Fill(j1l2->pt(),jet1.pt(),weight);
		(histo_.getHisto2D())["onlobject_turnOn_L3_pt1_vs_off"]->Fill(j1l3->pt(),jet1.pt(),weight);

		(histo_.getHisto())["onlobject_turnOn_L1ovOff_pt1"]->Fill(j1l1->pt()/jet1.pt(),weight);
		(histo_.getHisto())["onlobject_turnOn_L2ovOff_pt1"]->Fill(j1l2->pt()/jet1.pt(),weight);
		(histo_.getHisto())["onlobject_turnOn_L3ovOff_pt1"]->Fill(j1l3->pt()/jet1.pt(),weight);
	}
	//Level by level check
	if(this->triggerResult("HLT_PFJet80_v") && j1l1 && j1l1->pt() >= 100) (histo_.getHisto())["KinTrigEff_Num_1D_L1_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
	if(this->triggerResult("HLT_PFJet80_v") ) (histo_.getHisto())["KinTrigEff_Den_1D_L1_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

	if(this->triggerResult("HLT_PFJet80_v") && j1l2 && j1l2->pt() >= 100) (histo_.getHisto())["KinTrigEff_Num_1D_L2_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
	if(this->triggerResult("HLT_PFJet80_v") ) (histo_.getHisto())["KinTrigEff_Den_1D_L2_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

	if(this->triggerResult("HLT_PFJet80_v") && j1l3 && j1l3->pt() >= 100) (histo_.getHisto())["KinTrigEff_Num_1D_L3_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
	if(this->triggerResult("HLT_PFJet80_v") ) (histo_.getHisto())["KinTrigEff_Den_1D_L3_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

	//Without muons
	if(matchToPF100_PF80(jet1) && offlineMuons->size() == 0) (histo_.getHisto())["KinTrigEff_Num_1D_noMuons_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
	if(matchToPF80(jet1) && offlineMuons->size() == 0 ) (histo_.getHisto())["KinTrigEff_Den_1D_noMuons_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

	//with csvv2T
	if(matchToPF100_PF80(jet1) && jet1.btag()>= 0.935 && jet2.btag()>=0.935) (histo_.getHisto())["KinTrigEff_Num_1D_csvv2T_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
	if(matchToPF80(jet1) && jet1.btag()>= 0.935 && jet2.btag()>=0.935) (histo_.getHisto())["KinTrigEff_Den_1D_csvv2T_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

	if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_pt1_distr_matched_PFJet80"]->Fill(jet1.pt(),weight);
	if(matchToPF80(jet1)) (histo_.getHisto())["KinTrigEff_pt1_distr_not_matched_PFJet80"]->Fill(jet1.pt(),weight);

	if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_eta1_distr_matched_PFJet80"]->Fill(jet1.eta(),weight);
	if(matchToPF80(jet1)) (histo_.getHisto())["KinTrigEff_eta1_distr_not_matched_PFJet80"]->Fill(jet1.eta(),weight);

	if(offlineJets->size() >= 3){ if(offlineJets->at(2).pt() > 30) (histo_.getHisto())["jet_dR13"]->Fill(jet1.deltaR(offlineJets->at(2)),weight);}
	if(offlineJets->size() >= 4){ if(offlineJets->at(3).pt() > 30) (histo_.getHisto())["jet_dR14"]->Fill(jet1.deltaR(offlineJets->at(3)),weight);}
	if(offlineJets->size() >= 5){ if(offlineJets->at(4).pt() > 30) (histo_.getHisto())["jet_dR15"]->Fill(jet1.deltaR(offlineJets->at(4)),weight);}
	if(offlineJets->size() >= 6){ if(offlineJets->at(5).pt() > 30) (histo_.getHisto())["jet_dR16"]->Fill(jet1.deltaR(offlineJets->at(5)),weight);}
	if(offlineJets->size() >= 7){ if(offlineJets->at(6).pt() > 30) (histo_.getHisto())["jet_dR17"]->Fill(jet1.deltaR(offlineJets->at(6)),weight);}
	if(offlineJets->size() >= 8){ if(offlineJets->at(7).pt() > 30) (histo_.getHisto())["jet_dR18"]->Fill(jet1.deltaR(offlineJets->at(7)),weight);}
	if(lowM_){
		if(matchToPF100_PF80(jet1) && std::abs(jet1.eta()) < 1.5 ) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1) && std::abs(jet1.eta()) < 1.5 )		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta"]->Fill(jet1.pt(),weight);

		if(matchToPF100_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_pt1"]->Fill(jet1.pt(),weight);
		
		if(matchToPF100_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_pt2"]->Fill(jet2.pt(),weight);
		
		if(matchToPF100_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_eta1"]->Fill(jet1.eta(),weight);
		
		if(matchToPF100_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF100_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF100_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_eta2"]->Fill(jet2.eta(),weight);
		
		if(matchToPF100dEta1p6_PF60(jet1,jet2)) 								(histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);
		if(matchToPF100L3_PF60(jet1) && matchToPF100L3_PF60(jet2)) 				(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);

		if(matchToPF100dEta1p6_PF80(jet1,jet2)) 								(histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);
		if(matchToPF100L3_PF80(jet1) && matchToPF100L3_PF60(jet2)) 				(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_dEta"]->Fill(std::abs(jet1.eta()-jet2.eta()),weight);

		if(matchToPF100_PF60(jet1) && matchToPF100_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);	// pt1 vs pt2
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF100_PF60(jet1) && matchToPF100_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);	// pt2 vs pt1
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);

		if(matchToPF100_PF80(jet1) && matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);	// pt1 vs pt2
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF100_PF80(jet1) && matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);	// pt2 vs pt1
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);

		if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_finebins_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_finebins_pt1"]->Fill(jet1.pt(),weight);

		//eta dependence
		if(std::abs(jet1.eta()) < 1. && std::abs(jet2.eta()) < 1.){
		if(matchToPF100_PF80(jet1) && matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_eta_l1_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);	// pt1 vs pt2
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_eta_l1_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF100_PF80(jet1) && matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_eta_l1_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);	// pt2 vs pt1
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_eta_l1_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);
		}
		//latest pt,eta dependence
		if(matchToPF100_PF80(jet1))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);
		if(matchToPF80(jet1))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);

		if(matchToPF100_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);
		if(matchToPF80(jet2))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);

		if(matchToPF100_PF60(jet1))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);
		if(matchToPF60(jet1))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);

		if(matchToPF100_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);
		if(matchToPF60(jet2))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);

		if(isMC()){
			if(matchToPF100_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_True_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
			(histo_.getHisto())["KinTrigEff_Den_1D_True_PF80_PF100_pt1"]->Fill(jet1.pt(),weight);
		}
	}
	else{

		if(matchToPF160_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF160_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF160_PF60(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF60(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF160_PF60(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF60(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF160_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_pt1"]->Fill(jet1.pt(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_pt1"]->Fill(jet1.pt(),weight);

		if(matchToPF160_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_pt2"]->Fill(jet2.pt(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_pt2"]->Fill(jet2.pt(),weight);

		if(matchToPF160_PF80(jet1)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_eta1"]->Fill(jet1.eta(),weight);
		if(matchToPF80(jet1)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_eta1"]->Fill(jet1.eta(),weight);

		if(matchToPF160_PF80(jet2)) (histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_eta2"]->Fill(jet2.eta(),weight);
		if(matchToPF80(jet2)) 		(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_eta2"]->Fill(jet2.eta(),weight);

		if(matchToPF160_PF60(jet1) && matchToPF160_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF160_PF60(jet1) && matchToPF160_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);
		if(matchToPF60(jet1) && matchToPF60(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);

		if(matchToPF160_PF80(jet1) && matchToPF160_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt1vspt2"]->Fill(jet1.pt(),jet2.pt(),weight);

		if(matchToPF160_PF80(jet1) && matchToPF160_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);
		if(matchToPF80(jet1) && matchToPF80(jet2))				(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt2vspt1"]->Fill(jet2.pt(),jet1.pt(),weight);

		//latest pt,eta dependence
		if(matchToPF160_PF80(jet1))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);
		if(matchToPF80(jet1))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);

		if(matchToPF160_PF80(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);
		if(matchToPF80(jet2))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);

		if(matchToPF160_PF60(jet1))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);
		if(matchToPF60(jet1))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt1eta1"]->Fill(jet1.pt(),std::abs(jet1.eta()),weight);

		if(matchToPF160_PF60(jet2))	(histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);
		if(matchToPF60(jet2))		(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt2eta2"]->Fill(jet2.pt(),std::abs(jet2.eta()),weight);
	}
}

void TriggerEfficiency::writeHistograms(){

	//Divide Num and Den to get Efficiency:
	if(lowM_){
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_pt1_central_eta"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta"]);

		(histo_.getHisto())["KinTrigEff_1D_PF60_PF100_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_pt1"]);
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF100_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_pt2"] );
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF100_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_eta1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF100_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_eta2"] );

		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_pt2"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_eta1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_eta2"] );

		(histo_.getHisto())["KinTrigEff_1D_PF60_PF100_dEta"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF100_dEta"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF100_dEta"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_dEta"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_dEta"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_dEta"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF100_pt1vspt2"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt1vspt2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt1vspt2"] );
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_pt1vspt2"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt1vspt2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt1vspt2"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF100_pt2vspt1"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt2vspt1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt2vspt1"] );
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_pt2vspt1"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt2vspt1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt2vspt1"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_eta_l1_pt1vspt2"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_eta_l1_pt1vspt2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_eta_l1_pt1vspt2"] );
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_eta_l1_pt2vspt1"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_eta_l1_pt2vspt1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_eta_l1_pt2vspt1"] );

		(histo_.getHisto())["KinTrigEff_1D_PF80_PF100_finebins_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF100_finebins_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF100_finebins_pt1"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_pt1eta1"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt1eta1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt1eta1"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF100_pt2eta2"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF100_pt2eta2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF100_pt2eta2"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF100_pt1eta1"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt1eta1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt1eta1"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF100_pt2eta2"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF100_pt2eta2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF100_pt2eta2"]);
	}
	else{

		(histo_.getHisto())["KinTrigEff_1D_PF60_PF160_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_pt1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF160_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_pt2"] );
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF160_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_eta1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF60_PF160_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF60_PF160_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF60_PF160_eta2"] );

		(histo_.getHisto())["KinTrigEff_1D_PF80_PF160_pt1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_pt1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_pt1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF160_pt2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_pt2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_pt2"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF160_eta1"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_eta1"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_eta1"] );
		(histo_.getHisto())["KinTrigEff_1D_PF80_PF160_eta2"]->Divide((histo_.getHisto())["KinTrigEff_Num_1D_PF80_PF160_eta2"],(histo_.getHisto())["KinTrigEff_Den_1D_PF80_PF160_eta2"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF160_pt1vspt2"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt1vspt2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt1vspt2"] );
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF160_pt1vspt2"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt1vspt2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt1vspt2"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF160_pt2vspt1"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt2vspt1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt2vspt1"] );
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF160_pt2vspt1"] -> Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt2vspt1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt2vspt1"] );

		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF160_pt1eta1"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt1eta1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt1eta1"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF80_PF160_pt2eta2"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF80_PF160_pt2eta2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF80_PF160_pt2eta2"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF160_pt1eta1"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt1eta1"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt1eta1"]);
		(histo_.getHisto2D())["KinTrigEff_2D_PF60_PF160_pt2eta2"]->Divide((histo_.getHisto2D())["KinTrigEff_Num_2D_PF60_PF160_pt2eta2"],(histo_.getHisto2D())["KinTrigEff_Den_2D_PF60_PF160_pt2eta2"]);
	}

	std::string full_name;

	outputFile_->mkdir("distributions","general distributions of Jets and di-Jet objects");
	outputFile_->mkdir("TriggerEfficiencies","different trigger efficiencies");
	for(const auto & h: (histo_.getHisto())){
		if(h.second->GetEntries() == 0) continue; 			//skip empty histograms
		//if(h.first.find("TrigEff_Den") != std::string::npos || h.first.find("TrigEff_Num") != std::string::npos) continue;	// skip Numerators and Den
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

	//2d Efficiency:
	for(const auto & h: (histo_.getHisto2D())){
		if(h.second->GetEntries() == 0) continue; 			//skip empty histograms
		//if(h.first.find("TrigEff_Den") != std::string::npos || h.first.find("TrigEff_Num") != std::string::npos) continue;	// skip Numerators and Den
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
		weight = weight_["Lumi"] ;//* weight_["PU_central"];
	}
	//std::cout<<" weight = "<<weight<<" "<<weight_["dEta"]<<" "<<weight_["Lumi"]<<" "<<weight_["2DPt"]<<" "<<weight_["BTag"]<<" "<<weight_["PU_central"]<<" "<<weight_["SFb_central"]<<" "<<weight_["SFl_central"]<<std::endl;
	return weight;
}

void TriggerEfficiency::runAnalysis(const std::string &json, const std::string &output, const int &size){

	this->setupAnalysis(json);
	std::cout<<"\n Total number of events: "<<this->size()<<std::endl;
	this->makeHistograms(size);
	this->SetupStandardOutputFile(output);
	this->applySelection();
	this->writeHistograms();
}

bool TriggerEfficiency::matchToPF60(const Jet &jet){
	bool matched = false;
//	if(jet.matched("hltL1sL1SingleJet36") && jet.matched("hltSingleCaloJet40") &&jet.matched("hltSinglePFJet60") ) matched = true;
	matched = this->triggerResult("HLT_PFJet60_v");
	return matched;
}

bool TriggerEfficiency::matchToPF80(const Jet &jet){
	bool matched = false;
//	if(jet.matched("hltL1sL1SingleJet52") && jet.matched("hltSingleCaloJet50") &&jet.matched("hltSinglePFJet80") ) matched = true;
	matched = this->triggerResult("HLT_PFJet80_v");
	return matched;
}

bool TriggerEfficiency::matchToPF100_PF60(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet36");
	const Candidate * l2 = jet.matched("hltSingleCaloJet40");
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 100  ) matched = true;
	
	return matched;
}

bool TriggerEfficiency::matchToPF100_PF80(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 100  ) matched = true;

	return matched;
}

bool TriggerEfficiency::matchToPF100dEta1p6_PF60(const Jet &jet1 , const Jet &jet2){
	bool matched = false;
	const Candidate * J1l3 = jet1.matched("hltSinglePFJet60");
	const Candidate * J2l3 = jet2.matched("hltSinglePFJet60");
	if( J1l3 && J2l3 && J1l3->pt() >= 100 && J2l3->pt() >= 100 && std::abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;

	return matched;
}

bool TriggerEfficiency::matchToPF100dEta1p6_PF80(const Jet &jet1 , const Jet &jet2){
	bool matched = false;
	const Candidate * J1l3 = jet1.matched("hltSinglePFJet80");
	const Candidate * J2l3 = jet2.matched("hltSinglePFJet80");
	if( J1l3 && J2l3 && J1l3->pt() >= 100 && J2l3->pt() >= 100 && std::abs(J1l3->eta() - J2l3->eta()) <= 1.6) matched = true;

	return matched;
}

bool TriggerEfficiency::matchToPF160_PF60(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet36");
	const Candidate * l2 = jet.matched("hltSingleCaloJet40");
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 160  ) matched = true;

	return matched;
}

bool TriggerEfficiency::matchToPF160_PF80(const Jet &jet){
	bool matched = false;
	const Candidate * l1 = jet.matched("hltL1sL1SingleJet52");
	const Candidate * l2 = jet.matched("hltSingleCaloJet50");
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if( l1 && l2 && l3 && l1->pt() >= 100 && l2->pt() >= 100 && l3->pt() >= 160  ) matched = true;

	return matched;
}


bool TriggerEfficiency::matchToPF100L3_PF60(const analysis::tools::Jet & jet){
	bool matched = false;
	const Candidate * l3 = jet.matched("hltSinglePFJet60");
	if(l3 && l3->pt() >= 100) matched = true;

	return matched;
}

bool TriggerEfficiency::matchToPF100L3_PF80(const analysis::tools::Jet & jet){
	bool matched = false;
	const Candidate * l3 = jet.matched("hltSinglePFJet80");
	if(l3 && l3->pt() >= 100) matched = true;

	return matched;
}
