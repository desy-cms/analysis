
#include <iostream>

using namespace std;

int signalEfficiency(){
	gROOT->Reset();
	TH1::SetDefaultSumw2();

	TFile * fSignal_300 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M300_output_104_02_2016.root");
	TFile * fSignal_600 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M600_output_104_02_2016.root");
	TFile * fSignal_1100 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M1100_output_104_02_2016.root");

	TFile * fBg = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelectionMCoutput_101_02_2016.root");

	TTree *BgTree, *S300Tree, *S600Tree, *S1100Tree;
	fSignal_300->GetObject("MssmHbb",S300Tree);
	fSignal_600->GetObject("MssmHbb",S600Tree);
	fSignal_1100->GetObject("MssmHbb",S1100Tree);
	fBg -> GetObject("MssmHbb",BgTree);

	//...................M-300.....................

	TH1F *h300 = new TH1F("h300","M-300",1,0,12);
	TH1F *h300Tot = new TH1F("h300Tot","",1,0,10e7);
	TH1F *h300Lumi = new TH1F("h300Lumi","",1,0,10e7);
	S300Tree->Draw("Njets>>h300","LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S300Tree->Draw("TotalNumberOfEvents>>h300Tot");
	S300Tree->Draw("LumiWeight>>h300Lumi");
	h300->Scale(1./ (h300Tot->GetMean() * h300Lumi->GetMean()));
	h300->Draw();

	cout<<"M-300: "<<h300->GetMaximum()<<endl;

	S300Tree->Draw("Njets>>h300","(ObjM12> 240 &&ObjM12 < 360)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	h300->Scale(1./ (h300Tot->GetMean() * h300Lumi->GetMean()));
	cout<<"M-240:360 : "<<h300->GetMaximum()<<endl;


	//.................M-600......................

	TH1F *h600 = new TH1F("h600","M-300",1,0,12);
	TH1F *h600Tot = new TH1F("h600Tot","",1,0,10e7);
	TH1F *h600Lumi = new TH1F("h600Lumi","",1,0,10e7);
	S600Tree->Draw("Njets>>h600","LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S600Tree->Draw("TotalNumberOfEvents>>h600Tot");
	S600Tree->Draw("LumiWeight>>h600Lumi");
	h600->Scale(1./ (h600Tot->GetMean() * h600Lumi->GetMean()));
	h600->Draw();

	cout<<"M-600: "<<h600->GetMaximum()<<endl;

	S600Tree->Draw("Njets>>h600","(ObjM12> 480 &&ObjM12 < 720)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	h600->Scale(1./ (h600Tot->GetMean() * h600Lumi->GetMean()));
	cout<<"M-480:720 : "<<h600->GetMaximum()<<endl;

	//................M-1100..................

	TH1F *h1100 = new TH1F("h1100","M-300",1,0,12);
	TH1F *h1100Tot = new TH1F("h1100Tot","",1,0,10e7);
	TH1F *h1100Lumi = new TH1F("h1100Lumi","",1,0,10e7);
	S1100Tree->Draw("Njets>>h1100","LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S1100Tree->Draw("TotalNumberOfEvents>>h1100Tot");
	S1100Tree->Draw("LumiWeight>>h1100Lumi");
	h1100->Scale(1./ (h1100Tot->GetMean() * h1100Lumi->GetMean()));
	h1100->Draw();

	cout<<"M-1100: "<<h1100->GetMaximum()<<endl;

	S1100Tree->Draw("Njets>>h1100","(ObjM12> 880 &&ObjM12 < 1320)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	h1100->Scale(1./ (h1100Tot->GetMean() * h1100Lumi->GetMean()));
	cout<<"M-880:1320 : "<<h1100->GetMaximum()<<endl;

	return 0;
}
