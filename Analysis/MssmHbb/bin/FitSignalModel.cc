#include <vector>
#include "stdlib.h"
#include <memory>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooWorkspace.h"

#include "TFile.h"
#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/Tools.h"
#include "Analysis/BackgroundModel/interface/ProbabilityDensityFunctions.h"


namespace po = boost::program_options;
namespace ab = analysis::backgroundmodel;

typedef std::unique_ptr<TFile> pTFile;

using namespace analysis::backgroundmodel;


int main(int argc, char* argv[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	std::map<int,std::string> signal;
	signal[700] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[900] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[1100]= "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[1300]= "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[500] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[600] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[200] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root";
	signal[250] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root";
	signal[300] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root";
	signal[350] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root";
	signal[400] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root";

  const int verbosity = 6;
  std::string histo_name = "bbH_Mbb";

  for(const auto & mass : signal){
	  TFile f((cmsswBase + mass.second).c_str(),"read");
	  TH1D& hSignal = (TH1D&) f.Get(histo_name.c_str());
//	  ab::HistContainer histContainer(cmsswBase + mass.second);//MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
//	  histContainer.Rebin(1);
//	  ab::FitContainer fitter(histContainer,cmsswBase + "/src/Analysis/MssmHbb/test/" + "mass_point_" + std::to_string(mass.first));
	  ab::FitContainer fitter(hSignal,cmsswBase + "/src/Analysis/MssmHbb/test/" + "mass_point_test" + std::to_string(mass.first));
	  fitter.verbosity(verbosity - 1);

	  std::string model = "quadgausexp";//"quadgausexp";
	  fitter.setModel(ab::FitContainer::Type::signal,model);
	//  fitter.profileModel(ab::FitContainer::Type::signal);

	  std::unique_ptr<RooFitResult> Signalfit = fitter.FitSignal(model);
  }

  /*
  ab::HistContainer histContainer(cmsswBase + signal.at(0));//MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
  histContainer.Rebin(1);
  ab::FitContainer fitter(histContainer,cmsswBase + "/src/Analysis/MssmHbb/test");
  fitter.verbosity(verbosity - 1);

  std::string model = "quadgausexp";//"quadgausexp";
  fitter.setModel(ab::FitContainer::Type::signal,model);
//  fitter.profileModel(ab::FitContainer::Type::signal);

  std::unique_ptr<RooFitResult> Signalfit = fitter.FitSignal(model);

  /*
  if (bkgOnlyFit) {
    std::cout << "\nCorrelation matrix of background-only fit:" << std::endl;
    if (&(bkgOnlyFit->correlationMatrix()) != nullptr) {
      bkgOnlyFit->correlationMatrix().Print("v");

      TMatrixDSymEigen CM = bkgOnlyFit->correlationMatrix();
      TMatrixD CM_eigen = CM.GetEigenVectors();
      std::cout << "Eigenvectors of Correlation Matrix" << std::endl;
      CM_eigen.Print("v");

    } else {
      std::cout << ">>> no correlation matrix available" << std::endl;
      returnValue = 1;
    }
    if (&(bkgOnlyFit->covarianceMatrix()) != nullptr) {
      std::cout << "Covariance Matrix" << std::endl;
      bkgOnlyFit->covarianceMatrix().Print("v");

      TMatrixDSymEigen VM = bkgOnlyFit->covarianceMatrix();
      TMatrixD VM_eigen = VM.GetEigenVectors();
      std::cout << "Eigenvectors of Covariance Matrix" << std::endl;
      VM_eigen.Print("v");
    } else {
      std::cout << ">>> no covariance matrix available" << std::endl;
      returnValue = 1;
    }
  } else {
    returnValue = 1;
  } */

  return 0;

}
