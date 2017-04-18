#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *Draw(TH2F *histo, TCanvas *canva); 
double fitFunction(double *, double*);

void ptEffEta() 
{
	gROOT -> Reset();
    //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   //TFile * fC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015C.root");
   TFile * fD = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsJetHT2015DPromptReco4.root");
   HbbStyle style;
   style.set(PUBLIC);
   //..............................Pt efficiency....................
   //PFJet80 Pt trashold 100
   TH2F *btagEff = (TH2F*) fD ->Get("h2_eff80");
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   btagEff -> SetTitle(" #epsilon=f(Pt,#eta);Pt, [GeV]; #eta"); 
   btagEff -> SetStats(kFALSE);
   btagEff -> GetXaxis() -> SetTitleOffset(1.);
   btagEff -> Draw("colz");
   //canva1->Draw();
   std::cout<<btagEff -> Interpolate(100.,-1.)<<std::endl;
   
   TF1 *fit1 = new TF1("fit1",fitFunction,90,500,4);  
   fit1 -> SetParameters(0.100921,109.42,0.183585,110.153);
   // Projection X
   TCanvas *canva2 = new TCanvas("canva2","",2000,1600);
   TH1D* xProjectionCentral = (TH1D*)btagEff -> ProjectionX("xProjectionCentral",3,3,"e");
   xProjectionCentral -> SetMarkerColor(2);
   xProjectionCentral -> SetMarkerStyle(20);
   xProjectionCentral -> SetMarkerSize(1.5);
   xProjectionCentral -> GetYaxis()-> SetRangeUser(0.,1.02);
   xProjectionCentral -> GetYaxis() -> SetTitle("#epsilon");
   xProjectionCentral -> GetYaxis() -> SetTitleOffset(1.);
   xProjectionCentral -> Draw();
   fit1 -> SetLineColor(2);
   xProjectionCentral -> Fit("fit1");
   
   TH1D* xProjectionMiddle1 = (TH1D*)btagEff -> ProjectionX("xProjectionMiddle1",2,2,"e");
   xProjectionMiddle1 -> SetMarkerStyle(21);
   xProjectionMiddle1 -> SetMarkerSize(1.5);
   xProjectionMiddle1 -> Scale(0.5);
   TH1D* xProjectionMiddle2 = (TH1D*)btagEff -> ProjectionX("xProjectionMiddle2",4,4,"e");
   xProjectionMiddle2 -> Scale(0.5);
   xProjectionMiddle1 -> Add(xProjectionMiddle2);
   xProjectionMiddle1 -> Draw("P same");
   fit1 -> SetLineColor(1);
   xProjectionMiddle1 -> Fit("fit1","+");
   
   TH1D* xProjectionHigh1 = (TH1D*)btagEff -> ProjectionX("xProjectionHigh1",1,1,"e");
   xProjectionHigh1 -> SetMarkerStyle(22);
   xProjectionHigh1 -> SetMarkerSize(1.5);
   xProjectionHigh1 -> SetMarkerColor(4);
   xProjectionHigh1 -> Scale(0.5);
   TH1D* xProjectionHigh2 = (TH1D*)btagEff -> ProjectionX("xProjectionHigh2",5.,5,"e");
   xProjectionHigh2 -> Scale(0.5);
   xProjectionHigh1 -> Add(xProjectionHigh2);
   xProjectionHigh1 -> Draw("P same");
   fit1 -> SetLineColor(4);
   xProjectionHigh1 -> Fit("fit1","+");
   
   TLegend *leg = new TLegend(0.65,0.7,0.9,0.9);
   leg -> AddEntry(xProjectionCentral,"|#eta|<0.5","p");
   leg -> AddEntry(xProjectionMiddle1,"0.5<|#eta|<1.5","p");
   leg -> AddEntry(xProjectionHigh1,"1.5<|#eta|<2.5","p");
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
	double sigmoid = 1./(1.+exp( -par[0] * (x[0] - par[1]) ));
	double erf = 1 / 2. * (1 + par[0]*TMath::Erf((x[0]-par[1])/(sqrt(2)*par[2])));
	double erfc = 1+ TMath::Erfc((par[5]-x[0])/(sqrt(2)*par[6]));
	double result =  0.5 * (1. + TMath::Erf((par[2]*x[0]-par[1])/(sqrt(2)*par[0])));
	double sigmoid2 = 1./ (1.+exp( -par[2] * (x[0] - par[3]) ));
	return sigmoid * sigmoid2;
}
