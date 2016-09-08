#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

#include "TSystem.h"

TH1D * bbH_Mbb;

void renormalise(TH1*);

double Novosibirsk(double *x, double* par) {
	  double m0 = par[0];
	  double sigma = par[1];
	  double tau = par[2];
	  // compute
	  double Lambda = sinh(tau*sqrt(log(4.))) / (sigma*tau*sqrt(log(4.)));
	  double val = 0;

	  if(std::abs(tau) < 1.e-07){
		  val = exp(-0.5 * std::pow(( x[0] - m0)/sigma ,2.));
	  }
	  else if ((1 + Lambda*tau*(x[0]-m0))>0) {
	    double theLog = log(1 + Lambda*tau*(x[0]-m0));
	    val = exp(-0.5*(theLog*theLog /(tau*tau) + tau*tau));
	  }
	  else val = 0;
	  return par[3] * val;
}

double CrystalBall(double *x, double *par){

	//parameters according to: https://en.wikipedia.org/wiki/Crystal_Ball_function
	double xx = x[0]; double a = par[0]; double n = par[1];
	double xbar = par[2]; double sigma = par[3];

	double A = std::pow(n/(std::abs(a)),n) * std::exp( -1. * std::pow(std::abs(a),2)/2.);
	double B = n/std::abs(a) - std::abs(a);
	double C = n/std::abs(a) * 1./(n-1) * std::exp( -1. * std::pow(std::abs(a),2)/2.);
	double D = std::sqrt(TMath::Pi() /2. ) * (1 + TMath::Erf(std::abs(a)/std::sqrt(2)));
	double N = 1./ (sigma*(C+D) );

	double res = 0;
	if( (xx - xbar)/sigma > -a ){
		res = std::exp( -0.5* std::pow((xx - xbar)/sigma,2) );
	}
	else {
		res = A * std::pow((B - (xx - xbar)/sigma),-n);
	}

	return res * par[4];

	//parameters according to RooFit::RooCBShape
//	double m = x[0];
//	double m0 = par[0];
//	double sigma = par[1];
//	double alpha = par[2];
//	double n = par[3];
//	double res = 0;
//	Double_t t = (m-m0)/sigma;
//	if (alpha < 0) t = -t;
//	Double_t absAlpha = std::abs((Double_t)alpha);
//	if (t >= -absAlpha) {
//		res = exp(-0.5*t*t);
//	}
//	else {
//		Double_t a =  TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
//		Double_t b= n/absAlpha - absAlpha;
//		res = a/TMath::Power(b - t, n);
//	}
//
//	return (par[4] * res);
}

double Signal(double *x, double *par){

	if(!bbH_Mbb){
		std::cerr<<"No signal histogram were defined"<<std::endl;
		exit(-1);
	}

	int bin = bbH_Mbb->FindBin(x[0]);
	double val = bbH_Mbb->GetBinContent(bin);
	double mu = par[0];
	return (val * par[0]);
}

double PSNovosibirsk(double *x, double* par) {
	  double p0 = 1.32610e+00;
	  double p1 = 2.28699e+03;
	  double p2 = 3.33051e-02;
	  double p3 = p0 - 1;
	  double PSFac = p0 / (1. + p1 * exp(-p2 * x[0])) - p3;
	  if (PSFac <0) PSFac = 0;
//	  return (PSFac * Novosibirsk(x,par));
	  return (PSFac * Novosibirsk(x,par));
}

double complicateBg(double *x, double *par){
	return (PSNovosibirsk(x,par) * (par[5]*x[0]*x[0]*x[0] + par[6]*x[0]*x[0] + par[7]*x[0] + par[8]));
}

double S_PSNovosibirsk(double *x, double *par){
	return (PSNovosibirsk(x,par) + Signal(x,par+4));
}

double S_CrystalBall(double * x, double * par){
	return (CrystalBall(x,par) + Signal(x,par+5));
}

