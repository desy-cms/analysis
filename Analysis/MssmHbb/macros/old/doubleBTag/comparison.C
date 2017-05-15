#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *drawRatio(TH1F *,TH1F *, TCanvas *, TH1F *mc3 = 0);
TCanvas *drawRatio(TH1F *,std::map <std::string,TH1F*> , TCanvas *);

void comparison()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * dataFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionRun2015D-PromptReco-v4.root");
   TFile * mcFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCD.root");
   TFile * mcFile2 = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDnoPtReWeight.root");
   TFile * mcTTFileMad = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionTTjets_Madgraph.root");
   TFile * mcTTFileMCNLO = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionTTJets_MC_NLO.root");

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

   // Pt2
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

   data_j2_Pt = (TH1F*) dataFile ->Get("h_djet_Pt12");
   data_j2_Pt -> SetTitle(";#Delta Pt_{12}, [GeV]; Entries");
   data_j2_Pt -> SetMarkerColor(1);
   data_j2_Pt -> SetMarkerStyle(20);

   mc_j2_Pt = (TH1F*) mcFile -> Get("h_djet_Pt12");
   mc_j2_Pt -> SetMarkerColor(2);
   mc_j2_Pt -> SetMarkerStyle(22);
   TCanvas *canva42 = new TCanvas("canva42","",1000,800);
   drawRatio(data_j2_Pt,mc_j2_Pt,canva42);
   gPad->SetLogy();
   canva42->Draw();

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

   //btag scaled
   data_j1_btag = (TH1F*) dataFile ->Get("h_jet1_BTagZoom");
   data_j1_btag -> SetTitle(";BTag_{1}; Entries");
   data_j1_btag -> SetMarkerColor(1);
   data_j1_btag -> SetMarkerStyle(20);

   mc_j1_btag = (TH1F*) mcFile -> Get("h_jet1_BTagZoom");
   mc_j1_btag -> SetMarkerColor(2);
   mc_j1_btag -> SetMarkerStyle(22);
   TCanvas *canva7 = new TCanvas("canva7","",1000,800);
   drawRatio(data_j1_btag,mc_j1_btag,canva7);
   //gPad->SetLogy();
   canva7->Draw();

   //Reweighting study:
   TH1F *data2_j1_Pt = (TH1F*) dataFile ->Get("h_jet1_PtNoRew");
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_PtNoRew");
   mc_j1_Pt -> SetTitle("h_jet1_PtNoRew");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva10 = new TCanvas("canva10","",1000,800);
   drawRatio(data2_j1_Pt,mc_j1_Pt,canva10);
   gPad->SetLogy();
   canva10->Draw();

   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_Ptrew1");
   mc_j1_Pt -> SetTitle("h_jet1_PtRew1");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva11 = new TCanvas("canva11","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva11);
   gPad->SetLogy();
   canva11->Draw();

   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_Ptrew2");
   mc_j1_Pt -> SetTitle("h_jet1_PtRew2");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva12 = new TCanvas("canva12","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva12);
   gPad->SetLogy();
   canva12->Draw();

   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_PtplusDEta");
   mc_j1_Pt -> SetTitle("h_jet1_PtplusDEta");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva13 = new TCanvas("canva13","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva13);
   gPad->SetLogy();
   canva13->Draw();

   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_All");
   mc_j1_Pt -> SetTitle("h_jet1_All");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva14 = new TCanvas("canva14","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva14);
   gPad->SetLogy();
   canva14->Draw();

   mc_j1_Pt = (TH1F*) mcFile -> Get("h_jet1_All2");
   mc_j1_Pt -> SetTitle("h_jet1_All2");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   TCanvas *canva15 = new TCanvas("canva15","",1000,800);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva15);
   gPad->SetLogy();
   canva15->Draw();

   //Draw THstack for each flavour:
   // pT
   data_j1_Pt = (TH1F*) dataFile ->Get("h_jet1_Pt");
   data_j1_Pt -> SetTitle(";Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);

	 
   std::map <std::string, TH1F *> fl;
   fl["qq"] = (TH1F*) mcFile -> Get("h_jet1_qq_Pt");
   fl["bb"] = (TH1F*) mcFile -> Get("h_jet1_bb_Pt");
   fl["cc"] = (TH1F*) mcFile -> Get("h_jet1_cc_Pt");
   fl["qb"] = (TH1F*) mcFile -> Get("h_jet1_qb_Pt");
   fl["qc"] = (TH1F*) mcFile -> Get("h_jet1_qc_Pt");
   fl["cb"] = (TH1F*) mcFile -> Get("h_jet1_cb_Pt");
   fl["tt"] = (TH1F*) mcTTFileMad -> Get("h_jet1_Pt");

   TCanvas *canva01 = new TCanvas("canva01","",1000,800);
   drawRatio(data_j1_Pt,fl,canva01);
   gPad->SetLogy();
   canva01->Draw();

   TCanvas *canva02 = new TCanvas("canva02","",1000,800);
   fl["bb"] -> SetMarkerStyle(21);
   drawRatio(fl["bb"],mc_j1_Pt,canva02);
   gPad->SetLogy();
   canva02->Draw();

   //Compare two TT MC:
   /*
   fl["ttMC@NLO"] = (TH1F*) mcTTFileMCNLO -> Get("h_jet1_Pt");
   fl["ttMC@NLO"] -> SetMarkerColor(6);
   fl["ttMC@NLO"] -> SetMarkerStyle(22);
   fl["tt"] -> SetMarkerColor(1);
   fl["tt"] -> SetMarkerStyle(20);
   //fl["ttMC@NLO"] -> Scale(fl["tt"] -> Integral() / fl["ttMC@NLO"]->Integral());
   TCanvas *canvaTT1 = new TCanvas("canvaTT1","",1000,800);
   drawRatio(fl["tt"],fl["ttMC@NLO"],canvaTT1);
   gPad -> SetLogy();
   canvaTT1->Draw();
   */

}

TCanvas *drawRatio(TH1F *data,std::map<std::string,TH1F*> mc, TCanvas *canva)
{
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();

   TLegend *mcLegend = new TLegend(0.6,0.65,0.9,0.85);
   mcLegend -> SetTextSize(0.04);
   THStack *hs = new THStack("hs","");
   //hs -> SetRangeUser(0.01,data->GetMaximum()+0.1*data->GetMaximum());
   hs -> SetMinimum(0.01);
   hs -> SetMaximum(data->GetMaximum()+0.1*data->GetMaximum());
   //int colors[] = {67,64,46,100,32,217,43};
   int colors[]= {kCyan+2,kYellow-6,kMagenta+2,kRed+2,kYellow+2,kGreen+2,kBlue-6};
   TH1F * hRatio = (TH1F*)data->Clone("hRatio");
   int counter = 0;
   char name[200];
   for(const auto & ih : mc)
   {
   		counter ++;
   		//ih.second->SetMarkerColor(counter+1);
   		//ih.second -> SetMarkerStyle(21+counter);
   		ih.second -> SetFillColor(colors[counter-1]);
   		ih.second -> SetLineColor(colors[counter-1]);
   		hs->Add(ih.second);
   }

   float rel[10];
   counter = 0;
   for(const auto & ih : mc)
   {

   		rel[counter] = ih.second->Integral() / ((TH1*)hs -> GetStack() -> Last()) -> Integral();

   		sprintf(name,"%s, %.2f",ih.first.c_str(),rel[counter]);
    	mcLegend -> AddEntry(ih.second,name,"la");
    	counter ++;
   }

   hs -> Draw("hist");
   data -> Draw("same");
   mcLegend->Draw();
   canva -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   hRatio -> Sumw2();

   hRatio -> Divide( (TH1*)hs -> GetStack() -> Last());
   hRatio -> Draw();

   TLine *horizLine = new TLine(data->GetXaxis()->GetXmin(),1,data->GetXaxis()->GetXmax(),1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

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
   mcLegend -> AddEntry(data,"Run2015D","p");
   mcLegend -> AddEntry(mc,"QCD_13TeV_pythia8","p");
   if(mc3) mcLegend -> AddEntry(mc3,"QCD_mc no Pt eff","p");
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
