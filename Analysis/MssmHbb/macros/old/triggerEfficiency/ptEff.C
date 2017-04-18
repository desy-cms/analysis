#include "Analysis/MssmHbb/src/HbbStyle.cc"

double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );

void ptEff()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015C.root");
   TFile * fD = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsJetHTPromt-Reco-v4.root");
   TFile * fMC = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDEfficiencyStart50.root");//ResultsJetHT2015DPromptReco4.root");
   /*TFileMerger merger;
   merger.AddFile(fC);
   merger.AddFile(fD);
   merger.OutputFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsjetHTRun2015C+D.root");
   merger.Merge();
   */
   HbbStyle style;
   style.set(PUBLIC);

   //..............................Pt efficiency....................
   //PFJet80 Pt trashold 100

   TH1F *hpf80_effD = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_eff");
   hpf80_effD -> SetTitle("Kinematic turn-on for 100 GeV Pt trashold;p_{T}, [Gev]; #epsilon");
   hpf80_effD -> SetMarkerColor(1);
   hpf80_effD -> SetMarkerStyle(20);
   hpf80_effD -> Draw();
   TF1 *fit1 = new TF1("fit1",finaleFunction,90,500,4);
   fit1 -> SetParameters(0.100921,109.42,0.183585,110.153);
   //fit1 -> SetLineColor(kGray+3);
   //hpf80_effD -> Fit("fit1","R");
   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   drawRatio(hpf80_effD,fit1,canva1,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");


   TH1F *hpf80_effD160 = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_eff160");
   hpf80_effD160 -> SetMarkerColor(2);
   hpf80_effD160 -> SetMarkerStyle(21);
   hpf80_effD160 -> Draw("same");
    TF1 *fit2 = new TF1("fit2",finaleFunction,150,500,4);
   fit2 -> SetParameters(0.100921,109.42,0.183585,110.153);
   fit2 -> SetLineColor(kRed+1);
   hpf80_effD160 -> Fit("fit2","R+");

   char name[200];
   sprintf(name,"100 GeV treshold Fit: #chi^2 / ndf = %.2f / %i",fit1->GetChisquare(),fit1->GetNDF());

   TLegend *leg = new TLegend(0.5,0.45,0.9,0.65);
   leg -> AddEntry(hpf80_effD,"p_{T} treshold = 100 GeV","pl");
   leg -> AddEntry(fit1,name,"l");
   leg -> AddEntry(hpf80_effD160,"p_{T} treshold = 160 GeV","pl");
   sprintf(name,"160 GeV treshold Fit: #chi^2 / ndf = %.2f / %i",fit2->GetChisquare(),fit2->GetNDF());
   leg -> AddEntry(fit2,name,"l");
   leg -> Draw();
   /*
   TH1F *hpf80_eff_dEta = (TH1F*) fD -> Get("HLT_PFJet80/hpf80_dEta_eff");
   hpf80_eff_dEta ->  SetTitle("#Delta #eta turn-on of low-mass trigger;#Delta#eta;#epsilon");
   hpf80_eff_dEta -> SetMarkerColor(1);
   hpf80_eff_dEta -> SetMarkerStyle(20);
   TF1 *fit3 = new TF1("fit3",finaleFunction,0.,1.65,4);
   fit3 -> SetParameters(-1.15698e+02,1.59862e+00,9.93479e-02,-4.49790e+01);
   fit3 -> SetLineColor(4);
   TCanvas *canva3 = new TCanvas("canva3","",1000,800);
   drawRatio(hpf80_eff_dEta,fit3,canva3,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva3 -> Draw();


   /*
   hpf80_effD = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_effEtaL2p5");




   hpf80_effD -> SetTitle("HLT_PFJet80 100 GeV Pt trashold, #eta < 2.5 ;p_{T}, [Gev]; #epsilon");
   hpf80_effD -> SetMarkerColor(1);
   hpf80_effD -> SetMarkerStyle(20);
   TF1 *fit11 = new TF1("fit11",finaleFunction,90,500,4);
   fit11 -> SetParameters(0.100921,109.42,0.183585,110.153);
   fit11 -> SetLineColor(4);
   TCanvas *canva11 = new TCanvas("canva11","",1000,800);
   drawRatio(hpf80_effD,fit11,canva11,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva11->Draw();

   hpf80_effD = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_effEtaG2p5");
   hpf80_effD -> SetTitle("HLT_PFJet80 100 GeV Pt trashold, #eta > 2.5 ;p_{T}, [Gev]; #epsilon");
   hpf80_effD -> SetMarkerColor(1);
   hpf80_effD -> SetMarkerStyle(20);
   TF1 *fit12 = new TF1("fit12",finaleFunction,90,500,4);
   fit12 -> SetParameters(0.100921,109.42,0.183585,110.153);
   fit12 -> SetLineColor(4);
   TCanvas *canva12 = new TCanvas("canva12","",1000,800);
   drawRatio(hpf80_effD,fit12,canva12,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva12->Draw();

   //PFJet80 Pt trashold 160
   TH1F *hpf80_effD160 = (TH1F*) fD ->Get("HLT_PFJet80/hpf80_eff160");
   hpf80_effD160 -> SetTitle("HLT_PFJet80 160 GeV Pt trashold;p_{T}, [Gev]; #epsilon");
   hpf80_effD160 -> SetMarkerColor(1);
   hpf80_effD160 -> SetMarkerStyle(20);
   TF1 *fit2 = new TF1("fit2",finaleFunction,90,500,4);
   fit2 -> SetParameters(9.70248e-02,-1.08960e+02,1.36587e-01,1.74438e+02);
   fit2 -> SetLineColor(4);
   TCanvas *canva2 = new TCanvas("canva2","",1000,800);
   drawRatio(hpf80_effD160,fit2,canva2,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva2->Draw();



   //dEta
   TH1F *hpf80_eff_dEta = (TH1F*) fD -> Get("HLT_PFJet80/hpf80_dEta_eff");
   hpf80_eff_dEta ->  SetTitle("HLT_PFJet80 100 GeV Pt treshold, dEta<1.6;#Delta#eta;#epsilon");
   hpf80_eff_dEta -> SetMarkerColor(1);
   hpf80_eff_dEta -> SetMarkerStyle(20);
   TF1 *fit3 = new TF1("fit3",finaleFunction,0.,1.65,4);
   fit3 -> SetParameters(-1.15698e+02,1.59862e+00,9.93479e-02,-4.49790e+01);
   fit3 -> SetLineColor(4);
   TCanvas *canva3 = new TCanvas("canva3","",1000,800);
   drawRatio(hpf80_eff_dEta,fit3,canva3,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canva3 -> Draw();
   //hpf80_eff_dEta -> Draw();
/*
	// FOR MC
	TF1 *dataFit = (TF1*) fit1 ->Clone("dataFit");
	TH1F *num = (TH1F*) fMC -> Get("HLT_PFJet80/hpf80_effNumerator");
   TH1F *denum = (TH1F*) fMC -> Get("HLT_PFJet80/hpf80_effDenumerator");

   TH1F *hpf80_effMC = (TH1F*) fMC ->Get("HLT_PFJet80/hpf80_eff");
   hpf80_effMC -> Divide(num,denum,1,1,"b");
   hpf80_effMC -> SetTitle("HLT_PFJet80 100 GeV Pt trashold;; #epsilon");
   hpf80_effMC -> SetMarkerColor(1);
   hpf80_effMC -> SetMarkerStyle(20);
   TF1 *fitMC = new TF1("fitMC",finaleFunction,90,500,4);
   fitMC -> SetParameters(0.100921,109.42,0.183585,110.153);
   fitMC -> SetLineColor(4);

   TCanvas *canvaMC1 = new TCanvas("canvaMC1","",1000,800);
   drawRatio(hpf80_effMC,fitMC, canvaMC1,"0.5 #upoint (1. + Erf((x-p0)/(#sqrt{2}p1)))");
   canvaMC1->Draw();

   TCanvas *canvaMC = new TCanvas("canvaMC","",1000,800);
   drawRatio(hpf80_effMC,fitMC,dataFit, hpf80_effD,  canvaMC,"#frac{1}{1+exp( -p0 #upoint (x - p1))} #upoint #frac{1}{1+exp( -p2 #upoint (x - p3))}");
   canvaMC->Draw();
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
   //fitLegend->Draw();
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
