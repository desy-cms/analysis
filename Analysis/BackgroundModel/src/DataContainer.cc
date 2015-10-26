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


DataContainer::~DataContainer() {
}


std::shared_ptr<TH1> DataContainer::data() const {
  std::string name(data_->GetName());
  name += "_"+std::to_string(dataCount_);
  std::shared_ptr<TH1> data(static_cast<TH1*>(data_->Clone(name.c_str())));
  ++dataCount_;
  return data;
}


std::shared_ptr<TH1> DataContainer::bbH() const {
  std::string name(bbH_->GetName());
  name += "_"+std::to_string(bbHCount_);
  std::shared_ptr<TH1> bbH(static_cast<TH1*>(bbH_->Clone(name.c_str())));
  ++bbHCount_;
  return bbH;
}


std::shared_ptr<TH1> DataContainer::background() const {
  std::string name(summedBackground_->GetName());
  name += "_"+std::to_string(backgroundCount_);
  std::shared_ptr<TH1> background
    (static_cast<TH1*>(summedBackground_->Clone(name.c_str())));
  ++backgroundCount_;
  return background;
}


void DataContainer::show() const {
  std::cout << "Data events:            " << data_->Integral() << std::endl;
  std::cout << "Expected signal events: " << bbH_->Integral() << std::endl;
  std::cout << "Background events:      " << summedBackground_->Integral() << std::endl;
}


std::unique_ptr<TH1> DataContainer::getHistogram_(const std::string& name) {
  TFile file(histFileName_.c_str(), "read");
  std::unique_ptr<TH1> hist =
    staticCastUnique<TH1>(file.Get((name+"_Mbb").c_str())->Clone(name.c_str()));
  hist->SetDirectory(0);
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  return hist;
}


unsigned int DataContainer::dataCount_ = 0;
unsigned int DataContainer::bbHCount_ = 0;
unsigned int DataContainer::backgroundCount_ = 0;
