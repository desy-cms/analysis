/*
 * RooSuperDiJet.cpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: rostyslav
 */

#include "Analysis/BackgroundModel/interface/RooSuperDiJet.h"

using namespace analysis::backgroundmodel;

ClassImp(RooSuperDiJet)

RooSuperDiJet::RooSuperDiJet(const char *name, const char *title,
        RooAbsReal& x,
        RooAbsReal& mean,
        RooAbsReal& para,
        RooAbsReal& parb,
		const RooArgList& coefList) :
		RooAbsPdf(name, title),
		x_("x", "x", this, x),
		mean_("mean", "mean", this, mean),
		para_("para", "para", this, para),
		parb_("parb", "parb", this, parb),
		coefList_("coefList","List of coefficients",this)
		{
	// TODO Auto-generated constructor stub
	RooFIter coefIter = coefList.fwdIterator();
	RooAbsArg *coef;
	while((coef = (RooAbsArg*)coefIter.next())) {
		if (!static_cast<RooAbsReal*>(coef)) {
			throw std::invalid_argument("RooSuperDiJet::" + std::string(GetName()) + ", coeeficient " + std::string(coef->GetName()) + " is not of type RooAbsReal");
		}
		coefList_.add(*coef);
	}
}

RooSuperDiJet::RooSuperDiJet(const RooSuperDiJet& other, const char* newname) :
		RooAbsPdf(other, newname),
		x_("x", this, other.x_),
		mean_("mean", this, other.mean_),
		para_("para", this, other.para_),
		parb_("parb", this, other.parb_),
		coefList_("coefList",this,other.coefList_) {
};

TObject* RooSuperDiJet::clone(const char* newname) const {
  return new RooSuperDiJet(*this, newname);
}

double RooSuperDiJet::evaluate() const {
	/*
	 * Calculate and return value of SuperDiJet
	 */

	double result = 0.0;
	double arg = (x_ - mean_) / 13000.;
	double log = TMath::Log(arg);
	double add_term = 1 + parb_ * log;
	RooFIter it = coefList_.fwdIterator();
	RooAbsReal* c;
	int i = 2;
	while ((c = (RooAbsReal*) it.next())) {
		add_term += c->getVal() * TMath::Power(log,i);
		++i;
	}

	if(arg > 0.) {
		result = -para_ * log * (add_term);
	}

	return TMath::Exp(result);
}

