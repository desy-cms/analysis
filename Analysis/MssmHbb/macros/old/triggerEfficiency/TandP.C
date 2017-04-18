#include "Analysis/MssmHbb/src/HbbStyle.cc"

double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );

void TandP() 
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   TFile * fD = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsJetHTPromt-Reco-v4.root");
   TFile * fMC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsMCEffStart30New.root");//ResultsJetHT2015DPromptReco4.root");

   HbbStyle style;
   style.set(PUBLIC);

   //..............................Pt efficiency....................
   /*
   TH1F *TPpf80_eff = (TH1F*) fD -> Get("T&Ppf80_eff");
   TPpf80_eff -> SetMarkerColor(2);
   TPpf80_eff -> SetMarkerStyle(21);
   TPpf80_eff -> SetLineColor(4);
   TPpf80_eff -> SetTitle("PFJet80 efficiency");
   TH1F *hpf60for80_eff = (TH1F*) fD -> Get("HLT_PFJet60/hpf60for80_eff");
   hpf60for80_eff -> SetMarkerStyle(20);
   hpf60for80_eff -> SetMarkerColor(1);
   
   TCanvas *can = new TCanvas();
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
    pad1 -> SetBottomMargin(0.05);
    pad1 -> Draw();
    pad1 -> cd();
    
    TPpf80_eff -> Draw();
    hpf60for80_eff -> Draw("same");
    
    
    TLegend *l = new TLegend(0.4,0.45,0.9,0.65);
    l -> SetTextSize(0.04);
    l -> AddEntry(hpf60for80_eff,"Old approach","p");
    l -> AddEntry(TPpf80_eff,"T&P","p");
    l ->Draw();
    
    can -> cd();
   
    TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
    pad2 -> SetTopMargin(0);
    pad2 -> SetBottomMargin(0.2);
    pad2 -> Draw();
    pad2 -> cd();
	
	
	TH1F *ratio = (TH1F*) TPpf80_eff->Clone("ratio");
	ratio -> Reset();
	ratio->Divide(TPpf80_eff,hpf60for80_eff,1.,1.,"b");
	ratio->GetYaxis()->SetTitle("Method1/OldMethod");
	ratio -> GetYaxis() -> SetTitleSize(20);
    ratio -> GetYaxis() -> SetTitleFont(43);
    ratio -> GetYaxis() -> SetTitleOffset(1.55);
    ratio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio -> GetYaxis() -> SetLabelSize(20);
	ratio->Draw();

	/*
	TCanvas *canvaN = new TCanvas("canvaN","",1000,800);
	TH1F *TPpf100_eff = (TH1F*) fMC -> Get("T&Ppf100_eff");
	TPpf100_eff -> SetMarkerStyle(21);
	TPpf100_eff -> SetLineColor(4);
	TPpf100_eff -> SetTitle("PFJet100 efficiency");
	drawRatio(TPpf100_eff,fit11,canvaN,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
	canvaN->Draw();
	*/
    
	// FOR MC only:
	TCanvas *can2 = new TCanvas();
	TPad *pad12 = new TPad("pad12","pad1",0,0.3,1,1);
	pad12 -> SetBottomMargin(0.05);
	pad12 -> Draw();
	pad12 -> cd();
	TH1F *TPpf100_effMC = (TH1F*) fMC -> Get("T&P_probe80");
	TPpf100_effMC -> Divide( (TH1F*) fMC -> Get("T&P_probe100"), (TH1F*) fMC -> Get("T&P_tag100"),1.,1.,"b");
	TPpf100_effMC -> SetMarkerColor(2);
	TPpf100_effMC -> SetMarkerStyle(21);
	TPpf100_effMC -> SetLineColor(4);
	TPpf100_effMC -> SetTitle("PFJet100 efficiency");

	TH1F *hpf100_effMC = (TH1F*) fMC -> Get("HLT_PFJet80/hpf80_effNumerator");
	hpf100_effMC -> Divide((TH1F*) fMC -> Get("HLT_PFJet80/hpf80_effNumerator"),(TH1F*) fMC -> Get("HLT_PFJet80/hpf80_effDenumerator"),1,1,"b");
	hpf100_effMC -> SetMarkerStyle(20);
	hpf100_effMC -> SetMarkerColor(1);

	TH1F *hpf100_effMCModel = (TH1F*) fMC -> Get("hpf100_effMCmodel_Num");
	hpf100_effMCModel -> Divide((TH1F*) fMC -> Get("hpf100_effMCmodel_Num"),(TH1F*) fMC -> Get("hpf100_effMCmodel_Denum"),1,1,"b");
	hpf100_effMCModel -> SetMarkerStyle(22);
	hpf100_effMCModel -> SetMarkerColor(4);

    TPpf100_effMC -> Draw();
    hpf100_effMC -> Draw("same");
    hpf100_effMCModel -> Draw("same");



    TLegend *l2 = new TLegend(0.4,0.45,0.9,0.65);
    l2 -> SetTextSize(0.04);
    l2 -> AddEntry(hpf100_effMC,"Same sign method","p");
    l2 -> AddEntry(TPpf100_effMC,"T&P","p");
    l2 -> AddEntry(hpf100_effMCModel,"MC mod","p");
    l2 ->Draw();

	can2 -> cd();

    TPad *pad22 = new TPad("pad22","pad2",0,0.0,1,0.3);
    pad22 -> SetTopMargin(0);
    pad22 -> SetBottomMargin(0.2);
    pad22 -> Draw();
    pad22 -> cd();

	TH1F *ratio = (TH1F*) hpf100_effMCModel->Clone("ratio");
    ratio -> Reset();
    ratio->Divide(hpf100_effMC,hpf100_effMCModel,1.,1.,"b");
    ratio->GetYaxis()->SetTitle("Same sight/MC Model");
    ratio -> GetYaxis() -> SetTitleSize(20);
    ratio -> GetYaxis() -> SetTitleFont(43);
    ratio -> GetYaxis() -> SetTitleOffset(1.55);
    ratio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
    ratio -> GetYaxis() -> SetLabelSize(20);
        ratio->Draw();
	/*
	
	TH1F *TPpf40_eff = (TH1F*) fD ->Get("T&Ppf40_eff");
    TPpf40_eff -> SetMarkerColor(1);
    TPpf40_eff -> SetMarkerStyle(20);
    TCanvas *canva2 = new TCanvas("canva2","",1000,800);
    TF1 *fit11 = new TF1("fit11",finaleFunction,30,500,4);
    fit11 -> SetParameters(2.48966e-01,3.70690e+01,2.48966e-01,3.70690e+01);
    fit11 -> SetLineColor(4);
    drawRatio(TPpf40_eff,fit11,canva2,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
    canva2->Draw();
	
   TH1F *hpf80_effD = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_eff");
   hpf80_effD -> SetTitle("HLT_PFJet80 100 GeV Pt trashold;p_{T}, [Gev]; #epsilon");
   hpf80_effD -> SetMarkerColor(1);
   hpf80_effD -> SetMarkerStyle(20);
   TF1 *fit1 = new TF1("fit1",finaleFunction,90,500,4);  
   fit1 -> SetParameters(0.100921,109.42,0.183585,110.153);
   fit1 -> SetLineColor(4);
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   drawRatio(hpf80_effD,fit1,canva1,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva1->Draw();
   */

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

