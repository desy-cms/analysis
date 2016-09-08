#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

#include "TSystem.h"

void renormalise(TH1*);

double Novosibirsk(double *x, double* par) {
	  double m0 = par[0];
	  double sigma = par[1];
	  double tau = par[2];
	  // compute
	  double Lambda = sinh(tau*sqrt(log(4.))) / (sigma*tau*sqrt(log(4.)));
	  double val = 0;
	  if ((1 + Lambda*tau*(x[0]-m0))>0) {
	    double theLog = log(1 + Lambda*tau*(x[0]-m0));
	    val = exp(-0.5*(theLog*theLog /(tau*tau) + tau*tau));
	  }
	  return par[3] * val;
}

double PSNovosibirsk(double *x, double* par) {
	  double p0 = 1.32610e+00;
	  double p1 = 2.28699e+03;
	  double p2 = 3.33051e-02;
	  double p3 = p0 - 1;
	  double PSFac = p0 / (1. + p1 * exp(-p2 * x[0])) - p3;
	  if (PSFac <0) PSFac = 0;
	  return (PSFac * Novosibirsk(x,par));
}



int fit_bg(){

	gStyle->SetOptFit(1111);

	std::string name;
	std::string full_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/";
	std::map<std::string,TFile*> file;

	name = full_path + "output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file["data_bbx"] = new TFile(name.c_str(),"READ");

	name = full_path + "output/bbx_lowM_QCD_notRew.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file["mc_bbx"] = new TFile(name.c_str(),"READ");

	name = full_path + "output/DataMC_3b_NoHTrew_lowM_QCD.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file["mc_bbb"] = new TFile(name.c_str(),"READ");

	std::map<std::string, TH1*> h;
	std::map<std::string, TF1*> fit;
	std::map<std::string, TLegend*> leg;
	std::map<std::string, TCanvas*> can;

	bool data;

	for(const auto & f : file){
		std::string hName;
		if(f.first.find("data") == std::string::npos){
			data = false;
//			hName = "diJet_b_m/diJet_b_m";
			hName = "general/diJet_m";
		}
		else{
			data = true;
			hName = "QCD_Mbb";
		}

		can[f.first] = new TCanvas( (f.first + "can").c_str(),f.first.c_str(),800,600 );
		gPad->SetLogy();

		h[f.first] = (TH1D*) (f.second)->Get(hName.c_str());
		renormalise(h[f.first]);
		double scale = 1./h[f.first]->Integral();
		h[f.first]->Scale(scale);

//		h[f.first]->Scale(1./h[f.first]->Integral());
//		hTemp[f.first]->Scale(1./hTemp[f.first]->Integral())

		h[f.first]->SetTitle(f.first.c_str());
		h[f.first]->GetXaxis()->SetRangeUser(240.,1100);

		fit[f.first] = new TF1((f.first + "fit").c_str(),PSNovosibirsk,240.,1100,4);
		fit[f.first]->SetParNames("peak","width","tail","norm");
		fit[f.first]->SetParLimits(0,50,500);
		fit[f.first]->SetParameters(255.,61.8,-0.63,0.1);
//		fit[f.first]->SetParameters(255.,61.8,-0.63,1);
		h[f.first]->Fit(fit[f.first],"+R");

	}

	return 0;
}

void renormalise(TH1 * h){
	for(int i=1; i<= h->GetNbinsX();++i){
		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
		h->SetBinError(i,h->GetBinError(i)/h->GetBinWidth(i));
	}
}
