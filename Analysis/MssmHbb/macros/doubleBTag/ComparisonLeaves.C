#include "Analysis/Drawer/src/HbbStyle.cc"
#include "Analysis/Drawer/src/RatioPlots.cpp"
#include "Analysis/Drawer/src/SystematicErrorsCalculator.cpp"
#include "TCut.h"
#include "TGraphAsymmErrors.h"

double M12FitFunction(double *x, double *par);

void ComparisonLeaves()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionQCD_180116.root");

   TTree *dataTree, *mcTree;
   fMonteCarlo -> GetObject("MssmHbb",mcTree);
   fData -> GetObject("MssmHbb",dataTree);

   //Setup style
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PUBLIC);
   ratio->SetRatioTitle("Data / MC"); // Default Title for Ratio plot Y-axis
   ratio->SetRatioRange(0.2,1.8);	// Default Y-axis range for Ratio plot

   //Declare SystematicCalc object
   SystematicErrorsCalculator *systCalc = new SystematicErrorsCalculator();

   //TCut section
   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   TCut btagSFup = "BTagSFup[0] * BTagSFup[1]";
   TCut btagSFdown = "BTagSFdown[0] * BTagSFdown[1]";

   //Start drawing
   //..............................Pt1 ....................
   TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

   const int NumberOfPtBins = 24;
   double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

   //Main Histograms
   TH1F *fLeadPtMC = new TH1F("fLeadPtMC","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *fLeadPtMCSyst = new TH1F("fLeadPtMCSyst","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *fLeadPtData = new TH1F("fLeadPtData",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   fLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[0]>>fLeadPtMCSyst",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[0]>>fLeadPtData");

   //Systematic errors
   // Syst. for SFb
   TH1F *fLeadPtSystSFbUp = new TH1F("fLeadPtSystSFbUp","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *fLeadPtSystSFbCentral = new TH1F("fLeadPtSystSFbCentral","Systematic errors central",NumberOfPtBins,PtBins);
   TH1F *fLeadPtSystSFbDown = new TH1F("fLeadPtSystSFbDown","Systematic errors down",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[0]>>fLeadPtSystSFbUp",weightLumi*weightBTag*weightPt*weightdEta*btagSFup,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtSystSFbCentral",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtSystSFbDown",weightLumi*weightBTag*weightPt*weightdEta*btagSFdown,"E");

   //Syst. for 2D Pt trigger efficiency
   TH1F *fLeadPtSyst2DError = new TH1F("fLeadPtSyst2DError","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *fLeadPtSyst2DCentral = new TH1F("fLeadPtSyst2DCentral","Systematic errors central",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[0]>>fLeadPtSyst2DError",weightLumi*weightBTag*weightdEta*btagSFcentral*ptSystWeight,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtSyst2DCentral",weightLumi*weightBTag*weightdEta*btagSFcentral*weightPt,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   fLeadPtData->SetBinContent(i,fLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtData->SetBinError(i,fLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMC->SetBinContent(i,fLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMC->SetBinError(i,fLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMCSyst->SetBinContent(i,fLeadPtMCSyst->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMCSyst->SetBinError(i,fLeadPtMCSyst->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtSystSFbUp->SetBinContent(i,fLeadPtSystSFbUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtSystSFbCentral->SetBinContent(i,fLeadPtSystSFbCentral->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtSystSFbDown->SetBinContent(i,fLeadPtSystSFbDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtSyst2DError->SetBinContent(i,fLeadPtSyst2DError->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtSyst2DCentral->SetBinContent(i,fLeadPtSyst2DCentral->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
   }

   systCalc->CalculateSingleSystError(fLeadPtSyst2DCentral,fLeadPtSyst2DError);
   systCalc->CalculateSingleSystError(fLeadPtSystSFbCentral,fLeadPtSystSFbUp,fLeadPtSystSFbDown);
   systCalc->AddUncorelatedSystErrors(fLeadPtMCSyst);

   TLegend *leg_pt1 = new TLegend(0.55,0.75,0.8,0.9);
   leg_pt1->AddEntry(fLeadPtData,"Run2015D-Prompt-Reco-v4","p");
   leg_pt1->AddEntry(fLeadPtMC,"QCD 13 TeV pythia 8","p");

//   TH1F *ratioPt = (TH1F*) fLeadPtData->Clone("ratioPt");

   TH1F *ratioPt = ratio->DrawRatio(fLeadPtData,fLeadPtMC,fLeadPtMCSyst,leg_pt1,canva00);
   ratio->GetTopPad()->SetLogy();
   ratioPt->GetXaxis()->SetRangeUser(0.,1000.);
   ratio->DrawPhaseSpaceDescription(100.,0.1,400.,2.);




   //..............................Pt2 ....................
   TCanvas *canva01 = new TCanvas("canva01","Pt2",1000,800);

   //Main Histograms
   TH1F *sLeadPtMC = new TH1F("sLeadPtMC","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *sLeadPtMCSyst = new TH1F("sLeadPtMCSyst","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *sLeadPtData = new TH1F("sLeadPtData",";sub-leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   sLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[1]>>sLeadPtMCSyst",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[1]>>sLeadPtData");

   //Systematic errors
   // Syst. for SFb
   TH1F *sLeadPtSystSFbUp = new TH1F("sLeadPtSystSFbUp","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *sLeadPtSystSFbCentral = new TH1F("sLeadPtSystSFbCentral","Systematic errors central",NumberOfPtBins,PtBins);
   TH1F *sLeadPtSystSFbDown = new TH1F("sLeadPtSystSFbDown","Systematic errors down",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[1]>>sLeadPtSystSFbUp",weightLumi*weightBTag*weightPt*weightdEta*btagSFup,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtSystSFbCentral",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtSystSFbDown",weightLumi*weightBTag*weightPt*weightdEta*btagSFdown,"E");

   //Syst. for 2D Pt trigger efficiency
   TH1F *sLeadPtSyst2DError = new TH1F("sLeadPtSyst2DError","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *sLeadPtSyst2DCentral = new TH1F("sLeadPtSyst2DCentral","Systematic errors central",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[1]>>sLeadPtSyst2DError",weightLumi*weightBTag*weightdEta*btagSFcentral*ptSystWeight,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtSyst2DCentral",weightLumi*weightBTag*weightdEta*btagSFcentral*weightPt,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   sLeadPtData->SetBinContent(i,sLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtData->SetBinError(i,sLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMC->SetBinContent(i,sLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMC->SetBinError(i,sLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMCSyst->SetBinContent(i,sLeadPtMCSyst->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMCSyst->SetBinError(i,sLeadPtMCSyst->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtSystSFbUp->SetBinContent(i,sLeadPtSystSFbUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtSystSFbCentral->SetBinContent(i,sLeadPtSystSFbCentral->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtSystSFbDown->SetBinContent(i,sLeadPtSystSFbDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtSyst2DError->SetBinContent(i,sLeadPtSyst2DError->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtSyst2DCentral->SetBinContent(i,sLeadPtSyst2DCentral->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
   }

   systCalc->CalculateSingleSystError(sLeadPtSyst2DCentral,sLeadPtSyst2DError);
   systCalc->CalculateSingleSystError(sLeadPtSystSFbCentral,sLeadPtSystSFbUp,sLeadPtSystSFbDown);
   systCalc->AddUncorelatedSystErrors(sLeadPtMCSyst);

//   TH1F *ratioPt = (TH1F*) sLeadPtData->Clone("ratioPt");

   TH1F *ratioPt2 = ratio->DrawRatio(sLeadPtData,sLeadPtMC,sLeadPtMCSyst,leg_pt1,canva01);
   ratio->GetTopPad()->SetLogy();
   ratioPt2->GetXaxis()->SetRangeUser(0.,1000.);
   ratio->DrawPhaseSpaceDescription(100.,0.01,400.,.2);

   //.........................(Pt1-Pt2)/(Pt1+Pt2)..................
   TCanvas *canva02 = new TCanvas("canva02","PtAsym",1000,800);

   double PtAssymBins[] = {0.,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.24,0.28,0.32,0.36,0.4,0.45,0.5,0.6};
   const int NumberOfPtAsymBins = sizeof(PtAssymBins)/sizeof(double) - 1;
   //Central values
   //Main Histograms
   TH1F *AssymPtMC = new TH1F("AssymPtMC","First Leading Jet Pt",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtMCSyst = new TH1F("AssymPtMCSyst","First Leading Jet Pt",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtData = new TH1F("AssymPtData",";(p^{1}_{T}-p^{2}_{T})/(p^{1}_{T}+p^{2}_{T});dN / d#frac{p^{1}_{T}-p^{2}_{T}}{p^{1}_{T}+p^{2}_{T}}",NumberOfPtAsymBins,PtAssymBins);
   AssymPtData->GetXaxis()->SetRangeUser(0.,0.6);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMCSyst",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtData");

   //Systematic errors
   // Syst. for SFb
   TH1F *AssymPtSystSFbUp = new TH1F("AssymPtSystSFbUp","Systematic errors up",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtSystSFbCentral = new TH1F("AssymPtSystSFbCentral","Systematic errors central",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtSystSFbDown = new TH1F("AssymPtSystSFbDown","Systematic errors down",NumberOfPtAsymBins,PtAssymBins);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtSystSFbUp",weightLumi*weightBTag*weightPt*weightdEta*btagSFup,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtSystSFbCentral",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtSystSFbDown",weightLumi*weightBTag*weightPt*weightdEta*btagSFdown,"E");

   //Syst. for 2D Pt trigger efficiency
   TH1F *AssymPtSyst2DError = new TH1F("AssymPtSyst2DError","Systematic errors up",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtSyst2DCentral = new TH1F("AssymPtSyst2DCentral","Systematic errors central",NumberOfPtAsymBins,PtAssymBins);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtSyst2DError",weightLumi*weightBTag*weightdEta*btagSFcentral*ptSystWeight,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtSyst2DCentral",weightLumi*weightBTag*weightdEta*btagSFcentral*weightPt,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtAsymBins;++i){
	   AssymPtData->SetBinContent(i,AssymPtData->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtData->SetBinError(i,AssymPtData->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMC->SetBinContent(i,AssymPtMC->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMC->SetBinError(i,AssymPtMC->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMCSyst->SetBinContent(i,AssymPtMCSyst->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMCSyst->SetBinError(i,AssymPtMCSyst->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtSystSFbUp->SetBinContent(i,AssymPtSystSFbUp->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtSystSFbCentral->SetBinContent(i,AssymPtSystSFbCentral->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtSystSFbDown->SetBinContent(i,AssymPtSystSFbDown->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtSyst2DError->SetBinContent(i,AssymPtSyst2DError->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtSyst2DCentral->SetBinContent(i,AssymPtSyst2DCentral->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
   }

   systCalc->CalculateSingleSystError(AssymPtSyst2DCentral,AssymPtSyst2DError);
   systCalc->CalculateSingleSystError(AssymPtSystSFbCentral,AssymPtSystSFbUp,AssymPtSystSFbDown);
   systCalc->AddUncorelatedSystErrors(AssymPtMCSyst);

   TH1F *ratioPtAssym;
   ratioPtAssym = ratio->DrawRatio(AssymPtData,AssymPtMC,AssymPtMCSyst,leg_pt1,canva02);
   ratio->GetTopPad()->SetLogy();
   ratioPtAssym->GetXaxis()->SetRangeUser(0.,0.6);
   ratio->DrawPhaseSpaceDescription(0.02,300,0.2,7000);

   //........................dEta plot.............................
   TCanvas *canva03 = new TCanvas("canva03","dEta",1000,800);

   TH1F *dEtaMC = new TH1F("dEtaMC","First Leading Jet Pt",20,-1.6,1.6);
   TH1F *dEtaMCSyst = new TH1F("dEtaMCSyst","First Leading Jet Pt",20,-1.6,1.6);
   TH1F *dEtaData = new TH1F("dEtaData",";#Delta#eta; dN / d #Delta#eta",20,-1.6,1.6);
   dEtaData->GetXaxis()->SetRangeUser(-2.,2.);
   dEtaData->SetMinimum(10.);

   mcTree ->Draw("dEtaFS>>dEtaMCSyst",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("dEtaFS>>dEtaMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("dEtaFS>>dEtaData");

   //Systematic errors
   // Syst. for SFb
   TH1F *dEtaSystSFbUp = new TH1F("dEtaSystSFbUp","Systematic errors up",20,-1.6,1.6);
   TH1F *dEtaSystSFbCentral = new TH1F("dEtaSystSFbCentral","Systematic errors central",20,-1.6,1.6);
   TH1F *dEtaSystSFbDown = new TH1F("dEtaSystSFbDown","Systematic errors down",20,-1.6,1.6);

   mcTree ->Draw("dEtaFS>>dEtaSystSFbUp",weightLumi*weightBTag*weightPt*weightdEta*btagSFup,"E");
   mcTree ->Draw("dEtaFS>>dEtaSystSFbCentral",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("dEtaFS>>dEtaSystSFbDown",weightLumi*weightBTag*weightPt*weightdEta*btagSFdown,"E");

   //Syst. for 2D Pt trigger efficiency
   TH1F *dEtaSyst2DError = new TH1F("dEtaSyst2DError","Systematic errors up",20,-1.6,1.6);
   TH1F *dEtaSyst2DCentral = new TH1F("dEtaSyst2DCentral","Systematic errors central",20,-1.6,1.6);

   mcTree ->Draw("dEtaFS>>dEtaSyst2DError",weightLumi*weightBTag*weightdEta*btagSFcentral*ptSystWeight,"E");
   mcTree ->Draw("dEtaFS>>dEtaSyst2DCentral",weightLumi*weightBTag*weightdEta*btagSFcentral*weightPt,"E");

   //Normalization:

   for(int i = 1;i<=20;++i){
	   dEtaData->SetBinContent(i,dEtaData->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaData->SetBinError(i,dEtaData->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMC->SetBinContent(i,dEtaMC->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMC->SetBinError(i,dEtaMC->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMCSyst->SetBinContent(i,dEtaMCSyst->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMCSyst->SetBinError(i,dEtaMCSyst->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaSystSFbUp->SetBinContent(i,dEtaSystSFbUp->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaSystSFbCentral->SetBinContent(i,dEtaSystSFbCentral->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaSystSFbDown->SetBinContent(i,dEtaSystSFbDown->GetBinContent(i)/(dEtaData->GetBinWidth(20)));

	   dEtaSyst2DError->SetBinContent(i,dEtaSyst2DError->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaSyst2DCentral->SetBinContent(i,dEtaSyst2DCentral->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
   }

   systCalc->CalculateSingleSystError(dEtaSyst2DCentral,dEtaSyst2DError);
   systCalc->CalculateSingleSystError(dEtaSystSFbCentral,dEtaSystSFbUp,dEtaSystSFbDown);
   systCalc->AddUncorelatedSystErrors(dEtaMCSyst);


   TH1F *ratioDEta;
   ratioDEta = ratio->DrawRatio(dEtaData,dEtaMC,dEtaMCSyst,leg_pt1,canva03);
   ratioDEta->GetXaxis()->SetRangeUser(-2.,2.);
   ratio->DrawPhaseSpaceDescription(-1.6,10000,-0.5,30000);

   //..............................M12 ....................
   TCanvas *canva04 = new TCanvas("canva04","M12",1000,800);
   double M12Bins[] = {120,160,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,700,750,800,850,900,1000};
   const int NumberM12Bins = sizeof(M12Bins)/sizeof(double) -1;
   //Central values
   //Main Histograms
   TH1F *M12MC = new TH1F("M12MC","First Leading Jet Pt",NumberM12Bins,M12Bins);
   TH1F *M12MCSyst = new TH1F("M12MCSyst","First Leading Jet Pt",NumberM12Bins,M12Bins);
   TH1F *M12Data = new TH1F("M12Data",";sub-leading b jet pT (GeV); dN / d(di-jet Mass) (1/GeV)",NumberM12Bins,M12Bins);
   M12Data->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("ObjM12>>M12MCSyst",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("ObjM12>>M12MC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("ObjM12>>M12Data");

   //Systematic errors
   // Syst. for SFb
   TH1F *M12SystSFbUp = new TH1F("M12SystSFbUp","Systematic errors up",NumberM12Bins,M12Bins);
   TH1F *M12SystSFbCentral = new TH1F("M12SystSFbCentral","Systematic errors central",NumberM12Bins,M12Bins);
   TH1F *M12SystSFbDown = new TH1F("M12SystSFbDown","Systematic errors down",NumberM12Bins,M12Bins);

   mcTree ->Draw("ObjM12>>M12SystSFbUp",weightLumi*weightBTag*weightPt*weightdEta*btagSFup,"E");
   mcTree ->Draw("ObjM12>>M12SystSFbCentral",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("ObjM12>>M12SystSFbDown",weightLumi*weightBTag*weightPt*weightdEta*btagSFdown,"E");

   //Syst. for 2D Pt trigger efficiency
   TH1F *M12Syst2DError = new TH1F("M12Syst2DError","Systematic errors up",NumberM12Bins,M12Bins);
   TH1F *M12Syst2DCentral = new TH1F("M12Syst2DCentral","Systematic errors central",NumberM12Bins,M12Bins);

   mcTree ->Draw("ObjM12>>M12Syst2DError",weightLumi*weightBTag*weightdEta*btagSFcentral*ptSystWeight,"E");
   mcTree ->Draw("ObjM12>>M12Syst2DCentral",weightLumi*weightBTag*weightdEta*btagSFcentral*weightPt,"E");

   //Normalization:

   for(int i = 1;i<=NumberM12Bins;++i){
	   M12Data->SetBinContent(i,M12Data->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12Data->SetBinError(i,M12Data->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MC->SetBinContent(i,M12MC->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MC->SetBinError(i,M12MC->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MCSyst->SetBinContent(i,M12MCSyst->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MCSyst->SetBinError(i,M12MCSyst->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12SystSFbUp->SetBinContent(i,M12SystSFbUp->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12SystSFbCentral->SetBinContent(i,M12SystSFbCentral->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12SystSFbDown->SetBinContent(i,M12SystSFbDown->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12Syst2DError->SetBinContent(i,M12Syst2DError->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12Syst2DCentral->SetBinContent(i,M12Syst2DCentral->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
   }

   systCalc->CalculateSingleSystError(M12Syst2DCentral,M12Syst2DError);
   systCalc->CalculateSingleSystError(M12SystSFbCentral,M12SystSFbUp,M12SystSFbDown);
   systCalc->AddUncorelatedSystErrors(M12MCSyst);

   TH1F *ratioM12;
   ratioM12 = ratio->DrawRatio(M12Data,M12MC,M12MCSyst,leg_pt1,canva04);
   ratio->GetTopPad()->SetLogy();
   ratioM12->GetXaxis()->SetRangeUser(0.,1000.);
   ratioM12->SetTitle(";di-jet Mass (GeV);Data / MC");
   ratio->DrawPhaseSpaceDescription(200.,2,450.,19);

   /*
   //........................Eta1 plot.............................
   TCanvas *canva10 = new TCanvas("canva10","Eta1",1000,800);

   //Central values
   TH1F *fEtaMC = new TH1F("fEtaMC","Assym pt",20,-1.6,1.6);
   TH1F *fEtaData = new TH1F("fEtaData"," ;leading b jet #eta; dN / d #eta",20,-1.6,1.6);
   fEtaData->GetXaxis()->SetRangeUser(-2.,2.);

   mcTree ->Draw("LeadEta[0]>>fEtaMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadEta[0]>>fEtaData");

   //Systematic errors
   TH1F *fEtaMCSystUp = new TH1F("fEtaMCSystUp","Systematic errors up",20,-1.6,1.6);
   TH1F *fEtaMCSystDown = new TH1F("fEtaMCSystDown","Systematic errors down",20,-1.6,1.6);

   mcTree ->Draw("LeadEta[0]>>fEtaMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("LeadEta[0]>>fEtaMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=20;++i){
	   fEtaData->SetBinContent(i,fEtaData->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaData->SetBinError(i,fEtaData->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMC->SetBinContent(i,fEtaMC->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMC->SetBinError(i,fEtaMC->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMCSystUp->SetBinContent(i,fEtaMCSystUp->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMCSystUp->SetBinError(i,fEtaMCSystUp->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMCSystDown->SetBinContent(i,fEtaMCSystDown->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMCSystDown->SetBinError(i,fEtaMCSystDown->GetBinError(i)/(fEtaData->GetBinWidth(20)));
   }

   TGraphAsymmErrors *fEtaMCSyst;
   fEtaMCSyst = ratio->CreateSystTGraph(fEtaMC,fEtaMCSystUp,fEtaMCSystDown);

   TH1F *ratiofEta;
   ratiofEta = ratio->DrawRatio(fEtaData,fEtaMC,fEtaMCSyst,leg_pt1,canva10);
   ratiofEta->SetAxisRange(0.,2.,"Y");
   ratiofEta->GetXaxis()->SetRangeUser(-2.,2.);
   ratio->DrawPhaseSpaceDescription(-1.2,10000,-0.2,30000);

   //..............................M12 ....................
   TCanvas *canva11 = new TCanvas("canva11","M12 FIT",1000,800);
   //Central values
   TH1F *M12FitMC = new TH1F("M12FitMC","M12Fit",100,300,1300);
   TH1F *M12FitData = new TH1F("M12FitData","M12Fit",100,300,1300);
   M12FitData->SetTitle(" ;di-jet Mass (GeV); dN / d(di-jet Mass) (1/GeV)");
   M12FitData->GetXaxis()->SetRangeUser(300.,1300.);

   mcTree ->Draw("ObjM12>>M12FitMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("ObjM12>>M12FitData");

   //Systematic errors
   TH1F *M12FitMCSystUp = new TH1F("M12FitMCSystUp","Systematic errors up",100,300,1300);
   TH1F *M12FitMCSystDown = new TH1F("M12FitMCSystDown","Systematic errors down",100,300,1300);

   mcTree ->Draw("ObjM12>>M12FitMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("ObjM12>>M12FitMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=100;++i){
	   M12FitData->SetBinContent(i,M12FitData->GetBinContent(i)/(M12FitData->GetBinWidth(20)));
	   M12FitData->SetBinError(i,M12FitData->GetBinError(i)/(M12FitData->GetBinWidth(20)));

	   M12FitMC->SetBinContent(i,M12FitMC->GetBinContent(i)/(M12FitData->GetBinWidth(20)));
	   M12FitMC->SetBinError(i,M12FitMC->GetBinError(i)/(M12FitData->GetBinWidth(20)));

	   M12FitMCSystUp->SetBinContent(i,M12FitMCSystUp->GetBinContent(i)/(M12FitData->GetBinWidth(20)));
	   M12FitMCSystUp->SetBinError(i,M12FitMCSystUp->GetBinError(i)/(M12FitData->GetBinWidth(20)));

	   M12FitMCSystDown->SetBinContent(i,M12FitMCSystDown->GetBinContent(i)/(M12FitData->GetBinWidth(20)));
	   M12FitMCSystDown->SetBinError(i,M12FitMCSystDown->GetBinError(i)/(M12FitData->GetBinWidth(20)));
   }

   TGraphAsymmErrors *M12FitMCSyst;
   M12FitMCSyst = ratio->CreateSystTGraph(M12FitMC,M12FitMCSystUp,M12FitMCSystDown);

   TH1F *ratioM12Fit;
   ratioM12Fit = ratio->DrawRatio(M12FitData,M12FitMC,M12FitMCSyst,leg_pt1,canva11);
   ratio->GetTopPad()->SetLogy();
   ratioM12Fit->SetAxisRange(0.,2.,"Y");
   ratioM12Fit->GetXaxis()->SetRangeUser(300.,1300.);
   ratio->DrawPhaseSpaceDescription(200.,3,450.,15);

   //Fit M12 Data:
   TF1 *M12Fit = new TF1("M12Fit",M12FitFunction,300,1300,3);
   M12Fit->SetParameters(1,1,1);
   //F1 *M12Fit = new TF1("M12Fit","expo(0)*pol6",300,1300);
   M12FitData->Fit(M12Fit);


/**/
}

double M12FitFunction(double *x, double *par){
	return par[1]*pow((1-pow(x[0],par[0])),par[2]);
	//return exp(x[0]);
}


