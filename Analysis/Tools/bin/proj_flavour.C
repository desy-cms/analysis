#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TCanvas.h"

void proj_flavour()
{
TFile *f1[13], *f2[13]; 

//f1 = new TFile("histograms_wo_weight_signal.root", "READ");
//f2 = new TFile("histograms_with_weight_signal.root","READ");
f1[0] = new TFile("histograms_wo_weight_50_to_80.root" , "READ");
f2[0] = new TFile("histograms_with_weight_50_to_80.root", "READ");
f1[1] = new TFile("histograms_wo_weight_80_to_120.root" , "READ");
f2[1] = new TFile("histograms_with_weight_80_to_120.root", "READ");
f1[2] = new TFile("histograms_wo_weight_120_to_170.root" , "READ");
f2[2] = new TFile("histograms_with_weight_120_to_170.root", "READ");
f1[3] = new TFile("histograms_wo_weight_170_to_300.root" , "READ");
f2[3] = new TFile("histograms_with_weight_170_to_300.root", "READ");
f1[4] = new TFile("histograms_wo_weight_300_to_470.root" , "READ");
f2[4] = new TFile("histograms_with_weight_300_to_470.root", "READ");
f1[5] = new TFile("histograms_wo_weight_470_to_600.root" , "READ");
f2[5] = new TFile("histograms_with_weight_470_to_600.root", "READ");
f1[6] = new TFile("histograms_wo_weight_600_to_800.root" , "READ");
f2[6] = new TFile("histograms_with_weight_600_to_800.root", "READ");
f1[7] = new TFile("histograms_wo_weight_800_to_1000.root" , "READ");
f2[7] = new TFile("histograms_with_weight_800_to_1000.root", "READ");
f1[8] = new TFile("histograms_wo_weight_1000_to_1400.root" , "READ");
f2[8] = new TFile("histograms_with_weight_1000_to_1400.root", "READ");
f1[9] = new TFile("histograms_wo_weight_1400_to_1800.root" , "READ");
f2[9] = new TFile("histograms_with_weight_1400_to_1800.root", "READ");
f1[10] = new TFile("histograms_wo_weight_1800_to_2400.root" , "READ");
f2[10] = new TFile("histograms_with_weight_1800_to_2400.root", "READ");
f1[11] = new TFile("histograms_wo_weight_2400_to_3200.root" , "READ");
f2[11] = new TFile("histograms_with_weight_2400_to_3200.root", "READ");
f1[12] = new TFile("histograms_wo_weight_3200_to_inf.root" , "READ");
f2[12] = new TFile("histograms_with_weight_3200_to_inf.root", "READ");

 TH2F *hist1b = new TH2F();
 TH2F *hist1c = new TH2F();
 TH2F *hist1l = new TH2F();
 TH2F *hist1bb = new TH2F();
 TH2F *hist1cc = new TH2F();
 //TH2F *hist2 = new TH2F();
 TH2F *hist3b = new TH2F();
 TH2F *hist3c = new TH2F();
 TH2F *hist3l = new TH2F();
 TH2F *hist3bb = new TH2F();
 TH2F *hist3cc = new TH2F();
 //TH2F *hist4 = new TH2F();
 TH1F *hist5 = new TH1F();
 TH1F *hist6 = new TH1F();


 for(int i = 0; i < 13 ; ++i)
 {
 if ( i == 0)
 {
 hist1b = (TH2F*)f1[i]->Get("hist1b");
 hist1c = (TH2F*)f1[i]->Get("hist1_c");
 hist1l = (TH2F*)f1[i]->Get("hist1_l");
 hist1bb = (TH2F*)f1[i]->Get("hist1_bb");
 hist1cc = (TH2F*)f1[i]->Get("hist1_cc");

 //hist2 = (TH2F*)f1[i]->Get("hist2");
 //hist5 = (TH1F*)f1[i]->Get("hist5");
 hist3b = (TH2F*)f2[i]->Get("hist3b");
 hist3c = (TH2F*)f2[i]->Get("hist3_c");
 hist3l = (TH2F*)f2[i]->Get("hist3_l");
 hist3bb = (TH2F*)f2[i]->Get("hist3_bb");
 hist3cc = (TH2F*)f2[i]->Get("hist3_cc");
 //hist4 = (TH2F*)f2[i]->Get("hist4");
 //hist6 = (TH1F*)f2[i]->Get("hist6");
 }
// 
 else
 {
hist1b -> Add((TH2F*)f1[i]->Get("hist1b"));
hist1c -> Add((TH2F*)f1[i]->Get("hist1_c"));
hist1l -> Add((TH2F*)f1[i]->Get("hist1_l"));
hist1bb -> Add((TH2F*)f1[i]->Get("hist1_bb"));
hist1cc -> Add((TH2F*)f1[i]->Get("hist1_cc"));
//hist2 -> Add((TH2F*)f1[i]->Get("hist2"));
//hist5 -> Add((TH1F*)f1[i]->Get("hist5"));
hist3b -> Add((TH2F*)f2[i]->Get("hist3b"));
hist3c -> Add((TH2F*)f2[i]->Get("hist3_c"));
hist3l -> Add((TH2F*)f2[i]->Get("hist3_l"));
hist3bb -> Add((TH2F*)f2[i]->Get("hist3_bb"));
hist3cc -> Add((TH2F*)f2[i]->Get("hist3_cc"));
//hist4 -> Add((TH2F*)f2[i]->Get("hist4"));
//hist6 -> Add((TH1F*)f2[i]->Get("hist6"));
}
}



TH1D *pt1b = hist1b->ProjectionX();
TH1D *eta1b = hist1b->ProjectionY();
TH1D *pt1c = hist1c->ProjectionX();
TH1D *eta1c = hist1c->ProjectionY();
TH1D *pt1l = hist1l->ProjectionX();
TH1D *eta1l = hist1l->ProjectionY();
TH1D *pt1bb = hist1bb->ProjectionX();
TH1D *eta1bb = hist1bb->ProjectionY();
TH1D *pt1cc = hist1cc->ProjectionX();
TH1D *eta1cc = hist1cc->ProjectionY();
 //TH1D *pt2 = hist2->ProjectionX();
 //TH1D *eta2 = hist2->ProjectionY();
TH1D *pt3b = hist3b->ProjectionX();
TH1D *eta3b = hist3b->ProjectionY();
TH1D *pt3c = hist3c->ProjectionX();
TH1D *eta3c = hist3c->ProjectionY();
TH1D *pt3l = hist3l->ProjectionX();
TH1D *eta3l = hist3l->ProjectionY();
TH1D *pt3bb = hist3bb->ProjectionX();
TH1D *eta3bb = hist3bb->ProjectionY();
TH1D *pt3cc = hist3cc->ProjectionX();
TH1D *eta3cc = hist3cc->ProjectionY();
 //TH1D *pt4 = hist4->ProjectionX();
 //TH1D *eta4 = hist4->ProjectionY();

//jet1_pt
	TCanvas *c1b = new TCanvas("jet_pt_flav_b","jet_pt_flav_b");
	TPad *pad1b = new TPad("pad1b","pad1b",0,0.3,1,1.0);
	pad1b->SetBottomMargin(0.1);
	pad1b->SetGridx();
	pad1b->Draw();
	pad1b->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1b->SetStats(0);
	pt1b->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1b->Draw();
	pt3b->SetLineColor(kRed);
	pt3b->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3b->Draw("same");

        TLegend *leg1b = new TLegend(0.6,0.6,0.9,0.5);
        leg1b->AddEntry(pt1b, "unweighted");
        leg1b->AddEntry(pt3b, "weighted");
        leg1b->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1b->cd();
	TPad *pad2b = new TPad("pad2b","pad2b",0,0.05,1,0.3);
	pad2b->SetTopMargin(0.1);
	pad2b->SetBottomMargin(0.1);
	pad2b->SetGridx();
        pad2b->SetGridy();
	pad2b->Draw();
	pad2b->cd();
	
	TH1D *pt1_new_b = (TH1D*)pt3b->Clone("pt1b");
	pt1_new_b->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new_b->Sumw2();
	pt1_new_b->SetStats(0);
	pt1_new_b->Divide(pt1b);
	pt1_new_b->SetMarkerStyle(21);
	pt1_new_b->Draw("ep");
	
	pt1_new_b->SetTitle("");
        pt1_new_b->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new_b->GetYaxis()->SetTitleSize(10);
        pt1_new_b->GetYaxis()->SetTitleFont(43);
        pt1_new_b->GetYaxis()->SetTitleOffset(1.55);
        pt1_new_b->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new_b->GetYaxis()->SetNdivisions(505);
	pt1_new_b->GetYaxis()->SetLabelFont(43);
	pt1_new_b->GetYaxis()->SetLabelSize(15);
	pt1_new_b->GetXaxis()->SetLabelFont(43);
	pt1_new_b->GetXaxis()->SetLabelSize(15);

        pt1b->GetXaxis()->SetTitle("pT(GeV)");
        pt1b->GetYaxis()->SetTitle("Entries");
	
	c1b->Print("jet_pt_flav_b.pdf");

//jet1_pt
	TCanvas *c1c = new TCanvas("jet_pt_flav_c","jet_pt_flav_c");
	TPad *pad1c = new TPad("pad1c","pad1c",0,0.3,1,1.0);
	pad1c->SetBottomMargin(0.1);
	pad1c->SetGridx();
	pad1c->Draw();
	pad1c->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1c->SetStats(0);
	pt1c->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1c->Draw();
	pt3c->SetLineColor(kRed);
	pt3c->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3c->Draw("same");

        TLegend *leg1c = new TLegend(0.6,0.6,0.9,0.5);
        leg1c->AddEntry(pt1c, "unweighted");
        leg1c->AddEntry(pt3c, "weighted");
        leg1c->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1c->cd();
	TPad *pad2c = new TPad("pad2c","pad2c",0,0.05,1,0.3);
	pad2c->SetTopMargin(0.1);
	pad2c->SetBottomMargin(0.1);
	pad2c->SetGridx();
        pad2c->SetGridy();
	pad2c->Draw();
	pad2c->cd();
	
	TH1D *pt1_new_c = (TH1D*)pt3c->Clone("pt1c");
	pt1_new_c->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new_c->Sumw2();
	pt1_new_c->SetStats(0);
	pt1_new_c->Divide(pt1c);
	pt1_new_c->SetMarkerStyle(21);
	pt1_new_c->Draw("ep");
	
	pt1_new_c->SetTitle("");
        pt1_new_c->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new_c->GetYaxis()->SetTitleSize(10);
        pt1_new_c->GetYaxis()->SetTitleFont(43);
        pt1_new_c->GetYaxis()->SetTitleOffset(1.55);
        pt1_new_c->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new_c->GetYaxis()->SetNdivisions(505);
	pt1_new_c->GetYaxis()->SetLabelFont(43);
	pt1_new_c->GetYaxis()->SetLabelSize(15);
	pt1_new_c->GetXaxis()->SetLabelFont(43);
	pt1_new_c->GetXaxis()->SetLabelSize(15);

        pt1c->GetXaxis()->SetTitle("pT(GeV)");
        pt1c->GetYaxis()->SetTitle("Entries");
	
	c1c->Print("jet_pt_flav_c.pdf");


//jet1_pt
	TCanvas *c1l = new TCanvas("jet_pt_flav_l","jet_pt_flav_l");
	TPad *pad1l = new TPad("pad1l","pad1l",0,0.3,1,1.0);
	pad1l->SetBottomMargin(0.1);
	pad1l->SetGridx();
	pad1l->Draw();
	pad1l->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1l->SetStats(0);
	pt1l->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1l->Draw();
	pt3l->SetLineColor(kRed);
	pt3l->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3l->Draw("same");

        TLegend *leg1l = new TLegend(0.6,0.6,0.9,0.5);
        leg1l->AddEntry(pt1l, "unweighted");
        leg1l->AddEntry(pt3l, "weighted");
        leg1l->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1l->cd();
	TPad *pad2l = new TPad("pad2l","pad2l",0,0.05,1,0.3);
	pad2l->SetTopMargin(0.1);
	pad2l->SetBottomMargin(0.1);
	pad2l->SetGridx();
        pad2l->SetGridy();
	pad2l->Draw();
	pad2l->cd();
	
	TH1D *pt1_new_l = (TH1D*)pt3l->Clone("pt1l");
	pt1_new_l->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new_l->Sumw2();
	pt1_new_l->SetStats(0);
	pt1_new_l->Divide(pt1l);
	pt1_new_l->SetMarkerStyle(21);
	pt1_new_l->Draw("ep");
	
	pt1_new_l->SetTitle("");
        pt1_new_l->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new_l->GetYaxis()->SetTitleSize(10);
        pt1_new_l->GetYaxis()->SetTitleFont(43);
        pt1_new_l->GetYaxis()->SetTitleOffset(1.55);
        pt1_new_l->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new_l->GetYaxis()->SetNdivisions(505);
	pt1_new_l->GetYaxis()->SetLabelFont(43);
	pt1_new_l->GetYaxis()->SetLabelSize(15);
	pt1_new_l->GetXaxis()->SetLabelFont(43);
	pt1_new_l->GetXaxis()->SetLabelSize(15);

        pt1l->GetXaxis()->SetTitle("pT(GeV)");
        pt1l->GetYaxis()->SetTitle("Entries");
	
	c1l->Print("jet_pt_flav_l.pdf");

//jet1_pt
	TCanvas *c1bb = new TCanvas("jet_pt_flav_bb","jet_pt_flav_bb");
	TPad *pad1bb = new TPad("pad1bb","pad1bb",0,0.3,1,1.0);
	pad1bb->SetBottomMargin(0.1);
	pad1bb->SetGridx();
	pad1bb->Draw();
	pad1bb->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1bb->SetStats(0);
	pt1bb->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1bb->Draw();
	pt3bb->SetLineColor(kRed);
	pt3bb->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3bb->Draw("same");

        TLegend *leg1bb = new TLegend(0.6,0.6,0.9,0.5);
        leg1bb->AddEntry(pt1bb, "unweighted");
        leg1bb->AddEntry(pt3bb, "weighted");
        leg1bb->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1bb->cd();
	TPad *pad2bb = new TPad("pad2bb","pad2bb",0,0.05,1,0.3);
	pad2bb->SetTopMargin(0.1);
	pad2bb->SetBottomMargin(0.1);
	pad2bb->SetGridx();
        pad2bb->SetGridy();
	pad2bb->Draw();
	pad2bb->cd();
	
	TH1D *pt1_new_bb = (TH1D*)pt3bb->Clone("pt1bb");
	pt1_new_bb->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new_bb->Sumw2();
	pt1_new_bb->SetStats(0);
	pt1_new_bb->Divide(pt1bb);
	pt1_new_bb->SetMarkerStyle(21);
	pt1_new_bb->Draw("ep");
	
	pt1_new_bb->SetTitle("");
        pt1_new_bb->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new_bb->GetYaxis()->SetTitleSize(10);
        pt1_new_bb->GetYaxis()->SetTitleFont(43);
        pt1_new_bb->GetYaxis()->SetTitleOffset(1.55);
        pt1_new_bb->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new_bb->GetYaxis()->SetNdivisions(505);
	pt1_new_bb->GetYaxis()->SetLabelFont(43);
	pt1_new_bb->GetYaxis()->SetLabelSize(15);
	pt1_new_bb->GetXaxis()->SetLabelFont(43);
	pt1_new_bb->GetXaxis()->SetLabelSize(15);

        pt1bb->GetXaxis()->SetTitle("pT(GeV)");
        pt1bb->GetYaxis()->SetTitle("Entries");
	
	c1bb->Print("jet_pt_flav_bb.pdf");

//jet1_pt
	TCanvas *c1cc = new TCanvas("jet_pt_flav_cc","jet_pt_flav_cc");
	TPad *pad1cc = new TPad("pad1cc","pad1cc",0,0.3,1,1.0);
	pad1cc->SetBottomMargin(0.1);
	pad1cc->SetGridx();
	pad1cc->Draw();
	pad1cc->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1cc->SetStats(0);
	pt1cc->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1cc->Draw();
	pt3cc->SetLineColor(kRed);
	pt3cc->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3cc->Draw("same");

        TLegend *leg1cc = new TLegend(0.6,0.6,0.9,0.5);
        leg1cc->AddEntry(pt1cc, "unweighted");
        leg1cc->AddEntry(pt3cc, "weighted");
        leg1cc->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1cc->cd();
	TPad *pad2cc = new TPad("pad2cc","pad2cc",0,0.05,1,0.3);
	pad2cc->SetTopMargin(0.1);
	pad2cc->SetBottomMargin(0.1);
	pad2cc->SetGridx();
        pad2cc->SetGridy();
	pad2cc->Draw();
	pad2cc->cd();
	
	TH1D *pt1_new_cc = (TH1D*)pt3cc->Clone("pt1cc");
	pt1_new_cc->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new_cc->Sumw2();
	pt1_new_cc->SetStats(0);
	pt1_new_cc->Divide(pt1cc);
	pt1_new_cc->SetMarkerStyle(21);
	pt1_new_cc->Draw("ep");
	
	pt1_new_cc->SetTitle("");
        pt1_new_cc->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new_cc->GetYaxis()->SetTitleSize(10);
        pt1_new_cc->GetYaxis()->SetTitleFont(43);
        pt1_new_cc->GetYaxis()->SetTitleOffset(1.55);
        pt1_new_cc->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new_cc->GetYaxis()->SetNdivisions(505);
	pt1_new_cc->GetYaxis()->SetLabelFont(43);
	pt1_new_cc->GetYaxis()->SetLabelSize(15);
	pt1_new_cc->GetXaxis()->SetLabelFont(43);
	pt1_new_cc->GetXaxis()->SetLabelSize(15);

        pt1cc->GetXaxis()->SetTitle("pT(GeV)");
        pt1cc->GetYaxis()->SetTitle("Entries");
	
	c1cc->Print("jet_pt_flav_cc.pdf");



// jet1_eta
	TCanvas *c3b = new TCanvas("jet_eta_flav_b","jet_eta_flav_b");
	TPad *pad5b = new TPad("pad5b","pad5b",0,0.3,1,1.0);
	pad5b->SetBottomMargin(0.1);
	pad5b->SetGridx();
	pad5b->Draw();
	pad5b->cd();
	eta1b->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1b->Draw();
	eta3b->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3b->Draw("same");
	
        TLegend *leg3b = new TLegend(0.6,0.6,0.9,0.5);
        leg3b->AddEntry(eta1b, "unweighted");
        leg3b->AddEntry(eta3b, "weighted");
        leg3b->Draw();
	//eta1->GetYaxis()->SetLabelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3b->cd();
	TPad *pad6b = new TPad("pad6b","pad6b",0,0.05,1,0.3);
	pad6b->SetTopMargin(0.1);
	pad6b->SetBottomMargin(0.2);
	pad6b->SetGridx();
        pad6b->SetGridy();
	pad6b->Draw();
	pad6b->cd();
	
	TH1D *eta1_new_b = (TH1D*)eta3b->Clone("eta1b");
	eta1_new_b ->SetLineColor(kBlack);
	eta1_new_b ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new_b->Sumw2();
	eta1_new_b ->SetStats(0);
	eta1_new_b ->Divide(eta1b);
	eta1_new_b ->SetMarkerStyle(21);
	eta1_new_b ->Draw("ep");
	
	eta1_new_b ->SetTitle("");
        eta1_new_b->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new_b->GetYaxis()->SetTitleSize(10);
        eta1_new_b->GetYaxis()->SetTitleFont(43);
        eta1_new_b->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new_b->GetYaxis()->SetTitleOffset(1.55);
	eta1_new_b ->GetYaxis()->SetNdivisions(303);
	eta1_new_b ->GetYaxis()->SetLabelFont(43);
	eta1_new_b ->GetYaxis()->SetLabelSize(15);
	eta1_new_b ->GetXaxis()->SetLabelFont(43);
	eta1_new_b ->GetXaxis()->SetLabelSize(15);

        eta1b->GetXaxis()->SetTitle("abs(eta)");
        eta1b->GetYaxis()->SetTitle("Entries");
	
	c3b->Print("jet_eta_flav_b.pdf");

// jet1_eta
	TCanvas *c3c = new TCanvas("jet_eta_flav_c","jet_eta_flav_c");
	TPad *pad5c = new TPad("pad5c","pad5c",0,0.3,1,1.0);
	pad5c->SetBottomMargin(0.1);
	pad5c->SetGridx();
	pad5c->Draw();
	pad5c->cd();
	eta1c->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1c->Draw();
	eta3c->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3c->Draw("same");
	
        TLegend *leg3c = new TLegend(0.6,0.6,0.9,0.5);
        leg3c->AddEntry(eta1c, "unweighted");
        leg3c->AddEntry(eta3c, "weighted");
        leg3c->Draw();
	//eta1->GetYaxis()->SetLacelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3c->cd();
	TPad *pad6c = new TPad("pad6c","pad6c",0,0.05,1,0.3);
	pad6c->SetTopMargin(0.1);
	pad6c->SetBottomMargin(0.2);
	pad6c->SetGridx();
        pad6c->SetGridy();
	pad6c->Draw();
	pad6c->cd();
	
	TH1D *eta1_new_c = (TH1D*)eta3c->Clone("eta1c");
	eta1_new_c ->SetLineColor(kBlack);
	eta1_new_c ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new_c->Sumw2();
	eta1_new_c ->SetStats(0);
	eta1_new_c ->Divide(eta1c);
	eta1_new_c ->SetMarkerStyle(21);
	eta1_new_c ->Draw("ep");
	
	eta1_new_c ->SetTitle("");
        eta1_new_c->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new_c->GetYaxis()->SetTitleSize(10);
        eta1_new_c->GetYaxis()->SetTitleFont(43);
        eta1_new_c->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new_c->GetYaxis()->SetTitleOffset(1.55);
	eta1_new_c ->GetYaxis()->SetNdivisions(303);
	eta1_new_c ->GetYaxis()->SetLabelFont(43);
	eta1_new_c ->GetYaxis()->SetLabelSize(15);
	eta1_new_c ->GetXaxis()->SetLabelFont(43);
	eta1_new_c ->GetXaxis()->SetLabelSize(15);

        eta1c->GetXaxis()->SetTitle("abs(eta)");
        eta1c->GetYaxis()->SetTitle("Entries");
	
	c3c->Print("jet_eta_flav_c.pdf");

// jet1_eta
	TCanvas *c3l = new TCanvas("jet_eta_flav_l","jet_eta_flav_l");
	TPad *pad5l = new TPad("pad5l","pad5l",0,0.3,1,1.0);
	pad5l->SetBottomMargin(0.1);
	pad5l->SetGridx();
	pad5l->Draw();
	pad5l->cd();
	eta1l->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1l->Draw();
	eta3l->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3l->Draw("same");
	
        TLegend *leg3l = new TLegend(0.6,0.6,0.9,0.5);
        leg3l->AddEntry(eta1l, "unweighted");
        leg3l->AddEntry(eta3l, "weighted");
        leg3l->Draw();
	//eta1->GetYaxis()->SetLabelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3l->cd();
	TPad *pad6l = new TPad("pad6l","pad6l",0,0.05,1,0.3);
	pad6l->SetTopMargin(0.1);
	pad6l->SetBottomMargin(0.2);
	pad6l->SetGridx();
        pad6l->SetGridy();
	pad6l->Draw();
	pad6l->cd();
	
	TH1D *eta1_new_l = (TH1D*)eta3l->Clone("eta1l");
	eta1_new_l ->SetLineColor(kBlack);
	eta1_new_l ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new_l->Sumw2();
	eta1_new_l ->SetStats(0);
	eta1_new_l ->Divide(eta1l);
	eta1_new_l ->SetMarkerStyle(21);
	eta1_new_l ->Draw("ep");
	
	eta1_new_l ->SetTitle("");
        eta1_new_l->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new_l->GetYaxis()->SetTitleSize(10);
        eta1_new_l->GetYaxis()->SetTitleFont(43);
        eta1_new_l->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new_l->GetYaxis()->SetTitleOffset(1.55);
	eta1_new_l ->GetYaxis()->SetNdivisions(303);
	eta1_new_l ->GetYaxis()->SetLabelFont(43);
	eta1_new_l ->GetYaxis()->SetLabelSize(15);
	eta1_new_l ->GetXaxis()->SetLabelFont(43);
	eta1_new_l ->GetXaxis()->SetLabelSize(15);

        eta1l->GetXaxis()->SetTitle("abs(eta)");
        eta1l->GetYaxis()->SetTitle("Entries");
	
	c3l->Print("jet_eta_flav_l.pdf");

// jet1_eta
	TCanvas *c3bb = new TCanvas("jet_eta_flav_bb","jet_eta_flav_bb");
	TPad *pad5bb = new TPad("pad5bb","pad5bb",0,0.3,1,1.0);
	pad5bb->SetBottomMargin(0.1);
	pad5bb->SetGridx();
	pad5bb->Draw();
	pad5bb->cd();
	eta1bb->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1bb->Draw();
	eta3bb->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3bb->Draw("same");
	
        TLegend *leg3bb = new TLegend(0.6,0.6,0.9,0.5);
        leg3bb->AddEntry(eta1bb, "unweighted");
        leg3bb->AddEntry(eta3bb, "weighted");
        leg3bb->Draw();
	//eta1->GetYaxis()->SetLabelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3bb->cd();
	TPad *pad6bb = new TPad("pad6bb","pad6bb",0,0.05,1,0.3);
	pad6bb->SetTopMargin(0.1);
	pad6bb->SetBottomMargin(0.2);
	pad6bb->SetGridx();
        pad6bb->SetGridy();
	pad6bb->Draw();
	pad6bb->cd();
	
	TH1D *eta1_new_bb = (TH1D*)eta3bb->Clone("eta1bb");
	eta1_new_bb ->SetLineColor(kBlack);
	eta1_new_bb ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new_bb->Sumw2();
	eta1_new_bb ->SetStats(0);
	eta1_new_bb ->Divide(eta1bb);
	eta1_new_bb ->SetMarkerStyle(21);
	eta1_new_bb ->Draw("ep");
	
	eta1_new_bb ->SetTitle("");
        eta1_new_bb->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new_bb->GetYaxis()->SetTitleSize(10);
        eta1_new_bb->GetYaxis()->SetTitleFont(43);
        eta1_new_bb->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new_bb->GetYaxis()->SetTitleOffset(1.55);
	eta1_new_bb ->GetYaxis()->SetNdivisions(303);
	eta1_new_bb ->GetYaxis()->SetLabelFont(43);
	eta1_new_bb ->GetYaxis()->SetLabelSize(15);
	eta1_new_bb ->GetXaxis()->SetLabelFont(43);
	eta1_new_bb ->GetXaxis()->SetLabelSize(15);

        eta1bb->GetXaxis()->SetTitle("abs(eta)");
        eta1bb->GetYaxis()->SetTitle("Entries");
	
	c3bb->Print("jet_eta_flav_bb.pdf");

// jet1_eta
	TCanvas *c3cc = new TCanvas("jet_eta_flav_cc","jet_eta_flav_cc");
	TPad *pad5cc = new TPad("pad5cc","pad5cc",0,0.3,1,1.0);
	pad5cc->SetBottomMargin(0.1);
	pad5cc->SetGridx();
	pad5cc->Draw();
	pad5cc->cd();
	eta1cc->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1cc->Draw();
	eta3cc->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3cc->Draw("same");
	
        TLegend *leg3cc = new TLegend(0.6,0.6,0.9,0.5);
        leg3cc->AddEntry(eta1cc, "unweighted");
        leg3cc->AddEntry(eta3cc, "weighted");
        leg3cc->Draw();
	//eta1->GetYaxis()->SetLaccelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3cc->cd();
	TPad *pad6cc = new TPad("pad6cc","pad6cc",0,0.05,1,0.3);
	pad6cc->SetTopMargin(0.1);
	pad6cc->SetBottomMargin(0.2);
	pad6cc->SetGridx();
        pad6cc->SetGridy();
	pad6cc->Draw();
	pad6cc->cd();
	
	TH1D *eta1_new_cc = (TH1D*)eta3cc->Clone("eta1cc");
	eta1_new_cc ->SetLineColor(kBlack);
	eta1_new_cc ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new_cc->Sumw2();
	eta1_new_cc ->SetStats(0);
	eta1_new_cc ->Divide(eta1cc);
	eta1_new_cc ->SetMarkerStyle(21);
	eta1_new_cc ->Draw("ep");
	
	eta1_new_cc ->SetTitle("");
        eta1_new_cc->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new_cc->GetYaxis()->SetTitleSize(10);
        eta1_new_cc->GetYaxis()->SetTitleFont(43);
        eta1_new_cc->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new_cc->GetYaxis()->SetTitleOffset(1.55);
	eta1_new_cc ->GetYaxis()->SetNdivisions(303);
	eta1_new_cc ->GetYaxis()->SetLabelFont(43);
	eta1_new_cc ->GetYaxis()->SetLabelSize(15);
	eta1_new_cc ->GetXaxis()->SetLabelFont(43);
	eta1_new_cc ->GetXaxis()->SetLabelSize(15);

        eta1cc->GetXaxis()->SetTitle("abs(eta)");
        eta1cc->GetYaxis()->SetTitle("Entries");
	
	c3cc->Print("jet_eta_flav_cc.pdf");

return;
}