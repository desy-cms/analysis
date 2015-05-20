void plot_pt_eta_s_b()
{
   TString selection = "jpt100_jpt100_jpt30_jeta2p4_jeta2p4_jeta2p4";
   
   TString stype[4] = {"m300","m600","m1100","qcd"};
   
   TFile * f[4];
   for ( int i = 0; i < 4 ; ++i )
   {
      TString prefix = "signal/"+stype[i]+"/";
      if ( i == 3 )
         prefix = "background/hadd_";
      f[i] = new TFile(prefix+"genjets_"+selection+".root");
   }
   
   // Jets pT
   TH1F * h_pt[4];
   TH1F * h_eta[4];
   TH1F * h_deta[4];
   int color[4] = {2,3,4,1};
   
   TCanvas * c[7];
   TLegend * legPt = new TLegend(0.6,0.6,0.85,0.8);
   TLegend * legEta = new TLegend(0.33,0.15,0.67,0.35);
   TLegend * legDEta = new TLegend(0.55,0.6,0.85,0.8);

   legPt -> SetTextSize(0.034);
   legEta -> SetTextSize(0.034);
   legDEta -> SetTextSize(0.034);
   
   float maxbindeta;
   float maxbinpt;
   float maxbineta;

   TString output;
   
   for ( int r = 0; r < 3 ; ++r )
   {
      maxbinpt = -1;
      maxbineta = -1;
      if ( r == 0 ) maxbindeta = -1;
      for ( int i = 0; i < 4 ; ++i )
      {
         h_pt[i] = (TH1F*) f[i] -> Get(Form("h_PtJet_%i",r));
         h_pt[i] -> SetName(Form("h_pt_%i",i));
         h_pt[i] -> SetLineColor(color[i]);
         h_pt[i] -> SetLineWidth(2);
         h_pt[i] -> SetMarkerStyle(20);
         h_pt[i] -> SetMarkerColor(color[i]);
         
         h_eta[i] = (TH1F*) f[i] -> Get(Form("h_EtaJet_%i",r));
         h_eta[i] -> SetName(Form("h_eta_%i",i));
         h_eta[i] -> SetLineColor(color[i]);
         h_eta[i] -> SetLineWidth(2);
         h_eta[i] -> SetMarkerStyle(20);
         h_eta[i] -> SetMarkerColor(color[i]);
         
         if ( r == 0 )
         {
            h_deta[i] = (TH1F*) f[i] -> Get("h_dEta01");
            h_deta[i] -> SetName(Form("h_deta_%i",i));
            h_deta[i] -> SetLineColor(color[i]);
            h_deta[i] -> SetLineWidth(2);
            h_deta[i] -> SetMarkerStyle(20);
            h_deta[i] -> SetMarkerColor(color[i]);
            // Scale to unity area
            h_deta[i] -> Scale(1./h_deta[i]->Integral());
            if ( maxbindeta < h_deta[i] -> GetMaximum() ) maxbindeta = h_deta[i] -> GetMaximum();
         }
         
         // Scale to unity area
         h_pt[i] -> Scale(1./h_pt[i]->Integral());
         h_eta[i] -> Scale(1./h_eta[i]->Integral());
         
         if ( maxbinpt < h_pt[i] -> GetMaximum() ) maxbinpt = h_pt[i] -> GetMaximum();
         if ( maxbineta < h_eta[i] -> GetMaximum() ) maxbineta = h_eta[i] -> GetMaximum();
      }
      
      // PT plots
      h_pt[0] -> SetTitle(selection);
      h_pt[0] -> GetXaxis() -> SetRangeUser(80.,500.);
      if ( r == 2 )  h_pt[0] -> GetXaxis() -> SetRangeUser(20.,200.);
      h_pt[0] -> GetYaxis() -> SetRangeUser(0.,maxbinpt*1.1);
      h_pt[0] -> GetXaxis() -> SetTitle(Form("pT_Jet%i (GeV)",r+1));
      h_pt[0] -> GetYaxis() -> SetTitle("entries");
      h_pt[0] -> SetStats(0);
         
      c[r] = new TCanvas(Form("c%i",r),"",0,0,500,650);
      h_pt[0] -> Draw("HIST");
      h_pt[1] -> Draw("HIST same");
      h_pt[2] -> Draw("HIST same");
      h_pt[3] -> Draw("HIST same");
      legPt -> Clear();
      legPt -> SetHeader(Form("pT of jet %i",r+1));
      legPt->AddEntry(h_pt[0],stype[0],"l");
      legPt->AddEntry(h_pt[1],stype[1],"l");
      legPt->AddEntry(h_pt[2],stype[2],"l");
      legPt->AddEntry(h_pt[3],stype[3],"l");
      legPt->Draw();
      output = "figs/pT_Jet"+TString::Itoa(r+1,10)+"_s-b_"+selection+".png";
      c[r] -> SaveAs(output);
      
      // Eta plots
      h_eta[0] -> SetTitle(selection);
      h_eta[0] -> GetXaxis() -> SetRangeUser(3.,3.);
      h_eta[0] -> GetYaxis() -> SetRangeUser(0.,maxbineta*1.1);
      h_eta[0] -> GetXaxis() -> SetTitle(Form("Eta_Jet%i",r+1));
      h_eta[0] -> GetYaxis() -> SetTitle("entries");
      h_eta[0] -> SetStats(0);
         
      c[r+3] = new TCanvas(Form("c%i",r+3),"",0,0,500,650);
      h_eta[0] -> Draw("HIST");
      h_eta[1] -> Draw("HIST same");
      h_eta[2] -> Draw("HIST same");
      h_eta[3] -> Draw("HIST same");
      legEta -> Clear();
      legEta -> SetHeader(Form("pseudorapidity of jet %i",r+1));
      legEta->AddEntry(h_eta[0],stype[0],"l");
      legEta->AddEntry(h_eta[1],stype[1],"l");
      legEta->AddEntry(h_eta[2],stype[2],"l");
      legEta->AddEntry(h_eta[3],stype[3],"l");
      legEta->Draw();
      output = "figs/Eta_Jet"+TString::Itoa(r+1,10)+"_s-b_"+selection+".png";
      c[r+3] -> SaveAs(output);
      
      // dEta plots
      if ( r == 0 ) 
      {
         h_deta[0] -> SetTitle(selection);
         h_deta[0] -> GetXaxis() -> SetRangeUser(0.,6.);
         h_deta[0] -> GetYaxis() -> SetRangeUser(0.,maxbindeta*1.1);
         h_deta[0] -> GetXaxis() -> SetTitle("dEta_Jet12");
         h_deta[0] -> GetYaxis() -> SetTitle("entries");
         h_deta[0] -> SetStats(0);
            
         c[6] = new TCanvas("c6","",0,0,500,650);
         h_deta[0] -> Draw("HIST");
         h_deta[1] -> Draw("HIST same");
         h_deta[2] -> Draw("HIST same");
         h_deta[3] -> Draw("HIST same");
         legDEta -> Clear();
         legDEta -> SetHeader("|eta_jet1 - eta_jet2|");
         legDEta->AddEntry(h_deta[0],stype[0],"l");
         legDEta->AddEntry(h_deta[1],stype[1],"l");
         legDEta->AddEntry(h_deta[2],stype[2],"l");
         legDEta->AddEntry(h_deta[3],stype[3],"l");
         legDEta->Draw();
         output = "figs/DeltaEta_Jet12_s-b_"+selection+".png";
         c[6] -> SaveAs(output);
      }
   }
   
   //
   
}
