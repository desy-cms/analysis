#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooExpGausExp.h"


using namespace analysis::backgroundmodel;


ClassImp(RooExpGausExp)


RooExpGausExp::RooExpGausExp(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& mean,
                             RooAbsReal& sigma,
                             RooAbsReal& left,
                             RooAbsReal& right) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  sigma_("sigma", "sigma", this, sigma),
  left_("left", "left", this, left),
  right_("right", "right", this, right) {
}


RooExpGausExp::RooExpGausExp(const RooExpGausExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  sigma_("sigma", this, other.sigma_),
  left_("left", this, other.left_),
  right_("right", this, other.right_) {
}


TObject* RooExpGausExp::clone(const char* newname) const {
  return new RooExpGausExp(*this, newname);
}


double RooExpGausExp::evaluate() const {
  double std = (x_ - mean_) / sigma_;
  double result = 0.0;

  if (std < -left_) {
    result = std::exp(0.5*left_*left_ + left_*std);
  } else if (!(std > right_) && !(std < -left_)) {
    result = std::exp(-0.5 * std::pow(std, 2));
  } else if (std > right_) {
    result = std::exp(0.5*right_*right_ - right_*std);
  }

  return result;
}



