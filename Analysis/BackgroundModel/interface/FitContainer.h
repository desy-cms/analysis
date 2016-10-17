#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include <vector>
#include <string>
#include <memory>
#include <array>

#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TObject.h"
#include "TString.h"
#include "TIterator.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDEigen.h"
#include "TMatrixDSymEigen.h"
#include "RooMinimizer.h"

#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooList.h"

#include "RooGaussian.h"
#include "RooAddPdf.h"

#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/TreeContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/ProbabilityDensityFunctions.h"


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
        return "";              // to silence compiler
      };

      FitContainer(const TH1* data, const TH1* signal, const TH1* background,
		   const std::string& outputDir = defaultOutputDir_);
      FitContainer(const TH1* data, const std::string& outputDir = defaultOutputDir_, const std::string & type = "background");
      FitContainer(TTree& data, const std::string& outputDir = defaultOutputDir_);
      FitContainer(const HistContainer& container,
		   const std::string& outputDir = defaultOutputDir_);
      FitContainer(const TreeContainer& container,
		   const std::string& outputDir = defaultOutputDir_);
      virtual ~FitContainer();
      void initialize();

      FitContainer(const FitContainer&) = default;
      FitContainer& operator=(const FitContainer&) = default;
      FitContainer(FitContainer&&) = default;
      FitContainer& operator=(FitContainer&&) = default;

      FitContainer& verbosity(int level);
      FitContainer& fitRangeMin(float min);
      FitContainer& fitRangeMax(float max);

      void setModel(const Type& type, const std::string& model);
      void setModel(const Type& type, const std::string& model,
                    const std::vector<ParamModifier>& modifiers);
      std::unique_ptr<RooFitResult> backgroundOnlyFit(const std::string& model);
      std::unique_ptr<RooFitResult> FitSignal(const std::string & model);

      void profileModel(const Type& type);
      void showModels() const;
      void Import(const RooAbsArg& inArg);
      void Write();

    private:

      //Private constructor to avoid code duplication for private members initialisation
      FitContainer(const std::string& outputDir);

      // methods to set the fit model
      static std::unique_ptr<RooArgList>
      getCoefficients_(const int numCoeffs, const std::string& name);
      double getPeakStart_(const Type& type, double max);
      double getPeakStart_(const Type& type);
      double getMaxPosition_(const RooAbsData& data);

      // internal methods
      static void prepareCanvas_(TCanvas& raw);
      static void prepareFrame_(RooPlot& raw);
      std::string getOutputPath_(const std::string& subdirectory = "");
      int getNonZeroBins_(const RooAbsData& data);
      int getBlindedBins_(const RooAbsData& data, double blind_lowEdge, double blind_highEdge);
      double chiSquare_(const RooAbsData& data, const RooCurve& fit);
      double chiSquare_(const RooAbsData& data, const RooCurve& fit, double blind_LowEdge, double blind_HighEdge, int nFitParam);
      // chiSquare by CA
      double chiSquare_CA(const RooPlot& frame, const char* curvename, const char* histname, int nFitParam, double blind_lowEdge, double blind_highEdge);
      bool applyModifiers_(RooAbsPdf& pdf,
                           const std::vector<ParamModifier>& modifiers);

      // data member
      static const std::string defaultOutputDir_;
      bool initialized_;
      bool written_;
      bool splitrange_;
      std::string outputDir_;
      std::string plotDir_;
      std::string workspaceDir_;
      std::string fullRangeId_;
      std::string fitRangeId_;
      std::string fitRangeLowId_;	// for split range by CA
      //std::string fitRangeMedId_;       // for split range by CA
      std::string fitRangeHighId_;	// for split range by CA
      std::string fitSplRangeId_;	// for split range by CA
      double blind_lowEdge_;
      double blind_highEdge_;
      int verbosity_;
      RooWorkspace workspace_;
      std::string outRootFileName_;
      std::string mbb_;
      std::string weight_;
      std::string data_;
      std::string signal_;
      std::string bkg_;
      float fitRangeMin_;
      float fitRangeMax_;
      TTree bkgOnlyFit_;
      float chi2BkgOnly_;
      float normChi2BkgOnly_;
      int ndfBkgOnly_;
      double covMatrix_[100];
      double eigenVector_[100];
      int nbins_;
    };

    inline void FitContainer::Import(const RooAbsArg& inArg){ workspace_.import(inArg);}
    inline void FitContainer::Write(){ if(!written_) { workspace_.writeToFile(outRootFileName_.c_str()); written_ = true;}   }

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
