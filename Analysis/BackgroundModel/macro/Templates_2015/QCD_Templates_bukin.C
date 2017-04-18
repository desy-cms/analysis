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
#include "TArray.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooFormulaVar.h"
#include "RooEffProd.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"
#include "RooBernstein.h"
#include "RooChebychev.h"
#include "RooNovosibirsk.h"
#include "RooCBShape.h"
#include "RooBukinPdf.h"
#include "RooProdPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooList.h"
#include "RooAddPdf.h"

using namespace std;
using namespace RooFit;

TH1* QCD_HistSyst(bool up, int iPar, double para_a_array[5], double dpara_a_array[5])
{
  	TString outname; 

	double para_b_array[5] = {0.,0.,0.,0.,0.};

	for(int i = 0; i < 5; ++i)
	{	para_b_array[i] = para_a_array[i]; 	}

	if(up) 	{ para_b_array[iPar] += 2 * abs(dpara_a_array[iPar]); }
	else	{ para_b_array[iPar] -= 2 * abs(dpara_a_array[iPar]); }

	switch(iPar)
	{
	case 0: if(up) outname = "PAR0_13TeVUp"; else outname = "PAR0_13TeVDown"; break;
	case 1: if(up) outname = "PAR1_13TeVUp"; else outname = "PAR1_13TeVDown"; break;
	case 2: if(up) outname = "PAR2_13TeVUp"; else outname = "PAR2_13TeVDown"; break;
	case 3: if(up) outname = "PAR3_13TeVUp"; else outname = "PAR3_13TeVDown"; break;
	case 4: if(up) outname = "PAR4_13TeVUp"; else outname = "PAR4_13TeVDown"; break;
	}

	cout << "outname : " << outname << endl;
	cout << "para_b_array[0] = " << para_b_array[0] << endl;
	cout << "para_b_array[1] = " << para_b_array[1] << endl;
	cout << "para_b_array[2] = " << para_b_array[2] << endl;
	cout << "para_b_array[3] = " << para_b_array[3] << endl;
	cout << "para_b_array[4] = " << para_b_array[4] << endl;

	RooRealVar mbb("mbb","mbb", 240.0, 1700.0) ;

	RooRealVar Xp("Xp", "Xp", para_b_array[0], 50.0, 350.0, "GeV");
	RooRealVar rho1("rho1", "rho1", para_b_array[1], -0.1, 0.2);
	RooRealVar rho2("rho2", "rho2", para_b_array[2], -0.07, 0.045);
	RooRealVar sigp("sigp", "sigp", para_b_array[3], 20.0, 85.0, "GeV");
	RooRealVar xi("xi", "xi", para_b_array[4], 0.0, 0.55);

	mbb.setBins(73);

  	RooBukinPdf bukin("bukin","bukin PDF", mbb, Xp, sigp, xi, rho1, rho2);

	//RooDataSet* data = novopsprod.generate(mbb,9821) ;

	TH1* h1 = bukin.createHistogram("QCD", mbb, Binning(73,240.,1700.));
	h1->Scale(9821./h1->Integral());
	h1->SetTitle("QCD Templates");
	h1->SetName("QCD_Mbb_CMS_"+outname);
	cout << "h1->Integral() : " << h1->Integral() << endl;

//        for(int i = 0; i < h1->GetNbinsX(); i++)
//        {       h1->SetBinError(i,0.);      }

	//h1->SetLineColor(kRed);
	//h1->Draw("same hist");

  	// F i t   m o d e l   t o   d a t a ,   p l o t   m o d e l 
  	// ---------------------------------------------------------
  	// Make a second plot frame in x and draw both the 
  	// data and the p.d.f in the frame
        RooDataSet* data = bukin.generate(mbb,9821) ;
  	RooPlot* xframe = mbb.frame(Title("Bukin PDF "+outname)) ;
  	data->plotOn(xframe,RooFit::Name("data_curve"));
  	bukin.plotOn(xframe,RooFit::Name("fit_curve"));


  	// F i t   m o d e l   t o   d a t a
  	// -----------------------------
  	bukin.fitTo(*data) ;
  
  	double chi2 = xframe->chiSquare("fit_curve", "data_curve", 3);
  	std::cout << "Chi^2 = " << chi2 << std::endl;

  	// Draw all frames on a canvas
  	TCanvas* c = new TCanvas("fit result"+outname,"fit result"+outname,600,400) ;
  	gPad->SetLeftMargin(0.15); xframe->GetYaxis()->SetTitleOffset(1.6); xframe->Draw();
	c->SaveAs("Fitresult_"+outname+"_Bukin.pdf");
 
	return h1;
}

