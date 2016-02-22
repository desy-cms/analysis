#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/macros/Drawer/SystematicErrorsCalculator.cpp"
#include "TCut.h"
#include "TGraphAsymmErrors.h"
#include "THStack.h"

double M12FitFunction(double *x, double *par);

void highMComparison()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/EfficiencyStudy_Run2015_12_02_2016.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/EfficiencyStudy_QCD_12_02_2016.root");
   std::string mcName = "QCD HT 13 TeV Madgraph+Pythia8";

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
   TCut data = "(doubleJetTolopogy == 1 && LeadPt[0] > 160 && LeadPt[1] > 160 && LeadMatch160[0] == 1 && LeadMatch160[1] == 1)";
   TCut general = "(doubleJetTolopogy == 1 && LeadPt[0] > 160 && LeadPt[1] > 160)";
   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";


   //Start drawing
   //.....................Ht bins.........................
   TCanvas *canva001 = new TCanvas("canva001","Jet multiplicity",1000,800);

   const int NumberofHtBins = 37;
   double HtBins[NumberofHtBins+1]={200,205,210,215,220,
		   	   	   	   	   	   	    240,260,280,300,330,
									360,390,420,450,480,
									510,550,600,650,700,
									750,800,850,900,950,
									1000,1100,1200,1300,1400,
									1500,1600,1700,1800,1900,
   	   	   	   	   	   	   	   	   	2000,2200,2600};

   //Main Histograms
   TH1F *fHtMC = new TH1F("fHtMC","First Leading Jet Pt",NumberofHtBins,HtBins);
   TH1F *fHtData = new TH1F("fHtData",";H_{T}, (GeV) ; dN / d H_{T}",NumberofHtBins,HtBins);
   fHtData->GetXaxis()->SetRangeUser(0.,2600.);

   mcTree ->Draw("Ht>>fHtMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("Ht>>fHtData",data);


   //Normalization:
   fHtMC->Scale(fHtData->Integral(10,19)/fHtMC->Integral(10,19));
   for(int i = 1;i<=NumberofHtBins;++i){
	   fHtData->SetBinContent(i,fHtData->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   fHtData->SetBinError(i,fHtData->GetBinError(i)/(HtBins[i]-HtBins[i-1]));

	   fHtMC->SetBinContent(i,fHtMC->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   fHtMC->SetBinError(i,fHtMC->GetBinError(i)/(HtBins[i]-HtBins[i-1]));
   }

   TLegend *leg_pt1 = new TLegend(0.55,0.65,0.8,0.9);
   leg_pt1->AddEntry(fHtData,"Data","p");
   leg_pt1->AddEntry(fHtMC,mcName.c_str(),"p");

//   TH1F *ratioPt = (TH1F*) fHtData->Clone("ratioPt");

   TH1F *null_ptr = NULL;
   TH1F *ratioHt = ratio->DrawRatio(fHtData,fHtMC,null_ptr,leg_pt1,canva001);
   ratio->GetTopPad()->SetLogy();
   ratioHt->GetXaxis()->SetRangeUser(0.,2600.);
/*
   //.....................Jet Multiplicity.................
   TCanvas *canva000 = new TCanvas("canva000","Jet multiplicity",1000,800);

   const int NumberOfNBins = 8;
   double NBins[NumberOfNBins+1]={2,3,4,5,6,7,8,9,14};

   //Main Histograms
   TH1F *fNjetsMC = new TH1F("fNjetsMC","First Leading Jet Pt",NumberOfNBins,NBins);
   TH1F *fNjetsData = new TH1F("fNjetsData",";Jet multiplicity ; dN / d Jet Multiplicity",NumberOfNBins,NBins);
   fNjetsData->GetXaxis()->SetRangeUser(0.,14.);

   mcTree ->Draw("Njets>>fNjetsMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("Njets>>fNjetsData",general);

   //Normalization:

   for(int i = 1;i<=NumberOfNBins;++i){
	   fNjetsData->SetBinContent(i,fNjetsData->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   fNjetsData->SetBinError(i,fNjetsData->GetBinError(i)/(NBins[i]-NBins[i-1]));

	   fNjetsMC->SetBinContent(i,fNjetsMC->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   fNjetsMC->SetBinError(i,fNjetsMC->GetBinError(i)/(NBins[i]-NBins[i-1]));
   }

   TH1F *ratioN = ratio->DrawRatio(fNjetsData,fNjetsMC,null_ptr,leg_pt1,canva000);
   ratio->GetTopPad()->SetLogy();
   ratioN->GetXaxis()->SetRangeUser(0.,14.);
   //ratio->DrawPhaseSpaceDescription(100.,0.1,450.,3.);
*/
   //..............................Pt1 ....................
   TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

   double PtBins[]={150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};
   const int NumberOfPtBins = sizeof(PtBins)/sizeof(double)-1;

   //Main Histograms
   TH1F *fLeadPtMC = new TH1F("fLeadPtMC","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *fLeadPtData = new TH1F("fLeadPtData",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   fLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[0]>>fLeadPtMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("LeadPt[0]>>fLeadPtData",data);


   //Normalization:

   fLeadPtMC->Scale(fLeadPtData->Integral(10,19)/fLeadPtMC->Integral(10,19));
   for(int i = 1;i<=NumberOfPtBins;++i){
	   fLeadPtData->SetBinContent(i,fLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtData->SetBinError(i,fLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMC->SetBinContent(i,fLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMC->SetBinError(i,fLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
   }

   TH1F *ratioPt = ratio->DrawRatio(fLeadPtData,fLeadPtMC,null_ptr,leg_pt1,canva00);
   ratio->GetTopPad()->SetLogy();
   ratioPt->GetXaxis()->SetRangeUser(0.,1000.);
   //ratio->DrawPhaseSpaceDescription(100.,0.1,450.,3.);
/*
   //..............................Pt2 ....................
   TCanvas *canva01 = new TCanvas("canva01","Pt2",1000,800);

   //Main Histograms
   TH1F *sLeadPtMC = new TH1F("sLeadPtMC","First Leading Jet Pt",NumberOfPtBins,PtBins);
   TH1F *sLeadPtData = new TH1F("sLeadPtData",";sub-leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   sLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[1]>>sLeadPtMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("LeadPt[1]>>sLeadPtData",general);

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   sLeadPtData->SetBinContent(i,sLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtData->SetBinError(i,sLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMC->SetBinContent(i,sLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMC->SetBinError(i,sLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
   }

   TH1F *ratioPt2 = ratio->DrawRatio(sLeadPtData,sLeadPtMC,null_ptr,leg_pt1,canva01);
   ratio->GetTopPad()->SetLogy();
   ratioPt2->GetXaxis()->SetRangeUser(0.,1000.);
   ratio->DrawPhaseSpaceDescription(100.,0.01,450.,.3);

   //.........................(Pt1-Pt2)/(Pt1+Pt2)..................
   TCanvas *canva02 = new TCanvas("canva02","PtAsym",1000,800);

   double PtAssymBins[] = {0.,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.24,0.28,0.32,0.36,0.4,0.45,0.5,0.6};
   const int NumberOfPtAsymBins = sizeof(PtAssymBins)/sizeof(double) - 1;
   //Central values
   //Main Histograms
   TH1F *AssymPtMC = new TH1F("AssymPtMC","First Leading Jet Pt",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtData = new TH1F("AssymPtData",";(p^{1}_{T}-p^{2}_{T})/(p^{1}_{T}+p^{2}_{T});dN / d#frac{p^{1}_{T}-p^{2}_{T}}{p^{1}_{T}+p^{2}_{T}}",NumberOfPtAsymBins,PtAssymBins);
   AssymPtData->GetXaxis()->SetRangeUser(0.,0.6);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtData",general);

   //Normalization:

   for(int i = 1;i<=NumberOfPtAsymBins;++i){
	   AssymPtData->SetBinContent(i,AssymPtData->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtData->SetBinError(i,AssymPtData->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMC->SetBinContent(i,AssymPtMC->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMC->SetBinError(i,AssymPtMC->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

   }


   TH1F *ratioPtAssym;
   ratioPtAssym = ratio->DrawRatio(AssymPtData,AssymPtMC,null_ptr,leg_pt1,canva02);
   ratio->GetTopPad()->SetLogy();
   ratioPtAssym->GetXaxis()->SetRangeUser(0.,0.6);
   ratio->DrawPhaseSpaceDescription(0.03,500,0.22,9000);

   //........................dEta plot.............................
   TCanvas *canva03 = new TCanvas("canva03","dEta",1000,800);

   TH1F *dEtaMC = new TH1F("dEtaMC","First Leading Jet Pt",20,-1.6,1.6);
   TH1F *dEtaData = new TH1F("dEtaData",";#Delta#eta; dN / d #Delta#eta",20,-1.6,1.6);
   dEtaData->GetXaxis()->SetRangeUser(-2.,2.);
   dEtaData->SetMinimum(10.);

   mcTree ->Draw("dEtaFS>>dEtaMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("dEtaFS>>dEtaData",general);

   //Normalization:

   for(int i = 1;i<=20;++i){
	   dEtaData->SetBinContent(i,dEtaData->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaData->SetBinError(i,dEtaData->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMC->SetBinContent(i,dEtaMC->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMC->SetBinError(i,dEtaMC->GetBinError(i)/(dEtaData->GetBinWidth(20)));
   }


   TLegend *leg_pt2 = new TLegend(0.55,0.03,0.8,0.2);
   leg_pt2->AddEntry(fLeadPtData,"Data","p");
   leg_pt2->AddEntry(fLeadPtMC,mcName.c_str(),"p");

   TH1F *ratioDEta;
   ratioDEta = ratio->DrawRatio(dEtaData,dEtaMC,null_ptr,leg_pt2,canva03);
   ratioDEta->GetXaxis()->SetRangeUser(-2.,2.);
   ratio->DrawPhaseSpaceDescription(-1.6,10000,-0.5,70000);

   //..............................M12 ....................
   TCanvas *canva04 = new TCanvas("canva04","M12",1000,800);
   double M12Bins[] = {200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,700,750,800,850,900,1000};
   const int NumberM12Bins = sizeof(M12Bins)/sizeof(double) -1;
   //Central values
   //Main Histograms
   TH1F *M12MC = new TH1F("M12MC","First Leading Jet Pt",NumberM12Bins,M12Bins);
   TH1F *M12Data = new TH1F("M12Data",";di-jet Mass (GeV); dN / d(di-jet Mass) (1/GeV)",NumberM12Bins,M12Bins);
   M12Data->GetXaxis()->SetRangeUser(200.,1000.);

   mcTree ->Draw("ObjM12>>M12MC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("ObjM12>>M12Data",general);

   //Normalization:

   for(int i = 1;i<=NumberM12Bins;++i){
	   M12Data->SetBinContent(i,M12Data->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12Data->SetBinError(i,M12Data->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MC->SetBinContent(i,M12MC->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MC->SetBinError(i,M12MC->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));
   }

   TH1F *ratioM12;
   ratioM12 = ratio->DrawRatio(M12Data,M12MC,null_ptr,leg_pt1,canva04);
   ratio->GetTopPad()->SetLogy();
   ratioM12->GetXaxis()->SetRangeUser(200.,1000.);
   ratioM12->SetTitle(";di-jet Mass (GeV);Data / MC");
   ratio->DrawPhaseSpaceDescription(240.,3,480.,30);


   //........................Eta1 plot.............................
   TCanvas *canva10 = new TCanvas("canva10","Eta1",1000,800);

   TH1F *fEtaMC = new TH1F("fEtaMC","First Leading Jet Pt",30,-2.2,2.2);
   TH1F *fEtaData = new TH1F("fEtaData",";leading b jet #eta; dN / d #eta",30,-2.2,2.2);
   fEtaData->GetXaxis()->SetRangeUser(-2.4,2.4);
   fEtaData->SetMinimum(10.);

   mcTree ->Draw("LeadEta[0]>>fEtaMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("LeadEta[0]>>fEtaData",general);

   //Normalization:

   for(int i = 1;i<=30;++i){
	   fEtaData->SetBinContent(i,fEtaData->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaData->SetBinError(i,fEtaData->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMC->SetBinContent(i,fEtaMC->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMC->SetBinError(i,fEtaMC->GetBinError(i)/(fEtaData->GetBinWidth(20)));

   }

   TLegend *leg_pt3 = new TLegend(0.57,0.03,0.8,0.2);
   leg_pt3->AddEntry(fLeadPtData,"Data","p");
   leg_pt3->AddEntry(fLeadPtMC,mcName.c_str(),"p");

   TH1F *ratiofEta;
   ratiofEta = ratio->DrawRatio(fEtaData,fEtaMC,null_ptr,leg_pt3,canva10);
   ratiofEta->GetXaxis()->SetRangeUser(-2.4,2.4);
   ratio->DrawPhaseSpaceDescription(-1.45,10000,0.2,70000);

   //..............BTag Discriminant................

   TCanvas *canva05 = new TCanvas("canva05","BTagDiscr",1000,800);
   //Central values
   //Main Histograms
   TH1F *BTagDiscrMC = new TH1F("BTagDiscrMC","First Leading Jet Pt",30,0.94,1.);
   TH1F *BTagDiscrData = new TH1F("BTagDiscrData",";BTagCSV discr. ; dN / dD_{BTagCSV} ",30,0.94,1.);
   BTagDiscrData->GetXaxis()->SetRangeUser(0.93,1.);

   mcTree ->Draw("LeadBTag[0]>>BTagDiscrMC",general*weightLumi*weightPt,"E");
   dataTree ->Draw("LeadBTag[0]>>BTagDiscrData",general);

   //Normalization:

   for(int i = 1;i<=30;++i){
	   BTagDiscrData->SetBinContent(i,BTagDiscrData->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrData->SetBinError(i,BTagDiscrData->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

	   BTagDiscrMC->SetBinContent(i,BTagDiscrMC->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrMC->SetBinError(i,BTagDiscrMC->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));
   }

   TH1F *ratioBTagDiscr;
   ratioBTagDiscr = ratio->DrawRatio(BTagDiscrData,BTagDiscrMC,null_ptr,leg_pt1,canva05);
   ratioBTagDiscr->GetXaxis()->SetRangeUser(0.93,1.);
   ratioBTagDiscr->SetTitle(";BTagCSV discr. ;Data / MC");
   ratio->DrawPhaseSpaceDescription(0.942,45000000,0.96,62000000);

/**/
}

double M12FitFunction(double *x, double *par){
	return par[1]*pow((1-pow(x[0],par[0])),par[2]);
	//return exp(x[0]);
}


