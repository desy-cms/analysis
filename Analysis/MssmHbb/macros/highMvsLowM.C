#include "TH1.h"
#include "TH1D.h"

int highMvsLowM(){

	auto file = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_NoTriggerMatching_test_lowMTrigger_samples_04_03_2016.root");
	auto tree = (TTree*) file->Get("MssmHbb");

	TH1::SetDefaultSumw2();

	auto highMPt2 = new TH1D("highMPt2","PT2",50,100,500);
	highMPt2->SetStats(kFALSE);
	auto lowMPt2 = new TH1D("lowMPt2","",50,100,500);

	auto can2 = new TCanvas("can2","");
	tree->Draw("highMLeadPt[1]>>highMPt2","highMLeadPt[1]>0","");
	tree->Draw("LeadPt[1]>>lowMPt2","","E same");

	highMPt2->Divide(highMPt2,lowMPt2,1,1,"b");
	highMPt2->Draw();

	auto highMPt1 = new TH1D("highMPt1","PT1",50,100,500);
	highMPt1->SetStats(kFALSE);
	auto lowMPt1 = new TH1D("lowMPt1","",50,100,500);

	auto can1 = new TCanvas("can1","Pt1");
	tree->Draw("highMLeadPt[0]>>highMPt1","highMLeadPt[0]>0","");
	tree->Draw("LeadPt[0]>>lowMPt1","","E same");

	highMPt1->Divide(highMPt1,lowMPt1,1,1,"b");
	highMPt1->Draw();

	auto highMBTag1 = new TH1F("highMBTag1","BTAG1",50,0.9,1);
	highMBTag1->SetStats(kFALSE);
	auto lowMBTag1 = new TH1F("lowMBTag1","",50,0.9,1);

	auto can3 = new TCanvas("can3","BTag1");
	tree->Draw("highMLeadBTag[0]>>highMBTag1","highMLeadBTag[0]>0","");
	tree->Draw("LeadBTag[0]>>lowMBTag1","","E same");

	highMBTag1->Divide(highMBTag1,lowMBTag1,1,1,"b");
	highMBTag1->Draw();

	auto highdEta = new TH1F("highdEta","dETA",50,0.,2);
	highdEta->SetStats(kFALSE);
	auto lowdEta = new TH1F("lowdEta","",50,0,2);

	auto can4 = new TCanvas("can4","dETA");
	tree->Draw("abs(highMLeadEta[0]-highMLeadEta[1])>>highdEta","highMLeadPt[0]>0 && highMLeadPt[1]>0","");
	tree->Draw("abs(LeadEta[0]-LeadEta[1])>>lowdEta","","E same");

	highdEta->Divide(highdEta,lowdEta,1,1,"b");
	highdEta->Draw();




	return 0;
}
