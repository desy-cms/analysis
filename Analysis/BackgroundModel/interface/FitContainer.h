#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include <vector>
#include <string>
#include <memory>
#include "TH1.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCurve.h"
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
      enum class Type { signal, background };
      inline static std::string toString(const Type& type) {
	switch (type) {
	case Type::signal: return "signal";
	case Type::background: return "background";
	};
	return "";		// to silence compiler
      };

      FitContainer(const TH1& data, const TH1& signal, const TH1& background);
      FitContainer(const DataContainer& container);
      virtual ~FitContainer();

      FitContainer& verbosity(int level);

      inline static const std::vector<std::string>& availableModels() {
	return availableModels_; };
      void setModel(const Type& type, const std::string& model);
      void setModel(const Type& type, const std::string& model,
		    const std::vector<ParamModifier>& modifiers);
      std::unique_ptr<RooFitResult> backgroundOnlyFit();
      void profileModel(const Type& type);
      void showModels() const;

    private:
      // methods to set the fit model
      static std::unique_ptr<RooArgList>
      getCoefficients_(const int numCoeffs, const std::string& name);
      void setNovosibirsk_(const Type& type);
      void setCrystalBall_(const Type& type);
      void setNovoEffProd_(const Type& type);
      void setCBEffProd_(const Type& type);
      void setExpEffProd_(const Type& type);
      void setDoubleCB_(const Type& type);
      void setExpGausExp_(const Type& type);
      void setExpBWExp_(const Type& type);
      void setBukin_(const Type& type);
      void setBernstein_(const Type& type, const int numCoeffs);
      void setChebychev_(const Type& type, const int numCoeffs);
      void setBernEffProd_(const Type& type, const int numCoeffs);
      static const std::vector<std::string> availableModels_;

      // internal methods
      static std::string getOutputPath_(const std::string& subdirectory = "");
      static void prepareCanvas_(TCanvas& raw);
      static void prepareFrame_(RooPlot& raw);
      double getMaxPosition_(const RooDataHist& data);
      int getNonZeroBins_(const RooDataHist& data);
      double chiSquare_(const RooDataHist& data, const RooCurve& fit);
      bool applyModifiers_(RooAbsPdf& pdf,
			   const std::vector<ParamModifier>& modifiers);

      // data member
      static const int defaultNumberOfCoefficients_;
      int verbosity_;
      std::string plotDir_;
      std::string workspaceDir_;
      RooWorkspace workspace_;
      RooRealVar mbb_;
      RooDataHist data_;
      RooDataHist signal_;
      RooDataHist background_;
    };

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
