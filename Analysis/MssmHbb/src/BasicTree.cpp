/*
 * BasicTree.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/BasicTree.h"
#include <iostream>

double factorizationPtWeight1D(const double &);
double dEtaWeight(const double &);
double BTagWeight(TH2F *,const double &, const double &);
double twoDPtWeight(TH2F *, const double &, const double &);

BasicTree::BasicTree() {
	// TODO Auto-generated constructor stub
}

BasicTree::~BasicTree() {
	// TODO Auto-generated destructor stub
}

void BasicTree::createOutputFile(const std::string &name){
	OutFile_ = new TFile(name.c_str(),"RECREATE");
	OutTree_ = new TTree("MssmHbb","");

	this->setBranches();

	std::cout<<"File: "<<name<<" was created"<<std::endl;
}

void BasicTree::setNjets(const int &n){
	Njets_ = n;
}

void BasicTree::setJetCounter(const int &n){
	jetCounter_ = n;
}

void BasicTree::setJetVariables(const analysis::tools::Jet & Jet){

	if(jetCounter_ == -100) {std::cerr<<"Error: Please setup jet counter with setJetCounter method"<<std::endl; exit(4);}

	LeadJet_[jetCounter_] = Jet;
	LeadPt_[jetCounter_] = Jet.pt();
	LeadEta_[jetCounter_] = Jet.eta();
	LeadPhi_[jetCounter_] = Jet.phi();
	LeadBTag_[jetCounter_] = Jet.btag();

}

void BasicTree::calculateJetVariables(){

	dPhiFS_ = acos(cos(LeadJet_[0].phi() - LeadJet_[1].phi()));
	dEtaFS_ = LeadJet_[0].eta() - LeadJet_[1].eta();
	this->setPhysObjVariables(LeadJet_[0].p4()+LeadJet_[1].p4());
}

void BasicTree::setPhysObjVariables(const TLorentzVector & Obj){

	doubleJetObj_ = Obj;

	ObjM12_ = doubleJetObj_.M();
	ObjEta_ = doubleJetObj_.Eta();
	ObjPhi_ = doubleJetObj_.Phi();
	ObjPt_ = doubleJetObj_.Pt();
	ObjP_[0] = doubleJetObj_.Px();
	ObjP_[1] = doubleJetObj_.Py();
	ObjP_[2] = doubleJetObj_.Pz();

}

void BasicTree::calculateWeights(TH2F *btag, TH2F *pt){

	FactorizationPtWeight_ = factorizationPtWeight1D(LeadJet_[0].pt()) * factorizationPtWeight1D(LeadJet_[1].pt());
	dEtaWeight_ = dEtaWeight(dEtaFS_);
//	BTagWeight_ = BTagWeight(btag,LeadJet_[0].pt(),LeadJet_[0].eta()) * BTagWeight(btag,LeadJet_[1].pt(),LeadJet_[1].eta());
	TwoDPtWeight_ = twoDPtWeight(pt,LeadJet_[0].pt(),LeadJet_[1].pt());

}

void BasicTree::calculateWeights(TH2F *btag){


	FactorizationPtWeight_ = factorizationPtWeight1D(LeadJet_[0].pt()) * factorizationPtWeight1D(LeadJet_[1].pt());
	dEtaWeight_ = dEtaWeight(dEtaFS_);
//	BTagWeight_ = BTagWeight(btag,LeadJet_[0].pt(),LeadJet_[0].eta()) * BTagWeight(btag,LeadJet_[1].pt(),LeadJet_[1].eta());

}

void BasicTree::setBTagWeight(const double & weight){
	BTagWeight_ = weight;
}

void BasicTree::setBranches(){


	OutTree_->Branch("Njets",&Njets_,"Njets/I");
	OutTree_->Branch("LeadPt", LeadPt_,"LeadPt[20]/D");
	OutTree_->Branch("LeadEta", LeadEta_,"LeadEta[20]/D");
	OutTree_->Branch("LeadPhi", LeadPhi_,"LeadPhi[20]/D");
	OutTree_->Branch("LeadBTag", LeadBTag_,"LeadBTag[20]/D");

	OutTree_->Branch("dPhiFS",&dPhiFS_,"dPhiFS/D");
	OutTree_->Branch("dEtaFS",&dEtaFS_,"dEtaFS/D");

	//Trigger and Matching Objects
	/*
	OutTree_->Branch("lowMTriggerFired",&lowMTriggerFired_,"lowMTriggerFired/I");
	OutTree_->Branch("NL1Obj",&NL1Obj_,"NL1Obj/I");
	OutTree_->Branch("L1Pt",L1Pt_,"L1Pt[NL1Obj_]/D");
	OutTree_->Branch("L1Eta",L1Eta_,"L1Eta[NL1Obj_]/D");
	OutTree_->Branch("L1Phi",L1Phi_,"L1Phi[NL1Obj_]/D");
	OutTree_->Branch("L1dR",L1dR_,"L1dR[NL1Obj_]/D");

	OutTree_->Branch("NL2Obj",&NL2Obj_,"NL2Obj/I");
	OutTree_->Branch("L2Pt",L2Pt_,"L2Pt[NL2Obj_]/D");
	OutTree_->Branch("L2Eta",L2Eta_,"L2Eta[NL2Obj_]/D");
	OutTree_->Branch("L2Phi",L2Phi_,"L2Phi[NL2Obj_]/D");
	OutTree_->Branch("L2dR",L2dR_,"L2dR[NL2Obj_]/D");

	OutTree_->Branch("NL2p5Obj",&NL2p5Obj_,"NL2p5Obj/I");
	OutTree_->Branch("L2p5Pt",L2p5Pt_,"L2p5Pt[NL2p5Obj_]/D");
	OutTree_->Branch("L2p5Eta",L2p5Eta_,"L2p5Eta[NL2p5Obj_]/D");
	OutTree_->Branch("L2p5Phi",L2p5Phi_,"L2p5Phi[NL2p5Obj_]/D");
	OutTree_->Branch("L2p5dR",L2p5dR_,"L2p5dR[NL2p5Obj_]/D");

	OutTree_->Branch("NL3Obj",&NL3Obj_,"NL3Obj/I");
	OutTree_->Branch("L3Pt",L3Pt_,"L3Pt[NL3Obj_]/D");
	OutTree_->Branch("L3Eta",L3Eta_,"L3Eta[NL3Obj_]/D");
	OutTree_->Branch("L3Phi",L3Phi_,"L3Phi[NL3Obj_]/D");
	OutTree_->Branch("L3dR",L3dR_,"L3dR[NL3Obj_]/D");
*/
	//Double Jet Object
	OutTree_->Branch("ObjM12",&ObjM12_,"ObjM12/D");
	OutTree_->Branch("ObjEta",&ObjEta_,"ObjEta/D");
	OutTree_->Branch("ObjPhi",&ObjPhi_,"ObjPhi/D");
	OutTree_->Branch("ObjPt",&ObjPt_,"ObjPt/D");
	OutTree_->Branch("ObjP",ObjP_,"ObjP[3]/D");

	//Reweighting variables
	OutTree_->Branch("FactorizationPtWeight",&FactorizationPtWeight_,"FactorizationPtWeight/D");
	OutTree_->Branch("TwoDPtWeight",&TwoDPtWeight_,"TwoDPtWeight/D");
	OutTree_->Branch("dEtaWeight",&dEtaWeight_,"dEtaWeight/D");
	OutTree_->Branch("BTagWeight",&BTagWeight_,"BTagWeight/D");
	OutTree_->Branch("LumiWeight",&lumiWeight_,"LumiWeight/D");

	//BTag SF variables:
	OutTree_->Branch("BTagSFcentral",btagSFcentral_,"BTagSFcentral[20]/D");
	OutTree_->Branch("BTagSFup",btagSFup_,"BTagSFup[20]/D");
	OutTree_->Branch("BTagSFdown",btagSFdown_,"BTagSFdown[20]/D");

	//Flavour composition
	OutTree_->Branch("cc",&cc_,"cc/I");
	OutTree_->Branch("bb",&bb_,"bb/I");
	OutTree_->Branch("qq",&qq_,"qq/I");
	OutTree_->Branch("bc",&bc_,"bc/I");
	OutTree_->Branch("bq",&bq_,"bq/I");
	OutTree_->Branch("qc",&qc_,"qc/I");




