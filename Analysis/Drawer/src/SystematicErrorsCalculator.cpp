/*
 * SystematicErrorsCalculator.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: shevchen
 */

#ifndef DRAWER_SRC_SYSTEMATICERRORSCALCULATOR_H_
#define DRAWER_SRC_SYSTEMATICERRORSCALCULATOR_H_

#include "TH1.h"


class SystematicErrorsCalculator {
public:
	SystematicErrorsCalculator();
	virtual ~SystematicErrorsCalculator();

	void CalculateSingleSystError(TH1F* central, TH1F *upper, TH1F *lower); // For asymmetric errors
	void CalculateSingleSystError(TH1F* central, TH1F *errorHisto);				// For symmetric errors
	void AddUncorelatedSystErrors(TH1F* central);

protected:

	std::vector<TH1F*> SystHistograms_; //Vector of the SystErr histograms

};




SystematicErrorsCalculator::SystematicErrorsCalculator() {
	// TODO Auto-generated constructor stub

}

SystematicErrorsCalculator::~SystematicErrorsCalculator() {
	// TODO Auto-generated destructor stub
}

void SystematicErrorsCalculator::CalculateSingleSystError(TH1F* central, TH1F* upper, TH1F *lower){

	double error = 0;
	for(int bin = 1; bin <= central->GetNbinsX(); ++bin){
		error = upper->GetBinContent(bin) - lower->GetBinContent(bin);
		error = error / 4.;
		central->SetBinError(bin,error);
//		std::cout<<" up/central "<<upper->GetBinContent(bin)/central->GetBinContent(bin)-1<<" "<<
//				(upper->GetBinContent(bin)-central->GetBinContent(bin))/central->GetBinContent(bin)<<std::endl;
//		std::cout<<"SFb Syst: "<<central->GetBinContent(bin)<<" (centr.) "<<upper->GetBinContent(bin)<<" (up) "<<lower->GetBinContent(bin)<<" (down) "<<std::endl;
//		std::cout<<"Tot: "<<central->GetBinContent(bin)<<" "<<central->GetBinError(bin)<<" "<<central->GetBinError(bin)/central->GetBinContent(bin)<<std::endl;
	}
	SystHistograms_.push_back(central);

}

void SystematicErrorsCalculator::CalculateSingleSystError(TH1F* central, TH1F* errorHisto){

	double error = 0;
	for(int bin = 1; bin <= central->GetNbinsX(); ++bin){
		error = errorHisto->GetBinContent(bin);
		central->SetBinError(bin,error);
	}
	SystHistograms_.push_back(central);

}

void SystematicErrorsCalculator::AddUncorelatedSystErrors(TH1F *central){

	double statError = 0, systError = 0, totError = 0;
	for(int bin = 1; bin <= central->GetNbinsX(); ++bin){
		statError = central->GetBinError(bin);
		systError = 0;
		for(const auto & histo : SystHistograms_){
			systError += histo->GetBinError(bin) * histo->GetBinError(bin);
//			std::cout<<"Syst: "<<histo->GetBinError(bin)<<std::endl;
		}
//		std::cout<<"Total Syst"<<std::sqrt(systError)<<std::endl;
		totError = std::sqrt(systError + statError*statError);
		central->SetBinError(bin,totError);
	}
	SystHistograms_.clear();
}



#endif /* DRAWER_SRC_SYSTEMATICERRORSCALCULATOR_H_ */
