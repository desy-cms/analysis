#include <iostream>
#include "stdio.h"

#include "TH1.h"
#include "TH1F.h"

#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"


void btagging(){

	gROOT -> Reset();
	TH1::SetDefaultSumw2();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_BTag0p7_lowMTrigger_Run2015_17_03_2016.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_BTag0p7_lowMTrigger_Pythia8_18_03_2016.root");
   std::string mcName = "Pythia 8 QCD";

   TTree *dataTree, *mcTree;
   fMonteCarlo -> GetObject("MssmHbb",mcTree);
   fData -> GetObject("MssmHbb",dataTree);

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PRIVATE);
   ratio->SetRatioTitle("Data / MC"); // Default Title for Ratio plot Y-axis
   ratio->SetRatioRange(0.2,1.8);	// Default Y-axis range for Ratio plot

   //..............BTag Discriminant................
   TCut centralWeight = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[0]";
   TCut btag = "LeadBTag[0] > 0.935 && LeadBTag[1] > 0.935";

   TCanvas *canva05 = new TCanvas("canva05","BTagDiscr",1000,800);
   //Central values
   //Main Histograms
   TH1F *BTagDiscrMC = new TH1F("BTagDiscrMC","MC;BTagCSV discr. ; dN / dD_{BTagCSV}",100,0.7,1.);
   BTagDiscrMC->SetMarkerStyle(20);
   BTagDiscrMC->SetMarkerColor(2);

   BTagDiscrMC->SetMarkerSize(1.01);
   BTagDiscrMC->GetXaxis()->SetTitleOffset(1.);
   BTagDiscrMC->GetXaxis()->SetRangeUser(0.7,1.);
   TH1F *BTagDiscrData = new TH1F("BTagDiscrData",";BTagCSV discr. ; dN / dD_{BTagCSV} ",100,0.7,1.);
   BTagDiscrData->SetMarkerStyle(20);
   BTagDiscrData->SetMarkerSize(1.01);
   BTagDiscrData->GetXaxis()->SetTitleOffset(1.);
   BTagDiscrData->GetXaxis()->SetRangeUser(0.7,1.);

   mcTree ->Draw("LeadBTag[0]>>BTagDiscrMC","LumiWeight*(LeadPt[0] > 500 && LeadPt[1] > 500)","E");
   dataTree ->Draw("LeadBTag[0]>>BTagDiscrData","(LeadPt[0] > 500 && LeadPt[1] > 500)");
   mcTree ->Draw("LeadBTag[0]>>BTagDiscrMC","LumiWeight*(LeadPt[0] > 500 && LeadPt[1] > 500)","E");

   //Normalization:

   for(int i = 1;i<=100;++i){
	   BTagDiscrData->SetBinContent(i,BTagDiscrData->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrData->SetBinError(i,BTagDiscrData->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

	   BTagDiscrMC->SetBinContent(i,BTagDiscrMC->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrMC->SetBinError(i,BTagDiscrMC->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

   }

   TH1F * null_ptr = NULL;

   TH1F *ratioBTagDiscr;
   TLegend *leg_pt1;
   leg_pt1 = (TLegend*) style.legend("top,right",3,0.3);
   leg_pt1->AddEntry(BTagDiscrData,"Data","p");
   leg_pt1->AddEntry(BTagDiscrMC,mcName.c_str(),"p");
   leg_pt1->SetHeader("76X");

   ratioBTagDiscr = ratio->DrawRatio(BTagDiscrData,BTagDiscrMC,null_ptr,leg_pt1,canva05);
   ratioBTagDiscr->GetXaxis()->SetRangeUser(0.7,1.);
   ratioBTagDiscr->SetTitle(";BTagCSV discr. ;Data / MC");

   canva05->SaveAs("Btag1_MC.pdf");

}
