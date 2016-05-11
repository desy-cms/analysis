#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options/variables_map.hpp>
#include "TSystem.h"
#include "RooMsgService.h"

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "stdlib.h"

#include "Analysis/MssmHbb/interface/MssmHbb.h"
#include "Analysis/MssmHbb/interface/json.h"
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"
#include "Analysis/MssmHbb/interface/Weights.h"
#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"
#include "Analysis/MssmHbb/interface/selectionDoubleB.h"


using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;
using namespace boost::program_options;

// =============================================================================================
int main(int argc, char * argv[])
{
	TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

	/*
	std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/BTagCSVData/Run2015D-16Dec2015-v1.txt";


	selectionDoubleB analysis(inputList);
	analysis.setupAnalysis("Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt");
	analysis.SetupStandardOutputFile();
	analysis.addTriggerObjects();
	analysis.makeHistograms();
	analysis.applySelection();
	*/
	//Get CMSSW version
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));
	variables_map input_map_;
	// All options
	options_description allOptions("Allowed arguments");
	//General command line options
	options_description cmdLineOptions("Optional arguments");
	cmdLineOptions.add_options()
	    ("help,h", "Produce help message.")
	    ("verbose,v", value<int>()->default_value(0), "More verbose output.")
	    ("input_files,i", value<std::string>()->default_value("/nfs/dust/cms/user/shevchen/samples/miniaod/76X/filtered/MSSMHbb/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.txt"),"Input .txt file with NTuples")
	    ("output_file,o", value<std::string>()->default_value(""), "Output file name, if not specified will be created automatically")
		("json_file,j", value<std::string>()
				 ->default_value(cmsswBase+"/src/Analysis/MssmHbb/bin/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt"),
		 "Json file")
		("test,l", value<int>()->default_value(1),"if test specify 1. Default 0.")
	    ;
	  store(command_line_parser(argc, argv).options(cmdLineOptions)
	            .allow_unregistered().run(), input_map_);
	  notify(input_map_);

	  // Required options
	  options_description requiredOptions("Required arguments");
	  requiredOptions.add_options()
			  ("trigger,t",value<int>()->default_value(1),
			   "Selection type: \n"
			   "  \t1 - Low Mass; \n"
			   "  \t0 - High Mass.")
			  ("selection,s",value<std::string>()->required(),
			  "Specify selection type that you want to run: \n"
			  "  \tMssmHbb - main analysis script, apply 3b signal selection; \n"
			  "  \t2bJet   - apply 2b selection; \n"
			  "  \treverse - apply 3b background selection (reversse btag for third jet); \n"
			  "  \tTrigger - calculate kinematic trigger efficienies.")
	  	  	  ("analysis,a",value<std::string>()->required(),
	  	  	  "Specify Finale result: \n"
	  	  	  "  \trebuild - re-run everything; \n"
	  	  	  "  \tDataMC  - Data/MC comparison plots; \n"
	  	  	  "  \t")
			  ;
	  store(command_line_parser(argc, argv).options(requiredOptions)
	            .allow_unregistered().run(), input_map_);

	  allOptions.add(cmdLineOptions).add(requiredOptions);

	  if (input_map_.count("help")) {
	    std::cout << allOptions << std::endl;
	    exit(0);
	  }

	  try {
	    notify(input_map_);
	  } catch (const required_option& ex) {
	    std::cerr << ex.what() << std::endl;
	    exit(1);
	  }

//	  analysis_ = new Analysis(input_map_["input_files"].as<std::string>());
	  std::string  selection_ =  input_map_["selection"].as<std::string>();
	  std::string  analysis_  =  input_map_["analysis"].as<std::string>();
	  std::string  inputList_ =  input_map_["input_files"].as<std::string>();
	  std::string  output_    =  input_map_["output_file"].as<std::string>();
	  std::string json_file_ =  input_map_["json_file"].as<std::string>();
	  auto lowM_        =  input_map_["trigger"].as<int>();
	  auto test_		 =  input_map_["test"].as<int>();


	  //Start to apply input:
	  //TODO: Factoring pattern should be applied: http://stackoverflow.com/questions/34519878/decide-what-constructor-call-based-on-user-input/34520081
	  if(boost::iequals(selection_,"mssmhbb")){
		  MssmHbbSignal analysis(inputList_,lowM_,test_);//,analysis_);
		  analysis.runAnalysis(json_file_,output_,100);

	  }
	  else if (boost::iequals(selection_,"trigger")){
		  ;
	  }
	  else if (boost::iequals(selection_,"2bjet")){
		  selectionDoubleB analysis(inputList_);
	  }

/**/

	return 0;
}


/*
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-PromptReco-v4.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015C_25ns-05Oct2015-v1.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-05Oct2015-v1.root
*/
