#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "TEfficiency.h"

TEfficiency * GetEfficiency(TFile * f, const std::string & name_num, const std::string & name_den);
void BayesianErrors(TEfficiency *h);
void drawRatio(TH1D *, TF1 *,TCanvas *,std::string );
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);

int MCvsMC(){

	TH1::SetDefaultSumw2();

	TFile *fDataLowM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_Run2015D-16Dec2015-v1.root");
	TFile *fDataHighM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_highM_Run2015D-16Dec2015-v1.root");

   std::map<int,TFile* > SignalMC;
//   SignalMC[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
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
   TFile *BgMC = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_0p4.root");
   TFile *BgMC_b = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_b_0p4.root");
   std::map<std::string,TFile* > pTHat;
//   pTHat["50to80"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["80to120"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["120to170"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["170to300"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["300to470"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8.root");
//   pTHat["470to600"] = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8.root");
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
	TH1D *pf100Eff_rd_num = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_pt1");
	TLegend *leg = new TLegend(0.85,0.5,0.99,0.99);
	TH1D *pf100Eff_rd_den = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_pt1");
	TEfficiency *pf100Eff = new TEfficiency(*pf100Eff_rd_num,*pf100Eff_rd_den);
	pf100Eff->SetStatisticOption(TEfficiency::kBUniform);
	pf100Eff->SetPosteriorMode(1);
	pf100Eff->UsesBayesianStat();
	pf100Eff->SetMarkerStyle(20);
	TH1D *pf100Eff_bg_num = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_pt1");
	TH1D *pf100Eff_bg_den = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_pt1");
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
	   pf100Eff_Num_signal_mc[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_pt1");
	   pf100Eff_Den_signal_mc[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_pt1");
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

	TCanvas *oneDFitLowM_central = new TCanvas("oneDFitLowM_central","Factor Low M",1000,800);
	TH1D *pf100Eff_rd_num_central = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	TLegend *leg_central = new TLegend(0.85,0.5,0.99,0.99);
	TH1D *pf100Eff_rd_den_central = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	TEfficiency *pf100Eff_central = new TEfficiency(*pf100Eff_rd_num_central,*pf100Eff_rd_den_central);
	pf100Eff_central->SetStatisticOption(TEfficiency::kBUniform);
	pf100Eff_central->SetPosteriorMode(1);
	pf100Eff_central->UsesBayesianStat();
	pf100Eff_central->SetMarkerStyle(20);
	TH1D *pf100Eff_bg_num_central = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	TH1D *pf100Eff_bg_den_central = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	TEfficiency *pf100Eff_qcd_central = new TEfficiency(*pf100Eff_bg_num_central,*pf100Eff_bg_den_central);
	pf100Eff_qcd_central->SetStatisticOption(TEfficiency::kBUniform);
	pf100Eff_qcd_central->SetPosteriorMode(1);
	pf100Eff_qcd_central->UsesBayesianStat();
	pf100Eff_qcd_central->SetMarkerStyle(21);
	pf100Eff_qcd_central->SetMarkerColor(kMagenta);
  TH1D *pf100Eff_Num_signal_mc_central[15], *pf100Eff_Den_signal_mc_central[15];
  TEfficiency *pf100Eff_signal_mc_central[15];

  leg_central->AddEntry(pf100Eff_central,"Data CENTRAL #eta","p");
  leg_central->AddEntry(pf100Eff_qcd_central,"Bg MC","lp");
  pf100Eff_central->Draw("AP");
  pf100Eff_qcd_central->Draw("E same");
  i = 0;
  for(const auto & f : SignalMC){
	   ++i;
	   pf100Eff_Num_signal_mc_central[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1_central_eta");
	   pf100Eff_Den_signal_mc_central[i] = (TH1D*) f.second->Get("TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1_central_eta");
	   pf100Eff_signal_mc_central[i] = new TEfficiency(*pf100Eff_Num_signal_mc_central[i],*pf100Eff_Den_signal_mc_central[i]);
	   BayesianErrors(pf100Eff_signal_mc_central[i]);
	   pf100Eff_signal_mc_central[i]->SetMarkerColor(50+3*i);
	   pf100Eff_signal_mc_central[i]->SetLineColor(50+3*i);
	   pf100Eff_signal_mc_central[i]->SetMarkerStyle(20);
	   pf100Eff_signal_mc_central[i]->Draw("same E");
	   pf100Eff_central->Draw("same E");
	   leg_central->AddEntry(pf100Eff_signal_mc_central[i],(std::to_string(f.first) + " MC").c_str(),"p");
  }
  leg_central->Draw();

  TCanvas *can_bg_comp = new TCanvas("can_bg_comp","BG comparison",1000,800);
  TLegend *leg_bg_qcd_comp = new TLegend(0.7,0.5,0.9,0.7);
  TH1D *pf100Eff_bg_num_true_b = (TH1D*) BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_pt1");
  TH1D *pf100Eff_bg_den_true_b = (TH1D*) BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_pt1");
  TEfficiency *pf100Eff_qcd_true_b = new TEfficiency(*pf100Eff_bg_num_true_b,*pf100Eff_bg_den_true_b);
  BayesianErrors(pf100Eff_qcd_true_b);
  pf100Eff_qcd_true_b->SetMarkerStyle(20);
  leg_bg_qcd_comp->AddEntry(pf100Eff_qcd_true_b,"Hadron flavour b","p");
  leg_bg_qcd_comp->AddEntry(pf100Eff_qcd,"All jets","p");
  pf100Eff_qcd_true_b->Draw("AP");
  pf100Eff_qcd->Draw("P same");
  leg_bg_qcd_comp->Draw();

  TCanvas *can_bg_true_comp = new TCanvas("can_bg_true_comp","can_bg_true_comp",1000,800);
  TH1D *pf100Eff_bg_num_true_b_true = (TH1D*) BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Num_1D_True_PF80_PF100_pt1");
  TH1D *pf100Eff_bg_den_true_b_true = (TH1D*) BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Den_1D_True_PF80_PF100_pt1");
  TEfficiency *pf100Eff_qcd_true_b_true = new TEfficiency(*pf100Eff_bg_num_true_b_true,*pf100Eff_bg_den_true_b_true);
  BayesianErrors(pf100Eff_qcd_true_b_true);
  TH1D *pf100Eff_bg_num_true = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_1D_True_PF80_PF100_pt1");
  TH1D *pf100Eff_bg_den_true = (TH1D*) BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_1D_True_PF80_PF100_pt1");
  TEfficiency *pf100Eff_qcd_true = new TEfficiency(*pf100Eff_bg_num_true,*pf100Eff_bg_den_true);
  BayesianErrors(pf100Eff_qcd_true);
  pf100Eff_qcd_true->SetMarkerColor(kMagenta);
  pf100Eff_qcd_true->SetMarkerStyle(21);
  pf100Eff_qcd_true_b_true->SetMarkerStyle(20);
  pf100Eff_qcd_true->Draw("AP");
  pf100Eff_qcd_true_b_true->Draw("P same");
  leg_bg_qcd_comp->Draw();


  TCanvas *can_bg_comp_eta = new TCanvas("can_bg_comp_eta","BG comparison eta",1000,800);
  TEfficiency *pf100Eff_eta_qcd = (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_eta1","TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_eta1");
  pf100Eff_eta_qcd->SetMarkerStyle(21);
  pf100Eff_eta_qcd->SetMarkerColor(kMagenta);
  TEfficiency *pf100Eff_eta_qcd_true_b = (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_Pf80_Pf100_eta1","TriggerEfficiencies/KinTrigEff_Den_1D_Pf80_Pf100_eta1");
  pf100Eff_eta_qcd_true_b->SetMarkerStyle(20);
  pf100Eff_eta_qcd_true_b->Draw("AP");
  gPad->Update();
  pf100Eff_eta_qcd_true_b->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  pf100Eff_eta_qcd_true_b->Draw("AP");
  pf100Eff_eta_qcd->Draw("P same");
  leg_bg_qcd_comp->Draw();

  TCanvas *can_pt_eta_bins = new TCanvas("can_pt_eta_bins","can_pt_eta_bins",1000,800);
  TH2D *h_pt_eta_num 	= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_num_b 	= (TH2D*)	BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_den 	= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_den_b 	= (TH2D*)	BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");

  TH1D *h_pt_num_slice[3], *h_pt_num_slice_b[3], *h_pt_den_slice[3], *h_pt_den_slice_b[3];
  TEfficiency *eff_pt_slice[3], *eff_pt_slice_b[3];
  TCanvas *can_pt_slice[3];
  for(int i = 0; i<3;++i){
	  can_pt_slice[i] = new TCanvas(("can_pt_slice_"+std::to_string(i)).c_str(),"canv",1000,800);
	  h_pt_num_slice[i] = (TH1D*)  h_pt_eta_num->ProjectionX(("Slice_num_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_pt_den_slice[i] = (TH1D*)  h_pt_eta_den->ProjectionX(("Slice_den_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_pt_slice[i] = new TEfficiency(*h_pt_num_slice[i],*h_pt_den_slice[i]);
	  eff_pt_slice[i]->SetMarkerStyle(20);
	  BayesianErrors(eff_pt_slice[i]);

	  h_pt_num_slice_b[i] = (TH1D*)  h_pt_eta_num_b->ProjectionX(("Slice_num_b_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_pt_den_slice_b[i] = (TH1D*)  h_pt_eta_den_b->ProjectionX(("Slice_den_b_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_pt_slice_b[i] = new TEfficiency(*h_pt_num_slice_b[i],*h_pt_den_slice_b[i]);
	  BayesianErrors(eff_pt_slice_b[i]);
	  eff_pt_slice_b[i]->SetMarkerColor(2);
	  eff_pt_slice_b[i]->SetMarkerStyle(21);
	  eff_pt_slice[i]->Draw("AP");
	  eff_pt_slice_b[i]->Draw("P same");
  }

  /*
  //Check pT assymetry
  TCanvas *pt_assym = new TCanvas("pt_assym","pT assymetry",1000,800);
  TH1D* pt1_qcd_assym = (TH1D*) BgMC->Get("distributions/jet12_assym");
  TH1D *pt1_signal_assym[15];
  pt1_qcd_assym->SetMarkerStyle(20);
  pt1_qcd_assym->SetMarkerSize(1.5);
  pt1_qcd_assym->Draw("E");

  i = 0;
  for(const auto & f : SignalMC){
	  ++i;
	  pt1_signal_assym[i] = (TH1D*) f.second->Get("distributions/jet12_assym");
	  pt1_signal_assym[i]->SetMarkerColor(50+3*i);
	  pt1_signal_assym[i]->SetLineColor(50+3*i);
	  pt1_signal_assym[i]->SetMarkerStyle(20);
	  pt1_signal_assym[i]->Draw("E same");

  }


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
