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

using namespace std;
using namespace RooFit;
 
//const double n_obs = 1276034.; 
const double n_obs = 10261.;
const int toy_max = 1;

//const double n_1pb[3] = {12.25564,21.15929,25.50018}; // for 300,400 and 500 GeV signals
//const double sig_upper[3] = {14.31,7.78,3.98};
const double n_ref[4] = {88.,88.,82.,51.};	//number of events from 0.5*sig_upper

void Bias_study()
{
	// Initial values set here
        double mass     = 300.;
	int strenght    = 10;
	double reso     = mass*0.1;
	double n_gen;
	if(mass == 300) n_gen = n_ref[0];	
	if(mass == 350) n_gen = n_ref[1]; 
	if(mass == 400) n_gen = n_ref[2];
	if(mass == 500) n_gen = n_ref[3]; 
	double n_sgn = strenght*n_gen;

	//std::cout<< "mass = " << mass << std::endl;
	//std::cout<< "strenght = " << strenght << std::endl;
	//std::cout<< "reso = " << reso << std::endl;
	//std::cout<< "n_sgn = " << n_sgn << std::endl;

	// Initial results values (fill to TTree)
	double nb_toy,ns_toy,n_toy,ns_fit,ns_err,nb_fit,nb_err,chi_2,bias;

	RooRandom::randomGenerator()->SetSeed(0);
 
	// Nominal PDFs 
	TFile *f_novo = new TFile("workspace_novops.root");
	RooWorkspace *w_novo = (RooWorkspace*)f_novo->Get("workspace");
	// Alternative PDFs
	TFile *f_novoeff = new TFile("workspace_novoeff.root");
  	RooWorkspace *w_novoeff = (RooWorkspace*)f_novoeff->Get("workspace");
	TFile *f_cbeff = new TFile("workspace_cbeff.root");
  	RooWorkspace *w_cbeff = (RooWorkspace*)f_cbeff->Get("workspace");
	TFile *f_bukin = new TFile("workspace_bukin.root");
  	RooWorkspace *w_bukin = (RooWorkspace*)f_bukin->Get("workspace");

	RooRealVar *mbb =  w_novo->var("mbb");
	//mbb->setRange(240,1690);
	//mbb->setBins(29);

  	RooAbsPdf *pdf_bkg_fit = w_novo->pdf("background");
	//RooAbsPdf *pdf_bkg_alt = w_novo->pdf("background");
  	//RooAbsPdf *pdf_bkg_alt = w_novoeff->pdf("background");
  	RooAbsPdf *pdf_bkg_alt = w_cbeff->pdf("background");
  	//RooAbsPdf *pdf_bkg_alt = w_bukin->pdf("background");

	int npar = 5;
	
	// Signal PDF
 	RooRealVar mean("gausmean","Mean of Gaussian", mass);
  	RooRealVar sigma("gaussigma","Width of Gaussian", reso); 
 	RooGaussian pdf_sgn("gaus","gaus(mbb,mean,sigma)", *mbb, mean, sigma);
	// Extended signal PDF for toy
	RooRealVar sgn_norm("sgn_norm","", n_sgn);
	RooExtendPdf pdf_sgn_ext("pdf_sgn_ext","", pdf_sgn, sgn_norm); 

	// Extended background PDF for toy
	RooRealVar bkg_norm("bkg_norm","", n_obs);
	RooExtendPdf pdf_bkg_alt_ext("pdf_bkg_alt_ext","", *pdf_bkg_alt, bkg_norm);

	// N_s and N_b in S+B Fit
	RooRealVar n_s("n_s","", 0.);
	n_s.setConstant(0);	//turn off to be constant (freely floating)
	RooRealVar n_b("n_b","", bkg_norm.getVal());
	n_b.setConstant(0);	//turn off to be constant (freely floating)
	
	// Preparing S+B PDF to fit toy
	RooAddPdf pdf_fit_ext("pdf_fit_ext","", RooArgList(pdf_sgn, *pdf_bkg_fit), RooArgList(n_s,n_b));
	RooDataSet *data_toy;
	RooDataSet *data_toy_sgn;
	RooFitResult *res_fit;
	RooPlot* xframe = mbb->frame(Title("Toy-Turn-onxCB+Gaus Fit-PSxNovosibirsk+Gaus #mu=10")) ;

	// Preparing S+B toys unbinned dataset
	data_toy = pdf_bkg_alt_ext.generate(*mbb, Extended());
	nb_toy = data_toy->sumEntries();
	data_toy_sgn = pdf_sgn_ext.generate(*mbb, Extended());
	ns_toy = data_toy_sgn->sumEntries();
 
	RooCategory index("index","index");
	index.defineType("signal") ;
	index.defineType("background") ;
	RooDataSet data_toy_sb("data_toy_sb","", RooArgSet(*mbb), Index(index), Import("background",*data_toy), Import("signal",*data_toy_sgn));
	//RooDataSet data_toy_sb("data_toy_sb","", RooArgSet(*mbb), Index(index), Import("background",*data_toy));
	n_toy = data_toy_sb.sumEntries();
	
	n_s.setVal(0.);
	n_b.setVal(bkg_norm.getVal());
	
	res_fit = pdf_fit_ext.fitTo(data_toy_sb, RooFit::Save(), RooFit::PrintLevel(0), RooFit::Extended(kTRUE)); 
	//res_fit = pdf_fit_ext.fitTo(*data_toy, RooFit::Save(), RooFit::PrintLevel(0), RooFit::Extended(kTRUE));

	data_toy_sb.plotOn(xframe, RooFit::Name("data_curve"));	
	pdf_fit_ext.plotOn(xframe, RooFit::Name("fit_curve"));
	//pdf_fit_ext.plotOn(xframe, RooFit::Name("bkg"), Components("background"), LineStyle(kDashed), LineColor(kRed), LineWidth(4));
	//pdf_fit_ext.plotOn(xframe, RooFit::Name("signal"), Components("gaus"), LineStyle(kDashed), LineColor(kGreen+2), LineWidth(4));
	
	chi_2 = xframe->chiSquare("fit_curve", "data_curve", npar);	//parameters from sig+bkg (novops)
	
	std::cout << "Chi^2 from the fit = " << chi_2 << std::endl;
	
	// Bottom frame
	RooHist* hpull;
	hpull = xframe->pullHist();
	hpull->SetMarkerSize(0.8);    //0.8 for lowM
	RooPlot* xframe2 = mbb->frame();
	xframe2->addPlotable(hpull,"P");
	xframe2->SetMinimum(-5.);
	xframe2->SetMaximum(+5.);

        pdf_fit_ext.plotOn(xframe, RooFit::Name("bkg"), Components("background"), LineStyle(kDashed), LineColor(kRed), LineWidth(4));
        pdf_fit_ext.plotOn(xframe, RooFit::Name("signal"), Components("gaus"), LineStyle(kDashed), LineColor(kGreen+2), LineWidth(4));

	TCanvas canvas;
	canvas.SetCanvasSize(500,500);
	canvas.cd();

	TPad* pad1;
	pad1 = new TPad("pad1","",0,0.1,1,1);
	//pad1->SetTitle("");
	pad1->SetBottomMargin(0.2);
	pad1->SetRightMargin(0.05);
	pad1->SetLeftMargin(0.16);
	pad1->Draw();
	pad1->cd();
	xframe->GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
	xframe->GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
	xframe->GetYaxis()->SetTitleSize(0.038);
	xframe->GetYaxis()->SetTitleOffset(1.6);
	xframe->GetYaxis()->SetLabelSize(0.033);
	//frame->GetYaxis()->SetRangeUser(frame->GetMinimum(), frame->GetMaximum()+200);
	xframe->Draw();
	TLegend *leg1 = new TLegend(0.60,0.60,0.86,0.87);
	leg1->SetFillColor(kWhite);
	leg1->SetLineColor(kWhite);
	leg1->AddEntry("data_curve","Toy Data", "P");
	leg1->AddEntry("fit_curve","Signal + background","L");
	leg1->AddEntry("bkg","Background only", "L");
	leg1->AddEntry("signal","Signal only", "L");
	leg1->Draw("same");

	canvas.cd();
	TPad *pad2 = new TPad("pad2","",0,0.0,1,0.265);
	pad2->SetTopMargin(1);
	pad2->SetBottomMargin(0.33);
	pad2->SetLeftMargin(pad1->GetLeftMargin());
	pad2->SetRightMargin(pad1->GetRightMargin());
	pad2->SetGridy();
	pad2->Draw();
	pad2->cd();
	xframe2->SetTitle("");
	xframe2->GetXaxis()->SetTitleSize(0.15);
	xframe2->GetXaxis()->SetTitleOffset(0.9);
	xframe2->GetXaxis()->SetLabelSize(0.115);
	xframe2->GetXaxis()->SetLabelOffset(0.010);
	xframe2->SetYTitle("Pulls");
	xframe2->GetYaxis()->CenterTitle(kTRUE);
	xframe2->GetYaxis()->SetTitleSize(0.14);
	xframe2->GetYaxis()->SetTitleOffset(0.4);
	xframe2->GetYaxis()->SetNdivisions(3,5,0);
	xframe2->GetYaxis()->SetLabelSize(0.115);
	xframe2->GetYaxis()->SetLabelOffset(0.011);
	xframe2->Draw();

	canvas.SaveAs("Fit_toycbeff_fitnovops_300_r10_linear.pdf");
	pad1->SetLogy();
	xframe->GetYaxis()->SetRangeUser(0.1, xframe->GetMaximum()*5);
	canvas.Modified();
	canvas.Update();
	canvas.SaveAs("Fit_toycbeff_fitnovops_300_r10_log.pdf");
	
	std::cout << "nb_toy = " << nb_toy << std::endl; 
	std::cout << "ns_toy = " << ns_toy << std::endl;
	std::cout << "n_toy  = " << n_toy << std::endl;
	std::cout << "n_s    = " << n_s.getVal() << std::endl;
	std::cout << "err_s  = " << n_s.getError() << std::endl;
	std::cout << "n_b    = " << n_b.getVal() << std::endl;
	std::cout << "err_b  = " << n_b.getError() << std::endl;	
	std::cout << "Bias1  = " << (n_s.getVal() - ns_toy)/(n_gen*n_s.getError()) << std::endl;
	std::cout << "Bias2  = " << (n_s.getVal() - ns_toy)/(n_s.getError()) << std::endl;
	
	ns_fit = n_s.getVal();
	ns_err = n_s.getError();
	nb_fit = n_b.getVal();
	nb_err = n_b.getError();
	bias   = (n_s.getVal() - ns_toy)/(n_s.getError());
        cout << "End of code :)" << endl;

}

