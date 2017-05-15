/*
 * signal_fit.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: shevchen
 */
#include "../Drawer/HttStylesNew.cc"
#include "Analysis/MssmHbb/src/Lumi_Setup.C"
#include "../../interface/utilLib.h"

double SimplifiedModel(double *x, double *par);
double SignalLowMass(double *x, double *par);
double Signal3Gaus(double * x, double * par);
double Signal2Gaus(double * x, double * par);
double Signal4Gaus(double * x, double * par);
double RelBreitWigner(double *x, double *par);
double ExpGausExp(double *x, double *par);
double ExpGausLandau(double *x, double *par);
double Signal2GausExpo(double *x, double *par);
double DoubleGausExp(double *x, double *par);
double Bukin(double *x, double *par);


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
	std::vector<Point> signal = {
//								Point(700,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(900,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(1100,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(1300,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(500,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(600,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root"),
//								 Point(200,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root"),
//								 Point(250,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root"),
								 Point(300, cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root"),
//								 Point(350,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root"),
//								 Point(400,cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root")
			};

	TCanvas can;
	for(const auto & p : signal){
		std::string name = "#Phi("+std::to_string(p.mass)+")#rightarrowb#bar{b}";
		FitMass(p.path,"templates/bbH_Mbb_VIS",name,"low mass trigger",3);
	}
//	FitMass(signal.at(0).path);


	return 0;
}

double Bukin(double *x, double *par){
	double xx = x[0];
	double norm = par[0];
	double Xp = par[1];
	double sigp = par[2];
	double rho1 = par[3];
	double rho2 = par[4];
	double xi = par[5];
	double consts = 2*sqrt(2*log(2.));


	double r1=0,r2=0,r3=0,r4=0,r5=0,hp=0;
	double x1 = 0,x2 = 0;
	double fit_result = 0;
	hp=sigp*consts;
	r3=log(2.);
	r4=sqrt(TMath::Power(xi,2)+1);
	r1=xi/r4;

	  if(TMath::Abs(xi) > exp(-6.)){
	       r5=xi/log(r4+xi);
	  }
	     else
	       r5=1;

	     x1 = Xp + (hp / 2) * (r1-1);
	     x2 = Xp + (hp / 2) * (r1+1);

	     //--- Left Side
	     if(xx < x1){
	       r2=rho1*TMath::Power((xx-x1)/(Xp-x1),2)-r3 + 4 * r3 * (xx-x1)/hp * r5 * r4/TMath::Power((r4-xi),2);
	     }
	     //--- Center
	     else if(xx < x2) {
	       if(TMath::Abs(xi) > exp(-6.)) {
	         r2=log(1 + 4 * xi * r4 * (xx-Xp)/hp)/log(1+2*xi*(xi-r4));
	         r2=-r3*(TMath::Power(r2,2));
	       }
	       else{
	         r2=-4*r3*TMath::Power(((xx-Xp)/hp),2);
	       }
	     }
	     //--- Right Side
	     else {
	       r2=rho2*TMath::Power((xx-x2)/(Xp-x2),2)-r3 - 4 * r3 * (xx-x2)/hp * r5 * r4/TMath::Power((r4+xi),2);
	     }



	     if(TMath::Abs(r2) > 100){
	       fit_result = 0;
	     }
	     else{
	       //---- Normalize the result
	       fit_result = exp(r2);
	     }
	   return norm * fit_result;
}

