void L1Rates()
{

   TFile * f = new TFile("../test/output.root","old");
   
   TH1F * h_nTrg = (TH1F*) f -> Get("h_lumiTrg");
   TH1F * h_nRefTrg = (TH1F*) f -> Get("h_lumiRefTrg");
      
   h_nTrg -> Rebin(6);
   h_nRefTrg -> Rebin(6);
   
   TGraphAsymmErrors * g_effTrg = new TGraphAsymmErrors(h_nTrg,h_nRefTrg,"cl=0.683 b(1,1) mode");
   
   int n = g_effTrg -> GetN();
   double * exh = g_effTrg -> GetEXhigh();
   double * exl = g_effTrg -> GetEXlow();
   double * eyh = g_effTrg -> GetEYhigh();
   double * eyl = g_effTrg -> GetEYlow();
   double * x   = g_effTrg -> GetX();
   double * y   = g_effTrg -> GetY();
   
   for ( int i = 0 ; i < n ; ++i )
   {
      y[i] = y[i]*24800000./x[i];
      eyh[i] = eyh[i]*24800000./x[i];
      eyl[i] = eyl[i]*24800000./x[i];
      exh[i] = 0.;
      exl[i] = 0.;
   }
   
   TGraphAsymmErrors * gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
   
   gr -> SetMarkerStyle(20);
   
   TCanvas * c1 = new TCanvas("c1","");
   gr -> Draw("AP");
   
   TF1 *fa1 = new TF1("fa1","0.35+0.0000036*x",5000,15000);
   fa1->Draw("same");
}
