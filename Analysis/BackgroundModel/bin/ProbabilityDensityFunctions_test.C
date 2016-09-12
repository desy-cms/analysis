/*
 * ProbabilityDensityFunctions_test.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: shevchen
 */

#include "../interface/ProbabilityDensityFunctions.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooNovosibirsk.h"
#include "TCanvas.h"
#include "RooPlot.h"

using namespace analysis::backgroundmodel;

int main(){
//	RooWorkspace *workspace = new RooWorkspace("space");
	RooWorkspace workspace("sapace");
	TCanvas canvas("canvas", "", 600, 600);

	RooRealVar var("var","var",0,1700,"ma");
	workspace.import(var);
	ProbabilityDensityFunctions *pdfs = new ProbabilityDensityFunctions(workspace,"var");

	//test1
	pdfs->getNovosibirsk("name");

	//test2
	RooRealVar peak("peak", "peak", 450, 50.0, 500.0, "GeV");
	RooRealVar width("width", "width", 100.0, 5.0, var.getMax()/2.0, "GeV");
	RooRealVar tail("tail", "tail", -0.5, -1.0, 1.0);
	pdfs->getNovosibirsk("name2","name2",peak,width,tail);

	//test3
	pdfs->getNovoPSProd("name3");

	//test4
	pdfs->getChebEffProd("name4",3);


	workspace.Print();

	RooPlot* frame = var.frame() ;
	workspace.pdf("name")->plotOn(frame);
	std::cout<<"test1 Ok"<<std::endl;
	workspace.pdf("name2")->plotOn(frame);
	std::cout<<"test2 Ok"<<std::endl;
	workspace.pdf("name3")->plotOn(frame);
	std::cout<<"test3 Ok"<<std::endl;
	workspace.pdf("name4")->plotOn(frame);
	std::cout<<"test4 Ok"<<std::endl;
	frame->Draw();
	canvas.SaveAs("can.png");
//	workspace.import(pdfs.getNovosibirsk("name",var));
//	RooAbsPdf &Pdf = *workspace.pdf("name");
//	Pdf.Draw();

	return 0;
}

