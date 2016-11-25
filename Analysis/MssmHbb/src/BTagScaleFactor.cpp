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

void BTagScaleFactor::setBTagCalibrationReaders_(const BTagCalibration& lib){
//	Instances of BTagCalibrationReader for different operation points
	reader_["OP0"] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_LOOSE,  			// operating point
												  "central",										// central sys type
												  {"up","down"});           						// systematics type);

	reader_["OP1"] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_MEDIUM,"central",{"up","down"});
	reader_["OP2"] = BTagCalibrationReader(BTagEntry::OperatingPoint::OP_TIGHT,"central",{"up","down"});
}

void BTagScaleFactor::LoadPOGCalibrations_(const BTagCalibration& calib){
	//Load corrections for particular flavour:
	reader_["OP0"].load(calib,		// calibration instance
			    BTagEntry::FLAV_B,  // btag flavour
			    "comb");		// measurement type

	//Load other flavours 
	reader_["OP0"].load(calib,BTagEntry::FLAV_C,"comb");
	reader_["OP0"].load(calib,BTagEntry::FLAV_UDSG,"incl");

	//Load corrections for Medium WP
	reader_["OP1"].load(calib,BTagEntry::FLAV_B,"comb");
	reader_["OP1"].load(calib,BTagEntry::FLAV_C,"comb");
	reader_["OP1"].load(calib,BTagEntry::FLAV_UDSG,"incl");
	//Load orrections for Tight WP
	reader_["OP2"].load(calib,BTagEntry::FLAV_B,"comb");
        reader_["OP2"].load(calib,BTagEntry::FLAV_C,"comb");
        reader_["OP2"].load(calib,BTagEntry::FLAV_UDSG,"incl");
}

const BTagScaleFactor::ScaleFactor BTagScaleFactor::calculateBTagSF(const analysis::tools::Jet & jet,const int & op){

	BTagEntry::JetFlavor flav_b;
	if(jet.flavour() == 0)		{ flav_b = BTagEntry::FLAV_UDSG	;}
	else if(jet.flavour() == 5)	{ flav_b = BTagEntry::FLAV_B	;}
	else if(jet.flavour() == 4)	{ flav_b = BTagEntry::FLAV_C	;}
	else throw std::invalid_argument("Error: in BTagScaleFactor::calculateBTagSF: non b/c/udsg flavour was provided.");

	if(op!=0 && op != 1 && op != 2)
		throw std::invalid_argument("Error: in JetAnalysisBase::calculateBTagSF wrong operation point was provided");

	ScaleFactor sf;

 	sf.central 	= reader_["OP" +std::to_string(op)].eval_auto_bounds("central",flav_b,jet.eta(),jet.pt());
	sf.up 		= reader_["OP" +std::to_string(op)].eval_auto_bounds("up",flav_b,jet.eta(),jet.pt());
	sf.down		= reader_["OP" +std::to_string(op)].eval_auto_bounds("down",flav_b,jet.eta(),jet.pt());
	sf.flavour      = jet.flavour();

	return sf;
}
