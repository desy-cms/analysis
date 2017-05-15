/*
 * plotBgShape.cpp
 *
 *  Created on: 10 апр. 2017 г.
 *      Author: rostyslav
 *
 *      Macro to plot shape of the different pdfs
 */
#include "TPad.h"

#include "Analysis/MssmHbb/src/utilLib.cpp"
#include "Analysis/Tools/interface/RooFitUtils.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

RatioPlots style;

using namespace std;
using namespace analysis;

void AbsComparisonOfPdfs(RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooRealVar &x, const bool& logY = true, const std::string& signal_subr = "sr1");
void RelComparisonOfPdfs(RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooRealVar &x, const bool& logY = true, const std::string& signal_subr = "sr1");
void DrawSignalPDFs(RooPlot &frame, const std::string& sr);

int plotBgShape(){
	style.set(PRIVATE);

	// Get All workspaces
	//Novosibirsk workspaces
	auto &wNovo_sr1 = *GetRooWorkspace("../../../BackgroundModel/test/extnovoeffprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root");
	auto &wNovo_sr2 = *GetRooWorkspace("../../../BackgroundModel/test/novosibirsk_350to1190_20GeV_G4/workspace/FitContainer_workspace.root");
	auto &wNovo_sr3 = *GetRooWorkspace("../../../BackgroundModel/test/novosibirsk_500to1700_25GeV_G4/workspace/FitContainer_workspace.root");
	//Dijet workspaces
	auto &wDiJet_sr1 = *GetRooWorkspace("../../../BackgroundModel/test/dijetv3logprod_200to650_10GeV_G4/workspace/FitContainer_workspace.root");
	auto &wDiJet_sr2 = *GetRooWorkspace("../../../BackgroundModel/test/dijetv2_350to1190_20GeV_G4/workspace/FitContainer_workspace.root");
	auto &wDiJet_sr3 = *GetRooWorkspace("../../../BackgroundModel/test/dijetv2_500to1700_25GeV_G4/workspace/FitContainer_workspace.root");
	//Bernstein-9
	auto &wBernstein = *GetRooWorkspace("../../../BackgroundModel/test/berneffprod9par_200to650_10GeV_G4/workspace/FitContainer_workspace.root");
	//SuperDijet2 workspace
	auto &wSuperDiJet2_sr1 = *GetRooWorkspace("../../../BackgroundModel/test/Prescale_v5/superdijeteffprod2_200_to_650_10GeV_G4/workspace/FitContainer_workspace.root");
	//Bernstein-8 x Efficiency
	auto &wBernstein8 = *GetRooWorkspace("../../../BackgroundModel/test/Prescale_v5/berneff8_200_to_650_10GeV/workspace/FitContainer_workspace.root");
	//Get pdfs
	auto &pdfNovo_sr1 = *GetFromRooWorkspace<RooAbsPdf>(wNovo_sr1,"background");
	pdfNovo_sr1.SetTitle("Extnovoeffprod");
	auto &pdfNovo_sr2 = *GetFromRooWorkspace<RooAbsPdf>(wNovo_sr2,"background");
	pdfNovo_sr2.SetTitle("Novosibirsk_sr2");
	auto &pdfNovo_sr3 = *GetFromRooWorkspace<RooAbsPdf>(wNovo_sr3,"background");
	pdfNovo_sr3.SetTitle("Novosibirsk_sr3");

	auto &pdfDiJet_sr1 = *GetFromRooWorkspace<RooAbsPdf>(wDiJet_sr1,"background");
	pdfDiJet_sr1.SetTitle("DiJetLogProd");
	auto &pdfDiJet_sr2 = *GetFromRooWorkspace<RooAbsPdf>(wDiJet_sr2,"background");
	pdfDiJet_sr2.SetTitle("DiJet_sr2");
	auto &pdfDiJet_sr3 = *GetFromRooWorkspace<RooAbsPdf>(wDiJet_sr3,"background");
	pdfDiJet_sr3.SetTitle("DiJet_sr3");

	auto &pdfBernstein = *GetFromRooWorkspace<RooAbsPdf>(wBernstein,"background");
	pdfBernstein.SetTitle("Bernstein9EffProd");

	auto &pdfSuperDiJet2_sr1 = *GetFromRooWorkspace<RooAbsPdf>(wSuperDiJet2_sr1, "background");
	pdfSuperDiJet2_sr1.SetTitle("SuperDijet2EffProd");

	auto &pdfBernstein8 = *GetFromRooWorkspace<RooAbsPdf>(wBernstein8, "background");
	pdfBernstein8.SetTitle("Bernstein8EffProd");

	RooRealVar &x = *wNovo_sr1.var("mbb");
	AbsComparisonOfPdfs(pdfNovo_sr1,pdfDiJet_sr1,x);		// Novo vs DiJet
	RelComparisonOfPdfs(pdfNovo_sr1,pdfDiJet_sr1,x);		// Novo vs DiJet
	AbsComparisonOfPdfs(pdfDiJet_sr1,pdfNovo_sr1,x);		// DiJet vs Novo
	RelComparisonOfPdfs(pdfDiJet_sr1,pdfNovo_sr1,x);		// DiJet vs Novo
	AbsComparisonOfPdfs(pdfNovo_sr1,pdfBernstein,x);		// Novo vs Bernstein
	RelComparisonOfPdfs(pdfNovo_sr1,pdfBernstein,x);		// Novo vs Bernstein
	AbsComparisonOfPdfs(pdfBernstein,pdfNovo_sr1,x);		// Bernstein-9 vs Novo
	RelComparisonOfPdfs(pdfBernstein,pdfNovo_sr1,x);		// Bernstein-9 vs Novo
	AbsComparisonOfPdfs(pdfNovo_sr1, pdfSuperDiJet2_sr1, x);// Novo vs SuperDijet2
	RelComparisonOfPdfs(pdfNovo_sr1, pdfSuperDiJet2_sr1, x);// Novo vs SuperDijet2
	AbsComparisonOfPdfs(pdfNovo_sr1, pdfBernstein8, x);		// Novo vs Bernstein8
	RelComparisonOfPdfs(pdfNovo_sr1, pdfBernstein8, x);		// Novo vs Bernstein8

	//sr2
	x.setRange(350, 1190);
	AbsComparisonOfPdfs(pdfNovo_sr2,pdfDiJet_sr2,x,true,"sr2");	// Novo vs DiJet
	RelComparisonOfPdfs(pdfNovo_sr2,pdfDiJet_sr2,x,false,"sr2");	// Novo vs DiJet
	AbsComparisonOfPdfs(pdfDiJet_sr2,pdfNovo_sr2,x,true,"sr2");	// DiJet vs Novo
	RelComparisonOfPdfs(pdfDiJet_sr2,pdfNovo_sr2,x,false,"sr2");	// DiJet vs Novo

	//sr3
	x.setRange(500, 1700);
	AbsComparisonOfPdfs(pdfNovo_sr3,pdfDiJet_sr3,x,true,"sr3");	// Novo vs DiJet
	RelComparisonOfPdfs(pdfNovo_sr3,pdfDiJet_sr3,x,false,"sr3");	// Novo vs DiJet
	AbsComparisonOfPdfs(pdfDiJet_sr3,pdfNovo_sr3,x,true,"sr3");	// DiJet vs Novo
	RelComparisonOfPdfs(pdfDiJet_sr3,pdfNovo_sr3,x,false,"sr3");	// DiJet vs Novo

	return 0;
}

