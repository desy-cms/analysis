/*
 * Macro to compare shpae of the bg templates
 */
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

using namespace std;

int shape_comparison(){
	TCanvas *can = new TCanvas("can","",800,600);
	TFile *f2015 = new TFile("~/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
	TFile *f2016 = new TFile("~/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_2016BG_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");

	TH1D *h2015 = (TH1D*) f2015->Get("data_obs");
	TH1D *h2016 = (TH1D*) f2016->Get("data_obs");

	//Draw ratio objects;
	RatioPlots ratio(PRIVATE);
	//Style
	HbbStyle style;
	style.set(PRIVATE);

	h2015->Scale(h2016->Integral()/h2015->Integral());
	h2016->SetMarkerStyle(20);
	h2016->SetMarkerSize(1.1);
	h2016->SetTitle("");
	h2016->Draw("E");
	h2015->SetLineColor(2);
	h2015->Draw("same");

	TLegend *leg = new TLegend(0.6,0.5,0.9,0.85);
	leg->AddEntry(h2016,"2016","p");
	leg->AddEntry(h2015,"2015","l");

	TH1D* h_ratio = (TH1D*) ratio.DrawRatio(h2016,h2015,can,leg,nullptr);
	h_ratio -> SetAxisRange(0.2,1.8,"Y");
	h_ratio -> GetYaxis()->SetTitle("2016/2015");
	ratio.GetTopPad();		style.drawStandardTitle();

	can->Print("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/Background/Shape2015_vs_2016.pdf");
	gPad->SetLogy();
	can->Print("~/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/Background/Shape2015_vs_2016_log.pdf");
	return 0;
}