double SimplifiedModel(double *x, double *par){

	  Double_t aL = (x[0]-par[1])/par[2];
	  Double_t bL = (x[0]-par[1])/par[3];
	  Double_t aR = (x[0]-par[1])/par[4];
	  Double_t bR = (x[0]-par[1])/par[5];

	  Double_t cR  = (par[7]-par[1])/par[4];
	  Double_t dR  = (par[7]-par[1])/par[5];

	  double norm_g2 = (1./(par[5]*par[5])*TMath::Exp(-0.5*dR*dR)) / ( 1./(par[5]*par[5])*TMath::Exp(-0.5*dR*dR) - 1./(par[4]*par[4])*TMath::Exp(-0.5*cR*cR) );

	  Double_t gL = par[6]*TMath::Exp(-0.5*aL*aL)+(1-par[6])*TMath::Exp(-0.5*bL*bL);
	  Double_t gR = norm_g2*TMath::Exp(-0.5*aR*aR)+(1-norm_g2)*TMath::Exp(-0.5*bR*bR);

	  Double_t w = norm_g2*TMath::Exp(-0.5*cR*cR)+(1-norm_g2)*TMath::Exp(-0.5*dR*dR);
	  double tail_sigma = - w / ( norm_g2/pow(par[4],4.) * (par[7] - par[1]) * TMath::Exp(-0.5*cR*cR) + (1 - norm_g2) / pow(par[5],4) * (par[7] - par[1]) * TMath::Exp(-0.5*dR*dR));

	  Double_t aexp = (x[0]-par[7])/tail_sigma;

	  Double_t resultL = par[0]*gL;
	  Double_t resultR = par[0]*gR;
	  Double_t norm    = par[0]*w;
	  Double_t exp = norm*TMath::Exp(-aexp);

	  Double_t result = resultL;
	  if (x[0]>par[1])
	    result = resultR;
	  if (x[0]>par[7])
	    result = exp;

	  return result;

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


//	  if(M < par[3]) return pol;
//	  else return bw;
	  return bw;

}

double Signal2GausExpo(double *x, double *par){
	double xx = x[0];
	double norm = par[0];
	double a1 = (xx - par[1])/par[2];
	double a2 = (xx - par[1])/par[3];
	double n1 = par[4];
	double g1 = n1*TMath::Exp(-0.5 * a1* a1);
	double g2 = (1-n1)*TMath::Exp(-0.5 * a1 * a2);

	double shift = par[5];
	double b1 = (shift - par[1])/par[2];
	double b2 = (shift - par[1])/par[3];
	double n2 = n1*TMath::Exp(-0.5 * b1* b1) + (1-n1)*TMath::Exp(-0.5 * b2 * b2);
	double exp = n2 * TMath::Exp(- (xx - shift) / par[6] );

	double result = exp;
	if(xx < shift) result = g1+g2;
	return par[0] * result;
}

double Signal4Gaus(double * x, double * par){
	double xx = x[0];
	double norm = par[0];
	double aL1 = (xx - par[1])/par[2];
	double aL2 = (xx - par[1])/par[3];
	double n1 = par[4];

	double gL = n1*TMath::Exp(-0.5 * aL1* aL1) + (1-n1)*TMath::Exp(-0.5 * aL2 * aL2);

	//Right part;
//	double shift = par[5];
//	double bL1 = (shift - par[1])/par[2];
//	double bL2 = (shift - par[1])/par[3];
//	double nR = n1*TMath::Exp(-0.5 * bL1* bL1) + (1-n1)*TMath::Exp(-0.5 * bL2 * bL2);

	double aR1 = (xx - par[1])/par[5];
	double gR = TMath::Exp(-0.5 * aR1* aR1);

	double aR2 = (xx - par[7])/par[8];
//	double nR2 = nR * TMath::Exp(-0.5 * (par[7] - par[1])/par[6] * (par[7] - par[1])/par[6]);
	double nR2 = TMath::Exp(-0.5 * (par[7] - par[1])/par[5] * (par[7] - par[1])/par[5] );
	double gR2 = nR2 * TMath::Exp(-0.5 * aR2* aR2);



	double result = gR2;
//	if(xx < shift) result = gL;
//	else if (xx >= shift && xx < par[7]) result = gR;
//	else result = gR2;
	if(result < par[1]) result = gL;
	else if ( result >= par[1] && result < par[7] )	result = gR;
	else result = gR2;

	return norm * result;
}

