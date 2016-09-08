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
#include "TClass.h"
#include "TKey.h"
#include "TApplication.h"
#include "TROOT.h"

#include "Analysis/MssmHbb/interface/json.h"
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"
#include "Analysis/MssmHbb/interface/Weights.h"
//#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"
#include "Analysis/MssmHbb/interface/selectionDoubleB.h"
//#include "Analysis/MssmHbb/interface/TriggerEfficiency.h"
//#include "Analysis/MssmHbb/interface/X750Search.h"
//#include "Analysis/MssmHbb/interface/BgStudy.h"
#include "Analysis/MssmHbb/interface/DataMcComparison.h"
//#include "Analysis/MssmHbb/interface/bbx.h"
//#include "Analysis/MssmHbb/interface/CutOptimisation.h"
//#include "Analysis/MssmHbb/macros/Drawer/FitMassDistribution.C"

namespace fs = boost::filesystem;

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;
using namespace boost::program_options;

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

void addBackgroundTemplate(const std::string & signal_template, const std::string & bg_template, const int & nevents_to_scale = -1, const std::string & bgHisto = "QCD_Mbb");

// =============================================================================================
int main(int argc, char * argv[])
{

        string config_file;


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
				  "  MssmHbb 	- main analysis script, apply 3b signal selection; \n"
				  "  2bJet   	- apply 2b selection; \n"
				  "  reverse 	- apply 3b background selection (reverse btag for third jet); \n"
				  "  Trigger 	- calculate kinematic trigger efficiencies; \n"
				  "  X750	    - Trigger Efficiency for X750 Search; \n"
				  "  bgstudy 	- Bg Study asked by Chayanit; \n"
				  "  datavsmc	- 2b selection for Data vs MC comparison; \n"
				  "  cut_opt    - cut optimisation in double/triple-b selection; \n"
				  "  fitter     - Fit Signal Templates with Analytical Curves.")
				  ("nJets,n", value<int>()->required(),
				  "  2 - double-jet selection; \n"
				  "  3 - triple-jet selection."	  )
				  ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        options_description hidden("Hidden options");
        hidden.add_options()
            ("test", value<int>()->default_value(0),"if test specify 1. Default 0.")
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
	  std::string  inputList_ =  output_vm["input_files"].as<std::string>();
	  std::string  output_    =  output_vm["output_file"].as<std::string>();
	  std::string json_file_ =  output_vm["json_file"].as<std::string>();
	  auto lowM_        =  output_vm["trigger"].as<int>();
	  auto test_		 =  output_vm["test"].as<int>();
	  auto lumi_				= output_vm["lumi"].as<double>();
	  int nJets_		= output_vm["nJets"].as<int>();

	  //Check whether input file contain only .root files or .txt
	  std::string line;
	  std::ifstream infile(inputList_);
	  if(infile){
		  while(std::getline(infile, line)){
			  fs::path p(line);
			  try{
				  std::string txt_file;
				  if(fs::exists(p)){
					  if(fs::extension(p) == ".root"){
						  std::cout<<"THIS are ROOT files in .txt"<<std::endl;
						  txt_file = inputList_;
					  }
					  else if (fs::extension(p) == ".txt"){
						  std::cout<<"this is txt file that conatin .gtxt files inside"<<std::endl;
						  txt_file = line;
					  }
					  else {
						  std::cerr<<"Wrong files extension"<<std::endl;
						  exit(4);
					  }

/*					  if(boost::iequals(selection_,"mssmhbb")){
						  MssmHbbSignal analysis(txt_file,lumi_,lowM_,test_);//,analysis_);
						  analysis.runAnalysis(json_file_,output_,100);
						  if(analysis.isSignalMC()){
							  analysis.addStatErrorsTemplates(10);
							  analysis.getOutputFile()->Close();
							  std::string output_name = analysis.getOutputFile()->GetName();
							  if(analysis.getLowM()){
								  addBackgroundTemplate(output_name, "input_corrections/QCD_Templates_3M_lowM.root",19251);
							  }
							  else {
							  	addBackgroundTemplate(output_name, "input_corrections/QCD_Templates_Novo_3M_highM.root");
							  }
						  }


					  }
					  else if (boost::iequals(selection_,"trigger")){
						  TriggerEfficiency analysis(txt_file,lumi_,lowM_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
					  else */if (boost::iequals(selection_,"2bjet")){
						  selectionDoubleB analysis(txt_file,lumi_,lowM_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
/*					  else if (boost::iequals(selection_,"X750")){
						  X750Search analysis(txt_file,lumi_,lowM_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
					  else if (boost::iequals(selection_,"bgstudy")){
						  BgStudy analysis(txt_file,lumi_,lowM_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
*/					  else if (boost::iequals(selection_,"datavsmc")){
						  DataMcComparison analysis(txt_file,lumi_,lowM_,nJets_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
/*					  else if (boost::iequals(selection_,"bbx")){
						  bbx analysis(txt_file,lumi_,lowM_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
					  else if (boost::iequals(selection_,"cut_opt")){
						  CutOptimisation analysis(txt_file,lumi_,lowM_,nJets_,test_);
						  analysis.runAnalysis(json_file_,output_,100);
					  }
*/
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


*/

	return 0;
}

void addBackgroundTemplate(const std::string & signal_template, const std::string & bg_template, const int & nevents_to_scale, const std::string & bgHisto){

	TFile fsignal(signal_template.c_str(),"UPDATE");	// Opening file for writing;
	if(fsignal.IsZombie()){
		std::cerr<<"Error opening file: "<<signal_template<<" for merging"<<std::endl;
		exit(0);
	}
	TFile fbg(bg_template.c_str(),"READ");
	if(fbg.IsZombie()){
		std::cerr<<"Error opening file: "<<bg_template<<" for merging"<<std::endl;
		exit(1);
	}
	//Check whether
	if(!fbg.GetListOfKeys()->Contains(bgHisto.c_str())){
		std::cerr<<"No histo: "<<bgHisto.c_str()<<" in file: "<<bg_template<<std::endl;
		exit(2);
	}

	//Read all objects from Bg templates file
	TIter next(fbg.GetListOfKeys());
	TKey *key;
	std::string objName;
	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		objName = h->GetName();
		if(nevents_to_scale != -1){
			std::cout<<"Scale Bg histograms to: "<<nevents_to_scale/h->Integral()<<std::endl;
			h->Scale(nevents_to_scale/h->Integral());
		}
		if( objName.find(bgHisto) == std::string::npos) continue;
		std::cout<<h->GetName()<<std::endl;
		fsignal.cd();
		h->Write(h->GetName());
		if( objName == bgHisto){
			h->Write("data_obs");
		}
	}

}

