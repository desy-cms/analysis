#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include <vector>
#include <string>
#include <memory>
#include "TH1.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "Analysis/BackgroundModel/interface/DataContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"


namespace analysis {
  namespace backgroundmodel {

    class FitContainer {
    public:
      FitContainer(TH1& data, TH1& signal, TH1& background);
      FitContainer(const DataContainer& container);
      virtual ~FitContainer();

      inline static const std::vector<std::string>& availableModels() {
	return availableModels_; };
      void setModel(const std::string& type, const std::string& model);
      void setModel(const std::string& type, const std::string& model,
		    const std::vector<ParamModifier>& modifiers);
      std::unique_ptr<RooFitResult> backgroundOnlyFit();

    private:
      // methods to set the fit model
      static std::unique_ptr<RooArgList>
      getCoefficients_(const int numCoeffs, const std::string& name);
      void setNovosibirsk_(const std::string& type);
      void setCrystalBall_(const std::string& type);
      void setNovoEffProd_(const std::string& type);
      void setExpEffProd_(const std::string& type);
      void setDoubleCB_(const std::string& type);
      void setBernstein_(const std::string& type, const int numCoeffs);
      void setChebychev_(const std::string& type, const int numCoeffs);
      void setBernEffProd_(const std::string& type, const int numCoeffs);
      static const std::vector<std::string> availableModels_;

      // internal methods
      std::string getOutputPath_(const std::string& subdirectory = "");
      void prepareCanvas_(TCanvas& raw);
      void prepareFrame_(RooPlot& raw);
      bool checkType_(const std::string& type);
      double getMaxPosition_(const RooDataHist& data);
      int getNonZeroBins_(const RooDataHist& data);
      bool applyModifiers_(RooAbsPdf& pdf,
			   const std::vector<ParamModifier>& modifiers);

      // data member
      static const int defaultNumberOfCoefficients_;
      std::string plotDir_;
      RooWorkspace workspace_;
      RooRealVar mbb_;
      RooDataHist data_;
      RooDataHist signal_;
      RooDataHist background_;
    };

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
