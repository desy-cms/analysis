#ifndef Analysis_BackgroundModel_RooPolyDijet_h
#define Analysis_BackgroundModel_RooPolyDijet_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooPolyDijet : public RooAbsPdf {
    public:
      inline RooPolyDijet() = default;
      RooPolyDijet(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& mean,
                    RooAbsReal& par_a,
                    RooAbsReal& par_b,
		    RooAbsReal& par_c);
      RooPolyDijet(const RooPolyDijet& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooPolyDijet() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy mean_ ;
      RooRealProxy par_a_ ;
      RooRealProxy par_b_ ;
      RooRealProxy par_c_ ;

    private:
      ClassDef(RooPolyDijet,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExpGausExp_h
