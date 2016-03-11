/*
 * BasicTree.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/BasicTree.h"
#include <iostream>

BasicTree::BasicTree() {
	// TODO Auto-generated constructor stub
}

BasicTree::~BasicTree() {
	// TODO Auto-generated destructor stub
}

void BasicTree::SetupStandardOutputFile(const std::string & outputFileName, const std::string & folderName){

	//To get Name of the Sample (really not very nice solution)
	// Maybe some recursive function could be writen
	std::string outputFileName__ = folderName;
	size_t found;
	found=outputFileName__.find_last_of("/");
	outputFileName__ = outputFileName__.substr(0,found);
	found = outputFileName__.find_last_of("/");
	outputFileName__ = outputFileName__.substr(0,found);
	found = outputFileName__.find_last_of("/");
	outputFileName__ = outputFileName__.substr(0,found);
	found = outputFileName__.find_last_of("/");

	if(is_mc_){
		outputFileName__ = outputFileName__.substr(0,found);
		found = outputFileName__.find_last_of("/");
		outputFileName__ = outputFileName__.substr(found+1);
	}
	else{
		outputFileName__ = outputFileName__.substr(found+1);
	}

	outputFileName_ = outputFileName + outputFileName__ + "_" + get_date()  + ".root";
	//Create output TFile
	this->createOutputFile(outputFileName_);

}

void BasicTree::createOutputFile(const std::string &name){
	OutFile_ = new TFile(name.c_str(),"RECREATE");
	OutTree_ = new TTree("MssmHbb","");

	std::cout<<"File: "<<name<<" was created"<<std::endl;
}

std::string BasicTree::get_date(){

	time_t now;
	char the_date[12];
	the_date[0] = '\0';

	now = time(NULL);

   if (now != -1)
   {
      strftime(the_date, 12, "%d_%m_%Y", gmtime(&now));
   }

   return std::string(the_date);
}

void BasicTree::cleanVariables(){
	std::fill_n(ObjP_,3,-100.);
	ObjM12_ = -100;
	ObjPt_ = -100;
	ObjEta_ = -100;
	ObjPhi_ = -100;
	Njets_ = -100;
	dPhiFS_ = -100;
	dEtaFS_ = -100;
	Ht_ = 0;
	std::fill_n(LeadPt_,20,-100.);
	std::fill_n(LeadEta_,20,-100.);
	std::fill_n(LeadPhi_,20,-100.);
	std::fill_n(LeadBTag_,20,-100.);
	if(is_mc_){
		lumiWeight_ = -100;
		dEtaWeight_ = -100;
		TwoDPtWeight_ = -100;
		FactorizationPtWeight_ = -100;

		std::fill_n(LeadFl_,20,-100);
	}
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

	if(is_mc_){
		LeadFl_[jetCounter_] = Jet.flavour("Hadron");
	}

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

void BasicTree::setBranches(){

	OutTree_->Branch("TotalNumberOfEvents",&Ntot_,"TotalNumberOfEvents/I");

	OutTree_->Branch("Njets",&Njets_,"Njets/I");
	OutTree_->Branch("LeadPt", LeadPt_,"LeadPt[20]/D");
	OutTree_->Branch("LeadEta", LeadEta_,"LeadEta[20]/D");
	OutTree_->Branch("LeadPhi", LeadPhi_,"LeadPhi[20]/D");
	OutTree_->Branch("LeadBTag", LeadBTag_,"LeadBTag[20]/D");

	OutTree_->Branch("dPhiFS",&dPhiFS_,"dPhiFS/D");
	OutTree_->Branch("dEtaFS",&dEtaFS_,"dEtaFS/D");
	OutTree_->Branch("Ht",&Ht_,"Ht/D");

	OutTree_->Branch("ObjM12",&ObjM12_,"ObjM12/D");
	OutTree_->Branch("ObjEta",&ObjEta_,"ObjEta/D");
	OutTree_->Branch("ObjPhi",&ObjPhi_,"ObjPhi/D");
	OutTree_->Branch("ObjPt",&ObjPt_,"ObjPt/D");
	OutTree_->Branch("ObjP",ObjP_,"ObjP[3]/D");

	if(is_mc_){
		OutTree_->Branch("LeadFl",LeadFl_,"LeadFl[20]/I");
		OutTree_->Branch("LumiWeight",&lumiWeight_,"LumiWeight/D");
		OutTree_->Branch("FactorizationPtWeight",&FactorizationPtWeight_,"FactorizationPtWeight/D");
		OutTree_->Branch("TwoDPtWeight",&TwoDPtWeight_,"TwoDPtWeight/D");
		OutTree_->Branch("dEtaWeight",&dEtaWeight_,"dEtaWeight/D");
	}

}

void BasicTree::calculateJetVariables(){

	dPhiFS_ = acos(cos(LeadJet_[0].phi() - LeadJet_[1].phi()));
	dEtaFS_ = LeadJet_[0].eta() - LeadJet_[1].eta();
	this->setPhysObjVariables(LeadJet_[0].p4()+LeadJet_[1].p4());
}

void BasicTree::setLumiWeight(const double &dataLumi, const double & MCLumi){
	lumiWeight_ = dataLumi / MCLumi;
}

void BasicTree::fillTree(){
	NCand_ ++;
	OutTree_ ->Fill();
}

void BasicTree::writeTree(){
	OutTree_ ->Write();
	OutFile_ -> Close();
}



