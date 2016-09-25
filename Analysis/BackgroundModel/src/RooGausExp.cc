#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooGausExp.h"


using namespace analysis::backgroundmodel;


ClassImp(RooGausExp)


RooGausExp::RooGausExp(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& mean,
                             RooAbsReal& sigma,
                             RooAbsReal& right) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  sigma_("sigma", "sigma", this, sigma),
  right_("right", "right", this, right) {
}


RooGausExp::RooGausExp(const RooGausExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  sigma_("sigma", this, other.sigma_),
  right_("right", this, other.right_) {
}


TObject* RooGausExp::clone(const char* newname) const {
  return new RooGausExp(*this, newname);
}


double RooGausExp::evaluate() const {
  double std = (x_ - mean_) / sigma_;
  double result = 0.0;

  if (std <= right_) {
    result = std::exp(-0.5 * std::pow(std, 2));
  } else {
    result = std::exp(0.5*right_*right_ - right_*std);
  }

  return result;
}



