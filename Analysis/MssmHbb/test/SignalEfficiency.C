void SignalEfficiency()
{
   TFile * f[20];
   
   double eff1[20];
   double eff2[20];
   double mass[20];
   
   mass[0] = 300.;
   mass[1] = 400.;
   mass[2] = 500.;
   mass[3] = 700.;
   mass[4] = 900.;
   mass[5] = 1100.;
   mass[6] = 1300.;
   
   for ( int i = 0 ; i < 7; ++i )
   {
      f[i] = new TFile(Form("/nfs/dust/cms/user/walsh/cmsdas16/templates/cut_optimisation/CutOptimisation_test_lowM_SUSYGluGluToBBHToBB_M-%i_TuneCUETP8M1_13TeV-pythia8.root",int(mass[i])),"OLD");
      TH1D * m12_1 = (TH1D*)f[i]->Get("jet1_m12_d_csv>0.8_j2>0.8_j3>0.8");
      TH1D * m12_2 = (TH1D*)f[i]->Get("jet1_m12_d_csv>0.935_j2>0.935_j3>0.935");
      TH1D * genEvts = (TH1D*)f[i]->Get("TotalNumberOfGenEvents");
      double nGen = genEvts->GetMean();
      double nEvts = m12_1->GetEntries();
      eff1[i] = nEvts/nGen;
      nEvts = m12_2->GetEntries();
      eff2[i] = nEvts/nGen;
   }
   TCanvas * c1 = new TCanvas("c1","");
   
   
   TGraph * gr1 = new TGraph(7,mass,eff1);
   TGraph * gr2 = new TGraph(7,mass,eff2);
   gr2 -> SetLineColor(kRed);
   
   TMultiGraph * mg = new TMultiGraph();
   mg -> Add(gr1);
   mg -> Add(gr2);
   mg-> Draw("ALP");
   mg -> GetXaxis()->SetTitle("mass (GeV)");
   mg -> GetYaxis()->SetTitle("efficiency");
   mg-> Draw("ALP");
}
