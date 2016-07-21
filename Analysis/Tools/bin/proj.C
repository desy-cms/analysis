#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TCanvas.h"

void proj()
{
TFile *f1[13], *f2[13]; 

f1[0] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_50_to_80.root" , "READ");
f2[0] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_50_to_80.root", "READ");
f1[1] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_80_to_120.root" , "READ");
f2[1] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_80_to_120.root", "READ");
f1[2] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_120_to_170.root" , "READ");
f2[2] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_120_to_170.root", "READ");
f1[3] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_170_to_300.root" , "READ");
f2[3] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_170_to_300.root", "READ");
f1[4] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_300_to_470.root" , "READ");
f2[4] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_300_to_470.root", "READ");
f1[5] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_470_to_600.root" , "READ");
f2[5] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_470_to_600.root", "READ");
f1[6] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_600_to_800.root" , "READ");
f2[6] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_600_to_800.root", "READ");
f1[7] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_800_to_1000.root" , "READ");
f2[7] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_800_to_1000.root", "READ");
f1[8] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_1000_to_1400.root" , "READ");
f2[8] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_1000_to_1400.root", "READ");
f1[9] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_1400_to_1800.root" , "READ");
f2[9] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_1400_to_1800.root", "READ");
f1[10] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_1800_to_2400.root" , "READ");
f2[10] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_1800_to_2400.root", "READ");
f1[11] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_2400_to_3200.root" , "READ");
f2[11] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_2400_to_3200.root", "READ");
f1[12] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_wo_weight_3200_to_inf.root" , "READ");
f2[12] = new TFile("/afs/desy.de/user/s/sureshna/cmssw/CMSSW_7_6_5/src/Analysis/Tools/bin/BtagEffoutput/QCD_oldcalib_delR0.3/histograms_with_weight_3200_to_inf.root", "READ");

 TH2F *hist1 = new TH2F();
 //TH2F *hist2 = new TH2F();
 TH2F *hist3 = new TH2F();
 //TH2F *hist4 = new TH2F();
 //TH1F *hist5 = new TH1F();
 //TH1F *hist6 = new TH1F();


 for(int i = 0; i < 13 ; ++i)
 {
 if ( i == 0)
 {
 hist1 = (TH2F*)f1[i]->Get("hist1");
 //hist2 = (TH2F*)f1[i]->Get("hist2");
 //hist5 = (TH1F*)f1[i]->Get("hist5");
 hist3 = (TH2F*)f2[i]->Get("hist3");
 //hist4 = (TH2F*)f2[i]->Get("hist4");
 //hist6 = (TH1F*)f2[i]->Get("hist6");
 }
// 
else
{
hist1 -> Add((TH2F*)f1[i]->Get("hist1"));
//hist2 -> Add((TH2F*)f1[i]->Get("hist2"));
//hist5 -> Add((TH1F*)f1[i]->Get("hist5"));
hist3 -> Add((TH2F*)f2[i]->Get("hist3"));
//hist4 -> Add((TH2F*)f2[i]->Get("hist4"));
//hist6 -> Add((TH1F*)f2[i]->Get("hist6"));
}
}



TH1D *pt1 = hist1->ProjectionX();
TH1D *eta1 = hist1->ProjectionY();
 //TH1D *pt2 = hist2->ProjectionX();
 //TH1D *eta2 = hist2->ProjectionY();
TH1D *pt3 = hist3->ProjectionX();
TH1D *eta3 = hist3->ProjectionY();
 //TH1D *pt4 = hist4->ProjectionX();
 //TH1D *eta4 = hist4->ProjectionY();

//jet1_pt
	TCanvas *c1 = new TCanvas("jet_pt","jet_pt");
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1.0);
	pad1->SetBottomMargin(0.1);
	pad1->SetGridx();
	pad1->Draw();
	pad1->cd();
        gStyle->SetOptTitle(kFALSE);
	pt1->SetStats(0);
	pt1->GetXaxis()->SetRangeUser(0,600);
	//pt1->GetYaxis()->SetRangeUser(0,30000);
	pt1->Draw();
	pt3->SetLineColor(kRed);
	pt3->GetXaxis()->SetRangeUser(0,600);
	//pt3->GetYaxis()->SetRangeUser(0,30000);
	pt3->Draw("same");

        TLegend *leg1 = new TLegend(0.6,0.6,0.9,0.5);
        leg1->AddEntry(pt1, "unweighted");
        leg1->AddEntry(pt3, "weighted");
        leg1->Draw();
	
	//pt1->GetYaxis()->SetLabelSize(0.);
	//TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
	//axis1->SetLabelFont(43);
	//axis1->SetLabelSize(15);
	//axis1->SetLabelOffset(0.025);
	//axis1->Draw();
	
	c1->cd();
	TPad *pad2 = new TPad("pad2","pad2",0,0.05,1,0.3);
	pad2->SetTopMargin(0.1);
	pad2->SetBottomMargin(0.1);
	pad2->SetGridx();
        pad2->SetGridy();
	pad2->Draw();
	pad2->cd();
	
	TH1D *pt1_new = (TH1D*)pt3->Clone("pt1");
	pt1_new->SetLineColor(kBlack);
	//pt1_new->SetMinimum(0.001);
	//pt1_new->SetMaximum(1.6);
	pt1_new->Sumw2();
	pt1_new->SetStats(0);
	pt1_new->Divide(pt1);
	pt1_new->SetMarkerStyle(21);
	pt1_new->Draw("ep");
	
	pt1_new->SetTitle("");
        pt1_new->GetYaxis()->SetTitle("ratio weighted/unweighted");
        pt1_new->GetYaxis()->SetTitleSize(10);
        pt1_new->GetYaxis()->SetTitleFont(43);
        pt1_new->GetYaxis()->SetTitleOffset(1.55);
        pt1_new->GetYaxis()->SetRangeUser(0,1.5);
	pt1_new->GetYaxis()->SetNdivisions(505);
	pt1_new->GetYaxis()->SetLabelFont(43);
	pt1_new->GetYaxis()->SetLabelSize(15);
	pt1_new->GetXaxis()->SetLabelFont(43);
	pt1_new->GetXaxis()->SetLabelSize(15);

        pt1->GetXaxis()->SetTitle("pT(GeV)");
        pt1->GetYaxis()->SetTitle("Entries");
	
	c1->Print("jet_pt.pdf");

