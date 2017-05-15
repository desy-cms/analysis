/*
 * BTagScaleFactor.h
 *
 *  Created on: Nov 21, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_SRC_BTAGSCALEFACTOR_H_
#define MSSMHBB_SRC_BTAGSCALEFACTOR_H_

#include <string>
#include <map>

//#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h" // official tool provided by BTag POG
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "Analysis/Tools/interface/Jet.h"

class BTagScaleFactor {
//	Class to Calculate BTag scale factors, using official SW provided by BTag POG.

public:
//	Default constructor is forbiden
	BTagScaleFactor() = default;
//	Default destructor:
	virtual ~BTagScaleFactor() = default;
//	Default copy constructor:
	BTagScaleFactor(const BTagScaleFactor& b) = default;
//	Constructor from tagger and .csv path
	BTagScaleFactor(const std::string& tagger, const std::string& path_to_csv);
//	Method to setup everything
	void Setup(const std::string& tagger, const std::string& path_to_csv);
//	Setup for different .csv for different periods
	void Setup(const std::string& tagger, const std::vector<std::string>& path_to_csv);

    struct ScaleFactor{
    	ScaleFactor() : central(0), up(0), down (0), flavour(-100) {};
    	ScaleFactor(const double & centr, const double & upper, const double & lower, const int & flav) :
    		central(centr),
    		up(upper),
    		down(lower),
			flavour(flav) {};
    	~ScaleFactor(){};
    	void clear(){central = -100, up = -100, down = -100, flavour = -100;}
    	double 	central{};
    	double 	up{};
    	double 	down{};
    	int		flavour{};
    };

//    Method to perform a calcualtion of BTag Sfs and return the value.
//    const ScaleFactor calculateBTagSF(const analysis::tools::Jet & jet,const int & op, const int& period = 0);
    const std::vector<ScaleFactor> calculateBTagSF(const analysis::tools::Jet & jet,const int & op);

private:
	std::map<std::string,BTagCalibrationReader> reader_;
//	Method to fill BTagCalibrationReader map
	void LoadPOGCalibrations_(const BTagCalibration& calibration, const std::string& add = "_period0");
//  Method to load correcitons
	void setBTagCalibrationReaders_(const BTagCalibration& calibration, const std::string& add = "_period0");


};

#endif /* MSSMHBB_SRC_BTAGSCALEFACTOR_H_ */
