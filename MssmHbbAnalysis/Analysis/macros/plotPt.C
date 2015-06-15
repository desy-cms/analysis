void plotPt() 
{
   gStyle->SetOptStat(0);
   TFile * f[5]; 
   f[0] = new TFile("dijet20.root");
   f[1] = new TFile("dijet40.root");
   f[2] = new TFile("dijet70.root");
   f[3] = new TFile("dijet110.root");
   f[4] = new TFile("jet300.root");

   TCanvas * c1 = new TCanvas("c1","",700,800);
   c1 -> SetLogy();
   
   TH1F * h_pPt[5];
   TH1F * h_tPt[5];
   
   int colors[5] = {1,2,3,4,6};
   
   for ( int i = 0 ; i < 5 ; ++ i )
   {
      h_pPt[i] = (TH1F*) f[i] -> Get("h_jetProbePt");
      h_tPt[i] = (TH1F*) f[i] -> Get("h_jetTagPt");
      h_pPt[i] -> SetLineWidth(2);
      h_tPt[i] -> SetLineWidth(2);
      h_pPt[i] -> SetLineColor(colors[i]);
      h_tPt[i] -> SetLineColor(colors[i]);
      if ( i == 0 )
      {
         h_pPt[i] -> GetYaxis() -> SetRangeUser(50,6000);
         h_pPt[i] -> GetYaxis() -> SetTitle("events");
         h_pPt[i] -> GetXaxis() -> SetTitle("Probe Jet pT (GeV)");
         h_pPt[i] -> Draw();
      }
      else
         h_pPt[i] -> Draw("same");
         
   }
   
   c1 -> SaveAs("probe_jet_pt.png");
   
}
