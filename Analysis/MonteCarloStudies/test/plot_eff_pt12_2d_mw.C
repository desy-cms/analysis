void plot_eff_pt12_2d_mw()
{
   TFile * fs[3];
   TFile * fb[3];
   TString selection = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   float mw[3] = {300.,500.,700.};
   float dmw[3] = {60.,120., 300.};
   
   TString smw[3];
   TString smw_leg[3];
   
   for ( int i = 0; i < 3 ; ++i )
   {
      smw_leg[i] = TString::Itoa(int(mw[i]),10) + " #pm " + TString::Itoa(int(dmw[i]),10) + " GeV";
      smw[i] = "_m12w" + TString::Itoa(int(mw[i]),10) + "pm" + TString::Itoa(int(dmw[i]),10);
      fs[i] = new TFile("signal/"+m[i]+"/genjets_"+selection+smw[i]+".root");
      fb[i] = new TFile("background/hadd_genjets_"+selection+smw[i]+".root");
   }
   
   TH2F * h_pt[6];
   
   int color[6] = {2,6,4,2,6,4};
   
   for ( int i = 0; i < 3; ++i )
   {
      h_pt[i]    = (TH2F*) fs[i] -> Get("h_Jet0Jet1_Pt");
      h_pt[i]    -> SetName(Form("h_Jet0Jet1_Pt_S_%i",i));
      h_pt[i+3]  = (TH2F*) fb[i] -> Get("h_Jet0Jet1_Pt");
      h_pt[i+3]  -> SetName(Form("h_Jet0Jet1_Pt_B_%i",i));
   }
   
   double ptxcut[3][5000];
   double ptycut[3][5000];
   double eff[5000];
   double sig[5000];
   double sob[5000];
   
   TGraph2D * g[6];
   TH2F * hg[6];
   TCanvas * c1[6];
   
   float minpt[3] = {100.,180.,200.};
   float maxpt[3] = {140.,280.,400.};
   int step[3] = {2,2,5};
   int nbins[3];
   for ( int i = 0 ; i < 3; ++i)
      nbins[i] =  int((maxpt[i]-minpt[i])/step[i])+1;
   
   int j = 0;
   int index = -1;
   for ( int i = 0; i < 6; ++i )
   {
      index = i;
      if ( i > 2 ) index = i-3;
      if ( index > 2 ) continue;
      j = 0;
      hg[i] = new TH2F(Form("hg_%i",i),"",nbins[index],minpt[index],maxpt[index],nbins[index],minpt[index],maxpt[index]);
      float integral = h_pt[i]->Integral(1,h_pt[i]->GetNbinsX(),1,h_pt[i]->GetNbinsY());
      for ( int binx = 1; binx < h_pt[i]->GetNbinsX(); binx+=step[index] )
      {
         float ptx = h_pt[i] -> GetXaxis() -> GetBinLowEdge(binx);
         if ( ptx < minpt[index] ) continue;
         if ( ptx > maxpt[index] ) break;
         for ( int biny = 1; biny < h_pt[i]->GetNbinsY(); biny+=step[index] )
         {
            float pty = h_pt[i] -> GetYaxis() -> GetBinLowEdge(biny);
            if ( pty < minpt[index])  continue;
            if ( pty > maxpt[index])  break;
            ptxcut[index][j] = ptx;
            ptycut[index][j] = pty;
            eff[j] = 0.;
            if ( ptxcut[index][j] >= ptycut[index][j] )
               eff[j] =  h_pt[i]->Integral(binx,h_pt[i]->GetNbinsX(),biny,h_pt[i]->GetNbinsY())/integral;
            hg[i]->Fill(ptxcut[index][j],ptycut[index][j],eff[j]);
            ++j;
         }
      }
      cout << j << ", " << nbins[index] << endl;
      g[i] = new TGraph2D(Form("g_%i",i),"",j,ptxcut[index],ptycut[index],eff);
//       if ( i < 3 )
//       {
//          c1[i] =  new TCanvas(Form("c1_%i",i),"",600,700);
//          c1[i] -> SetGrid();
//          hg[i] -> SetContour(50);
//          hg[i] -> Draw("CONT4Z");
//       }

   }
   
    TCanvas * c2[3];
//    TCanvas * c2 = new TCanvas("c2","",600,700);
//    c2 -> SetGrid();
//    
   TH2F * hgsig[3];
   for ( int i = 0; i < 3 ; ++i )
   {
      if ( i > 2 ) continue;
      c2[i] = new TCanvas(Form("c2_%i",i),"",600,700);
      c2[i] -> SetGrid();
      c2[i]  -> SetBottomMargin(0.10);
      c2[i]  -> SetTopMargin   (0.10);  
      c2[i]  -> SetRightMargin (0.15);
      c2[i]  -> SetLeftMargin  (0.12);
      hgsig[i] = new TH2F(Form("hgsig_%i",i),"",nbins[i],minpt[i],maxpt[i],nbins[i],minpt[i],maxpt[i]);
      hgsig[i] -> SetStats(0);
      hgsig[i] -> SetTitle(m[i]);   
      hgsig[i] -> GetXaxis() -> SetTitle("pT_1 cut (GeV)");   
      hgsig[i] -> GetYaxis() -> SetTitle("pT_2 cut (GeV)");   
      hgsig[i] -> GetZaxis() -> SetTitle("relative gain sifnificance");  
      hgsig[i] -> GetXaxis() -> SetTitleOffset(1.2);   
      hgsig[i] -> GetYaxis() -> SetTitleOffset(1.4);   
      hgsig[i] -> GetZaxis() -> SetTitleOffset(1.4);  
      
      
      double * xs = g[i]->GetX();
      double * ys = g[i]->GetY();
      double * zs = g[i]->GetZ();
      double * xb = g[i+3]->GetX();
      double * yb = g[i+3]->GetY();
      double * zb = g[i+3]->GetZ();
      
      for ( int k = 0; k<nbins[i]*nbins[i]; ++k )
      {
         sig[k] = 0.;
         sob[k] = 0.;
         if ( zb[k] != 0 ) 
         {
            sig[k] = zs[k]/TMath::Sqrt(zb[k]);
            sob[k] = zs[k]/zb[k];
         }
         hgsig[i]->Fill(ptxcut[i][k],ptycut[i][k],sig[k]);
      }
      if ( i == 0 )
      {
         hgsig[i] -> SetContour(30);
         hgsig[i] -> GetZaxis() -> SetRangeUser(1.0,2.0);
         hgsig[i] -> GetZaxis() -> SetRangeUser(0.8,1.03);
      }
      if ( i == 1 )
      {
         hgsig[i] -> SetContour(27);
         hgsig[i] -> SetContour(30);
         hgsig[i] -> GetZaxis() -> SetRangeUser(1.7,3.05);
         hgsig[i] -> GetZaxis() -> SetRangeUser(1.3,1.8);
      }
      if ( i == 2 )
      {
         hgsig[i] -> SetContour(30);
         hgsig[i] -> SetContour(31);
         hgsig[i] -> GetZaxis() -> SetRangeUser(2.5,4.7);
         hgsig[i] -> GetZaxis() -> SetRangeUser(1.6,2.9);
      }
      hgsig[i] -> Draw("CONT4Z");
      
      c2[i] -> SaveAs("figs/j12ptcut_mw_"+m[i]+"_sig-"+selection+".png");
      
   }
   
   
//    
}
