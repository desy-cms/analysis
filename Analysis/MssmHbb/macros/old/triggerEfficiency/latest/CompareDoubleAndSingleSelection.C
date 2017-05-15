#include "Analysis/MssmHbb/src/HbbStyle.cc"
#include "TCut.h"

double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
void drawRatio(TH1F*, TH1F*, TCanvas*);
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );
TH1F *constructCombined(TH1F *, TFile * file,const int &,const int &);
TH1F *constructCombined(TH1F *,const TCut &, TTree * tree,const int &,const int &);

void CompareDoubleAndSingleSelection()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fSingleMCSelection = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsSingleSelectionMC.root");
	 TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/ResultsTreesMCQCD.root");
   //TFile * fDoubleSelection = new TFile("/nfs/dust/cms/user/shevchen/output/ResultsTreesJetHTPromt-Reco-v4.root");//ResultsJetHT2015DPromptReco4.root");
   TTree *dataTree, *mcTree;
   //fDoubleSelection->GetObject("OutTree",dataTree);
	 fMonteCarlo -> GetObject("OutTree",mcTree);
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //..............................Pt efficiency....................
   int bin40to60 = 90;
   int bin60to80 = 115;
   TCut cut = "dPhiFS>2.7";// && Njets == 2 && NL1Object >= 2";//"ptVeto < 10 && ((fLeadPt - sLeadPt)/(fLeadPt + sLeadPt) <= 0.2) && abs(fLeadEta) <= 2. && abs(sLeadEta)<=2";
   TCut cutPFJet80Match, cutRefRew, cutPfJet100Match, cutTnPRew;
   cutPFJet80Match = "LeadMatch80[0] == true && LeadMatch80[1] == true";
   cutRefRew = "weightPtRef";
   cutPfJet100Match = "LeadMatch100[0] == true && LeadMatch100[1] == true";
   cutTnPRew = "weightPtTnP";
   TCut weightLumi = "weightLumi";
   /*
	 //Monte Carlo

   TCanvas *canva00 = new TCanvas("canva00","",1000,800);
   TH1F *newTnP100mc = new TH1F("newTnP100mc","New T&P Method MC",200,0.,500.);
   TH1F *newTnP100mc_Num = new TH1F("newTnP100mc_Num","New T&P Method MC",200,0.,500.);
   mcTree ->Draw("TnP100_probe[0]>>newTnP100mc_Num",weightLumi*cut,"E");
   TH1F *newTnP100mc_Denum = new TH1F("newTnP100mc_Denum","New T&P Method MC",200,0.,500.);
   mcTree ->Draw("TnP100_tag[0]>>newTnP100mc_Denum",weightLumi*cut);
   newTnP100mc ->Divide(newTnP100mc_Num,newTnP100mc_Denum,1,1,"b");
   newTnP100mc ->SetMarkerStyle(21);
   newTnP100mc->Draw("E");

	 TH1F *newCombination100mc = new TH1F("newCombination100mc","Combination method MC",200,0.,500);
   constructCombined(newCombination100mc,weightLumi*cut,mcTree,bin40to60,bin60to80);
   newCombination100mc ->SetMarkerStyle(20);
   newCombination100mc ->SetMarkerColor(2);
   newCombination100mc -> Draw("E");
   newTnP100mc->Draw("E same");

	 TCanvas *canva0 = new TCanvas("canva0","Monte Carlo T&P vs Comb",1000,800);
	 gPad->SetLogy();
	 drawRatio(newCombination100mc,newTnP100mc,canva0);


	 TCanvas *canvaMC1 = new TCanvas("canvaMC1","Reference vs T&P",1000,800);
	 TH1F *hpf100mc_Num = new TH1F("hpf100mc_Num","Reference Method with PFJet80 Monte Carlo",200,0.,500.);
   mcTree ->Draw("hpf100_Num[0]>>hpf100mc_Num",weightLumi*cut,"E");
   TH1F *hpf100mc_Denum = new TH1F("hpf100mc_Denum","Reference Method with PFJet80 Monte Carlo",200,0.,500.);
   mcTree ->Draw("hpf100_Denum[0]>>hpf100mc_Denum",weightLumi*cut,"E");
   hpf100mc_Num ->Divide(hpf100mc_Num, hpf100mc_Denum,1.,1.,"b");
   hpf100mc_Num ->SetMarkerStyle(20);
   hpf100mc_Num ->SetMarkerColor(2);
   hpf100mc_Num ->Draw("E");
   TF1 *fit0mc = new TF1("fit0mc",finaleFunction,90,500,4);
   fit0mc -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit0mc -> SetLineColor(4);
   hpf100mc_Num ->Fit("fit0mc");
   gPad->SetLogy();
   drawRatio(hpf100mc_Num,newTnP100mc,canvaMC1);
 */
   TCanvas *canva1MC = new TCanvas("canva1MC","True MC eff",1000,800);
   TH1F *mcModel_Num = new TH1F("mcModel_Num","True MC J1 Double Selection",200,0.,500.);
   mcTree ->Draw("mcModel_Num[0]>>mcModel_Num",weightLumi*cut,"E");
   TH1F *mcModel_Denum = new TH1F("mcModel_Denum","True MC Method",200,0.,500.);
   mcTree ->Draw("mcModel_Denum[0]>>mcModel_Denum",weightLumi*cut,"E");
   mcModel_Num ->Divide(mcModel_Num, mcModel_Denum,1.,1.,"b");
   mcModel_Num ->SetMarkerStyle(20);
   mcModel_Num ->SetMarkerColor(2);

   //Second L jet
   TH1F *mcModel_Single_Num = (TH1F*) fSingleMCSelection ->Get("hpf100_effMCmodel_Num");
   mcModel_Single_Num ->SetTitle("True MC Single Selection");
   TH1F *mcModel_Single_Denum = (TH1F*) fSingleMCSelection ->Get("hpf100_effMCmodel_Denum");
   mcModel_Single_Num ->Divide(mcModel_Single_Num,mcModel_Single_Denum,1,1,"b");
   mcModel_Single_Num ->SetMarkerColor(1);
   mcModel_Single_Num ->SetMarkerStyle(21);
   drawRatio(mcModel_Num,mcModel_Single_Num,canva1MC);

}


TH1F *constructCombined(TH1F *CombinedH,const TCut &cut, TTree *tree,const int & bin40to60,const int &bin60to80){

	CombinedH -> Reset();
	TH1::SetDefaultSumw2();

	TH1F *h_Num = new TH1F("h_Num","",200,0.,500);
	TH1F *h_Denum = new TH1F("h_Denum","",200,0.,500);

	tree->Draw("hpf60for100_Num>>h_Num",cut);
	tree->Draw("hpf60for100_Denum>>h_Denum",cut);
	h_Num ->Divide(h_Num,h_Denum,1,1,"b");
	TH1F *HLT60 = (TH1F*) h_Num -> Clone("HLT60");

	tree->Draw("hpf100_Num>>h_Num",cut);
	tree->Draw("hpf100_Denum>>h_Denum",cut);
	h_Num ->Divide(h_Num,h_Denum,1,1,"b");
	TH1F *HLT80 =(TH1F*) h_Num -> Clone("HLT80");

    int xlow90 = HLT60 -> FindBin(bin40to60 + 2);
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
