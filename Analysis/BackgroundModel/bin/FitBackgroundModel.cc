#include <iostream>
#include <boost/program_options.hpp>
#include "TSystem.h"

#include "Analysis/BackgroundModel/interface/DataContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"


namespace po = boost::program_options;
namespace ab = analysis::backgroundmodel;

int main(int argc, char *argv[]) {
  std::cout << "Fitting Background Model..." << std::endl;

  const std::string cmsswBase(gSystem->Getenv("CMSSW_BASE"));

  po::options_description cmdLineOptions("Command line options");
  cmdLineOptions.add_options()
    ("help,h", "Produce help message.")
    ("input_file,i", po::value<std::string>()
     ->default_value(cmsswBase+"/src/Analysis/BackgroundModel/"
		     "data/HIG14017_HighMass2012_Packed_M350_inputs.root"),
     "ROOT file from which input histograms are retrieved.")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmdLineOptions), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cerr << cmdLineOptions << std::endl;
    return 1;
  }

  try {
    po::notify(vm);
  }
  catch(const boost::program_options::required_option& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  ab::DataContainer input = ab::DataContainer(vm["input_file"].as<std::string>());

  std::cout << "Data events:            " << input.data()->Integral() << std::endl;
  std::cout << "Expected signal events: " << input.bbH()->Integral() << std::endl;
  std::cout << "Background events:      " << input.background()->Integral() << std::endl;

  ab::FitContainer fitter = ab::FitContainer(input.data(), input.bbH(), input.background());

  return 0;
}
