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
                             RooAbsReal& tail_shift,
							 RooAbsReal& tail_sigma) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  mean_("mean", "mean", this, mean),
  sigma_("sigma", "sigma", this, sigma),
  tail_shift_("tail_shift", "tail_shift", this, tail_shift),
  tail_sigma_("tail_sigma", "tail_sigma", this, tail_sigma){
}


RooGausExp::RooGausExp(const RooGausExp& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  mean_("mean", this, other.mean_),
  sigma_("sigma", this, other.sigma_),
  tail_shift_("tail_shift", this, other.tail_shift_),
  tail_sigma_("tail_sigma", this, other.tail_sigma_){
//  mean_("mean", this, other.mean_),
//  sigma_("sigma", this, other.sigma_),
//  right_("right", this, other.right_) {
}


TObject* RooGausExp::clone(const char* newname) const {
  return new RooGausExp(*this, newname);
}


double RooGausExp::evaluate() const {
//  double std = (x_ - mean_) / sigma_;
//  double result = 0.0;
//
//  if (std <= right_) {
//    result = std::exp(-0.5 * std::pow(std, 2));
//  } else {
//    result = std::exp(0.5*right_*right_ - right_*std);
//  }

	double result = 0;
	double a1 = (x_ - mean_) / sigma_;
	double a2 = (x_ - tail_shift_) / tail_sigma_;
	double g1 = TMath::Exp(-0.5 * a1 * a1);
	double b1 = (tail_shift_ - mean_) / sigma_;
	double g1_shift = TMath::Exp(-0.5 * b1 * b1);
	double exp = 1. / g1_shift * TMath::Exp(-0.5 * a2);

	if(x_ < tail_shift_) result = g1;
	else result = exp;
	return result;
}



