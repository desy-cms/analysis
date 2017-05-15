/*
 * extnovoxeff_shape.cpp
 *
 *  Created on: 21 Mar 2017
 *      Author: shevchen
 */
#include "RooWorkspace.h"

#include "Analysis/MssmHbb/src/utilLib.cpp"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

RatioPlots style;

using namespace std;

int extnovoxeff_shape(){
	/*
	 * Function to visualise ExtendedNovosibirsk x Efficiency shape
	 * for different parameter configurations
	 */

	style.set(PRIVATE);
	string path_to_file = mssmhbb::cmsswBase + "/src/Analysis/BackgroundModel/test/novosibirsk_350to1190_20GeV_G4/workspace/FitContainer_workspace.root";
	RooWorkspace *w = GetRooWorkspace(path_to_file);
	RooWorkspace *w_clone = (RooWorkspace*)w->Clone("w_clone");

	RooRealVar *mbb = w->var("mbb");
	mbb->setBins(1000);
	RooPlot *frame = mbb->frame();

	RooAbsPdf *bg = w->pdf("background");
	RooAbsPdf *bg_clone = w_clone->pdf("background");
//	w->var("peak1")->setVal(140);
//	w->var("width1")->setVal(30);
//	w->var("tail1")->setVal(-0.9);
	TH1D *h = (TH1D*) bg->createHistogram("h", *mbb);
	bg->plotOn(frame, RooFit::LineColor(4));
//	w_clone->var("peak1")->setVal(440);
	w_clone->var("width1")->setVal(53);
//	w_clone->var("tail1")->setVal(-0.9);
	bg_clone->plotOn(frame, RooFit::LineColor(2));

	TH1D *h_clone = (TH1D*) bg_clone->createHistogram("h_clone", *mbb);
	frame->Draw();

	TCanvas *can = new TCanvas("can","can",800,600);
	TPad *pad1_ = new TPad("pad1","pad1",0,0.1,1,1);
	pad1_ -> SetBottomMargin(0.2);
	pad1_ -> SetRightMargin(0.05);
	pad1_ -> SetLeftMargin(0.16);
	pad1_ -> Draw();
	pad1_ -> cd();

	frame->GetXaxis()->SetNdivisions(505);
	frame->GetYaxis()->SetNdivisions(206);
	frame->GetYaxis()->SetTitleOffset(1.1);
	frame->GetXaxis()->SetTitleOffset(999);
	frame->GetXaxis()->SetLabelOffset(999);
	frame->GetYaxis()->SetTitleSize(0.048);
	frame->SetStats(0);
	frame->SetTitle("ExtendedNovosibirsk x Eff: different pars;;a.u.");

	gPad->SetLogy();
	frame->Draw();

	can->cd();
	TPad *pad2_ = new TPad("pad2","pad2",0,0.0,1,0.265);
	pad2_ -> SetTopMargin(0);
	pad2_ -> SetLeftMargin(pad1_->GetLeftMargin());
	pad2_ -> SetRightMargin(pad1_->GetRightMargin());
	pad2_ -> SetBottomMargin(0.28);
	pad2_ -> Draw();
	pad2_ -> cd();

	style.SetBottomStyle(h_clone);
//	h_clone
	h_clone->Divide(h);
	h_clone->SetStats(0);
	h_clone->SetTitle(";M_{12} [GeV]; RED / BLUE");
	h_clone->Draw();

	return 0;
}


