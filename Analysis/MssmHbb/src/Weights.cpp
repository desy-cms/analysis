/*
 * Weights.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: shevchen
 */

#include "TH1.h"
#include "TH2.h"
#include <iostream>

#include "cmath"

#include "Analysis/MssmHbb/interface/Weights.h"

double Weights::FactorizationPtWeight(const double & pt1, const double & pt2){

	if(pt1 <= 0 || pt2<=0){
		std::cerr<<"Error: in Weights::FactorizationPtWeight wrong pt (<0)"<<std::endl;
		exit(0);
	}


	double weight = 0;

	/*
	// NUMBERS FOR 74X
	if(lowM_){
		weight = 1./(1.+exp( -8.87968e-02 * (pt1 - 1.03879e+02) )) * 1./ (1.+exp( -1.88620e-01 * (pt1 - 1.02891e+02) ));
		weight *= 1./(1.+exp( -8.87968e-02 * (pt2 - 1.03879e+02) )) * 1./ (1.+exp( -1.88620e-01 * (pt2 - 1.02891e+02) ));
	}
	else {
		weight = 1./(1.+exp( -1.60938e-01 * (pt1 - 1.64206e+02) )) * 1./ (1.+exp( -1.53883e-01 * (pt1 + 3.50936e+01) ));
		weight *= 1./(1.+exp( -1.60938e-01 * (pt2 - 1.64206e+02) )) * 1./ (1.+exp( -1.53883e-01 * (pt2 + 3.50936e+01) ));
	}
	*/
	if(lowM_){
		weight = 1./(1.+exp( -1.13097e-01 * (pt1 - 1.05075e+02) )) * 1./ (1.+exp( -1.13097e-01 * (pt1 - 1.05075e+02) ));
		weight *= 1./(1.+exp( -1.13097e-01 * (pt2 - 1.05075e+02) )) * 1./ (1.+exp( -1.13097e-01 * (pt2 - 1.05075e+02) ));
	}
	else {
		weight = 1./(1.+exp( -1.46385e-01 * (pt1 - 1.66919e+02) )) * 1./ (1.+exp( -5.99105e-02 * (pt1 + 1.29321e+02) ));
		weight *= 1./(1.+exp( -1.46385e-01 * (pt2 - 1.66919e+02) )) * 1./ (1.+exp( -5.99105e-02 * (pt2 + 1.29321e+02) ));
	}

	return weight;
}

double Weights::TwoDPtWeight(TH2 *histo, const double & pt1, const double & pt2){

	if(histo==0){
		std::cerr<<"Error: in Weights::TwoDPtWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	if(pt1 <= 0 || pt2<=0){
		std::cerr<<"Error: in Weights::TwoDPtWeight wrong pt (<0)"<<std::endl;
		exit(0);
	}

	double weight = 0;

	if (pt1 > 500) {
		if (pt2 > 500){
			weight = histo->Interpolate(499,499);
		}
		else weight = histo->Interpolate(pt2,499);
	}
	else weight = histo->Interpolate(pt2,pt1);

	return weight;

}

double Weights::dEtaWeight(const double & dEta){
	double weight = 0;

	if(lowM_){
		weight = 1./(1.+exp( 1.41132e+02 * (dEta - 1.59991e+00) )) * 1./ (1.+exp( -2.06352e-02 * (dEta + 2.27738e+02) ));
	}
	else weight = 1;

	return weight;
}

double Weights::BTagWeight(TH1 * low_eta, TH1 * middle_eta, TH1 * high_eta, const double & pt, const double &eta){

	if(low_eta == 0 || middle_eta == 0 || high_eta == 0){
		std::cerr<<"Error: in Weights::BTagWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	if(pt <= 0){
		std::cerr<<"Error: in Weights::BTagWeight wrong pt (<0)"<<std::endl;
		exit(0);
	}

	double weight = 0;

	if(pt <1000) {
		if(eta < 0.9) weight = low_eta -> Interpolate(pt);
		else if (eta > 0.9 &&eta < 1.4) weight = middle_eta -> Interpolate(pt);
		else weight = high_eta -> Interpolate(pt);

	}
	else {
		if(eta < 0.9) weight = low_eta -> Interpolate(995.);
		else if (eta > 0.9 &&eta < 1.4) weight = middle_eta -> Interpolate(995.);
		else weight = high_eta -> Interpolate(995.);
	}

	return weight;
}