void AbsComparisonOfPdfs(RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooRealVar &x, const bool& logY, const std::string& signal_subr){
	/*
	 * Function to compare pdfs in absolute scale
	 */
	x.setBins(1000);
	RooPlot &frame = *x.frame();
	TH1D &h1 = *static_cast<TH1D*>(pdf1.createHistogram("h1",x));
	TH1D &h2 = *static_cast<TH1D*>(pdf2.createHistogram("h2",x));

	pdf1.plotOn(&frame,RooFit::LineColor(2),RooFit::LineWidth(2));
	pdf2.plotOn(&frame,RooFit::LineColor(kBlue),RooFit::LineWidth(2));

	TCanvas can("can","can",800,600);
	TPad pad1_("pad1","pad1",0,0.1,1,1);
	pad1_ . SetBottomMargin(0.2);
	pad1_ . SetRightMargin(0.05);
	pad1_ . SetLeftMargin(0.16);
	pad1_ . Draw();
	pad1_ . cd();

	frame.GetXaxis()->SetNdivisions(505);
	frame.GetYaxis()->SetNdivisions(206);
	frame.GetYaxis()->SetTitleOffset(1.1);
	frame.GetXaxis()->SetTitleOffset(999);
	frame.GetXaxis()->SetLabelOffset(999);
	frame.GetYaxis()->SetTitleSize(0.048);
	frame.SetStats(0);
	string title = static_cast<string>(pdf1.GetTitle()) + "_vs_" + static_cast<string>(pdf2.GetTitle());
	frame.SetTitle( (title + "; M_{A} [GeV]; a.u.").c_str() );
	gPad->SetLogy(logY);
	DrawSignalPDFs(frame,signal_subr);
	frame.Draw();

	can.cd();
	TPad pad2_("pad2","pad2",0,0.0,1,0.265);
	pad2_ . SetTopMargin(0);
	pad2_ . SetLeftMargin(pad1_.GetLeftMargin());
	pad2_ . SetRightMargin(pad1_.GetRightMargin());
	pad2_ . SetBottomMargin(0.28);
	pad2_ . Draw();
	pad2_ . cd();

	h1.Add(&h2,-1);
//	h1.Divide(&h2);
	h1.Scale(1000);
//	h1.SetTitle((";M_{A} [GeV];#frac{(" + static_cast<string>(pdf1.GetTitle()) + " - " + static_cast<string>(pdf2.GetTitle()) + ")}{" + static_cast<string>(pdf2.getTitle()) + "}, %").c_str() );
	h1.SetTitle((";M_{A} [GeV];#splitline{(" + static_cast<string>(pdf1.GetTitle()) + " -}{- " + static_cast<string>(pdf2.GetTitle()) + ")  x 10^{3} }").c_str() );
	style.SetBottomStyle(&h1);
	h1.SetStats(0);
	h1.SetLineWidth(1.5);
	h1.Draw();
	TLine l(x.getMin(),0,x.getMax(),0);
	l.SetLineStyle(9);
	l.Draw();

	h1.GetYaxis()->SetTitleSize(0.07);
	h1.GetYaxis()->SetTitleOffset(0.9);
	can.Update();
	can.Print(("../pictures/Bias_test/" + title + ".pdf").c_str());
}

