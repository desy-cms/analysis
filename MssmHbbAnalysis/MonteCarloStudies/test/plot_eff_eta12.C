void plot_eff_eta12()
{
   TFile * f[4];
   TString selection = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   for ( int i = 0; i < 3 ; ++i )
      f[i] = new TFile("signal/"+m[i]+"/genjets_"+selection+".root");
   f[3] = new TFile("background/hadd_genjets_"+selection+".root");
   
   TH2F * h_eta[4];
   int color[4] = {2,6,4,1};
   for ( int i = 0; i < 4; ++i )
   {
      h_eta[i] = (TH2F*) f[i] -> Get("h_Jet0Jet1_Eta");
      h_eta[i] -> SetName(Form("h_Jet0Jet1_Eta_%i",i));
   }
   
   float etacut[30];
   float eff[30];
   
   TGraph * g[4];
   
   float mineta = 1.2;
   float maxeta = 2.4;
   
   int j = 0;
   
   for ( int i = 0; i < 4; ++i )
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
   
   TLegend * leg = new TLegend(0.42,0.12,0.87,0.3);
//   leg->SetHeader("The Legend Title");
   leg->AddEntry(g[0],m[0],"l");
   leg->AddEntry(g[1],m[1],"l");
   leg->AddEntry(g[2],m[2],"l");
   leg->AddEntry(g[3],"QCD","l");
   leg->Draw();
   
   double * y0 = g[1]->GetY();
   double * x0 = g[1]->GetX();
   double * y3 = g[3]->GetY();
   for ( int i = 0; i<j; ++i )
   {
      cout << x0[i] <<", "<< y0[i]/TMath::Sqrt(y3[i]) << endl;
   }
   
   c1 -> SaveAs("figs/j12etacut_eff-"+selection+".png");
//    
}
