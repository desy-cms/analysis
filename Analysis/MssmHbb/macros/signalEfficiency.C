#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"

void signalEfficiency()
{
   gStyle->SetOptStat(0);
   const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

   TFile out((cmsswBase + "Analysis/MssmHbb/output/SignalEfficiency.root").c_str());
   std::map<int,TFile* > inF;

   inF[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inF[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   inF[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inF[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   inF[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   inF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   inF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   inF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   inF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");

   TGraphErrors *Eff = new TGraphErrors(inF.size());
   Eff->SetMarkerStyle(20);
   Eff->SetMarkerSize(1.1);
   Eff->SetTitle("Signal Efficiency; di-jet M, [GeV]; #epsilon");

   TCanvas * c1 = new TCanvas("c1","",700,800);
   c1->Divide(1,2);
   c1->cd(1);
   c1 -> SetLogy();

   int i = 0;
   for(const auto & f : inF){
	   ++i;
	   TH1* denum =  (TH1*) f.second->Get("distributions/NumberOfGenEvents_afterMHat");
	   TH1* numer =  (TH1*) f.second->Get("distributions/NumberOfSelectedEvents");
	   std::cout<<f.first<<" "<<numer->GetMean()/denum->GetMean()<<std::endl;
	   Eff->SetPoint(i,f.first,(double) numer->GetMean()/denum->GetMean());
	   std::cout<<f.first<<" sel: "<<numer->GetMean()<<" tot: "<<denum->GetMean()<<" eff: "<<numer->GetMean()/denum->GetMean()<<std::endl;

//	   Eff->SetPoint(i,)
   }
   Eff->Draw("AP");
   
}