void RelComparisonOfPdfs(RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooRealVar &x, const bool& logY = true, const std::string& signal_subr){
	/*
	 * Function to compare pdfs in relative scale
	 */
	x.setBins(1000);
	RooPlot &frame = *x.frame();
	TH1D &h1 = *static_cast<TH1D*>(pdf1.createHistogram("h1",x));
	TH1D &h2 = *static_cast<TH1D*>(pdf2.createHistogram("h2",x));

	pdf1.plotOn(&frame,RooFit::LineColor(2),RooFit::LineWidth(2));
	pdf2.plotOn(&frame,RooFit::LineColor(kBlue),RooFit::LineWidth(2));

	TCanvas can("can","can",800,600);
	TPad pad1_("pad1","pad1",0,0.1,1,1);
	pad1_ . SetBottomMargin(0.2);
	pad1_ . SetRightMargin(0.05);
	pad1_ . SetLeftMargin(0.16);
	pad1_ . Draw();
	pad1_ . cd();

	frame.GetXaxis()->SetNdivisions(505);
	frame.GetYaxis()->SetNdivisions(206);
	frame.GetYaxis()->SetTitleOffset(1.1);
	frame.GetXaxis()->SetTitleOffset(999);
	frame.GetXaxis()->SetLabelOffset(999);
	frame.GetYaxis()->SetTitleSize(0.048);
	frame.SetStats(0);
	string title = static_cast<string>(pdf1.GetTitle()) + "_vs_" + static_cast<string>(pdf2.GetTitle());
	frame.SetTitle( (title + "; M_{A} [GeV]; a.u.").c_str() );
	gPad->SetLogy(logY);
	DrawSignalPDFs(frame,signal_subr);
	frame.Draw();

	can.cd();
	TPad pad2_("pad2","pad2",0,0.0,1,0.265);
	pad2_ . SetTopMargin(0);
	pad2_ . SetLeftMargin(pad1_.GetLeftMargin());
	pad2_ . SetRightMargin(pad1_.GetRightMargin());
	pad2_ . SetBottomMargin(0.28);
	pad2_ . Draw();
	pad2_ . cd();

	h1.Divide(&h2);
	h1.SetTitle((";M_{A} [GeV];#frac{" + static_cast<string>(pdf1.GetTitle()) + "}{" + static_cast<string>(pdf2.getTitle()) + "}").c_str() );
	style.SetBottomStyle(&h1);
	h1.SetStats(0);
	h1.SetLineWidth(1.5);
	h1.Draw();
	TLine l(x.getMin(),1,x.getMax(),1);
	l.SetLineStyle(9);
	l.Draw();
//	cout<<"WTF: "<<h1.GetBinContent(10)<<endl;

	can.Print(("../pictures/Bias_test/relative_" + title + ".pdf").c_str());
}

