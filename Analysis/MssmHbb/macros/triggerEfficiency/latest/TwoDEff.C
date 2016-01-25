#include "Analysis/MssmHbb/src/HbbStyle.cc"
#include "TCut.h"
#include "TF2.h"


double twoDFit(double *x, double *par);
double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(const double &x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
void drawRatio(TH1F*, TH1F*, TCanvas*);
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );
TH1F *constructCombined(TH1F *, TFile * file,const int &,const int &);
TH1F *constructCombined(TH1F *,const TCut &, TTree * tree,const int &,const int &);

void TwoDEff()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fOldSelection = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsSingleSelectionJetHTPromt-Reco-v4.root");
	 TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/ResultsTreesMCQCD.root");
	 TFile *f = new TFile("TwoDimEff.root","RECREATE");
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
   cutPFJet80Match = "LeadMatch80[0] == 1 && LeadMatch80[1] == 1";
   cutRefRew = "weightPtRef";
   cutPfJet100Match = "LeadMatch100[0] == 1 && LeadMatch100[1] == 1";
   cutTnPRew = "weightPtTnP";
   TCut weightLumi = "weightLumi";

   float Bins[] = {0.,90.,92.5,95.,97.5,100.,105,110,115.,120.,125.,130.,135.,140.,145,150,170,200,250,300,360,420,500};
   int size = sizeof(Bins)/sizeof(float) - 1;

   TCanvas *can1 = new TCanvas("can1","Canva with 2D Plot",800,800);
   TH2F *TwoDEffTrueMC_Num = new TH2F("TwoDEffTrueMC_Num","2D Efficiency with True method",size,Bins,size,Bins);
   TwoDEffTrueMC_Num ->SetMarkerStyle(20);
   TH2F *TwoDEffTrueMC_Denum = new TH2F("TwoDEffTrueMC_Denum","2D Efficiency with True method",size,Bins,size,Bins);
   TwoDEffTrueMC_Num ->SetTitle("2D Efficiency with True method;p_{T2};p_{T1}");
   mcTree ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEffTrueMC_Num",(cut+cutPfJet100Match)*weightLumi);
   mcTree ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEffTrueMC_Denum",(cut)*weightLumi);
   TwoDEffTrueMC_Num ->Divide(TwoDEffTrueMC_Num,TwoDEffTrueMC_Denum,1,1,"b");
   TwoDEffTrueMC_Num ->Draw("E");


