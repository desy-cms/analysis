#include "Analysis/BackgroundModel/interface/ParamModifier.h"

using namespace analysis::backgroundmodel;

ParamModifier::ParamModifier(const std::string& name)
  : name_(name), constant_(false), floating_(false), start_(0), min_(0), max_(0),
    changeStart_(false), changeMin_(false), changeMax_(false) {
}


bool ParamModifier::modify(RooRealVar& var) const {
  if (var.GetName() != name_) return false;
  if (changeStart_) var.setVal(start_);
  if (changeMin_) var.setMin(min_);
  if (changeMax_) var.setMax(max_);
  if (constant_ && !floating_) var.setConstant();
  if (floating_ && !constant_) var.setConstant(false);
  return true;
}
