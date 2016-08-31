#ifndef Analysis_BackgroundModel_RooGausExp_h
#define Analysis_BackgroundModel_RooGausExp_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooGausExp : public RooAbsPdf {
    public:
      inline RooGausExp() = default;
      RooGausExp(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& mean,
                    RooAbsReal& sigma,
                    RooAbsReal& right);
      RooGausExp(const RooGausExp& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooGausExp() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy mean_ ;
      RooRealProxy sigma_ ;
      RooRealProxy right_ ;

    private:
      ClassDef(RooGausExp,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExpGausExp_h
