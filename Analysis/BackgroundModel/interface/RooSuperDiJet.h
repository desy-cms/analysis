/*
 * RooSuperDiJet.h
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERDIJET_H_
#define ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERDIJET_H_

#include "RooAbsPdf.h"
#include "RooAbsArg.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooListProxy.h"
#include <string>
#include "TMath.h"

namespace analysis {
namespace backgroundmodel {

class RooSuperDiJet : public RooAbsPdf {
public:
	inline RooSuperDiJet() = default;
	RooSuperDiJet(const char *name, const char *title,
            RooAbsReal& x,
            RooAbsReal& mean,
            RooAbsReal& para,
            RooAbsReal& parb,
			const RooArgList& coefList);
	RooSuperDiJet(const RooSuperDiJet& other, const char* name=0) ;
	virtual TObject* clone(const char* newname) const;
	inline virtual ~RooSuperDiJet() = default;

protected:
	double evaluate() const;

    RooRealProxy x_ ;
    RooRealProxy mean_ ;
    RooRealProxy para_ ;
    RooRealProxy parb_ ;
    RooListProxy coefList_;

private:
    ClassDef(RooSuperDiJet,1)
};

} /* namespace backgroundmodel */
} /* namespace analysis */

#endif /* ANALYSIS_BACKGROUNDMODEL_INTERFACE_ROOSUPERDIJET_H_ */
