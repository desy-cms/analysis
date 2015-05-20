void plot_eff_presel()
{
   TFile * fs[3][3];
   TFile * fb[3];
   
   TString m[3] = {"m300", "m600", "m1100"};
   
   int color[4] = {2,3,4,1};
   
   for ( int i = 0 ; i < 3 ; ++i )
   {
      fs[i][0] =  new TFile("signal/"+m[i]+"/genjets_jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
      fs[i][1] =  new TFile("signal/"+m[i]+"/genjets_jpt110_jpt110_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
      fs[i][2] =  new TFile("signal/"+m[i]+"/genjets_jpt120_jpt120_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   }
   
   fb[0] = new TFile("background/hadd_genjets_jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   fb[1] = new TFile("background/hadd_genjets_jpt110_jpt110_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   fb[2] = new TFile("background/hadd_genjets_jpt120_jpt120_jpt30_jeta2p4_jeta2p4_jeta2p4.root");
   
   float ptcut[3] = {100,110,120};
   float effb[3];
   float effs[3][3];
   
   TH1F * h_pt;
   float nref;
   
   // efficiency background
   for ( int j = 0; j < 3 ; ++j )
   {
      h_pt = (TH1F*) fb[j] -> Get("h_PtJet_0");
      if ( j == 0 ) nref =  h_pt->Integral();
      float n = h_pt->Integral();
      effb[j] = n/nref;
   }
   
   // efficiency signals
   for ( int i = 0; i < 3 ; ++i )
   {
      for ( int j = 0; j < 3 ; ++j )
      {
         h_pt = (TH1F*) fs[i][j] -> Get("h_PtJet_0");
         if ( j == 0 ) nref =  h_pt->Integral();
         float n = h_pt->Integral();
         effs[i][j] = n/nref;
      }
   }
      
   
   TGraph * gb = new TGraph(3,ptcut,effb);
   TGraph * gs[3];
   
   for ( int i = 0; i < 3 ; ++i )
   {
      gs[i] = new TGraph(3,ptcut,effs[i]);
      gs[i] -> SetLineWidth(2);
      gs[i] -> SetLineColor(color[i]);
   }
   gb -> SetLineWidth(2);
   gb -> SetLineColor(color[3]);
   
   TCanvas * c1 = new TCanvas("c1","",600,700);
//   c1 -> DrawFrame(90,0.4,140,1.1);
   
   gs[0] -> SetTitle("preselection");
   gs[0] -> GetXaxis() -> SetTitle("pT_1,2 cut (GeV)");
   gs[0] -> GetYaxis() -> SetTitle("efficiency");
   gs[0] -> GetYaxis() -> SetRangeUser(0.,1.1);
   gs[0] -> Draw("APC");
   gs[1] -> Draw("PC");
   gs[2] -> Draw("PC");
   gb -> Draw("PC");

   TLegend * leg = new TLegend(0.2,0.2,0.4,0.4);
//   leg->SetHeader("The Legend Title");
   leg->AddEntry(gs[0],m[0],"l");
   leg->AddEntry(gs[1],m[1],"l");
   leg->AddEntry(gs[2],m[2],"l");
   leg->AddEntry(gb,"QCD","l");
   leg->Draw();
    
    double * y0 = gs[0]->GetY();
    double * x0 = gs[0]->GetX();
    double * y3 = gb->GetY();
    for ( int i = 0; i<gs[0]->GetN(); ++i )
    {
       cout << x0[i] <<", "<< y0[i]-y3[i] << endl;
    }
   
   c1->SaveAs("figs/j12ptcut_eff-presel.png");
   
}
