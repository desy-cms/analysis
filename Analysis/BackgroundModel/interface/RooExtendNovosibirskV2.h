#ifndef Analysis_BackgroundModel_RooExtendNovosibirskV2_h
#define Analysis_BackgroundModel_RooExtendNovosibirskV2_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooExtendNovosibirskV2 : public RooAbsPdf {
    public:
      inline RooExtendNovosibirskV2() = default;
      RooExtendNovosibirskV2(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& peak,
                    RooAbsReal& width,
                    RooAbsReal& tail,
                    RooAbsReal& par4,
		    RooAbsReal& par5);
      RooExtendNovosibirskV2(const RooExtendNovosibirskV2& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooExtendNovosibirskV2() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy peak_ ;
      RooRealProxy width_ ;
      RooRealProxy tail_ ;
      RooRealProxy par4_;
      RooRealProxy par5_;

    private:
      ClassDef(RooExtendNovosibirskV2,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooExtendNovosibirskV2_h
