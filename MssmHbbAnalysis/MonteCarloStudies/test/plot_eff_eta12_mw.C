void plot_eff_eta12_mw()
{
   TFile * fs[3];
   TFile * fb[3];
   TString selection = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   float mw[3]  = {300.,500.,700.};
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
      
   TH2F * h_eta[6];
   
   int color[6] = {2,6,4,2,6,4};
   
   for ( int i = 0; i < 3; ++i )
   {
      h_eta[i]    = (TH2F*) fs[i] -> Get("h_Jet0Jet1_Eta");
      h_eta[i]    -> SetName(Form("h_Jet0Jet1_Eta_S_%i",i));
      h_eta[i+3]  = (TH2F*) fb[i] -> Get("h_Jet0Jet1_Eta");
      h_eta[i+3]  -> SetName(Form("h_Jet0Jet1_Eta_B_%i",i));
   }
   
   float etacut[30];
   float eff[30];
   float sig[30];
   float sob[30];
   
   TGraph * g[6];
   
   float mineta = 1.2;
   float maxeta = 2.4;
   
   int j = 0;
   for ( int i = 0; i < 6; ++i )
   {
      j = 0;
      for ( int bin = h_eta[i]->GetNbinsX(); bin > 1; --bin )
      {
         float etax = h_eta[i] -> GetXaxis() -> GetBinLowEdge(bin) + h_eta[i] -> GetXaxis() -> GetBinWidth(bin);
         float etay = h_eta[i] -> GetYaxis() -> GetBinLowEdge(bin) + h_eta[i] -> GetYaxis() -> GetBinWidth(bin);;
         if ( etax < mineta || etay < mineta)  break;
         if ( etax > maxeta || etay > maxeta)  continue;
         etacut[j] = etax;
         eff[j] =  h_eta[i]->Integral(1,bin,1,bin)/h_eta[i]->Integral(1,h_eta[i]->GetNbinsX(),1,h_eta[i]->GetNbinsY());
         ++j;
      }
      g[i] = new TGraph(j,etacut,eff);
      g[i] -> SetLineWidth(2);
      g[i] -> SetLineColor(color[i]);
      if ( i >= 3 ) g[i] -> SetLineStyle(2);
   }
   
   
   TCanvas * c1 = new TCanvas("c1","",600,700);
//   c1 -> DrawFrame(100,0.5,150,1.1);
   c1->SetGrid();
   
   g[0] -> SetTitle(selection);
   g[0] -> GetXaxis() -> SetTitle("eta_1,2 cut");
   g[0] -> GetYaxis() -> SetTitle("efficiency");
   g[0] -> GetYaxis() -> SetRangeUser(0.,1.1);
   g[0] -> Draw("APC");
   g[1] -> Draw("PC");
   g[2] -> Draw("PC");
   g[3] -> Draw("PC");
   g[4] -> Draw("PC");
   g[5] -> Draw("PC");
   
   TLegend * leg = new TLegend(0.42,0.12,0.87,0.30);
//   leg->SetHeader("The Legend Title");
   for ( int i = 0; i < 3 ; ++i )
   {
      leg->AddEntry(g[i],m[i]+" - "+smw_leg[i],"l");
      leg->AddEntry(g[i+3],"QCD - "+smw_leg[i],"l");
   }
   leg->Draw();
   
   c1 -> SaveAs("figs/j12etacut_mw_eff-"+selection+".png");
   
   TCanvas * c2 = new TCanvas("c2","",600,700);
   c2 -> SetGrid();
   
   TGraph * gsig[3];
   TGraph * gsob[3];
   for ( int k = 0; k < 3 ; ++k )
   {
      double * ys = g[k]->GetY();
      double * xs = g[k]->GetX();
      double * yb = g[k+3]->GetY();
      
      for ( int i = 0; i<j; ++i )
      {
         sig[i] = ys[i]/TMath::Sqrt(yb[i]);
         sob[i] = ys[i]/yb[i];
         cout << xs[i] <<", "<< ys[i]/TMath::Sqrt(yb[i]) << " ,  " << ys[i] << " ,   " << yb[i] << endl;
      }
      gsig[k] = new TGraph(j,etacut,sig);
      gsob[k] = new TGraph(j,etacut,sob);
      gsig[k] -> SetLineWidth(2);
      gsig[k] -> SetLineColor(color[k]);
      gsob[k] -> SetLineColor(color[k]);
      gsob[k] -> SetLineStyle(2);
   }
   
   gsob[0] -> SetTitle(selection);
   gsob[0] -> GetYaxis() -> SetRangeUser(0.5,1.25);
   gsob[0] -> GetYaxis() -> SetTitle("S/#sqrt{B} (S/B)");
   gsob[0] -> GetXaxis() -> SetTitle("eta_1,2 cut");

   gsob[0] -> Draw("APC");
   gsob[1] -> Draw("PC");
   gsob[2] -> Draw("PC");
   gsig[0] -> Draw("PC");
   gsig[1] -> Draw("PC");
   gsig[2] -> Draw("PC");
   
   TLegend * leg2 = new TLegend(0.12,0.12,0.53,0.30);
   leg2 -> SetTextSize(0.028);
   leg2 ->SetHeader("mass window");
   leg2->AddEntry(g[0],m[0]+" ("+smw_leg[0]+")","l");
   leg2->AddEntry(g[1],m[1]+" ("+smw_leg[1]+")","l");
   leg2->AddEntry(g[2],m[2]+" ("+smw_leg[2]+")","l");
   leg2->Draw();

   c2 -> SaveAs("figs/j12etacut_mw_sig-"+selection+".png");
//    
}
