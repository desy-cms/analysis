#ifndef Analysis_BackgroundModel_RooPhaseSpace_h
#define Analysis_BackgroundModel_RooPhaseSpace_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 

namespace analysis {
  namespace backgroundmodel {
 
 	class RooPhaseSpace : public RooAbsPdf {
	public:
  	   RooPhaseSpace() {} ; 
  	   RooPhaseSpace(const char *name, const char *title,
	      		 RooAbsReal& _mbb);
  	   RooPhaseSpace(const RooPhaseSpace& other, const char* name=0) ;
  	virtual TObject* clone(const char* newname) const;
  	inline virtual ~RooPhaseSpace() = default;

	protected:
  	   RooRealProxy mbb ;
  	   Double_t evaluate() const ;
 
	private:

  	   ClassDef(RooPhaseSpace,1)
	};

  }
}

 
#endif