//        OutTree_->Branch("BTagSFL",&BTagSFL_,"BTa
}

void BasicTree::calculateBTagSF(const BTagCalibrationReader & reader, const BTagCalibrationReader &reader_up, const BTagCalibrationReader &reader_down){

	double btagJetPt = 0;
	btagJetPt = LeadPt_[jetCounter_];
	bool DoubleUncertainty = false;
	if(btagJetPt >= maxBJetPt__){
		btagJetPt = maxBJetPt__ - 1; //Doesn't work at the limit, so that I add "-1"
		DoubleUncertainty = true;
	}

	btagSFcentral_[jetCounter_] = reader.eval(BTagEntry::FLAV_B, LeadEta_[jetCounter_], btagJetPt);
	btagSFup_[jetCounter_] = reader_up.eval(BTagEntry::FLAV_B, LeadEta_[jetCounter_], btagJetPt);
	btagSFdown_[jetCounter_] = reader_down.eval(BTagEntry::FLAV_B, LeadEta_[jetCounter_], btagJetPt);

	// If Pt > Maximum - uncertinties should be doubled and calculated with the extremum value
	if(DoubleUncertainty){
		btagSFup_[jetCounter_] = 2*(btagSFup_[jetCounter_] - btagSFcentral_[jetCounter_]) + btagSFcentral_[jetCounter_];
		btagSFdown_[jetCounter_] = 2*(btagSFdown_[jetCounter_] - btagSFcentral_[jetCounter_]) + btagSFcentral_[jetCounter_];
	}

	//For 2 Sigma variation:
	btagSFup_[jetCounter_] = 2*(btagSFup_[jetCounter_] - btagSFcentral_[jetCounter_]) + btagSFcentral_[jetCounter_];
	btagSFdown_[jetCounter_] = 2*(btagSFdown_[jetCounter_] - btagSFcentral_[jetCounter_]) + btagSFcentral_[jetCounter_];

}