//jet2_pt
// 	TCanvas *c2 = new TCanvas("jet2_pt","jet2_pt");
// 	TPad *pad3 = new TPad("pad3","pad3",0,0.3,1,1.0);
// 	pad3->SetBottomMargin(0.1);
// 	pad3->SetGridx();
// 	pad3->Draw();
// 	pad3->cd();
// 	pt2->SetStats(0);
// 	pt2->GetXaxis()->SetRangeUser(0,1000);
// 	//pt2->GetYaxis()->SetRangeUser(0,20000);
// 	pt2->Draw();
// 	pt4->SetLineColor(kRed);
// 	pt4->GetXaxis()->SetRangeUser(0,1000);
// 	//pt4->GetYaxis()->SetRangeUser(0,20000);
// 	pt4->Draw("same");
// 
//         TLegend *leg2 = new TLegend(0.6,0.6,0.9,0.5);
//         leg2->AddEntry(pt2, "unweighted");
//         leg2->AddEntry(pt4, "weighted");
//         leg2->Draw();
// 	
// 	// pt2->GetYaxis()->SetLabelSize(0.);
// 	// TGaxis *axis2 = new TGaxis(-5,-20,-5,220,20,220,510,"");
// 	// axis2->SetLabelFont(43);
// 	// axis2->SetLabelSize(15);
// 	// axis2->Draw();
// 	
// 	c2->cd();
// 	TPad *pad4 = new TPad("pad4","pad4",0,0.05,1,0.3);
// 	pad4->SetTopMargin(0.1);
// 	pad4->SetBottomMargin(0.2);
// 	pad4->SetGridx();
//         pad4->SetGridy();
// 	pad4->Draw();
// 	pad4->cd();
// 	
// 	TH1D *pt2_new = (TH1D*)pt4->Clone("pt2");
// 	pt2_new->SetLineColor(kBlack);
// 	//pt2_new->SetMinimum(0.5);
// 	//pt2_new->SetMaximum(2.0);
// 	pt2_new->Sumw2();
// 	pt2_new->SetStats(0);
// 	pt2_new->Divide(pt2);
// 	pt2_new->SetMarkerStyle(21);
// 	pt2_new->Draw("ep");
// 	
// 	pt2_new->SetTitle("");
//         pt2_new->GetYaxis()->SetTitle("ratio weighted/unweighted");
//         pt2_new->GetYaxis()->SetTitleSize(10);
//         pt2_new->GetYaxis()->SetTitleFont(43);
//         pt2_new->GetYaxis()->SetTitleOffset(1.55);
// 	pt2_new->GetYaxis()->SetNdivisions(505);
// 	pt2_new->GetYaxis()->SetLabelFont(43);
// 	pt2_new->GetYaxis()->SetLabelSize(15);
// 	pt2_new->GetXaxis()->SetLabelFont(43);
// 	pt2_new->GetXaxis()->SetLabelSize(15);
// 
//         pt2->GetXaxis()->SetTitle("pT(GeV)");
//         pt2->GetYaxis()->SetTitle("Entries");
// 	
// 	c2->Print("jet2_pt_hadron.pdf");

