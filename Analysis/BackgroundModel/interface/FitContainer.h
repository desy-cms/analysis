#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include "TH1.h"
#include "RooRealVar.h"
#include "RooDataHist.h"


namespace analysis {
  namespace backgroundmodel {

    class FitContainer {
    public:
      FitContainer(TH1* data, TH1* signal, TH1* background);
      virtual ~FitContainer();

    private:
      RooRealVar mbb_;
      RooDataHist data_;
      RooDataHist signal_;
      RooDataHist background_;
    };

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
