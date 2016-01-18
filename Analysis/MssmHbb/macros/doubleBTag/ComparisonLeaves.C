#include "Analysis/Drawer/src/HbbStyle.cc"
#include "Analysis/Drawer/src/RatioPlots.cpp"
#include "Analysis/Drawer/src/Measurement.cpp"
#include "TCut.h"
#include "TGraphAsymmErrors.h"

double M12FitFunction(double *x, double *par);

void ComparisonLeaves()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionQCD_170116.root");

   TTree *dataTree, *mcTree;
   fMonteCarlo -> GetObject("MssmHbb",mcTree);
   fData -> GetObject("MssmHbb",dataTree);

   //Setup style
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PUBLIC);

   int bin40to60 = 90;
   int bin60to80 = 115;
   TCut cut = "dPhiFS>2.7";// && Njets == 2 && NL1Object >= 2";//"ptVeto < 10 && ((fLeadPt - M12)/(fLeadPt + sLeadPt) <= 0.2) && abs(fLeadEta) <= 2. && abs(sLeadEta)<=2";
   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   TCut SystUp = "SystUp";
   TCut SystDown = "SystDown";
   double SFb = 0.97*0.97;

   //..............................Pt1 ....................
   TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

   const int NumberOfPtBins = 24;
   double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

   //Central values
   TH1F *fLeadPtMC = new TH1F("fLeadPtMC","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *fLeadPtData = new TH1F("fLeadPtData","First Leading Jet Pt",NumberOfPtBins,PtBins);
   fLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);
   fLeadPtData->SetTitle(" ; leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)");


   mcTree ->Draw("LeadPt[0]>>fLeadPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[0]>>fLeadPtData");

   //Systematic errors
   TH1F *fLeadPtMCSystUp = new TH1F("fLeadPtMCSystUp","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *fLeadPtMCSystDown = new TH1F("fLeadPtMCSystDown","Systematic errors down",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[0]>>fLeadPtMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   fLeadPtData->SetBinContent(i,fLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtData->SetBinError(i,fLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMC->SetBinContent(i,fLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMC->SetBinError(i,fLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMCSystUp->SetBinContent(i,fLeadPtMCSystUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMCSystUp->SetBinError(i,fLeadPtMCSystUp->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMCSystDown->SetBinContent(i,fLeadPtMCSystDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMCSystDown->SetBinError(i,fLeadPtMCSystDown->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
   }

   TGraphAsymmErrors *fLeadPtMCSyst;
   fLeadPtMCSyst = ratio->CreateSystTGraph(fLeadPtMC,fLeadPtMCSystUp,fLeadPtMCSystDown);

   TLegend *leg_pt1 = new TLegend(0.55,0.75,0.8,0.9);
   leg_pt1->AddEntry(fLeadPtData,"Run2015D-Prompt-Reco-v4","p");
   leg_pt1->AddEntry(fLeadPtMC,"QCD 13 TeV pythia 8","p");

   TH1F *ratioPt = (TH1F*) fLeadPtData->Clone("ratioPt");
   //Setup general Y-axis title for Ratio
   ratio->SetRatioTitle("Data / MC");
   ratioPt = ratio->DrawRatio(fLeadPtData,fLeadPtMC,fLeadPtMCSyst,leg_pt1,canva00);
   ratio->GetTopPad()->SetLogy();
   ratioPt->SetAxisRange(0.,2.,"Y");
   ratioPt->GetXaxis()->SetRangeUser(0.,1000.);
   ratio->DrawPhaseSpaceDescription(100.,0.1,400.,2.);




   //..............................Pt2 ....................
   TCanvas *canva01 = new TCanvas("canva01","Pt2",1000,800);
   //Central values
   TH1F *sLeadPtMC = new TH1F("sLeadPtMC","Second Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *sLeadPtData = new TH1F("sLeadPtData","Second Leading Jet Pt",NumberOfPtBins,PtBins);
   sLeadPtData->SetTitle(" ;sub-leading b jet p_{T} (GeV); dN / dp_{T} (1/GeV)");
   sLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[1]>>sLeadPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[1]>>sLeadPtData");

   //Systematic errors
   TH1F *sLeadPtMCSystUp = new TH1F("sLeadPtMCSystUp","Systematic errors up",NumberOfPtBins,PtBins);
   TH1F *sLeadPtMCSystDown = new TH1F("sLeadPtMCSystDown","Systematic errors down",NumberOfPtBins,PtBins);

   mcTree ->Draw("LeadPt[1]>>sLeadPtMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   sLeadPtData->SetBinContent(i,sLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtData->SetBinError(i,sLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMC->SetBinContent(i,sLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMC->SetBinError(i,sLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMCSystUp->SetBinContent(i,sLeadPtMCSystUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMCSystUp->SetBinError(i,sLeadPtMCSystUp->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMCSystDown->SetBinContent(i,sLeadPtMCSystDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMCSystDown->SetBinError(i,sLeadPtMCSystDown->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
   }

   TGraphAsymmErrors *sLeadPtMCSyst;
   sLeadPtMCSyst = ratio->CreateSystTGraph(sLeadPtMC,sLeadPtMCSystUp,sLeadPtMCSystDown);

   TH1F *ratioPt2;
   ratioPt2 = ratio->DrawRatio(sLeadPtData,sLeadPtMC,sLeadPtMCSyst,leg_pt1,canva01);
   ratio->GetTopPad()->SetLogy();
   ratioPt2->SetAxisRange(0.,2.,"Y");
   ratioPt2->GetXaxis()->SetRangeUser(0.,1000.);
   ratio->DrawPhaseSpaceDescription(100.,0.01,400.,.2);

   //.........................(Pt1-Pt2)/(Pt1+Pt2)..................
   TCanvas *canva02 = new TCanvas("canva02","PtAsym",1000,800);

   double PtAssymBins[] = {0.,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.24,0.28,0.32,0.36,0.4,0.45,0.5,0.6};
   const int NumberOfPtAsymBins = sizeof(PtAssymBins)/sizeof(double) - 1;
   //Central values
   TH1F *AssymPtMC = new TH1F("AssymPtMC","Assym pt",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtData = new TH1F("AssymPtData",";(p^{1}_{T}-p^{2}_{T})/(p^{1}_{T}+p^{2}_{T});dN / d#frac{p^{1}_{T}-p^{2}_{T}}{p^{1}_{T}+p^{2}_{T}}",NumberOfPtAsymBins,PtAssymBins);
   AssymPtData->GetXaxis()->SetRangeUser(0.,0.6);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtData");

   //Systematic errors
   TH1F *AssymPtMCSystUp = new TH1F("AssymPtMCSystUp","Systematic errors up",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtMCSystDown = new TH1F("AssymPtMCSystDown","Systematic errors down",NumberOfPtAsymBins,PtAssymBins);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtAsymBins;++i){
	   AssymPtData->SetBinContent(i,AssymPtData->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtData->SetBinError(i,AssymPtData->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMC->SetBinContent(i,AssymPtMC->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMC->SetBinError(i,AssymPtMC->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMCSystUp->SetBinContent(i,AssymPtMCSystUp->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMCSystUp->SetBinError(i,AssymPtMCSystUp->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMCSystDown->SetBinContent(i,AssymPtMCSystDown->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMCSystDown->SetBinError(i,AssymPtMCSystDown->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
   }

   TGraphAsymmErrors *AssymPtMCSyst;
   AssymPtMCSyst = ratio->CreateSystTGraph(AssymPtMC,AssymPtMCSystUp,AssymPtMCSystDown);

   TH1F *ratioPtAssym;
   ratioPtAssym = ratio->DrawRatio(AssymPtData,AssymPtMC,AssymPtMCSyst,leg_pt1,canva02);
   ratio->GetTopPad()->SetLogy();
   ratioPtAssym->SetAxisRange(0.,2.,"Y");
   ratioPtAssym->GetXaxis()->SetRangeUser(0.,0.6);
   ratio->DrawPhaseSpaceDescription(0.02,600,0.2,7000);

   //........................dEta plot.............................
   TCanvas *canva03 = new TCanvas("canva03","dEta",1000,800);

   //Central values
   TH1F *dEtaMC = new TH1F("dEtaMC","Assym pt",20,-1.6,1.6);
   TH1F *dEtaData = new TH1F("dEtaData"," ;#Delta#eta; dN / d #Delta#eta",20,-1.6,1.6);
   dEtaData->GetXaxis()->SetRangeUser(-2.,2.);

   mcTree ->Draw("dEtaFS>>dEtaMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("dEtaFS>>dEtaData");

   //Systematic errors
   TH1F *dEtaMCSystUp = new TH1F("dEtaMCSystUp","Systematic errors up",20,-1.6,1.6);
   TH1F *dEtaMCSystDown = new TH1F("dEtaMCSystDown","Systematic errors down",20,-1.6,1.6);

   mcTree ->Draw("dEtaFS>>dEtaMCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("dEtaFS>>dEtaMCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=20;++i){
	   dEtaData->SetBinContent(i,dEtaData->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaData->SetBinError(i,dEtaData->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMC->SetBinContent(i,dEtaMC->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMC->SetBinError(i,dEtaMC->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMCSystUp->SetBinContent(i,dEtaMCSystUp->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMCSystUp->SetBinError(i,dEtaMCSystUp->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMCSystDown->SetBinContent(i,dEtaMCSystDown->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMCSystDown->SetBinError(i,dEtaMCSystDown->GetBinError(i)/(dEtaData->GetBinWidth(20)));
   }

   TGraphAsymmErrors *dEtaMCSyst;
   dEtaMCSyst = ratio->CreateSystTGraph(dEtaMC,dEtaMCSystUp,dEtaMCSystDown);

   TH1F *ratioDEta;
   ratioDEta = ratio->DrawRatio(dEtaData,dEtaMC,dEtaMCSyst,leg_pt1,canva03);
   ratioDEta->SetAxisRange(0.,2.,"Y");
   ratioDEta->GetXaxis()->SetRangeUser(-2.,2.);
   ratio->DrawPhaseSpaceDescription(-1.6,10000,-0.5,30000);

   //..............................M12 ....................
   TCanvas *canva04 = new TCanvas("canva04","M12",1000,800);
   double M12Bins[] = {120,160,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,700,750,800,850,900,1000};
   const int NumberM12Bins = sizeof(M12Bins)/sizeof(double) -1;
   //Central values
   TH1F *M12MC = new TH1F("M12MC","M12",NumberM12Bins,M12Bins);
   TH1F *M12Data = new TH1F("M12Data","M12",NumberM12Bins,M12Bins);
   M12Data->SetTitle(" ;sub-leading b jet pT (GeV); dN / d(di-jet Mass) (1/GeV)");
   M12Data->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("ObjM12>>M12MC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("ObjM12>>M12Data");

   //Systematic errors
   TH1F *M12MCSystUp = new TH1F("M12MCSystUp","Systematic errors up",NumberM12Bins,M12Bins);
   TH1F *M12MCSystDown = new TH1F("M12MCSystDown","Systematic errors down",NumberM12Bins,M12Bins);

   mcTree ->Draw("ObjM12>>M12MCSystUp",weightLumi*weightBTag*weightdEta*SystUp,"E");
   mcTree ->Draw("ObjM12>>M12MCSystDown",weightLumi*weightBTag*weightdEta*SystDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberM12Bins;++i){
	   M12Data->SetBinContent(i,M12Data->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12Data->SetBinError(i,M12Data->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MC->SetBinContent(i,M12MC->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MC->SetBinError(i,M12MC->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MCSystUp->SetBinContent(i,M12MCSystUp->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MCSystUp->SetBinError(i,M12MCSystUp->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MCSystDown->SetBinContent(i,M12MCSystDown->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MCSystDown->SetBinError(i,M12MCSystDown->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));
   }

   TGraphAsymmErrors *M12MCSyst;
   M12MCSyst = ratio->CreateSystTGraph(M12MC,M12MCSystUp,M12MCSystDown);

   TH1F *ratioM12;
   ratioM12 = ratio->DrawRatio(M12Data,M12MC,M12MCSyst,leg_pt1,canva04);
   ratio->GetTopPad()->SetLogy();
   ratioM12->SetAxisRange(0.,2.,"Y");
   ratioM12->GetXaxis()->SetRangeUser(0.,1000.);
   ratioM12->SetTitle(";di-jet Mass (GeV);Data / MC");
   ratio->DrawPhaseSpaceDescription(200.,3,450.,15);

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


