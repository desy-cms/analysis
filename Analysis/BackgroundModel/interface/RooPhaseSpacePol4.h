#ifndef Analysis_BackgroundModel_RooPhaseSpacePol4_h
#define Analysis_BackgroundModel_RooPhaseSpacePol4_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooPhaseSpacePol4 : public RooAbsPdf {
    public:
      inline RooPhaseSpacePol4() = default;
      RooPhaseSpacePol4(const char *name, const char *title,
                    RooAbsReal& x);

      RooPhaseSpacePol4(const RooPhaseSpacePol4& other, const char* name=0) ;

      virtual TObject* clone(const char* newname) const;

      inline virtual ~RooPhaseSpacePol4() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;

    private:
      ClassDef(RooPhaseSpacePol4,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExpGausExp_h
