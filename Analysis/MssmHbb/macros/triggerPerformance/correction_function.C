#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "TEfficiency.h"

TEfficiency * GetEfficiency(TFile * f, const std::string & name_num, const std::string & name_den);
void BayesianErrors(TEfficiency *h);

int correction_function(){

	TH1::SetDefaultSumw2();

	TFile *fDataLowM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_Run2015D-16Dec2015-v1.root");
	TFile *fDataHighM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_highM_Run2015D-16Dec2015-v1.root");
//

	std::map<int,TFile* > SignalMC;
   SignalMC[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   SignalMC[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");

   //BG MC:
   TFile *BgMC = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_lowM_QCD.root");
   TFile *BgMC_high = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_highM_QCD.root");

   std::string error_mode = "";

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup Ratio
   RatioPlots ratio(PRIVATE);
   ratio.SetRatioRange(0.5,1.5);
   ratio.SetRatioTitle("Data/MC");
   TH1 *nullH = nullptr;

   /****************************LOW M*************************/
  TPaveText *eta_def[3];
  TLegend *leg_low_m = new TLegend(0.7,0.5,0.9,0.7);
  TH2D *h_pt_eta_num 		= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF60_PF100_pt1eta1");
  TH2D *h_pt_eta_num_data 	= (TH2D*)	fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF60_PF100_pt1eta1");
  TH2D *h_pt_eta_den 		= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF60_PF100_pt1eta1");
  TH2D *h_pt_eta_den_data 	= (TH2D*)	fDataLowM->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF60_PF100_pt1eta1");

  TH1D *h_pt_num_slice[3], *h_pt_num_slice_data[3], *h_pt_den_slice[3], *h_pt_den_slice_data[3];
  TEfficiency *eff_pt_slice[3], *eff_pt_slice_data[3];
  TGraphAsymmErrors *ratio_pt_slice[3];
  TCanvas *can_pt_slice[3];
  std::string eta_bins[3]={"0-0.6","0.6-1.7","1.7-2.2"};

  for(int i = 0; i<3;++i){
	  can_pt_slice[i] = new TCanvas(("can_pt_slice_"+std::to_string(i)).c_str(),"canv",1000,800);
	  h_pt_num_slice[i] = (TH1D*)  h_pt_eta_num->ProjectionX(("Slice_num_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_pt_den_slice[i] = (TH1D*)  h_pt_eta_den->ProjectionX(("Slice_den_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_pt_slice[i] = new TEfficiency(*h_pt_num_slice[i],*h_pt_den_slice[i]);
	  eff_pt_slice[i]->SetMarkerStyle(21);
	  eff_pt_slice[i]->SetMarkerColor(kMagenta);
	  BayesianErrors(eff_pt_slice[i]);

	  h_pt_num_slice_data[i] = (TH1D*)  h_pt_eta_num_data->ProjectionX(("Slice_num_data_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_pt_den_slice_data[i] = (TH1D*)  h_pt_eta_den_data->ProjectionX(("Slice_den_data_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_pt_slice_data[i] = new TEfficiency(*h_pt_num_slice_data[i],*h_pt_den_slice_data[i]);
	  BayesianErrors(eff_pt_slice_data[i]);
	  eff_pt_slice_data[i]->SetMarkerColor(1);
	  eff_pt_slice_data[i]->SetMarkerStyle(20);
	  eff_pt_slice[i]->SetTitle(";p^{(1)}_{T}, [GeV]; #epsilon");
	  eta_def[i] = new TPaveText(0.7,0.7,0.9,0.8,"NDC");
	  eta_def[i]->SetFillColor(0);
	  eta_def[i]->SetBorderSize(0);
	  eta_def[i]->AddText(("|#eta| = "+eta_bins[i]).c_str());
	  eff_pt_slice[i]->Draw("AP");
	  eff_pt_slice_data[i]->Draw("P same");

	  gPad->Update();
	  eff_pt_slice[i]->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
	  eff_pt_slice[i]->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
	  eff_pt_slice_data[i]->Draw("P same");
//	  ratio_pt_slice[i] = (TGraphAsymmErrors*) ratio.DrawRatio((TGraphAsymmErrors*) eff_pt_slice_data[i]->GetPaintedGraph()->Clone(),(TGraphAsymmErrors*) eff_pt_slice[i]->GetPaintedGraph()->Clone(), can_pt_slice[i],nullptr);
//	  ratio.GetTopPad()->cd();
	  style.drawStandardTitle();
	  eta_def[i]->Draw();
	  can_pt_slice[i]->Print(("../pictures/TriggerPerformance/CorrectionFunction_ratio_PF60_PF100_pt1_eta1_"+eta_bins[i] + ".pdf").c_str());
  }
  leg_low_m->AddEntry(eff_pt_slice_data[2],"JetHT, 2015D","pl");
  leg_low_m->AddEntry(eff_pt_slice[2],"QCD, #hat{p_{T}}","pl");
  leg_low_m->Draw();
  style.drawStandardTitle();
  
  TCanvas *can_correction_funct[3];
  for(int i=0;i<3;++i){
  can_correction_funct[i] = new TCanvas();
  	ratio_pt_slice[i] = (TGraphAsymmErrors*) ratio.DrawRatio((TGraphAsymmErrors*) eff_pt_slice_data[i]->GetPaintedGraph()->Clone(),(TGraphAsymmErrors*) eff_pt_slice[i]->GetPaintedGraph()->Clone(), can_correction_funct[i],nullptr);
  	can_correction_funct[i]->cd();
  	ratio_pt_slice[i]->GetXaxis()->SetTitleOffset(1.1);
  	ratio_pt_slice[i]->GetXaxis()->SetTitle("p^{(1)}_{T}, [GeV]");
  	ratio_pt_slice[i]->Draw("AP");
  	eta_def[i]->Draw();
  	style.drawStandardTitle();
  	can_correction_funct[i]->Print(("../pictures/TriggerPerformance/CorrectionFunction_PF60_PF100_pt1_eta1_"+eta_bins[i] + ".pdf").c_str());
  }
  /************************************************************/

  /**************************high M****************************/
  TH2D *h_highM_pt_eta_num 			= (TH2D*)	BgMC_high->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF160_pt1eta1");
  TH2D *h_highM_pt_eta_num_data 	= (TH2D*)	fDataHighM->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF160_pt1eta1");
  TH2D *h_highM_pt_eta_den 			= (TH2D*)	BgMC_high->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF160_pt1eta1");
  TH2D *h_highM_pt_eta_den_data 	= (TH2D*)	fDataHighM->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF160_pt1eta1");

  TH1D *h_highM_pt_num_slice[3], *h_highM_pt_num_slice_data[3], *h_highM_pt_den_slice[3], *h_highM_pt_den_slice_data[3];
  TEfficiency *eff_highM_pt_slice[3], *eff_highM_pt_slice_data[3];
  TGraphAsymmErrors *ratio_highM_pt_slice[3];
  TCanvas *can_highM_pt_slice[3];

  for(int i = 0; i<3;++i){
	  can_highM_pt_slice[i] = new TCanvas(("can_highM_pt_slice_"+std::to_string(i)).c_str(),"canv",1000,800);
	  h_highM_pt_num_slice[i] = (TH1D*)  h_highM_pt_eta_num->ProjectionX(("Slice_num_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_highM_pt_den_slice[i] = (TH1D*)  h_highM_pt_eta_den->ProjectionX(("Slice_den_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_highM_pt_slice[i] = new TEfficiency(*h_highM_pt_num_slice[i],*h_highM_pt_den_slice[i]);
	  eff_highM_pt_slice[i]->SetMarkerStyle(21);
	  eff_highM_pt_slice[i]->SetMarkerColor(kMagenta);
	  BayesianErrors(eff_highM_pt_slice[i]);

	  h_highM_pt_num_slice_data[i] = (TH1D*)  h_highM_pt_eta_num_data->ProjectionX(("Slice_num_data_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  h_highM_pt_den_slice_data[i] = (TH1D*)  h_highM_pt_eta_den_data->ProjectionX(("Slice_den_data_"+std::to_string(i)).c_str(),i+1,i+1,"E");
	  eff_highM_pt_slice_data[i] = new TEfficiency(*h_highM_pt_num_slice_data[i],*h_highM_pt_den_slice_data[i]);
	  BayesianErrors(eff_highM_pt_slice_data[i]);
	  eff_highM_pt_slice_data[i]->SetMarkerColor(1);
	  eff_highM_pt_slice_data[i]->SetMarkerStyle(20);
	  eff_highM_pt_slice[i]->SetTitle(";p^{(1)}_{T}, [GeV]; #epsilon");
	  eff_highM_pt_slice[i]->Draw("AP");
	  eff_highM_pt_slice_data[i]->Draw("P same");

	  gPad->Update();
	  eff_highM_pt_slice[i]->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
	  eff_highM_pt_slice[i]->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
	  eff_highM_pt_slice_data[i]->Draw("P same");
//	  ratio_highM_pt_slice[i] = (TGraphAsymmErrors*) ratio.DrawRatio((TGraphAsymmErrors*) eff_highM_pt_slice_data[i]->GetPaintedGraph()->Clone(),(TGraphAsymmErrors*) eff_highM_pt_slice[i]->GetPaintedGraph()->Clone(), can_highM_pt_slice[i],nullptr);
//	  ratio.GetTopPad()->cd();
	  style.drawStandardTitle();
	  eta_def[i]->Draw();
	  can_highM_pt_slice[i]->Print(("../pictures/TriggerPerformance/CorrectionFunction_ratio_PF80_PF160_pt1_eta1_"+eta_bins[i] + ".pdf").c_str());
  }
  leg_low_m->Draw();
  style.drawStandardTitle();

  TCanvas *can_highM_correction_funct[3];
  for(int i=0;i<3;++i){
  can_highM_correction_funct[i] = new TCanvas();
  	ratio_highM_pt_slice[i] = (TGraphAsymmErrors*) ratio.DrawRatio((TGraphAsymmErrors*) eff_highM_pt_slice_data[i]->GetPaintedGraph()->Clone(),(TGraphAsymmErrors*) eff_highM_pt_slice[i]->GetPaintedGraph()->Clone(), can_highM_correction_funct[i],nullptr);
  	can_highM_correction_funct[i]->cd();
  	ratio_highM_pt_slice[i]->GetXaxis()->SetTitleOffset(1.1);
  	ratio_highM_pt_slice[i]->GetXaxis()->SetTitle("p^{(1)}_{T}, [GeV]");
  	ratio_highM_pt_slice[i]->Draw("AP");
  	style.drawStandardTitle();
  	eta_def[i]->Draw();
  	can_highM_correction_funct[i]->Print(("../pictures/TriggerPerformance/CorrectionFunction_PF80_PF160_pt1_eta1_"+eta_bins[i] + ".pdf").c_str());
  }

  /************************************************************/

	return 0;


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
		out->SetTitle(num->GetTitle());
	}
	else {
		num2D = (TH2D*) f->Get(name_num.c_str());
		den2D = (TH2D*) f->Get(name_den.c_str());
		out = new TEfficiency(*num2D,*den2D);
		out->SetTitle(num2D->GetTitle());
	}
	BayesianErrors(out);

	return out;
}
