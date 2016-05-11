#include <iostream>
#include <fstream>
#include <sstream>
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
#include "Analysis/MssmHbb/interface/TriggerEfficiency.h"

namespace fs = boost::filesystem;

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;
using namespace boost::program_options;

// =============================================================================================
int main(int argc, char * argv[])
{

        string config_file;
    	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

    	//Declare a group of general options
    	options_description generalOptions("General options");
    	generalOptions.add_options()
				("help,h", "Produce help message.")
				("verbose,v", value<int>()->default_value(0), "More verbose output.")
				;

		// Declare a group of options that will be allowed only with config file
		options_description configOptions("Config file options");
		configOptions.add_options()
				("config,c", value<string>(&config_file), "Name of a cfg file.")
				;
        // Declare a group of options that will be allowed only on command line
		options_description cmdLineOptions("CMD Optional arguments");
		cmdLineOptions.add_options()
		    ("input_files,i", value<std::string>()->default_value("/nfs/dust/cms/user/shevchen/samples/miniaod/76X/filtered/MSSMHbb/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.txt"),"Input .txt file with NTuples")
		    ("output_file,o", value<std::string>()->default_value(""), "Output file name, if not specified will be created automatically")
			("json_file,j", value<std::string>()
					 ->default_value(cmsswBase+"/src/Analysis/MssmHbb/bin/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt"),
			 "Json file")
			("lumi,l",value<double>()->default_value(2318.278),"Data luminosity")
		    ;

		  // Required options
		  options_description requiredOptions("CMD Required arguments");
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

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        options_description hidden("Hidden options");
        hidden.add_options()
            ("test", value<int>()->default_value(1),"if test specify 1. Default 0.")
            ;

        options_description all_options;
        all_options.add(generalOptions).add(configOptions).add(cmdLineOptions).add(requiredOptions).add(hidden);

        options_description cmdline_options;
        cmdline_options.add(generalOptions).add(cmdLineOptions).add(requiredOptions).add(hidden);

        options_description config_file_options;
        config_file_options.add(generalOptions).add(configOptions).add(hidden);

        options_description visible("Allowed options");
        visible.add(generalOptions).add(configOptions).add(cmdLineOptions).add(requiredOptions);

        variables_map output_vm;

        store(command_line_parser(argc, argv).options(config_file_options).allow_unregistered().run(), output_vm);
        notify(output_vm);

        if (output_vm.count("help")) {
    	    std::cout << visible << std::endl;
    	    exit(0);
    	 }

        if(!output_vm.count("config"))
        {
        	//If no config file were specified - try cmd input
            store(parse_command_line(argc, argv,cmdline_options), output_vm);
//        	store(command_line_parser(argc, argv).options(cmdline_options).run(), output_vm);
            try {
            	notify(output_vm);
            }	catch (const required_option& ex) {
    		    std::cerr << ex.what() << std::endl;
    		    exit(1);
    		  }
        }
        else
        {
        	std::ifstream ifs(config_file.c_str());
        	if (!ifs) {
        		cerr<<"wrong file name. Exception."<<endl;;
        		exit(1);
        	}
        	else {
            store(parse_config_file(ifs, cmdline_options), output_vm);
            notify(output_vm);
        	}
        }


//	  analysis_ = new Analysis(input_map_["input_files"].as<std::string>());
	  std::string  selection_ =  output_vm["selection"].as<std::string>();
	  std::string  analysis_  =  output_vm["analysis"].as<std::string>();
	  std::string  inputList_ =  output_vm["input_files"].as<std::string>();
	  std::string  output_    =  output_vm["output_file"].as<std::string>();
	  std::string json_file_ =  output_vm["json_file"].as<std::string>();
	  auto lowM_        =  output_vm["trigger"].as<int>();
	  auto test_		 =  output_vm["test"].as<int>();
	  auto lumi_				= output_vm["lumi"].as<double>();

	  //Check whether input file contain only .root files or .txt
	  std::string line;
	  std::ifstream infile(inputList_);
	  if(infile){
		  while(std::getline(infile, line)){
			  fs::path p(line);
			  try{
				  if(fs::exists(p)){
					  if(fs::extension(p) == ".root"){
						  std::cout<<"THIS is ROOT"<<std::endl;

	//					  if(boost::iequals(selection_,"mssmhbb")){
	//						  MssmHbbSignal analysis(inputList_,lumi_,lowM_,test_);//,analysis_);
	//						  analysis.runAnalysis(json_file_,output_,100);
	//
	//					  }
	//					  else if (boost::iequals(selection_,"trigger")){
	//						  ;
	//					  }
	//					  else if (boost::iequals(selection_,"2bjet")){
	//						  selectionDoubleB analysis(inputList_,lumi_,lowM_);
	//					  }
						  break;
					  }
					  else if (fs::extension(p) == ".txt"){

						  if(boost::iequals(selection_,"mssmhbb")){
						  MssmHbbSignal analysis(line,lumi_,lowM_,test_);//,analysis_);
						  analysis.runAnalysis(json_file_,output_,100);

						  }
						  else if (boost::iequals(selection_,"trigger")){
							  TriggerEfficiency analysis(line,lumi_,lowM_,test_);
							  analysis.runAnalysis(json_file_,output_,100);
						  }
						  else if (boost::iequals(selection_,"2bjet")){
							  selectionDoubleB analysis(inputList_,lumi_,lowM_);
						  }

					  }
					  else {
						  std::cerr<<"Wrong files extension"<<std::endl;
						  exit(4);
					  }

				  }
				  else{
					  std::cerr<<"Input file is empty"<<std::endl;
					  exit(3);
				  }
			  }
			  catch (const fs::filesystem_error& ex) {
			      cout << ex.what() << '\n';
			    }
		  }

	  }
	  else {
		  std::cerr<<"Error wrong file name."<<std::endl;
		  exit(2);
	  }

	  /*

	  //Start to apply input:
	  //TODO: Factoring pattern should be applied: http://stackoverflow.com/questions/34519878/decide-what-constructor-call-based-on-user-input/34520081


/**/

	return 0;
}


/*
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-PromptReco-v4.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015C_25ns-05Oct2015-v1.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-05Oct2015-v1.root
*/
