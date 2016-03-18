#include <regex>
#include "Analysis/BackgroundModel/interface/ParamModifier.h"

using namespace analysis::backgroundmodel;

ParamModifier::ParamModifier(const std::string& name)
  : name_(name), constant_(false), floating_(false), start_(0), min_(0), max_(0),
    changeStart_(false), changeMin_(false), changeMax_(false) {
}


ParamModifier& ParamModifier::start(float value) {
  start_ = value;
  changeStart_ = true;
  return *this;
}


ParamModifier& ParamModifier::min(float value) {
  min_ = value;
  changeMin_ = true;
  return *this;
}


ParamModifier& ParamModifier::max(float value) {
  max_ = value;
  changeMax_ = true;
  return *this;
}


ParamModifier& ParamModifier::constant() {
  constant_ = true;
  floating_ = false;
  return *this;
}


ParamModifier& ParamModifier::floating() {
  floating_ = true;
  constant_ = false;
  return *this;
}


bool ParamModifier::modify(RooRealVar& var) const {
  const std::regex nameRegex(name_);
  if (!std::regex_match(var.GetName(), nameRegex)) return false;
  if (changeMin_) {
    if (min_ > var.getMax()) var.setMax(min_);
    var.setMin(min_);
  }
  if (changeMax_) {
    if (max_ < var.getMin()) var.setMin(max_);
    var.setMax(max_);
  }
  if (changeStart_) var.setVal(start_);
  if (constant_ && !floating_) var.setConstant();
  if (floating_ && !constant_) var.setConstant(false);
  return true;
}


void ParamModifier::show() const {
  std::cout << "\nThe following properties of the parameters matching '"
            << name_ << "' are modified:" << std::endl;
  if (changeStart_) std::cout << "  -> start value = " << start_ << std::endl;
  if (changeMin_)   std::cout << "  -> minimum = " << min_ << std::endl;
  if (changeMax_)   std::cout << "  -> maximum = " << max_ << std::endl;
  if (constant_ && !floating_)  std::cout << "  -> constant" << std::endl;
  if (floating_ && !constant_)  std::cout << "  -> floating" << std::endl;
}
