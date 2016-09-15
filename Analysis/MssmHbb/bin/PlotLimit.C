#include "Analysis/MssmHbb/macros/Drawer/HttStylesNew.cc"
#include "Analysis/MssmHbb/macros/Drawer/CMS_lumi.C"
#include "Analysis/MssmHbb/src/PlotTanBetaLimit.C"
//#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
//#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"

#include "TString.h"
#include "TFile.h"
#include "TLatex.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TH2.h"
#include "TSystem.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "stdlib.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options/variables_map.hpp>

namespace fs = boost::filesystem;

using namespace std;
using namespace boost::program_options;

void PlotSigmaBRLimit(const char * fileList = "Hbb.limits",
	       std::string output = "",
		   bool blindData = true,
	       float yMin = 0.5,
	       float yMax = 1000,
	       float xMax = -150,
	       TString Lumi = "2.62 fb^{-1}",
	       TString xtitle = "m_{#Phi} [GeV]",
	       TString ytitle = "95% C.L. limit on #sigma#times BR [pb]",
	       bool logY = true
	       );

int main(int argc, char * argv[]){

	const string cmsswBase = getenv("CMSSW_BASE");

	//Declare a group of general options
	options_description generalOptions("General options");
	generalOptions.add_options()
			("help,h", "Produce help message.")
			("verbose,v", value<int>()->default_value(0), "More verbose output.")
			;

    // Declare a group of options that will be allowed only on command line
	options_description cmdLineOptions("CMD Optional arguments");
	cmdLineOptions.add_options()
		("mode,M", value<std::string>()->default_value("all"),"Mode. Can be: tanBeta/sigmaBR/all depending on what kind of limits you would like to compute")
		("output_file,o", value<std::string>()->default_value(""),"Output file name. Will be conbined with default 'Hbb.limits'")
	    ("input_file,i", value<std::string>()->default_value(cmsswBase + "/src/Analysis/MssmHbb/datacards/Hbb.limits"),"Input file with outputs of the Combination tool - Hbb.limits")
		("benchmark,b",value<std::string>()->default_value(cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root"),"Input root file with benchmark scenario")
//		("yMin", value<double>()->default_value(5e-01),"YMin")
//		("yMax", value<double>()->default_value(1000),"YMax")
//		("xMax", value<double>()->default_value(-150),"XMax")
//		("Lumi", value<std::string>()->default_value("2.62 fb^{-1}"),"String with Lumi, i.e.: 2.62 fb^{-1}")
//		("xtitle", value<std::string>()->default_value("m_{#Phi} [GeV]"),"X axis Title")
//		("ytitle", value<std::string>()->default_value("95% C.L. limit on #sigma#times BR [pb]"),"Y axis Title")
//		("logY", value<bool>()->default_value(true),"Log Y axis? True/False")
		("blindData", value<bool>()->default_value(true),"Blinded Data? True/False")
	    ;
    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    options_description hidden("Hidden options");
    hidden.add_options()
        ("test", value<int>()->default_value(0),"if test specify 1. Default 0.")
        ;

    options_description all_options;
    all_options.add(generalOptions).add(cmdLineOptions).add(hidden);


    options_description visible("Allowed options");
    visible.add(generalOptions).add(cmdLineOptions);

    variables_map output_vm;

    store(parse_command_line(argc, argv,all_options), output_vm);
    notify(output_vm);

    if (output_vm.count("help")) {
	    std::cout << visible << std::endl;
	    exit(0);
	 }
    store(parse_command_line(argc, argv,all_options), output_vm);

    std::string  mode_			=  output_vm["mode"].as<std::string>();
    std::string  inputList_ 	=  output_vm["input_file"].as<std::string>();
    std::string  benchmark_		=  output_vm["benchmark"].as<std::string>();
    std::string  outFileName_   =  output_vm["output_file"].as<std::string>();
//  auto yMin_				=	output_vm["yMin"].as<double>();
//  auto yMax_				=	output_vm["yMax"].as<double>();
//  auto xMax_				=	output_vm["xMax"].as<double>();
//  TString Lumi_ 				=  output_vm["Lumi"].as<std::string>();
//  TString xtitle_ 				=  output_vm["xtitle"].as<std::string>();
//  TString ytitle_ 				=  output_vm["ytitle"].as<std::string>();
//  auto logY_ 				=  output_vm["logY"].as<bool>();
  auto blindData_ 			=  output_vm["blindData"].as<bool>();

  //Check whether input file contain only .root files or .txt
  cout<<inputList_<<endl;
  std::string line;
  std::ifstream infile(inputList_);
  std::ifstream benchFile(benchmark_);
  if(infile){
	  //Check which mode to use:
	  if( (mode_ == "all" || mode_ == "tanBeta") && !benchFile){
		  std::cerr<<"Error no file: "<<benchmark_<<" with benchmark scenarious"<<std::endl;
		  exit(-1);
	  }
	  if(mode_ == "all"){
		  PlotSigmaBRLimit(inputList_.c_str(),outFileName_,blindData_);

		  PlotTanBetaLimit(inputList_,outFileName_,benchmark_,blindData_);
	  }
	  else if( mode_ == "tanBeta"){
		  PlotTanBetaLimit(inputList_,outFileName_,benchmark_,blindData_);
	  }
	  else if (mode_ == "sigmaBR"){
		  PlotSigmaBRLimit(inputList_.c_str(),outFileName_,blindData_);
	  }

  }
  else {
	  std::cerr<<"Error wrong Input file name."<<std::endl;
	  exit(-1);
  }
	return 0;
}

void PlotSigmaBRLimit(const char * fileList, std::string output, bool blindData, float yMin, float yMax, float xMax, TString Lumi, TString xtitle, TString ytitle, bool logY) {

  // char * filelist - List of files (output RooT files
  //                   produced by 'combine -M Asymptotic')
  // blindData -  true : blind observed limits
  //             false : show observed limits 
  // int benchmark - 0 : mh-max, mu = +200 GeV
  //                 1 : mh-mod +
  //                 2 : mh-mod -


  SetStyle();
  gStyle->SetOptFit(0000);
  gStyle->SetErrorX(0.5);

  //output .txt file with limits
  std::string suffix(fileList);
  if(output!="") suffix += "_" + output;
  std::fstream fs;
  fs.open( (suffix + ".txt").c_str(),std::fstream::out );

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

  TString FileList(fileList);

  TString fileName;

  double MH;
  double LIMIT;

  int counter = 0;

  float massMin = 1000;
  float massMax = 0;

  while (inputList >> fileName) {

    //    std::cout << fileName << std::endl;

    TFile * file = new TFile(fileName);
    if(gSystem->AccessPathName(fileName)){
    	std::cout<<"No file: "<<fileName<<std::endl;
    	exit(-1);
    }

    TTree * tree = (TTree*)file->Get("limit");

    //    std::cout << "file : " << file << std::endl;
    //    std::cout << "tree : " << tree << std::endl;

    tree->SetBranchAddress("limit",&LIMIT);
    tree->SetBranchAddress("mh",&MH);

    tree->GetEntry(0);

    if (MH<massMin) massMin = MH;
    if (MH>massMax) massMax = MH;

    mA[counter] = float(MH);
    minus2R[counter] = float(LIMIT);

    //    std::cout << mA[counter] << std::endl;
    
    tree->GetEntry(1);
    minus1R[counter] = float(LIMIT);

    tree->GetEntry(2);
    medianR[counter] = float(LIMIT);

    tree->GetEntry(3);
    plus1R[counter] = float(LIMIT);

    tree->GetEntry(4);
    plus2R[counter] = float(LIMIT);

    tree->GetEntry(5);
    obsR[counter] = float(LIMIT);
    if (blindData)
      obsR[counter] = medianR[counter];

    counter++; 
      
  }


  std::cout << std::endl;
  std::cout << "m(H)     -2s     -1s  median     +1s     +2s     obs" << std::endl; 
  //           " 100  136.83  197.90  308.62  507.92  840.93  308.62"
  fs<< "m(H)     -2s     -1s  median     +1s     +2s     obs \n";
 

  for (int i=0; i<counter; ++i) {

    obs[i]    = obsR[i];
    minus2[i] = minus2R[i];
    minus1[i] = minus1R[i];
    median[i] = medianR[i];
    plus1[i]  = plus1R[i];
    plus2[i]  = plus2R[i];

    char strOut[400];
    sprintf(strOut,"%4i  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f",
	    int(mA[i]),minus2[i],minus1[i],median[i],plus1[i],plus2[i],obs[i]);
    std::cout << strOut << std::endl;
    fs<<strOut;
    fs<<"\n";

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
  frame->GetXaxis()->SetTitle(xtitle);
  frame->GetYaxis()->SetTitle(ytitle);
  frame->GetXaxis()->SetNdivisions(505);
  frame->GetYaxis()->SetNdivisions(206);
  frame->GetYaxis()->SetTitleOffset(1.25);  
  frame->GetYaxis()->SetTitleSize(0.048);  
  

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

  TLegend * leg = new TLegend(0.67,0.54,0.90,0.76);
  leg->SetFillColor(0);
  leg->SetTextSize(0.035);
  leg->SetBorderSize(0);
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
  writeExtraText = true;
  lumi_13TeV = Lumi;
  extraText = "Simulation";
  CMS_lumi(pad,4,33); 
  pad->RedrawAxis();

  leg->Draw();
  if(logY) canv->SetLogy();
  canv->Update();
  canv->Print( (suffix+".pdf").c_str() ,"Portrait pdf");
  canv->Print( (suffix+".png").c_str()) ;
  fs.close();
}