// jet1_eta
	TCanvas *c3 = new TCanvas("jet_eta","jet_eta");
	TPad *pad5 = new TPad("pad5","pad5",0,0.3,1,1.0);
	pad5->SetBottomMargin(0.1);
	pad5->SetGridx();
	pad5->Draw();
	pad5->cd();
	eta1->SetStats(0);
	//eta1->GetYaxis()->SetRangeUser(0,35000);
	eta1->Draw();
	eta3->SetLineColor(kRed);
	//eta3->GetYaxis()->SetRangeUser(0,35000);
	eta3->Draw("same");
	
        TLegend *leg3 = new TLegend(0.6,0.6,0.9,0.5);
        leg3->AddEntry(eta1, "unweighted");
        leg3->AddEntry(eta3, "weighted");
        leg3->Draw();
	//eta1->GetYaxis()->SetLabelSize(0.);
	
	// TGaxis *axis3 = new TGaxis(-5,-20,-5,220,20,220,510,"");
	// axis3->SetLabelFont(43);
	// axis3->SetLabelSize(15);
	// axis3->Draw();
	
	c3->cd();
	TPad *pad6 = new TPad("pad6","pad6",0,0.05,1,0.3);
	pad6->SetTopMargin(0.1);
	pad6->SetBottomMargin(0.2);
	pad6->SetGridx();
        pad6->SetGridy();
	pad6->Draw();
	pad6->cd();
	
	TH1D *eta1_new = (TH1D*)eta3->Clone("eta1");
	eta1_new ->SetLineColor(kBlack);
	eta1_new ->SetMinimum(0.);
	//eta1_new ->SetMaximum(1.5);
	eta1_new ->Sumw2();
	eta1_new ->SetStats(0);
	eta1_new ->Divide(eta1);
	eta1_new ->SetMarkerStyle(21);
	eta1_new ->Draw("ep");
	
	eta1_new ->SetTitle("");
        eta1_new->GetYaxis()->SetTitle("ratio weighted/unweighted");
        eta1_new->GetYaxis()->SetTitleSize(10);
        eta1_new->GetYaxis()->SetTitleFont(43);
        eta1_new->GetYaxis()->SetRangeUser(0,1.5);
        eta1_new->GetYaxis()->SetTitleOffset(1.55);
	eta1_new ->GetYaxis()->SetNdivisions(303);
	eta1_new ->GetYaxis()->SetLabelFont(43);
	eta1_new ->GetYaxis()->SetLabelSize(15);
	eta1_new ->GetXaxis()->SetLabelFont(43);
	eta1_new ->GetXaxis()->SetLabelSize(15);

        eta1->GetXaxis()->SetTitle("abs(eta)");
        eta1->GetYaxis()->SetTitle("Entries");
	
	c3->Print("jet_eta.pdf");

// jet2_eta
// 	TCanvas *c4 = new TCanvas("jet2_eta","jet2_eta");
// 	TPad *pad7 = new TPad("pad7","pad7",0,0.3,1,1.0);
// 	pad7->SetBottomMargin(0.1);
// 	pad7->SetGridx();
// 	pad7->Draw();
// 	pad7->cd();
// 	eta2->SetStats(0);
// 	//eta2->GetYaxis()->SetRangeUser(0,12000);
// 	eta2->Draw();
// 	eta4->SetLineColor(kRed);
// 	//eta4->GetYaxis()->SetRangeUser(0,12000);
// 	eta4->Draw("same");
// 
//         TLegend *leg4 = new TLegend(0.6,0.6,0.9,0.5);
//         leg4->AddEntry(eta2, "unweighted");
//         leg4->AddEntry(eta4, "weighted");
//         leg4->Draw();
// 	
// 	// eta2->GetYaxis()->SetLabelSize(0.);
// 	// TGaxis *axis4 = new TGaxis(-5,-20,-5,220,20,220,510,"");
// 	// axis4->SetLabelFont(43);
// 	// axis4->SetLabelSize(15);
// 	// axis4->Draw();
// 	
// 	c4->cd();
// 	TPad *pad8 = new TPad("pad8","pad8",0,0.05,1,0.3);
// 	pad8->SetTopMargin(0.1);
// 	pad8->SetBottomMargin(0.2);
// 	pad8->SetGridx();
//         pad8->SetGridy();
// 	pad8->Draw();
// 	pad8->cd();
// 	
// 	TH1D *eta2_new = (TH1D*)eta4->Clone("eta2");
// 	eta2_new ->SetLineColor(kBlack);
// 	eta2_new ->SetMinimum(0.);
// 	eta2_new ->SetMaximum(1.5);
// 	eta2_new ->Sumw2();
// 	eta2_new ->SetStats(0);
// 	eta2_new ->Divide(eta2);
// 	eta2_new ->SetMarkerStyle(21);
// 	eta2_new ->Draw("ep");
// 	
// 	eta2_new ->SetTitle("");
//         eta2_new->GetYaxis()->SetTitle("ratio weighted/unweighted");
//         eta2_new->GetYaxis()->SetTitleSize(10);
//         eta2_new->GetYaxis()->SetTitleFont(43);
//         eta2_new->GetYaxis()->SetTitleOffset(1.55);
// 	eta2_new ->GetYaxis()->SetNdivisions(303);
// 	eta2_new ->GetYaxis()->SetLabelFont(43);
// 	eta2_new ->GetYaxis()->SetLabelSize(15);
// 	eta2_new ->GetXaxis()->SetLabelFont(43);
// 	eta2_new ->GetXaxis()->SetLabelSize(15);
// 
//         eta2->GetXaxis()->SetTitle("abs(eta)");
//         eta2->GetYaxis()->SetTitle("Entries");
// 
// 	c4->Print("jet2_eta_hadron.pdf");

