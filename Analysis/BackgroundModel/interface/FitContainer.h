#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include <string>
#include "TH1.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"


namespace analysis {
  namespace backgroundmodel {

    class FitContainer {
    public:
      FitContainer(TH1* data, TH1* signal, TH1* background);
      virtual ~FitContainer();

      void setModel(const std::string& type, const std::string& model);
      RooFitResult* backgroundOnlyFit();

    private:
      std::string getOutputPath_(const std::string& subdirectory = "");
      bool checkType_(const std::string& type);
      double getMaxPosition_(const RooDataHist& data);
      void setNovosibirsk_(const std::string& type);

      const std::string plotDir_;
      RooWorkspace workspace_;
      RooRealVar mbb_;
      RooDataHist data_;
      RooDataHist signal_;
      RooDataHist background_;
    };

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