double Weights::BTagWeight(TH2 *pt_eta, const double &pt, const double & eta){

	if(pt_eta == 0){
		std::cerr<<"Error: in Weights::BTagWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	double weight = 0;

	if(pt <1000) {
		weight = pt_eta->Interpolate(pt,eta);
	}
	else {
		weight = pt_eta->Interpolate(995.,eta);
	}

	return weight;
}

double Weights::PileUpWeight(TH1 *data, TH1 *mc, const double & NTruePileUpMC){
	if(data==0 || mc ==0){
		std::cerr<<"Error: in Weights::PileUpWeight empty weight histogram"<<std::endl;
		exit(1);
	}
	double weight = 0;

	auto hData = (TH1*) data->Clone("hData");
	auto hMc   = (TH1*) mc->Clone("hMc");

	//Scale to 1

	hData->Scale( (1/(hData->Integral()) ) );
	hMc->Scale( (1/(hMc->Integral()) ) );

	double N_data = hData->GetBinContent(hData->FindBin(NTruePileUpMC));
	double N_mc = hMc->GetBinContent(hMc->FindBin(NTruePileUpMC));
	weight = N_data/N_mc;

//	std::cout<<" True = "<<NTruePileUpMC<<std::endl;
//	std::cout<<" Data: "<<hData->FindBin(NTruePileUpMC)<<" "<<hData->GetBinContent(hData->FindBin(NTruePileUpMC))<<std::endl;
//	std::cout<<" MC:   "<<hMc->FindBin(NTruePileUpMC)<<" "<<hMc->GetBinContent(hMc->FindBin(NTruePileUpMC))<<std::endl;
//	std::cout<<" weight = "<<weight<<" "<<N_data<<" "<<N_mc<<std::endl;

	return weight;
}

double Weights::PileUpWeight(TH1 *data, const double & N_mc, const double & NTruePileUpMC){
	if(data==0){
		std::cerr<<"Error: in Weights::PileUpWeight empty weight histogram"<<std::endl;
		exit(1);
	}
	double weight = 0;
	double N_data = data->GetBinContent(data->FindBin(NTruePileUpMC));
	weight = N_data/N_mc;

	std::cout<<"PileUp = "<<N_data<<" Nmc = "<<N_mc<<" "<<NTruePileUpMC<<std::endl;

	return weight;
}

/*
double Weights::PileUpWeight(TH1 *data, TH1 *mc, const double & NTruePileUpMC){

	if(data==0 || mc ==0){
		std::cerr<<"Error: in Weights::PileUpWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	NVertex_mc_ = NTruePileUpMC;

	double weight = 0;

	PileUp pu;
	pu.setDataHisto(data);
	pu.setMCHisto(mc);
	weight = pu.getPUweight(NVertex_mc_);

	return weight;
}

double Weights::PileUpWeight(TH1 *data, TH1 *mc){

	if(data==0 || mc ==0){
		std::cerr<<"Error: in Weights::PileUpWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	double weight = 0;

	PileUp pu;
	pu.setDataHisto(data);
	pu.setMCHisto(mc);
	weight = pu.getPUweight(NVertex_mc_);

	return weight;
}
*/

double Weights::HtWeight(TH1 * histo, const double &Ht){

	if(histo==0){
		std::cerr<<"Error: in Weights::HtWeight empty weight histogram"<<std::endl;
		exit(1);
	}

	if(Ht == 0){
		std::cerr<<"Error: in Weights::HtWeight Ht = 0"<<std::endl;
		exit(0);
	}

	double weight = 0;

	if(Ht < 2600){
		weight = histo->Interpolate(Ht);
	}
	else {
		weight = histo->Interpolate(2550);
	}

	return weight;
}

//.............Functions...................

