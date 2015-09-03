void plot_eff_pt12_2d()
{
   TFile * f[4];
   TString selection = "jpt30_jpt30_jpt30_jeta2p4_jeta2p4_jeta2p4";
   TString m[3] = {"m300", "m600", "m1100"};
   
   for ( int i = 0; i < 3 ; ++i )
      f[i] = new TFile("signal/"+m[i]+"/genjets_"+selection+".root");
   f[3] = new TFile("background/hadd_genjets_"+selection+".root");
   
   TH2F * h_pt[4];
   
   int color[4] = {2,6,4,1};
   
   for ( int i = 0; i < 4; ++i )
   {
      h_pt[i]  = (TH2F*) f[i] -> Get("h_Jet0Jet1_Pt");
      h_pt[i]  -> SetName(Form("h_Jet0Jet1_Pt_%i",i));
   }
   
   double ptxcut[50000];
   double ptycut[50000];
   double eff[50000];
   
   TH2F * hg[6];
   TGraph2D * g[4];
   
   float minpt = 50.;
   float maxpt = 150.;
   int step = 5;
   int nbins = int((maxpt-minpt)/step)+1;
   
   int j = 0;
   for ( int i = 0; i < 4; ++i )
   {
      if ( i!=3 ) continue; 
      j = 0;
      hg[i] = new TH2F(Form("hg_%i",i),"",nbins,minpt,maxpt,nbins,minpt,maxpt);
      float integral = h_pt[i]->Integral(1,h_pt[i]->GetNbinsX(),1,h_pt[i]->GetNbinsY());
      for ( int binx = 1; binx < h_pt[i]->GetNbinsX(); binx += step )
      {
         float ptx = h_pt[i] -> GetXaxis() -> GetBinLowEdge(binx);
         if ( ptx < minpt ) continue;
         if ( ptx > maxpt ) break;
         for ( int biny = 1; biny < h_pt[i]->GetNbinsY(); biny += step )
         {
            float pty = h_pt[i] -> GetYaxis() -> GetBinLowEdge(biny);
            if ( pty < minpt)  continue;
            if ( pty > maxpt)  break;
            ptxcut[j] = ptx;
            ptycut[j] = pty;
            eff[j] = 0.;
            if ( ptxcut[j] >= ptycut[j] )
            eff[j] =  h_pt[i]->Integral(binx,h_pt[i]->GetNbinsX(),biny,h_pt[i]->GetNbinsY())/integral;
            hg[i]->Fill(ptxcut[j],ptycut[j],eff[j]);
            ++j;
         }
      }
      g[i] = new TGraph2D(Form("g_%i",i),"",j,ptxcut,ptycut,eff);
      g[i] -> SetLineWidth(2);
      g[i] -> SetLineColor(color[i]);
   }
   
   
   TCanvas * c1 = new TCanvas("c1","",600,700);
//   c1 -> DrawFrame(100,0.5,150,1.1);
   c1->SetGrid();
   hg[3] -> SetContour(50);
   hg[3] -> Draw("CONT4Z");
   
//    g[0] -> SetTitle(selection);
//    g[0] -> GetXaxis() -> SetTitle("pt_1,2 cut (GeV)");
//    g[0] -> GetYaxis() -> SetTitle("efficiency");
//    g[0] -> GetYaxis() -> SetRangeUser(0.,1.1);
//    g[0] -> Draw("APC");
//    g[1] -> Draw("PC");
//    g[2] -> Draw("PC");
//    g[3] -> Draw("PC");
//    
//    TLegend * leg = new TLegend(0.15,0.15,0.35,0.35);
// //   leg->SetHeader("The Legend Title");
//    leg->AddEntry(g[0],m[0],"l");
//    leg->AddEntry(g[1],m[1],"l");
//    leg->AddEntry(g[2],m[2],"l");
//    leg->AddEntry(g[3],"QCD","l");
//    leg->Draw();
   
//    int mp = 0;
//    double * ys = g[mp]->GetY();
//    double * xs = g[mp]->GetX();
//    double * yb = g[mp+3]->GetY();
//    for ( int i = 0; i<j; ++i )
//    {
//       cout << xs[i] <<", "<< ys[i]/TMath::Sqrt(yb[i]) << " ,  " << ys[i] << " ,   " << yb[i] << endl;
//    }
//    
//    c1 -> SaveAs("figs/j12ptcut_eff-"+selection+".png");
//    
}
