#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"
#include "TGraph.h"
#include "ResultPoint.cpp"
#include "Analysis/MssmHbb/macros/sensitivity/Measurement.cpp"

const double Lumi = 2621; //pb-1

double upLimitCalc(const double & Bg, const double & eff){
	const double fUL  = 1.96; // For Gaussian stat.
	double ul = fUL * sqrt(Bg) / eff / Lumi;
	return ul;
}

void upLimits()
{
   gStyle->SetOptStat(0);
   TH1::SetDefaultSumw2();

   const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

   std::vector<std::pair<int, double> > upLimitsExpectedLow = { std::pair<int, double>(300,44.81),
		   	   	   	   	   	   	   	   	   	   	   	   	   	 std::pair<int, double>(400,16.19),
															 std::pair<int, double>(500,6.97),
															 std::pair<int, double>(600, 3.58),
															 std::pair<int, double>(700,2.52),
															 std::pair<int, double>(900,1.57) };
   std::vector<std::pair<int, double> > upLimitsExpectedHigh = { std::pair<int, double>(300,315.62),
		   	   	   	   	   	   	   	   	   	   	   	   	   	 std::pair<int, double>(400,37.88),
															 std::pair<int, double>(500,8.95),
															 std::pair<int, double>(600, 4.36),
															 std::pair<int, double>(700,3.36),
															 std::pair<int, double>(900,1.96) };


   std::map<int,ResultPoint * > p8TeV;
   p8TeV[300] = new ResultPoint(8,300,21661,0.0064,2.2951,6.1);
   p8TeV[400] = new ResultPoint(8,400,5970,0.0061,1.2644,2.7);
   p8TeV[500] = new ResultPoint(8,500,1849,0.0051,0.8438,1.7);
   p8TeV[600] = new ResultPoint(8,600,643,0.0039,0.6553,1.3);
   p8TeV[700] = new ResultPoint(8,700,275,0.0029,0.5689,1.1);
   p8TeV[900] = new ResultPoint(8,900,52,0.0014,0.5162,1.0);

   p8TeV[900] = new ResultPoint(8,900,52,0.0014,0.5162,1.0);

   std::map<int,ResultPoint * > p13TeV_low;
   std::map<int,ResultPoint * > p13TeV_high;

   std::map<int,TFile* > inFlowM;
   std::map<int,TFile* > inFhighM;

   inFlowM[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inFlowM[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFlowM[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFlowM[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFlowM[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");

   inFhighM[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inFhighM[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFhighM[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFhighM[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
   inFhighM[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");

   for(const auto & pair : upLimitsExpectedLow){

	   double low_border = pair.first*0.8;
	   double up_border	 = pair.first*1.2;

	   TH1D *hBg = (TH1D*) inFlowM[pair.first]->Get("QCD_Mbb");
	   double nBg = hBg->Integral( hBg->FindBin(low_border), hBg->FindBin(up_border) );

	   TH1D *hSig = (TH1D*) inFlowM[pair.first]->Get("bbH_Mbb");
	   TH1D *hSigAll = (TH1D*) inFlowM[pair.first]->Get("distributions/NumberOfGenEvents_afterMHat_rewPU");
	   double eff = hSig->Integral() / (hSigAll->GetMean() * Lumi / hSigAll->GetMean());
	   double tot = (hSigAll->GetMean() * Lumi / hSigAll->GetMean());
	   double upLim = upLimitCalc(nBg,eff);
	   p13TeV_low[pair.first] = new ResultPoint(13,pair.first,nBg,hSig->Integral(),tot,upLim,pair.second);

	   std::cout<<"Low PUNZI: "<<hSig->Integral( hSig->FindBin(low_border), hSig->FindBin(up_border) ) / std::sqrt(nBg)<<std::endl;

   }
   for(const auto & pair : upLimitsExpectedHigh){

	   double low_border = pair.first*0.8;
	   double up_border	 = pair.first*1.2;

	   //high M part
	   TH1D* hBg = (TH1D*) inFhighM[pair.first]->Get("QCD_Mbb");
	   double nBg = hBg->Integral( hBg->FindBin(low_border), hBg->FindBin(up_border) );

	   TH1D* hSig = (TH1D*) inFhighM[pair.first]->Get("bbH_Mbb");
	   TH1D* hSigAll = (TH1D*) inFhighM[pair.first]->Get("distributions/NumberOfGenEvents_afterMHat_rewPU");
	   double eff = hSig->Integral() / (hSigAll->GetMean() * Lumi / hSigAll->GetMean());
	   double tot = (hSigAll->GetMean() * Lumi / hSigAll->GetMean());
	   double upLim = upLimitCalc(nBg,eff);
	   p13TeV_high[pair.first] = new ResultPoint(13,pair.first,nBg,hSig->Integral(  ),tot,upLim,pair.second);
	   std::cout<<"High PUNZI: "<<hSig->Integral( hSig->FindBin(low_border), hSig->FindBin(up_border) ) / std::sqrt(nBg)<<std::endl;
   }

   std::cout<<" Low Mass "<<std::endl;
   std::cout<<"        MASS "<<" BACKGROUND "<<" SELECTED_SIGNAL "<<" TOTAL SIGNAL "<<" SIGNAL_EFFICIENCY "<<" XSec_BG = B / (L * eff) "<<" XSec_SIGNAL "<<" SIGNAL_BR "<<"  ulPred "<<"    ulData "<<"   f_p "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   p13TeV_low[pair.first]->Print();
   }
   std::cout<<" High Mass "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   p13TeV_high[pair.first]->Print();
   }
   std::cout<<" 8 TeV Results "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   p8TeV[pair.first]->Print();
   }
   std::cout<<" 13 TeV vs 8 TeV block"<<std::endl;
   std::cout<<" XSec_BG = B / (L * eff): (13 TeV)/(8 TeV) "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   std::cout<<"M: "<<pair.first<<" Low: "<<p13TeV_low[pair.first]->XSec_Bg() / p8TeV[pair.first]->XSec_Bg()<<" High: "<<p13TeV_high[pair.first]->XSec_Bg() / p8TeV[pair.first]->XSec_Bg()<<std::endl;
   }
   std::cout<<" XSec_SIGNAL = A*BR_{A} + H*BR_{H}: (13 TeV)/(8 TeV) "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   std::cout<<"M: "<<pair.first<<" Low: "<<p13TeV_low[pair.first]->XSec_Signal() / p8TeV[pair.first]->XSec_Signal()<<" High: "<<p13TeV_high[pair.first]->XSec_Signal() / p8TeV[pair.first]->XSec_Signal()<<std::endl;
   }
   std::cout<<" BR A->bb: (13 TeV)/(8 TeV) "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   std::cout<<"M: "<<pair.first<<" Low: "<<p13TeV_low[pair.first]->Sig_BR() / p8TeV[pair.first]->Sig_BR()<<" High: "<<p13TeV_high[pair.first]->Sig_BR() / p8TeV[pair.first]->Sig_BR()<<std::endl;
   }
   std::cout<<" SIG_EFF/SQRT(BG): (13 TeV) , (8 TeV) , (13 TeV)/(8 TeV) "<<std::endl;
   for(const auto & pair : upLimitsExpectedLow){
	   std::cout<<"M: "<<pair.first<<" Low: 13TeV="<<p13TeV_low[pair.first]->PunziSignificance()<<
			   " 8TeV = "<< p8TeV[pair.first]->PunziSignificance()<<" 13/8: "<<
			   p13TeV_low[pair.first]->PunziSignificance()/p8TeV[pair.first]->PunziSignificance()<<
			   " High: "<<p13TeV_high[pair.first]->PunziSignificance()<<
			   " 13/8: "<<
			   p13TeV_high[pair.first]->PunziSignificance()/p8TeV[pair.first]->PunziSignificance()<<std::endl;
   }
   /*
   //Compare 8 and 13 TeV
   std::cout<<"f_p = ulData / ulPred"<<std::endl;
   for(const auto & pair : upLimitsExpected){
	   std::cout<<"8 TeV: "<<p8TeV[pair.first]->f_p_<<" 13 TeV: "<<p13TeV_low[pair.first]->f_p_<<std::endl;
   }

   std::cout<<"XSec_B = B / (L * eff)"<<std::endl;
   for(const auto & pair : upLimitsExpected){
	   std::cout<<"8 TeV: "<<p8TeV[pair.first]->XSec_B_<<" 13 TeV: "<<p13TeV_low[pair.first]->XSec_B_<<std::endl;
   }
   std::cout<<"B / L"<<std::endl;
   for(const auto & pair : upLimitsExpected){
	   std::cout<<"8 TeV: "<<p8TeV[pair.first]->bg_ / p8TeV[pair.first]->lumi_<<" 13 TeV: "<<p13TeV_low[pair.first]->bg_ / p13TeV_low[pair.first]->lumi_<<std::endl;
   }

//   c1->SaveAs("pictures/signalEfficiency.pdf");
//   c2->SaveAs("pictures/signalSensitivity.pdf");
/**/
}
