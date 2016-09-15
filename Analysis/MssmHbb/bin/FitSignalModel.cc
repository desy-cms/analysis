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
#include "TFile.h"
#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


namespace po = boost::program_options;
namespace ab = analysis::backgroundmodel;

typedef std::unique_ptr<TFile> pTFile;

using namespace analysis::backgroundmodel;


int main(int argc, char* argv[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  /*std::map<std::string, pTFile> fInput;
  fInput["300"] = std::make_unique<TFile>( (cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root").c_str(),"read" );
  TH1D * hInput = (TH1D*) fInput["300"]->Get("bbH_Mbb");
*/
  const int verbosity = 3;

  std::cout<<"Go to histContainer"<<std::endl;
  ab::HistContainer histContainer(cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");//MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
  histContainer.Rebin(2);
  std::cout<<"Go to FitContainer"<<std::endl;
  ab::FitContainer fitter(histContainer,"/src/Analysis/MssmHbb/test");
  std::cout<<"After FitContainer"<<std::endl;
  fitter.verbosity(verbosity - 1);
  std::cout<<"After verbosity"<<std::endl;

//  fitter.fitRangeMin(240.);
  std::cout<<"After fitRangeMin"<<std::endl;
//  fitter.fitRangeMax(1700.);
  std::cout<<"After fitRangeMax"<<std::endl;
  std::cout<<"Before setModel"<<std::endl;
  std::string model = "relbreitwigner";//"quadgausexp";
  fitter.setModel(ab::FitContainer::Type::signal,model);
//  fitter.profileModel(ab::FitContainer::Type::signal);

  std::cout<<"Start fitting"<<std::endl;
  std::unique_ptr<RooFitResult> bkgOnlyFit = fitter.FitSignal(model);
  std::cout<<"ALMOST DONE"<<std::endl;
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
