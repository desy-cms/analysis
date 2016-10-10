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

#include "Analysis/MssmHbb/interface/PlotLimit.h"

namespace fs = boost::filesystem;

using namespace std;
using namespace boost::program_options;

int main(int argc, char * argv[]){

	const string cmsswBase = getenv("CMSSW_BASE");

	//Declare a group of general options
	options_description generalOptions("General options");
	generalOptions.add_options()
			("help,h", "Produce help message.")
			("verbose,v", value<int>()->default_value(0), "More verbose output.")
			;

    std::string  mode_;
    std::string  inputList_;
    std::string  benchmark_path_;
    std::string  benchmark_name_;
    std::string  outFileName_;
    std::vector<double> yMin_ = {0.5,1};
    std::vector<double> yMax_ = {1000.,60.};
    std::vector<double> xMin_ = {200.,200.};
    std::vector<double> xMax_ = {1300.,700.};
    std::vector<std::string> Lumi_ = {"2.69 fb^{-1}","2.69 fb^{-1}"};
    std::vector<std::string> xtitle_ = {"m_{#Phi} [GeV]","m_{#Phi} [GeV]"};
    std::vector<std::string> ytitle_ = {"95% C.L. limit on #sigma#times BR [pb]","tan#beta"};
    std::vector<bool> logY_ = {true,false};
    bool blindData_;

    // Declare a group of options that will be allowed only on command line
	options_description cmdLineOptions("General arguments");
	cmdLineOptions.add_options()
		("mode,M", value<std::string>(&mode_)->default_value("all"),"Mode. Can be: tanBeta/sigmaBR/all depending on what kind of limits you would like to compute")
		("output_file,o", value<std::string>(&outFileName_)->default_value(""),"Output file name. Will be conbined with default 'Hbb.limits'")
	    ("input_file,i", value<std::string>(&inputList_)->default_value(cmsswBase + "/src/Analysis/MssmHbb/datacards/Hbb.limits"),"Input file with outputs of the Combination tool - Hbb.limits")
		("compare_with", value<std::string>()->default_value("run1"),"If additional limits have to be drawn on the same canva (TEST version)")
		("blindData", value<bool>(&blindData_)->default_value(true),"Blinded Data, default: GxBR(tanB) = true(true)")
		("yMin", value<std::vector<double> >(&yMin_)->multitoken(),"YMin, default: GxBR(tanB) = 0.5(1)")
		("yMax", value<std::vector<double> >(&yMax_)->multitoken(),"YMax, default: GxBR(tanB) = 1000(60)")
		("xMin", value<std::vector<double> >(&xMin_)->multitoken(),"XMin, default: GxBR(tanB) = 200(200)")
		("xMax", value<std::vector<double> >(&xMax_)->multitoken(),"XMax, default: GxBR(tanB) = 1300(700)")
		("Lumi", value<std::vector<std::string> >(&Lumi_)->multitoken(),"String with Lumi")
		("xtitle", value<std::vector<std::string> >(&xtitle_)->multitoken(),"X axis Title")
		("ytitle", value<std::vector<std::string> >(&ytitle_)->multitoken(),"Y axis Title, default: GxBR(tanB) = 95% C.L. limit on #sigma#times BR [pb](tan#beta)")
		("logY", value<std::vector<bool> >(&logY_)->multitoken(),"Log Y axis, default: GxBR(tanB) = true(false)")
	    ;

	//Options for tan \beta limits
	options_description tanBetaOptions("Options For tanBeta limits");
	tanBetaOptions.add_options()
			("benchmark_name",value<std::string>(&benchmark_name_)->default_value("mhmod+ #mu=+200"), "Name of the benchmark scenario, affect ouput name")
			("benchmark_path",value<std::string>(&benchmark_path_)->default_value(cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root"),"Input root file with benchmark scenario")
			;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    options_description hidden("Hidden options");
    hidden.add_options()
        ("test", value<int>()->default_value(0),"if test specify 1. Default 0.")
        ;

    options_description all_options;
    all_options.add(generalOptions).add(cmdLineOptions).add(tanBetaOptions).add(hidden);


    options_description visible("Allowed options");
    visible.add(generalOptions).add(cmdLineOptions).add(tanBetaOptions);

    variables_map output_vm;

    store(parse_command_line(argc, argv,all_options), output_vm);
    notify(output_vm);

    if (output_vm.count("help")) {
	    std::cout << visible << std::endl;
	    exit(0);
	 }
    store(parse_command_line(argc, argv,all_options), output_vm);

  //Check whether input file contain only .root files or .txt
  cout<<inputList_<<endl;
  std::string line;
  std::ifstream infile(inputList_);
  std::ifstream benchFile(benchmark_path_);
  if(infile){
	  //Instantiation
	  PlotLimit plot(inputList_,blindData_);
	  //Calculate G x BR Limits
	  plot.CalculateSigmaBRLimits();
	  //Plot G x BR Limits
	  plot.PlotSigmaBRLimits(outFileName_,yMin_.at(0),yMax_.at(0),xMin_.at(0),xMax_.at(0),Lumi_.at(0),xtitle_.at(0),ytitle_.at(0),logY_.at(0));

	  //Calculate tanB Limits for current scenario:
	  plot.CalculateTanBetaLimits(benchmark_name_,benchmark_path_);
	  //Plot tanB Limits:
	  plot.PlotTanBetaLimit(benchmark_name_,outFileName_,yMin_.at(1),yMax_.at(1),xMin_.at(1),xMax_.at(1),Lumi_.at(1),xtitle_.at(1),ytitle_.at(1),logY_.at(1));
  }
  else {
	  std::cerr<<"Error wrong Input file name."<<std::endl;
	  exit(-1);
  }
	return 0;
}
