void plot_eta_s_b()
{
   TFile * f1 = new TFile("signal/M300/genjets_susybbHtobb-M300.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f2 = new TFile("signal/M600/genjets_susybbHtobb-M600.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f3 = new TFile("background/hadd_genjets_qcd.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   TFile * f4 = new TFile("signal/M1100/genjets_susybbHtobb-M1100.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   
   TH1F * h_eta_1 = (TH1F*) f1 -> Get("h_EtaJet_2");
   TH1F * h_eta_2 = (TH1F*) f2 -> Get("h_EtaJet_2");
   TH1F * h_eta_3 = (TH1F*) f3 -> Get("h_EtaJet_2");
   TH1F * h_eta_4 = (TH1F*) f4 -> Get("h_EtaJet_2");
   
   
   h_eta_1 -> SetLineColor(2);
   h_eta_1 -> SetLineWidth(2);
   h_eta_1 -> SetMarkerStyle(20);
   h_eta_1 -> SetMarkerColor(2);
     
     
   h_eta_2 -> SetLineWidth(2);
   h_eta_2 -> SetLineColor(1);
   h_eta_2 -> SetMarkerColor(1);
   h_eta_2 -> SetMarkerStyle(20);
     
   h_eta_3 -> SetLineWidth(2);
   h_eta_3 -> SetLineColor(4);
   h_eta_3 -> SetMarkerColor(1);
   h_eta_3 -> SetMarkerStyle(20);
     
   h_eta_4 -> SetLineWidth(2);
   h_eta_4 -> SetLineColor(3);
   h_eta_4 -> SetMarkerColor(1);
   h_eta_4 -> SetMarkerStyle(20);
   
//    h_eta_1 -> Rebin(2);
//    h_eta_2 -> Rebin(2);
//    h_eta_3 -> Rebin(2);
//    h_eta_4 -> Rebin(2);
   
   h_eta_1 -> Scale(1./h_eta_1->Integral());
   h_eta_2 -> Scale(1./h_eta_2->Integral());
   h_eta_3 -> Scale(1./h_eta_3->Integral());
   h_eta_4 -> Scale(1./h_eta_4->Integral());
     
//   h_eta_1 -> GetXaxis() -> SetRangeUser(0.,200.);
   h_eta_1 -> GetXaxis() -> SetTitle("eta");
   h_eta_1 -> GetYaxis() -> SetTitle("entries");
      
   TCanvas * c1 = new TCanvas("c1","",0,0,700,500);
//   c1->SetLogy();
   h_eta_1 -> Draw("HIST");
   h_eta_2 -> Draw("HIST same");
   h_eta_3 -> Draw("HIST same");
//   h_eta_4 -> Draw("HIST same");
   
   c1 -> SaveAs("s_b_eta3.png");
   
}
