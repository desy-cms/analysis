void plot_pt()
{
   TFile * f1 = new TFile("m300/genjets_jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   
   TH1F * h_pt_1 = (TH1F*) f1 -> Get("h_PtJet_0");
   TH1F * h_pt_2 = (TH1F*) f1 -> Get("h_PtJet_1");
   TH1F * h_pt_3 = (TH1F*) f1 -> Get("h_PtJet_2");
   
   
   h_pt_1 -> SetLineWidth(2);
   h_pt_1 -> SetLineColor(2);
   h_pt_1 -> SetMarkerStyle(20);
   h_pt_1 -> SetMarkerColor(2);
     
   h_pt_2 -> SetLineWidth(2);
   h_pt_2 -> SetLineColor(1);
   h_pt_2 -> SetMarkerColor(1);
   h_pt_2 -> SetMarkerStyle(20);
     
   h_pt_3 -> SetLineWidth(2);
   h_pt_3 -> SetLineColor(4);
   h_pt_3 -> SetMarkerColor(4);
   h_pt_3 -> SetMarkerStyle(20);
     
   h_pt_3 -> GetXaxis() -> SetRangeUser(20.,500.);
   h_pt_3 -> GetXaxis() -> SetTitle("pT (GeV)");
   h_pt_3 -> GetYaxis() -> SetTitle("entries");
      
   TCanvas * c1 = new TCanvas("c1","",0,0,700,500);
//   c1->SetLogy();
   h_pt_3 -> Draw("hist");
   h_pt_2 -> Draw("hist same");
   h_pt_1 -> Draw("hist same");
   
   c1 -> SaveAs("m300_pt.png");
   
}
