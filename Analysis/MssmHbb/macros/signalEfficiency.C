#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"
#include "TGraph.h"
#include "TLegend.h"
#include "Drawer/HbbStyle.cc"

double xsection(const int & point);
void efficiency(const  std::vector<std::map<int,TFile* > >& files,const std::vector<std::string>& legenda);

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

void signalEfficiency(){
   gStyle->SetOptStat(0);
   TH1::SetDefaultSumw2();

   std::map<int,TFile* > inFlowM;
   std::map<int,TFile* > inFhighM;

   inFlowM[300] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[350] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-350_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[400] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-400_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[500]		= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[600]		= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-600_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[700] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-700_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[900] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-900_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[1100] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-1100_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());
   inFlowM[1300] 	= new TFile((cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_ReReco_lowM_clange-SUSYGluGluToBBHToBB_NarrowWidth_M-1300_TuneCUETP8M1_13TeV-pythia8_MiniAODv2_80X-b773ac922eef67f3b5600ed596c9ce54.root").c_str());

//   inFhighM[100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[120] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[160] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[200] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[250] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
//   inFhighM[500]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
//   inFhighM[600]	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
//   inFhighM[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
//   inFhighM[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
//   inFhighM[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
//   inFhighM[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_highM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");

   HbbStyle style;
   style.set(PRIVATE);
   style.drawStandardTitle();
   gStyle->SetTitleXOffset(1.);

   std::vector<std::map<int,TFile* > > datasets;
   datasets.push_back(inFlowM);
   std::vector<std::string> legends;
   legends.push_back("PromptReco MC");

   efficiency(datasets,legends);

/*
   //LowM
   TGraphErrors *Eff = new TGraphErrors(inFlowM.size());
   Eff->SetMarkerStyle(20);
   Eff->SetMarkerSize(1.1);
   Eff->SetTitle("Signal Efficiency; di-jet M, [GeV]; #epsilon");

   //highM
//   TGraphErrors *EffhighM = new TGraphErrors(inFhighM.size());
//   EffhighM->SetMarkerStyle(20);
//   EffhighM->SetMarkerColor(2);
//   EffhighM->SetMarkerSize(1.1);
//   EffhighM->SetTitle("; di-jet M, [GeV]; #epsilon");

   TCanvas * c1 = new TCanvas("c1","",700,800);
   TLegend *leg = new TLegend(0.25,0.7,0.45,0.85);
   leg->SetBorderSize(0);

   int i = 0;
   for(const auto & f : inFlowM){
	   ++i;

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   TH1* denum =  (TH1*) f.second->Get("distributions/NumberOfGenEvents_afterMHat_rewPU");
	   if(denum->GetMean() == 0){
		   denum = (TH1*) f.second->Get("distributions/TotalNumberOfGenEvents");
	   }
	   TH1 *integ = (TH1*) f.second->Get("bbH_Mbb");
	   Eff->SetPoint(i,f.first,(double) integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))/denum->GetMean());
   }
   i=0;

//   for(const auto & f : inFhighM){
//	   ++i;
//
//	   double low_border = f.first*0.8;
//	   double up_border	 = f.first*1.2;
//
//	   TH1* denum =  (TH1*) f.second->Get("distributions/NumberOfGenEvents_afterMHat");
//	   if(denum->GetMean() == 0){
//		   denum = (TH1*) f.second->Get("distributions/TotalNumberOfGenEvents");
//	   }
//	   TH1 *integ = (TH1*) f.second->Get("bbH_Mbb");
//	   EffhighM->SetPoint(i,f.first,(double) integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))/denum->GetMean());
//   }
//   EffhighM->Draw("AP");
   Eff->Draw("AP");
   style.drawStandardTitle();
   
   leg->AddEntry(Eff,"Low Mass","p");
//   leg->AddEntry(EffhighM,"High Mass","p");
   leg->Draw();

   /*
   //Sensitivity
   TH1D	*hBgLowM = (TH1D*) inFlowM[500]->Get("QCD_Mbb");
   TH1D	*hBgHighM = (TH1D*) inFhighM[500]->Get("QCD_Mbb");

   //LowM
   TGraphErrors *Sensitivity = new TGraphErrors(inFlowM.size());
   Sensitivity->SetMarkerStyle(20);
   Sensitivity->SetMarkerSize(1.1);
   Sensitivity->SetTitle("Signal Sensitivity = S/#sqrt{Bg}; di-jet M, [GeV]; S/#sqrt{Bg}");
   Sensitivity->GetXaxis()->SetTitleOffset(0.);

   //highM
   TGraphErrors *SensitivityHighM = new TGraphErrors(inFhighM.size());
   SensitivityHighM->SetMarkerStyle(20);
   SensitivityHighM->SetMarkerColor(2);
   SensitivityHighM->SetMarkerSize(1.1);
   SensitivityHighM->SetTitle("Signal Sensitivity = S/#sqrt{Bg}; di-jet M, [GeV]; #epsilon");

   TCanvas * c2 = new TCanvas("c2","",700,800);
   TLegend *leg2 = new TLegend(0.6,0.7,0.85,0.85);
   leg2->SetBorderSize(0);

   i = 0;
   std::cout<<"Low M:"<<std::endl;
   for(const auto & f : inFlowM){
	   ++i;
	   TH1 *s = (TH1*) f.second->Get("bbH_Mbb");

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   double signalErr_ = 0;
	   double signal_ = s->IntegralAndError(s->FindBin(low_border),s->FindBin(up_border),signalErr_);

	   double bgErr_ = 0;
	   double bg_ = hBgLowM->IntegralAndError(hBgLowM->FindBin(low_border),hBgLowM->FindBin(up_border),bgErr_);

	   double sens = signal_/std::sqrt(bg_) * xsection(f.first);
	   double sens_err = xsection(f.first) * std::sqrt(1./bg_ * signalErr_ * signalErr_ + signal_*signal_/(4. * bg_*bg_*bg_) * bgErr_* bgErr_);
	   Sensitivity->SetPoint(i,f.first,sens);
	   Sensitivity->SetPointError(i,0,sens_err);


	   std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<signal_<<" Bg_no_w= "<<hBgLowM->GetEntries()<<" +w= "<<bg_<<std::endl;
	   std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<s->IntegralAndError(1,s->GetNbinsX(),signalErr_)<<" Bg_no_w= "<<hBgLowM->GetEntries()<<" +w= "<<hBgLowM->IntegralAndError(1,s->GetNbinsX(),bgErr_)<<" \n"<<std::endl;
   }
   i = 0;
std::cout<<"High M: "<<std::endl;
   for(const auto & f : inFhighM){
	   ++i;
	   TH1 *s = (TH1*) f.second->Get("bbH_Mbb");

	   double low_border = f.first*0.8;
	   double up_border	 = f.first*1.2;

	   double signalErr_ = 0;
	   double signal_ = s->IntegralAndError(s->FindBin(low_border),s->FindBin(up_border),signalErr_);

	   double bgErr_ = 0;
	   double bg_ = hBgHighM->IntegralAndError(hBgLowM->FindBin(low_border),hBgLowM->FindBin(up_border),bgErr_);

	   double sens = xsection(f.first) * signal_/std::sqrt(bg_);
	   double sens_err = xsection(f.first) * std::sqrt(1./bg_ * signalErr_ * signalErr_ + signal_*signal_/(4. * bg_*bg_*bg_) * bgErr_* bgErr_);
	   SensitivityHighM->SetPoint(i,f.first,sens);
	   SensitivityHighM->SetPointError(i,0,sens_err);

        std::cout<<f.first<<" Signal_no_w = "<<s->GetEntries()<<" +w= "<<signal_<<" Bg_no_w= "<<hBgHighM->GetEntries()<<" +w= "<<bg_<<std::endl;  
 }
//   Sensitivity->GetYaxis()->SetRangeUser(0,10);
   Sensitivity->SetMinimum(0.);
   Sensitivity->SetTitle("");
   Sensitivity->Draw("AP");
   SensitivityHighM->Draw("Psame");
   style.drawStandardTitle();

   leg2->AddEntry(Sensitivity,"Low Mass","p");
   leg2->AddEntry(SensitivityHighM,"High Mass","p");
   leg2->Draw();


   c1->SaveAs("pictures/signalEfficiency.pdf");
   c2->SaveAs("pictures/signalSensitivity.pdf");
/**/
}

double xsection(const int &point){
	switch (point){
		case 100:
			return 5.23;
		case 120:
			return 41.79;
		case 160:
			return 75.32;
		case 200:
			return 35.42;
		case 250:
			return 15.55;
		case 300:
			return 7.64;
		case 350:
			return 4.1;
		case 400:
			return 2.34;
		case 500:
			return 0.88;
		case 600:
			return 0.377;
		case 700:
			return 0.18;
		case 900:
			return 0.051;
		case 1100:
			return 0.018;
		case 1300:
			return 0.008;
		default:
			std::cerr<<"No such mass point in the list of xsections"<<std::endl;
			exit(-1);
	}
}

void efficiency(const  std::vector<std::map<int,TFile* > >& files,const std::vector<std::string>& legenda){
	std::vector<short int> colours = {1,2,3,4,6,9,12,28,46};
	std::vector<TGraphErrors> Efficiencies;
	TCanvas can("can","",700,800);
	TLegend leg(0.25,0.7,0.45,0.85);
	//Loop  over the datasets:
	int j =0;
	for(const auto& set : files){
		//Create efficiency plot
		TGraphErrors *eff = new TGraphErrors(set.size());
		eff->SetMarkerStyle(20);
		eff->SetMarkerSize(1.1);
		eff->SetLineColor(colours.at(j));
		eff->SetTitle("Signal Efficiency; di-jet M, [GeV]; #epsilon");
		int i = 0;
		for(const auto& f : set){
			++i;
			double low_border = f.first*0.8;
			double up_border	 = f.first*1.2;

			TH1* denum =  (TH1*) f.second->Get("distributions/TotalNumberOfGenEvents");
			TH1 *integ = (TH1*) f.second->Get("bbH_Mbb");
			double value = integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))/denum->GetMean();
			eff->SetPoint(i,f.first,value);
			std::cout<<"M12 = "<<f.first<<" Eff. = "<<value<<" Tot = "<<denum->GetMean()<<" Sel. = "<<integ->Integral(integ->FindBin(low_border),integ->FindBin(up_border))<<std::endl;
		}
		i = 0;
		++j;
		Efficiencies.push_back(*eff);
	}

	for(unsigned int i = 0; i != Efficiencies.size(); ++i){
		TGraphErrors& eff = Efficiencies.at(i);
		if(i==0) eff.Draw("AP");
		else eff.Draw("Psame");

		leg.AddEntry(&eff,(legenda.at(i)).c_str(),"p");
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/SignalEfficiency.pdf").c_str());
}
