/*
 * RooSuperNovosibirsk.h
 *
 *  Created on: 12 Apr 2017
 *      Author: shevchen
 */

#ifndef ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERNOVOSIBIRSK_H_
#define ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERNOVOSIBIRSK_H_

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooListProxy.h"
#include <string>
#include "TMath.h"

namespace analysis {
namespace backgroundmodel {

/*
 * @author shevchen
 *
 * 12 Apr 2017
 */
class RooSuperNovosibirsk: public RooAbsPdf {
public:
	inline RooSuperNovosibirsk() = default;
	RooSuperNovosibirsk(const char *name, const char *title,
            RooAbsReal& x,
            RooAbsReal& peak,
            RooAbsReal& width,
            RooAbsReal& tail,
			const RooArgList& coefList);
	RooSuperNovosibirsk(const RooSuperNovosibirsk& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const;
	inline virtual ~RooSuperNovosibirsk() = default;

protected:
	double evaluate() const;

    RooRealProxy x_ ;
    RooRealProxy peak_ ;
    RooRealProxy width_ ;
    RooRealProxy tail_ ;
    RooListProxy coefList_;

private:
    ClassDef(RooSuperNovosibirsk,1)
};

} /* namespace backgroundmodel */
} /* namespace analysis */

#endif /* ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERNOVOSIBIRSK_H_ */
