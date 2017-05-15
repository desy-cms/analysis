/*
 * FlavourContamination_2b.cpp
 *
 *  Created on: 13 Mar 2017
 *      Author: shevchen
 */

#include <string>

#include "TSystem.h"
#include "TH1.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/interface/utilLib.h"

using namespace std;
using namespace mssmhbb;

RatioPlots style;

void renormalise(TH1*);
void DrawFlavourContamination(const std::string& data, const std::string& qcd, const std::string& ttbar, const bool& logY);
void DrawQCDtoTTratio(const std::string& qcd, const std::string& ttbar, const bool& logY = false);

string mode="2b";

int main(){
	//Input from QCD and tt MC:
	string qcd 		= cmsswBase + "/src/Analysis/MssmHbb/output/DataMC_ReReco_35673fb_lowM_QCD_Pt.root";
	string mode_name= "";
	if(mode=="3b") mode_name = "_3b";
	string qcd_benr	= cmsswBase + "/src/Analysis/MssmHbb/output/DataMC" + mode_name +"_ReReco_35673fb_lowM_bEnriched_Tot.root";
	string ttbar 	= cmsswBase + "/src/Analysis/MssmHbb/output/DataMC" + mode_name +"_ReReco_35673fb_lowM_TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.root";
	//Input data
	string data		= cmsswBase + "/src/Analysis/MssmHbb/output/DataMC_ReReco_35673fb_lowM_Run2016.root";

	//Set drawing style
	style.set(PRIVATE);
	DrawFlavourContamination(data,qcd,ttbar,true);
	DrawQCDtoTTratio(qcd_benr, ttbar,true);
	return 0;
}

