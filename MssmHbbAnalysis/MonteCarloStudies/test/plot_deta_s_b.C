void plot_deta_s_b()
{
   TFile * f1 = new TFile("signal/M300/genjets_susybbHtobb-M300.jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4.root");
   TFile * f2 = new TFile("signal/M600/genjets_susybbHtobb-M600.jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4.root");
   TFile * f3 = new TFile("signal/M1100/genjets_susybbHtobb-M1100.jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4.root");
   TFile * f4 = new TFile("background/hadd_genjets_qcd.jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4.root");
   
   TH1F * h_deta_1 = (TH1F*) f1 -> Get("h_dEta01");
   TH1F * h_deta_2 = (TH1F*) f2 -> Get("h_dEta01");
   TH1F * h_deta_3 = (TH1F*) f3 -> Get("h_dEta01");
   TH1F * h_deta_4 = (TH1F*) f4 -> Get("h_dEta01");
   
   
   h_deta_1 -> SetLineColor(2);
   h_deta_1 -> SetLineWidth(2);
   h_deta_1 -> SetMarkerStyle(20);
   h_deta_1 -> SetMarkerColor(2);
     
     
   h_deta_2 -> SetLineWidth(2);
   h_deta_2 -> SetLineColor(1);
   h_deta_2 -> SetMarkerColor(1);
   h_deta_2 -> SetMarkerStyle(20);
   
   h_deta_3 -> SetLineWidth(2);
   h_deta_3 -> SetLineColor(3);
   h_deta_3 -> SetMarkerColor(1);
   h_deta_3 -> SetMarkerStyle(20);
     
   h_deta_4 -> SetLineWidth(2);
   h_deta_4 -> SetLineColor(4);
   h_deta_4 -> SetMarkerColor(1);
   h_deta_4 -> SetMarkerStyle(20);
     
//    h_deta_1 -> Rebin(2);
//    h_deta_2 -> Rebin(2);
//    h_deta_3 -> Rebin(2);
//    h_deta_4 -> Rebin(2);
   
   h_deta_1 -> Scale(1./h_deta_1->Integral());
   h_deta_2 -> Scale(1./h_deta_2->Integral());
   h_deta_3 -> Scale(1./h_deta_3->Integral());
   h_deta_4 -> Scale(1./h_deta_4->Integral());
     
//   h_deta_1 -> GetXaxis() -> SetRangeUser(0.,200.);
   h_deta_1 -> GetXaxis() -> SetTitle("eta");
   h_deta_1 -> GetYaxis() -> SetTitle("entries");
      
   TCanvas * c1 = new TCanvas("c1","",0,0,700,500);
//   c1->SetLogy();
   h_deta_1 -> Draw("HIST");
   h_deta_2 -> Draw("HIST same");
   h_deta_3 -> Draw("HIST same");
   h_deta_4 -> Draw("HIST same");
   
   c1 -> SaveAs("s_b_deta12_v2.png");
   
}
