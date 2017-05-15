#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TF1.h"
#include "TMath.h"

Double_t fitf(Double_t *v, Double_t *par)
{
  Double_t arg = 0;
  //if (par[2] != 0) arg = (v[0] - par[1])/par[2];
  
  // Double_t fitval = 1. / (1. + TMath::Exp(-par[0]*v[0]));
  Double_t fitval = par[0] / (1. + par[1] * TMath::Exp(-par[2]*v[0])) - par[3];
//  if (fitval < 0) fitval =0;
  return fitval;
}

void fitpsfac() {
  TFile* f;
  f = new TFile("psfacDATA_dEta1p55.root");
  TH1F* h = (TH1F*) f->Get("h_obj_M12ips1_Ratio");
  if (h == NULL) {
    std::cout << "Histogram not found" << std::endl;
    return;
  }

  // perform the fit
  float mLow = 200.;
  float mHigh = 500.;
  TF1 *func = new TF1("fit",fitf,mLow,mHigh,4);
  func->SetParameter(0,1.326);
  func->SetParameter(1,2.287e+03);
  func->SetParameter(2,-3.331e-02 );
  func->SetParameter(3,1.326e-1);
  func->SetParName(0,"Amplitude");
  func->SetParName(1,"efac");
  func->SetParName(2,"efac2");
  func->SetParName(3,"vOffset");
  func->SetRange(mLow,mHigh);
  h->Fit("fit","R","",mLow,mHigh);
  
  // plot and print
  h->SetStats(kFALSE);
  h->GetXaxis()->SetRangeUser(100.,500);//  SetRange(1,95);
  h->GetXaxis()->SetTitle("M_{12}");
  h->SetMarkerStyle(20);
  h->Draw("");
  gPad->Print("fitpsfac.png");
}
