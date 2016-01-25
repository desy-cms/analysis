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

void branches()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fOldSelection = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsSingleSelectionJetHTPromt-Reco-v4.root");
	 TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/ResultsTreesMCQCD.root");
   TFile * fDoubleSelection = new TFile("/nfs/dust/cms/user/shevchen/output/ResultsTreesJetHTFull.root");//ResultsJetHT2015DPromptReco4.root");
   TTree *dataTree, *mcTree;
   fDoubleSelection->GetObject("OutTree",dataTree);
	 fMonteCarlo -> GetObject("OutTree",mcTree);
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //..............................Pt efficiency....................
   int bin40to60 = 90;
   int bin60to80 = 115;
   TCut cut = "dPhiFS > 2.7";//"ptVeto < 10 && ((fLeadPt - sLeadPt)/(fLeadPt + sLeadPt) <= 0.2) && abs(fLeadEta) <= 2. && abs(sLeadEta)<=2";

   // Tag and probe with new selection
   TH1F *newTnP100 = new TH1F("newTnP100","New T&P Method",200,0.,500.);
   TH1F *newTnP100_Num = new TH1F("newTnP100_Num","New T&P Method",200,0.,500.);
   dataTree ->Draw("TnP100_probe[0]>>newTnP100_Num",cut,"E");
   TH1F *newTnP100_Denum = new TH1F("newTnP100_Denum","New T&P Method",200,0.,500.);
   dataTree ->Draw("TnP100_tag[0]>>newTnP100_Denum",cut);
   newTnP100 ->Divide(newTnP100_Num,newTnP100_Denum,1,1,"b");
   newTnP100 ->SetMarkerStyle(21);
   newTnP100->Draw("E");

   TCanvas *canva3 = new TCanvas("canva3","",1000,800);
   TH1F *hpf100_Num = new TH1F("hpf100_Num","Reference Method with PFJet80",200,0.,500.);
   dataTree ->Draw("hpf100_Num[0]>>hpf100_Num",cut,"E");
   TH1F *hpf100_Denum = new TH1F("hpf100_Denum","",200,0.,500.);
   dataTree ->Draw("hpf100_Denum[0]>>hpf100_Denum",cut,"E");
   hpf100_Num ->Divide(hpf100_Num, hpf100_Denum,1.,1.,"b");
   hpf100_Num ->SetMarkerStyle(20);
   hpf100_Num ->SetMarkerColor(2);
   hpf100_Num ->Draw("E");
   TF1 *fit0 = new TF1("fit0",finaleFunction,30,500,4);
   fit0 -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit0 -> SetLineColor(4);
   hpf100_Num ->Fit("fit0");
   drawRatio(hpf100_Num,newTnP100,canva3);
   std::string fitName;

   // Fit histograms:
 /*  TCanvas *canva5 = new TCanvas("canva5","Fit to TnP",1000,800);
   fit0 -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit0 -> SetLineColor(4);
   drawRatio(newTnP100,fit0,canva5); */

   //Comparison of the Data/Data distribution
   TCut cutPFJet80Match, cutRefRew, cutPfJet100Match, cutTnPRew;
   cutPFJet80Match = "fLeadMatch80 == 1 && sLeadMatch80 == 1";
   cutRefRew = "weightPtRef";
   cutPfJet100Match = "fLeadMatch100 == 1 && sLeadMatch100 == 1";
   cutTnPRew = "weightPtTnP";

   // Pt of the first Leading jet
   TCanvas *can1 = new TCanvas("can1","First Leading Pt Ref/Trig",1000,800);
   TH1F *pt1RefRew = new TH1F("pt1RefRew","Pt1 with Ref Rew",100,0.,1000.);
   pt1RefRew->SetMarkerStyle(21);
   pt1RefRew->SetMarkerColor(1);
   dataTree ->Draw("LeadPt[0] >> pt1RefRew",cutPFJet80Match*cutRefRew,"E");
   TH1F *pt1PF100Match = new TH1F("pt1PF100Match","Pt1 with PFJet100 matching",100,0.,1000.);
   pt1PF100Match ->SetMarkerStyle(20);
   pt1PF100Match ->SetMarkerColor(2);
   dataTree->Draw("LeadPt[0] >> pt1PF100Match",cutPfJet100Match,"E same");
   gPad->SetLogy();
   drawRatio(pt1RefRew,pt1PF100Match,can1);

   TCanvas *can2 = new TCanvas("can2","First Leading Pt TnP/Trig",1000,800);
   TH1F *pt1TnPRew = new TH1F("pt1TnPRew","Pt1 with TnP Rew",100,0.,1000.);
   pt1TnPRew->SetMarkerStyle(21);
   pt1TnPRew->SetMarkerColor(1);
   dataTree ->Draw("LeadPt[0] >> pt1TnPRew",cutPFJet80Match*cutTnPRew,"E");
   gPad->SetLogy();
   drawRatio(pt1TnPRew,pt1PF100Match,can2);

   // Pt of the Second Leading jet
   TCanvas *can12 = new TCanvas("can12","Second Leading Pt Ref/Trig",1000,800);
   TH1F *pt2RefRew = new TH1F("pt2RefRew","Pt2 with Ref Rew",100,0.,1000.);
   pt2RefRew->SetMarkerStyle(21);
   pt2RefRew->SetMarkerColor(1);
   dataTree ->Draw("LeadPt[1] >> pt2RefRew",cutPFJet80Match*cutRefRew,"E");
   TH1F *pt2PF100Match = new TH1F("pt2PF100Match","Pt2 with PFJet100 matching",100,0.,1000.);
   pt2PF100Match ->SetMarkerStyle(20);
   pt2PF100Match ->SetMarkerColor(2);
   dataTree->Draw("LeadPt[1] >> pt2PF100Match",cutPfJet100Match,"E same");
   gPad->SetLogy();
   drawRatio(pt2RefRew,pt2PF100Match,can12);

   TCanvas *can22 = new TCanvas("can22","Second Leading Pt TnP/Trig",1000,800);
   TH1F *pt2TnPRew = new TH1F("pt2TnPRew","Pt2 with TnP Rew",100,0.,1000.);
   pt2TnPRew->SetMarkerStyle(21);
   pt2TnPRew->SetMarkerColor(1);
   dataTree ->Draw("LeadPt[1] >> pt2TnPRew",cutPFJet80Match*cutTnPRew,"E");
   gPad->SetLogy();
   drawRatio(pt2TnPRew,pt2PF100Match,can22);


	 //Monte Carlo
	 TCut weightLumi = "weightLumi";
	 TCanvas *canva00 = new TCanvas("canva00","",1000,800);
	 TH1F *newTnP100mc = new TH1F("newTnP100mc","New T&P Method MC",200,0.,500.);
   TH1F *newTnP100mc_Num = new TH1F("newTnP100mc_Num","New T&P Method MC",200,0.,500.);
   mcTree ->Draw("TnP100_probe>>newTnP100mc_Num",weightLumi,"E");
   TH1F *newTnP100mc_Denum = new TH1F("newTnP100mc_Denum","New T&P Method MC",200,0.,500.);
   mcTree ->Draw("TnP100_tag>>newTnP100mc_Denum",weightLumi);
   newTnP100mc ->Divide(newTnP100mc_Num,newTnP100mc_Denum,1,1,"b");
   newTnP100mc ->SetMarkerStyle(21);
   newTnP100mc->Draw("E");

	 TH1F *newCombination100mc = new TH1F("newCombination100mc","Combination method MC",200,0.,500);
   constructCombined(newCombination100mc,weightLumi,mcTree,bin40to60,bin60to80);
   newCombination100mc ->SetMarkerStyle(20);
   newCombination100mc ->SetMarkerColor(2);
   newCombination100mc -> Draw("E");
   newTnP100mc->Draw("E same");

	 TCanvas *canva0 = new TCanvas("canva0","Monte Carlo T&P vs Comb",1000,800);
	 gPad->SetLogy();
	 drawRatio(newTnP100mc,newCombination100mc,canva0);

	 TCanvas *canvaMC1 = new TCanvas("canvaMC1","Reference vs T&P",1000,800);
	 TH1F *hpf100mc_Num = new TH1F("hpf100mc_Num","Reference Method with PFJet80 Monte Carlo",200,0.,500.);
   mcTree ->Draw("hpf100_Num>>hpf100mc_Num",weightLumi,"E");
   TH1F *hpf100mc_Denum = new TH1F("hpf100mc_Denum","",200,0.,500.);
   mcTree ->Draw("hpf100_Denum>>hpf100mc_Denum",weightLumi,"E");
   hpf100mc_Num ->Divide(hpf100mc_Num, hpf100mc_Denum,1.,1.,"b");
   hpf100mc_Num ->SetMarkerStyle(20);
   hpf100mc_Num ->SetMarkerColor(2);
   hpf100mc_Num ->Draw("E");
   TF1 *fit0mc = new TF1("fit0mc",finaleFunction,30,500,4);
   fit0mc -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit0mc -> SetLineColor(4);
   hpf100mc_Num ->Fit("fit0mc");
   gPad->SetLogy();
   drawRatio(hpf100mc_Num,newTnP100mc,canvaMC1);

   // Fit histograms:

   TCanvas *canva4MC = new TCanvas("canva4MC","Fit to Combination Monte Carlo",1000,800);
   TF1 *fit1MC = new TF1("fit1MC",finaleFunction,30,500,4);
   fit1MC -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit1MC -> SetLineColor(4);
   fitName = "#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}";
   drawRatio(newCombination100mc,fit1MC,canva4MC,fitName);

   TCanvas *canva5MC = new TCanvas("canva5MC","Fit to TnP Monte Carlo",1000,800);
   fit1MC -> SetParameters(1.06207e-01,1.06026e+02,2.87048e-01,1.00032e+02);
   fit1MC -> SetLineColor(4);
   gPad->SetLogy();
   drawRatio(newTnP100mc,fit1MC,canva5MC,fitName);

   TCanvas *canva6MC = new TCanvas("canva6MC","True MC eff",1000,800);
   TH1F *mcModel_Num = new TH1F("mcModel_Num","True MC Method",200,0.,500.);
   mcTree ->Draw("mcModel_Num>>mcModel_Num",weightLumi,"E");
   TH1F *mcModel_Denum = new TH1F("mcModel_Denum","True MC Method",200,0.,500.);
   mcTree ->Draw("mcModel_Denum>>mcModel_Denum",weightLumi,"E");
   mcModel_Num ->Divide(mcModel_Num, mcModel_Denum,1.,1.,"b");
   mcModel_Num ->SetMarkerStyle(20);
   mcModel_Num ->SetMarkerColor(2);
   drawRatio(mcModel_Num,newTnP100mc,canva6MC);

   TCanvas *canva7MC = new TCanvas("canva7MC","True MC eff VS Ref",1000,800);
   drawRatio(mcModel_Num,hpf100mc_Num,canva7MC);

   //Comparison of the MC/MC distribution
   // Pt of the first Leading jet
   TCanvas *can1mc = new TCanvas("can1mc","First Leading Pt Comb/Trig Monte Carlo",1000,800);
   TH1F *pt1RefRewMC = new TH1F("pt1RefRewMC","Pt1 with Reference Rew MC",100,0.,1000.);
   pt1RefRewMC->SetMarkerStyle(21);
   pt1RefRewMC->SetMarkerColor(1);
   mcTree ->Draw("fLeadPt >> pt1RefRewMC",cutPFJet80Match*cutRefRew*weightLumi,"E");
   TH1F *pt1PF100MatchMC = new TH1F("pt1PF100MatchMC","Pt1 with PFJet100 matching MC",100,0.,1000.);
   pt1PF100MatchMC ->SetMarkerStyle(20);
   pt1PF100MatchMC ->SetMarkerColor(2);
   mcTree->Draw("fLeadPt >> pt1PF100MatchMC",cutPfJet100Match*weightLumi,"E same");
   gPad->SetLogy();
   drawRatio(pt1RefRewMC,pt1PF100MatchMC,can1mc);

   TCanvas *can2mc = new TCanvas("can2mc","First Leading Pt TnP/Trig Monte Carlo",1000,800);
   TH1F *pt1TnPRewMC = new TH1F("pt1TnPRewMC","Pt1 with TnP Rew Monte Carlo",100,0.,1000.);
   pt1TnPRewMC->SetMarkerStyle(21);
   pt1TnPRewMC->SetMarkerColor(1);
   mcTree ->Draw("fLeadPt >> pt1TnPRewMC",cutPFJet80Match*cutTnPRew*weightLumi,"E");
   gPad->SetLogy();
   drawRatio(pt1TnPRewMC,pt1PF100MatchMC,can2mc);

   // Pt of the Second Leading jet
   TCanvas *can12mc = new TCanvas("can12mc","Second Leading Pt Ref/Trig Monte Carlo",1000,800);
   TH1F *pt2RefRewMC = new TH1F("pt2RefRewMC","Pt2 with Ref Rew Monte Carlo",100,0.,1000.);
   pt2RefRewMC->SetMarkerStyle(21);
   pt2RefRewMC->SetMarkerColor(1);
   mcTree ->Draw("sLeadPt >> pt2RefRewMC",cutPFJet80Match*cutRefRew*weightLumi,"E");
   TH1F *pt2PF100MatchMC = new TH1F("pt2PF100MatchMC","Pt2 with PFJet100 matching Monte Carlo",100,0.,1000.);
   pt2PF100MatchMC ->SetMarkerStyle(20);
   pt2PF100MatchMC ->SetMarkerColor(2);
   mcTree->Draw("sLeadPt >> pt2PF100MatchMC",cutPfJet100Match*weightLumi,"E same");
   gPad->SetLogy();
   drawRatio(pt2RefRewMC,pt2PF100MatchMC,can12mc);

   TCanvas *can22mc = new TCanvas("can22mc","Second Leading Pt TnP/Trig Monte Carlo",1000,800);
   TH1F *pt2TnPRewMC = new TH1F("pt2TnPRewMC","Pt2 with TnP Rew Monte Carlo",100,0.,1000.);
   pt2TnPRewMC->SetMarkerStyle(21);
   pt2TnPRewMC->SetMarkerColor(1);
   mcTree ->Draw("sLeadPt >> pt2TnPRewMC",cutPFJet80Match*cutTnPRew*weightLumi,"E");
   gPad->SetLogy();
   drawRatio(pt2TnPRewMC,pt2PF100MatchMC,can22mc);


   /*
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
