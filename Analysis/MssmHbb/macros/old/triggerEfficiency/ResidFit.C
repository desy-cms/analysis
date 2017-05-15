#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *Draw(TH1F *histo, TCanvas *canva); 

void ResidFit() 
{
	gROOT -> Reset();
    gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   //TFile * fC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015C.root");
   TFile * fD = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015D.root");
   
   //..............................Pt efficiency....................
   //PFJet80 Pt trashold 100
   TH1F *slice1 = (TH1F*) fD ->Get("h_slice_80_90");
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   Draw(slice1,canva1);
   canva1->Draw();
   
   TH1F *slice11 = (TH1F*) fD ->Get("h_slice_80_90_dif");
   TCanvas *canva11 = new TCanvas("canva11","",1000,800);
   Draw(slice11,canva11);
   canva11->Draw();
   
   TH1F *slice2 = (TH1F*) fD ->Get("h_slice_90_100");
   TCanvas *canva2 = new TCanvas("canva2","",1000,800);
   Draw(slice2,canva2);
   canva2->Draw();
   
   TH1F *slice21 = (TH1F*) fD ->Get("h_slice_90_100_dif");
   TCanvas *canva21 = new TCanvas("canva21","",1000,800);
   Draw(slice21,canva21);
   canva21->Draw();
   
   TH1F *slice3 = (TH1F*) fD ->Get("h_slice_100_120");
   TCanvas *canva3 = new TCanvas("canva3","",1000,800);
   Draw(slice3,canva3);
   canva3->Draw();
   
   TH1F *slice31 = (TH1F*) fD ->Get("h_slice_100_120_dif");
   TCanvas *canva31 = new TCanvas("canva31","",1000,800);
   Draw(slice31,canva31);
   canva31->Draw();
   
   TH1F *slice4 = (TH1F*) fD ->Get("h_slice_170_180");
   TCanvas *canva4 = new TCanvas("canva4","",1000,800);
   Draw(slice4,canva4);
   canva4->Draw();
   
   TH1F *slice41 = (TH1F*) fD ->Get("h_slice_170_180_dif");
   TCanvas *canva41 = new TCanvas("canva41","",1000,800);
   Draw(slice41,canva41);
   canva41->Draw();
   
   TH1F *slice5 = (TH1F*) fD ->Get("h_slice_180_190");
   TCanvas *canva5 = new TCanvas("canva5","",1000,800);
   Draw(slice5,canva5);
   canva5->Draw();
   
   TH1F *slice51 = (TH1F*) fD ->Get("h_slice_180_190_dif");
   TCanvas *canva51 = new TCanvas("canva51","",1000,800);
   Draw(slice51,canva51);
   canva51->Draw();
   
   TH1F *slice6 = (TH1F*) fD ->Get("h_slice_280_290");
   TCanvas *canva6 = new TCanvas("canva6","",1000,800);
   Draw(slice6,canva6);
   canva6->Draw();
   
   TH1F *slice61 = (TH1F*) fD ->Get("h_slice_280_290_dif");
   TCanvas *canva61 = new TCanvas("canva61","",1000,800);
   Draw(slice61,canva61);
   canva61->Draw();
   
   TH1F *slice7 = (TH1F*) fD ->Get("h_slice_270_280");
   TCanvas *canva7 = new TCanvas("canva7","",1000,800);
   Draw(slice7,canva7);
   canva7->Draw();
   
   TH1F *slice71 = (TH1F*) fD ->Get("h_slice_270_280_dif");
   TCanvas *canva71 = new TCanvas("canva71","",1000,800);
   Draw(slice71,canva71);
   canva71->Draw();


   
}

TCanvas *Draw(TH1F *histo,TCanvas *canva)
{
	histo -> SetMarkerColor(1);
	histo -> SetMarkerStyle(20);
	TF1 *fit = new TF1("fit","gaus",histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax());
	histo->Fit(fit);
	std::string name = "./"+(std::string)histo->GetName()+".png";
	canva->SaveAs(name.c_str());
	
	return canva;
}

