#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "TEfficiency.h"

TEfficiency * GetEfficiency(TFile * f, const std::string & name_num, const std::string & name_den);
void BayesianErrors(TEfficiency *h);

int flavour_dependency(){

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
   TFile *BgMC_b = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/TriggerEff_slimmed_lowM_QCD_b_0p4.root");

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
	TEfficiency *PF100Eff_qcd 	= (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1");
	PF100Eff_qcd->SetMarkerStyle(21);
	PF100Eff_qcd->SetMarkerColor(kMagenta);
	TEfficiency *PF100Eff		= (TEfficiency*) GetEfficiency(fDataLowM,"TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1");
	PF100Eff->SetMarkerStyle(20);
	PF100Eff->SetTitle(";p^{(1)}_{T}, [GeV]; #epsilon");
	TLegend *leg = new TLegend(0.5,0.2,0.9,0.45);
	leg-> SetNColumns(4);
   TH1D *PF100Eff_Num_signal_mc[15], *PF100Eff_Den_signal_mc[15];
   TEfficiency *PF100Eff_signal_mc[15];
   leg->AddEntry(PF100Eff,"JetHT","lp");
   leg->AddEntry(PF100Eff_qcd,"QCD #hat{p_{T}}","lp");
   PF100Eff->Draw("AP");
   PF100Eff_qcd->Draw("E same");
   gPad->Update();
   PF100Eff->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
   PF100Eff->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);

   int i = 0;
   for(const auto & f : SignalMC){
	   ++i;
	   PF100Eff_signal_mc[i]	= (TEfficiency*) GetEfficiency(f.second,"TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1");
	   PF100Eff_signal_mc[i]->SetMarkerColor(50+3*i);
	   PF100Eff_signal_mc[i]->SetLineColor(50+3*i);
	   PF100Eff_signal_mc[i]->SetMarkerStyle(20);
	   PF100Eff_signal_mc[i]->Draw("same E");
	   PF100Eff->Draw("same E");
	   leg->AddEntry(PF100Eff_signal_mc[i],(std::to_string(f.first) + " Signal").c_str(),"p");
   }
   leg->Draw();
   style.drawStandardTitle();
   oneDFitLowM->Print(("../pictures/TriggerPerformance/KinTrigEff_AllSamples_1D_PF80_PF100_pt1.pdf"));


  TCanvas *can_bg_comp = new TCanvas("can_bg_comp","BG comparison",1000,800);
  TLegend *leg_bg_qcd_comp = new TLegend(0.7,0.5,0.9,0.7);
  TEfficiency *PF100Eff_qcd_true_b = (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_PF80_PF100_pt1");
  PF100Eff_qcd_true_b->SetMarkerStyle(20);
  leg_bg_qcd_comp->SetHeader("QCD #hat{p_{T}}");
  leg_bg_qcd_comp->AddEntry(PF100Eff_qcd_true_b,"Hadron flavour b","p");
  leg_bg_qcd_comp->AddEntry(PF100Eff_qcd,"All jets","p");
  PF100Eff_qcd_true_b->SetTitle(";p^{(1)}_{T}, [GeV]; #epsilon");
  PF100Eff_qcd_true_b->Draw("AP");
  PF100Eff_qcd->Draw("P same");
  gPad->Update();
  PF100Eff_qcd_true_b->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  PF100Eff_qcd_true_b->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
  leg_bg_qcd_comp->Draw();
  style.drawStandardTitle();
  can_bg_comp->Print(("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_1D_PF80_PF100_pt1.pdf"));

  TPaveText *eta_def[3];
  TH2D *h_pt_eta_num 	= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_num_b 	= (TH2D*)	BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Num_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_den 	= (TH2D*)	BgMC->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
  TH2D *h_pt_eta_den_b 	= (TH2D*)	BgMC_b->Get("TriggerEfficiencies/KinTrigEff_Den_2D_PF80_PF100_pt1eta1");
  TLegend *leg_pt_eta_bins = new TLegend(0.7,0.5,0.9,0.7);

  TH1D *h_pt_num_slice[3], *h_pt_num_slice_b[3], *h_pt_den_slice[3], *h_pt_den_slice_b[3];
  TEfficiency *eff_pt_slice[3], *eff_pt_slice_b[3];
  TCanvas *can_pt_slice[3];
  std::string eta_bins[3]={"0-0.6","0.6-1.7","1.7-2.2"};

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
	  eff_pt_slice_b[i]->SetMarkerColor(kMagenta);
	  eff_pt_slice_b[i]->SetMarkerStyle(21);
	  eff_pt_slice[i]->SetTitle(";p^{(1)}_{T}, [GeV]; #epsilon");
	  eta_def[i] = new TPaveText(0.7,0.7,0.9,0.8,"NDC");
	  eta_def[i]->SetTextSize(leg_bg_qcd_comp->GetTextSize()/2.);
	  eta_def[i]->SetFillColor(0);
	  eta_def[i]->SetBorderSize(0);
	  eta_def[i]->AddText(("|#eta| = "+eta_bins[i]).c_str());
	  eff_pt_slice[i]->Draw("AP");
	  eff_pt_slice_b[i]->Draw("P same");

	  gPad->Update();
	  eff_pt_slice[i]->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
	  eff_pt_slice[i]->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
	  eff_pt_slice_b[i]->Draw("P same");
	  eta_def[i]->Draw();
	  style.drawStandardTitle();
	  can_pt_slice[i]->Print(("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_2D_PF80_PF100_pt1eta1_"+eta_bins[i] + ".pdf").c_str());
  }
  leg_bg_qcd_comp->Draw();
  style.drawStandardTitle();


  /****************** Level by level ******************/

  TCanvas *can_l1 = new TCanvas("can_l1","can_l1",1000,800);
  TEfficiency *eff_only_l1 		= (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_L1_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L1_PF80_PF100_pt1");
  eff_only_l1->SetMarkerStyle(20);
  eff_only_l1->SetTitle("1D Kinematic Trigger Efficiency with PF80 f(pt1) with only L1 matching;p^{(1)}_{T}, [GeV]; #epsilon");
  TEfficiency *eff_only_l1_b	= (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_L1_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L1_PF80_PF100_pt1");
  eff_only_l1_b->SetMarkerColor(kMagenta);
  eff_only_l1_b->SetMarkerStyle(21);
  eff_only_l1->Draw("AP");
  eff_only_l1_b->Draw("P same");
  gPad->Update();
  eff_only_l1->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  eff_only_l1->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
  eff_only_l1_b->Draw("P same");
  leg_bg_qcd_comp->Draw();
  can_l1->Print("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_1D_L1_PF80_PF100_pt1.pdf");

  TCanvas *can_l2 = new TCanvas("can_l2","can_l2",1000,800);
  TEfficiency *eff_only_l2 		= (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_L2_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L2_PF80_PF100_pt1");
  eff_only_l2->SetMarkerStyle(20);
  eff_only_l2->SetTitle("1D Kinematic Trigger Efficiency with PF80 f(pt1) with only L2 matching;p^{(1)}_{T}, [GeV]; #epsilon");
  TEfficiency *eff_only_l2_b	= (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_L2_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L2_PF80_PF100_pt1");
  eff_only_l2_b->SetMarkerColor(kMagenta);
  eff_only_l2_b->SetMarkerStyle(21);
  eff_only_l2->Draw("AP");
  eff_only_l2_b->Draw("P same");
  gPad->Update();
  eff_only_l2->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  eff_only_l2->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
  eff_only_l2_b->Draw("P same");
  leg_bg_qcd_comp->Draw();
  can_l2->Print("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_1D_L2_PF80_PF100_pt1.pdf");

  TCanvas *can_l3 = new TCanvas("can_l3","can_l3",1000,800);
  TEfficiency *eff_only_l3 		= (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_L3_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L3_PF80_PF100_pt1");
  eff_only_l3->SetMarkerStyle(20);
  eff_only_l3->SetTitle("1D Kinematic Trigger Efficiency with PF80 f(pt1) with only L3 matching;p^{(1)}_{T}, [GeV]; #epsilon");
  TEfficiency *eff_only_l3_b	= (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_L3_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_L3_PF80_PF100_pt1");
  eff_only_l3_b->SetMarkerColor(kMagenta);
  eff_only_l3_b->SetMarkerStyle(21);
  eff_only_l3->Draw("AP");
  eff_only_l3_b->Draw("P same");
  gPad->Update();
  eff_only_l3->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  eff_only_l3->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
  eff_only_l3_b->Draw("P same");
  leg_bg_qcd_comp->Draw();
  can_l3->Print("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_1D_L3_PF80_PF100_pt1.pdf");

  /***************************************************/

  /*******************NO MUONS***********************/

  TCanvas *can_nomu = new TCanvas("can_nomu","can_nomu",1000,800);
  TEfficiency *eff_only_noMuons 		= (TEfficiency*) GetEfficiency(BgMC,"TriggerEfficiencies/KinTrigEff_Num_1D_noMuons_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_noMuons_PF80_PF100_pt1");
  eff_only_noMuons->SetMarkerStyle(20);
  eff_only_noMuons->SetTitle("1D Kinematic Trigger Efficiency with PF80 f(pt1) with muon subtraction;p^{(1)}_{T}, [GeV]; #epsilon");
  TEfficiency *eff_only_noMuons_b	= (TEfficiency*) GetEfficiency(BgMC_b,"TriggerEfficiencies/KinTrigEff_Num_1D_noMuons_PF80_PF100_pt1","TriggerEfficiencies/KinTrigEff_Den_1D_noMuons_PF80_PF100_pt1");
  eff_only_noMuons_b->SetMarkerColor(kMagenta);
  eff_only_noMuons_b->SetMarkerStyle(21);
  eff_only_noMuons->Draw("AP");
  eff_only_noMuons_b->Draw("P same");
  gPad->Update();
  eff_only_noMuons->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.,1.1);
  eff_only_noMuons->GetPaintedGraph()->GetXaxis()->SetTitleOffset(1.1);
  eff_only_noMuons_b->Draw("P same");
  leg_bg_qcd_comp->Draw();
  can_nomu->Print("../pictures/TriggerPerformance/KinTrigEff_flavorStudy_1D_noMuons_PF80_PF100_pt1.pdf");

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
