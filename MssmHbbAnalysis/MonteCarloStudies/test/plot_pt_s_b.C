void plot_pt_s_b()
{
   TFile * f1 = new TFile("signal/M300/genjets_susybbHtobb-M300.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f2 = new TFile("signal/M600/genjets_susybbHtobb-M600.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f3 = new TFile("background/hadd_genjets_qcd.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f4 = new TFile("signal/M1100/genjets_susybbHtobb-M1100.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   
   TH1F * h_pt_1 = (TH1F*) f1 -> Get("h_PtJet_2");
   TH1F * h_pt_2 = (TH1F*) f2 -> Get("h_PtJet_2");
   TH1F * h_pt_3 = (TH1F*) f3 -> Get("h_PtJet_2");
   TH1F * h_pt_4 = (TH1F*) f4 -> Get("h_PtJet_2");
   
   
   h_pt_1 -> SetLineColor(2);
   h_pt_1 -> SetLineWidth(2);
   h_pt_1 -> SetMarkerStyle(20);
   h_pt_1 -> SetMarkerColor(2);
     
     
   h_pt_2 -> SetLineWidth(2);
   h_pt_2 -> SetLineColor(1);
   h_pt_2 -> SetMarkerColor(1);
   h_pt_2 -> SetMarkerStyle(20);
     
   h_pt_3 -> SetLineWidth(2);
   h_pt_3 -> SetLineColor(4);
   h_pt_3 -> SetMarkerColor(1);
   h_pt_3 -> SetMarkerStyle(20);
     
   h_pt_4 -> SetLineWidth(2);
   h_pt_4 -> SetLineColor(3);
   h_pt_4 -> SetMarkerColor(1);
   h_pt_4 -> SetMarkerStyle(20);
   
   h_pt_1 -> Rebin(2);
   h_pt_2 -> Rebin(2);
   h_pt_3 -> Rebin(2);
   h_pt_4 -> Rebin(2);
   
   h_pt_1 -> Scale(1./h_pt_1->Integral());
   h_pt_2 -> Scale(1./h_pt_2->Integral());
   h_pt_3 -> Scale(1./h_pt_3->Integral());
   h_pt_4 -> Scale(1./h_pt_4->Integral());
     
   h_pt_1 -> GetXaxis() -> SetRangeUser(0.,200.);
   h_pt_1 -> GetXaxis() -> SetTitle("pT (GeV)");
   h_pt_1 -> GetYaxis() -> SetTitle("entries");
      
   TCanvas * c1 = new TCanvas("c1","",0,0,700,500);
//   c1->SetLogy();
   h_pt_1 -> Draw("HIST");
   h_pt_2 -> Draw("HIST same");
   h_pt_3 -> Draw("HIST same");
   h_pt_4 -> Draw("HIST same");
   
   c1 -> SaveAs("s_b_pt3.png");
   
}