/*
   TCanvas *can3 = new TCanvas("can3","Canva with Histo/Fit",800,800);
   TH2F *Ratio = (TH2F*) Ratio -> Clone("TwoDEffTrueMC_Num");
   Ratio->Divide(fit);

   Ratio -> GetZaxis() -> SetRangeUser(0.,2.);
   //Ratio->Draw("LEGO");
   Ratio->Draw("E");
*/
   /*
   TCanvas *can2 = new TCanvas("can2","Symetry Canva",1000,800);
   TH2F *TwoDEffTrueMC_NumSym = new TH2F("TwoDEffTrueMC_NumSym","2D Efficiency with True method",size,Bins,size,Bins);
   TwoDEffTrueMC_NumSym ->SetMarkerStyle(20);
   TH2F *TwoDEffTrueMC_DenumSym = new TH2F("TwoDEffTrueMC_DenumSym","2D Efficiency with True method",size,Bins,size,Bins);
   TwoDEffTrueMC_NumSym ->SetTitle("2D Efficiency with True method;p_{T2};p_{T1}");
   mcTree ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEffTrueMC_NumSym",(cut+cutPfJet100Match)*weightLumi);
   mcTree ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEffTrueMC_DenumSym",(cut)*weightLumi);
   TwoDEffTrueMC_NumSym ->Divide(TwoDEffTrueMC_NumSym,TwoDEffTrueMC_DenumSym,1,1,"b");

   for(int binx = 1; binx <= size; binx++)
   {
	   for(int biny = 1; biny <= size; biny++)
	   {
		   if(binx == biny){
			   std::cout<<"bin = "<<binx<<std::endl;
			   TwoDEffTrueMC_NumSym->SetBinContent(binx,biny,0);
			   TwoDEffTrueMC_NumSym->SetBinError(binx,biny,0);
		   }
	   }
   }

   TwoDEffTrueMC_Num->Add(TwoDEffTrueMC_NumSym);
   TwoDEffTrueMC_Num->Draw("E");
   TwoDEffTrueMC_Num ->Write();

	 //Slices
   TCanvas *first4 =  new TCanvas("first4","First 4 Slices",1000,800);
   TPad *pad41 = new TPad("pad41","pad1",0,0.3,1,1);
   pad41 -> SetBottomMargin(0.05);
   pad41 -> Draw();
   pad41 -> cd();

   TH1D* bin2 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P0_{T2}, [GeV]",2,2,"e");
   bin2 -> SetMarkerColor(1);
   TH1D* bin4 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P2_{T2}, [GeV]",4,4,"e");
   bin4 -> SetMarkerColor(2);
   TH1D* bin6 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P3_{T2}, [GeV]",6,6);
   bin6 -> SetMarkerColor(4);
   TH1D* bin8 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P_{T2}, [GeV]",8,8);
   bin8 -> SetMarkerColor(3);
   gPad->SetLogy();
   bin8 ->Draw();
   bin2 ->Draw("same");
   bin4 ->Draw("same");
   bin6 ->Draw("same");

   TLegend *legP28 = new TLegend(0.4,0.45,0.65,0.65);
   legP28 -> AddEntry(bin2,"Pt1=90-92.5","p");
   legP28 -> AddEntry(bin4,"Pt1=95-97.5","p");
   legP28 -> AddEntry(bin6,"Pt1=100-105","p");
   legP28 -> AddEntry(bin8,"Pt1=110-115","p");
   legP28 -> Draw();
   first4 ->cd();

   TPad *pad42 = new TPad("pad42","pad2",0,0.0,1,0.3);
   pad42 -> SetTopMargin(0);
   pad42 -> SetBottomMargin(0.2);
   pad42 -> Draw();
   pad42 -> cd();
   TH1D *Ratio2 = (TH1D*) bin8->Clone("Ratio2");
   bin2 -> Scale(bin8->Integral()/bin2->Integral());
   Ratio2 ->Divide(bin2);
   Ratio2 -> SetMarkerColor(1);
   TH1D *Ratio4 = (TH1D*) bin8->Clone("Ratio4");
   bin4 -> Scale(bin8->Integral()/bin4->Integral());
   Ratio4 ->Divide(bin4);
   Ratio4 ->SetMarkerColor(2);
   TH1D *Ratio6 = (TH1D*) bin8->Clone("Ratio6");
   bin6 -> Scale(bin8->Integral()/bin6->Integral());
   Ratio6 ->Divide(bin6);
   Ratio6 -> SetMarkerColor(4);
   Ratio2->Draw();
   Ratio4->Draw("same");
   Ratio6->Draw("same");

   TLine *horizLine1 = new TLine(0,1,500,1);
   horizLine1 -> SetLineStyle(2);
   horizLine1 -> Draw();


   TCanvas *second4 =  new TCanvas("second4","Second 4 Slices",1000,800);
   TPad *pad421 = new TPad("pad421","pad1",0,0.3,1,1);
   pad421 -> SetBottomMargin(0.05);
   pad421 -> Draw();
   pad421 -> cd();

   TH1D* bin10 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P4_{T2}, [GeV]",10,10,"e");
   bin10 -> SetMarkerColor(1);
   TH1D* bin12 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P5_{T2}, [GeV]",12,12,"e");
   bin12 -> SetMarkerColor(2);
   TH1D* bin14 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P6_{T2}, [GeV]",14,14);
   bin14 -> SetMarkerColor(4);
   TH1D* bin16 = (TH1D*) TwoDEffTrueMC_Num -> ProjectionX("P7_{T2}, [GeV]",16,16);
   bin16 -> SetMarkerColor(3);
   gPad -> SetLogy();
   bin16 ->Draw();
   bin14 ->Draw("same");
   bin12 ->Draw("same");
   bin10 ->Draw("same");

   TLegend *legP1016 = new TLegend(0.4,0.45,0.65,0.65);
   legP1016 -> AddEntry(bin10,"Pt1=120-125","p");
   legP1016 -> AddEntry(bin12,"Pt1=135-140","p");
   legP1016 -> AddEntry(bin14,"Pt1=150-170","p");
   legP1016 -> AddEntry(bin16,"Pt1=200-250","p");
   legP1016 -> Draw();

   second4 ->cd();

   TPad *pad422 = new TPad("pad422","pad2",0,0.0,1,0.3);
   pad422 -> SetTopMargin(0);
   pad422 -> SetBottomMargin(0.2);
   pad422 -> Draw();
   pad422 -> cd();

   TH1D *Ratio10 = (TH1D*) bin16->Clone("Ratio2");
   bin10 -> Scale(bin16->Integral()/bin10->Integral());
   Ratio10 ->Divide(bin10);
   Ratio10 -> SetMarkerColor(1);
   TH1D *Ratio12 = (TH1D*) bin16->Clone("Ratio4");
   bin12 -> Scale(bin16->Integral()/bin12->Integral());
   Ratio12 ->Divide(bin12);
   Ratio12 ->SetMarkerColor(2);
   TH1D *Ratio14 = (TH1D*) bin16->Clone("Ratio6");
   bin14 -> Scale(bin16->Integral()/bin14->Integral());
   Ratio14 ->Divide(bin14);
   Ratio14 -> SetMarkerColor(4);
   Ratio10->Draw();
   Ratio12->Draw("same");
   Ratio14->Draw("same");


   horizLine1 -> Draw();

   TCanvas *canRef1 = new TCanvas("canRef1","canRef1",1000,800);
   TH2F *TwoDEffRefMC_Num = new TH2F("TwoDEffRefMC_Num","2D Efficiency with Ref method",size,Bins,size,Bins);
   TwoDEffRefMC_Num ->SetMarkerStyle(20);
   TH2F *TwoDEffRefMC_Denum = new TH2F("TwoDEffRefMC_Denum","2D Efficiency with Ref method",size,Bins,size,Bins);
   TwoDEffRefMC_Num ->SetTitle("2D Efficiency with Ref method;p_{T2};p_{T1}");
   mcTree ->Draw("hpf100_Num[0]:hpf100_Num[1]>>TwoDEffRefMC_Num",(cut+cutPfJet100Match)*weightLumi);
   mcTree ->Draw("hpf100_Denum[0]:hpf100_Denum[1]>>TwoDEffRefMC_Denum",(cut)*weightLumi);
   TwoDEffRefMC_Num ->Divide(TwoDEffRefMC_Num,TwoDEffRefMC_Denum,1,1,"b");
   TwoDEffRefMC_Num ->Draw("E");

   TCanvas *ratioRefTrue = new TCanvas("ratioRefTrue","Canva Ratio of Ref and True",1000,800);
   TwoDEffRefMC_Num -> Divide(TwoDEffTrueMC_Num);
   TwoDEffRefMC_Num -> Draw("E");

   f->Close();
   /*

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

   TCanvas *canva6MC = new TCanvas("canva6MC","True MC eff",1000,800);
   TH1F *mcModel_Num = new TH1F("mcModel_Num","True MC Method",200,0.,500.);
   mcTree ->Draw("mcModel_Num[0]>>mcModel_Num",weightLumi*cut,"E");
   TH1F *mcModel_Denum = new TH1F("mcModel_Denum","True MC Method",200,0.,500.);
   mcTree ->Draw("mcModel_Denum[0]>>mcModel_Denum",weightLumi*cut,"E");
   mcModel_Num ->Divide(mcModel_Num, mcModel_Denum,1.,1.,"b");
   mcModel_Num ->SetMarkerStyle(20);
   mcModel_Num ->SetMarkerColor(2);
   drawRatio(mcModel_Num,newTnP100mc,canva6MC);

   TCanvas *canva7MC = new TCanvas("canva7MC","True MC eff VS Ref",1000,800);
   drawRatio(mcModel_Num,hpf100mc_Num,canva7MC);

   // Fit histograms:
   TCanvas *canva5MC = new TCanvas("canva5MC","Fit to TnP Monte Carlo",1000,800);
   fit0mc -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit0mc -> SetLineColor(4);
   gPad->SetLogy();
   std::string fitName="name";
   drawRatio(newTnP100mc,fit0mc,canva5MC,fitName);

   TCanvas *canva61MC = new TCanvas("canva61MC","Fit to Ref Method MC",1000,800);
   fit0mc->SetParameters(8.84158e-02,1.00909e+02,1.82035e-01,9.98952e+01);
   drawRatio(hpf100mc_Num,fit0mc,canva61MC,fitName);

   TCanvas *canva8MC = new TCanvas("canva8MC","True MC eff VS Ref DENUM",1000,800);
   mcModel_Denum ->SetMarkerStyle(20);
   hpf100mc_Denum ->SetMarkerStyle(21);
   hpf100mc_Denum ->SetMarkerColor(2);
   drawRatio(mcModel_Denum,hpf100mc_Denum,canva8MC);
   printf("NRef = %.7f  Ntrue = %.7f",hpf100mc_Denum->GetEntries(),mcModel_Denum->GetEntries());
   */

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

double twoDFit(double *x, double *par){
	double result = 0;
	double *p1 = &par[0];
	double *p2 = &par[4];
	result = finaleFunction(x[0],p1) * finaleFunction(x[1],p2);

	return result;
}

double finaleFunction(const double &x, double *par)
{

	double sigmoid = 1./(1.+exp( -par[0] * (x - par[1]) ));
	double erf1 = 1 / 2. * (1 + par[0]*TMath::Erf((x-par[1])/(sqrt(2)*par[2])));
	double erf2 = 1 / 2. * (1 + par[3]*TMath::Erf((x-par[4])/(sqrt(2)*par[5])));
	double erfc = 1+ TMath::Erfc((par[5]-x)/(sqrt(2)*par[6]));
	double result =  0.5 * (1. + TMath::Erf((par[2]*x-par[1])/(sqrt(2)*par[0])));
	double sigmoid2 = 1./ (1.+exp( -par[2] * (x - par[3]) ));
	return sigmoid * sigmoid2;
	//return erf1 * erf2;

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
