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
#include "TArray.h"
#include "TTree.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"

using namespace std;
using namespace RooFit;

void makeplot_novops()
{
	gStyle->SetOptStat(0);

	TH1::SetDefaultSumw2();

	float chi2BkgOnly_ = 72.2;
        int ndfBkgOnly_ = 67;
	float normChi2BkgOnly_ = chi2BkgOnly_/ndfBkgOnly_;
	double fitRangeMin_ = 240.; //240.;
	double fitRangeMax_ = 1700.; //1700.;

	//TFile *f = new TFile("Workspace_Novopsprod_lowM.root");
	//TFile *f = new TFile("Workspace_Novoeffprod_lowM.root");
	//TFile *f = new TFile("Workspace_CBeffprod_lowM.root");
 	//TFile *f = new TFile("Workspace_Bukin_lowM.root");	
	//TFile *f = new TFile("Workspace_Bukin_highM.root"); 
	//TFile *f = new TFile("Workspace_CBeffprod_highM.root");
	//TFile *f = new TFile("Workspace_Novoeffprod_highM.root");
	//TFile *f = new TFile("Workspace_Novosibirsk_highM.root");
	TFile *f = new TFile("../test/Optimization_18_08_2016/2016_extnovoeffprod_240to1700_20GeV/workspace/FitContainer_workspace.root");

	//TTree *t = (TTree*)f->Get("fit_b");
	//t->SetBranchAddress("chi2", &chi2BkgOnly_);
	//t->SetBranchAddress("normChi2", &normChi2BkgOnly_);
	//t->SetBranchAddress("ndf", &ndfBkgOnly_);

  	RooWorkspace *w = (RooWorkspace*)f->Get("workspace");

	RooRealVar *mbb  = w->var("mbb");
	RooAbsPdf *pdf 	 = w->pdf("background");
	RooAbsData *data = w->data("data_container");
 
	TH1* datahist = data->createHistogram("datahist", *mbb, Binning(73,240.,1700.));
	TH1* pdfhist  = pdf->createHistogram("pdfhist", *mbb, Binning(73,240.,1700.));

	//TH1* datahist = data->createHistogram("datahist", *mbb, Binning(106,350.,3000.));
	//TH1* pdfhist  = pdf->createHistogram("pdfhist", *mbb, Binning(106,350.,3000.));

	pdfhist->Scale(datahist->Integral()/pdfhist->Integral());

	for(int i = 0; i < datahist->GetNbinsX(); ++i)
	{
		if(i > 21 && i < 32){
		//if(i > 11 && i < 19){ 
		 	datahist->SetBinContent(i,0.);
			datahist->SetBinError(i,0.);
		}
	}

	datahist->SetMarkerStyle(8);	
	//datahist->SetMarkerSize(0.9);
	datahist->SetMarkerSize(0.8);
	datahist->SetMarkerColor(kBlack);
	datahist->SetLineWidth(1);
	datahist->SetLineColor(kBlack);
	datahist->SetName("");
	datahist->SetTitle("");

        //pdfhist->Scale(datahist->Integral()/pdfhist->Integral());
	//pdfhist->Smooth();
	pdfhist->SetLineColor(kRed);
	pdfhist->SetLineWidth(3);
	pdfhist->SetName("");
	pdfhist->SetTitle("");
	
  	TCanvas canvas;
  	canvas.SetCanvasSize(500,500);
  	canvas.SetLeftMargin(0.15);
  	canvas.SetRightMargin(0.05);
	datahist->GetYaxis()->SetTitle("Events/ ( 20 GeV )");
	//datahist->GetYaxis()->SetTitle("Events/ ( 25 GeV )");
	datahist->GetYaxis()->SetTitleOffset(1.8);
	datahist->GetXaxis()->SetTitle("M_{12} (GeV)");
	datahist->Draw("E0");
	pdfhist->Draw("same AC");
	
  	TLatex latex;
  	latex.SetTextFont(43);
  	latex.SetTextSize(17);
  	latex.SetTextAlign(11);
  	latex.DrawLatexNDC(canvas.GetLeftMargin()-0.004, 1.02-canvas.GetTopMargin(),
        //             "CMS Preliminary #sqrt{s} = 13 TeV, L = 2.69 fb^{-1}");
			"CMS Preliminary #sqrt{s} = 13 TeV, L = 12.89 fb^{-1}");
  	latex.SetTextSize(15);
  	latex.SetTextAlign(33);
  	latex.SetTextColor(kBlue+2);
	std::string chi2str(Form("%.1f/%d = %.1f", chi2BkgOnly_, ndfBkgOnly_, normChi2BkgOnly_));
  	latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.98-canvas.GetTopMargin(),
                     (std::string("#chi^{2}_{RooFit}/ndf = ")+chi2str).c_str());
  	latex.SetTextColor(kGreen+2);
	double prob = TMath::Prob(chi2BkgOnly_,ndfBkgOnly_);
	std::string probstr(Form("%.2f", prob));
  	latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.93-canvas.GetTopMargin(),
                       (std::string("p-value = ")+probstr).c_str());
  	latex.SetTextColor(kOrange+2);
	std::string minstr(Form("%.0f", fitRangeMin_));
  	std::string maxstr(Form("%.0f", fitRangeMax_)); 
 	latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.88-canvas.GetTopMargin(),
                       (minstr+std::string(" < M_{12} < ")+maxstr).c_str());
 
	canvas.SaveAs("BBnB_extnovoeffprod_lowM_linear.pdf");
  	canvas.SetLogy();
  	datahist->GetYaxis()->SetRangeUser(0.1, datahist->GetMaximum()*5);
  	canvas.Modified();
  	canvas.Update();
  	canvas.SaveAs("BBnB_extnovoeffprod_lowM_log.pdf");


}