// //mass
//         TCanvas *c5 = new TCanvas("masses","masses");
// 	TPad *pad9 = new TPad("pad9","pad9",0,0.3,1,1.0);
// 	pad1->SetBottomMargin(0.1);
// 	pad1->SetGridx();
// 	pad1->Draw();
// 	pad1->cd();
//         gStyle->SetOptTitle(kFALSE);
// 	hist5->SetStats(0);
// 	//hist5->GetYaxis()->SetRangeUser(0,200000);
// 	//hist5->GetXaxis()->SetRangeUser(0,700);
// 	hist5->Draw();
// 	hist6->SetLineColor(kRed);
// 	//hist6->GetYaxis()->SetRangeUser(0,200000);
// 	//hist6->GetXaxis()->SetRangeUser(0,700);
// 	hist6->Draw("same");
// 
//         TLegend *leg5 = new TLegend(0.6,0.6,0.9,0.5);
//         leg5->AddEntry(hist5, "unweighted");
//         leg5->AddEntry(hist6, "weighted");
//         leg5->Draw();
// 	
// 	pt1->GetYaxis()->SetLabelSize(0.);
// 	TGaxis *axis1 = new TGaxis(0,0,0,160,20,160,203,"");
// 	axis1->SetLabelFont(43);
// 	axis1->SetLabelSize(15);
// 	axis1->SetLabelOffset(0.025);
// 	axis1->Draw();
// 	
// 	c5->cd();
// 	TPad *pad10 = new TPad("pad10","pad10",0,0.05,1,0.3);
// 	pad10->SetTopMargin(0.1);
// 	pad10->SetBottomMargin(0.1);
// 	pad10->SetGridx();
//         pad10->SetGridy();
// 	pad10->Draw();
// 	pad10->cd();
// 	
// 	TH1F *hist_new = (TH1F*)hist6->Clone("hist5");
// 	hist_new->SetLineColor(kBlack);
// 	//pt1_new->SetMinimum(0.001);
// 	//pt1_new->SetMaximum(1.5);
// 	hist_new->Sumw2();
// 	hist_new->SetStats(0);
// 	hist_new->Divide(hist5);
// 	hist_new->SetMarkerStyle(21);
// 	hist_new->Draw("ep");
// 	
// 	hist_new->SetTitle("");
//         hist_new->GetYaxis()->SetTitle("ratio weighted/unweighted");
//         hist_new->GetYaxis()->SetTitleSize(10);
//         hist_new->GetYaxis()->SetTitleFont(43);
//         hist_new->GetYaxis()->SetRangeUser(0,1.5);
//         hist_new->GetYaxis()->SetTitleOffset(1.55);
// 	hist_new->GetYaxis()->SetNdivisions(505);
// 	hist_new->GetYaxis()->SetLabelFont(43);
// 	hist_new->GetYaxis()->SetLabelSize(15);
// 	hist_new->GetXaxis()->SetLabelFont(43);
// 	hist_new->GetXaxis()->SetLabelSize(15);
// 
//         //pt1->GetXaxis()->SetTitle("pT(GeV)");
//         //pt1->GetYaxis()->SetTitle("Entries");
// 	
// 	c5->Print("masses.pdf");

return;
}