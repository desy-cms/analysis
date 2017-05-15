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

	void CalculateSingleSystError(TH1 *upper, TH1 *lower); // For asymmetric errors
	void CalculateSingleSystError(TH1 *errorHisto);				// For symmetric errors
	void AddUncorelatedSystErrors(TH1* central);

protected:

	std::vector<std::vector<double>> SystErr_;

};




SystematicErrorsCalculator::SystematicErrorsCalculator() {
	// TODO Auto-generated constructor stub

}

SystematicErrorsCalculator::~SystematicErrorsCalculator() {
	// TODO Auto-generated destructor stub
}

void SystematicErrorsCalculator::CalculateSingleSystError(TH1* upper, TH1 *lower){

	std::vector <double> errors;
	double error = 0;
	for(int bin = 1; bin <= upper->GetNbinsX(); ++bin){
		error = upper->GetBinContent(bin) - lower->GetBinContent(bin);
		error = error / 4.;
		errors.push_back(error);
//		std::cout<<" up/central "<<upper->GetBinContent(bin)/central->GetBinContent(bin)-1<<" "<<
//				(upper->GetBinContent(bin)-central->GetBinContent(bin))/central->GetBinContent(bin)<<std::endl;
//		std::cout<<"SFb Syst: "<<central->GetBinContent(bin)<<" (centr.) "<<upper->GetBinContent(bin)<<" (up) "<<lower->GetBinContent(bin)<<" (down) "<<std::endl;
//		std::cout<<"Tot: "<<central->GetBinContent(bin)<<" "<<central->GetBinError(bin)<<" "<<central->GetBinError(bin)/central->GetBinContent(bin)<<std::endl;
	}
	SystErr_.push_back(errors);

}

void SystematicErrorsCalculator::CalculateSingleSystError(TH1* errorHisto){

	std::vector <double> errors;
	double error = 0;
	for(int bin = 1; bin <= errorHisto->GetNbinsX(); ++bin){
		error = errorHisto->GetBinContent(bin);
		errors.push_back(error);
	}
	SystErr_.push_back(errors);

}

void SystematicErrorsCalculator::AddUncorelatedSystErrors(TH1 *central){

	double statError = 0, systError = 0, totError = 0;
	for(int bin = 1; bin <= central->GetNbinsX(); ++bin){
		statError = central->GetBinError(bin);
		systError = 0;
		for(const auto & error : SystErr_){
			systError += error.at(bin-1) * error.at(bin-1);
		}
//		std::cout<<"Total Syst = "<<std::sqrt(systError)<<" Stat = "<<statError<<std::endl;
		totError = std::sqrt(systError + statError*statError);
		central->SetBinError(bin,totError);
	}
//	std::cout<<"\n"<<std::endl;

	SystErr_.clear();
}



#endif /* DRAWER_SRC_SYSTEMATICERRORSCALCULATOR_H_ */
