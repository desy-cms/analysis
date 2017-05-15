/*
 * BTagScaleFactor.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/BTagScaleFactor.h"

BTagScaleFactor::BTagScaleFactor(const std::string& tagger, const std::string& path_to_csv){
	BTagCalibration lib(tagger.c_str(), (path_to_csv).c_str());
	setBTagCalibrationReaders_(lib);
	LoadPOGCalibrations_(lib);
}

void BTagScaleFactor::Setup(const std::string& tagger, const std::string& path_to_csv){
	BTagCalibration lib(tagger.c_str(), (path_to_csv).c_str());
	setBTagCalibrationReaders_(lib);
	LoadPOGCalibrations_(lib);
}

void BTagScaleFactor::Setup(const std::string& tagger, const std::vector<std::string>& path_to_csv){
	int i = 0;
	for(const auto& csv : path_to_csv){
		BTagCalibration lib(tagger.c_str(), csv);
		std::string additional = "_period" + std::to_string(i);
		setBTagCalibrationReaders_(lib,additional);
		LoadPOGCalibrations_(lib,additional);
		++i;
	}
}

void BTagScaleFactor::setBTagCalibrationReaders_(const BTagCalibration& lib, const std::string& add){
//	Instances of BTagCalibrationReader for different operation points
	reader_["OP0" + add] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_LOOSE,  			// operating point
												  "central",										// central sys type
												  {"up","down"});           						// systematics type);

	reader_["OP1" + add] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_MEDIUM,"central",{"up","down"});
	reader_["OP2" + add] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_TIGHT,"central",{"up","down"});
}

void BTagScaleFactor::LoadPOGCalibrations_(const BTagCalibration& calib, const std::string& add){
	//Load corrections for particular flavour:
	reader_["OP0" + add].load(calib,		// calibration instance
			    BTagEntry::FLAV_B,  // btag flavour
			    "comb");		// measurement type

	//Load other flavours 
	reader_["OP0" + add].load(calib,BTagEntry::FLAV_C,"comb");
	reader_["OP0" + add].load(calib,BTagEntry::FLAV_UDSG,"incl");

	//Load corrections for Medium WP
	reader_["OP1" + add].load(calib,BTagEntry::FLAV_B,"comb");
	reader_["OP1" + add].load(calib,BTagEntry::FLAV_C,"comb");
	reader_["OP1" + add].load(calib,BTagEntry::FLAV_UDSG,"incl");
	//Load orrections for Tight WP
	reader_["OP2" + add].load(calib,BTagEntry::FLAV_B,"comb");
	reader_["OP2" + add].load(calib,BTagEntry::FLAV_C,"comb");
	reader_["OP2" + add].load(calib,BTagEntry::FLAV_UDSG,"incl");
}

//const BTagScaleFactor::ScaleFactor BTagScaleFactor::calculateBTagSF(const analysis::tools::Jet & jet,const int & op, const int& period){
//
//	BTagEntry::JetFlavor flav_b;
//	if(jet.flavour() == 0)		{ flav_b = BTagEntry::FLAV_UDSG	;}
//	else if(jet.flavour() == 5)	{ flav_b = BTagEntry::FLAV_B	;}
//	else if(jet.flavour() == 4)	{ flav_b = BTagEntry::FLAV_C	;}
//	else throw std::invalid_argument("Error: in BTagScaleFactor::calculateBTagSF: non b/c/udsg flavour was provided.");
//
//	if(op!=0 && op != 1 && op != 2)
//		throw std::invalid_argument("Error: in JetAnalysisBase::calculateBTagSF wrong operation point was provided");
//
//	ScaleFactor sf;
//
// 	sf.central 	= reader_["OP" +std::to_string(op) + "_period" + std::string(period)].eval_auto_bounds("central",flav_b,jet.eta(),jet.pt());
//	sf.up 		= reader_["OP" +std::to_string(op) + "_period" + std::string(period)].eval_auto_bounds("up",flav_b,jet.eta(),jet.pt());
//	sf.down		= reader_["OP" +std::to_string(op) + "_period" + std::string(period)].eval_auto_bounds("down",flav_b,jet.eta(),jet.pt());
//	sf.flavour      = jet.flavour();
//
//	return sf;
//}

const std::vector<BTagScaleFactor::ScaleFactor> BTagScaleFactor::calculateBTagSF(const analysis::tools::Jet & jet,const int & op){

	BTagEntry::JetFlavor flav_b;
	if(jet.flavour() == 0)		{ flav_b = BTagEntry::FLAV_UDSG	;}
	else if(jet.flavour() == 5)	{ flav_b = BTagEntry::FLAV_B	;}
	else if(jet.flavour() == 4)	{ flav_b = BTagEntry::FLAV_C	;}
	else throw std::invalid_argument("Error: in BTagScaleFactor::calculateBTagSF: non b/c/udsg flavour was provided.");

	if(op!=0 && op != 1 && op != 2)
		throw std::invalid_argument("Error: in JetAnalysisBase::calculateBTagSF wrong operation point was provided");

	std::vector<ScaleFactor> v_sf;
	for(auto i = 0; i != int(reader_.size() / 3); ++i){

		ScaleFactor sf;
	 	sf.central 	= reader_["OP" +std::to_string(op) + "_period" + std::to_string(i)].eval_auto_bounds("central",flav_b,jet.eta(),jet.pt());
		sf.up 		= reader_["OP" +std::to_string(op) + "_period" + std::to_string(i)].eval_auto_bounds("up",flav_b,jet.eta(),jet.pt());
		sf.down		= reader_["OP" +std::to_string(op) + "_period" + std::to_string(i)].eval_auto_bounds("down",flav_b,jet.eta(),jet.pt());
		sf.flavour      = jet.flavour();

		v_sf.push_back(sf);
	}

	return v_sf;
}
