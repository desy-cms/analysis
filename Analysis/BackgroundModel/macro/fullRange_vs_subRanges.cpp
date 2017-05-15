/*
 * Macro to compare shpae of the bg templates
 */
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

using namespace std;

int fullRange_vs_subRanges(){
	TCanvas *can = new TCanvas("can","",800,600);
	TFile *fFull = new TFile("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_fullrange_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root","read");
	TFile *fsub1 = new TFile("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_subrange_presc_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root","read");
	TFile *fsub2 = new TFile("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_subrange_presc_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-600_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root","read");
	TFile *fsub3 = new TFile("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_subrange_presc_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-1100_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root","read");

	//Draw ratio objects;
	RatioPlots ratio(PRIVATE);
	//Style
	HbbStyle style;
	style.set(PRIVATE);

	TCanvas *can_qcd = new TCanvas("can_qcd","",800,600);
	//full is binned from 200 to 1700, 20 GeV bin
	TH1D *h_qcd_full = (TH1D*) fFull->Get("QCD_Mbb");
	// subrange 1: binned from 200 to 650 with 10 GeV bins
	TH1D *h_qcd_sub1 = (TH1D*) fsub1->Get("QCD_Mbb");
	// subrange 2: binned from 350 to 1190 with 20 GeV bins
	TH1D *h_qcd_sub2 = (TH1D*) fsub2->Get("QCD_Mbb");
	// subrange 3: binned from 500 to 1700 with 25 GeV bins
	TH1D *h_qcd_sub3 = (TH1D*) fsub3->Get("QCD_Mbb");

	h_qcd_full->SetLineColor(1);
	h_qcd_full->SetLineWidth(1.1);
	int bin_sub1_low = h_qcd_full->FindBin(200);
	int bin_sub1_up  = h_qcd_full->FindBin(650);
	int bin_sub2_low = h_qcd_full->FindBin(350);
	int bin_sub2_up  = h_qcd_full->FindBin(1190);
	int bin_sub3_low = h_qcd_full->FindBin(500);
	int bin_sub3_up  = h_qcd_full->FindBin(1700);

	h_qcd_sub1->SetMarkerColor(2);
	h_qcd_sub1->SetMarkerStyle(20);
	h_qcd_sub1->SetMarkerSize(1.1);

	h_qcd_sub2->SetMarkerColor(3);
	h_qcd_sub2->SetMarkerStyle(21);
	h_qcd_sub2->SetMarkerSize(1.1);

	h_qcd_sub3->SetMarkerColor(4);
	h_qcd_sub3->SetMarkerStyle(22);
	h_qcd_sub3->SetMarkerSize(1.1);

	h_qcd_full->Draw();
	h_qcd_sub1->Draw("Esame");
	h_qcd_sub2->Draw("Esame");
	h_qcd_sub3->Draw("Esame");


//	TH1D *h2015 = (TH1D*) f2015->Get("data_obs");
//	TH1D *h2016 = (TH1D*) f2016->Get("data_obs");
//
//	//Draw ratio objects;
//	RatioPlots ratio(PRIVATE);
//	//Style
//	HbbStyle style;
//	style.set(PRIVATE);
//
//	h2015->Scale(h2016->Integral()/h2015->Integral());
//	h2016->SetMarkerStyle(20);
//	h2016->SetMarkerSize(1.1);
//	h2016->SetTitle("");
//	h2016->Draw("E");
//	h2015->SetLineColor(2);
//	h2015->Draw("same");
//
//	TLegend *leg = new TLegend(0.6,0.5,0.9,0.85);
//	leg->AddEntry(h2016,"2016","p");
//	leg->AddEntry(h2015,"2015","l");
//
//	TH1D* h_ratio = (TH1D*) ratio.DrawRatio(h2016,h2015,can,leg,nullptr);
//	h_ratio -> SetAxisRange(0.2,1.8,"Y");
//	h_ratio -> GetYaxis()->SetTitle("2016/2015");
//	ratio.GetTopPad();		style.drawStandardTitle();
//
//	can->Print("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/Background/Shape2015_vs_2016.pdf");
//	gPad->SetLogy();
//	can->Print("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/Background/Shape2015_vs_2016_log.pdf");
	return 0;
}
