/*
 * BTagScaleFactor.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/BTagScaleFactor.h"

BTagScaleFactor::BTagScaleFactor(const std::string& tagger, const std::string& path_to_csv){

	BTagCalibration lib(tagger.c_str(), (path_to_csv).c_str());
	setBTagCalibrationReaders(&lib);
}

void BTagScaleFactor::setBTagCalibrationReaders(const BTagCalibration* lib){
//	Example with comments
	reader_["OP0_FLAV_B_central"] = BTagCalibrationReader(lib,             // calibration instance
																	BTagEntry::OperatingPoint::OP_LOOSE,  		// operating point
																	"mujets",               					// measurement type
																	"central");           						// systematics type);

	reader_["OP0_FLAV_B_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"mujets","up");
	reader_["OP0_FLAV_B_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"mujets","down");

	reader_["OP0_FLAV_C_central"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"mujets","central");
	reader_["OP0_FLAV_C_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"mujets","up");
	reader_["OP0_FLAV_C_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"mujets","down");

	reader_["OP0_FLAV_UDSG_central"] 	= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"incl","central");
	reader_["OP0_FLAV_UDSG_up"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"incl","up");
	reader_["OP0_FLAV_UDSG_down"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_LOOSE,"incl","down");

	reader_["OP1_FLAV_B_central"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","central");
	reader_["OP1_FLAV_B_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","up");
	reader_["OP1_FLAV_B_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","down");

	reader_["OP1_FLAV_C_central"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","central");
	reader_["OP1_FLAV_C_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","up");
	reader_["OP1_FLAV_C_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"mujets","down");

	reader_["OP1_FLAV_UDSG_central"] 	= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"incl","central");
	reader_["OP1_FLAV_UDSG_up"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"incl","up");
	reader_["OP1_FLAV_UDSG_down"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_MEDIUM,"incl","down");

	reader_["OP2_FLAV_B_central"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","central");
	reader_["OP2_FLAV_B_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","up");
	reader_["OP2_FLAV_B_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","down");

	reader_["OP2_FLAV_C_central"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","central");
	reader_["OP2_FLAV_C_up"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","up");
	reader_["OP2_FLAV_C_down"] 			= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"mujets","down");

	reader_["OP2_FLAV_UDSG_central"] 	= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"incl","central");
	reader_["OP2_FLAV_UDSG_up"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"incl","up");
	reader_["OP2_FLAV_UDSG_down"] 		= BTagCalibrationReader(lib,BTagEntry::OperatingPoint::OP_TIGHT,"incl","down");
}

const BTagScaleFactor::ScaleFactor BTagScaleFactor::calculateBTagSF(const analysis::tools::Jet & jet,const int & op){

	std::string measurementType;
	BTagEntry::JetFlavor flav_b;
	double pt_limit = jet.pt();
	bool doubleUncertainty = false;
	if(jet.flavour() == 0)		{ flav_b = BTagEntry::FLAV_UDSG	; measurementType = "FLAV_UDSG"; }
	else if(jet.flavour() == 5)	{ flav_b = BTagEntry::FLAV_B	; measurementType = "FLAV_B"; }
	else if(jet.flavour() == 4)	{ flav_b = BTagEntry::FLAV_C	; measurementType = "FLAV_C"; }
	else throw std::invalid_argument("Error: in BTagScaleFactor::calculateBTagSF: non b/c/udsg flavour was provided.");

	if(op!=0 || op != 1 || op != 2)
		throw std::invalid_argument("Error: in JetAnalysisBase::calculateBTagSF wrong operation point was provided");

	if(jet.flavour() == 0 && pt_limit >= 1000)		{ pt_limit = 999; doubleUncertainty = true; }
	else if(jet.flavour() != 0 && pt_limit >= 670. ){ pt_limit = 669; doubleUncertainty = true; }

	ScaleFactor sf;

 	sf.central 	= reader_["OP" +std::to_string(op) + "_" + measurementType + "_" + "central"].eval(flav_b,jet.eta(),pt_limit);
	sf.up 		= reader_["OP" +std::to_string(op) + "_" + measurementType + "_" + "up"]	 .eval(flav_b,jet.eta(),pt_limit);
	sf.down		= reader_["OP" +std::to_string(op) + "_" + measurementType + "_" + "down"]	 .eval(flav_b,jet.eta(),pt_limit);
	sf.flavour  = jet.flavour();

	if(doubleUncertainty){
		sf.up 	= 2*(sf.up - sf.central) + sf.central;
		sf.down = 2*(sf.down - sf.central) + sf.central;
	}

	return sf;
}
