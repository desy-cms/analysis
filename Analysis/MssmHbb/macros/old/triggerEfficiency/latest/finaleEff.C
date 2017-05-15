#include "Analysis/MssmHbb/src/HbbStyle.cc"

double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
void drawRatio(TH1F*, TH1F*, TCanvas*);
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );
TH1F *constructCombined(TH1F *, TFile *,const int &,const int &);

void finaleEff() 
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   TFile * fOldSelection = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsSingleSelectionJetHTPromt-Reco-v4.root");
   TFile * fDoubleSelection = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsDoubleSelectionJetHTPromt-Reco-v4.root");//ResultsJetHT2015DPromptReco4.root");

   HbbStyle style;
   style.set(PUBLIC);

   //..............................Pt efficiency....................
   int bin40to60 = 90;
   int bin60to80 = 115;

   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   TH1F *oldCombine = (TH1F*) fOldSelection -> Get("hpf40_eff");
   oldCombine = constructCombined(oldCombine, fOldSelection, bin40to60, bin60to80);
   oldCombine -> SetTitle("Old Combination Method;p_{T}, [GeV];#epsilon");
   TH1F *newCombine = (TH1F*) fDoubleSelection -> Get("hpf40_eff");
   newCombine = constructCombined(newCombine, fDoubleSelection, bin40to60, bin60to80);
   newCombine -> SetTitle("New Combination Method;p_{T}, [GeV];#epsilon");
   oldCombine -> SetMarkerStyle(20);
   newCombine -> SetMarkerStyle(21);
   newCombine -> SetMarkerColor(2);
   drawRatio(oldCombine,newCombine,canva1);

   //Tag and probe comparison:
   TCanvas *canva2 = new TCanvas("canva2","",1000,800);
   TH1F *oldTP = (TH1F*) fOldSelection -> Get("T&Ppf100_eff");
   oldTP -> SetTitle("Old T&P Method; p_{T}, [GeV]; #epsilon");
   oldTP -> SetMarkerStyle(20);
   TH1F *newTP = (TH1F*) fDoubleSelection -> Get("T&Ppf100_eff");
   newTP -> SetTitle("New T&P Method; p_{T}, [GeV]; #epsilon");
   newTP -> SetMarkerStyle(21);
   newTP -> SetMarkerColor(2);
   drawRatio(oldTP,newTP,canva2);

   //TLegend *leg1 = new TLegend(0.4,0.45,0.9,0.65);
   //leg1 -> AddEntry(oldCombine,"Old Combination Method","p");
   //leg1 -> AddEntry(newCombine,"New Combination method","p");
   //leg1 -> Draw();



   /*

	TCanvas *can2 = new TCanvas();
    TH1F *TPpf100_eff = (TH1F*) fOldSelection ->Get("T&Ppf100_eff");
    TPpf100_eff -> SetTitle("Kinematic turn-on for 100 GeV Pt trashold (Tag and Probe method);p_{T}^{probe}, GeV;#epsilon");
    TPpf100_eff -> SetMarkerColor(2);
    TPpf100_eff -> SetMarkerStyle(20);
    drawRatio(TPpf100_eff,fit1,can2,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
    can2 -> Draw();
    
    //delete []fit1;
    TCanvas *canva3 = new TCanvas("canva3","",1000,800);
    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1 -> SetBottomMargin(0.05);
    pad1 -> Draw();
    pad1 -> cd();
    TPpf100_eff -> Draw();
    finalePtEff -> Draw("same");

    TLegend *LegendRatioCombTP = new TLegend(0.4,0.45,0.9,0.65);
    LegendRatioCombTP -> SetTextSize(0.04);
    LegendRatioCombTP -> AddEntry(TPpf100_eff,"Tag and Probe method","p");
    LegendRatioCombTP -> AddEntry(finalePtEff,"Combined method","p");
    LegendRatioCombTP ->Draw();

    canva3 -> cd();
    TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
    pad2 -> SetTopMargin(0);
    pad2 -> SetBottomMargin(0.2);
    pad2 -> Draw();
    pad2 -> cd();

    TH1F *ratio = (TH1F*) finalePtEff->Clone("ratio");
   	ratio -> Reset();
   	ratio->Divide(TPpf100_eff,finalePtEff,1.,1.,"b");
   	ratio->GetYaxis()->SetTitle("T&P/Combined");
   	ratio -> GetYaxis() -> SetTitleSize(20);
    ratio -> GetYaxis() -> SetTitleFont(43);
    ratio -> GetYaxis() -> SetTitleOffset(1.55);
    ratio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio -> GetYaxis() -> SetLabelSize(20);
  	ratio->Draw();

    /*
    //drawRatio(TPpf100_eff,fit2,canva3,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
    //canva3->Draw();
    fit1->SetLineColor(2);
    fit1->Draw("same");

	/*
	TCanvas *canva2 = new TCanvas();
	TH1F *hBand = new TH1F("hBand","",finalePtEff->GetNbinsX(),finalePtEff->GetXaxis()->GetXmin(),finalePtEff->GetXaxis()->GetXmax());
	double sigma = 0.683;
	(TVirtualFitter::GetFitter())->GetConfidenceIntervals(hBand,sigma);
	hBand -> SetFillColor(2);
	hBand -> SetFillStyle(3004);
	finalePtEff -> Draw();
	hBand -> Draw("e3 same");
	//fit0 -> Draw("same");
	
	TCanvas *can = new TCanvas();
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1 -> SetBottomMargin(0.05);
    pad1 -> Draw();
    pad1 -> cd();
    
	finalePtEff -> SetMarkerColor(2);
	finalePtEff -> SetLineColor(2);
	finalePtEff -> SetMarkerStyle(34);
	finalePtEff -> Draw();
	hpf80for100 -> Draw("same");
	TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
    fitLegend -> SetTextSize(0.04);
    fitLegend -> AddEntry(hpf80for100,"Old approach: PFJet80 only","p");
    fitLegend -> AddEntry(finalePtEff,"New approach: Method1","p");
    fitLegend ->Draw();
    
    can -> cd();
   
    TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
    pad2 -> SetTopMargin(0);
    pad2 -> SetBottomMargin(0.2);
    pad2 -> Draw();
    pad2 -> cd();
	
	
	TH1F *ratio = (TH1F*) finalePtEff->Clone("ratio");
	ratio -> Reset();
	ratio->Divide(finalePtEff,hpf80for100,1.,1.,"b");
	ratio->GetYaxis()->SetTitle("Method1/OldMethod");
	ratio -> GetYaxis() -> SetTitleSize(20);
    ratio -> GetYaxis() -> SetTitleFont(43);
    ratio -> GetYaxis() -> SetTitleOffset(1.55);
    ratio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio -> GetYaxis() -> SetLabelSize(20);
	ratio->Draw();
    //T&P method
    
    /*
    TCanvas *can2 = new TCanvas();
    TH1F *TPpf100_eff = (TH1F*) fOldSelection ->Get("T&Ppf100_eff");
    TPpf100_eff -> SetTitle(";p_{T}^{probe}, GeV;#epsilon");
    TPpf100_eff -> SetMarkerColor(1);
    TPpf100_eff -> SetMarkerStyle(20);
    TCanvas *canva3 = new TCanvas("canva3","",1000,800);
    TF1 *fit2 = new TF1("fit2",erfFunction,120,500,2);
    fit2 -> SetParameters(0.100921,109.42,0.183585,110.153);
    fit2 -> SetLineColor(4);
    TPpf100_eff->Fit(fit2,"R");
    /*
    //drawRatio(TPpf100_eff,fit2,canva3,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
    //canva3->Draw();
    fit1->SetLineColor(2);
    fit1->Draw("same");
    
   TLegend *fitLegend2 = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend2 -> SetTextSize(0.04);
   fitLegend2 -> AddEntry(fit2,"T&P method","l");
   fitLegend2 -> AddEntry(fit1,"Combination method","l");
   fitLegend2->Draw();

	/*
	
	TH1F *TPpf40_eff = (TH1F*) fOldSelection ->Get("T&Ppf40_eff");
    TPpf40_eff -> SetMarkerColor(1);
    TPpf40_eff -> SetMarkerStyle(20);
    TCanvas *canva2 = new TCanvas("canva2","",1000,800);
    TF1 *fit11 = new TF1("fit11",finaleFunction,30,500,4);
    fit11 -> SetParameters(2.48966e-01,3.70690e+01,2.48966e-01,3.70690e+01);
    fit11 -> SetLineColor(4);
    drawRatio(TPpf40_eff,fit11,canva2,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
    canva2->Draw();
	
   TH1F *HLT80 = (TH1F*) fOldSelection ->Get("HLT_PFJet80/hpf80_eff");
   HLT80 -> SetTitle("HLT_PFJet80 100 GeV Pt trashold;p_{T}, [Gev]; #epsilon");
   HLT80 -> SetMarkerColor(1);
   HLT80 -> SetMarkerStyle(20);
   TF1 *fit1 = new TF1("fit1",finaleFunction,90,500,4);  
   fit1 -> SetParameters(0.100921,109.42,0.183585,110.153);
   fit1 -> SetLineColor(4);
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   drawRatio(HLT80,fit1,canva1,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva1->Draw();
   */

}

