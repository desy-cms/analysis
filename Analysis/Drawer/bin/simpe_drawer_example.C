#include "/Users/rostyslav/Documents/DESY/CMS/development/MssmHbbBackUp/Analysis/Drawer/src/HbbStyle.cc"
#include "/Users/rostyslav/Documents/DESY/CMS/development/MssmHbbBackUp/Analysis/Drawer/src/RatioPlots.cpp"


int simpe_drawer_example(){

	//Add Data input file:
	TFile *data = new TFile("/Users/rostyslav/Documents/DESY/CMS/development/MssmHbbBackUp/Analysis/MssmHbb/bin/outputFiles/SelectionBTag2015DPromptReco4.root");
	//Add MC input File:
	TFile *mc = new TFile("/Users/rostyslav/Documents/DESY/CMS/development/MssmHbbBackUp/Analysis/MssmHbb/bin/outputFiles/SelectionQCD.root");

	//Setup style
	HbbStyle style;
	style.set(PUBLIC);

	//Setup RatioDrawer class
	RatioPlots *ratio = new RatioPlots;

	TH1F *pt1_data = (TH1F*) data->Get("h_jet1_Pt");
	TH1F *pt1_mc = (TH1F*) mc->Get("h_jet1_Pt");
	TLegend *leg_pt1 = style.legend("top_reight",100000);

	TCanvas *canva = new TCanvas();
	gPad->SetLogy();
	ratio->SetRatioRange(0.6,1.4);
	ratio->SetRatioTitle("blabla");
	ratio->DrawRatio(pt1_data,pt1_mc,leg_pt1,canva);

	return 0;
}
