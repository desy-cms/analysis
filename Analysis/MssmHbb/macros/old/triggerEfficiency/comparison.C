#include "Analysis/MssmHbb/src/HbbStyle.cc"

TCanvas *drawRatio(TH1F *,TH1F *, TCanvas *, TH1F *mc3);
TCanvas *drawRatio(TH1F *,TH1F *, TCanvas *, TH1F *mc3, bool scale);
TCanvas *drawRatio(TH1F *,TH1F *, TCanvas *, bool scale);

void comparison() 
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle(); 

   TFile * dataFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/ResultsMCJetHT2015DPromptReco4.root");
   TFile * mcFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDEffNoPreTrigStart50.root");
   //TFile * mcFile2 = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDEfficiency.root"); 
   //TFile * mcFile = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDEffNoPreTrigStart50Pt100.root");
   TFile * mcFile2 = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/SelectionQCDEffNoPreTrig.root");
   
   HbbStyle style;
   style.set(PUBLIC);
   
	
   // Pt1
   TH1F *data_j1_Pt = (TH1F*) dataFile ->Get("h_Only1jet_jet1_Pt");
   data_j1_Pt -> SetTitle("Single Jet;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   TH1F *mc_j1_Pt = (TH1F*) mcFile -> Get("h_Only1jet_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   TH1F *mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_Only1jet_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Single Jet");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva1 = new TCanvas("canva1","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva1,mc_j1_Pt);   
   gPad->SetLogy();
   canva1->Draw();

   TCanvas *canva11 = new TCanvas("canva11","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva11,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva11->Draw();
   
   
   // double jets
   
   data_j1_Pt = (TH1F*) dataFile ->Get("h_TwoJets_jet1_Pt");
   data_j1_Pt -> SetTitle("Two Matched Jets;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_TwoJets_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_TwoJets_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Two Matched Jets + #eta < 2.2");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva2 = new TCanvas("canva2","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva2,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva2->Draw();

   TCanvas *canva21 = new TCanvas("canva21","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva21,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva21->Draw();
   
   TCanvas *canva22 = new TCanvas("canva22","",1800,1000);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva22,1);   
   gPad->SetLogy();
   canva22->Draw();
   
   // double jets + bTag
   
   data_j1_Pt = (TH1F*) dataFile ->Get("h_TwoBtagJets_jet1_Pt");
   data_j1_Pt -> SetTitle("Two Matched Jets;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_TwoBtagJets_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_TwoBtagJets_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Two Matched Jets + BtagCSV > 0.9 + #eta < 2.2");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva3 = new TCanvas("canva3","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva3,mc_j1_Pt);   
   gPad->SetLogy();
   canva3->Draw();

   TCanvas *canva31 = new TCanvas("canva31","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva31,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva31->Draw();
   
   
   // Offline Pt cut
   
   data_j1_Pt = (TH1F*) dataFile ->Get("h_PtStudy_ptCut_jet1_Pt");
   data_j1_Pt -> SetTitle("Two Matched Jets;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_PtStudy_ptCut_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_PtStudy_ptCut_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Two Matched Jets + Pt12 > 80 + #eta < 2.2");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva4 = new TCanvas("canva4","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva4,mc_j1_Pt);   
   gPad->SetLogy();
   canva4->Draw();

   TCanvas *canva41 = new TCanvas("canva41","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva41,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva41->Draw();
   
   // Offline dR cut
   
   data_j1_Pt = (TH1F*) dataFile ->Get("h_PtStudy_dRCut_jet1_Pt");
   data_j1_Pt -> SetTitle("Two Matched Jets;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_PtStudy_dRCut_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_PtStudy_dRCut_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Two Matched Jets + dR + #eta < 2.2");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva6 = new TCanvas("canva6","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva6,mc_j1_Pt);   
   gPad->SetLogy();
   canva6->Draw();

   TCanvas *canva61 = new TCanvas("canva61","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva61,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva61->Draw();
   
   // Offline all cuts
   
   data_j1_Pt = (TH1F*) dataFile ->Get("h_PtStudy_AllCut_jet1_Pt");
   data_j1_Pt -> SetTitle("Two Matched Jets;Pt_{1}, [GeV]; Entries");
   data_j1_Pt -> SetMarkerColor(1);
   data_j1_Pt -> SetMarkerStyle(20);
   
   mc_j1_Pt = (TH1F*) mcFile -> Get("h_PtStudy_AllCut_jet1_Pt");
   mc_j1_Pt -> SetMarkerColor(2);
   mc_j1_Pt -> SetMarkerStyle(22);
   
   mc2_j1_Pt = (TH1F*) mcFile2 -> Get("h_PtStudy_AllCut_jet1_Pt");
   mc2_j1_Pt -> SetTitle("Two Matched Jets + Pt12 > 80 + dR + #eta < 2.2");
   mc2_j1_Pt -> SetMarkerColor(4);
   mc2_j1_Pt -> SetMarkerStyle(25);
   TCanvas *canva7 = new TCanvas("canva7","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva7,mc_j1_Pt);   
   gPad->SetLogy();
   canva7->Draw();

   TCanvas *canva71 = new TCanvas("canva71","",1800,1000);
   drawRatio(data_j1_Pt,mc2_j1_Pt,canva71,mc_j1_Pt,1);   
   gPad->SetLogy();
   canva71->Draw();
   
   TCanvas *canva72 = new TCanvas("canva72","",1800,1000);
   drawRatio(data_j1_Pt,mc_j1_Pt,canva72,1);   
   gPad->SetLogy();
   canva72->Draw();
   
   
   // All Pt hat bins separately
   TCanvas *can = new TCanvas("can","");
   gPad -> SetLogy();
   std::string ptBins[17] = {"5to10","10to15","15to30","30to50","50to80","80to120","120to170","170to300","300to470","470to600","600to800","800to1000","1000to1400","1400to1800","1800to2400","2400to3200","3200toInf"};
   TFile *pt1[17]; TH1F *histo[17];
   TLegend *lol = new TLegend(0.6,0.6,0.9,0.9);
   for(int i = 0;i<17;++i) 
   {
    	//std::string name = (string)"~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/" + (string)"ResultsQCD_Pt_" + ptBins[i] + (string)"_Asympt25ns_MCRUN2_13TeV.root";
    	std::string name = (string)"~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/" + (string)"ResultsMCQCD_Pt_" + ptBins[i] + (string)"_Asympt25ns_MCRUN2_13TeV.root";
    	pt1[i] = new TFile(name.c_str());
    	histo[i] = (TH1F*) pt1[i] ->Get("h_Only1jet_jet1_Pt");
    	histo[i] -> SetMarkerStyle(20);
    	histo[i] -> SetMarkerColor(i+1);
    	if(i== 0) 
    	{ 
    	
    		histo[i] -> SetMaximum(4*10e11);
    		histo[i] -> SetMinimum(1);
    		histo[i] -> Draw();
    	}
    	else histo[i] -> Draw("same");
    	lol->AddEntry(histo[i],ptBins[i].c_str(),"p");
    	std::cout<<name<<std::endl;
		
   }
   	lol->Draw();
   
}

TCanvas *drawRatio(TH1F *hdata,TH1F *hmc, TCanvas *canva, TH1F *hmc3)
{
   
   TH1F * data = (TH1F*)hdata->Clone("data");
   TH1F * mc = (TH1F*)hmc->Clone("mc");
   TH1F * mc3 = (TH1F*)hmc3->Clone("mc3");
   char name[200];
   TLegend *mcLegend = new TLegend(0.4,0.45,0.9,0.65);
   mcLegend -> SetTextSize(0.04);
   mcLegend -> AddEntry(data,"JetHT Prompt-Reco4 1 fb^{-1}","p");
   	mcLegend -> AddEntry(mc3,"MC start at 50","p");
   if(mc3) mcLegend -> AddEntry(mc,"Full MC","p");
   
   mc -> Draw();
   data -> Draw("same");
   if(mc3 != NULL) mc3->Draw("same");
   mcLegend->Draw();
   	return canva;
}

TCanvas *drawRatio(TH1F *data,TH1F *mc, TCanvas *canva, TH1F *mc3,bool scale)
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
   	mcLegend -> AddEntry(data,"JetHT Prompt-Reco4 1 fb^{-1}","p");
   	mcLegend -> AddEntry(mc3,"MC start at 50 NORMALIZED to data","p");
   if(mc3) mcLegend -> AddEntry(mc,"Full MC NORMALIZED to data","p");

   mc->Scale(data->Integral(20,40)/mc->Integral(20,40));
   mc3->Scale(data->Integral(20,40)/mc3->Integral(20,40));
  
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
   
   hRatio -> Divide(mc3);
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

TCanvas *drawRatio(TH1F *data,TH1F *mc, TCanvas *canva, bool scale)
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
   	mcLegend -> AddEntry(data,"JetHT Prompt-Reco4 1 fb^{-1}","p");
   	mcLegend -> AddEntry(mc,"MC start at 50 NORMALIZED to data","p");

   mc->Scale(data->Integral(20,40)/mc->Integral(20,40));
  
   mc -> Draw();
   data -> Draw("same");
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