void DrawSignalPDFs(RooPlot &frame, const std::string& sr){
	/*
	 * Draw signal pdfs on the frame according to the sub-range
	 */
	if(sr == "sr1"){
		auto &p300 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-300/workspace/FitContainer_workspace.root","signal");
		auto &p350 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-350/workspace/FitContainer_workspace.root","signal");
		auto &p400 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-400/workspace/FitContainer_workspace.root","signal");
		auto &p500 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-500/workspace/FitContainer_workspace.root","signal");

		p300.plotOn(&frame,RooFit::LineColor(kRed-4),RooFit::LineStyle(1),RooFit::Normalization(0.01));
		p350.plotOn(&frame,RooFit::LineColor(kGreen-4),RooFit::LineStyle(2),RooFit::Normalization(0.01));
		p400.plotOn(&frame,RooFit::LineColor(kCyan-4),RooFit::LineStyle(6),RooFit::Normalization(0.01));
		p500.plotOn(&frame,RooFit::LineColor(kBlue-4),RooFit::LineStyle(10),RooFit::Normalization(0.01));
	}
	else if(sr  == "sr2"){
		auto &p600 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-600/workspace/FitContainer_workspace.root","signal");
		auto &p700 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-700/workspace/FitContainer_workspace.root","signal");
		auto &p900 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-900/workspace/FitContainer_workspace.root","signal");

		p600.plotOn(&frame,RooFit::LineColor(kRed-4),RooFit::LineStyle(1),RooFit::Normalization(0.01));
		p700.plotOn(&frame,RooFit::LineColor(kGreen-4),RooFit::LineStyle(2),RooFit::Normalization(0.01));
		p900.plotOn(&frame,RooFit::LineColor(kCyan-4),RooFit::LineStyle(6),RooFit::Normalization(0.01));
	}
	else if(sr == "sr3"){
		auto &p1100 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-1100/workspace/FitContainer_workspace.root","signal");
		auto &p1300 = *GetRooObjectFromTFile<RooAbsPdf>("../../output/ReReco_signal_M-1300/workspace/FitContainer_workspace.root","signal");

		p1100.plotOn(&frame,RooFit::LineColor(kRed-4),RooFit::LineStyle(1),RooFit::Normalization(0.01));
		p1300.plotOn(&frame,RooFit::LineColor(kGreen-4),RooFit::LineStyle(2),RooFit::Normalization(0.01));
	}

}
