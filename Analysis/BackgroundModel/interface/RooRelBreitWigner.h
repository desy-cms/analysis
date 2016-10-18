#ifndef Analysis_BackgroundModel_RooRelBreitWigner_h
#define Analysis_BackgroundModel_RooRelBreitWigner_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooRelBreitWigner : public RooAbsPdf {
    public:
      inline RooRelBreitWigner() = default;
      RooRelBreitWigner(const char *name, const char *title,
                  RooAbsReal& x,
                  RooAbsReal& peak,
                  RooAbsReal& width);
      RooRelBreitWigner(const RooRelBreitWigner& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooRelBreitWigner() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy peak_ ;
      RooRealProxy width_ ;

    private:
      ClassDef(RooRelBreitWigner,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooRelBreitWigner_h
