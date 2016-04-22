/*
 * JetAnalysisBase.cpp
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */
/*
#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"

using namespace analysis;
using namespace analysis::mssmhbb;
using namespace boost::program_options;

template <std::string str>
JetAnalysisBase<str>::JetAnalysisBase(const int & argc, char * argv[]) {
	// TODO Auto-generated constructor stub
	// Get CMSSW version
	cmsswBase_ = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));
	// All options
	options_description allOptions("Allowed arguments");
	//General command line options
	options_description cmdLineOptions("Optional arguments");
	cmdLineOptions.add_options()
	    ("help,h", "Produce help message.")
	    ("verbose,v", value<int>()->default_value(0), "More verbose output.")
	    ("input_files,i", value<std::string>(),"Input .txt file with NTuples")
	    ("output_file,o", value<std::string>(), "Output file name")
		 ("json_file", value<std::string>()
				 ->default_value(cmsswBase+"/src/Analysis/MssmHbb/bin/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt"),
		 "Json file")
	    ;
	  store(command_line_parser(argc, argv).options(cmdLineOptions)
	            .allow_unregistered().run(), input_map_);
	  notify(input_map_);

	  // Required options
	  options_description requiredOptions("Required arguments");
	  requiredOptions.add_options()
	    ("jets_number,j",value<int>()->required(), "Number of requird jets")
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

	  analysis_ = new Analysis(input_map_["input_files"].as<std::string>());

}

template <>
JetAnalysisBase<"TriggerEfficiency">::JetAnalysisBase(const int & argc, char * argv[]) {
	// TODO Auto-generated constructor stub
	// Get CMSSW version
	cmsswBase_ = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	// All options
	options_description allOptions("Allowed arguments");

	// general command line options
	  options_description cmdLineOptions("Optional arguments");
	  cmdLineOptions.add_options()
	    ("help,h", "Produce help message.")
	    ("verbose,v", value<int>()->default_value(0), "More verbose output.")
	    ("input_files,i", value<std::string>()
	    		->default_value("/nfs/dust/cms/user/shevchen/samples/miniaod/76X/JetHT/Run2015C_25ns-16Dec2015-v1.txt"),
	     "Input .txt file with NTuples")
	    ("output_file,o", value<std::string>()
	    		->default_value(cmsswBase+"/src/Analysis/MssmHbb/output/TriggerEfficiency.root"),
	     "Output file name")
		 ("json_file", value<std::string>()
				 ->default_value(cmsswBase+"/src/Analysis/MssmHbb/bin/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_v2.txt"),
		 "Json file")
	    ;
	  store(command_line_parser(argc, argv).options(cmdLineOptions)
	            .allow_unregistered().run(), input_map_);
	  notify(input_map_);

	  // Required options
	  options_description requiredOptions("Required arguments");
	  requiredOptions.add_options()
	    ("jets_number,j",value<int>()->required(), "Number of requird jets")
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

	  analysis_ = new Analysis(input_map_["input_files"].as<std::string>());

}



JetAnalysisBase::~JetAnalysisBase() {
	// TODO Auto-generated destructor stub
}
/**/
