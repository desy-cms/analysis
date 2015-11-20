#include <sstream>
#include "TFile.h"
#include "Analysis/BackgroundModel/interface/DataContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;


DataContainer::DataContainer(const std::string& input) : histFileName_(input) {
  data_ = getHistogram_("data");
  data_->GetSumw2()->Set(0);
  bbH_ = getHistogram_("bbH");
  const std::vector<std::string> bkgNames = 
    {"B2C2B1bb", "C1bb", "Qbb", "bbB2B1C2", "bbC1Q"};
  for (const auto& bkg : bkgNames) {
    backgroundTemplates_.push_back(getHistogram_(bkg));
    if (bkg == bkgNames.front()) {
      summedBackground_ =
	staticCastUnique<TH1>(backgroundTemplates_.front()->Clone("background"));
      summedBackground_->SetDirectory(0);
    } else {
      summedBackground_->Add(backgroundTemplates_.back().get());
    }
  }
}


DataContainer::~DataContainer() = default;


std::unique_ptr<TH1> DataContainer::data() const {
  return uniqueClone_(*data_);
}


std::unique_ptr<TH1> DataContainer::bbH() const {
  return uniqueClone_(*bbH_);
}


std::unique_ptr<TH1> DataContainer::background() const {
  return uniqueClone_(*summedBackground_);
}


void DataContainer::show() const {
  std::cout << "Data events:            " << data_->Integral() << std::endl;
  std::cout << "Expected signal events: " << bbH_->Integral() << std::endl;
  std::cout << "Background events:      " << summedBackground_->Integral() << std::endl;
}


std::unique_ptr<TH1> DataContainer::getHistogram_(const std::string& name) const {
  TFile file(histFileName_.c_str(), "read");
  std::unique_ptr<TH1> hist =
    staticCastUnique<TH1>(file.Get((name+"_Mbb").c_str())->Clone(name.c_str()));
  hist->SetDirectory(0);
  file.Close();
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  return hist;
}


std::unique_ptr<TH1> DataContainer::uniqueClone_(const TH1& original) {
  std::unique_ptr<TH1> clone(static_cast<TH1*>(original.Clone()));
  std::stringstream uniqueName;
  uniqueName << original.GetName() << "_" << clone.get();
  clone->SetName(uniqueName.str().c_str());
  return clone;
}
