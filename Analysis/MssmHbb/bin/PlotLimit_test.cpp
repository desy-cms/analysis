/*
 * PlotLimit_test.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/PlotLimit.h"
#include "Analysis/MssmHbb/interface/Limit.h"

int main(){

	PlotLimit* plot = new PlotLimit("../datacards/20161013/binned/Hbb.limits",true);
	plot->CalculateSigmaBRLimits();

//	std::vector<Limit> parametric = {Limit(200,1180.5),Limit(250,743.27),Limit(300,53.62),Limit(350,13.69),Limit(400,18.69),Limit(500,12.81),Limit(600,5.33),Limit(700,2.8),Limit(900,1.23),Limit(1100,0.9),Limit(1300,0.85)};
	std::vector<Limit> parametric = {Limit(300,42.95),Limit(350,24.19),Limit(400,15.91),Limit(500,7.09),Limit(600,3.83),Limit(700,2.59),Limit(900,1.59),Limit(1100,1.23),Limit(1300,1.14)};
	std::vector<Limit> eightTeV;
	std::vector<Limit> eightTeV_mhmodp_mu_p200;
	//8 TeV results:
	std::vector<int> mass = {200,300,350,400,500,600,700,900};
	std::vector<double> GxBR_8TeV = {28.2,6.1,3.9,2.7,1.7,1.3,1.1,1.0};
	std::vector<double> XTanBeta_mhmodp_mu_p200 = {19.2,24.8,30,36.5,53.5};
	int counter = 0, counter2 = 0;
	for(const auto& m : mass){
		Limit l(m,GxBR_8TeV.at(counter));
		eightTeV.push_back(l);

		if(m <= 500){
			Limit tanbeta_8Tev(m,XTanBeta_mhmodp_mu_p200.at(counter2));
			eightTeV_mhmodp_mu_p200.push_back(tanbeta_8Tev);
			++counter2;
		}
		++counter;
	}

//	plot->PlotSigmaBRLimits(eightTeV);
//	plot->PlotSigmaBRLimits("parametric_vs_templates");
	plot->PlotSigmaBRLimits(parametric);
//

//	plot->CalculateTanBetaLimits("mhmod+, #mu=+200","../macros/signal/mhmodp_mu200_13TeV.root");
//	plot->PlotTanBetaLimit("mhmod+, #mu=+200");
//	plot->PlotTanBetaLimit(eightTeV_mhmodp_mu_p200,"mhmod+, #mu=+200");

	return 0;

}

