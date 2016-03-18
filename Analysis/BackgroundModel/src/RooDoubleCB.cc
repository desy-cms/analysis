#include <iostream>
#include <cmath>
#include "TMath.h"

#include "RooRealVar.h"
#include "RooRealConstant.h"
#include "Analysis/BackgroundModel/interface/RooDoubleCB.h"


using namespace analysis::backgroundmodel;


ClassImp(RooDoubleCB)


RooDoubleCB::RooDoubleCB(const char* name, const char* title,
                         RooAbsReal& x,
                         RooAbsReal& mean,
                         RooAbsReal& width,
                         RooAbsReal& alpha1,
                         RooAbsReal& n1,
                         RooAbsReal& alpha2,
                         RooAbsReal& n2
                         ) :
   RooAbsPdf(name, title),
   x_("x", "x", this, x),
   mean_("mean", "mean", this, mean),
   width_("width", "width", this, width),
   alpha1_("alpha1", "alpha1", this, alpha1),
   n1_("n1", "n1", this, n1),
   alpha2_("alpha2", "alpha2", this, alpha2),
   n2_("n2", "n2", this, n2) {
}


RooDoubleCB::RooDoubleCB(const RooDoubleCB& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  width_("width", this, other.width_),
  alpha1_("alpha1", this, other.alpha1_),
  n1_("n1", this, other.n1_),
  alpha2_("alpha2", this, other.alpha2_),
  n2_("n2", this, other.n2_) {
}


TObject* RooDoubleCB::clone(const char* newname) const {
  return new RooDoubleCB(*this, newname);
}


double RooDoubleCB::evaluate() const {
  double t = (x_ - mean_)/width_;
  if (t > -alpha1_ && t < alpha2_) {
    return std::exp(-0.5*t*t);
  } else if (!(t > -alpha1_)) {
    double A1 = std::pow(n1_/fabs(alpha1_), n1_) * std::exp(-alpha1_*alpha1_/2);
    double B1 = n1_/std::fabs(alpha1_) - std::fabs(alpha1_);
    return A1 * std::pow(B1-t, -n1_);
  } else if (!(t < alpha2_)) {
    double A2 = std::pow(n2_/fabs(alpha2_), n2_) * std::exp(-alpha2_*alpha2_/2);
    double B2 = n2_/std::fabs(alpha2_)-std::fabs(alpha2_);
    return A2 * std::pow(B2+t, -n2_);
  } else {                      // Can this else clause actually happen?
    std::cerr << "ERROR evaluating range..." << std::endl;
    return 99;
  }
}


int RooDoubleCB::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars,
                                       const char* /* range */) const {
  if (matchArgs(allVars, analVars, x_)) return 1;

  return 0;
}


double RooDoubleCB::analyticalIntegral(int code,
                                       const char* rangeName) const {
  assert(code == 1) ;

  double central = 0;
  double left = 0;
  double right = 0;

  static const double root2 = std::sqrt(2) ;
  static const double rootPiBy2 = std::sqrt(std::atan2(0.0, -1.0) / 2.0);
  double xscale = root2 * width_;

  //compute gaussian contribution
  double centralLow = std::max(x_.min(rangeName), mean_ - alpha1_ * width_);
  double centralHigh = std::min(x_.max(rangeName), mean_ + alpha2_ * width_);
  if (centralLow < centralHigh) // is the gaussian part in range?
    central = rootPiBy2 * width_ * (TMath::Erf((centralHigh - mean_)/xscale) -
                                    TMath::Erf((centralLow - mean_)/xscale));

  //compute left tail;
  double A1 = (std::pow(n1_/std::fabs(alpha1_), n1_) *
               std::exp(-alpha1_*alpha1_/2));
  double B1 = n1_/std::fabs(alpha1_) - std::fabs(alpha1_);

  double leftLow = x_.min(rangeName);
  double leftHigh = std::min(x_.max(rangeName), mean_ - alpha1_*width_);
  if (leftLow < leftHigh) { //is the left tail in range?
    if (std::fabs(n1_ - 1.0) > 1.e-5)
      left = A1/(-n1_+1.0) * width_ *
        (std::pow(B1-(leftLow-mean_)/width_,-n1_+1.) -
         std::pow(B1-(leftHigh-mean_)/width_,-n1_+1.));
    else
      left = A1 * width_ * (std::log(B1-(leftLow-mean_)/width_) -
                            std::log(B1-(leftHigh-mean_)/width_));
  }

  //compute right tail;
  double A2 = (std::pow(n2_/std::fabs(alpha2_), n2_) *
               std::exp(-alpha2_*alpha2_/2));
  double B2 = n2_/std::fabs(alpha2_) - std::fabs(alpha2_);

  double rightLow = std::max(x_.min(rangeName), mean_ + alpha2_*width_);
  double rightHigh = x_.max(rangeName);
  if (rightLow < rightHigh) { //is the right tail in range?
    if (std::fabs(n2_-1.0)>1.e-5)
      right = A2/(-n2_+1.0) * width_ *
        (std::pow(B2+(rightHigh-mean_)/width_, -n2_+1.) -
         std::pow(B2 + (rightLow-mean_)/width_, -n2_+1.));
    else
      right = A2 * width_ * (std::log(B2+(rightHigh-mean_)/width_) -
                             std::log(B2+(rightLow-mean_)/width_));
  }

  return left + central + right;
}
