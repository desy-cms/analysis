#include "Analysis/QCDSideBand/interface/ProgramOptions.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

ProgramOptions::ProgramOptions(int argc, char* argv[]) :
 // Set default for program args
  iDir(""),
  oDir(""),
  datasetFile("")//,
  //lumi(0.),
  //wTauMethod(2),
  //qcdMethod(3),
  //doMCFMWeights(false),
  //leptCorr("central")
 {

  namespace po = boost::program_options;

  // program options
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Display this message")
    ("outdir,o", po::value<std::string>(), "Output directory")
    ("indir,i", po::value<std::string>(), "Input directory")
    ("datasets,f", po::value<std::string>(), "Datasets file");
    //("lumi,l", po::value<double>(), "Integrated luminosity")
    //("qcd,q", po::value<int>(), "QCD method")
    //("tau,t", po::value<int>(), "W->tau method")
    //("mcfmWeights,m", po::value<bool>(), "apply MCFM re-weighting")
    //("leptCorr,c", po::value<std::string>(), "apply lepton corrections, options are off, central (default if nothing specified), elUp, elDown, muUp, muDown. You can't do both (yet).");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);  
  po::notify(vm);

  // help
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    std::exit(1);
  }
  if (vm.count("outdir"))     oDir = vm["outdir"].as<std::string>();
  if (vm.count("indir"))      iDir=vm["indir"].as<std::string>();
  if (vm.count("datasets"))   datasetFile=vm["datasets"].as<std::string>();
  //if (vm.count("lumi"))       lumi=vm["lumi"].as<double>();
  //if (vm.count("qcd"))        qcdMethod=vm["qcd"].as<int>();
  //if (vm.count("tau"))        wTauMethod=vm["tau"].as<int>();
  //if (vm.count("mcfmWeights")) doMCFMWeights=vm["mcfmWeights"].as<bool>();
  //if (vm.count("leptCorr"))   leptCorr=vm["leptCorr"].as<std::string>();
 
  // create output directory if it doesn't exist already  
  if (oDir!="") {
    boost::filesystem::path opath(oDir);
    if (!exists(opath)) {
      std::cout << "Creating output directory : " << oDir << std::endl;
      boost::filesystem::create_directory(opath);
    }
    std::cout << "Writing results to " << oDir << std::endl;
  }

  //std::cout << "Integrated luminosity : " << lumi << " pb-1" << std::endl;
  std::cout << std::endl;

}

ProgramOptions::~ProgramOptions() {

}















