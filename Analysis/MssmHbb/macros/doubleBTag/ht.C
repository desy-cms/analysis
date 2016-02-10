#include <iostream>

using namespace std;

int ht(){

	TFile *f1 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT100200output_105_02_2016.root");
	TFile *f2 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT200300output_107_02_2016.root");
	TFile *f3 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT300500output_107_02_2016.root");
	TFile *f4 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT500700output_107_02_2016.root");
	TFile *f5 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT7001000output_107_02_2016.root");
	TFile *f6 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT10001500output_107_02_2016.root");
	TFile *f7 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT15002000output_107_02_2016.root");
	TFile *f8 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT2000Infoutput_107_02_2016.root");

	TTree *t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8;
	f1->GetObject("MssmHbb",t1);
	f2->GetObject("MssmHbb",t2);
	f3->GetObject("MssmHbb",t3);
	f4->GetObject("MssmHbb",t4);
	f5->GetObject("MssmHbb",t5);
	f6->GetObject("MssmHbb",t6);
	f7->GetObject("MssmHbb",t7);
	f8->GetObject("MssmHbb",t8);

	TH1F *h1 = new TH1F("h1","",40,200.,2600);
	h1->SetMarkerColor(1);
	h1->SetMarkerStyle(20);
	TH1F *h2 = new TH1F("h2","",40,200.,2600);
	h2->SetMarkerColor(2);
	h2->SetMarkerStyle(21);
	TH1F *h3 = new TH1F("h3","",40,200.,2600);
	h3->SetMarkerColor(3);
	h3->SetMarkerStyle(22);
	TH1F *h4 = new TH1F("h4","",40,200.,2600);
	h4->SetMarkerColor(4);
	h4->SetMarkerStyle(23);
	TH1F *h5 = new TH1F("h5","",40,200.,2600);
	h5->SetMarkerColor(5);
	h5->SetMarkerStyle(24);
	TH1F *h6 = new TH1F("h6","",40,200.,2600);
	h6->SetMarkerColor(6);
	h6->SetMarkerStyle(25);
	TH1F *h7 = new TH1F("h7","",40,200.,2600);
	h7->SetMarkerColor(7);
	h7->SetMarkerStyle(26);
	TH1F *h8 = new TH1F("h8","",40,200.,2600);
	h8->SetMarkerColor(8);
	h8->SetMarkerStyle(27);


	h1->GetYaxis()->SetRangeUser(0.01,1e+06);
	t1->Draw("Ht>>h1","LumiWeight");
	t2->Draw("Ht>>h2","LumiWeight","same");
	t3->Draw("Ht>>h3","LumiWeight","same");
	t4->Draw("Ht>>h4","LumiWeight","same");
	t5->Draw("Ht>>h5","LumiWeight","same");
	t6->Draw("Ht>>h6","LumiWeight","same");
	t7->Draw("Ht>>h7","LumiWeight","same");
	t8->Draw("Ht>>h8","LumiWeight","same");

	TLegend *leg = new TLegend(0.5,0.7,0.8,0.9);
	leg->AddEntry(h1,"100-200","p");
	leg->AddEntry(h2,"200-300","p");
	leg->AddEntry(h3,"300-500","p");
	leg->AddEntry(h4,"500-700","p");
	leg->AddEntry(h5,"700-1000","p");
	leg->AddEntry(h6,"1000-1500","p");
	leg->AddEntry(h7,"1500-2000","p");
	leg->AddEntry(h8,"2000-Inf","p");
	leg->Draw();


	return 0;
}
