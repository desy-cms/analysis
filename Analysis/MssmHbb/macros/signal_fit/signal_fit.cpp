/*
 * signal_fit.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: shevchen
 */
#include "../Drawer/HttStylesNew.cc"
#include "../Drawer/CMS_lumi.C"
#include "../../interface/utilLib.h"

double SimplifiedModel(double *x, double *par);
double SignalLowMass(double *x, double *par);
double Signal3Gaus(double * x, double * par);
double Signal2Gaus(double * x, double * par);
double RelBreitWigner(double *x, double *par);


void FitMass(const std::string & fileName = "MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8",
		const std::string & histName = "bbH_Mbb",
		const std::string & legend = "#Phi(900)#rightarrowb#bar{b}",
		const std::string & header = "low mass trigger",
        int iopt = 1,
        int rebin = 2);

struct Point{
	Point(const int & mass, const std::string& path) : mass(mass), path(path) {};
	int mass;
	std::string path;
};

int signal_fit(){

	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));
	std::vector<Point> signal = {Point(700,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(900,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(1100,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(1300,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(500,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(600,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
								 Point(200,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root"),
								 Point(250,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root"),
								 Point(300, cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root"),
								 Point(350,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root"),
								 Point(400,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root")};

	TCanvas can;
	for(const auto & p : signal){
		std::string name = "#Phi("+std::to_string(p.mass)+")#rightarrowb#bar{b}";
		FitMass(p.path,"bbH_Mbb",name);
	}
//	FitMass(signal.at(0).path);


	return 0;
}

double SimplifiedModel(double *x, double *par){
	//pol2 + gaus + exp
	double x1 = x[0];
	double pol = par[0]*(par[1]*x1*x1 + par[2]*x1 + 1);

	double x2 = x1 - par[3];
	double norm_gaus = par[0]*(par[1]*par[3]*par[3] + par[2]*par[3] + 1);
	double gaus = norm_gaus * TMath::Exp(-0.5 * (x2 - par[4]) / par[5] * (x2 - par[4]) / par[5]);

	double x3 = x1 - par[6];
	double norm_exp = TMath::Exp(-0.5 * (par[6] - par[4]) / par[5] * (par[6] - par[4]) / par[5]);
	double exp = norm_exp * TMath::Exp(-x3*par[8]*x3*par[8]);

	pol = pol*par[7];
	gaus = gaus * par[7];
	exp = exp*par[7];

	if(x1 < par[3]) return pol;
	else if(x1 < par[6]) return gaus;
	else return exp;

  return 0;
}

double RelBreitWigner(double *x, double *par){

	double pol = par[0]*(par[1]*x[0]*x[0] + par[2]*x[0] + 1);
	double norm = par[0]*(par[1]*par[3]*par[3] + par[2]*par[3] + 1);

	  double M  = x[0]-par[3];
	  double M2 = M*M;
	  double G = par[4];
	  double G2 = G*G;
	  double E = par[5];
	  double E2= E*E;
	  double gamma = std::sqrt(M2 * (M2 + G2));
	  double k = (2 * std::sqrt(2) * M * G * gamma) / (TMath::Pi() * std::sqrt(M2 + gamma));
	  double bw = norm * k / ( std::pow(E2-M2,2.)+ M2*G2 );


	  if(M < par[3]) return pol;
	  else return bw;

}

double Signal2Gaus(double * x, double * par) {

  Double_t a1 = (x[0]-par[1])/par[2];
  Double_t a2 = (x[0]-par[4])/par[5];

  Double_t b1 = (par[6]-par[1])/par[2];
  Double_t b2 = (par[6]-par[4])/par[5];

  Double_t aexp = (x[0]-par[6])/par[7];

  Double_t g1 = par[0]*TMath::Exp(-0.5*a1*a1);
  Double_t g2 = par[3]*TMath::Exp(-0.5*a2*a2);
  Double_t result = g1 + g2;


  Double_t w1 = par[0]*TMath::Exp(-0.5*b1*b1);
  Double_t w2 = par[3]*TMath::Exp(-0.5*b2*b2);
  Double_t norm = w1 + w2;

  Double_t exp = norm*TMath::Exp(-aexp);

  if (x[0]>par[6])
    result = exp;

  return result;

}

double Signal3Gaus(double * x, double * par) {

  Double_t a1 = (x[0]-par[1])/par[2];
  Double_t a2 = (x[0]-par[4])/par[5];
  Double_t a3 = (x[0]-par[7])/par[8];

  Double_t b1 = (par[9]-par[1])/par[2];
  Double_t b2 = (par[9]-par[4])/par[5];
  Double_t b3 = (par[9]-par[7])/par[8];

  Double_t aexp = (x[0]-par[9])/par[10];

  Double_t g1 = par[0]*TMath::Exp(-0.5*a1*a1);
  Double_t g2 = par[3]*TMath::Exp(-0.5*a2*a2);
  Double_t g3 = par[6]*TMath::Exp(-0.5*a3*a3);
  Double_t result = g1 + g2 + g3;


  Double_t w1 = par[0]*TMath::Exp(-0.5*b1*b1);
  Double_t w2 = par[3]*TMath::Exp(-0.5*b2*b2);
  Double_t w3 = par[6]*TMath::Exp(-0.5*b3*b3);
  Double_t norm = w1 + w2 + w3;

  Double_t exp = norm*TMath::Exp(-aexp);

  if (x[0]>par[9])
    result = exp;

  return result;

}

double SignalLowMass(Double_t * x, Double_t * par) {

  Double_t aL = (x[0]-par[1])/par[2];
  Double_t bL = (x[0]-par[1])/par[3];
  Double_t aR = (x[0]-par[1])/par[4];
  Double_t bR = (x[0]-par[1])/par[5];

  Double_t aexp = (x[0]-par[8])/par[9];

  Double_t cR  = (par[8]-par[1])/par[4];
  Double_t dR  = (par[8]-par[1])/par[5];

  Double_t gL = par[6]*TMath::Exp(-0.5*aL*aL)+(1-par[6])*TMath::Exp(-0.5*bL*bL);
  Double_t gR = par[7]*TMath::Exp(-0.5*aR*aR)+(1-par[7])*TMath::Exp(-0.5*bR*bR);

  Double_t w = par[7]*TMath::Exp(-0.5*cR*cR)+(1-par[7])*TMath::Exp(-0.5*dR*dR);

  Double_t resultL = par[0]*gL;
  Double_t resultR = par[0]*gR;
  Double_t norm    = par[0]*w;
  Double_t exp = norm*TMath::Exp(-aexp);

  Double_t result = resultL;
  if (x[0]>par[1])
    result = resultR;
  if (x[0]>par[8])
    result = exp;

  return result;

}

void FitMass(const std::string & fileName,
		const std::string & histName,
		const std::string & legend,
		const std::string & header,
             int iopt,
             int rebin) {

  SetStyle();

  TFile * file = new TFile((fileName).c_str());
  TH1D * hist = (TH1D*)file->Get(histName.c_str());

  if (rebin>=2)
    hist->Rebin(rebin);


  InitData(hist);
  hist->SetLineColor(1);
  hist->SetTitle("");
  hist->GetXaxis()->SetTitle("M_{12} [GeV]");
  hist->GetYaxis()->SetTitle("a.u");
  hist->GetXaxis()->SetTitleOffset(1.);
  hist->GetYaxis()->SetTitleOffset(1.3);
  hist->GetXaxis()->SetTitleSize(0.06);
  hist->GetYaxis()->SetTitleSize(0.06);

  double mean = hist->GetMean();
  double rms  = hist->GetRMS();
  double maximum = hist->GetMaximum();
  int nBins = hist->GetNbinsX();
  double xmin = hist->GetBinLowEdge(1);
  double xmax = hist->GetBinLowEdge(nBins+1);
  double maxY = 0;
  double peak = mean;
  for (int iB=1; iB<=nBins; ++iB) {
    double y = hist->GetBinContent(iB);
    if (y>maxY) {
      maxY = y;
      peak = hist->GetBinCenter(iB);
    }
  }


  TF1 * fitFunc = nullptr;
  if (iopt==1) {
    fitFunc = new TF1("fitFunc",SignalLowMass,xmin,xmax,10);
    fitFunc->SetParameter(0,maxY);
    fitFunc->SetParameter(1,peak);
    fitFunc->SetParameter(2,0.1*peak);
    fitFunc->SetParameter(3,0.2*peak);
    fitFunc->SetParameter(4,0.1*peak);
    fitFunc->SetParameter(5,0.2*peak);
    fitFunc->SetParameter(6,0.5);
    fitFunc->SetParameter(7,0.5);
    fitFunc->SetParameter(8,1.2*peak);
    fitFunc->SetParameter(9,peak);
    fitFunc->SetParNames("global_norm","mean","sigmaL1","sigmaL2","sigmaR1","sigmaR2","norm_g1","norm_g2","tail_shift","tail_sigma");
  }
  else if (iopt==2) {
	  fitFunc = new TF1("fitFunc",Signal3Gaus,xmin,xmax,8);
	  fitFunc->SetParameter(0,0.3*maximum);
	  fitFunc->SetParameter(1,0.7*mean);
	  fitFunc->SetParameter(2,0.6*rms);
	  fitFunc->SetParameter(3,0.5*maximum);
	  fitFunc->SetParameter(4,mean);
	  fitFunc->SetParameter(5,0.6*rms);
	  fitFunc->SetParameter(6,0.3*maximum);
	  fitFunc->SetParameter(7,1.3*mean);
	  fitFunc->SetParameter(8,0.6*rms);
	  fitFunc->SetParameter(9,1.6*mean);
	  fitFunc->SetParameter(10,rms);
  }
  else if (iopt==3){
	  fitFunc = new TF1("fitFunc",SimplifiedModel,xmin,xmax,8);
		fitFunc->SetParameter(0,0.1);
		fitFunc->SetParameter(1,3.5);
		fitFunc->SetParameter(2,-0.02);
		fitFunc->SetParameter(3,550);
		fitFunc->SetParameter(4,0.5*peak);
		fitFunc->SetParameter(5,0.5*rms);
		fitFunc->SetParameter(6,700);
		fitFunc->SetParameter(7,1);
		fitFunc->SetParameter(8,1);
//	  fitFunc->SetParameter(0,0.3*maximum);
//	  fitFunc->SetParameter(4,0.7*mean);
//	  fitFunc->SetParameter(5,0.6*rms);
//	  fitFunc->SetParameter(3,3.5);
//	  fitFunc->SetParameter(1,-0.02);
//	  fitFunc->SetParameter(2,0.03);
//	  fitFunc->SetParameter(6,600);
//	  fitFunc->SetParameter(7,1);
    }


  TCanvas * canv = MakeCanvas(("canv"+legend).c_str(),"",600,600);

  hist->Fit("fitFunc","WLER");
  double chi2_ndf = fitFunc->GetChisquare()/fitFunc->GetNDF();
  double p_val    = fitFunc->GetProb();
  TLegend * leg = new TLegend(0.62,0.5,0.84,0.7);
  SetLegendStyle(leg);
  leg->SetHeader(header.c_str());
  leg->SetTextSize(0.033);
  leg->AddEntry(hist,legend.c_str(),"lp");
  leg->AddEntry(fitFunc,("#splitline{Fit: #chi^{2}/ndf = " + to_string_with_precision<double>(chi2_ndf,2) + ",}{p = " + to_string_with_precision<double>(p_val,2) + "}").c_str(),"l");
  leg->Draw();

  writeExtraText = true;
  extraText = "Simulation";
  CMS_lumi(canv,4,33);

  canv->Update();
  if (iopt==1)
    canv->Print(("figures/"+legend+"_fit_opt1.pdf").c_str());
  else
    canv->Print(("figures/"+legend+"_fit_opt2.pdf").c_str());

}