int pro_fit(){

	gStyle->SetOptFit(1111);
	ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");

	std::string name;
	std::string full_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/";
	std::map<std::string,TFile*> file;

	RatioPlots ratio(PRIVATE);

	bool addSignal = false;

	name = full_path + "output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file["signal_350"] = new TFile(name.c_str(),"READ");

	name = full_path + "output/bbx_lowM_Run2015.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file["data_bbx"] = new TFile(name.c_str(),"READ");

	//Get signal histogram:
	bbH_Mbb = (TH1D*) file["signal_350"]->Get("distributions/diJet_m");
	if(!bbH_Mbb){
		std::cerr<<"No signal histo was found"<<std::endl;
		exit(-1);
	}

	std::string benchmark = full_path + "macros/signal/mhmodp_mu200_13TeV.root";
	mssm_xs_tools my(benchmark.c_str(),true,0);

	double tanBeta = 30;
	double mA = 350;
    double sigmaBBA = my.bbHSantander_A(mA,tanBeta);
    double sigmaBBH = my.bbHSantander_H(mA,tanBeta);

    double BrAbb = my.br_Abb(mA,tanBeta);
    double BrHbb = my.br_Hbb(mA,tanBeta);

    double totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;

    //Scale bbH to xsection, for tanBeta
    bbH_Mbb->Scale(totXSec);
//    renormalise(bbH_Mbb);

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
	std::map<std::string, TH1*> hTemp;
	std::map<std::string, TH1*> hRatio;
	std::map<std::string, TF1*> fitBg;
	std::map<std::string, TF1*> fitSBg;
	std::map<std::string, TLegend*> leg;
	std::map<std::string, TCanvas*> can;
	std::map<std::string, TF1*> fitPol;

	bool data;

	for(const auto & f : file){

		if(f.first == "signal_350") continue;


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
		hName = "general/diJet_m";

		can[f.first] = new TCanvas( (f.first + "can").c_str(),f.first.c_str(),800,600 );
		gPad->SetLogy();

		h[f.first] = (TH1D*) (f.second)->Get(hName.c_str());

		h[f.first]->SetStats(kTRUE);
		h[f.first]->SetMarkerStyle(20);
//		renormalise(h[f.first]);
		if(addSignal){
//			hTemp[f.first] = (TH1D*) h[f.first]->Clone( (f.first + "_clone").c_str() );
//			if(!data) h[f.first]->Add(bbH_Mbb);
		}
//		double scale = 1./h[f.first]->Integral();
//		h[f.first]->Scale(scale);
//		hTemp[f.first]->Scale(scale);

//		if(addSignal && !data){
//			hRatio[f.first] = (TH1D*) ratio.DrawRatio(h[f.first],hTemp[f.first],can[f.first],leg[f.first],nullptr);
//			hRatio[f.first]->GetYaxis()->SetRangeUser(0.999,1.004);
//		}

		h[f.first]->SetTitle(f.first.c_str());
		h[f.first]->GetXaxis()->SetRangeUser(0.,1100);

		std::cout<<"Fit to : "<<f.first<<std::endl;

		fitBg[f.first] = new TF1((f.first + "fitBg").c_str(),PSNovosibirsk,240.,1100,4);
		fitBg[f.first]->SetParNames("peak","width","tail","norm");
		fitBg[f.first]->SetParameters(255.,61.8,-0.63,h[f.first]->GetMaximum()*1.5);
//		fitBg[f.first]->SetParameters(280.,80.,100.,100.,h[f.first]->GetMaximum()*1.5);
		fitBg[f.first]->SetLineColor(2);
		h[f.first]->Fit(fitBg[f.first],"+RLL");

		fitSBg[f.first] = new TF1((f.first + "fitSBg").c_str(),S_PSNovosibirsk,240.,1100,5);
		fitSBg[f.first]->SetParNames("peak","width","tail","norm","r");
		fitSBg[f.first]->SetParameters(fitBg[f.first]->GetParameters());
		fitSBg[f.first]->SetParameter(4,0);
		fitSBg[f.first]->SetParLimits(4,-50.,50.);
		fitSBg[f.first]->SetLineColor(6);
		fitSBg[f.first]->SetLineStyle(9);
		h[f.first]->Fit(fitSBg[f.first],"+RLL");
		std::cout<<fitSBg[f.first]->GetProb()<<std::endl;

		fitPol[f.first] = new TF1((f.first + "fitPol").c_str(),"pol15 * erf",240.,1100);
//		fitPol[f.first]->SetParameters(1000,1.,1.8,-1,1,1,1);
//		fitBg[f.first]->SetParameters(280.,80.,100.,100.,h[f.first]->GetMaximum()*1.5);
		fitPol[f.first]->SetLineColor(4);
		h[f.first]->Fit(fitPol[f.first],"+R");



	}


/*
	for(int i = 1; i < h["data_bbx"]->GetNbinsX(); ++ i){
		std::cout<<"X: "<<bbH_Mbb->GetBinCenter(i)<<" "<<h["data_bbx"]->GetBinCenter(i)<<" "<<" Signal: "<<bbH_Mbb->GetBinContent(i)<<" "<<h["data_bbx"]->GetBinContent(i)<<std::endl;
	}
	std::cout<<"Signal integral: "<<bbH_Mbb->Integral()<<std::endl;
/*
	//TEST
	can["data_bbx"]->cd();
	TF1 *draw_fit = new TF1("test_fit",PSNovosibirsk,0.,1100,5);
	draw_fit->SetParameters(fit["data_bbx"]->GetParameters());
	draw_fit->SetParameter(4,50);
	draw_fit->SetLineColor(6);
	draw_fit->Draw("same");

	TF1 *draw_fit_nov = new TF1("test_fit",PSNovosibirsk,0.,1100,5);
	draw_fit_nov->SetParameters(fit["data_bbx"]->GetParameters());
	draw_fit_nov->SetParameter(4,0);
	draw_fit_nov->SetLineColor(1);
	draw_fit_nov->Draw("same");

	/**/
	return 0;
}

void renormalise(TH1 * h){
	for(int i=1; i<= h->GetNbinsX();++i){
		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
		h->SetBinError(i,h->GetBinError(i)/h->GetBinWidth(i));
	}
}