void drawcanvas(TH1* h_novo, TH1* h_syst)
{
  	//gStyle->SetOptTitle(0);
  	gStyle->SetOptStat("");
  	gStyle->SetLegendBorderSize(0);

	TH1* h_central = (TH1*)h_novo->Clone("h_central");
	TH1* h_ratio = (TH1*)h_novo->Clone("h_ratio");

	bool logy = 1;
	TCanvas canvas;
	canvas.SetCanvasSize(500,500);

	TPad* pad1;
   	pad1 = new TPad("pad1","",0,0.1,1,1);
    	pad1->SetBottomMargin(0.2);
    	pad1->SetRightMargin(0.05); // The ratio plot below inherits the right and left margins settings here!
    	pad1->SetLeftMargin(0.16); 
  	pad1->Draw();
  	pad1->cd();
  	if (logy) { 	pad1->SetLogy();
			h_central->SetMaximum(h_central->GetMaximum()*10);
			h_central->SetMinimum(h_central->GetMinimum()/10);
	}
 	else {		h_central->SetMaximum(h_central->GetMaximum()+200.);
			h_central->SetMinimum(0.);
	}
	h_central->SetTitle(h_syst->GetName());
      	h_central->GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
      	h_central->GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
      	h_central->GetYaxis()->SetTitleSize(0.041);
      	h_central->GetYaxis()->SetTitleOffset(1.20);
      	h_central->GetYaxis()->SetLabelSize(0.04);
 	h_central->Draw("hist");
	h_syst->Draw("same hist");	

	canvas.cd();
	//bottom ratio
	TPad *pad2 = new TPad("pad2","",0,0.0,1,0.25);
	pad2->SetTopMargin(1);
	pad2->SetBottomMargin(0.33);
	pad2->SetLeftMargin(pad1->GetLeftMargin());
	pad2->SetRightMargin(pad1->GetRightMargin());
     	pad2->SetGridy();
	pad2->Draw();
	pad2->cd();

	h_ratio->SetTitle("");
	h_ratio->Divide(h_syst);
    	h_ratio->SetMarkerStyle(8);
  	h_ratio->SetMarkerSize(0.5);	
    	h_ratio->GetXaxis()->SetTitleSize(0.15);
    	h_ratio->GetXaxis()->SetTitleOffset(0.85);
    	h_ratio->GetXaxis()->SetLabelSize(0.12);
    	h_ratio->GetXaxis()->SetLabelOffset(0.008);
    	h_ratio->SetYTitle("Ratio");
    	h_ratio->GetYaxis()->CenterTitle(kTRUE);
    	h_ratio->GetYaxis()->SetTitleSize(0.15);
    	h_ratio->GetYaxis()->SetTitleOffset(0.3);
    	h_ratio->GetYaxis()->SetNdivisions(3,5,0);
    	h_ratio->GetYaxis()->SetLabelSize(0.12);
    	h_ratio->GetYaxis()->SetLabelOffset(0.011);

    	h_ratio->Draw("p");
    	h_ratio->SetMaximum(1.5);
    	h_ratio->SetMinimum(0.5);

	TString pdfname;
	pdfname = h_syst->GetName();
	canvas.SaveAs(pdfname+"_Bukin.pdf");

}