void DrawFlavourContamination(const std::string& data, const std::string& qcd, const std::string& ttbar, const bool& logY){
	/*
	 * Function to draw flavour contamination
	 */
	TFile fQCD(qcd.c_str(),"READ");
	CheckZombie(fQCD);
	TFile fTTbar(ttbar.c_str(),"READ");
	CheckZombie(fTTbar);
	TFile fData(data.c_str(),"READ");
	CheckZombie(fData);

	//histo name
	string hname = "diJet_m";
	string folder = "general";

	TCanvas can("can","can",800,600);
	THStack flavourComposition("flavourComposition","");
	flavourComposition.SetTitle(";di-jet Mass (GeV); dN / d(di-jet Mass) (1/GeV)");
	TLegend flavourCompLegend = *style.legend("top, right",8,0.4);

	TH1F *hData = (TH1F*) fData.Get((folder + "/" + hname).c_str());
	flavourCompLegend.AddEntry(hData,"Data","p");

//	array<string,7> flavours 	{{"bb","cc","ll","bc","bl","cl","tt"}};
	array<string,7> flavours 	{{"cl","ll","tt","bl","cc","bc","bb"}};
	array<int,7> colours 		{{kCyan+2,kYellow-6,kMagenta+2,kRed+2,kYellow+2,kGreen+2,kBlue-6}};
	array<TH1*,7> histograms;

	//Get flavour histograms
	for(const auto& fl : flavours){
		//current index
		auto index = &fl - &flavours[0];
		TFile *f = &fTTbar;
		hname = "diJet_m";
		if(fl != "tt" ) {
			f = &fQCD;
			hname += "_" + fl;
		}
		//get histo
//		CheckZombieObjectInTFile(*f, hname);
		histograms[index] = (TH1F*) f->Get((folder + "/" + hname).c_str());

		//histo style
		renormalise((TH1F*) histograms[index]);
		histograms[index]->Rebin(3);
		histograms[index]->SetFillColor(colours[index]);
		histograms[index]->SetLineColor(colours[index]);

		//Add THStack
		flavourComposition.Add(histograms[index]);
	}

	//To get the proper fraction - have to iterate again:
	for(const auto& h : histograms){
		double fraction  = h->Integral() / ((TH1*) flavourComposition.GetStack()->Last())->Integral();
		auto index = &h - &histograms[0];
		string leg_entry = flavours[index] + " " + to_string_with_precision(fraction, 2);
		//Add TLegend Entry
		flavourCompLegend.AddEntry(h,leg_entry.c_str(),"flg");
	}
//	flavourComposition.Sizeof()

	renormalise(hData);
	hData->SetMinimum(1);
	hData->Rebin(3);
	hData->SetTitle(flavourComposition.GetTitle());
	hData->SetMarkerStyle(20);
	hData->SetMarkerSize(1.);
	hData->SetMaximum(hData->GetMaximum()*1.2);
	if(logY) gPad->SetLogy();
	hData->Draw();
	flavourComposition.Draw("hist same");
	hData->Draw("same");
	flavourCompLegend.Draw();

	can.Print((cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/DataVsMC/FlavourComposition_vs_Data.pdf").c_str());

	//Draw bb/ALL as well
	TCanvas can2("can2","can2",800,600);
	TH1F *bb_all = (TH1F*) histograms[6]->Clone("bb_all");
	bb_all -> Divide( ((TH1*) flavourComposition.GetStack()->Last()) );
	bb_all->SetMarkerStyle(20);
	bb_all->SetTitle("bb vs All");
	bb_all->Draw();
	can2.Print((cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/DataVsMC/FlavourComposition_bb_vs_all.pdf").c_str());

}

void DrawQCDtoTTratio(const std::string& qcd, const std::string& ttbar, const bool& logY){
	/*
	 * Method to Draw ratio of bb to tt,
	 * to check the shape difference
	 */
	TH1::SetDefaultSumw2();

	TFile fQCD(qcd.c_str(),"READ");
	CheckZombie(fQCD);
	TFile fTTbar(ttbar.c_str(),"READ");
	CheckZombie(fTTbar);

	//histo name
	string hname = "diJet_b_m";//"jet_b_pt1";//
	string folder = "diJet_b_m";//"jet_b_pt1";//

	TH1D *hqcd = (TH1D*) fQCD.Get( (folder + "/" + hname ).c_str() );
	TH1D *tt = (TH1D*) fTTbar.Get( (folder + "/" + hname).c_str() );

	renormalise(hqcd);
	renormalise(tt);

	cout<<"WTF: "<<hqcd->GetNbinsX()<<" tt: "<<tt->GetNbinsX()<<endl;

	hqcd->SetMarkerStyle(20);
	hqcd->SetMarkerSize(1.05);

	tt->SetFillColor(kRed-10);
	TH1D *tt_stat = (TH1D*) tt->Clone("stat");

	TLegend *leg = style.legend("top,right",3);
	leg->AddEntry(hqcd,"QCD MC","p");
	leg->AddEntry(tt,"t#bar{t} MC","fl");
	leg->AddEntry(tt_stat,"t#bar{t} stat.","f");

	TCanvas *can = new TCanvas("can","can",800,600);
	hqcd->SetTitle("QCD vs t#bar{t}");
	TH1D *ratio = (TH1D*) style.DrawRatio(hqcd, tt, can, leg, nullptr,tt_stat);
	ratio->SetTitle("");
	ratio->GetYaxis()->SetTitle("QCD/tt");
	ratio->SetAxisRange(0.,100.,"Y");
	gPad->RedrawAxis();
	style.GetTopPad();
	leg->Draw();
	hqcd->SetMinimum(1e-06);
	double multi_factor = 1.2;

	if(logY){
		multi_factor = 8.;
		gPad->SetLogy();
	}
	hqcd->SetMaximum(multi_factor*hqcd->GetMaximum());

	double tt_ntot = ((TH1F*) fTTbar.Get("general/diJet_m"))->Integral();
	double qcd_ntot = ((TH1F*) fQCD.Get("general/diJet_m"))->Integral();
	cout<<"Ratio tt/(tt+qcd) :"<<tt_ntot / (tt_ntot + qcd_ntot) * 100<<endl;

	can->Print((cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/DataVsMC/FlavourComposition" + "_" + mode + "_qcd_vs_tt.pdf").c_str());

	// Shape comparison
	hqcd->Scale(1./hqcd->Integral());
	tt_stat->Scale(1./tt_stat->Integral());
	tt->Scale(1./tt->Integral());
	ratio = (TH1D*) style.DrawRatio(hqcd, tt, can, leg, nullptr,tt_stat);
	ratio->SetTitle("");
	ratio->SetAxisRange(0.,2.,"Y");
	if(logY){
		multi_factor = 1.5;
		gPad->SetLogy();
	}
	style.GetTopPad();
	leg->Draw();
	gPad->RedrawAxis();

	can->Print((cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/DataVsMC/FlavourComposition" + "_" + mode + "_qcd_vs_tt_shape.pdf").c_str());

}

void renormalise(TH1 * h){
	for(int i=1; i<= h->GetNbinsX();++i){
		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
		h->SetBinError(i,h->GetBinError(i)/h->GetBinWidth(i));
	}
}
