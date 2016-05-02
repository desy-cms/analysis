#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"

void templates()
{
   gStyle->SetOptStat(0);
   const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

   TFile out((cmsswBase + "/src/Analysis/MssmHbb/output/Templates.root").c_str(),"RECREATE");
   std::map<int,TFile* > inF;
   std::vector<TH1D*> histo;

   inF[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
//   inF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");

   TCanvas * c1 = new TCanvas("c1","",700,800);
   c1->Divide(1,2);
   c1->cd(1);
   c1 -> SetLogy();

   int i = 0, j =0;
   for(const auto & f : inF){
	   ++i;
	   histo.push_back((TH1D*) f.second->Get("Mbb"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVDown"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVUp"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVDown"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVUp"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVDown"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_JES_13TeVUp"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVDown"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PU_13TeVUp"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVDown"));
	   histo.push_back((TH1D*) f.second->Get("bbH_Mbb_CMS_PtEff_13TeVUp"));
	   for(const auto & h : histo){
		   ++j;
		   h->SetMarkerStyle(20);
		   h->SetMarkerColor(1+j);
		   h->Draw("same");
	   }


//	   Eff->SetPoint(i,)
   }
   
}
