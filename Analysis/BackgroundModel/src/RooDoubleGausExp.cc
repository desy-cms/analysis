#include <cmath>
#include "Riostream.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooDoubleGausExp.h"


using namespace analysis::backgroundmodel;


ClassImp(RooDoubleGausExp)


RooDoubleGausExp::RooDoubleGausExp(const char *name, const char *title,
                  RooAbsReal& x,
                  RooAbsReal& mean,
                  RooAbsReal& sigmaL,
				  RooAbsReal& sigmaR,
				  RooAbsReal& tail_shift,
				  RooAbsReal& tail_sigma) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  sigmaL_("sigmaL", "sigmaL", this, sigmaL),
  sigmaR_("sigmaR", "sigmaR", this, sigmaR),
  tail_shift_("tail_shift", "tail_shift", this, tail_shift),
  tail_sigma_("tail_sigma", "tail_sigma", this, tail_sigma)
  {}


RooDoubleGausExp::RooDoubleGausExp(const RooDoubleGausExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  sigmaL_("sigmaL", this, other.sigmaL_),
  sigmaR_("sigmaR_", this, other.sigmaR_),
  tail_shift_("tail_shift", this, other.tail_shift_),
  tail_sigma_("tail_sigma", this, other.tail_sigma_){
}


TObject* RooDoubleGausExp::clone(const char* newname) const {
  return new RooDoubleGausExp(*this, newname);
}


double RooDoubleGausExp::evaluate() const {

	double aL = (x_-mean_)/sigmaL_;
	double aR = (x_-mean_)/sigmaR_;

	double aexp = (x_-tail_shift_)/tail_sigma_;

	double dR  = (tail_shift_-mean_)/sigmaR_;

	double gL = TMath::Exp(-0.5*aL*aL);
	double gR = TMath::Exp(-0.5*aR*aR);

	double w = TMath::Exp(-0.5*dR*dR);

	double resultL = gL;
	double resultR = gR;
	double norm    = w;
	double exp = norm*TMath::Exp(-aexp);
//	double exp = norm*TMath::Exp(-aexp*aexp*0.5);

	double result = resultL;
	if (x_>mean_)
		result = resultR;
	if (x_>tail_shift_)
		result = exp;

  return result;
}



