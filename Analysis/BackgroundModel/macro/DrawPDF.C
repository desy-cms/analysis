#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <memory>
#include <fstream>
#include <ostream>
#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TTree.h"
#include "TPad.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooRandom.h"
#include "RooEffProd.h"
#include "RooNovosibirsk.h"

#ifndef __CINT__
//#include "RooMyNovosibirsk.h"
#include "RooExtendNovosibirsk.h"
#endif

using namespace std;
using namespace RooFit;

void DrawPDF()
{ 

  RooRealVar mbb("mbb", "mbb", 200., 1200.);
  RooRealVar peak("peak", "peak", 265.9, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 52.67, 5.0, 325.0, "GeV");
  RooRealVar tail1("tail1", "tail1", -0.90, -1.0, 1.0);
  RooRealVar tail2("tail2", "tail2", -0.72, -1.0, 1.0);
  //RooRealVar par4("par4", "par4", -0.00063946, -1., 1);
  //RooRealVar turnon("turnon", "turnon", 223.027, 150., 650., "GeV");
  //RooRealVar slope("slope", "slope", 0.0159765, 0., 0.1);

  //RooRealVar par7("par7", "par7", +0.0002, -1., 1.);
  //RooRealVar peak1("peak1", "peak1", 200., 50.0, 500.0, "GeV");
  //RooRealVar peak2("peak2", "peak2", 300., 50.0, 500.0, "GeV");

  mbb.setBins(50);

  //RooRealVar peak("peak", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  //RooRealVar width("width", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  //RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
 
//#ifdef __CINT__
  //gROOT->ProcessLineSync(".x RooExtendNovosibirskV2.cc+") ;
  //gROOT->ProcessLineSync(".x RooExtendNovosibirsk.cc+") ;
  //gROOT->ProcessLineSync(".x RooMyNovosibirsk.cc+") ;
  //gROOT->ProcessLineSync(".x RooNovosibirsk.cc+") ;
//#endif
 
  //RooExtendNovosibirsk extnovo("extnovo", "mynovosibirsk", mbb, peak, width, tail, par4);
  //RooExtendNovosibirskV2 extnovo1("extnovo1", "extnovosibirsk1", mbb, peak, width, tail, par4, par5);
  //RooExtendNovosibirskV2 extnovo2("extnovo2", "extnovosibirsk2", mbb, peak, width, tail, par4, par6);

  //RooExtendNovosibirsk extnovo3("extnovo3", "extnovosibirsk3", mbb, peak, width, tail, par6);
  //RooExtendNovosibirsk extnovo4("extnovo4", "extnovosibirsk4", mbb, peak, width, tail, par7);
  RooNovosibirsk extnovo1("extnovo1", "extnovosibirsk1", mbb, peak, width, tail1);
  RooNovosibirsk extnovo2("extnovo2", "extnovosibirsk2", mbb, peak, width, tail2);
  //RooFormulaVar phasespace("phasespace", "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)", RooArgList(mbb));
  //RooFormulaVar phasespace("phasespace",("0.5*(TMath::Erf(slope*(" + mbb + "-turnon)) + 1)").c_str(), RooArgSet(mbb, slope, turnon));
  //RooFormulaVar phasespace("phasespace","0.5*(TMath::Erf(0.0159765*(mbb - 223.027)) + 1)", RooArgList(mbb));
  //RooEffProd novoeffprod("background", "novoeffprod", extnovo, phasespace);

  RooPlot* xframe = mbb.frame(Title("Novosibirsk PDF Subrange2")) ;
  //extnovo1.plotOn(xframe, RooFit::Name("extnovo"), LineColor(kBlue)) ;
  //novoeffprod.plotOn(xframe, RooFit::Name("background"), LineColor(kRed)) ;
  extnovo1.plotOn(xframe, RooFit::Name("extnovo1"), LineColor(kRed)) ;
  extnovo2.plotOn(xframe, RooFit::Name("extnovo2"), LineColor(kGreen+2)) ;
  //extnovo3.plotOn(xframe,LineColor(kRed)) ;
  //extnovo4.plotOn(xframe,LineColor(kGreen+2)) ;

  xframe->Draw(); 
 
  TLegend leg(0.60,0.60,0.86,0.87);
  leg.SetFillColor(0);
  leg.AddEntry("extnovo1","265.9,52.67,-0.90","L");
  leg.AddEntry("extnovo2","265.9,52.67,-0.72","L");
  leg.Draw("same"); 
 //novopsprod.Draw();


/*
  gStyle->SetOptStat("");
  gStyle->SetLegendBorderSize(0); 

  TH1::SetDefaultSumw2();
 
  TFile *f1 = new TFile("../test/Prescale_v4/extnovoeffprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v4/berneffprod9par_200to650_10GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v4/dijetv3logprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v4/dijetv3effprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v4/dijetv4logprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root") ;
  TFile *f2 = new TFile("../../MssmHbb/bin/ReReco_Signal_PDF/ReReco_signal_M-400/workspace/FitContainer_workspace.root") ;
 
  //TFile *f1 = new TFile("../test/Prescale_v3/novosibirsk_350to1190_20GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v3/dijetv2_350to1190_20GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f1 = new TFile("../test/Prescale_v3/novosibirsk_500to1700_25GeV_G4/workspace/FitContainer_workspace.root") ;
  //TFile *f2 = new TFile("../test/Prescale_v3/dijetv2_500to1700_25GeV_G4/workspace/FitContainer_workspace.root") ; 

  RooWorkspace* w1 = (RooWorkspace*) f1->Get("workspace") ;
  RooWorkspace* w2 = (RooWorkspace*) f2->Get("workspace") ;

  RooRealVar* mbb = w1->var("mbb") ;
  //RooAbsData* data = w1->data("data_obs") ;
  RooAbsData* data = w1->data("data_container") ;
  RooAbsPdf* extnovoeffprod = w1->pdf("background") ;
  //RooAbsPdf* berneffprod = w2->pdf("background") ;
  RooAbsPdf* berneffprod = w2->pdf("signal") ;
 
  //RooPlot* xframe = mbb->frame(Title("Turn-on x Extended Novosibirsk vs Turn-on x Bernstein9")) ;
  //data->plotOn(xframe,RooFit::Name("data_curve")) ;
  //extnovoeffprod->plotOn(xframe,RooFit::Name("extnovoeffprod"),LineColor(kBlue)) ;
  //berneffprod->plotOn(xframe,RooFit::Name("berneffprod"),LineColor(kRed)) ;  
  //xframe->Draw();

  TH1* h_novo = extnovoeffprod->createHistogram("Novo", *mbb, Binning(10000,200.,650.));
  TH1* h_bern = berneffprod->createHistogram("Bern", *mbb, Binning(10000,200.,650.));
  h_novo->Scale(1/h_novo->Integral());
  h_bern->Scale(1/h_bern->Integral());

  //TH1* h_novo = extnovoeffprod->createHistogram("Novo", *mbb, Binning(10000,350.,1190.));
  //TH1* h_bern = berneffprod->createHistogram("Bern", *mbb, Binning(10000,350.,1190.));

  //TH1* h_novo = extnovoeffprod->createHistogram("Novo", *mbb, Binning(10000,500.,1700.));
  //TH1* h_bern = berneffprod->createHistogram("Bern", *mbb, Binning(10000,500.,1700.));
 
  h_novo->SetLineColor(kBlue);
  h_bern->SetLineColor(kRed);
  TH1* h_ratio = (TH1*)h_novo->Clone("ratio");

  TCanvas canvas;
  canvas.SetCanvasSize(500,500);
  TPad* pad1;
  pad1 = new TPad("pad1","",0,0.1,1,1);
  pad1->SetBottomMargin(0.2);
  pad1->SetRightMargin(0.05); // The ratio plot below inherits the right and left margins settings here!
  pad1->SetLeftMargin(0.16);
  pad1->Draw();
  pad1->cd();
  pad1->SetLogy(); 
  //h_novo->SetTitle("Turn-on x extended Novosibirsk vs Turn-on x Bernstein");
  //h_novo->SetTitle("Turn-on x extended Novosibirsk vs Logistic x Dijet");
  //h_novo->SetTitle("Turn-on x extended Novosibirsk vs Turn-on x Dijet");
  h_novo->SetTitle("Turn-on x extended Novosibirsk vs Signal 400");
  //h_novo->SetTitle("Novosibirsk vs Dijet SR3");
  h_novo->GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
  h_novo->GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
  h_novo->GetYaxis()->SetTitleSize(0.041);
  h_novo->GetYaxis()->SetTitleOffset(1.20);
  h_novo->GetYaxis()->SetLabelSize(0.04);
  h_novo->Draw("hist");
  h_bern->Draw("same hist");
  
  canvas.cd();

  TPad *pad2 = new TPad("pad2","",0,0.0,1,0.25);
  pad2->SetTopMargin(1);
  pad2->SetBottomMargin(0.33);
  pad2->SetLeftMargin(pad1->GetLeftMargin());
  pad2->SetRightMargin(pad1->GetRightMargin());
  pad2->SetGridy();
  pad2->Draw();
  pad2->cd();

  h_ratio->SetTitle("");
  h_ratio->Divide(h_bern);
  h_ratio->SetMarkerStyle(8);
  h_ratio->SetMarkerSize(0.1);
  h_ratio->GetXaxis()->SetTitleSize(0.15);
  h_ratio->GetXaxis()->SetTitleOffset(0.85);
  h_ratio->GetXaxis()->SetLabelSize(0.12);
  h_ratio->GetXaxis()->SetLabelOffset(0.008);
  //h_ratio->SetYTitle("#frac{Novo}{CB}");
  //h_ratio->SetYTitle("#frac{Novo}{Bern}");
  //h_ratio->SetYTitle("#frac{Novo}{Dijet}");
  h_ratio->SetYTitle("#frac{Background}{Signal}");
  h_ratio->GetYaxis()->CenterTitle(kTRUE);
  h_ratio->GetYaxis()->SetTitleSize(0.15);
  h_ratio->GetYaxis()->SetTitleOffset(0.3);
  h_ratio->GetYaxis()->SetNdivisions(3,5,0);
  h_ratio->GetYaxis()->SetLabelSize(0.12);
  h_ratio->GetYaxis()->SetLabelOffset(0.011);
  h_ratio->Draw("p");
  h_ratio->SetMaximum(1.1);
  h_ratio->SetMinimum(0.9);

  //TString pdfname;
  //pdfname = h_->GetName();
  //canvas.SaveAs("NovovsBern_Ratio.pdf");
  //canvas.SaveAs("NovovsCB_R1_Ratio.pdf");
  //canvas.SaveAs("NovovsDijetEff_SR1_Ratio.pdf");
  canvas.SaveAs("BackgroundSignal_400.pdf");
*/

}




