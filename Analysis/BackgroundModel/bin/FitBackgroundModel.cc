#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "RooFitResult.h"
#include "Analysis/BackgroundModel/interface/DataContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


namespace po = boost::program_options;
namespace ab = analysis::backgroundmodel;


int main(int argc, char *argv[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  // general command line options
  po::options_description cmdLineOptions("Optional arguments");
  cmdLineOptions.add_options()
    ("help,h", "Produce help message.")
    ("verbose,v", po::value<int>()->default_value(0), "More verbose output.")
    ("profile,p", "Create profile likelihoods of the fit parameters.")
    ("list_parameters,l", "List parameters of the chosen fit models and exit.")
    ("input_file,i", po::value<std::string>()
     ->default_value(cmsswBase+"/src/Analysis/BackgroundModel/"
		     "data/HIG14017_HighMass2012_Packed_M350_inputs.root"),
     "ROOT file from which input histograms are retrieved.")
    ("fit_min", po::value<float>(), "Lower bound of the fit range.")
    ("fit_max", po::value<float>(), "Upper bound of the fit range.")
    ("modify_param,m", po::value<std::vector<std::string> >()->composing()
     ->default_value(std::vector<std::string>(), ""),
     "Modify parameters as follows: "
     "\"name: [start=<value>,] [min=<value>,] [max=<value>,] "
     "[constant,] [floating]\"")
    ;
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(cmdLineOptions)
	    .allow_unregistered().run(), vm);
  po::notify(vm);


  // now add required options
  po::options_description requiredOptions("Required arguments");
  requiredOptions.add_options()
    ("background_model,b", po::value<std::string>()->required(),
     ("Name of the background model (" +
      boost::algorithm::join(ab::FitContainer::availableModels(), ", ") +
      ").").c_str())
    ;
  po::store(po::command_line_parser(argc, argv).options(requiredOptions)
	    .allow_unregistered().run(), vm);


  po::options_description allOptions("Allowed arguments");
  allOptions.add(cmdLineOptions).add(requiredOptions);

  // check for help flag before checking for required options
  if (vm.count("help")) {
    std::cerr << allOptions << std::endl;
    return 0;
  }

  try {
    po::notify(vm);
  } catch (const po::required_option& ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  auto verbosity = vm["verbose"].as<int>();
  if (verbosity < 1) {
    RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  } else if (verbosity == 1) {
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  } else {
    // highest level of verbosity reached
  }

  std::cout << "Fitting Background Model..." << std::endl;

  ab::DataContainer input(vm["input_file"].as<std::string>());
  input.show();

  ab::FitContainer fitter = ab::FitContainer(input).verbosity(verbosity - 1);
  if (!vm["fit_min"].empty()) fitter.fitRangeMin(vm["fit_min"].as<float>());
  if (!vm["fit_max"].empty()) fitter.fitRangeMax(vm["fit_max"].as<float>());
  std::vector<ab::ParamModifier> bkgModifiers =
    ab::parseModifiers(vm["modify_param"].as<std::vector<std::string> >());
  fitter.setModel(ab::FitContainer::Type::background,
		  vm["background_model"].as<std::string>(),
  		  bkgModifiers);

  if (vm.count("list_parameters")) {
    fitter.showModels();
    return 0;
  }

  std::unique_ptr<RooFitResult> bkgOnlyFit = fitter.backgroundOnlyFit();
  if (bkgOnlyFit) {
    std::cout << "\nCorrelation matrix of background-only fit:" << std::endl;
    if (&(bkgOnlyFit->correlationMatrix()) != nullptr)
      bkgOnlyFit->correlationMatrix().Print("v");
    else
      std::cout << ">>> no correlation matrix available" << std::endl;
  }

  if (vm.count("profile")) {
    fitter.profileModel(ab::FitContainer::Type::background);
  }

  for (const auto& m : bkgModifiers) m.show();

  return 0;
}
