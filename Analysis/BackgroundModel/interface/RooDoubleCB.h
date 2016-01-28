#ifndef Analysis_BackgroundModel_RooDoubleCB_h
#define Analysis_BackgroundModel_RooDoubleCB_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"

namespace analysis {
  namespace backgroundmodel {

    class RooDoubleCB : public RooAbsPdf {
    public:
      inline RooDoubleCB() = default;
      RooDoubleCB(const char* name, const char* title,
                  RooAbsReal& x,
                  RooAbsReal& mean,
                  RooAbsReal& width,
                  RooAbsReal& alpha1,
                  RooAbsReal& n1,
                  RooAbsReal& alpha2,
                  RooAbsReal& n2
                  );
      RooDoubleCB(const RooDoubleCB& other, const char* name = 0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooDoubleCB() {};
      int getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,
                                const char* name = 0) const;
      double analyticalIntegral(int code, const char* rangeName = 0) const;

    protected:
      double evaluate() const;

      RooRealProxy x_;
      RooRealProxy mean_;
      RooRealProxy width_;
      RooRealProxy alpha1_;
      RooRealProxy n1_;
      RooRealProxy alpha2_;
      RooRealProxy n2_;

    private:
      ClassDef(RooDoubleCB,1);
    };

  }
}

#endif  // Analysis_BackgroundModel_RooDoubleCB_h
