/*
 * Weights.h
 *
 *  Created on: Feb 25, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_SRC_WEIGHTS_H_
#define MSSMHBB_SRC_WEIGHTS_H_

#include <iostream>

#include "Analysis/MssmHbb/interface/PileUp.h"

class Weights {

public:
	Weights(const bool & selection): lowM_(selection) {};
	virtual ~Weights() {};

	// Factorized Pt trigger efficiency weight
	double FactorizationPtWeight(const double & pt1, const double &pt2);

	// 2D Pt trigger Efficiency weight
	double TwoDPtWeight(TH2F *histo, const double &pt1, const double &pt2);

	// dEta trigger efficiency weight
	double dEtaWeight(const double & dEta);

	// Online BTag trigger efficiency
	double BTagWeight(TH1F * low_eta, TH1F * middle_eta, TH1F * high_eta, const double & pt, const double & eta);
	double BTagWeight(TH2F * pt_eta, const double & pt, const double & eta);

	// PileUp weight
	double PileUpWeight(TH1F * data, TH1F * mc, const double &NTruePileUpMC);
//	double PileUpWeight(TH1F * data, TH1F * mc);																	// Could be used after the first call

	// Ht weight
	double HtWeight(TH1F * histo, const double & Ht);

	// Luminosity normalisation
	double LumiWeight(const double & data_lumi, const double & mc_lumi);

private:
	const bool lowM_;
	double NVertex_mc_;

};

inline double Weights::LumiWeight(const double & data_lumi, const double & mc_lumi){
	if(mc_lumi ==0 ){
		std::cerr<<"Error in Weights::LumiWeight - 0 MCLuminosity were provided. Interrupt."<<std::endl;
		exit(0);
	}
	double weight = data_lumi/mc_lumi;
	return weight;
}

#endif /* MSSMHBB_SRC_WEIGHTS_H_ */
