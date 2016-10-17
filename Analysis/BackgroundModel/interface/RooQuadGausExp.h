#ifndef Analysis_BackgroundModel_RooQuadGausExp_h
#define Analysis_BackgroundModel_RooQuadGausExp_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooQuadGausExp : public RooAbsPdf {
    public:
      inline RooQuadGausExp() = default;
      //RooQuadGausExp() : fValue(-1) {};
      RooQuadGausExp(const char *name, const char *title,
                  RooAbsReal& x,
                  RooAbsReal& mean,
                  RooAbsReal& sigmaL1,
				  RooAbsReal& sigmaL2,
				  RooAbsReal& sigmaR1,
				  RooAbsReal& sigmaR2,
				  RooAbsReal& tail_shift,
				  RooAbsReal& tail_sigma,
				  RooAbsReal& norm_g1,
				  RooAbsReal& norm_g2);
      RooQuadGausExp(const RooQuadGausExp& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooQuadGausExp() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy mean_ ;
      RooRealProxy sigmaL1_ ;
      RooRealProxy sigmaL2_ ;
      RooRealProxy sigmaR1_ ;
      RooRealProxy sigmaR2_ ;
      RooRealProxy tail_shift_;
      RooRealProxy tail_sigma_;
      RooRealProxy norm_g1_;
      RooRealProxy norm_g2_;

    private:
      ClassDef(RooQuadGausExp,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooQuadGausExp_h
