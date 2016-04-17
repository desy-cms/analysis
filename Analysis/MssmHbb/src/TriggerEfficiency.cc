/**\class MssmHbb MssmHbb.cc Analysis/Tools/src/MssmHbb.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include <iostream>
// 
// user include files
#include "Analysis/MssmHbb/interface/TriggerEfficiency.h"

//Delcare functions

//
// class declaration
//

using namespace analysis;
using namespace analysis::mssmhbb;
using namespace boost::program_options;

//
// constructors and destructor
//
TriggerEfficiency::TriggerEfficiency(const int & argc, char * argv[]){

	// Get CMSSW version
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

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
	    		->default_value(cmsswBase+"/src/Analysis/MssmHbb/output/kinematicTriggerEfficiency.root"),
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

TriggerEfficiency::~TriggerEfficiency()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

