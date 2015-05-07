void plot_eff()
{
   TFile * f[4];
   
   f[0] = new TFile("signal/m300/genjets_m300.jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   f[1] = new TFile("signal/m600/genjets_m600.jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   f[2] = new TFile("signal/m1100/genjets_m1100.jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   f[3] = new TFile("background/hadd_genjets_qcd.jpt0_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   
   TH1F * h_pt[4];
   
   for ( int i = 0; i < 3; ++i )
   {
      h_pt[i] = (TH1F*) f[i] -> Get("h_PtJet_0");
      h_pt[i] -> SetName(Form("h_PtJet1_%i",i));
   }
   
   float ptcut[1000];
   float eff[1000];
   
   TGraph * g[4];
   
   float minpt = 100.;
   float maxpt = 150.;
   
   for ( int i = 0; i < 3; ++i )
   {
      int j = 0;
      for ( int bin = 1; bin < h_pt[i]->GetNbinsX(); ++bin )
      {
         float pt = h_pt[i] -> GetBinLowEdge(bin);
         if ( pt < minpt ) continue;
         if ( pt > maxpt ) break;
         ptcut[j] = pt;
         eff[j] = h_pt[i]->Integral(bin,h_pt[i]->GetNbinsX())/h_pt[i]->Integral(1,h_pt[i]->GetNbinsX());
         ++j;
      }
      g[i] = new TGraph(j,ptcut,eff);
   }
   g[3] -> Draw("AC*");
   
   
   
}
