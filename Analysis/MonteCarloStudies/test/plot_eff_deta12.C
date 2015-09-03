void plot_eff_deta12()
{
   TFile * f[4];
   TString selection = "jpt100_jpt100_jpt30_jeta1p7_jeta1p7_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   for ( int i = 0; i < 3 ; ++i )
      f[i] = new TFile("signal/"+m[i]+"/genjets_"+selection+".root");
   f[3] = new TFile("background/hadd_genjets_"+selection+".root");
   
   TH1F * h_deta[4];
   int color[4] = {2,6,4,1};
   for ( int i = 0; i < 4; ++i )
   {
      h_deta[i] = (TH1F*) f[i] -> Get("h_dEta01");
      h_deta[i] -> SetName(Form("h_dEta01_%i",i));
   }
   
   float detacut[30];
   float eff[30];
   
   TGraph * g[4];
   
   float mindeta = 1.;
   float maxdeta = 3.;

   int j = 0;
   
   for ( int i = 0; i < 4; ++i )
   {
      j = 0;
      for ( int bin = h_deta[i]->GetNbinsX(); bin > 1; --bin )
      {
         float detax = h_deta[i] -> GetXaxis() -> GetBinLowEdge(bin) + h_deta[i] -> GetXaxis() -> GetBinWidth(bin);
         if ( detax < mindeta )  break;
         if ( detax > maxdeta )  continue;
         detacut[j] = detax;
         eff[j] =  h_deta[i]->Integral(1,bin)/h_deta[i]->Integral(1,h_deta[i]->GetNbinsX());
         ++j;
      }
      g[i] = new TGraph(j,detacut,eff);
      g[i] -> SetLineWidth(2);
      g[i] -> SetLineColor(color[i]);
   }
   
   
   TCanvas * c1 = new TCanvas("c1","",600,700);
//   c1 -> DrawFrame(100,0.5,150,1.1);
   c1->SetGrid();
   
   g[0] -> SetTitle(selection);
   g[0] -> GetXaxis() -> SetTitle("deta_12 cut");
   g[0] -> GetYaxis() -> SetTitle("efficiency");
   g[0] -> GetYaxis() -> SetRangeUser(0.,1.1);
   g[0] -> Draw("APC");
   g[1] -> Draw("PC");
   g[2] -> Draw("PC");
   g[3] -> Draw("PC");
   
   TLegend * leg = new TLegend(0.2,0.2,0.4,0.4);
//   leg->SetHeader("The Legend Title");
   leg->AddEntry(g[0],m[0],"l");
   leg->AddEntry(g[1],m[1],"l");
   leg->AddEntry(g[2],m[2],"l");
   leg->AddEntry(g[3],"QCD","l");
   leg->Draw();
   
   double * y0 = g[0]->GetY();
   double * x0 = g[0]->GetX();
   double * y3 = g[3]->GetY();
   for ( int i = 0; i<j; ++i )
   {
      cout << x0[i] <<", "<< y0[i]/TMath::Sqrt(y3[i]) << endl;
   }
   
   c1 -> SaveAs("figs/j12detacut_eff-"+selection+".png");
//    
}
