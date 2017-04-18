/*
 * RooSuperNovosibirsk.cpp
 *
 *  Created on: 12 Apr 2017
 *      Author: shevchen
 */

#include "Analysis/BackgroundModel/interface/RooSuperNovosibirsk.h"

using namespace analysis::backgroundmodel;

ClassImp(RooSuperNovosibirsk)

RooSuperNovosibirsk::RooSuperNovosibirsk(const char *name, const char *title,
        RooAbsReal& x,
        RooAbsReal& peak,
        RooAbsReal& width,
        RooAbsReal& tail,
		const RooArgList& coefList) :
		RooAbsPdf(name, title),
		x_("x", "x", this, x),
		peak_("peak", "peak", this, peak),
		width_("width", "width", this, width),
		tail_("tail", "tail", this, tail),
		coefList_("coefList","List of coefficients",this)
		{
	// TODO Auto-generated constructor stub
	RooFIter coefIter = coefList.fwdIterator();
	RooAbsArg *coef;
	while((coef = (RooAbsArg*)coefIter.next())) {
		if (!static_cast<RooAbsReal*>(coef)) {
			throw std::invalid_argument("RooSuperNovosibirsk::" + std::string(GetName()) + ", coeeficient " + std::string(coef->GetName()) + " is not of type RooAbsReal");
		}
		coefList_.add(*coef);
	}
}

RooSuperNovosibirsk::RooSuperNovosibirsk(const RooSuperNovosibirsk& other, const char* newname) :
		RooAbsPdf(other, newname),
		x_("x", this, other.x_),
		peak_("peak", this, other.peak_),
		width_("width", this, other.width_),
		tail_("tail", this, other.tail_),
		coefList_("coefList",this,other.coefList_) {
};

TObject* RooSuperNovosibirsk::clone(const char* newname) const {
  return new RooSuperNovosibirsk(*this, newname);
}

double RooSuperNovosibirsk::evaluate() const {
	/*
	 * Calculate and return value of SuperNovosibirsk
	 */
//	const unsigned sz = coefList_.getSize();
	//if no coefficients provided - return normal Novosibirsk function
//	if(!sz || TMath::Abs(tail_) < 1.e-7) return RooMyNovosibirsk::evaluate();
	if (TMath::Abs(tail_) < 1.e-7) {
		return TMath::Exp( -0.5 * TMath::Power( ( (x_ - peak_) / width_ ), 2 ));
	}

	double pre_arg = ( x_ - peak_ ) * tail_ / width_;
	double arg = 1.0 - pre_arg;
	RooFIter it = coefList_.fwdIterator();
	RooAbsReal* c;
	int i = 1;
	while ((c = (RooAbsReal*) it.next())) {
		arg -= c->getVal() * TMath::Power(pre_arg,i);
		++i;
	}

	if (arg < 1.e-7) {
		//Argument of logaritem negative. Real continuation -> function equals zero
		return 0.0;
	}

	double log = TMath::Log(arg);
	static const double xi = 2.3548200450309494; // 2 Sqrt( Ln(4) )

	double width_zero = ( 2.0 / xi ) * TMath::ASinH( tail_ * xi * 0.5 );
	double width_zero2 = width_zero * width_zero;
	double exponent = ( -0.5 / (width_zero2) * log * log ) - ( width_zero2 * 0.5 );

	return TMath::Exp(exponent) ;
}

