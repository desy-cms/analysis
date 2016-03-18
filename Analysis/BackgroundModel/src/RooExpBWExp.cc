#include <cmath>
#include "Riostream.h"
#include "TMath.h"
#include "Analysis/BackgroundModel/interface/RooExpBWExp.h"


using namespace analysis::backgroundmodel;


ClassImp(RooExpBWExp)


RooExpBWExp::RooExpBWExp(const char *name, const char *title,
                         RooAbsReal& x,
                         RooAbsReal& peak,
                         RooAbsReal& width,
                         RooAbsReal& left,
                         RooAbsReal& right) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  peak_("peak", "peak", this, peak),
  width_("width", "width", this, width),
  left_("left", "left", this, left),
  right_("right", "right", this, right) {
}


RooExpBWExp::RooExpBWExp(const RooExpBWExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  peak_("peak", this, other.peak_),
  width_("width", this, other.width_),
  left_("left", this, other.left_),
  right_("right", this, other.right_) {
}


TObject* RooExpBWExp::clone(const char* newname) const {
  return new RooExpBWExp(*this, newname);
}


double RooExpBWExp::evaluate() const {
  double arg = (x_ - peak_)/width_;
  double result = 1.0/(width_*width_);

  if (arg < -left_) {
    result *= (std::exp(2.0*left_*(left_+arg)/(left_*left_+0.25)) /
               (left_*left_+0.25));
  } else if (!(arg > right_) && !(arg < -left_)) {
    result *= 1.0 / (arg*arg+0.25);
  } else if (arg > right_) {
    result *= (std::exp(2.0*right_*(right_-arg)/((right_*right_+0.25))) /
               (right_*right_+0.25));
  }

  return result;
}



