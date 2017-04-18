#include "Analysis/Drawer/src/HbbStyle.cc"

TH1F * constractEffForMC(const std::string &ptName);

TFile *data, *mc;

int forYeChen(){
	gROOT->Reset();


	data = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsForYeChen.root");
	mc = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsForYeChenQCDMC.root");

	HbbStyle style;
	style.set(PUBLIC);

	//Data
	TH1F *pt300 = (TH1F*) data->Get("pTEffL3_300");
	pt300->SetMarkerStyle(20);
	pt300->SetAxisRange(200,700,"X");
	pt300->SetTitle(";p_{T} , [GeV]; #Epsilon");
	TH1F *pt350 = (TH1F*) data->Get("pTEffL3_350");
	pt350->SetMarkerStyle(21);
	pt350->SetMarkerColor(kRed);
	TH1F *pt400 = (TH1F*) data->Get("pTEffL3_400");
	pt400->SetMarkerStyle(22);
	pt400->SetMarkerColor(kMagenta);
	TH1F *pt450 = (TH1F*) data->Get("pTEffL3_450");
	pt450->SetMarkerStyle(23);
	pt450->SetMarkerColor(kYellow+3);

	TLegend *legenda = new TLegend(0.5,0.5,0.9,0.85);
	legenda->AddEntry(pt300,"L3>300 GeV, Off. Plato #approx 380 GeV","p");
	legenda->AddEntry(pt350,"L3>350 GeV, Off. Plato #approx 440 GeV","p");
	legenda->AddEntry(pt400,"L3>400 GeV, Off. Plato #approx 480 GeV","p");
	legenda->AddEntry(pt450,"L3>450 GeV, Off. Plato #approx 520 GeV","p");

	pt300->Draw();
	pt350->Draw("same");
	pt400->Draw("same");
	pt450->Draw("same");
	legenda->Draw();
	style.standardTitle()->Draw();

	//Data/MC comparison
	TCanvas *canva = new TCanvas("canva","canva",1000,800);

	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
	pad1 -> SetBottomMargin(0.05);
	pad1 -> Draw();
	pad1 -> cd();

	TH1F *pt300mc = constractEffForMC("pTEffL3_300");
	pt300mc->SetMarkerStyle(24);
	TH1F *pt350mc = constractEffForMC("pTEffL3_350");
	pt350mc->SetMarkerStyle(25);
	pt350mc->SetMarkerColor(kRed);
	TH1F *pt400mc = constractEffForMC("pTEffL3_400");
	pt400mc->SetMarkerStyle(26);
	pt400mc->SetMarkerColor(kMagenta);
	TH1F *pt450mc = constractEffForMC("pTEffL3_450");
	pt450mc->SetMarkerStyle(32);
	pt450mc->SetMarkerColor(kYellow+3);

	TLegend *legenda2 = new TLegend(0.5,0.5,0.9,0.85);

	legenda2->AddEntry(pt300,"L3>300 GeV, Off. Plato #approx 380 GeV","p");
	legenda2->AddEntry(pt300mc,"L3>300 GeV, MC","p");
	legenda2->AddEntry(pt350,"L3>350 GeV, Off. Plato #approx 440 GeV","p");
	legenda2->AddEntry(pt350mc,"L3>350 GeV, MC","p");
	legenda2->AddEntry(pt400,"L3>400 GeV, Off. Plato #approx 480 GeV","p");
	legenda2->AddEntry(pt400mc,"L3>400 GeV, MC","p");
	legenda2->AddEntry(pt450,"L3>450 GeV, Off. Plato #approx 520 GeV","p");
	legenda2->AddEntry(pt450mc,"L3>450 GeV, MC","p");

	pt300->Draw();
	pt350->Draw("same");
	pt400->Draw("same");
	pt450->Draw("same");
	pt300mc->Draw("same");
	pt350mc->Draw("same");
	pt400mc->Draw("same");
	pt450mc->Draw("same");

	legenda2->Draw();
	style.standardTitle()->Draw();

   canva -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0.1);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   TH1F *hRatio300 = (TH1F*) pt300->Clone("hRatio300");
   hRatio300->Divide(pt300mc);
   TH1F *hRatio350 = (TH1F*) pt350->Clone("hRatio350");
   hRatio350->Divide(pt350mc);
   TH1F *hRatio400 = (TH1F*) pt400->Clone("hRatio400");
   hRatio400->Divide(pt400mc);
   TH1F *hRatio450 = (TH1F*) pt450->Clone("hRatio450");
   hRatio450->Divide(pt450mc);

   hRatio300->Draw();
   hRatio350->Draw("same");
   hRatio400->Draw("same");
   hRatio450->Draw("same");

   hRatio300 -> SetTitle("");
   hRatio300 -> GetYaxis() -> SetRangeUser(0.,2.);
   hRatio300 -> GetYaxis() -> SetTitle("Ratio Data/MC ");
   hRatio300 -> GetYaxis() -> SetNdivisions(505);
   hRatio300 -> GetYaxis() -> SetTitleSize(20);
   hRatio300 -> GetYaxis() -> SetTitleFont(43);
   hRatio300 -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio300 -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio300 -> GetYaxis() -> SetLabelSize(20);

	   // X axis ratio plot settings
   hRatio300 -> GetXaxis() -> SetTitle(pt300->GetXaxis() -> GetTitle());
   hRatio300 -> GetXaxis() -> SetTitleSize(20);
   hRatio300 -> GetXaxis() -> SetTitleFont(43);
   hRatio300 -> GetXaxis() -> SetTitleOffset(3.);
   hRatio300 -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio300 -> GetXaxis() -> SetLabelSize(20);

	return 0;

}

TH1F * constractEffForMC(const std::string &ptName){
	std::string name;
	name = ptName + "Num";
	TH1F *numer = (TH1F*) mc -> Get(name.c_str());
	name = ptName + "DeNum";
	TH1F *deNum = (TH1F*) mc -> Get(name.c_str());
	numer->Divide(deNum);
	return numer;
}
