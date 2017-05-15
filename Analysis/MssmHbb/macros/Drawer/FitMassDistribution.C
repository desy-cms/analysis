#include "HttStylesNew.cc"
#include "Analysis/MssmHbb/src/Lumi_Setup.C"

Double_t SignalHighMass(Double_t * x, Double_t * par) {

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

Double_t SignalLowMass(Double_t * x, Double_t * par) {

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




TF1 * FitMass(TH1D * hist,
	      TString histName = "Mbb",
	      int iopt = 2,
	      int rebin = 1) {

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

  TString funcName = histName+"_func";
  TF1 * fitFunc = NULL;
  if (iopt==1) {
    double maxY = 0;
    double peak = mean;
    for (int iB=1; iB<=nBins; ++iB) {
      double y = hist->GetBinContent(iB);
      if (y>maxY) {
	maxY = y;
	peak = hist->GetBinCenter(iB);
      }
    }
    fitFunc = new TF1(funcName,SignalLowMass,xmin,xmax,10);
    fitFunc->SetParameter(0,maxY);
    fitFunc->SetParameter(1,peak);
    fitFunc->SetParameter(2,0.1*peak);
    fitFunc->SetParameter(3,0.2*peak);
    fitFunc->SetParameter(4,0.1*peak);
    fitFunc->SetParameter(5,0.2*peak);
    fitFunc->SetParameter(6,0.5);
    fitFunc->SetParLimits(6,0.1,0.9);
    fitFunc->SetParameter(7,0.5);
    fitFunc->SetParLimits(7,0.1,0.9);
    fitFunc->SetParameter(8,1.2*peak);
    fitFunc->SetParameter(9,peak);
  }
  else {
    fitFunc = new TF1(funcName,SignalHighMass,xmin,xmax,11);
    fitFunc->SetParameter(0,0.3*maximum);
    fitFunc->SetParLimits(0,0.1*maximum,0.9*maximum);
    fitFunc->SetParameter(1,0.7*mean);
    fitFunc->SetParameter(2,0.6*rms);
    fitFunc->SetParameter(3,0.5*maximum);
    fitFunc->SetParLimits(3,0.1*maximum,0.9*maximum);
    fitFunc->SetParameter(4,mean);
    fitFunc->SetParameter(5,0.6*rms);
    fitFunc->SetParameter(6,0.3*maximum);
    fitFunc->SetParLimits(6,0.1*maximum,0.9*maximum);
    fitFunc->SetParameter(7,1.3*mean);
    fitFunc->SetParameter(8,0.6*rms);
    fitFunc->SetParameter(9,1.6*mean);
    fitFunc->SetParameter(10,rms);
  }
  fitFunc->SetLineColor(2);

  TCanvas * canv = MakeCanvas("dummy","",700,700);
  hist->Fit(funcName,"LWER");
  canv->Print("../macros/pictures/SignalPDFs/"+histName+".pdf");
  delete canv;

  double reScale = 1.0/double(rebin);
  if (rebin) {
    if (iopt==1) {
      fitFunc->SetParameter(0,reScale*fitFunc->GetParameter(0));
    }
    else {
      fitFunc->SetParameter(0,reScale*fitFunc->GetParameter(0));
      fitFunc->SetParameter(3,reScale*fitFunc->GetParameter(3));
      fitFunc->SetParameter(6,reScale*fitFunc->GetParameter(6));
    }
  }

  return fitFunc;

}
