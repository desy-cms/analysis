#include <iostream>
#include <string.h>
#include "TGraphErrors.h"

using namespace std;

TH1 * style74(TH1 *h){
	h->SetMarkerStyle(21);
	h->SetMarkerColor(2);
	h->SetMarkerSize(1.1);
	h->SetStats(kFALSE);

	return h;
}

TH1 * style76(TH1 *h){
	h->SetMarkerStyle(20);
	h->SetMarkerColor(1);
	h->SetMarkerSize(1.1);
	h->SetStats(kFALSE);

	return h;
}

int bla(){

	TH1::SetDefaultSumw2();

	/*
	 *
	//Data
	auto f76X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_lowMTrigger_76X_08_03_2016.root");
	auto f74X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_lowMTrigger_74X_08_03_2016.root");
	auto t76X = (TTree*) f76X->Get("MssmHbb");
	auto t74X = (TTree*) f74X->Get("MssmHbb");

	auto cPt = new TCanvas("cPt","Pt",1000,800);
	auto pt76X = new TH1D("pt76X","p_{T}, [GeV], Leading jet",100,0,2000);
	pt76X->SetMarkerStyle(20);
	pt76X->SetStats(kFALSE);
	auto pt74X = new TH1D("pt74X","p_{T}, [GeV]",100,0,2000);

	t76X->Draw("LeadPt[0]>>pt76X","LeadBTag[0]>0.941 && LeadBTag[1] > 0.941","E");
	t74X->Draw("LeadPt[0]>>pt74X","LeadBTag[0]>0.941 && LeadBTag[1] > 0.941","same");

	std::cout<<"	        74X 	 76X			"<<std::endl;
	std::cout<<"BTag=0.941: "<<pt74X->Integral()<<" "<<pt76X->Integral()<<std::endl;

	t76X->Draw("LeadPt[0]>>pt76X","LeadBTag[0]>0.935 && LeadBTag[1] > 0.935","E");
	t74X->Draw("LeadPt[0]>>pt74X","LeadBTag[0]>0.935 && LeadBTag[1] > 0.935","same");

	std::cout<<"BTag=0.935: "<<pt74X->Integral()<<" "<<pt76X->Integral()<<std::endl;

	t76X->Draw("LeadPt[0]>>pt76X","LeadBTag[0]>0.935 && LeadBTag[1] > 0.935","E");
	t74X->Draw("LeadPt[0]>>pt74X","LeadBTag[0]>0.97 && LeadBTag[1] > 0.97","same");

	std::cout<<"BTag: 0.97 "<<pt74X->Integral()<<" 0.935 "<<pt76X->Integral()<<std::endl;

	auto cBTag = new TCanvas("cBTag","BTag",1000,800);
	auto btag76X = new TH1D("btag76X","BTag_{CSVv2}",100,0,1);
	btag76X->SetMarkerStyle(20);
	auto btag74X = new TH1D("btag74X","BTag_{CSVv2}, Leading jet",100,0,1);
	btag74X->SetStats(kFALSE);

	t74X->Draw("LeadBTag[0]>>btag74X","","");
	t76X->Draw("LeadBTag[0]>>btag76X","","E same");

	auto cdEta = new TCanvas("cdEta","dETA",1000,800);
	auto deta76X = new TH1D("deta76X","dEta12",100,0,1.7);
	deta76X->SetMarkerStyle(20);
	auto deta74X = new TH1D("deta74X","#Delta #eta",100,0,1.7);
	deta74X->SetStats(kFALSE);

	t74X->Draw("abs(LeadEta[0]-LeadEta[1])>>deta74X","","");
	t76X->Draw("abs(LeadEta[0]-LeadEta[1])>>deta76X","","E same");

	*/

	// Monte Carlo
	auto fmc76X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_lowMTrigger_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_10_03_2016.root");
	auto fmc74X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_74X_lowMTrigger_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_10_03_2016.root");
	auto tmc76X = (TTree*) fmc76X->Get("MssmHbb");
	auto tmc74X = (TTree*) fmc74X->Get("MssmHbb");

	const int nBins = 200;

	auto cmcEffBTag = new TCanvas("cmcEffBTag","Eff MC Btag",1000,800);

	auto h76BEffNum = new TH1D("h76BEffNum","Flavour composition N_{real b} / N_{all};BTag_{CSVv2}",nBins,0,1);
	style76(h76BEffNum);
	auto h76BEffDen = (TH1D*) h76BEffNum->Clone("h76BEffDen");
	auto h74BEffNum = new TH1D("h74BEffNum","",nBins,0,1);
	style74(h74BEffNum);
	auto h74BEffDen = (TH1D*) h74BEffNum->Clone("h74BEffDen");

	auto h76BMisNum = new TH1D("h76BMisNum","Flavour composition  N_{udsg}/N_{all};BTag_{CSVv2}",nBins,0,1);
	style76(h76BMisNum);
	auto h76BMisDen = (TH1D*) h76BMisNum->Clone("h76BMisDen");
	auto h74BMisNum = new TH1D("h74BMisNum","",nBins,0,1);
	style74(h74BMisNum);
	auto h74BMisDen = (TH1D*) h74BMisNum->Clone("h74BMisDen");


	tmc76X->Draw("LeadBTag[0]>>h76BEffNum","LeadFl[0] == 5","E");
	tmc76X->Draw("LeadBTag[0]>>h76BEffDen","","E");
	h76BEffNum->Divide(h76BEffNum,h76BEffDen,1,1,"b");

	tmc74X->Draw("LeadBTag[0]>>h74BEffNum","LeadFl[0] == 5","E");
	tmc74X->Draw("LeadBTag[0]>>h74BEffDen","","E");
	h74BEffNum->Divide(h74BEffNum,h74BEffDen,1,1,"b");

//	h76BEffNum->SetMarkerStyle(20);
//	h74BEffNum->SetMarkerStyle(21);
	h76BEffNum->Draw();
	h74BEffNum->Draw("same");

	auto leg = new TLegend(0.8,0.8,0.93,0.93);
	leg->AddEntry(h76BEffNum,"76X","p");
	leg->AddEntry(h74BEffNum,"74X","p");
	leg->Draw();


	auto cmcMisBTag = new TCanvas("cmcMisBTag","Mis MC Btag",1000,800);

	tmc76X->Draw("LeadBTag[0]>>h76BMisNum","LeadFl[0] == 0","E");
	tmc76X->Draw("LeadBTag[0]>>h76BMisDen","","E");
	h76BMisNum->Divide(h76BMisNum,h76BMisDen,1,1,"b");

	tmc74X->Draw("LeadBTag[0]>>h74BMisNum","LeadFl[0] == 0","E");
	tmc74X->Draw("LeadBTag[0]>>h74BMisDen","","E");
	h74BMisNum->Divide(h74BMisNum,h74BMisDen,1,1,"b");

	h76BMisNum->SetMarkerStyle(20);
	h74BMisNum->SetMarkerStyle(21);
	h76BMisNum->Draw();
	h74BMisNum->Draw("same");
	leg->Draw();


	// Monte Carlo
	auto fmc76X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_lowMTrigger_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_10_03_2016.root");
	auto fmc74X = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_74X_lowMTrigger_QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8_10_03_2016.root");
	auto tmc76X = (TTree*) fmc76X->Get("MssmHbb");
	auto tmc74X = (TTree*) fmc74X->Get("MssmHbb");

	auto cmcPerfBTag = new TCanvas("cmcPerfBTag","Eff MC Btag",1000,800);

	auto hCMVA_EffNum = new TH1D("hCMVA_EffNum","",nBins,0,1);
	auto hCMVA_EffDen = new TH1D("hCMVA_EffNum","",nBins,0,1);
	auto hCMVA_MisNum = new TH1D("hCMVA_EffNum","",nBins,0,1);
	auto hCMVA_MisDen = new TH1D("hCMVA_EffNum","",nBins,0,1);
	double cMVAEff[nBins], cMVAMis[nBins];


	double cut;
	char ccutEff[200], ccutMis[200];
	double Effval76[nBins], Effval74[nBins], ErrEffval76[nBins], ErrEffval74[nBins];
	double Misval76[nBins], Misval74[nBins], ErrMisval76[nBins], ErrMisval74[nBins];
	for(int i = 0; i <= nBins; ++i){
		cut = 0. + (0.999-0.)/nBins*i;
		sprintf(ccutEff,"LeadFl[0] == 5 && LeadBTag[0] > %f",cut);
		sprintf(ccutMis,"LeadFl[0] == 0 && LeadBTag[0] > %f",cut);

		tmc76X->Draw("LeadBTag[0]>>h76BEffNum",ccutEff,"E");
		tmc76X->Draw("LeadBTag[0]>>h76BEffDen","LeadFl[0] == 5","E");
		Effval76[i] = h76BEffNum->Integral() / h76BEffDen->Integral();

		tmc74X->Draw("LeadBTag[0]>>h74BEffNum",ccutEff,"E");
		tmc74X->Draw("LeadBTag[0]>>h74BEffDen","LeadFl[0] == 5","E");
		Effval74[i] = h74BEffNum->Integral() / h74BEffDen->Integral();

		tmc76X->Draw("LeadBTag[0]>>h76BMisNum",ccutMis,"E");
		tmc76X->Draw("LeadBTag[0]>>h76BMisDen","LeadFl[0] == 0","E");
		Misval76[i]	= h76BMisNum->Integral() / h76BMisDen->Integral();

		tmc74X->Draw("LeadBTag[0]>>h74BMisNum",ccutMis,"E");
		tmc74X->Draw("LeadBTag[0]>>h74BMisDen","LeadFl[0] == 0","E");
		Misval74[i]	= h74BMisNum->Integral() / h74BMisDen->Integral();

		tmc76X->Draw("LeadBTag[0]>>hCMVA_EffNum",ccutEff,"E");
		tmc76X->Draw("LeadBTag[0]>>hCMVA_EffDen","LeadFl[0] == 5","E");
		cMVAEff[i] = h76BEffNum->Integral() / h76BEffDen->Integral();

		tmc76X->Draw("LeadBTag[0]>>hCMVA_MisNum",ccutMis,"E");
		tmc76X->Draw("LeadBTag[0]>>hCMVA_MisDen","LeadFl[0] == 0","E");
		cMVAMis[i]	= h76BMisNum->Integral() / h76BMisDen->Integral();

		std::cout<<"Results, step "<<i<<std::endl;
		std::cout<<"cMVA BTag> "<<cut<<" Efficiency = "<<cMVAEff[i]<<" Mis = "<<cMVAMis[i]<<std::endl;
		std::cout<<"76X: BTag> "<<cut<<" Efficiency = "<<Effval76[i]<<" Mis = "<<Misval76[i]<<std::endl;
		std::cout<<"74X: BTag> "<<cut<<" Efficiency = "<<Effval74[i]<<" Mis = "<<Misval74[i]<<std::endl;
		std::cout<<"\n"<<std::endl;

	}

	auto h76BTagPerfomance = new TGraphErrors(nBins,Effval76,Misval76,ErrEffval76,ErrMisval76);
	h76BTagPerfomance->SetTitle(";b-jet efficiency;udsg-jet misid. prob.");
	h76BTagPerfomance->SetMarkerStyle(20);
	auto h74BTagPerfomance = new TGraphErrors(nBins,Effval74,Misval74,ErrEffval74,ErrMisval74);
	h74BTagPerfomance->SetMarkerStyle(21);
	h74BTagPerfomance->SetMarkerColor(2);

	h76BTagPerfomance->Draw("AP");
	h74BTagPerfomance->Draw("P same");
	leg->Draw();
*/



	return 0;
}
