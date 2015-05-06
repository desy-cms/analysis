void plot_m12()
{
   TFile * f1 = new TFile("hadd_genjets_qcd.jpt140_jpt100_jpt30_jeta1p6_jeta1p6_jeta2p4.root");
   TFile * f2 = new TFile("hadd_genjets_qcd.jpt140_jpt100_jpt60_jeta2p4_jeta2p4_jeta2p4.root");
   
   TH1F * h_m12_1 = (TH1F*) f1 -> Get("h_MassJet01");
   TH1F * h_m12_2 = (TH1F*) f2 -> Get("h_MassJet01");
   
   
   h_m12_1 -> SetLineColor(2);
   h_m12_1 -> SetLineWidth(2);
   h_m12_1 -> SetMarkerStyle(20);
   h_m12_1 -> SetMarkerColor(2);

   
   h_m12_2 -> SetLineWidth(2);
   h_m12_2 -> SetLineColor(1);
   h_m12_2 -> SetMarkerColor(1);
   h_m12_2 -> SetMarkerStyle(20);

   h_m12_1 -> Rebin(5);
   h_m12_2 -> Rebin(5);

   h_m12_1 -> GetXaxis() -> SetRangeUser(100.,1200.);
   h_m12_1 -> GetXaxis() -> SetTitle("M12 (GeV)");
   h_m12_1 -> GetYaxis() -> SetTitle("entries");
      
   TCanvas * c1 = new TCanvas("c1","",0,0,700,500);
   h_m12_1 -> Draw("HIST");
   h_m12_2 -> Draw("HIST same");
   
   c1 -> SaveAs("qcd_m12_jet1cut_jet12eta_jet3cut.png");
   
}
