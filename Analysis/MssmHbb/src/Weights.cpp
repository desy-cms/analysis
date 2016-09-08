/*
 * Weights.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: shevchen
 */

#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include "TMath.h"

#include "cmath"

#include "Analysis/MssmHbb/interface/Weights.h"

double sigmoid(const double &x, double *par){
	return 1./(1.+exp( -par[0] * (x - par[1]) ));
}

double func_erf(const double &x, double *par){
	return 1 / 2. * (1 + par[0]*TMath::Erf((x-par[1])/(sqrt(2)*par[2])));
}

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

double Weights::triggerCorrectionFunction(const double & pt, const double & eta){

	if(pt <= 0 || std::abs(eta)>100){
		std::cerr<<"Error: in Weights::triggerCorrectionFunction wrong pt (<0) or |eta|>100"<<std::endl;
		exit(0);
	}

	double par_data[15];
	double par_mc[15];
	double data;
	double mc;
	//Low M case
	if(lowM_){
		if(std::abs(eta) >= 0 && std::abs(eta) <=0.6){
			par_data[0] = 0.01793; par_data[1] = -152.4; par_data[2] = 0.119; par_data[3] = 105;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 105.1; par_mc[1] = -285.6; par_mc[2] = 0.1196; par_mc[3] = 106.1; par_mc[4] = 0.9999; par_mc[5] = -79.09; par_mc[6] = 86.02;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else if(std::abs(eta) >0.6 && std::abs(eta) <=1.7){
			par_data[0] = 0.09786; par_data[1] = 105.2; par_data[2] = 0.4661; par_data[3] = 101.5;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 185.8; par_mc[1] = -919.2; par_mc[2] = 0.1182; par_mc[3] = 108.; par_mc[4] = 0.9999; par_mc[5] = -66.07; par_mc[6] = 84.45;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else if(std::abs(eta) > 1.7 && std::abs(eta) < 2.2){
			par_data[0] = 0.01771; par_data[1] = 111.6; par_data[2] = 0.04307; par_data[3] = 51.66;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 64.45; par_mc[1] = -122; par_mc[2] = 0.1707; par_mc[3] = 110.3; par_mc[4] = 0.9988; par_mc[5] = -17.53; par_mc[6] = 68.63;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else{
			std::cerr<<"Error: WRONG ETA in Weights::triggerCorrectionFunction"<<std::endl;
			exit(-1);
		}
	}
	//High Mass
	else{
		if(std::abs(eta) >= 0 && std::abs(eta) <=0.6){
			par_data[0] = 0.008951; par_data[1] = -427.9; par_data[2] = 0.1461; par_data[3] = 158.3;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 99.51; par_mc[1] = -385.; par_mc[2] = 0.1891; par_mc[3] = 159.1; par_mc[4] = 0.9998; par_mc[5] = -115.6; par_mc[6] = 119.1;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else if(std::abs(eta) >0.6 && std::abs(eta) <=1.7){
			par_data[0] = 0.01579; par_data[1] = -117.1; par_data[2] = 0.1415; par_data[3] = 161.5;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 122.2; par_mc[1] = -324.3; par_mc[2] = 0.1563; par_mc[3] = 161.7; par_mc[4] = 0.9998; par_mc[5] = -140.8; par_mc[6] = 128.6;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else if(std::abs(eta) > 1.7 && std::abs(eta) < 2.2){
			par_data[0] = 0.2366; par_data[1] = 86.47; par_data[2] = 0.1485; par_data[3] = 170.2;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
			par_mc[0] = 40.48; par_mc[1] = -103.7; par_mc[2] = 0.03285; par_mc[3] = 43.11; par_mc[4] = 0.9988; par_mc[5] = 167.4; par_mc[6] = 8.899;
			mc   = sigmoid(pt,par_mc)*sigmoid(pt,par_mc+2)*func_erf(pt,par_mc+4);
		}
		else{
			std::cerr<<"Error: WRONG ETA in Weights::triggerCorrectionFunction"<<std::endl;
			exit(-1);
		}
	}

	return data/mc;
}

double Weights::PtTriggerEfficiency(const double &pt, const double &eta){
	if(pt <= 0 || std::abs(eta)>100){
		std::cerr<<"Error: in Weights::triggerCorrectionFunction wrong pt (<0) or |eta|>100"<<std::endl;
		exit(0);
	}

	double par_data[15];
	double data;
	//Low M case
	if(lowM_){
		if(std::abs(eta) >= 0 && std::abs(eta) <=0.6){
			par_data[0] = 0.01793; par_data[1] = -152.4; par_data[2] = 0.119; par_data[3] = 105;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else if(std::abs(eta) >0.6 && std::abs(eta) <=1.7){
			par_data[0] = 0.09786; par_data[1] = 105.2; par_data[2] = 0.4661; par_data[3] = 101.5;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else if(std::abs(eta) > 1.7 && std::abs(eta) < 2.2){
			par_data[0] = 0.01771; par_data[1] = 111.6; par_data[2] = 0.04307; par_data[3] = 51.66;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else{
			std::cerr<<"Error: WRONG ETA in Weights::triggerCorrectionFunction"<<std::endl;
			exit(-1);
		}
	}
	//High Mass
	else{
		if(std::abs(eta) >= 0 && std::abs(eta) <=0.6){
			par_data[0] = 0.008951; par_data[1] = -427.9; par_data[2] = 0.1461; par_data[3] = 158.3;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else if(std::abs(eta) >0.6 && std::abs(eta) <=1.7){
			par_data[0] = 0.01579; par_data[1] = -117.1; par_data[2] = 0.1415; par_data[3] = 161.5;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else if(std::abs(eta) > 1.7 && std::abs(eta) < 2.2){
			par_data[0] = 0.2366; par_data[1] = 86.47; par_data[2] = 0.1485; par_data[3] = 170.2;
			data = sigmoid(pt,par_data)*sigmoid(pt,par_data+2);
		}
		else{
			std::cerr<<"Error: WRONG ETA in Weights::triggerCorrectionFunction"<<std::endl;
			exit(-1);
		}
	}

	return data;
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
   
   delete hData;
   delete hMc;
   
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

