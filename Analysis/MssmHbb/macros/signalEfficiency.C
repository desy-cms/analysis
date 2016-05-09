#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"

void signalEfficiency()
{
   gStyle->SetOptStat(0);
   TH1::SetDefaultSumw2();

   const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

   std::map<int,TFile* > inFlowM;
   std::map<int,TFile* > inFhighM;

   inFlowM[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
//   inFlowM[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");


   inFhighM[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");


   //LowM
   TGraphErrors *Eff = new TGraphErrors(inFlowM.size());
   Eff->SetMarkerStyle(20);
   Eff->SetMarkerSize(1.1);
   Eff->SetTitle("Signal Efficiency; di-jet M, [GeV]; #epsilon");

   //highM
   TGraphErrors *EffhighM = new TGraphErrors(inFhighM.size());
   EffhighM->SetMarkerStyle(20);
   EffhighM->SetMarkerColor(2);
   EffhighM->SetMarkerSize(1.1);
   EffhighM->SetTitle("Signal Efficiency; di-jet M, [GeV]; #epsilon");

   TCanvas * c1 = new TCanvas("c1","",700,800);
   TLegend *leg = new TLegend(0.15,0.7,0.42,0.85);
   leg->SetBorderSize(0);

   int i = 0;
   for(const auto & f : inFlowM){
	   ++i;

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   TH1* denum =  (TH1*) f.second->Get("distributions/NumberOfGenEvents_afterMHat");
	   TH1* numer =  (TH1*) f.second->Get("distributions/NumberOfSelectedEvents");
	   TH1 *integ = (TH1*) f.second->Get("distributions/diJet_m");
	   Eff->SetPoint(i,f.first,(double) integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))/denum->GetMean());
   }
   i=0;

   for(const auto & f : inFhighM){
	   ++i;

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   TH1* denum =  (TH1*) f.second->Get("distributions/NumberOfGenEvents_afterMHat");
	   TH1* numer =  (TH1*) f.second->Get("distributions/NumberOfSelectedEvents");
	   TH1 *integ = (TH1*) f.second->Get("distributions/diJet_m");
	   EffhighM->SetPoint(i,f.first,(double) integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))/denum->GetMean());
   }
   EffhighM->Draw("AP");
   Eff->Draw("Psame");
   
   leg->AddEntry(Eff,"Low Mass","p");
   leg->AddEntry(EffhighM,"High Mass","p");
   leg->Draw();

   //Sensitivity
   TFile *inFBgLowM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_QCD.root");
   TFile *inFBgHighM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_highM_QCD.root");
   TH1D	*hBgLowM = (TH1D*) inFBgLowM->Get("distributions/diJet_m");
   TH1D	*hBgHighM = (TH1D*) inFBgHighM->Get("distributions/diJet_m");

   //LowM
   TGraphErrors *Sensitivity = new TGraphErrors(inFlowM.size());
   Sensitivity->SetMarkerStyle(20);
   Sensitivity->SetMarkerSize(1.1);
   Sensitivity->SetTitle("Signal Sensitivity = S/#sqrt{Bg}; di-jet M, [GeV]; #epsilon");

   //highM
   TGraphErrors *SensitivityHighM = new TGraphErrors(inFhighM.size());
   SensitivityHighM->SetMarkerStyle(20);
   SensitivityHighM->SetMarkerColor(2);
   SensitivityHighM->SetMarkerSize(1.1);
   SensitivityHighM->SetTitle("Signal Sensitivity = S/#sqrt{Bg}; di-jet M, [GeV]; #epsilon");

   TCanvas * c2 = new TCanvas("c2","",700,800);
   TLegend *leg2 = new TLegend(0.15,0.7,0.42,0.85);
   leg2->SetBorderSize(0);

   i = 0;
   std::cout<<"Low M:"<<std::endl;
   for(const auto & f : inFlowM){
	   ++i;
	   TH1* numer =  (TH1*) f.second->Get("distributions/NumberOfSelectedEvents");
	   TH1 *s = (TH1*) f.second->Get("distributions/diJet_m");

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   double signalErr_ = 0;
	   double signal_ = s->IntegralAndError(s->FindBin(low_border),s->FindBin(up_border),signalErr_);

	   double bgErr_ = 0;
	   double bg_ = hBgLowM->IntegralAndError(hBgLowM->FindBin(low_border),hBgLowM->FindBin(up_border),bgErr_);

	   double sens = signal_/std::sqrt(bg_);
	   double sens_err = std::sqrt(1./bg_ * signalErr_ * signalErr_ + signal_*signal_/(4. * bg_*bg_*bg_) * bgErr_* bgErr_);
	   Sensitivity->SetPoint(i,f.first,sens);
	   Sensitivity->SetPointError(i,0,sens_err);


	   std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<signal_<<" Bg_no_w= "<<hBgLowM->GetEntries()<<" +w= "<<bg_<<std::endl;
	   std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<s->IntegralAndError(1,s->GetNbinsX(),signalErr_)<<" Bg_no_w= "<<hBgLowM->GetEntries()<<" +w= "<<hBgLowM->IntegralAndError(1,s->GetNbinsX(),bgErr_)<<" \n"<<std::endl;
   }
   i = 0;
std::cout<<"High M: "<<std::endl;
   for(const auto & f : inFhighM){
	   ++i;
	   TH1* numer =  (TH1*) f.second->Get("distributions/NumberOfSelectedEvents");
	   TH1 *s = (TH1*) f.second->Get("distributions/diJet_m");

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   double signalErr_ = 0;
	   double signal_ = s->IntegralAndError(s->FindBin(low_border),s->FindBin(up_border),signalErr_);

	   double bgErr_ = 0;
	   double bg_ = hBgHighM->IntegralAndError(hBgLowM->FindBin(low_border),hBgLowM->FindBin(up_border),bgErr_);

	   double sens = signal_/std::sqrt(bg_);
	   double sens_err = std::sqrt(1./bg_ * signalErr_ * signalErr_ + signal_*signal_/(4. * bg_*bg_*bg_) * bgErr_* bgErr_);
	   SensitivityHighM->SetPoint(i,f.first,sens);
	   SensitivityHighM->SetPointError(i,0,sens_err);

        std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<signal_<<" Bg_no_w= "<<hBgHighM->GetEntries()<<" +w= "<<bg_<<std::endl;  
 }
   Sensitivity->Draw("AP");
   SensitivityHighM->Draw("Psame");

   leg2->AddEntry(Sensitivity,"Low Mass","p");
   leg2->AddEntry(SensitivityHighM,"High Mass","p");
   leg2->Draw();


   c1->SaveAs("pictures/signalEfficiency.pdf");
   c2->SaveAs("pictures/signalSensitivity.pdf");
/**/
}
