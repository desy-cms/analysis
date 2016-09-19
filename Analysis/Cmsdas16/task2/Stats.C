float crossSection(const int &, const double &);

void Stats()
{
   double significance[20];
   double background[20];
   double signal[20];
   double eff[20];
   double ul95[20];
   double mass[20];
   double xs[20];
   
   double lumi = 2690.496;
   double ful = 1.96;
   double fp = 1; 
   double tanb = 60;
   
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
      f[i] = new TFile(Form("selection_bbb_MMM/mssmhbb_%i_analysis_histograms.root",(int)round(mass[i])),"OLD");
      TH1F * m12 = (TH1F*)f[i]->Get("m12");
      double nGen = ((TH1F*)f[i]->Get("nGen"))->GetBinContent(1);
      double nEvts = m12->GetEffectiveEntries();

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
      eff[i] = nEvts/nGen;                                         // signal efficiency
      significance[i] = signal[i]/TMath::Sqrt(background[i]);      // significance
      ul95[i] = fp*ful*TMath::Sqrt(background[i])/(eff[i]*lumi);   // uppper limit
      xs[i] = crossSection((int)round(mass[i]),tanb);
   }
   
   
   TCanvas * c1 = new TCanvas("c1","");
   TGraph * gr1 = new TGraph(8,mass,eff);
   gr1 -> SetName("Efficiency");
   gr1 -> SetTitle("CMS (in preparation)");
   gr1 -> SetMarkerStyle(20);
   gr1 -> GetXaxis()->SetTitle("mass (GeV)");
   gr1 -> GetYaxis()->SetTitle("Efficiency");
   gr1 -> Draw("ALP");
   
   TCanvas * c2 = new TCanvas("c2","");
   TGraph * gr2 = new TGraph(8,mass,significance);
   gr2 -> SetName("Significance");
   gr2 -> SetTitle("CMS (in preparation)");
   gr2 -> SetMarkerStyle(20);
   gr2 -> GetXaxis()->SetTitle("mass (GeV)");
   gr2 -> GetYaxis()->SetTitle("Significance");
   gr2 -> Draw("ALP");
   
   
   TCanvas * c3 = new TCanvas("c3","");
   TGraph * gr3 = new TGraph(8,mass,ul95);
   gr3 -> SetName("UpperLimits95");
   gr3 -> SetTitle("CMS (in preparation)");
   gr3 -> SetMarkerStyle(20);
   gr3 -> GetXaxis()->SetTitle("mass (GeV)");
   gr3 -> GetYaxis()->SetTitle("upper limit x-section*BR (pb) 95%CL");
   gr3 -> Draw("ALP");
   
   TGraph * gr4 = new TGraph(8,mass,xs);
   gr4 -> SetName("XSection");
   gr4 -> SetTitle("");
   gr4 -> SetMarkerStyle(20);
   gr4 -> SetMarkerColor(kRed);
   gr4 -> SetLineColor(kRed);
   gr4 -> GetXaxis()->SetTitle("mass (GeV)");
   gr4 -> GetYaxis()->SetTitle("x-section*BR (pb)");
   
   TMultiGraph * mg = new TMultiGraph();
   mg -> Add(gr3);
   mg -> Add(gr4);
   mg -> Draw("ALP");
   mg -> GetXaxis()->SetTitle("mass (GeV)");
   mg -> GetYaxis()->SetTitle("x-section*BR (pb)");
   mg -> GetYaxis()->SetRangeUser(0.001,100);
   c3->SetLogy();
   mg -> SetTitle("CMS (in preparation)");
   mg -> Draw("ALP");
   
   TLegend * leg = new TLegend(0.4,0.75,0.9,0.9); 
   leg->AddEntry("UpperLimits95","upper limit x-section*BR 95%CL","p");
   leg->AddEntry("XSection","MSSM x-section*BR tan beta 60 mhmod+","p");
   leg -> Draw();
   
   TFile * out = new TFile("Stats_MMM.root","RECREATE");
   gr1 -> Write();
   gr2 -> Write();
   gr3 -> Write();
   gr4 -> Write();
   
   
}


float crossSection(const  int & mass, const double & tanb)  // in pb
{
   // values for tan beta =20
   std::map<int,double> xs;
   xs[100] =  365.04 * 0.889297        +   5.21588 * 0.829672      ;
   xs[120] =  207.345 * 0.886169       +   41.7379 * 0.88271       ;
   xs[160] =  79.5406 * 0.88124        +   75.3253 * 0.879037      ;
   xs[200] =  36.1034 * 0.857523       +   35.4616 * 0.869342      ;
   xs[250] =  15.8387 * 0.811031       +   15.6594 * 0.85409       ;
   xs[300] =  7.7196 * 0.705892        +   7.65395 * 0.809695      ;
   xs[350] =  4.14873 * 0.564904       +   4.13288 * 0.708487      ;
   xs[400] =  2.36367 * 0.515287       +   2.36207 * 0.624898      ;
   xs[500] =  0.887828 * 0.435839      +   0.889008 * 0.415354     ;
   xs[600] =  0.380582 * 0.350134      +   0.381322 * 0.346045     ;
   xs[700] =  0.180691 * 0.311139      +   0.181021 * 0.309895     ;
   xs[900] =  0.0497564 * 0.27335      +   0.0497311 * 0.272931    ;
   xs[1100] =  0.0164384 * 0.254142    +   0.0164648 * 0.253845    ;
   xs[1300] =  0.00616693 * 0.242023   +   0.00617024 * 0.241754   ;

   
   // scale for tan beta (approx)
   float sf = (tanb/20)*(tanb/20);
   return xs[mass]*sf;
}