//104.991 central: 0.907225 up: 0.938664 down: 0.875786
//0.823057 0.0570449 0.0693086

void BasicTree::calculateBTagSF(const BTagCalibrationReader & reader){

	btagSFcentral_[jetCounter_] = reader.eval(BTagEntry::FLAV_B, LeadEta_[jetCounter_], LeadPt_[jetCounter_]);
}

void BasicTree::cleanVariables(){

	std::fill_n(LeadPt_,20,-100.);
	std::fill_n(LeadEta_,20,-100.);
	std::fill_n(LeadPhi_,20,-100.);
	std::fill_n(LeadBTag_,20,-100.);

	std::fill_n(btagSFcentral_,20,-100.);
	std::fill_n(btagSFdown_,20,-100);
	std::fill_n(btagSFup_,20,-100);

	Njets_ = -100;
	dPhiFS_ = -100;
	dEtaFS_ = -100;

	ObjM12_ = -100;
	ObjPt_ = -100;
	ObjEta_ = -100;
	ObjPhi_ = -100;
	std::fill_n(ObjP_,3,-100.);
	lumiWeight_ = -100;
	BTagWeight_ = -100;
	dEtaWeight_ = -100;
	TwoDPtWeight_ = -100;
	FactorizationPtWeight_ = -100;

	qq_ = 0;
	bb_ = 0;
	cc_ = 0;
	bq_ = 0;
	bc_ = 0;
	qc_ = 0;

}

void BasicTree::calculateFlavourComposition(){

	if (LeadJet_[0].flavour("Hadron") == 0 && LeadJet_[1].flavour("Hadron") == 0	)   qq_ = 1; //qq
	else if (LeadJet_[0].flavour("Hadron") == 4 && LeadJet_[1].flavour("Hadron") == 4 ) cc_ = 1; //cc
	else if (LeadJet_[0].flavour("Hadron") == 5 && LeadJet_[1].flavour("Hadron") == 5 ) bb_ = 1;
	else if ((LeadJet_[0].flavour("Hadron") == 0 || LeadJet_[1].flavour("Hadron") == 0) && ( LeadJet_[0].flavour("Hadron") == 4 || LeadJet_[1].flavour("Hadron") == 4 )) qc_ = 1;
	else if ((LeadJet_[0].flavour("Hadron") == 5 || LeadJet_[1].flavour("Hadron") == 5) && ( LeadJet_[0].flavour("Hadron") == 0 || LeadJet_[1].flavour("Hadron") == 0 )) bq_ = 1;
	else if ((LeadJet_[0].flavour("Hadron") == 5 || LeadJet_[1].flavour("Hadron") == 5) && ( LeadJet_[0].flavour("Hadron") == 4 || LeadJet_[1].flavour("Hadron") == 4 )) bc_ = 1;
	else {
		std::cerr<<"Error: Undefined color. Interrupt!"<<std::endl;
		exit(5);
	}
}

void BasicTree::fillTree(){
	OutTree_ ->Fill();
}

void BasicTree::writeTree(){
	OutTree_ ->Write();
	OutFile_ -> Close();
}

void BasicTree::setLumiWeight(const double &dataLumi, const double & MCLumi){
	lumiWeight_ = dataLumi / MCLumi;
}

double factorizationPtWeight1D(const double &pt)
{
	return 1./(1.+exp( -8.87968e-02 * (pt - 1.03879e+02) )) * 1./ (1.+exp( -1.88620e-01 * (pt - 1.02891e+02) ));
}
double dEtaWeight(const double & dEta)
{
	return 1./(1.+exp( 1.41132e+02 * (dEta - 1.60010e+00) )) * 1./ (1.+exp( -2.06352e-02 * (dEta + 2.27738e+02) ));
}
double BTagWeight(TH2F *histo, const double &pt, const double &eta)
{
	if(pt <1000) return histo -> Interpolate(pt,eta);
	else return 1;
}

double twoDPtWeight(TH2F *histo, const double &pt1, const double &pt2){
	if (pt1 > 500) {
		if (pt2 > 500){
			return histo->Interpolate(499,499);
		}
		else return histo->Interpolate(pt2,499);
	}
	else return histo -> Interpolate(pt2,pt1);
}

