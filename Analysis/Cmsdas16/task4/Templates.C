void Templates()
{
   gStyle -> SetOptStat(0);
   TFile * f = new TFile("/nfs/dust/cms/user/walsh/cmsdas16/templates/combine_tool/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root","OLD");
   
   TH1D * m12 = (TH1D*) f -> Get("bbH_Mbb");
   m12 -> SetName("m12");
   m12 -> SetLineWidth(2);
   m12 -> SetMarkerStyle(20);
   m12 -> SetLineColor(kBlack);
   
   TH1D * m12_up = (TH1D*) f -> Get("bbH_Mbb_CMS_SFb_13TeVUp");
   m12_up -> SetName("m12_up");
   m12_up -> SetLineWidth(2);
   m12_up -> SetLineColor(kRed);
   m12_up -> GetXaxis()->SetTitle("m12 (GeV)");
   m12_up -> GetYaxis()->SetTitle("entries");
   
   TH1D * m12_down = (TH1D*) f -> Get("bbH_Mbb_CMS_SFb_13TeVDown");
   m12_down -> SetName("m12_down");
   m12_down -> SetLineWidth(2);
   m12_down -> SetLineColor(kBlue);
   
   TCanvas * c1 = new TCanvas("c1","");
    
   m12_up -> Draw("hist");
   m12 -> Draw("histsame");
   m12_down -> Draw("histsame");

   TLegend * leg = new TLegend(0.4,0.70,0.9,0.9); 
   leg->AddEntry("m12","nominal","l");
   leg->AddEntry("m12_up","systematics 2-sigma up","l");
   leg->AddEntry("m12_down","systematics 2-sigma down","l");
   leg -> Draw();
}
