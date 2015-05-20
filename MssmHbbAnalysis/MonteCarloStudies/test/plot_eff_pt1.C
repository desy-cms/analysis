void plot_eff_pt1()
{
   TFile * f[4];
   TString selection = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   for ( int i = 0; i < 3 ; ++i )
      f[i] = new TFile("signal/"+m[i]+"/genjets_"+selection+".root");
   f[3] = new TFile("background/hadd_genjets_"+selection+".root");
   
   TH1F * h_pt[4];
   int color[4] = {2,6,4,1};
   for ( int i = 0; i < 4; ++i )
   {
      h_pt[i] = (TH1F*) f[i] -> Get("h_PtJet_0");
      h_pt[i] -> SetName(Form("h_PtJet1_%i",i));
   }
   
   float ptcut[100];
   float eff[100];
   
   TGraph * g[4];
   
   float minpt = 100.;
   float maxpt = 165.;
   
   int j = 0;
   
   for ( int i = 0; i < 4; ++i )
   {
      j = 0;
      for ( int bin = 1; bin < h_pt[i]->GetNbinsX(); ++bin )
      {
         float pt = h_pt[i] -> GetBinLowEdge(bin);
         if ( pt < minpt ) continue;
         if ( pt > maxpt ) break;
         ptcut[j] = pt;
         eff[j] = h_pt[i]->Integral(bin,h_pt[i]->GetNbinsX())/h_pt[i]->Integral(1,h_pt[i]->GetNbinsX());
         ++j;
      }
      g[i] = new TGraph(j,ptcut,eff);
      g[i] -> SetLineWidth(2);
      g[i] -> SetLineColor(color[i]);
   }
   
   TCanvas * c1 = new TCanvas("c1","",600,700);
   c1 -> DrawFrame(100,0.5,150,1.1);
   c1->SetGrid();
   g[0] -> SetTitle(selection);
   g[0] -> GetXaxis() -> SetTitle("pT_1 cut (GeV)");
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
   
   c1 -> SaveAs("figs/j1ptcut_eff-"+selection+".png");
   
   
//    for ( int k = 0; k < 3 ; ++k )
//    {
//       double * ys = g[k]->GetY();
//       double * xs = g[k]->GetX();
//       double * yb = g[k+3]->GetY();
//       
//       for ( int i = 0; i<j; ++i )
//       {
//          cout << xs[i] <<", "<< ys[i]/TMath::Sqrt(yb[i]) << " ,  " << ys[i] << " ,   " << yb[i] << endl;
//       }
//    }
   
   
}
