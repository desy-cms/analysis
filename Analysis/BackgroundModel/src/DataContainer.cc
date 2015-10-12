#include <boost/assign/list_of.hpp>
#include "Analysis/BackgroundModel/interface/DataContainer.h"


namespace bass = boost::assign;

using namespace analysis::backgroundmodel;


DataContainer::DataContainer(const std::string& input)
  : histFileName_(input), data_(nullptr), bbH_(nullptr),
    summedBackgroundTemplates_(nullptr) {
  data_ = getHistogram_("data");
  data_->GetSumw2()->Set(0);
  bbH_ = getHistogram_("bbH");
  const std::vector<std::string> bkgNames = bass::list_of
    ("B2C2B1bb")("C1bb")("Qbb")("bbB2B1C2")("bbC1Q");
  for (auto bkg = bkgNames.cbegin(); bkg != bkgNames.cend(); ++bkg) {
    backgroundTemplates_.push_back(getHistogram_(*bkg));
    if (bkg == bkgNames.begin()) {
      summedBackgroundTemplates_ =
	static_cast<TH1*>(backgroundTemplates_.front()->Clone("background"));
      summedBackgroundTemplates_->SetDirectory(0);
    } else {
      summedBackgroundTemplates_->Add(backgroundTemplates_.back());
    }
  }
}


DataContainer::~DataContainer() {
}


TH1* DataContainer::getHistogram_(const std::string& name) {
  TFile* file = TFile::Open(histFileName_.c_str(), "read");
  TH1* hist = 
    static_cast<TH1*>(file->Get((name+"_Mbb").c_str())->Clone(name.c_str()));
  hist->SetDirectory(0);
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  delete file;
  return hist;
}
