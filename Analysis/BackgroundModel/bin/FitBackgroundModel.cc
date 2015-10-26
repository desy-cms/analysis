#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include "TSystem.h"
#include "RooFitResult.h"
#include "Analysis/BackgroundModel/interface/DataContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


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
    ("background_model,b", po::value<std::string>()->required(),
     "Name of the background model.")
    ("modify_param,m", po::value<std::vector<std::string> >()->composing(),
     "Modify parameters as follows: "
     "\"name: [start=<value>], [min=<value>], [max=<value>], "
     "[constant], [floating]\"")
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

  ab::DataContainer input(vm["input_file"].as<std::string>());
  input.show();

  ab::FitContainer fitter = ab::FitContainer(input);
  std::vector<ab::ParamModifier> bkgModifiers =
    vm["modify_param"].empty() ? std::vector<ab::ParamModifier>() :
    ab::parseModifiers(vm["modify_param"].as<std::vector<std::string> >());
  fitter.setModel("background", vm["background_model"].as<std::string>(), bkgModifiers);

  std::unique_ptr<RooFitResult> bkgOnlyFit = fitter.backgroundOnlyFit();
  std::cout << "\nCorrelation matrix of background-only fit:" << std::endl;
  bkgOnlyFit->correlationMatrix().Print("v");

  for (const auto& m : bkgModifiers) m.show();

  return 0;
}
