#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"
#include "TExec.h"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

HbbStyle style;

void SetBottomStyle(TH1 *hRatio);
void sistTemp(const std::map<int,TFile*> & inF,const std::string & regime);
void allInOne(const std::map<int,TFile*> & inF,const std::string & regime);
void selectedTemplates(const std::map<int,TFile*> & inF,const std::string &regime, const std::vector<int> & points);

void templates()
{
	TH1::SetDefaultSumw2();
   gStyle->SetOptStat(0);

   std::map<int,TFile* > inF;
   std::vector<TH1D*> histo;

   style.set(PRIVATE);


   TExec *er_0 = new TExec("er_0","gStyle->SetErrorX(0)");
   TExec *er_1 = new TExec("er_1","gStyle->SetErrorX(0.5)");

   inF[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
   inF[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
   inF[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
   inF[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
   inF[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
   inF[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inF[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   inF[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inF[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inF[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");

//   inF[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");
/**/
   std::map<int,TFile* > highMF;
   std::vector<TH1D*> histoHighM;
   highMF[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   highMF[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   highMF[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   highMF[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   highMF[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   highMF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   highMF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   highMF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   highMF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");


   //   highMF[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
//   highMF[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
//   highMF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
//   highMF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
//   highMF[1100] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
//   highMF[1300] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_1pb_xsection_highM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");


   allInOne(inF,"lowM");
//   allInOne(highMF,"highM");

   sistTemp(inF,"lowM");
//   sistTemp(highMF,"highM");

   std::vector<int> points = {300,500,700,900,1100,1300};

   selectedTemplates(inF,"lowM",points);
   selectedTemplates(highMF,"highM",points);




//   double xMin = 0, xMax = 0;
//   TLegend *leg = new TLegend(0.3,0.3,0.9,0.85);
//   leg->SetBorderSize(0);
//
//   TCanvas * c1[15];
//   TH1D *hLowM[15];
//
//   int i = 0, j =0, k=0;
//   for(const auto & f : inF){
//	   ++i;
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVUp"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVUp"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVUp"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JER_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JER_13TeVUp"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVUp"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVDown"));
//	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVUp"));
//	   c1[i-1] = new TCanvas();
//	   for(const auto & h : histo){
//		   ++j;
//		   h->SetTitle((std::to_string(f.first) + "-M mass point").c_str());
//		   h->SetMarkerStyle(20);
//		   h->SetMarkerColor(j);
//		   h->SetLineColor(j);
//		   h->SetLineWidth(2);
//		   if(j==1){
//			   h->Draw("hist");
//			   if(i==1) leg->AddEntry(h,h->GetName(),"l");
//		   }
//		   else {
//			   h->Draw("same");
//			   if(i==1) leg->AddEntry(h,h->GetName(),"p");
//		   }
//
//		   xMin = h->GetXaxis()->GetXmin();
//		   xMax = h->GetXaxis()->GetXmax();
//	   }
//	   j=0;
//
//	   if(i==1) leg->Draw();
//	   c1[i-1]->Print(("pictures/AllinOne_lowM_"+std::to_string(f.first)+"_GeV_mass_point.pdf").c_str());
//	   histo.clear();
//   }

   /*
   i=0;
   TCanvas *cAll_lowM = new TCanvas("cAll_lowM","All mass points lowM",1000,800);
   std::array<int,15> colors = {1,2,4,6,80,95};
   TLegend *lAll_lowM = new TLegend(0.6,0.4,0.9,0.85);
   lAll_lowM->SetLineColor(0);
   for(const auto & f :inF){
	   if(f.first != 300 && f.first != 500 && f.first != 700 && f.first != 900 && f.first != 1100 && f.first != 1300) continue;
	   ++i;
	   cAll_lowM->cd();
	   hLowM[i-1] = (TH1D*) f.second->Get("bbH_Mbb");
	   hLowM[i-1]->SetLineColor(colors.at(i-1));
	   hLowM[i-1]->SetLineWidth(3.5);
	   if(i==1){
		   hLowM[i-1]->GetYaxis()->SetTitle("a.u.");
		   hLowM[i-1]->SetTitle("");
		   hLowM[i-1]->SetAxisRange(0.,4.,"y");
		   hLowM[i-1]->Draw("hist");
	   }
	   else hLowM[i-1]->Draw("hist same");
	   lAll_lowM->AddEntry(hLowM[i-1], (std::to_string(f.first) + " GeV").c_str(),"l" );
   }
   lAll_lowM->SetHeader("Low Mass Scenario");
   lAll_lowM->Draw();
   style.drawStandardTitle();
   cAll_lowM->Print("pictures/AllinOne_lowM_Masses.pdf");

   i=0;
   TCanvas *cAll_highM = new TCanvas("cAll_highM","All mass points highM",1000,800);
   TH1D *hHighM[15];
   TLegend *lAll_highM = new TLegend(0.6,0.4,0.9,0.85);
   lAll_highM->SetLineColor(0);
   for(const auto & f :highMF){
	   if(f.first != 300 && f.first != 500 && f.first != 700 && f.first != 900 && f.first != 1100 && f.first != 1300) continue;
	   ++i;
	   cAll_highM->cd();
	   hHighM[i-1] = (TH1D*) f.second->Get("bbH_Mbb");
	   hHighM[i-1]->SetLineColor(colors.at(i-1));
	   hHighM[i-1]->SetLineWidth(3.5);
	   if(i==1){
		   hHighM[i-1]->GetYaxis()->SetTitle("a.u.");
		   hHighM[i-1]->SetTitle("");
		   hHighM[i-1]->SetAxisRange(0.,12.,"y");
		   hHighM[i-1]->SetAxisRange(hHighM[i-1]->GetXaxis()->GetXmin(),1700.,"x");
		   hHighM[i-1]->Draw("hist");
	   }
	   else hHighM[i-1]->Draw("hist same");
	   lAll_highM->AddEntry(hHighM[i-1], (std::to_string(f.first) + " GeV").c_str(),"l" );
   }
   lAll_highM->SetHeader("High Mass Scenario");
   lAll_highM->Draw();
   style.drawStandardTitle();
   cAll_highM->Print("pictures/AllinOne_highM_Masses.pdf");



   /*********************High M********************/

//   i = 0, j =0, k=0;
//   TCanvas * c2[15];
//   for(const auto & f : highMF){
//	   ++i;
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVDown"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVUp"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVDown"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVUp"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVDown"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVUp"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVDown"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVUp"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVDown"));
//	   histoHighM.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVUp"));
//	   c2[i-1] = new TCanvas();
//	   for(const auto & h : histoHighM){
//		   ++j;
//		   h->SetTitle((std::to_string(f.first) + "-M mass point").c_str());
//		   h->SetMarkerStyle(20);
//		   h->SetMarkerColor(j);
//		   h->SetLineColor(j);
//		   h->SetLineWidth(2);
//		   if(j==1){
//			   h->Draw("hist");
//		   }
//		   else {
//			   h->Draw("same");
//		   }
//	   }
//	   j=0;
//	   if(i==1) leg->Draw();
//	   c2[i-1]->SaveAs(("pictures/AllinOne_highM_"+std::to_string(f.first)+"_GeV_mass_point.pdf").c_str());
//	   histoHighM.clear();
//
//   }
/**/
   //One by one

   /*
   sistTemp(inF,"lowM");
   sistTemp(highMF,"highM");


/*
   //One by one
   TF1 *fit = new TF1("fit","pol0",xMin,xMax);
   TCanvas *singleC[15][15];
   TPad *cPad[15][15];
   TPad *uPad[15][15];
   TPad	*dPad[15][15];
   TLegend *singleL[15][15];
   TPaveText   *text[15][15];
   std::pair<double,double> lnN[15][15];
   std::pair<double,double> elnN[15][15];
   std::map<std::string,std::pair<TH1D*, TH1D*> > singleH;

   i = 0, j =0, k=0;
   for(const auto & f : inF){
	   ++i;
	   TH1D * first = (TH1D*) f.second->Get("bbH_Mbb");
	   f.second->SetTitle((std::to_string(f.first) + "-M mass point").c_str());
	   first->SetLineWidth(2);
	   singleH["SFb"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVUp"));
	   singleH["SFudsg"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVUp"));
	   singleH["JER"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_JER_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_JER_13TeVUp"));
	   singleH["JES"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVUp"));
	   singleH["PU"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVUp"));
	   singleH["pT trig Eff"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVUp"));
	   for(const auto & p: singleH){
		   ++j;
		   singleC[i-1][j-1] 	= new TCanvas();

		   std::string name2 = "box1_" + std::to_string(i) + "_" + std::to_string(j);
		   cPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.35,1,1);
		   cPad[i-1][j-1]->SetBottomMargin(0.0);
		   cPad[i-1][j-1]->SetLeftMargin(0.16);
		   cPad[i-1][j-1]->SetRightMargin(0.05);
		   cPad[i-1][j-1]->Draw();
		   cPad[i-1][j-1]->cd();
		   singleL[i-1][j-1] 	= new TLegend(0.6,0.7,0.9,0.9);
		   (p.second).second->SetTitle((std::to_string(f.first)+ " GeV mass point, " + p.first+" variation").c_str());

		   (p.second).first->SetMarkerColor(kRed);
		   (p.second).second->SetMarkerColor(kBlue);
		   (p.second).first->SetMarkerStyle(20);
		   (p.second).second->SetMarkerStyle(20);

		   (p.second).second->Draw();
		   (p.second).first->Draw("same");
		   first->Draw("hist same");

		   singleL[i-1][j-1]->AddEntry((p.second).first,("-2#sigma variation " + p.first).c_str(),"p");
		   singleL[i-1][j-1]->AddEntry((p.second).second,("+2#sigma variation " + p.first).c_str(),"p");
		   singleL[i-1][j-1]->Draw();

		   singleC[i-1][j-1]->cd();
		   name2 = "box2_" + std::to_string(i) + "_" + std::to_string(j);
		   uPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.21,1,0.35);
		   uPad[i-1][j-1]->SetBottomMargin(0.0);
		   uPad[i-1][j-1]->SetTopMargin(0.);
		   uPad[i-1][j-1]->SetLeftMargin(cPad[i-1][j-1]->GetLeftMargin());
		   uPad[i-1][j-1]->SetRightMargin(cPad[i-1][j-1]->GetRightMargin());
		   uPad[i-1][j-1]->Draw();
		   uPad[i-1][j-1]->cd();

		   TH1D *centrU = (TH1D*) first->Clone("centrU");
		   lnN[i-1][j-1].first = centrU->Integral() / (p.second).first -> Integral();
		   centrU->Divide((p.second).first);
		   centrU->SetTitle("");
		   centrU->SetMarkerStyle(20);
		   SetBottomStyle(centrU);
//		   centrU->Fit("fit","N");
//		   lnN[i-1][j-1].first = fit->GetParameter(0);
//		   elnN[i-1][j-1].first = fit->GetParError(0);

		   centrU->Draw();

		   TLine *horizLine = new TLine(centrU->GetXaxis()->GetXmin(),1,centrU->GetXaxis()->GetXmax(),1);
		   horizLine -> SetLineStyle(2);
		   horizLine -> Draw();

		   singleC[i-1][j-1]->cd();
		   name2 = "box3_" + std::to_string(i) + "_" + std::to_string(j);
		   dPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.,1,0.21);
		   dPad[i-1][j-1]->SetBottomMargin(0.33);
		   dPad[i-1][j-1]->SetTopMargin(0.);
		   dPad[i-1][j-1]->SetLeftMargin(cPad[i-1][j-1]->GetLeftMargin());
		   dPad[i-1][j-1]->SetRightMargin(cPad[i-1][j-1]->GetRightMargin());
		   dPad[i-1][j-1]->Draw();
		   dPad[i-1][j-1]->cd();

		   TH1D *centrD = (TH1D*) first->Clone("centrD");
		   lnN[i-1][j-1].second = centrD->Integral() / (p.second).second -> Integral();
		   centrD->Divide((p.second).second);
		   centrD->SetTitle("");
		   centrD->SetMarkerStyle(20);
		   SetBottomStyle(centrD);
//		   centrD->Fit("fit","N");
//		   lnN[i-1][j-1].second = fit->GetParameter(0);
//		   elnN[i-1][j-1].second = fit->GetParError(0);
		   centrD->Draw();

		   TLine *horizLine2 = new TLine(centrD->GetXaxis()->GetXmin(),1,centrD->GetXaxis()->GetXmax(),1);
		   horizLine2 -> SetLineStyle(2);
		   horizLine2 -> Draw();

		   cPad[i-1][j-1]->cd();
		   text[i-1][j-1]		= new TPaveText  (0.6,0.5,0.9,0.7,"NDC");
		   text[i-1][j-1]->SetFillColor(0);
		   text[i-1][j-1]->SetShadowColor(0);
		   text[i-1][j-1]->AddText(("lnN +2#sigma: " + std::to_string(lnN[i-1][j-1].first)).c_str());
		   text[i-1][j-1]->AddText(("lnN -2#sigma: " + std::to_string(lnN[i-1][j-1].second)).c_str());
		   text[i-1][j-1]->AddText(("lnN #pm1#sigma: " + std::to_string(1. + std::abs((lnN[i-1][j-1].first - lnN[i-1][j-1].second)/4.))).c_str());
		   //("lnN +2\sigma: " + std::to_string(lnN[i-1][j-1].first) + " \pm " + std::to_string(elnN[i-1][j-1].first)  + " \splitline lnN -2#sigma: " + std::to_string(lnN[i-1][j-1].second) + " #pm " + std::to_string(elnN[i-1][j-1].second)).c_str());
		   text[i-1][j-1]->Draw();
		   std::string name = "pictures/"+std::to_string(f.first)+ "_GeV_mass_point_" + p.first+"_variation.pdf";
		   singleC[i-1][j-1]->SaveAs(name.c_str());
	   }
	   j=0;


   }
/**/
   
}

void SetBottomStyle(TH1 *hRatio){

	hRatio -> SetNdivisions(505,"YZ");
	hRatio -> SetNdivisions(510,"X");
	hRatio -> SetTickLength(0.04,"YZ");
	hRatio -> SetTickLength(0.07,"X");
	hRatio -> SetTitleSize(0.06*7/3,"XYZ");
	hRatio -> SetTitleFont(42,"XYZ");

	hRatio -> SetLabelFont(42,"XYZ");
	hRatio -> SetLabelSize(0.05*7/3,"XYZ");
	hRatio -> SetLabelOffset(0.01,"XYZ");

	hRatio -> GetYaxis() -> SetTitleOffset(0.6);
	hRatio -> GetXaxis() -> SetTitleOffset(1.);
	hRatio -> GetYaxis() -> SetRangeUser(0.,2.);


}

void selectedTemplates(const std::map<int,TFile*> & inF,const std::string &regime, const std::vector<int> & points){
	bool lowM = regime.find("low") != std::string::npos;


	TCanvas *c = new TCanvas(("c"+regime).c_str(),("All mass points " + regime).c_str(),1000,800);
	std::array<int,15> colors = {1,2,4,6,80,95};
	double legXMin = 0.6;
	double legXMax = 0.95;
	double legYMin = 0.4;
	double legYMax = 0.85;
	TLegend *leg = new TLegend(0.69,0.4,0.92,0.85);
	leg->SetEntrySeparation(0.01);
	leg->SetLineColor(0);

	TH1D *h[15];
	int i=0;
	for(const auto & point : points){
		++i;
		c->cd();
		auto it = inF.find(point);
		h[i-1] = (TH1D*) it->second->Get("templates/bbH_Mbb_VIS");
		h[i-1]->SetLineColor(colors.at(i-1));
		h[i-1]->SetLineWidth(3.5);
		if(i==1){
			h[i-1]->GetYaxis()->SetTitle("a.u.");
			h[i-1]->SetTitle("");
			if(lowM) h[i-1]->SetAxisRange(0.,9.,"y");
			else h[i-1]->SetAxisRange(0.,9.,"y");
			h[i-1]->SetAxisRange(h[i-1]->GetXaxis()->GetXmin(),1700.,"x");
			h[i-1]->Draw("hist");
		}
		else h[i-1]->Draw("hist same");
		leg->AddEntry(h[i-1], (std::to_string(point) + " GeV").c_str(),"l" );
	}
	std::string legHeader;
	if(lowM) legHeader = "Low Mass Scenario";
	else legHeader = "High Mass Scenario";
	leg->SetHeader(legHeader.c_str());
	leg->Draw();
	style.drawStandardTitle();
	std::string cPrinter = "pictures/AllinOne_" + regime + "_Masses.pdf";
	c->Print(cPrinter.c_str());

//	for(const auto & f :inF){
//		if(f.first != 300 && f.first != 500 && f.first != 700 && f.first != 900 && f.first != 1100 && f.first != 1300) continue;
//		++i;
//		c->cd();
//		h[i-1] = (TH1D*) f.second->Get("templates/bbH_Mbb_VIS");
//		h[i-1]->SetLineColor(colors.at(i-1));
//		h[i-1]->SetLineWidth(3.5);
//		if(i==1){
//			h[i-1]->GetYaxis()->SetTitle("a.u.");
//			h[i-1]->SetTitle("");
//			h[i-1]->SetAxisRange(0.,4.,"y");
//			h[i-1]->Draw("hist");
//		}
//		else h[i-1]->Draw("hist same");
//		leg->AddEntry(h[i-1], (std::to_string(f.first) + " GeV").c_str(),"l" );
//	}
//	std::string legHeader;
//	if(regime.find("low")) legHeader = "Low Mass Scenario";
//	else legHeader = "High Mass Scenario"
//	leg->SetHeader(legHeader.c_str());
//	leg->Draw();
//	style.drawStandardTitle();
//	std::string cPrinter = "pictures/AllinOne_" + regime + "_Masses.pdf";
//	c->Print(cPrinter.c_str());
}

void allInOne(const std::map<int,TFile*> & inF,const std::string & regime){
	double xMin = 0, xMax = 0;
	std::vector<TH1D*> histo;		//vector of syst. histograms.

	TCanvas * c1[15];
	TLegend *leg = new TLegend(0.3,0.3,0.9,0.85);
	leg->SetBorderSize(0);

	int i = 0, j =0;
	   for(const auto & f : inF){
		   ++i;
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_VIS"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFb_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFb_VIS_13TeVUp"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFl_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFl_VIS_13TeVUp"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JES_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JES_VIS_13TeVUp"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JER_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JER_VIS_13TeVUp"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PU_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PU_VIS_13TeVUp"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PtEff_VIS_13TeVDown"));
		   histo.push_back((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PtEff_VIS_13TeVUp"));
		   c1[i-1] = new TCanvas();
		   for(const auto & h : histo){
			   ++j;
			   h->SetTitle((std::to_string(f.first) + "-M mass point").c_str());
			   h->SetMarkerStyle(20);
			   h->SetMarkerColor(j);
			   h->SetLineColor(j);
			   h->SetLineWidth(2);
			   if(j==1){
				   h->Draw("hist");
				   if(i==1) leg->AddEntry(h,h->GetName(),"l");
			   }
			   else {
				   h->Draw("same");
				   if(i==1) leg->AddEntry(h,h->GetName(),"p");
			   }

			   xMin = h->GetXaxis()->GetXmin();
			   xMax = h->GetXaxis()->GetXmax();
		   }
		   j=0;

		   if(i==1) leg->Draw();
		   std::string canva_name = "pictures/AllinOne_" + regime + "_" + std::to_string(f.first) + "_GeV_mass_point.pdf";
		   c1[i-1]->Print(canva_name.c_str());
		   histo.clear();
	   }
}

void sistTemp(const std::map<int,TFile*> & inF,const std::string & regime){
	   //One by one
	   TCanvas *singleC[15][15];
	   TPad *cPad[15][15];
	   TPad *uPad[15][15];
	   TPad	*dPad[15][15];
	   TLegend *singleL[15][15];
	   TPaveText   *text[15][15];
	   std::pair<double,double> lnN[15][15];
	   std::pair<double,double> elnN[15][15];
	   std::map<std::string,std::pair<TH1D*, TH1D*> > singleH;

	   int i = 0, j =0, k=0;
	   for(const auto & f : inF){
		   ++i;
		   TH1D * first = (TH1D*) f.second->Get("templates/bbH_Mbb_VIS");
		   f.second->SetTitle((std::to_string(f.first) + "-M mass point").c_str());
		   first->SetLineWidth(2);
		   singleH["SFb"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFb_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFb_VIS_13TeVUp"));
		   singleH["SFudsg"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFl_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_SFl_VIS_13TeVUp"));
		   singleH["JER"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JER_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JER_VIS_13TeVUp"));
		   singleH["JES"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JES_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_JES_VIS_13TeVUp"));
		   singleH["PU"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PU_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PU_VIS_13TeVUp"));
		   singleH["pT trig Eff"] = std::make_pair((TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PtEff_VIS_13TeVDown"),(TH1D*) f.second->Get("templates/bbH_Mbb_CMS_PtEff_VIS_13TeVUp"));
		   for(const auto & p: singleH){
			   ++j;
			   singleC[i-1][j-1] 	= new TCanvas();

			   std::string name2 = "box1_" + std::to_string(i) + "_" + std::to_string(j);
			   cPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.35,1,1);
			   cPad[i-1][j-1]->SetBottomMargin(0.0);
			   cPad[i-1][j-1]->SetLeftMargin(0.16);
			   cPad[i-1][j-1]->SetRightMargin(0.05);
			   cPad[i-1][j-1]->Draw();
			   cPad[i-1][j-1]->cd();
			   double lxmin = 0.6, lxmax = 0.9, lymin = 0.7, lymax = 0.9;
			   if(f.first > 900) {lxmin = 0.25; lxmax = 0.55;}
			   singleL[i-1][j-1] 	= new TLegend(lxmin,lymin,lxmax,lymax);
			   (p.second).second->SetTitle((std::to_string(f.first)+ " GeV mass point, " + p.first+" variation").c_str());

			   (p.second).first->SetMarkerColor(kRed);
			   (p.second).second->SetMarkerColor(kBlue);
			   (p.second).first->SetMarkerStyle(20);
			   (p.second).second->SetMarkerStyle(20);

			   (p.second).second->Draw();
			   (p.second).first->Draw("same");
			   first->Draw("hist same");

			   singleL[i-1][j-1]->AddEntry((p.second).first,("-2#sigma variation " + p.first).c_str(),"p");
			   singleL[i-1][j-1]->AddEntry((p.second).second,("+2#sigma variation " + p.first).c_str(),"p");
			   singleL[i-1][j-1]->Draw();

			   singleC[i-1][j-1]->cd();
			   name2 = "box2_" + std::to_string(i) + "_" + std::to_string(j);
			   uPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.21,1,0.35);
			   uPad[i-1][j-1]->SetBottomMargin(0.0);
			   uPad[i-1][j-1]->SetTopMargin(0.);
			   uPad[i-1][j-1]->SetLeftMargin(cPad[i-1][j-1]->GetLeftMargin());
			   uPad[i-1][j-1]->SetRightMargin(cPad[i-1][j-1]->GetRightMargin());
			   uPad[i-1][j-1]->Draw();
			   uPad[i-1][j-1]->cd();

			   TH1D *centrU = (TH1D*) first->Clone("centrU");
			   lnN[i-1][j-1].first = centrU->Integral() / (p.second).first -> Integral();
			   centrU->Divide((p.second).first);
			   centrU->SetTitle("");
			   centrU->SetMarkerStyle(20);
			   SetBottomStyle(centrU);

			   centrU->Draw();

			   TLine *horizLine = new TLine(centrU->GetXaxis()->GetXmin(),1,centrU->GetXaxis()->GetXmax(),1);
			   horizLine -> SetLineStyle(2);
			   horizLine -> Draw();

			   singleC[i-1][j-1]->cd();
			   name2 = "box3_" + std::to_string(i) + "_" + std::to_string(j);
			   dPad[i-1][j-1]		= new TPad(name2.c_str(),"",0.,0.,1,0.21);
			   dPad[i-1][j-1]->SetBottomMargin(0.33);
			   dPad[i-1][j-1]->SetTopMargin(0.);
			   dPad[i-1][j-1]->SetLeftMargin(cPad[i-1][j-1]->GetLeftMargin());
			   dPad[i-1][j-1]->SetRightMargin(cPad[i-1][j-1]->GetRightMargin());
			   dPad[i-1][j-1]->Draw();
			   dPad[i-1][j-1]->cd();

			   TH1D *centrD = (TH1D*) first->Clone("centrD");
			   lnN[i-1][j-1].second = centrD->Integral() / (p.second).second -> Integral();
			   centrD->Divide((p.second).second);
			   centrD->SetTitle("");
			   centrD->SetMarkerStyle(20);
			   SetBottomStyle(centrD);
			   centrD->Draw();

			   TLine *horizLine2 = new TLine(centrD->GetXaxis()->GetXmin(),1,centrD->GetXaxis()->GetXmax(),1);
			   horizLine2 -> SetLineStyle(2);
			   horizLine2 -> Draw();

			   cPad[i-1][j-1]->cd();
			   double xmin = 0.6, ymin = 0.5, xmax = 0.9, ymax = 0.7;
			   if (f.first > 900) {xmin = 0.25; xmax = 0.55;}
			   text[i-1][j-1]		= new TPaveText  (xmin,ymin,xmax,ymax,"NDC");
			   text[i-1][j-1]->SetFillColor(0);
			   text[i-1][j-1]->SetShadowColor(0);
			   text[i-1][j-1]->AddText(("lnN +2#sigma: " + std::to_string(lnN[i-1][j-1].first)).c_str());
			   text[i-1][j-1]->AddText(("lnN -2#sigma: " + std::to_string(lnN[i-1][j-1].second)).c_str());
			   text[i-1][j-1]->AddText(("lnN #pm1#sigma: " + std::to_string(1. + std::abs((lnN[i-1][j-1].first - lnN[i-1][j-1].second)/4.))).c_str());
			   //("lnN +2\sigma: " + std::to_string(lnN[i-1][j-1].first) + " \pm " + std::to_string(elnN[i-1][j-1].first)  + " \splitline lnN -2#sigma: " + std::to_string(lnN[i-1][j-1].second) + " #pm " + std::to_string(elnN[i-1][j-1].second)).c_str());
			   text[i-1][j-1]->Draw();
			   std::string name = "pictures/"+std::to_string(f.first)+ "_GeV" + "_mass_point_" +regime + "_" + p.first+"_variation.pdf";
			   singleC[i-1][j-1]->SaveAs(name.c_str());
		   }
		   j=0;
	   }
}
