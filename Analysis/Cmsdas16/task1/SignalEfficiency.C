// Number of selected signal events divided by the total number of generated events

void SignalEfficiency()
{
   TFile * f[20];
   
   double eff[20];
   double mass[20];
   
   mass[0] = 300.;
   mass[1] = 400.;
   mass[2] = 500.;
   mass[3] = 600.;
   mass[4] = 700.;
   mass[5] = 900.;
   mass[6] = 1100.;
   mass[7] = 1300.;
   
   for ( int i = 0 ; i < 8; ++i )
   {
      f[i] = new TFile(Form("selection_bbb_MMM/mssmhbb_%i_analysis_histograms.root",int(mass[i])),"OLD");
      
      TH1F * m12 = (TH1F*)f[i]->Get("m12");
      double nGen = ((TH1F*)f[i]->Get("nGen"))->GetBinContent(1);
      double nEvts = m12->GetEffectiveEntries();
      eff[i] = nEvts/nGen;
   }
   TCanvas * c1 = new TCanvas("c1","");
   
   TGraph * gr = new TGraph(8,mass,eff);
   gr -> SetTitle("Signal Efficiency");
   gr -> SetMarkerStyle(20);
   gr -> GetXaxis()->SetTitle("mass (GeV)");
   gr -> GetYaxis()->SetTitle("efficiency");
   gr-> Draw("ALP");

   TFile * out = new TFile("signal_eff_MMM.root","RECREATE");
   gr -> Write();
      
}