void QCD_Templates_bukin() 
{
  	// R e a d   w o r k s p a c e   f r o m   f i l e
  	// -----------------------------------------------

  	// Open input file with workspace
  	TFile *f = new TFile("FitContainer_workspace_bukin.root") ;

  	// Retrieve workspace from file
  	RooWorkspace* w = (RooWorkspace*) f->Get("workspace") ;

  	// R e t r i e v e   p d f ,   d a t a   f r o m   w o r k s p a c e
  	// -----------------------------------------------------------------

  	// Retrieve x,model and data from workspace
  	RooRealVar* mbb = w->var("mbb") ;
  	RooRealVar* Xp = w->var("Xp");
  	RooRealVar* rho1 = w->var("rho1");
  	RooRealVar* rho2 = w->var("rho2");
	RooRealVar* sigp = w->var("sigp");
	RooRealVar* xi = w->var("xi");
  	RooAbsPdf* bukin = w->pdf("background") ;
  	//int nPars = novopsprod->getParameters().getSize();

  	Xp->Print();
  	rho1->Print();
  	rho2->Print();
	sigp->Print();
	xi->Print();
	
  	double Xp_ = Xp->getVal();
  	double rho1_ = rho1->getVal();
  	double rho2_ = rho2->getVal();
	double sigp_ = sigp->getVal();
	double xi_ = xi->getVal();
  	double dXp_ = Xp->getError();
  	double drho1_ = rho1->getError();
  	double drho2_ = rho2->getError();
	double dsigp_ = sigp->getError();
	double dxi_ = xi->getError();

  	RooDataSet* data = bukin->generate(*mbb, 9821.);
  	// Create Pseu-do Data from function
  	TH1* h_central = data->createHistogram("QCD", *mbb, Binning(73,240.,1700.));
  	h_central->SetTitle("QCD Templates");
  	h_central->SetName("data_obs");
  	h_central->SetStats(kFALSE);
  	h_central->SetLineColor(kBlack);
  	h_central->SetLineWidth(2);
  	//h_central->Draw("hist");

  	// F i t   m o d e l   t o   d a t a
  	// ---------------------------------
  	//RooFitResult* r = novopsprod->fitTo(*data,Save()) ;
  	bukin->fitTo(*data) ;

  	// F i t   m o d e l   t o   d a t a ,   p l o t   m o d e l 
  	// ---------------------------------------------------------
  	// Make a second plot frame in x and draw both the 
  	// data and the p.d.f in the frame
  	RooPlot* xframe = mbb->frame(Title("Bukin PDF with Pseudo-Data in SR")) ;
  	data->plotOn(xframe,RooFit::Name("data_curve"));
  	bukin->plotOn(xframe,RooFit::Name("fit_curve")); //,VisualizeError(*r,1));

  	double chi2 = xframe->chiSquare("fit_curve", "data_curve", 3);
  	std::cout << "Chi^2 = " << chi2 << std::endl;

  	// Draw all frames on a canvas
  	TCanvas* c = new TCanvas("fit result","fit result",650,500) ;
  	gPad->SetLeftMargin(0.15); xframe->GetYaxis()->SetTitleOffset(1.6); xframe->Draw();
  	c->SaveAs("Fitresult_Central_Bukin.pdf");

  	// Central Histogram for central templates without errors
  	TH1* h_novo = bukin->createHistogram("QCD", *mbb, Binning(73,240.,1700.));
  	h_novo->Scale(9821./h_novo->Integral());
  	h_novo->SetTitle("QCD Templates");
        h_novo->SetName("QCD_Mbb");
  	h_novo->SetStats(kFALSE);
  	h_novo->SetLineColor(kBlack);
  	h_novo->SetLineWidth(2);
 
//	for(int i = 0; i < h_novo->GetNbinsX(); i++)
//	{	h_novo->SetBinError(i,0.);	}

  	// Output file to store all QCD Templates
  	TFile* outputFile = new TFile("QCD_Templates_Bukin.root","RECREATE");
  	outputFile->cd();
  	h_central->Write();
 	h_novo->Write();

  	double para_a_array[5] = {Xp_,rho1_,rho2_,sigp_,xi_};
  	double dpara_a_array[5] = {dXp_,drho1_,drho2_,dsigp_,dxi_};

  	TCanvas *canvas = new TCanvas("QCD Templates","QCD Templates",1200,800);
  	canvas->Divide(3,2);
  	canvas->cd(1);
  	h_central->Draw("hist");

  	TH1* h_syst_up;
  	TH1* h_syst_down;

  	h_syst_up = QCD_HistSyst(1,0,para_a_array,dpara_a_array);
  	h_syst_up->SetLineColor(kRed);
  	h_syst_up->SetLineWidth(2);
  	h_syst_up->SetStats(kFALSE);
  	drawcanvas(h_novo,h_syst_up);
  	h_syst_down = QCD_HistSyst(0,0,para_a_array,dpara_a_array);  
  	h_syst_down->SetLineColor(kBlue);
  	h_syst_down->SetLineWidth(2);
  	h_syst_down->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_down);  
	canvas->cd(2);
  	gPad->SetLogy();
  	h_novo->Draw("hist");
  	h_syst_up->Draw("same hist");
  	h_syst_down->Draw("same hist");
  	h_syst_up->Write();
  	h_syst_down->Write();

  	h_syst_up = QCD_HistSyst(1,1,para_a_array,dpara_a_array);
  	h_syst_up->SetLineColor(kRed);
  	h_syst_up->SetLineWidth(2);
  	h_syst_up->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_up);
  	h_syst_down = QCD_HistSyst(0,1,para_a_array,dpara_a_array);
  	h_syst_down->SetLineColor(kBlue);
  	h_syst_down->SetLineWidth(2);
  	h_syst_down->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_down);
  	canvas->cd(3);
  	gPad->SetLogy();
  	h_novo->Draw("hist");
  	h_syst_up->Draw("same hist");
  	h_syst_down->Draw("same hist");
  	h_syst_up->Write();
  	h_syst_down->Write();

  	h_syst_up = QCD_HistSyst(1,2,para_a_array,dpara_a_array);
  	h_syst_up->SetLineColor(kRed);
  	h_syst_up->SetLineWidth(2);
  	h_syst_up->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_up);
  	h_syst_down = QCD_HistSyst(0,2,para_a_array,dpara_a_array);
  	h_syst_down->SetLineColor(kBlue);
  	h_syst_down->SetLineWidth(2);
  	h_syst_down->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_down);
  	canvas->cd(4);
  	gPad->SetLogy();
  	h_novo->Draw("hist");
  	h_syst_up->Draw("same hist");
  	h_syst_down->Draw("same hist");
  	h_syst_up->Write();
  	h_syst_down->Write();

  	h_syst_up = QCD_HistSyst(1,3,para_a_array,dpara_a_array);
  	h_syst_up->SetLineColor(kRed);
  	h_syst_up->SetLineWidth(2);
  	h_syst_up->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_up);
  	h_syst_down = QCD_HistSyst(0,3,para_a_array,dpara_a_array);
  	h_syst_down->SetLineColor(kBlue);
  	h_syst_down->SetLineWidth(2);
  	h_syst_down->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_down);
  	canvas->cd(5);
  	gPad->SetLogy();
  	h_novo->Draw("hist");
  	h_syst_up->Draw("same hist");
  	h_syst_down->Draw("same hist");
  	h_syst_up->Write();
  	h_syst_down->Write();

  	h_syst_up = QCD_HistSyst(1,4,para_a_array,dpara_a_array);
  	h_syst_up->SetLineColor(kRed);
  	h_syst_up->SetLineWidth(2);
  	h_syst_up->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_up);
  	h_syst_down = QCD_HistSyst(0,4,para_a_array,dpara_a_array);
  	h_syst_down->SetLineColor(kBlue);
  	h_syst_down->SetLineWidth(2);
  	h_syst_down->SetStats(kFALSE);
	drawcanvas(h_novo,h_syst_down);
  	canvas->cd(6);
  	gPad->SetLogy();
  	h_novo->Draw("hist");
  	h_syst_up->Draw("same hist");
  	h_syst_down->Draw("same hist");
  	h_syst_up->Write();
  	h_syst_down->Write();


  	canvas->SaveAs("QCD_Templates_Bukin.pdf");

  	outputFile->Close();
  	cout << "End of code :)" << endl;

}






