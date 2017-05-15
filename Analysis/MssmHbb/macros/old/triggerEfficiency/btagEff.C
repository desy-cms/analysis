#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *Draw(TH2F *histo, TCanvas *canva); 
double fitFunction(double *, double*);

void btagEff() 
{
	gROOT -> Reset();
    //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   //TFile * fC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015C.root");
   TFile * fD = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/bTagEfficiency2D_pteta_zomin.root");
   HbbStyle style;
   style.set(PUBLIC);
   //..............................Pt efficiency....................
   //PFJet80 Pt trashold 100
   TH2F *btagEff = (TH2F*) fD ->Get("h2ehn");
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   btagEff -> SetTitle("BTagCSV #epsilon=f(Pt,#eta) for 0.941 Offline;Pt, [GeV]; #eta"); 
   btagEff -> SetStats(kFALSE);
   btagEff -> GetXaxis() -> SetTitleOffset(1.);
   btagEff -> Draw("colz");
   //canva1->Draw();
   std::cout<<btagEff -> Interpolate(100.,-1.)<<std::endl;
   
   
   // Projection X
   TCanvas *canva2 = new TCanvas("canva2","",2000,1600);
   TH1D* xProjectionCentral = (TH1D*)btagEff -> ProjectionX("xProjectionCentral",3,3,"e");
   xProjectionCentral -> SetMarkerColor(2);
   xProjectionCentral -> SetMarkerStyle(20);
   xProjectionCentral -> SetMarkerSize(1.5);
   xProjectionCentral -> GetYaxis()-> SetRangeUser(0.4,0.95);
   xProjectionCentral -> GetYaxis() -> SetTitle("#epsilon");
   xProjectionCentral -> GetYaxis() -> SetTitleOffset(1.);
   xProjectionCentral -> Draw("P");
   xProjectionCentral -> SetLineColor(kRed-3);
   xProjectionCentral -> Draw("HIST L same");
   
   TH1D* xProjectionMiddle1 = (TH1D*)btagEff -> ProjectionX("xProjectionMiddle1",2,2,"e");
   xProjectionMiddle1 -> SetMarkerStyle(21);
   xProjectionMiddle1 -> SetMarkerSize(1.5);
   xProjectionMiddle1 -> Scale(0.5);
   TH1D* xProjectionMiddle2 = (TH1D*)btagEff -> ProjectionX("xProjectionMiddle2",4,4,"e");
   xProjectionMiddle2 -> Scale(0.5);
   xProjectionMiddle1 -> Add(xProjectionMiddle2);
   xProjectionMiddle1 -> Draw("P same");
   xProjectionMiddle1 -> SetLineColor(kGray);
   xProjectionMiddle1 -> Draw("HIST L same");
   
   TH1D* xProjectionHigh1 = (TH1D*)btagEff -> ProjectionX("xProjectionHigh1",1,1,"e");
   xProjectionHigh1 -> SetMarkerStyle(22);
   xProjectionHigh1 -> SetMarkerSize(1.5);
   xProjectionHigh1 -> SetMarkerColor(4);
   xProjectionHigh1 -> Scale(0.5);
   TH1D* xProjectionHigh2 = (TH1D*)btagEff -> ProjectionX("xProjectionHigh2",5.,5,"e");
   xProjectionHigh2 -> Scale(0.5);
   xProjectionHigh1 -> Add(xProjectionHigh2);
   xProjectionHigh1 -> Draw("P same");
   xProjectionHigh1 -> SetLineColor(kBlue-3);
   xProjectionHigh1 -> Draw("HIST L same");
   
   TLegend *leg = new TLegend(0.65,0.7,0.9,0.9);
   leg -> AddEntry(xProjectionCentral,"|#eta|<0.5","pl");
   leg -> AddEntry(xProjectionMiddle1,"0.5<|#eta|<1.5","pl");
   leg -> AddEntry(xProjectionHigh1,"1.5<|#eta|<2.5","pl");
   leg->Draw();
   //Draw(btagEff,canva1);


   
}

TCanvas *Draw(TH2F *histo,TCanvas *canva)
{
	histo -> SetMarkerColor(1);
	histo -> SetMarkerStyle(20);
	TF2 *fit = new TF2("fit",fitFunction,histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax(),histo->GetYaxis()->GetXmin(),histo->GetYaxis()->GetXmax(),5);
	histo->Fit(fit);
	histo -> Draw("colz");
	std::string name = "./"+(std::string)histo->GetName()+".png";
	canva->SaveAs(name.c_str());
	
	return canva;
}

double fitFunction( double *x, double *par)
{
	double gaus = par[0] /( sqrt(2.*TMath::Pi()) * par[1]) * TMath::Exp(-0.5*((x[1]-par[2])/par[1])*((x[1]-par[2])/par[1]));
	double lin1 = par[3] * x[0] + par[4];
	return gaus * lin1;
}
