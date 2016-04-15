#include <boost/lexical_cast.hpp>

using namespace std;


int test(){	
	gStyle->SetOptStat(0);

	TFile * fBg = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_lowMTrigger_Pythia8_16_03_2016.root");
	TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_nodEta_lowMTrigger_Run2015D-16Dec2015-v1_08_04_2016.root");
	TFile * fSignal = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_New3dJetSelection_lowMTrigger_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8_14_04_2016.root");

	TTree *dataTree, *mcBg, *mcSignal;
	fSignal -> GetObject("MssmHbb",mcSignal);
	fBg -> GetObject("MssmHbb",mcBg);
	fData -> GetObject("MssmHbb",dataTree);

	TCut centralWeight = "LumiWeight*TwoDPtWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[0]";




	auto can1 = new TCanvas("can1","can1",1200,1200);
	can1->Divide(1,2);
	can1->cd(1);
	auto hLead3dJet_pt 	= new TH1D("hLead3dJet_pt","",100,0.,600.);
	hLead3dJet_pt->SetMarkerColor(2);
	hLead3dJet_pt->SetMarkerStyle(20);
	auto h3dJet_pt		= new TH1D("h3dJet_pt","",100,0.,600.);
	h3dJet_pt->SetMarkerStyle(21);

	mcSignal->Draw("New3dJet_pt>>h3dJet_pt","New3dJet_pt>0","E");
	mcSignal->Draw("LeadPt[2]>>hLead3dJet_pt","LeadPt[2]>0","E same");


	TLegend *leg = new TLegend(0.5,0.5,0.8,0.8);
	std::string old_title = "Leading 3d jet, Events: " + boost::lexical_cast<std::string>(hLead3dJet_pt->GetEntries());
	std::string new_title = "New 3d Main jet, Events: " + boost::lexical_cast<std::string>(h3dJet_pt->GetEntries());

	leg->AddEntry(hLead3dJet_pt,old_title.c_str(),"p");
	leg->AddEntry(h3dJet_pt,new_title.c_str(),"p");
	leg->Draw();

	can1->cd(2);
	auto copy_h3dJet_pt = (TH1D*)h3dJet_pt->Clone("copy_h3dJet_pt");
	copy_h3dJet_pt->Scale(hLead3dJet_pt->Integral()/copy_h3dJet_pt->Integral());
	copy_h3dJet_pt->Draw("E");
	hLead3dJet_pt->Draw("E same");

	auto can2 = new TCanvas("can2","can2",1200,1200);
	can2->Divide(1,2);
	can2->cd(1);
	auto hLead3dJet_eta 	= new TH1D("hLead3dJet_eta","",100,-2.2,2.2);
	hLead3dJet_eta->SetMarkerColor(2);
	hLead3dJet_eta->SetMarkerStyle(20);
	auto h3dJet_eta		= new TH1D("h3dJet_eta","",100,-2.2,2.2);
	h3dJet_eta->SetMarkerStyle(21);

	mcSignal->Draw("New3dJet_eta>>h3dJet_eta","","E");
	mcSignal->Draw("LeadEta[2]>>hLead3dJet_eta","","E same");

	leg->Draw();

	can2->cd(2);
	auto copy_h3dJet_eta = (TH1D*)h3dJet_eta->Clone("copy_h3dJet_eta");
	copy_h3dJet_eta->Scale(hLead3dJet_eta->Integral()/copy_h3dJet_eta->Integral());
	copy_h3dJet_eta->Draw("E");
	hLead3dJet_eta->Draw("E same");

	auto can3 = new TCanvas("can3","can3",1200,1200);
	can3->Divide(1,2);
	can3->cd(1);
	auto hLead3dJet_btag 	= new TH1D("hLead3dJet_btag","",100,0.8,1);
	hLead3dJet_btag->SetMarkerColor(2);
	hLead3dJet_btag->SetMarkerStyle(20);
	auto h3dJet_btag		= new TH1D("h3dJet_btag","",100,0.8,1);
	h3dJet_btag->SetMarkerStyle(21);

	mcSignal->Draw("New3dJet_btag>>h3dJet_btag","","E");
	mcSignal->Draw("LeadBTag[2]>>hLead3dJet_btag","","E same");

	leg->Draw();

	can3->cd(2);
	auto copy_h3dJet_btag = (TH1D*)h3dJet_btag->Clone("copy_h3dJet_btag");
	copy_h3dJet_btag->Scale(hLead3dJet_btag->Integral()/copy_h3dJet_btag->Integral());
	copy_h3dJet_btag->Draw("E");
	hLead3dJet_btag->Draw("E same");

	auto can4 = new TCanvas("can4","can4",1200,1200);
	auto h3dJet_index = new TH1I("h3dJet_index","Index of 3d jet",10,0,10);
	mcSignal->Draw("New3dJet_index>>h3dJet_index","New3dJet_pt>0");
	auto text = new TPaveText(6,8e+03,10,1e+04);
	std::string additional_events = "Advantage: " +  boost::lexical_cast<std::string>( (h3dJet_index->GetEntries() - h3dJet_index->GetBinContent(4))/h3dJet_index->GetBinContent(4) * 100 ) + " %";
	text->AddText(additional_events.c_str());
	text->Draw();
	cout<<h3dJet_index->GetEntries()<<" "<<h3dJet_index->GetBinContent(4)<<" "<<(h3dJet_index->GetEntries() - h3dJet_index->GetBinContent(4))/h3dJet_index->GetBinContent(4) * 100<<endl;




	return 0;
}
