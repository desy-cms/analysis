#ifndef Analysis_BackgroundModel_RooExpBWExp_h
#define Analysis_BackgroundModel_RooExpBWExp_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooExpBWExp : public RooAbsPdf {
    public:
      inline RooExpBWExp() = default;
      RooExpBWExp(const char *name, const char *title,
                  RooAbsReal& x,
                  RooAbsReal& peak,
                  RooAbsReal& width,
                  RooAbsReal& left,
                  RooAbsReal& right);
      RooExpBWExp(const RooExpBWExp& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooExpBWExp() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy peak_ ;
      RooRealProxy width_ ;
      RooRealProxy left_ ;
      RooRealProxy right_ ;

    private:
      ClassDef(RooExpBWExp,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExpBWExp_h
