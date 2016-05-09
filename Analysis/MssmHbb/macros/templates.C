#include <memory>
#include <map>
#include <iostream>
#include "TFile.h"
#include "TExec.h"

void SetBottomStyle(TH1 *hRatio);

void templates()
{
	TH1::SetDefaultSumw2();
   gStyle->SetOptStat(0);

   std::map<int,TFile* > inF;
   std::vector<TH1D*> histo;

   TExec *er_0 = new TExec("er_0","gStyle->SetErrorX(0)");
   TExec *er_1 = new TExec("er_1","gStyle->SetErrorX(0.5)");

   inF[300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
   inF[350] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root");
   inF[400] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
   inF[500]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
   inF[600]		= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
   inF[700] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
   inF[900] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
   inF[1100] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
   inF[1300] 	= new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/Selection_lowM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");


   TLegend *leg = new TLegend(0.3,0.3,0.9,0.85);
   leg->SetBorderSize(0);

   TCanvas * c1[15];

   int i = 0, j =0, k=0;
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
	   c1[i] = new TCanvas();
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
	   }
	   j=0;

	   if(i==1) leg->Draw();
	   c1[i]->SaveAs(("pictures/AllinOne_"+std::to_string(f.first)+"_GeV_mass_point.pdf").c_str());
	   histo.clear();

   }
/**/
   //One by one
   TCanvas *singleC[15][15];
   TPad *cPad[15][15];
   TPad *uPad[15][15];
   TPad	*dPad[15][15];
   TLegend *singleL[15][15];
   std::map<std::string,std::pair<TH1D*, TH1D*> > singleH;

   i = 0, j =0, k=0;
   for(const auto & f : inF){
	   ++i;
	   TH1D * first = (TH1D*) f.second->Get("Mbb");
	   first->SetLineWidth(2);
	   singleH["SFb"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_SFb_13TeVUp"));
	   singleH["SFudsg"] = std::make_pair((TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVDown"),(TH1D*) f.second->Get("bbH_Mbb_CMS_SFl_13TeVUp"));
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
//		   first->SetTitle((std::to_string(f.first)+ " GeV mass point, " + p.first+" variation").c_str());

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
		   centrD->Divide((p.second).second);
		   centrD->SetTitle("");
		   centrD->SetMarkerStyle(20);
		   SetBottomStyle(centrD);
		   centrD->Draw();

		   TLine *horizLine2 = new TLine(centrD->GetXaxis()->GetXmin(),1,centrD->GetXaxis()->GetXmax(),1);
		   horizLine2 -> SetLineStyle(2);
		   horizLine2 -> Draw();

		   std::string name = "pictures/"+std::to_string(f.first)+ "_GeV_mass_point_" + p.first+"_variation.pdf";
		   singleC[i-1][j-1]->SaveAs(name.c_str());
	   }
	   j=0;

/**/
   }

   
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
