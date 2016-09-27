/*
#include "Analysis/MssmHbb/macros/Drawer/HttStylesNew.cc"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"

#include "TMath.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TString.h"

#include "sstream"
#include "string"
#include <iostream>
#include <fstream>
#include "Analysis/MssmHbb/src/Lumi_Setup.C"

double XSecToTanBeta(mssm_xs_tools my,
		     double mA,
		     double xsection,
		     double minTanBeta,
		     double maxTanBeta) {

  double minimalDifference = 1e+10;
  bool rangeExceeded = true;
  double tanBetaTarget = -1;
  double xsecTarget    = -1;
  int ibmax = 10*int(maxTanBeta-minTanBeta);
  for (int ib=0; ib<ibmax; ++ib) {

    double tanBeta = minTanBeta + 0.1*double(ib);

    double sigmaBBA = my.bbHSantander_A(mA,tanBeta);
    double sigmaBBH = my.bbHSantander_H(mA,tanBeta);

    double BrAbb = my.br_Abb(mA,tanBeta);
    double BrHbb = my.br_Hbb(mA,tanBeta);

    double totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
    double difference = TMath::Abs(totXSec-xsection);

    if (difference<minimalDifference) {
      minimalDifference = difference;
      tanBetaTarget = tanBeta;
      xsecTarget = totXSec;
    }

    if (totXSec>xsection) {
      rangeExceeded = false;
      break;
    }

  }

  if (rangeExceeded) 
    tanBetaTarget = tanBetaTarget*TMath::Sqrt(xsection/xsecTarget);
  
  return tanBetaTarget;

}

// benchmarks = hMSSM_13TeV.root
//              mhmodp_mu200_13TeV.root

void PlotTanBetaLimit(const std::string fileList = "Hbb.limits",
			  std::string output = "",
		      const std::string benchmark = "mhmodp_mu200_13TeV.root",
			  bool blindData = true,
		      float yMin =   1,
		      float yMax =  60,
		      float xMin = 200,
		      float xMax = 700,
		      std::string Lumi = "2.62 fb^{-1}",
		      std::string xtitle = "m_{A} [GeV]",
		      std::string ytitle = "tan#beta",
		      bool logY = true
		      ) {


  // char * filelist - List of files (output RooT files
  //                   produced by 'combine -M Asymptotic')
  // char * benchmarks - MSSM benchmark scenario file 
  //                     (cross sections + BRs) :
  //                     hMSSM_13TeV.root,
  //                     mhmodp_mu200_13TeV.root, 
  //                     etc     
  // blindData -  true : blind observed limits
  //             false : show observed limits 

  SetStyle();
  gStyle->SetOptFit(0000);
  gStyle->SetErrorX(0.5);


  mssm_xs_tools my(benchmark.c_str(),true,0);

  double minTanBeta =  1;
  double maxTanBeta = 60;

  std::string Benchmark(benchmark.c_str());
  std::string bench_name;
  if(Benchmark.find("mhmodp_mu200") != std::string::npos) bench_name = "mhmodp_mu200";
  else bench_name = "ANOTHER_BENCH";
  std::string header("mhmod+");
  if (Benchmark.find("hMSSM") != std::string::npos)
    header = "hMSSM";


  const int nPoints = 100;

  // signal strength limits sigma*BR / sigma*BR (at tanb=30)
  double mA[nPoints];      
  double minus2R[nPoints]; 
  double minus1R[nPoints]; 
  double medianR[nPoints]; 
  double plus1R[nPoints];  
  double plus2R[nPoints];  
  double obsR[nPoints];    

  double obs[nPoints];
  double minus2[nPoints];
  double minus1[nPoints];
  double median[nPoints];
  double plus1[nPoints];
  double plus2[nPoints];

  std::ifstream inputList(fileList);
  std::string FileList(fileList.c_str());

  std::string fileName;

  double MH;
  double LIMIT;

  int counter = 0;

  float massMin = 1000;
  float massMax = 0;

  while (inputList >> fileName) {

    //    std::cout << fileName << std::endl;

    TFile * file = new TFile(fileName.c_str());

    TTree * tree = (TTree*)file->Get("limit");

    //    std::cout << "file : " << file << std::endl;
    //    std::cout << "tree : " << tree << std::endl;

    tree->SetBranchAddress("limit",&LIMIT);
    tree->SetBranchAddress("mh",&MH);

    tree->GetEntry(0);

    if (MH<massMin) massMin = MH;
    if (MH>massMax) massMax = MH;

    mA[counter] = MH;
    minus2R[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);

    //    std::cout << mA[counter] << std::endl;
    
    tree->GetEntry(1);
    minus1R[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);

    tree->GetEntry(2);
    medianR[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);

    tree->GetEntry(3);
    plus1R[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);

    tree->GetEntry(4);
    plus2R[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);

    tree->GetEntry(5);
    obsR[counter] = XSecToTanBeta(my,MH,LIMIT,minTanBeta,maxTanBeta);
    if (blindData)
      obsR[counter] = medianR[counter];

    counter++; 
      
  }


  std::cout << std::endl;
  std::cout << "m(H)     -2s     -1s   median     +1s     +2s     obs" << std::endl; 

  for (int i=0; i<counter; ++i) {

    obs[i]    = obsR[i];
    minus2[i] = minus2R[i];
    minus1[i] = minus1R[i];
    median[i] = medianR[i];
    plus1[i]  = plus1R[i];
    plus2[i]  = plus2R[i];

    char strOut[400];
    sprintf(strOut,"%4i  %6.1f  %6.1f   %6.1f  %6.1f  %6.1f  %6.1f",
	    int(mA[i]),minus2[i],minus1[i],median[i],plus1[i],plus2[i],obs[i]);
    std::cout << strOut << std::endl;

  }
  std::cout << std::endl;

  double zeros[nPoints];
  double upper[nPoints];
  double lower[nPoints];
  double central[nPoints];
  for (int i=0; i<counter; ++i) {
    zeros[i] = 0;
    central[i] = 15; 
    minus2[i] = median[i] - minus2[i];
    minus1[i] = median[i] - minus1[i];
    plus1[i]  = plus1[i]  - median[i];
    plus2[i]  = plus2[i]  - median[i];
    upper[i] = 15 - central[i];
    lower[i] = central[i] - obs[i];
  }
  
  
  int nPointsX = counter;

  TGraph * obsG = new TGraph(nPointsX, mA, obs);
  obsG->SetLineWidth(3);
  obsG->SetLineColor(1);
  obsG->SetLineWidth(2);
  obsG->SetMarkerColor(1);
  obsG->SetMarkerStyle(20);
  obsG->SetMarkerSize(1.4);

  TGraph * expG = new TGraph(nPointsX, mA, median);
  expG->SetLineWidth(3);
  expG->SetLineColor(2);
  expG->SetLineStyle(2);
  
  TGraphAsymmErrors * observed = new TGraphAsymmErrors(nPointsX, mA, central, zeros, zeros, lower, upper);
  observed->SetFillColor(kCyan-4);
  observed->SetLineWidth(3);

  TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(nPointsX, mA, median, zeros, zeros, minus1, plus1);
  innerBand->SetFillColor(kGreen);
  innerBand->SetLineColor(kGreen);

  TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(nPointsX, mA, median, zeros, zeros, minus2, plus2);
  outerBand->SetFillColor(kYellow);
  outerBand->SetLineColor(kYellow);

  TH2F * frame = NULL;

  if (xMax>0) massMax = xMax;

  frame = new TH2F("frame","",2,massMin,massMax,2,yMin,yMax);
  frame->GetXaxis()->SetTitle(xtitle.c_str());
  frame->GetYaxis()->SetTitle(ytitle.c_str());
  frame->GetXaxis()->SetNdivisions(510);
  frame->GetYaxis()->SetNdivisions(206);
  frame->GetYaxis()->SetTitleOffset(1.3);  
  frame->GetYaxis()->SetTitleSize(0.048);  
  frame->GetXaxis()->SetTitleOffset(1.05);

  TCanvas *canv = new TCanvas("canv", "histograms", 600, 600);

  frame->Draw();

  outerBand->Draw("3same");
  innerBand->Draw("3same");
  expG->Draw("lsame");
  if (!blindData)
    obsG->Draw("lpsame");

//  float xLeg = 0.18;
//  float yLeg = 0.83;
//  float xLegend = 0.57;
//  float yLegend = 0.41;
//  float sizeLeg = 0.27;

  TLegend * leg = new TLegend(0.65,0.17,0.92,0.44);
  leg->SetFillColor(0);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
  leg->SetHeader(header.c_str());
  if (!blindData) 
    leg->AddEntry(obsG,"Observed","lp");
  leg->AddEntry(expG,"Expected","l");
  leg->AddEntry(innerBand,"#pm1#sigma Expected","f");
  leg->AddEntry(outerBand,"#pm2#sigma Expected","f");
  leg->Draw();
  //  TLine * line = new TLine(massMin,1,massMax,1);
  //  line->SetLineWidth(2);
  //  line->SetLineStyle(2);
  //  line->Draw();

  TPad * pad = (TPad*)canv->GetPad(0);
  LumiSetup lum;
  lum.CMS_lumi(pad,4,11);
  lum.writeExtraText = true;
  lum.lumi_13TeV = Lumi;
  lum.extraText = "Simulation";
  pad->RedrawAxis();

  leg->Draw();
  canv->SetLogy(false);
  canv->Update();
  std::string out_name = bench_name  + "_tanBeta";
  if(output != "") out_name = out_name + output;
  //  canv->Print(suffix+".pdf","Portrait pdf");
  canv->Print( (out_name + ".png").c_str() );
  canv->Print( (out_name+".pdf").c_str());

}
*/
