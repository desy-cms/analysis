#include <string>

#include "TRandom3.h"
#include "TSystem.h"
#include "TMath.h"
#include "TH1D.h"
#include "TFile.h"
#include "RooFit.h"

#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

using namespace std;
namespace ab = analysis::backgroundmodel;

void prescale(TH1& h, const double& val = 9);
void reweight(TH1& h);
double weight_function(const double& x);

int main(int argc, char **argv) {
	/*
	 * Script to fit bbnb data (weighted and not)
	 */
	HbbStyle style;
	style.set(PRIVATE);

	const string cmsswBase = gSystem->Getenv("CMSSW_BASE");
	const string selection = "bbx_ReReco_35673fb_lowM_Run2016.root";
	string output_name = "../test/test_bg_fit";
	bool rew = false;
	TFile f( (cmsswBase + "/src/Analysis/MssmHbb/output/" + selection).c_str(),"read");
	string histo_name = "general/diJet_m";

	//Preferences for the fit
	int rebin = 1;
	const int verbosity = 1;
	double min = 200;
	double max = 650;
	string model = "extnovoeffprod";

	TH1D *h = (TH1D*) f.Get(histo_name.c_str());
	h->Rebin(rebin);
	prescale(*h);
	if(rew) {
//		reweight(*h);
		output_name += "_Rew";
	}

	//Setup fitter
	ab::FitContainer fitter(h,output_name,"background");
	fitter.fitRangeMax(max);
	fitter.fitRangeMin(min);
	fitter.verbosity(verbosity - 1);
	fitter.setModel(ab::FitContainer::Type::background,model);

	//Set Initial values
	fitter.getWorkspace().var("slope_novoeff")->setVal(0.0151088); // 0.0159765
	fitter.getWorkspace().var("turnon_novoeff")->setVal(227.188); // 223.027
	fitter.getWorkspace().var("peak")->setVal(268.093);
	fitter.getWorkspace().var("width")->setVal(63.3651);
	fitter.getWorkspace().var("tail")->setVal(-0.448627);
	fitter.getWorkspace().var("par4")->setVal(-0.00063946);

	fitter.getWorkspace().var("tail")->setRange(-5,5);
	fitter.getWorkspace().var("turnon_novoeff")->setRange(200,250);

	if(rew) {
		fitter.getWorkspace().var("slope_novoeff")->setConstant();
		fitter.getWorkspace().var("turnon_novoeff")->setConstant();
	}
//	fitter.getWorkspace().var("peak")->setConstant();
//	fitter.getWorkspace().var("width")->setConstant();
//	fitter.getWorkspace().var("par4")->setConstant();
//	fitter.getWorkspace().var("tail")->setConstant();


	//Fit
	std::unique_ptr<RooFitResult> fit = fitter.backgroundOnlyFit(model,1);
}

void prescale(TH1& h, const double& val){
//	double prob = 1. / val;
	TRandom3 r;
	for(int i = 1;i <= h.GetNbinsX(); ++i){
		int Ntot = h.GetBinContent(i);
//		int Npresc = Ntot / val;
//		double binomial_prob = TMath::Factorial(Ntot) / (TMath::Factorial(Npresc) * Tmath::Factorial(Ntot - Npresc)) * pow(prob,Npresc) * pow(1-prob,Ntot - Npresc);
		double binomial_prob = 0;
		for(int i = 1; i <= Ntot; ++i){
			if(r.Rndm() < 1./9.) binomial_prob+=1;
//			binomial_prob += TMath::BinomialI(prob,Ntot,Npresc);
//			binomial_prob += TMath::BinomialI(prob,9,1);
//			binomial_prob += TMath::Factorial(9) / (TMath::Factorial(1) * TMath::Factorial(9 - 1)) * pow(prob,1) * pow(1-prob,9 - 1);
		}
		int Nfinale = binomial_prob;
		h.SetBinContent(i,Nfinale);
		h.SetBinError(i,sqrt(Nfinale));
//		h.SetBinContent(i,(int) h.GetBinContent(i)/ val);
//		h.SetBinError(i,h.GetBinError(i) / sqrt(val));
	}
}

void reweight(TH1& h){
	/*
	 * Method to perform bin-by-bin reweighting
	 */
	for(int i = 1;i <= h.GetNbinsX(); ++i){
		double weight = weight_function(h.GetXaxis()->GetBinCenter(i));
		h.SetBinContent(i,weight * h.GetBinContent(i));
		h.SetBinError(i,h.GetBinError(i) * weight);
	}
}

double weight_function(const double& x){
	/*
	 * weight function accroding to the ratio bbb to bbnb
	 */
	double weight = 1;
	if(x < 400) weight= 1./(1.+exp( -0.0540249 * (x - 188.762) ));

	return weight;
}
