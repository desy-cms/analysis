#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *drawRatio(TH1F *,TH1F *, TCanvas *, TH1F *mc3 = 0);

void dataComparison() 
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   TFile * dataFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionDataCompBTag2015DPromptReco4.root");
   TFile * mcFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionJetHT2015DPromptReco4.root");

   HbbStyle style;
   style.set(PUBLIC);
   

   // Pt1
   TH1F *data_j1_Pt = (TH1F*) dataFile ->Get("h_jet1_Pt");
   data_j1_Pt -> SetTitle(";Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   TH1F *mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);

   TCanvas *canva1 = new TCanvas("canva1","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva1);
   gPad->SetLogy();
   canva1->Draw();
   
   // dEta
   TH1F *data_j_dEta = (TH1F*) dataFile ->Get("h_jet_dEta");
   data_j_dEta -> SetTitle(";#Delta #eta; Entries");
   data_j_dEta -> SetMarkerColor(1);
   data_j_dEta -> SetMarkerStyle(20);
   
   TH1F *mc_j_dEta = (TH1F*) mcFile -> Get("h_jet_dEta");
   mc_j_dEta -> SetMarkerColor(2);
   mc_j_dEta -> SetMarkerStyle(22);
   TCanvas *canva2 = new TCanvas("canva2","",1000,800);
   drawRatio(data_j_dEta,mc_j_dEta,canva2);
   //gPad->SetLogy();
   canva2->Draw();
   
   // M12
   TH1F *data_j_M12 = (TH1F*) dataFile ->Get("h_obj_M12");
   data_j_M12 -> SetTitle(";M_{12}, [Gev]; Entries");
   data_j_M12 -> SetMarkerColor(1);
   data_j_M12 -> SetMarkerStyle(20);
   
   TH1F *mc_j_M12 = (TH1F*) mcFile -> Get("h_obj_M12");
   mc_j_M12 -> SetMarkerColor(2);
   mc_j_M12 -> SetMarkerStyle(22);
   TCanvas *canva3 = new TCanvas("canva3","",1000,800);
   drawRatio(data_j_M12,mc_j_M12,canva3);
   //gPad->SetLogy();
   canva3->Draw();
   
   // Pt1
   TH1F *data_j2_Pt = (TH1F*) dataFile ->Get("h_jet2_Pt");
   data_j2_Pt -> SetTitle(";Pt_{2}, [GeV]; Entries");
   data_j2_Pt -> SetMarkerColor(1);
   data_j2_Pt -> SetMarkerStyle(20);
   
   TH1F *mc_j2_Pt = (TH1F*) mcFile -> Get("h_jet2_Pt");
   mc_j2_Pt -> SetMarkerColor(2);
   mc_j2_Pt -> SetMarkerStyle(22);
   TCanvas *canva4 = new TCanvas("canva4","",1000,800);
   drawRatio(data_j2_Pt,mc_j2_Pt,canva4);
   gPad->SetLogy();
   canva4->Draw();
   
   //Eta 1
   TH1F *data_j1_eta = (TH1F*) dataFile ->Get("h_jet1_Eta");
   data_j1_eta -> SetTitle(";#eta_{1}; Entries");
   data_j1_eta -> SetMarkerColor(1);
   data_j1_eta -> SetMarkerStyle(20);
   
   TH1F *mc_j1_eta = (TH1F*) mcFile -> Get("h_jet1_Eta");
   mc_j1_eta -> SetMarkerColor(2);
   mc_j1_eta -> SetMarkerStyle(22);
   TCanvas *canva5 = new TCanvas("canva5","",1000,800);
   drawRatio(data_j1_eta,mc_j1_eta,canva5);
   //gPad->SetLogy();
   canva5->Draw();
   
   //btag 1
   TH1F *data_j1_btag = (TH1F*) dataFile ->Get("h_jet1_BTag");
   data_j1_btag -> SetTitle(";#btag_{1}; Entries");
   data_j1_btag -> SetMarkerColor(1);
   data_j1_btag -> SetMarkerStyle(20);
   
   TH1F *mc_j1_btag = (TH1F*) mcFile -> Get("h_jet1_BTag");
   mc_j1_btag -> SetMarkerColor(2);
   mc_j1_btag -> SetMarkerStyle(22);
   TCanvas *canva6 = new TCanvas("canva6","",1000,800);
   drawRatio(data_j1_btag,mc_j1_btag,canva6);
   //gPad->SetLogy();
   canva6->Draw();
   
}


TCanvas *drawRatio(TH1F *data,TH1F *mc, TCanvas *canva, TH1F *mc3)
{

   //TCanvas *canva = new TCanvas("canva","",1000,800);
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();
   
   TH1F * hRatio = (TH1F*)data->Clone("hRatio");
   char name[200];
   TLegend *mcLegend = new TLegend(0.4,0.45,0.9,0.65);
   mcLegend -> SetTextSize(0.04);
   mcLegend -> AddEntry(data,"BTag data 2015D PrR4","p");
   mcLegend -> AddEntry(mc,"JetHT data 2015D PrR4","p");
   if(mc3) mcLegend -> AddEntry(mc3,"QCD_mc no Pt eff","p");
   mc -> Scale(data->Integral()/mc->Integral());
   mc -> Draw();
   data -> Draw("same");
   if(mc3 != NULL) mc3->Draw("same");
   mcLegend->Draw();
   canva -> cd();
   
   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();
   
   hRatio -> Sumw2();
   
   hRatio -> Divide(mc);
   hRatio -> Draw();
   
   TLine *horizLine = new TLine(data->GetXaxis()->GetXmin(),1,data->GetXaxis()->GetXmax(),1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

   
//   cc2->cd();
//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.,2.);
   hRatio -> GetYaxis() -> SetTitle("Ratio data/mc ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);
   
   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(data->GetXaxis()->GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);
   
   pad1 -> cd();
   
//   std::cout<<" Chi2/ndf: = "<<mc1D -> GetChisquare()/mc1D -> GetNDF()<<" sigmoid: "<<mc2D -> GetChisquare() / mc2D -> GetNDF()<<" Finale: "<<mc3D -> GetChisquare() / mc3D -> GetNDF()<<std::endl;
	return canva;
}

