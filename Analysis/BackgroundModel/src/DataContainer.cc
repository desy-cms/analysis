#include <boost/assign/list_of.hpp>
#include "Analysis/BackgroundModel/interface/DataContainer.h"


namespace bass = boost::assign;

using namespace analysis::backgroundmodel;


DataContainer::DataContainer(const std::string& input)
  : histFileName_(input), data_(nullptr), bbH_(nullptr),
    summedBackgroundTemplates_(nullptr) {
  TFile* histFile = TFile::Open(histFileName_.c_str(), "read");
  data_ = getHistogram_(histFile, "data");
  data_->GetSumw2()->Set(0);
  bbH_ = getHistogram_(histFile, "bbH");
  const std::vector<std::string> bkgNames = bass::list_of
    ("B2C2B1bb")("C1bb")("Qbb")("bbB2B1C2")("bbC1Q");
  for (auto bkg = bkgNames.begin(); bkg != bkgNames.end(); ++bkg) {
    backgroundTemplates_.push_back(getHistogram_(histFile, *bkg));
    if (bkg == bkgNames.begin()) {
      summedBackgroundTemplates_ =
	static_cast<TH1*>(backgroundTemplates_.front()->Clone("background"));
      summedBackgroundTemplates_->SetDirectory(0);
    } else {
      summedBackgroundTemplates_->Add(backgroundTemplates_.back());
    }
  }
  delete histFile;
}


DataContainer::~DataContainer() {
}


TH1* DataContainer::getHistogram_(TFile* file, const std::string& name) {
  TH1* hist = static_cast<TH1*>(file->Get((name+"_Mbb").c_str())->Clone(name.c_str()));
  hist->SetDirectory(0);
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  return hist;
}
