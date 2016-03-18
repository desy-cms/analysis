#include <iostream>
#include "stdio.h"

#include "TH1.h"
#include "TH1F.h"
#include "TCut.h"

/*
 * This is a simple example of how to use ntuples,
 * how to access variables, how to make additional cuts
 * without re-runing ntuples production.
 */

void SetHistoStyle(TH1 *h);

void simpleHistoBuilderExample(){

	//For proper errors treatment
	TH1::SetDefaultSumw2();

	//Add input file
	auto file = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_lowMTrigger_Pythia8_16_03_2016.root");
	//Add input TTree from the file
	TTree *tree;
	//Assign MssmHbb tree in the input TFile to the TTree that was created
	file->GetObject("MssmHbb",tree);

	//Create TH1 object
	const int nBins = 100;
	auto canva = new TCanvas();
	auto LeadingJetPt = new TH1F("LeadingJetPt","Leading Jet p_{T};p_{T}, [GeV];Entries",nBins,90,1200);
	SetHistoStyle(LeadingJetPt);	//basic cosmetics

	//Fill histogram from the tree
	gPad->SetLogy();
	tree->Draw("LeadPt[0]>>LeadingJetPt","","E");

	/*
	 * With trees you are able to cut without re-running sample
	 * production. It's should be done in the next way
	 */

	auto canva2 = new TCanvas();
	auto LeadingJetPtCut = (TH1F*) LeadingJetPt->Clone("LeadingJetPtCut");
	LeadingJetPtCut->SetTitle("Leading Jet p_{T}, BTag CSVv2 > 0.97");

	//you can cut directly wit char expression:
	// * means reweighting
	gPad->SetLogy();
	tree->Draw("LeadPt[0]>>LeadingJetPtCut","(LeadBTag[0] > 0.97)*LumiWeight","E");

	//or using TCut
	auto canva3 = new TCanvas();
	TCut pt_cut = "LeadPt[0] > 140 && LeadBTag[0] > 0.97";
	TCut eta_cut = "abs(LeadEta[0]) < 1.5";
	TCut lumi_weight = "LumiWeight";

	auto LeadingJetPtCut2 = (TH1F*) LeadingJetPt->Clone("LeadingJetPtCut2");
	LeadingJetPtCut2->SetTitle("Leading Jet p_{T}, All cuts");
	gPad->SetLogy();
	tree->Draw("LeadPt[0]>>LeadingJetPtCut2",(pt_cut+eta_cut)*lumi_weight,"E");

}

void SetHistoStyle(TH1 *h){

	if(!h){
		std::cerr<<"Error null_ptr were provided"<<std::endl;
		exit(0);
	}
	h->SetMarkerStyle(20);
	h->SetMarkerColor(1);
	h->SetMarkerSize(1.02);
}