double Signal2Gaus(double * x, double * par) {

  Double_t a1 = (x[0]-par[1])/par[2];
  Double_t a2 = (x[0]-par[1])/par[3];

  Double_t b1 = (par[4]-par[1])/par[2];
  Double_t b2 = (par[4]-par[1])/par[3];

  Double_t aexp = (x[0]-par[4])/par[5];

  Double_t g1 = TMath::Exp(-0.5*a1*a1);
  Double_t g2 = TMath::Exp(-0.5*a2*a2);
  Double_t result = g1 + g2;


  Double_t w1 = TMath::Exp(-0.5*b1*b1);
  Double_t w2 = TMath::Exp(-0.5*b2*b2);
  Double_t norm = w1 + w2;

  Double_t exp = w2*TMath::Exp(-aexp);

  if (x[0]>par[6])
    result = exp;
  else if( x[0] > par[1]) result = g2;
  else result = g1;

  return par[0] * result;

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

double ExpGausExp(double *x, double *par){
	double xbar = par[0];
	double sigma = par[1];
	double kh = par[2];
	double kl = par[3];
	double X = (x[0] - xbar)/sigma;
	double X2 = X * X;

	double resultR = TMath::Exp( kh*kh/2 - kh*X );
	double resultM = TMath::Exp( - 0.5 * X*X);
	double resultL = TMath::Exp( kl*kl/2 + kl*X);
	double result = 0;
	if(X < kl) result = resultL;
	else if ( X >= kl && X < kh) result = resultM;
	else result = resultR;
	result *= par[4];

	return result;

}

double ExpGausLandau(double *x, double *par){
	double exp = TMath::Exp( (x[0] - par[1])/ par[2] );

	double Xgaus = ( x[0] - par[1])/par[3];
	double Xgaus2 = ( x[0] - par[1])/par[6];
	double Xgaus_p5 = ( par[4] - par[1])/par[3];
	double Xgaus2_p5 = ( par[4] - par[1])/par[6];
	double Xland = (x[0] - par[4])/par[5];


	double gaus = par[7] * TMath::Exp(-0.5 * Xgaus * Xgaus ) + (1-par[7])*TMath::Exp(-0.5 * Xgaus2 * Xgaus2);
	double gaus_p5 = (par[7] * TMath::Exp(-0.5 * Xgaus_p5 * Xgaus_p5 ) + (1-par[7])*TMath::Exp(-0.5 * Xgaus2_p5 * Xgaus2_p5))/ TMath::Exp(-0.5);

	double landau = gaus_p5 * TMath::Exp(-0.5 * ( Xland + TMath::Exp(-1. * Xland) ) );


	double result = exp;
	if(x[0] > par[1] && x[0] <= par[4]) result = gaus;
	if( x[0] > par[4]) result = landau;

	result = result * par[0];

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
  xmin = 200;

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
	  fitFunc = new TF1("fitFunc",Signal2Gaus,xmin,800,7);
	  fitFunc->SetParameter(0,maximum);
	  fitFunc->SetParameter(1,mean);
	  fitFunc->SetParameter(2,rms);
	  fitFunc->SetParameter(3,1.2 * rms);
	  fitFunc->SetParameter(4,1.2*mean);
	  fitFunc->SetParameter(5,0.6*rms);
	  fitFunc->SetParameter(6,0.5);
//	  fitFunc->SetParameter(7,1.3*mean);
//	  fitFunc->SetParameter(8,0.6*rms);
//	  fitFunc->SetParameter(9,1.6*mean);
//	  fitFunc->SetParameter(10,rms);
  }
  else if (iopt==3){
	    fitFunc = new TF1("fitFunc",SimplifiedModel,xmin,xmax,8);
	    fitFunc->SetParameter(0,maxY);
	    fitFunc->SetParameter(1,peak);
	    fitFunc->SetParameter(2,0.1*peak);
	    fitFunc->SetParameter(3,0.2*peak);
	    fitFunc->SetParameter(4,0.1*peak);
	    fitFunc->SetParameter(5,0.2*peak);
	    fitFunc->SetParameter(6,0.5);
	    fitFunc->SetParameter(7,1.2*peak);
	    fitFunc->SetParNames("global_norm","mean","sigmaL1","sigmaL2","sigmaR1","sigmaR2","norm_g1","tail_shift","tail_sigma");
    }
  else if (iopt ==4){ //ExpGausExp
	  fitFunc = new TF1("fitFunc",ExpGausExp,xmin,xmax,5);
	  fitFunc->SetParameter(0,0.7*mean);
	  fitFunc->SetParameter(1,0.5 * rms);
	  fitFunc->SetParameter(2,1);
	  fitFunc->SetParameter(3,1);
	  fitFunc->SetParameter(4,0.5*maximum);
  }
  else if(iopt ==5){
	    fitFunc = new TF1("fitFunc",ExpGausLandau,xmin,xmax,8);
	    fitFunc->SetParameter(0,maxY);
	    fitFunc->SetParameter(1,0.5*peak);
	    fitFunc->SetParameter(2,0.7*rms);
	    fitFunc->SetParameter(4,peak);
	    fitFunc->SetParameter(5,1.2*rms);
	    fitFunc->SetParameter(6,rms);
	    fitFunc->SetParameter(7,0.5);
	    fitFunc->SetParLimits(7,0,1);
	    fitFunc->SetParNames("global_norm","exp(mean)","exp(sigma)","gaus(sigma)","landau(mean)","landau(sigma)","gaus2(sigma)","gaus1(frac)");
  }
  else if(iopt == 6){
	  fitFunc = new TF1("fitFunc",Signal2GausExpo,xmin,1100,7);
	  fitFunc->SetParameter(0,50);
	  fitFunc->SetParameter(1,mean);
	  fitFunc->SetParameter(2,0.5*rms);
	  fitFunc->SetParameter(3,1.5*rms);
	  fitFunc->SetParameter(4,0.5);
	  fitFunc->SetParameter(5,1.2*mean);
	  fitFunc->SetParameter(6,0.5*rms);
	  fitFunc->SetParNames("global_norm","peak","Sigma1","Sigma2","Norm g1-g2","shift","Exp_sigma");
  }
  else if(iopt == 7){
	  fitFunc = new TF1("fitFunc",Signal4Gaus,xmin,xmax,9);
//	  fitFunc->SetParameter(0,50);
//	  fitFunc->SetParameter(1,mean);
//	  fitFunc->SetParameter(2,0.5*rms);
//	  fitFunc->SetParameter(3,1.5*rms);
//	  fitFunc->SetParameter(4,0.5);
//	  fitFunc->SetParameter(5,1.2*mean);
//	  fitFunc->SetParameter(6,0.5*rms);
//	  fitFunc->SetParameter(7,1.5*mean);
//	  fitFunc->SetParameter(8,0.5*rms);
	  fitFunc->SetParameter(0,50);
	  fitFunc->SetParameter(1,768);
	  fitFunc->SetParameter(2,218);
	  fitFunc->SetParameter(3,100);
	  fitFunc->SetParameter(4,0.15);
	  fitFunc->SetParameter(5,50);
	  fitFunc->SetParameter(6,1);
	  fitFunc->SetParameter(7,1037.);
	  fitFunc->SetParameter(8,202);
	  fitFunc->SetParNames("global_norm","peak","Sigma1","Sigma2","Norm g1-g2","shift","Exp_sigma");
  }
  else if (iopt == 8){
	  fitFunc = new TF1("fitFunc",Bukin,xmin,xmax,6);
	  fitFunc->SetParameters(2,800,100,-0.08,0.14,-0.4);
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

  LumiSetup lum;
  lum.CMS_lumi(canv,4,33);
  lum.writeExtraText = true;
  lum.extraText = "Simulation";

  canv->Update();
  if (iopt==1)
    canv->Print(("figures/"+legend+"_fit_opt1.pdf").c_str());
  else
    canv->Print(("figures/"+legend+"_fit_opt2.pdf").c_str());

}
