#ifndef Analysis_BackgroundModel_RooMyNovosibirsk_h
#define Analysis_BackgroundModel_RooMyNovosibirsk_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"


namespace analysis {
  namespace backgroundmodel {

    class RooMyNovosibirsk : public RooAbsPdf {
    public:
      inline RooMyNovosibirsk() = default;
      RooMyNovosibirsk(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& peak,
                    RooAbsReal& width,
                    RooAbsReal& tail);
      RooMyNovosibirsk(const RooMyNovosibirsk& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooMyNovosibirsk() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy peak_ ;
      RooRealProxy width_ ;
      RooRealProxy tail_ ;

    private:
      ClassDef(RooMyNovosibirsk,1)
    };

  }
}

#endif  // Analysis_BackgroundModel_RooMyNovosibirsk_h
