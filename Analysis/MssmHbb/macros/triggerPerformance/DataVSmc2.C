#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "TEfficiency.h"

TEfficiency * GetEfficiency(TFile * f, const std::string & name_num, const std::string & name_den);
void BayesianErrors(TEfficiency *h);
void drawRatio(TH1D *, TF1 *,TCanvas *,std::string );
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);

int DataVSmc2(){

	TH1::SetDefaultSumw2();

	TFile *fDataLowM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_Run2015D-16Dec2015-v1.root");
	TFile *fDataHighM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_highM_Run2015D-16Dec2015-v1.root");

   std::map<int,TFile* > SignalMC;
   SignalMC[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");

   //BG MC:
   TFile *BgMC = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD.root");
   std::map<std::string,TFile* > pTHat;
   pTHat["50to80"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.root");
   pTHat["80to120"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.root");
   pTHat["120to170"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.root");
   pTHat["170to300"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.root");
   pTHat["300to470"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.root");
   pTHat["470to600"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["600to800"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["800to1000"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["1000to1400"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["1400to1800"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["1800to2400"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["2400to3200"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["3200toInf"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8.root");


   std::string error_mode = "";

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup Ratio
   RatioPlots ratio(PRIVATE);
   ratio.SetRatioRange(0.5,1.5);
   ratio.SetRatioTitle("Data/Fit");
   TH1 *nullH = nullptr;





   //........................LOW M..................................//
   //........................PT.....................................//
	TCanvas *oneDFitLowM = new TCanvas("oneDFitLowM","Factor Low M",1000,800);
	TH1D *pf100Eff_rd_num = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	TLegend *leg = new TLegend(0.85,0.5,0.99,0.99);
	TH1D *pf100Eff_rd_den = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	TEfficiency *pf100Eff = new TEfficiency(*pf100Eff_rd_num,*pf100Eff_rd_den);
	pf100Eff->SetStatisticOption(TEfficiency::kBUniform);
	pf100Eff->SetPosteriorMode(1);
	pf100Eff->UsesBayesianStat();
	pf100Eff->SetMarkerStyle(20);
	TH1D *pf100Eff_bg_num = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	TH1D *pf100Eff_bg_den = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	TEfficiency *pf100Eff_qcd = new TEfficiency(*pf100Eff_bg_num,*pf100Eff_bg_den);
	pf100Eff_qcd->SetStatisticOption(TEfficiency::kBUniform);
	pf100Eff_qcd->SetPosteriorMode(1);
	pf100Eff_qcd->UsesBayesianStat();
	pf100Eff_qcd->SetMarkerStyle(21);
	pf100Eff_qcd->SetMarkerColor(kMagenta);
   TH1D *pf100Eff_Num_signal_mc[15], *pf100Eff_Den_signal_mc[15];
   TEfficiency *pf100Eff_signal_mc[15];

   leg->AddEntry(pf100Eff,"LowM Data","p");
   leg->AddEntry(pf100Eff_qcd,"Bg MC","lp");
   pf100Eff->Draw("AP");
   pf100Eff_qcd->Draw("E same");
   int i = 0;
   for(const auto & f : SignalMC){
	   ++i;
	   pf100Eff_Num_signal_mc[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	   pf100Eff_Den_signal_mc[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	   pf100Eff_signal_mc[i] = new TEfficiency(*pf100Eff_Num_signal_mc[i],*pf100Eff_Den_signal_mc[i]);
	   pf100Eff_signal_mc[i]->SetStatisticOption(TEfficiency::kBUniform);
	   pf100Eff_signal_mc[i]->SetPosteriorMode(1);
	   pf100Eff_signal_mc[i]->UsesBayesianStat();
//	   pf100Eff_signal_mc[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_1D_Pf80_Pf100_pt1");
	   pf100Eff_signal_mc[i]->SetMarkerColor(50+3*i);
	   pf100Eff_signal_mc[i]->SetLineColor(50+3*i);
	   pf100Eff_signal_mc[i]->SetMarkerStyle(20);
	   pf100Eff_signal_mc[i]->Draw("same E");
	   pf100Eff->Draw("same E");
	   leg->AddEntry(pf100Eff_signal_mc[i],(std::to_string(f.first) + " MC").c_str(),"p");
   }
   leg->Draw();
   oneDFitLowM->Print("../pictures/TriggerPerformance/1DEff_PF80_PF100_lowM_pt1_DifSamples.pdf");

   //Pt distributions ;)
   TCanvas *can_pt_matched_and_not = new TCanvas("can_pt_matched_and_not","can_pt_matched_and_not",1000,800);
   TLegend *leg_pt_matched_and_not = new TLegend(0.85,0.5,0.99,0.99);
   TH1D *data_pt_matched = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_pt1_distr_matched_PFJet80");
   data_pt_matched->SetMarkerStyle(20);
   TH1D *data_pt_not_matched = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_pt1_distr_not_matched_PFJet80");
   data_pt_not_matched->SetLineWidth(1.1);
   TH1D *signal_pt_matched[15], *signal_pt_not_matched[15];
   data_pt_not_matched->Draw("hist");
   data_pt_not_matched->SetAxisRange(0.0005,1.1*data_pt_not_matched->GetMaximum(),"Y");
   data_pt_matched->Draw("E same");
   int j = 0;
   for(const auto & f : SignalMC){
	   ++j;
	   signal_pt_matched[j] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_pt1_distr_matched_PFJet80");
	   signal_pt_not_matched[j] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_pt1_distr_not_matched_PFJet80");
	   signal_pt_matched[j]->SetMarkerColor(50+3*j);
	   signal_pt_matched[j]->SetMarkerStyle(20);
	   signal_pt_not_matched[j]->SetLineColor(50+3*j);
	   leg_pt_matched_and_not->AddEntry(signal_pt_matched[j],(std::to_string(f.first) + " MC").c_str(),"p");


	   signal_pt_matched[j]->Draw("E same");
	   signal_pt_not_matched[j]->Draw("hist same");
   }
   leg_pt_matched_and_not->AddEntry(data_pt_matched,"matched","p");
   leg_pt_matched_and_not->AddEntry(data_pt_not_matched,"not matched","l");
   leg_pt_matched_and_not->AddEntry(data_pt_matched,"Data","p");
   leg_pt_matched_and_not->Draw();
   gPad->SetLogy();
   can_pt_matched_and_not->Print("../pictures/TriggerPerformance/PtSpectrum_PF80_PF100_lowM_pt1_DifSamples.pdf");

   //Central eta

   //........................PtHat........................//
	TCanvas *oneDEta_pt_hat1 = new TCanvas("oneDEta_pt_hat1","Low M ETa ptHat",1000,800);
	TH1D *pf100Eff_Num_signal_mc_pthat[15], *pf100Eff_Den_signal_mc_pthat[15];
	TEfficiency *pf100Eff_signal_mc_pthat[15];
	TLegend *ptHatEtaLeg = new TLegend(0.5,0.1,0.85,0.4);
	i=0;
	ptHatEtaLeg->AddEntry(pf100Eff,"LowM Data","p");
	pf100Eff->SetMarkerSize(1.5);
	pf100Eff->Draw("AP");
	for(const auto & f: pTHat){
		++i;
		pf100Eff_Num_signal_mc_pthat[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_pt1");
		pf100Eff_Den_signal_mc_pthat[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_pt1");
		pf100Eff_signal_mc_pthat[i] = new TEfficiency(*pf100Eff_Num_signal_mc_pthat[i],*pf100Eff_Den_signal_mc_pthat[i]);
		pf100Eff_signal_mc_pthat[i]->SetStatisticOption(TEfficiency::kBUniform);
		pf100Eff_signal_mc_pthat[i]->SetPosteriorMode(1);
		pf100Eff_signal_mc_pthat[i]->UsesBayesianStat();
		pf100Eff_signal_mc_pthat[i]->SetMarkerStyle(20);
		pf100Eff_signal_mc_pthat[i]->SetMarkerColor(50+3*i);
		pf100Eff_signal_mc_pthat[i]->SetLineColor(50+3*i);
		ptHatEtaLeg->AddEntry(pf100Eff_signal_mc_pthat[i],(f.first + " QCD MC").c_str(),"p");
		pf100Eff_signal_mc_pthat[i]->Draw("same E");
	}
	ptHatEtaLeg->Draw();

//   leg->Draw();
	oneDEta_pt_hat1->Print("../pictures/TriggerPerformance/1DEff_QCD_pTHat_PF80_PF100_lowM_pt1.pdf");


   //........................ETA....................................//
   TCanvas *oneDFitLowM_eta = new TCanvas("oneDFitLowM_eta","Factor Low M ETa",1000,800);
   TLegend *leg_eta_plot = new TLegend(0.4,0.2,0.75,0.5);

   TEfficiency *pf100Eff_eta_rd = (TEfficiency*) GetEfficiency(fDataLowM,"TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_eta1","TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_eta1");
   pf100Eff_eta_rd->SetMarkerStyle(20);
   TEfficiency *pf100Eff_eta_qcd = (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_eta1","TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_eta1");
   pf100Eff_eta_qcd->SetMarkerStyle(21);
   pf100Eff_eta_qcd->SetMarkerColor(kMagenta);
   TEfficiency *pf100Eff_eta_signal[15];
   leg_eta_plot->AddEntry(pf100Eff_eta_rd,"LowM Data","p");
   leg_eta_plot->AddEntry(pf100Eff_eta_qcd,"QCD MC","p");

	i = 0;
	pf100Eff_eta_rd->Draw("AP");
	gPad->Update();
	pf100Eff_eta_rd->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
	pf100Eff_eta_rd->Draw("AP");
	for(const auto & f : SignalMC){
	   ++i;
	   pf100Eff_eta_signal[i] = (TEfficiency*) GetEfficiency(f.second,"TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_eta1","TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_eta1");
	   pf100Eff_eta_signal[i]->SetMarkerColor(50+3*i);
	   pf100Eff_eta_signal[i]->SetMarkerStyle(20);
	   leg_eta_plot->AddEntry(pf100Eff_eta_signal[i],(std::to_string(f.first) + " Signal MC").c_str(),"p");
	   pf100Eff_eta_signal[i]->Draw("same P");
   }
	pf100Eff_eta_qcd->Draw("same P");
	leg_eta_plot->Draw();
	oneDFitLowM_eta->Print("../pictures/TriggerPerformance/1DEff_AllSamples_PF80_PF100_lowM_eta1.pdf");

	TCanvas *oneDLowM_eta_comp = new TCanvas("oneDLowM_eta_comp","Factor Low M ETa",1000,800);
	TLegend *legenda = new TLegend(0.5,0.2,0.85,0.6);
	TGraphAsymmErrors *ratio_of_eta = (TGraphAsymmErrors*) ratio.DrawRatio((TGraphAsymmErrors*)pf100Eff_eta_rd->GetPaintedGraph(),(TGraphAsymmErrors*)pf100Eff_eta_qcd->GetPaintedGraph(),oneDLowM_eta_comp,legenda,nullptr);
	oneDLowM_eta_comp->Print("../pictures/TriggerPerformance/Ratio_data_mc_qcd_eta1.pdf");
	/*
	//Check for different pT hat bins
	TCanvas *oneDEta_pt_hat = new TCanvas("oneDEta_pt_hat","Low M ETa ptHat",1000,800);
	TH1D *ptHatEta1[15];
	TLegend *ptHatEtaLeg2 = new TLegend(0.5,0.1,0.85,0.4);
	i=0;
	for(const auto & f: pTHat){
		++i;
		ptHatEta1[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_1D_Pf80_Pf100_eta1");
		ptHatEta1[i]->SetMarkerStyle(20);
		ptHatEta1[i]->SetMarkerColor(i);
		ptHatEtaLeg2->AddEntry(ptHatEta1[i],(f.first + " QCD MC").c_str(),"p");
		ptHatEta1[i]->Draw("same E");
		pf100Eff_eta->Draw("same E");
	}
	ptHatEtaLeg2->Draw();

	//.................................f(pt,eta).....................................//
	//.................................Hope- Finale version..........................//
	TH2D *RD_num_pf80for100_pt1eta1 = (TH2D*) fDataLowM ->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
	TH2D *RD_den_pf80for100_pt1eta1 = (TH2D*) fDataLowM ->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
	TEfficiency *eff_rd_pt1eta1 	= new TEfficiency(*RD_num_pf80for100_pt1eta1,*RD_den_pf80for100_pt1eta1);
	BayesianErrors(eff_rd_pt1eta1);
	TH2D *qcd_num_pf80for100_pt1eta1[15];// = (TH2D*) BgMC ->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
	TH2D *qcd_den_pf80for100_pt1eta1[15];// = (TH2D*) BgMC ->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
	TEfficiency *eff_qcd_pt1eta1[15];// 	= new TEfficiency(*qcd_num_pf80for100_pt1eta1,*qcd_den_pf80for100_pt1eta1);
//	BayesianErrors(eff_qcd_pt1eta1);

	TCanvas *can_eta1_slices = new TCanvas("can_eta1_slices","canva with 1t Eta bin adn pT Hat",1000,800);
	/*
	//...............Study of the different slices and Eta in different pT hat bin
    TCanvas *can_eta1_slices = new TCanvas("can_eta1_slices","canva with 1t Eta bin adn pT Hat",1000,800);
    TCanvas *can_eta2_slices = new TCanvas("can_eta2_slices","canva with 2d Eta bin adn pT Hat",1000,800);
    TCanvas *can_eta3_slices = new TCanvas("can_eta3_slices","canva with 3d Eta bin adn pT Hat",1000,800);

    TH1D ** slix_eta1_qcd_lowM = new TH1D*[15];
    TH1D ** slix_eta2_qcd_lowM = new TH1D*[15];
    TH1D ** slix_eta3_qcd_lowM = new TH1D*[15];

    TH1D ** slix_eta1_data_lowM = new TH1D*[15];
    TH1D ** slix_eta2_data_lowM = new TH1D*[15];
    TH1D ** slix_eta3_data_lowM = new TH1D*[15];
    TH2D *temp;
    std::string name;
    i = 0;
    	can_eta1_slices->cd();
    	for(const auto & f: pTHat){
        	//eta1 slice:
        	++i;
        	qcd_num_pf80for100_pt1eta1[i] = (TH2D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
        	qcd_den_pf80for100_pt1eta1[i] = (TH2D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
        	eff_qcd_pt1eta1[i] = new TEfficiency(*qcd_num_pf80for100_pt1eta1[i],*qcd_den_pf80for100_pt1eta1[i]);
        	BayesianErrors(eff_qcd_pt1eta1[i]);
        	temp = (TH2D*) eff_qcd_pt1eta1[i]->CreateHistogram();
        	slix_eta1_qcd_lowM[i] = (TH1D*) temp->ProjectionX(("Slice_data_pt1eta1_LowM_etabin1_pthat_"+f.first).c_str(),1,1,"e");
        	slix_eta1_qcd_lowM[i]->Draw("E same");

    	}/*
        	slix_eta1_qcd_lowM[i] = eff_qcd_pt1eta1[i]->CreateHistogram();


        	slix_pt1eta1_data_lowM[biny] 	= pf80for100_pt1eta1->ProjectionX(("Slice_data_pt1eta1_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
        	slix_pt1eta1_data_lowM[biny]->SetLineColor(2);
        	slix_pt1eta1_data_lowM[biny]->Draw("E");
        	slix_pt1eta1_mc_lowM[biny] 	= qcd_num_pf80for100_pt1eta1->ProjectionX(("Slice_pt1eta1_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
        	slix_pt1eta1_mc_lowM[biny]->Draw("E same");
        	slix_pt1eta1_ratio_lowM[biny] = (TH1D*) ratio.DrawRatio(slix_pt1eta1_data_lowM[biny],slix_pt1eta1_mc_lowM[biny],can_eta_slices);
        	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_pt1eta1_data_lowM[biny]->GetName()) + ".pdf";
        	canpl->Print(name.c_str());
    }
	/*
    TCanvas *canpl;
    TLegend *slice_leg_2 = nullptr;
    TH1D ** slix_pt1eta1_data_lowM = new TH1D*[pf80for100_pt1eta1->GetNbinsY()];
    TH1D ** slix_pt1eta1_mc_lowM = new TH1D*[pf80for100_pt1eta1->GetNbinsY()];
    TH1D ** slix_pt1eta1_ratio_lowM = new TH1D*[pf80for100_pt1eta1->GetNbinsY()];
    std::string name;

    for(int biny = 0; biny < pf80for100_pt1eta1->GetNbinsY(); biny++){
    	canpl = new TCanvas("canpl");
    	slix_pt1eta1_data_lowM[biny] 	= pf80for100_pt1eta1->ProjectionX(("Slice_data_pt1eta1_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
    	slix_pt1eta1_data_lowM[biny]->SetLineColor(2);
    	slix_pt1eta1_data_lowM[biny]->Draw("E");
    	slix_pt1eta1_mc_lowM[biny] 	= qcd_num_pf80for100_pt1eta1->ProjectionX(("Slice_pt1eta1_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
    	slix_pt1eta1_mc_lowM[biny]->Draw("E same");
    	slix_pt1eta1_ratio_lowM[biny] = (TH1D*) ratio.DrawRatio(slix_pt1eta1_data_lowM[biny],slix_pt1eta1_mc_lowM[biny],nullH,slice_leg_2,canpl);
    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_pt1eta1_data_lowM[biny]->GetName()) + ".pdf";
    	canpl->Print(name.c_str());

    }


	pf80for100_pt1eta1->SetAxisRange(100.,500.,"X");
	qcd_num_pf80for100_pt1eta1->SetAxisRange(100.,500.,"X");
	qcd_den_pf80for100_pt1eta1->SetAxisRange(100.,500.,"X");
    TCanvas *can_twoDLowM_pt1eta1 = new TCanvas("can_twoDLowM_pt1eta1","2D Trigger eff = f(pt1,eta1)",1000,800);
	pf80for100_pt1eta1->Divide(pf80for100_pt1eta1,qcd_num_pf80for100_pt1eta1);

	can_twoDLowM_pt1eta1->SetLeftMargin(0.08); // ->SetPadLeftMargin(0.1);
	can_twoDLowM_pt1eta1->SetRightMargin(0.1);
	pf80for100_pt1eta1->Draw("COLZ");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_colz.pdf");
	pf80for100_pt1eta1->Draw("E");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_e.pdf");
	pf80for100_pt1eta1->Draw("CONT1");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_cont1.pdf");
	pf80for100_pt1eta1->Draw("CONT4");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_cont4.pdf");
	pf80for100_pt1eta1->Draw("SURF2");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_surf2.pdf");
	pf80for100_pt1eta1->Draw("SURF4");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/CorrectionFunction_LowM_pt1eta1_surf4.pdf");

	/*

	//Corelation Coefficient
	TCanvas *twoDplotLowM = new TCanvas("twoDplotLowM","Two dimensional Efficiency");
	//data for ratio
	TH2D *TwoDEff_Num = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf100_pt1vspt2");
    TH2D *TwoDEff_NumSym = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf100_pt2vspt1");
    for(int binx = 1; binx <= TwoDEff_NumSym->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEff_NumSym->GetNbinsY(); biny++)
    	{
    		if(binx == biny){
    			TwoDEff_NumSym->SetBinContent(binx,biny,0);
    			TwoDEff_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }
    TwoDEff_Num->Add(TwoDEff_NumSym);

    //mc
	TH2D *TwoDEff_QCD_Num = (TH2D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_2D_Pf80_Pf100_pt1vspt2");
	TH2D *TwoDEff_QCD_Den = (TH2D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_2D_Pf80_Pf100_pt1vspt2");
	TwoDEff_QCD_Num->Divide(TwoDEff_QCD_Num,TwoDEff_QCD_Den,1,1,error_mode.c_str());
	TH2D *TwoDEff_QCD_NumSym = (TH2D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_2D_Pf80_Pf100_pt2vspt1");
	TH2D *TwoDEff_QCD_DenSym = (TH2D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_2D_Pf80_Pf100_pt2vspt1");
	TwoDEff_QCD_NumSym->Divide(TwoDEff_QCD_NumSym,TwoDEff_QCD_DenSym,1,1,error_mode.c_str());
	    for(int binx = 1; binx <= TwoDEff_QCD_NumSym->GetNbinsX(); binx++)
	    {
	    	for(int biny = 1; biny <= TwoDEff_QCD_NumSym->GetNbinsY(); biny++)
	    	{
	    		if(binx == biny){
	    			TwoDEff_QCD_NumSym->SetBinContent(binx,biny,0);
	    			TwoDEff_QCD_NumSym->SetBinError(binx,biny,0);
	    		}
	    	}
	    }
	    TwoDEff_QCD_Num->Add(TwoDEff_QCD_NumSym);

		TwoDEff_QCD_Num ->SetMarkerStyle(20);
		TwoDEff_QCD_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
	    gPad->SetRightMargin(0.15);
	    TwoDEff_QCD_Num->GetZaxis()->SetTitleOffset(0.8);
	//    gStyle->SetPadLeftMargin(0.85);
	    TwoDEff_QCD_Num->GetXaxis()->SetTitleOffset(1.1);
		TwoDEff_QCD_Num->Draw("E");
	//	outputLow->cd();
	//	TwoDEff_QCD_Num->Write();

		twoDplotLowM->SetLeftMargin(0.08);
		twoDplotLowM->SetRightMargin(0.1);
		TwoDEff_QCD_Num->Draw("COLZ");
		twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_PF80_PF100_LowM_COLZ.pdf");
		TwoDEff_QCD_Num->Draw("BOX");
		twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_PF80_PF100_LowM_BOX.pdf");
		TwoDEff_QCD_Num->Draw("CONT1");
		twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_PF80_PF100_LowM_CONT1.pdf");
		TwoDEff_QCD_Num->Draw("CONT4");
		twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_PF80_PF100_LowM_CONT4.pdf");
		TwoDEff_QCD_Num->Draw("E");
		twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_PF80_PF100_LowM_E.pdf");
	//	TwoDEff_QCD_Num->Write();


		//........Corelation.......//
		//data for ratio
		TH2D *Correlation_PF80_LowM = new TH2D(*TwoDEff_Num);
	    for(int binx = 1; binx <= TwoDEff_Num->GetNbinsX(); binx++)
	    {
	    	for(int biny = 1; biny <= TwoDEff_Num->GetNbinsY(); biny++)
	    	{
	    		double xx = TwoDEff_Num->GetBinContent(binx,binx);
	    		double yy = TwoDEff_Num->GetBinContent(biny,biny);
	    		double xy = TwoDEff_Num->GetBinContent(binx,biny);
	    		double cxy = 0;
	    		double ecxy = 0;
	    		if(xx > 0 && yy>0){
	    			cxy = xy/sqrt(xx*yy);
	    			if(xx!=yy && xy != 0){
	    		          double exy = TwoDEff_Num->GetBinError(binx,biny) / xy;
	    		          double exx = TwoDEff_Num->GetBinError(binx,binx) / xx;
	    		          double eyy = TwoDEff_Num->GetBinError(biny,biny) / yy;
	    		          ecxy = sqrt( exy*exy + 0.25*exx*exx + 0.25*eyy*eyy );
	    			}
	    		}
	    		Correlation_PF80_LowM->SetBinContent(binx,biny,cxy);
	    		Correlation_PF80_LowM->SetBinError(binx,biny,ecxy);
	    	}
	    }

	    //MC
		TH2D *Correlation_QCD_PF80_LowM = new TH2D(*TwoDEff_QCD_Num);
	    for(int binx = 1; binx <= TwoDEff_QCD_Num->GetNbinsX(); binx++)
	    {
	    	for(int biny = 1; biny <= TwoDEff_QCD_Num->GetNbinsY(); biny++)
	    	{
	    		double xx = TwoDEff_QCD_Num->GetBinContent(binx,binx);
	    		double yy = TwoDEff_QCD_Num->GetBinContent(biny,biny);
	    		double xy = TwoDEff_QCD_Num->GetBinContent(binx,biny);
	    		double cxy = 0;
	    		if(xx > 0 && yy>0) std::cout<<"x: "<<binx<<" y: "<<biny<<" val: "<<xy/sqrt(xx*yy)<<std::endl;
	    		double ecxy = 0;
	    		if(xx > 0 && yy>0){
	    			cxy = xy/sqrt(xx*yy);
	    			if(xx!=yy && xy != 0){
	    		          double exy = TwoDEff_QCD_Num->GetBinError(binx,biny) / xy;
	    		          double exx = TwoDEff_QCD_Num->GetBinError(binx,binx) / xx;
	    		          double eyy = TwoDEff_QCD_Num->GetBinError(biny,biny) / yy;
	    		          ecxy = sqrt( exy*exy + 0.25*exx*exx + 0.25*eyy*eyy );
	    			}
	    		}
	    		Correlation_QCD_PF80_LowM->SetBinContent(binx,biny,cxy);
	    		Correlation_QCD_PF80_LowM->SetBinError(binx,biny,ecxy);
	    	}
	    }
	    Correlation_QCD_PF80_LowM->SetTitle("Correlation coefficient");
	    TCanvas *canCorelLowM = new TCanvas("canCorelLowM","Correlation for LowM");
	    canCorelLowM->SetLeftMargin(0.08);
	    canCorelLowM->SetRightMargin(0.1);
	    Correlation_QCD_PF80_LowM->Draw("COLZ");
	    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_Correlation_PF80_PF100_LowM_COLZ.pdf");
	    Correlation_QCD_PF80_LowM->Draw("BOX");
	    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_Correlation_PF80_PF100_LowM_BOX.pdf");
	    Correlation_QCD_PF80_LowM->Draw("CONT1");
	    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_Correlation_PF80_PF100_LowM_CONT1.pdf");
	    Correlation_QCD_PF80_LowM->Draw("CONT4");
	    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_Correlation_PF80_PF100_LowM_CONT4.pdf");
	    Correlation_QCD_PF80_LowM->Draw("E");
	    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_QCD_Correlation_PF80_PF100_LowM_E.pdf");

	    //Slices:
	    TCanvas *canl, *canb;
	    TLegend *slice_leg = nullptr;
	    TH1D ** slix_eff_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
	    TH1D ** slix_corel_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];

	    TH1D ** slix_qcd_eff_lowM = new TH1D*[TwoDEff_QCD_Num->GetNbinsY()];
	    TH1D ** slix_qcd_corel_lowM = new TH1D*[TwoDEff_QCD_Num->GetNbinsY()];

	    TH1D ** slix_eff_ratio_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
	    TH1D ** slix_eff_corel_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
	    std::string name;

	    for(int biny = 0; biny < TwoDEff_Num->GetNbinsY(); biny++){
	    	canl = new TCanvas("canl");
	    	slix_qcd_eff_lowM[biny] 	= TwoDEff_QCD_Num->ProjectionX(("Slice_qcd_Eff_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
	    	slix_qcd_eff_lowM[biny]->SetLineColor(2);
	    	slix_qcd_eff_lowM[biny]->Draw("E");
	    	slix_eff_lowM[biny] 	= TwoDEff_Num->ProjectionX(("Slice_Eff_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
	    	slix_eff_lowM[biny]->Draw("E same");
	    	slix_eff_ratio_lowM[biny] = (TH1D*) ratio.DrawRatio(slix_eff_lowM[biny],slix_qcd_eff_lowM[biny],nullH,slice_leg,canl);
	    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_qcd_eff_lowM[biny]->GetName()) + ".pdf";
	    	canl->Print(name.c_str());
	    	canb = new TCanvas("canb");
	    	slix_qcd_corel_lowM[biny]	= Correlation_QCD_PF80_LowM->ProjectionX(("Slice_qcd_Corel_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
	    	slix_qcd_corel_lowM[biny]->SetLineColor(2);
	    	slix_qcd_corel_lowM[biny]->Draw("E");
	    	slix_corel_lowM[biny]	= Correlation_PF80_LowM->ProjectionX(("Slice_Corel_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
	    	slix_corel_lowM[biny]->Draw("E");
	    	slix_eff_corel_lowM[biny] = (TH1D*) ratio.DrawRatio(slix_corel_lowM[biny],slix_qcd_corel_lowM[biny],nullH,slice_leg,canb);
	    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_qcd_corel_lowM[biny]->GetName()) + ".pdf";
	    	canb->Print(name.c_str());
	    }
	    
/*

	TCanvas *oneDFitLowM = new TCanvas("oneDFitLowM","Factor Low M",1000,800);
	TH1D *pf100Eff = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_1D_Pf80_Pf100_pt1");
	pf100Eff->SetMarkerStyle(20);
	TH1D *pf100Eff_mc = (TH1D*) fMCLowM->Get("TriggerEfficiencies/KinTrigEff_1D_Pf80_Pf100_pt1");
	auto ratioDataMC_lowM_PF80 = (TH1D*) ratio.DrawRatio(pf100Eff,pf100Eff_mc,nullH,leg,oneDFitLowM);

	/*
	oneDFitLowM->SaveAs("../pictures/TriggerPerformance_MC_1DEff_PF80_LowM.pdf");

	/*
	TCanvas *twoDplotLowM = new TCanvas("twoDplotLowM","Two dimensional Efficiency");
	TH2D *TwoDEff_Num = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf100_pt1vspt2");
    TH2D *TwoDEff_NumSym = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf100_pt2vspt1");
    for(int binx = 1; binx <= TwoDEff_NumSym->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEff_NumSym->GetNbinsX(); biny++)
    	{
    		if(binx == biny){
    			TwoDEff_NumSym->SetBinContent(binx,biny,0);
    			TwoDEff_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }
    TwoDEff_Num->Add(TwoDEff_NumSym);

	TwoDEff_Num ->SetMarkerStyle(20);
	TwoDEff_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
    gPad->SetRightMargin(0.15);
    TwoDEff_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEff_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEff_Num->Draw("E");

	TCanvas *canSave_low;
	TwoDEff_Num->Draw("COLZ");
	TwoDEff_Num->SaveAs("../pictures/TriggerPerformance_2DEff_PF80_PF100_LowM.pdf");
//	TwoDEff_Num->Write();


	auto dEtaCan = new TCanvas("dEtaCan","dEta Trig Eff",1000,800);
	style.standardTitle(PRIVATE);

	TH1D *dEtaEff = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_1D_Pf60_Pf100_dEta");
	dEtaEff->SetMarkerStyle(20);

	TF1 *fitEta = new TF1("fitEta",finaleFunction,0,2.5,4);
	fitEta-> SetParameters(-1.15698e+02,1.59862e+00,9.93479e-02,-4.49790e+01);
	auto ratiodEta = (TH1D*) ratio.DrawRatio(dEtaEff,fitEta,"Fit:",dEtaCan);
	ratiodEta->Draw();
	dEtaCan->SaveAs("../pictures/TriggerPerformance_dEtaEff_lowM.pdf");



	//............................	HIGH M ....................................

	TCanvas *oneDFitHighM = new TCanvas("oneDFitHighM","Factor Low M",1000,800);
	TH1D *pf160Eff = (TH1D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_1D_Pf80_Pf160_pt1");
	pf160Eff->SetMarkerStyle(20);
	pf160Eff->Draw("E1");
	TF1 *fit1DHighM = new TF1("fit1DHighM",finaleFunction,160,500,4);
	fit1DHighM->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
	auto hHighMRatio = (TH1D*) ratio.DrawRatio(pf160Eff,fit1DHighM,"Sigmoid*Sigmoid",oneDFitHighM);

	oneDFitHighM->SaveAs("../pictures/TriggerPerformance_1DEff_PF80_HighM.pdf");

	TCanvas *twoDplotHighM = new TCanvas("twoDplotHighM","Two dimensional Efficiency");

	TH2D *TwoDEffPf160_Num = (TH2D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf160_pt1vspt2");
    TH2D *TwoDEffPf160_NumSym = (TH2D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_2D_Pf80_Pf160_pt2vspt1");
    for(int binx = 1; binx <= TwoDEffPf160_NumSym->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEffPf160_NumSym->GetNbinsX(); biny++)
    	{
    		if(binx == biny){
    			TwoDEffPf160_NumSym->SetBinContent(binx,biny,0);
    			TwoDEffPf160_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }
    TwoDEffPf160_Num->Add(TwoDEffPf160_NumSym);

	TwoDEffPf160_Num ->SetMarkerStyle(20);
	TwoDEffPf160_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
    gPad->SetRightMargin(0.15);
    TwoDEffPf160_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEffPf160_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEffPf160_Num->Draw("E");

	TCanvas *canSave_high;
	TwoDEffPf160_Num->Draw("COLZ");
	TwoDEffPf160_Num->SaveAs("../pictures/TriggerPerformance_2DEff_PF80_PF160_HighM.pdf");
//	TwoDEffPf160_Num->Write();

	/*
	TFile *outputHM = new TFile("TwoDPtHighMassEfficiency.root","recreate");

	TCanvas *can_high_M = new TCanvas("can_high_M","Two D Pt trig Eff",1000,800);

	const float Bins_H_mass[] = {120.,140.,150,160,170,180,190,200,220,240,260,300,350,400,500};
	size = sizeof(Bins_H_mass)/sizeof(float) - 1;

	TH2F *TwoDEff_H_mass_Num = new TH2F("TwoDEff_H_mass_Num","2D Efficiency",size,Bins_H_mass,size,Bins_H_mass);
	TwoDEff_H_mass_Num ->SetMarkerStyle(20);
	TH2F *TwoDEff_H_mass_Denum = new TH2F("TwoDEff_H_mass_Denum","2D Efficiency",size,Bins_H_mass,size,Bins_H_mass);
	TwoDEff_H_mass_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_H_mass_Num","LeadPt[0] > 140 && LeadPt[1] > 140 && LeadMatch160[0] == 1 && LeadMatch160[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_H_mass_Denum","LeadPt[0] > 140 && LeadPt[1] > 140 && PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	TwoDEff_H_mass_Num ->Divide(TwoDEff_H_mass_Num,TwoDEff_H_mass_Denum,1,1,error_mode.c_str());
	TwoDEff_H_mass_Num ->Draw("E");

    TH2F *TwoDEff_H_mass_NumSym = new TH2F("TwoDEff_H_mass_NumSym","2D Efficiency with True method",size,Bins_H_mass,size,Bins_H_mass);
    TwoDEff_H_mass_NumSym ->SetMarkerStyle(20);
    TH2F *TwoDEff_H_massTrueMC_DenumSym = new TH2F("TwoDEff_H_massTrueMC_DenumSym","2D Efficiency with True method",size,Bins_H_mass,size,Bins_H_mass);
    TwoDEff_H_mass_NumSym ->SetTitle("2D Efficiency with True method;sub-leading jet p_{T};leading jet p_{T}");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_H_mass_NumSym","LeadPt[0] > 140 && LeadPt[1] > 140 && LeadMatch160[0] == 1 && LeadMatch160[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_H_massTrueMC_DenumSym","LeadPt[0] > 140 && LeadPt[1] > 140 && PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    TwoDEff_H_mass_NumSym ->Divide(TwoDEff_H_mass_NumSym,TwoDEff_H_massTrueMC_DenumSym,1,1,error_mode.c_str());

    for(int binx = 1; binx <= size; binx++)
    {
    	for(int biny = 1; biny <= size; biny++)
    	{
    		if(binx == biny){
    			std::cout<<"bin = "<<binx<<std::endl;
    			TwoDEff_H_mass_NumSym->SetBinContent(binx,biny,0);
				TwoDEff_H_mass_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }

    TwoDEff_H_mass_Num->SetTitle("Work in progress, 2.2 fb^{-1}, #sqrt{s} = 13 TeV; p^{(1)}_{T}, (GeV); p^{(2)}_{T}, (GeV)");
    TwoDEff_H_mass_Num->Add(TwoDEff_H_mass_NumSym);
    gPad->SetRightMargin(0.15);
    TwoDEff_H_mass_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEff_H_mass_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEff_H_mass_Num->Draw("E");

	TwoDEff_H_mass_Num->Draw("COLZ");
	TwoDEff_H_mass_Num->SaveAs("pictures/HighMTwoDPtTriggerEff.pdf");
//	TwoDEff_H_mass_Num->Write("TwoDEff_Num");


	TCanvas *oneDFitHighM = new TCanvas("oneDFitHighM","Factor High M",1000,800);
	TH1D *pf160Eff = new TH1D("pf160Eff",";p_{T}, [GeV]; #epsilon",100,100.,500.);
	pf160Eff->SetMarkerStyle(20);
	TH1D *pf160EffDenum = new TH1D("pf160EffDenum","",100,100.,500.);
	data->Draw("LeadPt[0]>>pf160Eff","LeadMatch160[0] == 1");
	data->Draw("LeadPt[0]>>pf160EffDenum","PFJet80 == 1");
	pf160Eff->Divide(pf160Eff,pf160EffDenum,1,1,error_mode.c_str());
	pf160Eff->Draw("E");

	fit->SetRange(160,500);
	fit->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
//	pf160Eff->Fit(fit);
//	pf160Eff->Draw("E");
//	drawRatio(pf160Eff,fit,oneDFitLowM,"bla");
	auto hHighMRatio = (TH1D*) ratio.DrawRatio(pf160Eff,fit,"Sigmoid*Sigmoid",oneDFitHighM);
	oneDFitLowM->SaveAs("pictures/OneDEfficiencyHighM.pdf");

//	outputHM->Close();
//	output->Close();


/**/
	return 0;


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

double dEtaFitFunction(double *x, double *par)
{
	double sigmoid = 1./(1.+exp( - par[0] * (x[0] - par[1]) ));
	return sigmoid;
}

void drawRatio(TH1D *histo,TF1 *fit, TCanvas *canva, std::string fitName)
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

   TH1D * hRatio = (TH1D*)histo->Clone("hRatio");
   histo -> Fit(fit,"R+");
   char name[200];
   sprintf(name,"}{#chi^{2}/ndf = %.2f}",fit->GetChisquare()/fit->GetNDF());
   fitName = "#splitline{ " + fitName + name;
   TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend -> SetTextSize(0.04);
   fitLegend -> AddEntry(fit,fitName.c_str(),"l");
   histo -> Draw();
   fitLegend->Draw();
   canva -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   hRatio -> Sumw2();

   hRatio -> Divide(fit);
   hRatio -> Draw();

   TLine *horizLine = new TLine(120,1,500,1);
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

void BayesianErrors(TEfficiency *h){
	h->SetStatisticOption(TEfficiency::kBUniform);
	h->SetPosteriorMode(1);
	h->UsesBayesianStat();
	h->SetUseWeightedEvents();
}

TEfficiency * GetEfficiency(TFile * f, const std::string & name_num, const std::string & name_den){
	if(!f){
		std::cerr<<"empty file were provided. Interupt."<<std::endl;
		exit(0);
	}

	//check 1D or 2D
	bool twoD = false;
	if(name_num.find("2D") != std::string::npos) twoD = true;

	TEfficiency *out;
	TH1D *num, *den;
	TH2D *num2D, *den2D;
	//1D case:
	if(!twoD){
		num = (TH1D*) f->Get(name_num.c_str());
		den = (TH1D*) f->Get(name_den.c_str());
		out = new TEfficiency(*num,*den);
	}
	else {
		TH2D *num = (TH2D*) f->Get(name_num.c_str());
		TH2D *den = (TH2D*) f->Get(name_den.c_str());
		out = new TEfficiency(*num2D,*den2D);
	}
	BayesianErrors(out);

	return out;
}