void drawRatio(TH1F *hNum, TH1F *hDenum, TCanvas *can){

	HbbStyle style;
	style.set(PUBLIC);
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();

   TH1F * hRatio = (TH1F*)hNum->Clone("hRatio");
   TLegend *Legend = new TLegend(0.4,0.45,0.9,0.65);
   Legend -> SetTextSize(0.04);
   Legend -> AddEntry(hNum,hNum->GetTitle(),"p");
   Legend -> AddEntry(hDenum,hDenum->GetTitle(),"p");
   hNum -> Draw();
   hDenum -> Draw("same");
   Legend->Draw();
   can -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   hRatio -> Sumw2();
   hRatio -> Divide(hDenum);
   hRatio -> Draw();

   TLine *horizLine = new TLine(0,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

	//   cc2->cd();
	//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.6,1.4);
   hRatio -> GetYaxis() -> SetTitle("Ratio Old/New ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);

	   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(hNum->GetXaxis() -> GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);
}

TH1F *constructCombined(TH1F *CombinedH, TFile *file,const int & bin40to60,const int &bin60to80){

	CombinedH -> Reset();
	TH1F *h = (TH1F*) file -> Get("hpf40_eff");
	TH1F *HLT40 = (TH1F*) h -> Clone("HLT40");
	h = (TH1F*) file -> Get("HLT_PFJet60/hpf60_eff");
	TH1F *HLT60 = (TH1F*) h -> Clone("HLT60");
	h = (TH1F*) file ->Get("HLT_PFJet80/hpf80_eff");
	TH1F *HLT80 =(TH1F*) h -> Clone("HLT80");

    int xlow90 = HLT60 -> FindBin(bin40to60 + 2);
    int xupp90 = HLT40 -> FindBin(bin40to60 - 2);
    for(int bin = 1;bin< HLT40 -> GetNbinsX (); bin++)
    {
    		if(bin > xupp90) {
   			HLT40 -> SetBinContent(bin,0.);
   			HLT40 -> SetBinError(bin,0.);
   		}
   	}

   	int xlow115 = HLT80  -> FindBin(bin60to80 + 2);
   	int xupp115 = HLT60  -> FindBin(bin60to80 - 2);
   	for(int bin = 1;bin< HLT60 -> GetNbinsX (); bin++)
   {
   		if(bin > xupp115) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   		if(bin < xlow90) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   	}

   	for(int bin = 1;bin< HLT80 -> GetNbinsX (); bin++)
   {
   		if(bin < xlow115) {
   			HLT80 -> SetBinContent(bin,0.);
   			HLT80 -> SetBinError(bin,0.);
   		}
   	}

//   	CombinedH->Add(HLT40);
   	CombinedH->Add(HLT60);
   	CombinedH->Add(HLT80);
   	CombinedH -> SetMaximum(1.02);

   	return CombinedH;
}

double dEtaFitFunction(double *x, double *par)
{
	double sigmoid = 1./(1.+exp( - par[0] * (x[0] - par[1]) ));
	return sigmoid;
}
double finaleFunction(double *x, double *par)
{

	double sigmoid = 1./(1.+exp( -par[0] * (x[0] - par[1]) ));
	double erf = 1 / 2. * (1 + par[0]*TMath::Erf((x[0]-par[1])/(sqrt(2)*par[2])));
	double erfc = 1+ TMath::Erfc((par[5]-x[0])/(sqrt(2)*par[6]));
	double result =  0.5 * (1. + TMath::Erf((par[2]*x[0]-par[1])/(sqrt(2)*par[0])));
	double sigmoid2 = 1./ (1.+exp( -par[2] * (x[0] - par[3]) ));
	return sigmoid * sigmoid2;

}

double erfFunction(double *x, double *par) 
{
	return 0.5 * (1. + TMath::Erf((x[0]-par[0])/(sqrt(2)*par[1])));
	//return 0.5 * (1. + TMath::Erf((x[0]-par[0])/(sqrt(2)*par[1]))) * 1./(1.+exp( -par[2] * (x[0] - par[3]) ));
	//return (par[0]/2)*(par[0]/2) * (1 + TMath::Erf((x[0]-par[2])/(sqrt(2)*par[1]))) * (1 + TMath::Erf((x[0]-par[3])/(sqrt(2)*par[4])));
}

double sigmoid(double *x, double *par)
{
	return 1./(1.+exp( -par[0] * (x[0] - par[1]) ));
}

void drawRatio(TH1F *histo,TF1 *fit, TCanvas *canva, std::string fitName)
{

   //TCanvas *canva = new TCanvas("canva","",1000,800);
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();
   
//   histo -> SetTitle("HLT_PFJet80 100 GeV Pt treshold;; #epsilon");
//   histo -> SetMarkerColor(1);  
//   histo -> SetMarkerStyle(20);
//   histo -> Draw("ep");
   
   TH1F * hRatio = (TH1F*)histo->Clone("hRatio");
   histo -> Fit(fit,"R+");
   char name[200];
   sprintf(name,"}{#chi^{2}/ndf = %.2f}",fit->GetChisquare()/fit->GetNDF());
   fitName = "#splitline{ " + fitName + name;
   TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend -> SetTextSize(0.04);
   fitLegend -> AddEntry(fit,fitName.c_str(),"l");
   histo -> Draw();
   fitLegend->Draw();
   canva -> cd();
   
   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();
   
   hRatio -> Sumw2();
   
   hRatio -> Divide(fit);
   hRatio -> Draw();
   
   TLine *horizLine = new TLine(0,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

   
//   cc2->cd();
//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.6,1.4);
   hRatio -> GetYaxis() -> SetTitle("Ratio histo/fit ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);
   
   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(histo->GetXaxis() -> GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);
   
//   std::cout<<" Chi2/ndf: = "<<fit1D -> GetChisquare()/fit1D -> GetNDF()<<" sigmoid: "<<fit2D -> GetChisquare() / fit2D -> GetNDF()<<" Finale: "<<fit3D -> GetChisquare() / fit3D -> GetNDF()<<std::endl;

}

TCanvas *drawRatio(TH1F *histo,TF1 *fitMC, TF1 *fitData, TH1F *hData,  TCanvas *canva, std::string fitName)
{

TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();
   
//   histo -> SetTitle("HLT_PFJet80 100 GeV Pt treshold;; #epsilon");
//   histo -> SetMarkerColor(1);  
//   histo -> SetMarkerStyle(20);
//   histo -> Draw("ep");
   
   histo -> Fit(fitMC,"R+");
   fitData -> SetLineColor(2);
   char name[200];
   sprintf(name,"}{#chi^{2}/ndf = %.2f}",fitMC->GetChisquare()/fitMC->GetNDF());
   fitName = "#splitline{ " + fitName + name;
   TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend -> SetTextSize(0.04);
   fitLegend -> AddEntry(fitMC,fitName.c_str(),"l");
   fitLegend -> AddEntry(fitData,"Fit to Data","l");
   histo -> Draw();
   fitLegend->Draw();
   fitData -> Draw("same");
   canva->cd();
   
   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();
   
   
   
   TH1F* hRatio = (TH1F*) hData->Clone("hRatio");
   hRatio -> Divide(hData,histo,1,1,"b");
   hRatio -> Draw();
   
   TLine *horizLine = new TLine(0,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

   
//   cc2->cd();
//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.6,1.4);
   hRatio -> GetYaxis() -> SetTitle("Ratio fitData/fitMC ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);
   
   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(histo->GetXaxis() -> GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);
   
//   std::cout<<" Chi2/ndf: = "<<fit1D -> GetChisquare()/fit1D -> GetNDF()<<" sigmoid: "<<fit2D -> GetChisquare() / fit2D -> GetNDF()<<" Finale: "<<fit3D -> GetChisquare() / fit3D -> GetNDF()<<std::endl;
	return canva;
}

