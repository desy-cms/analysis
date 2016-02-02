/*
 * BasicTree.h
 *
 *  Created on: Dec 17, 2015
 *      Author: shevchen
 */

#ifndef MSSMHBB_SRC_BASICTREE_H_
#define MSSMHBB_SRC_BASICTREE_H_

#include "TTree.h"
#include "Analysis/Tools/interface/Jet.h"
#include "Analysis/Tools/interface/TriggerObject.h"
#include "TLorentzVector.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraph2D.h"

//For BTag SF calculation
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"

class BasicTree {
public:
	BasicTree();
	virtual ~BasicTree();

	//Methods Declaration
	void calculateBTagSF(const BTagCalibrationReader & reader, const BTagCalibrationReader &reader_up, const BTagCalibrationReader &reader_down);
	void calculateBTagSF(const BTagCalibrationReader & reader);
	void calculateFlavourComposition();

	//Sets
	void setNjets(const int &n);
	void setJetCounter(const int &n);
	void addHt(const double &pt);

	void calculateJetVariables();
	void calculateWeights(TH2F *btag,TH2F *pt);
	void calculateWeights(TH2F *btag);
	void setBTagWeight(const double & weight);
	void setLumiWeight(const double & dataLumi, const double & mcLumi);

	void setJetVariables(const analysis::tools::Jet & Jet);
	void setPhysObjVariables(const TLorentzVector & Obj);
	void cleanVariables();

	//Create Output File and TTree
	void createOutputFile(const std::string &name = "output.root");
	//Construct output Branches
	void setBranches();
	//Fill Output Tree
	void fillTree();
	//Write Output Tree to the File and Close the File
	void writeTree();

	//Gets
	void getdPhi();
	int getNumberOfCandidates();
	int getNcand();

private:

	//Methods Declaration

	// Declare Variables
	int NCand_ = 0;
	analysis::tools::Jet LeadJet_[20];
	//Jet Variables
	int Njets_;
    double LeadPt_[20];
    double LeadEta_[20];
    double LeadPhi_[20];
    double LeadBTag_[20];
    double dPhiFS_;
    double dEtaFS_;
    double Ht_;

    //Trigger and Matching variables
    int lowMTriggerFired_;
    int NL1Obj_;
    double L1Pt_[20];
    double L1Eta_[20];
    double L1Phi_[20];
    double L1dR_[20];

    int NL2Obj_;
    double L2Pt_[20];
    double L2Eta_[20];
    double L2Phi_[20];
    double L2dR_[20];

    int NL2p5Obj_;
    double L2p5Pt_[20];
    double L2p5Eta_[20];
    double L2p5Phi_[20];
    double L2p5dR_[20];

    int NL3Obj_;
    double L3Pt_[20];
    double L3Eta_[20];
    double L3Phi_[20];
    double L3dR_[20];

    //Double Jet object
    TLorentzVector doubleJetObj_;
    double ObjP_[3];
    double ObjM12_;
    double ObjEta_;
    double ObjPhi_;
    double ObjPt_;

    // Reweighting variables
    double FactorizationPtWeight_;
    double TwoDPtWeight_;
    double dEtaWeight_;
    double BTagWeight_;
    double lumiWeight_;

    // BTag SF weight
    double btagSFcentral_[20];
    double btagSFup_[20];
    double btagSFdown_[20];

    TFile *OutFile_;
    TTree *OutTree_;
    int jetCounter_ = -100;

    // Flavour composition:
    int cc_;
    int bb_;
    int qq_;
    int bc_;
    int qc_;
    int bq_;

    //Functions:


protected:

    double maxBJetPt__ = 670.; // For BTagCalibrationStandAlone





};

inline int BasicTree::getNumberOfCandidates(){return NCand_;}
inline int BasicTree::getNcand(){return NCand_;}
inline void BasicTree::addHt(const double &pt) {Ht_ += pt;}


#endif /* MSSMHBB_SRC_BASICTREE_H_ */

