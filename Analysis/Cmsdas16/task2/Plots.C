void Plots()
{
   double mass[20];
   mass[0] = 300.;
   mass[1] = 700.;
   mass[2] = 1300.;
   
   double scale[20];
   scale[0] = 10;
   scale[1] = 1000;
   scale[2] = 100000;
   
   // data in the control region
   TFile * fd = new TFile("selection_bbnb_MMM/data_analysis_histograms.root","OLD");
   TH1F * m12d = (TH1F*) fd -> Get("m12");
   m12d -> SetName("m12d");
   m12d -> SetLineWidth(2);
   m12d -> SetMarkerStyle(20);
   m12d -> SetMarkerColor(kBlack);
   m12d -> GetXaxis() -> SetTitle("m12 (GeV)");
   m12d -> GetYaxis() -> SetTitle("entries");
   m12d -> SetStats(0);
   
   TH1F * pt0d = (TH1F*) fd -> Get("pt_jet0");
   pt0d -> SetName("pt0d");
   pt0d -> SetLineWidth(2);
   pt0d -> SetMarkerStyle(20);
   pt0d -> SetMarkerColor(kBlack);
   pt0d -> GetXaxis() -> SetTitle("leading jet pT (GeV)");
   pt0d -> GetYaxis() -> SetTitle("entries");
   pt0d -> SetStats(0);
   
   TH1F * btag0d = (TH1F*) fd -> Get("btag_jet0");
   btag0d -> SetName("btag0d");
   btag0d -> SetLineWidth(2);
   btag0d -> SetMarkerStyle(20);
   btag0d -> SetMarkerColor(kBlack);
   btag0d -> GetXaxis() -> SetTitle("leading jet CSVv2 btag");
   btag0d -> GetYaxis() -> SetTitle("entries");
   btag0d -> SetStats(0);
   btag0d -> GetXaxis() -> SetRangeUser(0.8,1);
         
         
   TCanvas * c1 = new TCanvas("c1","");
   m12d -> SetTitle("CMS (in progress)");
   m12d->Draw();
   
   TCanvas * c2 = new TCanvas("c2","");
   pt0d -> SetTitle("CMS (in progress)");
   pt0d->Draw();
   
   TCanvas * c3 = new TCanvas("c3","");
   btag0d -> SetTitle("CMS (in progress)");
   btag0d->Draw();
   
   // Signal 
   TFile * fs[20];
   TH1F * m12[20];
   TH1F * pt0[20];
   TH1F * btag0[20];
   TLegend * leg = new TLegend(0.4,0.50,0.9,0.7); 
   leg->AddEntry("m12d","background (data-driven)","p");
   for ( int i = 0; i < 3; ++i )
   {
      // M12
      fs[i] = new TFile(Form("selection_bbb_MMM/mssmhbb_%i_analysis_histograms.root",(int)round(mass[i])),"OLD");
      c1 -> cd();
      m12[i] = (TH1F*) fs[i] -> Get("m12");
      m12[i] -> SetName(Form("m12_%i",i));
      m12[i] -> Scale(scale[i]);
      m12[i] -> SetLineColor(i+2);
      m12[i] -> SetLineWidth(2);
      m12[i] -> Draw("samehist");
      leg->AddEntry(Form("m12_%i",i),Form("signal M=%i (GeV) x %i",(int)round(mass[i]), (int)round(scale[i])),"l");
      
      // Pt leading jet
      c2 -> cd();
      pt0[i] = (TH1F*) fs[i] -> Get("pt_jet0");
      pt0[i] -> SetName(Form("pt0_%i",i));
      pt0[i] -> Scale(scale[i]);
      pt0[i] -> SetLineColor(i+2);
      pt0[i] -> SetLineWidth(2);
      pt0[i] -> Draw("samehist");

      // btag leading jet
      c3 -> cd();
      btag0[i] = (TH1F*) fs[i] -> Get("btag_jet0");
      btag0[i] -> SetName(Form("btag0_%i",i));
      btag0[i] -> Scale(scale[i]);
      btag0[i] -> SetLineColor(i+2);
      btag0[i] -> SetLineWidth(2);
      btag0[i] -> Draw("samehist");
      
   }
   c1 -> cd();
   leg -> Draw();
   c2 -> cd();
   leg -> Draw();
   c3 -> cd();
   TLegend * leg3 = (TLegend*)leg ->Clone();
   leg3 -> SetX1(0.1);
   leg3 -> SetX2(0.6);
   leg3 -> Draw();
}
