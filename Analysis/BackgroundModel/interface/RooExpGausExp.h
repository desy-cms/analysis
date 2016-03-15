#ifndef Analysis_BackgroundModel_RooExpGausExp_h
#define Analysis_BackgroundModel_RooExpGausExp_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooExpGausExp : public RooAbsPdf {
    public:
      inline RooExpGausExp() = default;
      RooExpGausExp(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& mean,
                    RooAbsReal& sigma,
                    RooAbsReal& left,
                    RooAbsReal& right);
      RooExpGausExp(const RooExpGausExp& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooExpGausExp() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy mean_ ;
      RooRealProxy sigma_ ;
      RooRealProxy left_ ;
      RooRealProxy right_ ;

    private:
      ClassDef(RooExpGausExp,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExpGausExp_h
