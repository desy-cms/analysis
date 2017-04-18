#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooPolyDijet.h"


using namespace analysis::backgroundmodel;


ClassImp(RooPolyDijet)


RooPolyDijet::RooPolyDijet(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& mean,
                             RooAbsReal& par_a,
			     RooAbsReal& par_b,
                             RooAbsReal& par_c) :

  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  par_a_("par_a", "par_a", this, par_a),
  par_b_("par_b", "par_b", this, par_b),
  par_c_("par_c", "par_c", this, par_c) {
}


RooPolyDijet::RooPolyDijet(const RooPolyDijet& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  par_a_("par_a", this, other.par_a_),
  par_b_("par_b", this, other.par_b_),
  par_c_("par_c", this, other.par_c_) {
}


TObject* RooPolyDijet::clone(const char* newname) const {
  return new RooPolyDijet(*this, newname);
}


double RooPolyDijet::evaluate() const {
  double std = (x_ - mean_) / 13000.;
  double result = 0.0;

  if(std > 0.) {
	double log = TMath::Log(std);
	//if(log >= 0.) {
	result = TMath::Exp(-(par_a_ * log * (1 + par_b_ * log + par_c_ * TMath::Sqrt(TMath::Abs(log)))));
	//}
	//else result = 0.0;
  }

  return result;
}



