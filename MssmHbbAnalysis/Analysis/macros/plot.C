void plot() 
{
   gStyle->SetOptStat(0);
   TFile * f = new TFile("jet300.root");
   
   TH1F * h_sel = (TH1F*) f -> Get("h_SelChain");
   
   TCanvas * c1 = new TCanvas("c1","",1000,800);
   
   c1 -> SetLogy();
   
   h_sel -> SetLineWidth(2);
   h_sel -> GetYaxis()->SetTitle("events");
   cout << h_sel -> GetBinContent(8) << endl;
   
   h_sel -> Draw();
   
   c1 -> SaveAs("events_jet300.png");
   
   
   TH1F * h_pf = (TH1F*) f -> Get("h_jetProbeFlavour");
   TH1F * h_tf = (TH1F*) f -> Get("h_jetTagFlavour");
   
   TCanvas * c2 = new TCanvas("c2","",800,1000);
   h_pf -> GetXaxis() -> SetTitle("probe jet flavour");
   h_pf -> GetYaxis() -> SetTitle("event");
   h_pf -> SetLineWidth(2);
   h_pf -> Draw();
   
   
   TCanvas * c3 = new TCanvas("c3","",800,1000);
   h_tf -> GetXaxis() -> SetTitle("tag jet flavour");
   h_tf -> GetYaxis() -> SetTitle("event");
   h_tf -> SetLineWidth(2);
   h_tf -> Draw();
   
   c2 -> SaveAs("probe_flavour_jet300.png");
   c3 -> SaveAs("tag_flavour_jet300.png");
   
   
}
