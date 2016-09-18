// Get the S/Sqrt(B) within a mass window +-20% around the nominal Higgs mass
void Significance()
{
   double significance[20];
   double background[20];
   double signal[20];
   double mass[20];
   
   mass[0] = 300.;
   mass[1] = 400.;
   mass[2] = 500.;
   mass[3] = 600.;
   mass[4] = 700.;
   mass[5] = 900.;
   mass[6] = 1100.;
   mass[7] = 1300.;
   
   TFile * fd = new TFile("selection_bbnb_MMM/data_analysis_histograms.root","OLD");
   TH1F * m12d = (TH1F*)fd->Get("m12");
   
   TFile * f[20];
   for ( int i = 0 ; i < 8; ++i )
   {
      f[i] = new TFile(Form("selection_bbb_MMM/mssmhbb_%i_analysis_histograms.root",int(mass[i])),"OLD");
      TH1F * m12 = (TH1F*)f[i]->Get("m12");
      // integral limits
      int low = -1;
      int high = -1;
      for ( int j = 0; j < m12->GetNbinsX() ; ++j )
      {
         float value = m12 -> GetBinCenter(j+1);
         if ( value >= mass[i]*0.8 && low < 0  ) low = j+1;
         if ( value > mass[i]*1.2 && high < 0  ) high = j+1;
      }
      if ( high < 0 ) high = m12->GetNbinsX();
      signal[i] = m12->Integral(low,high);
      background[i] = m12d->Integral(low,high);
      significance[i] = signal[i]/TMath::Sqrt(background[i]);
      std::cout << mass[i] << "  " << signal[i] << "   " << background[i] << "  " << significance[i] << std::endl;
      
   }
   TCanvas * c1 = new TCanvas("c1","");
   
   TGraph * gr = new TGraph(8,mass,significance);
   gr -> SetTitle("Significance");
   gr -> SetMarkerStyle(20);
   gr -> GetXaxis()->SetTitle("mass (GeV)");
   gr -> GetYaxis()->SetTitle("significance");
   gr-> Draw("ALP");
   
   TFile * out = new TFile("significance_MMM.root","RECREATE");
   gr -> Write();
   
   
}
