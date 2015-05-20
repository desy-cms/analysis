void plot_m12()
{
   gStyle->SetOptStat(0);
   
   int type = 1;
   
   float mw[3] = {300.,500.,700.};
   float dmw[3] = {60.,120., 300.};
      
   TCanvas * c[4];
   TLegend * leg = new TLegend(0.3,0.75,0.9,0.9);
   leg -> SetTextSize(0.022);

   TString output;
   
   for ( int t = 0; t < 4 ; ++t )
   {
      type = t;
      TString stype[4] = {"m300","m600","m1100","qcd"};
      TString prefix = "signal/"+stype[type]+"/";
      if ( type == 3 )
         prefix = "background/hadd_";
      
      TString selection[3];
      selection[0] = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
      selection[1] = "jpt100_jpt100_jpt30_jeta1p7_jeta1p7_jeta2p4";
      selection[2] = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4_deta1p4";
      
      TFile * f[3];
      TH1F * h_m12[3];
      int color[3] = {1,2,4};
      
      for ( int i = 0; i < 3; ++i )
      {
         f[i] = new TFile(prefix+"genjets_"+selection[i]+".root");
         h_m12[i] = (TH1F*) f[i] -> Get("h_MassJet01");
         h_m12[i] -> SetName(Form("h_MassJet01_%i",i));
         
         h_m12[i] -> SetLineColor(color[i]);
         h_m12[i] -> SetLineWidth(2);
         h_m12[i] -> SetMarkerStyle(20);
         h_m12[i] -> SetMarkerColor(color[i]);
      }
      
      
      
      if ( type == 0 )
         h_m12[0] -> GetXaxis() -> SetRangeUser(150,500);
      else if ( type == 1 )
         h_m12[0] -> GetXaxis() -> SetRangeUser(150,1000);
      else if ( type == 2 )
         h_m12[0] -> GetXaxis() -> SetRangeUser(150,2000);
      else if ( type == 3 )
         h_m12[0] -> GetXaxis() -> SetRangeUser(150,2000);
         
      h_m12[0] -> SetTitle(stype[type]);
      h_m12[0] -> GetXaxis() -> SetTitle("m12 (GeV)");
      h_m12[0] -> GetYaxis() -> SetTitle("entries");
      
      c[t] = new TCanvas(Form("c%i",t),"",0,0,600,700);
      h_m12[0] -> GetYaxis() -> SetRangeUser(0,h_m12[0]->GetMaximum()*1.35);
      if ( type == 3 )
      {
         c[t] -> SetLogy();
         h_m12[0] -> GetYaxis() -> SetRangeUser(80,h_m12[0]->GetMaximum()*1.35);
      }
      h_m12[0] -> Draw("HIST");
      h_m12[1] -> Draw("HIST same");
      h_m12[2] -> Draw("HIST same");
      
   //   leg->SetHeader("The Legend Title");
      leg -> Clear();
      leg->AddEntry(h_m12[0],selection[0],"l");
      leg->AddEntry(h_m12[1],selection[1],"l");
      leg->AddEntry(h_m12[2],selection[2],"l");
      leg->Draw();
      
      output = "figs/M12_selections_deta_"+stype[t]+".png";
      c[t] -> SaveAs(output);
      
      std::cout << "Efficiency in a mass window - " << stype[t] << std::endl;
      
      for ( int i = 0; i < 3 ; ++i )
      {
         float min = mw[i] - dmw[i];
         float max = mw[i] + dmw[i];
         int bin0 = 0;
         int bin1 = 0;
         float m12min;
         float m12max;
         for ( int bin = 1; bin < h_m12[0]->GetNbinsX(); ++bin )
         {
            float m12 = h_m12[0] -> GetBinLowEdge(bin);
            if ( m12 == min ) 
            {
               m12min = m12;
               bin0 = bin; 
            }
            if ( m12 == max )
            {
               m12max = m12;
               bin1 = bin-1;
               break;
            }
         }
   //      cout << m12min << "   " << m12max << endl;
   //      cout << bin0 << "   " << bin1 << endl;
         float eff =  h_m12[1]->Integral(bin0,bin1)/h_m12[0]->Integral(bin0,bin1);
         cout << "Efficiency (" << mw[i] << " +- " << dmw[i] << ") = " << eff << endl;
      }
      
   }
   
}
