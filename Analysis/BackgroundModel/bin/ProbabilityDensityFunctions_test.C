/*
 * ProbabilityDensityFunctions_test.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: shevchen
 */

#include "../interface/ProbabilityDensityFunctions.h"
#include "../interface/RooQuadGausExp.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooNovosibirsk.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCmdArg.h"

using namespace analysis::backgroundmodel;

int main(){

	RooRealVar x("x","x",200,1700);

	RooPlot *frame = x.frame();
	RooWorkspace *workspace = new RooWorkspace("work");

	workspace->import(x);
	ProbabilityDensityFunctions *pdfs = new ProbabilityDensityFunctions(*workspace,"x");
	pdfs->setPeakStart(850.);
	pdfs->getRooQuadGausExp("model");
	(workspace->pdf("model"))->plotOn(frame);
	TCanvas canvas;
	frame->Draw();

	RooRealVar mean("mean2","mean",850.015,200,1500,"GeV");
	RooRealVar sigmaL1("sigmaL12", "sigmaL1", 89.062, 0.5, 800.0, "GeV");
	RooRealVar sigmaL2("sigmaL22", "sigmaL2", 225.257, 0.5, 800.0, "GeV");
	RooRealVar sigmaR1("sigmaR12", "sigmaR1", 409.087, 0.5, 800.0, "GeV");
	RooRealVar sigmaR2("sigmaR22", "sigmaR2", 59.4907, 0.5, 800.0, "GeV");
	RooRealVar tail_shift("tail_shift2", "tail_shift", 1083.93, 200.0, 1500.0, "GeV");
	RooRealVar tail_sigma("tail_sigma2", "tail_sigma", 157.0, 0.5, 900.0, "GeV");
	RooRealVar norm_g1("norm_g12", "norm_g1", 0.511, 0, 1);
	RooRealVar norm_g2("norm_g22", "norm_g2", 0.01, 0, 1);
	RooQuadGausExp quadgexp("HARDCODED","HARDCODED_quadgexp",x,mean,sigmaL1,sigmaL2,sigmaR1,sigmaR2,tail_shift,tail_sigma,norm_g1,norm_g2);
	quadgexp.plotOn(frame,RooFit::LineColor(kRed));
	frame->Draw();

	canvas.SaveAs("can.png");

	/*
	workspace.pdf("name")->plotOn(frame);
	std::cout<<"test1 Ok"<<std::endl;
	workspace.pdf("name2")->plotOn(frame);
	std::cout<<"test2 Ok"<<std::endl;
	workspace.pdf("name3")->plotOn(frame);
	std::cout<<"test3 Ok"<<std::endl;
	*/

//	workspace.import(pdfs.getNovosibirsk("name",var));
//	RooAbsPdf &Pdf = *workspace.pdf("name");
//	Pdf.Draw();

	return 0;
}

