#include "TFile.h"
#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;

/*
HistContainer::HistContainer(const std::string& input) : histFileName_(input) {
  data_ = getHistogram_("data");
  //if(data_) data_->GetSumw2()->Set(0);
  bbH_ = getHistogram_("bbH");
  summedBackground_ = getHistogram_("QCD");

  if(!data_ && !bbH_ && !summedBackground_){
	  std::cerr<<"No valid histograms in: "<<input<<std::endl;
  }
  
//  const std::vector<std::string> bkgNames =
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
//
}
*/

HistContainer::HistContainer(const std::string& input) : histFileName_(input) {
  data_ = getHistogram_("data");
  //data_->GetSumw2()->Set(0);
}

HistContainer::~HistContainer() = default;

void HistContainer::Rebin(const int& n) const {
	if(data_) data_->Rebin(n);
//	if(bbH_)  bbH_ ->Rebin(n);
//	if(summedBackground_) summedBackground_ ->Rebin(n);
}

std::unique_ptr<TH1> HistContainer::data() const {
  return uniqueClone_(*data_);
}


std::unique_ptr<TH1> HistContainer::bbH() const {
  return uniqueClone_(*bbH_);
}


std::unique_ptr<TH1> HistContainer::background() const {
  return uniqueClone_(*summedBackground_);
}


std::vector<std::unique_ptr<TH1> > HistContainer::backgrounds() const {
  std::vector<std::unique_ptr<TH1> > result;
  for (const auto& bkg: backgroundTemplates_) {
    result.emplace_back(uniqueClone_(*bkg));
  }
  return result;
}


void HistContainer::show() const {
  if(data_) std::cout << "Data events:            " << data_->Integral() << std::endl;
  //if (bbH_) std::cout << "Expected signal events: " << bbH_->Integral() << std::endl;
  //if(summedBackground_) std::cout << "Background events:      " << summedBackground_->Integral() << std::endl;
  //if(!data_ && !bbH_ && !summedBackground_) throw std::logic_error("No histograms was defined in HistContainer");
}


std::unique_ptr<TH1> HistContainer::getHistogram_(const std::string& name) const {
  TFile file(histFileName_.c_str(), "read");
  std::string dir_name = "general";
  std::string hist_name = "";
  if(name == "data") hist_name = "diJet_m";
  TDirectory *dir = (TDirectory*)file.Get((dir_name).c_str());
  if(!file.Get((dir_name).c_str())) return nullptr;
  //if(!dir->Get((hist_name).c_str())) return nullptr;
  //if(!file.Get((name+add_name).c_str())) return nullptr;
  std::unique_ptr<TH1> hist =
    //staticCastUnique<TH1>(file.Get((name+add_name).c_str())->Clone(name.c_str()));
    staticCastUnique<TH1>(dir->Get((hist_name).c_str())->Clone(name.c_str()));
  if(!hist) return nullptr;
 
/*  //normalization the bin width
  double bin_content = -100.;
  double bin_err = -100.;
  for(int i = 1; i <= hist->GetNbinsX(); ++i) {
	bin_content = hist->GetBinContent(i)/hist->GetBinWidth(i);
	bin_err = hist->GetBinError(i)/hist->GetBinWidth(i);
	hist->SetBinContent(i,bin_content);
	hist->SetBinError(i,bin_err);
  }
*/
  hist->SetDirectory(0);
  file.Close();
  if (hist->GetSumw2N() == 0) hist->Sumw2();
  return hist;
}


std::unique_ptr<TH1> HistContainer::uniqueClone_(const TH1& original) {
  std::unique_ptr<TH1> clone(static_cast<TH1*>(uniqueClone(&original)));
  if (clone != nullptr) clone->SetDirectory(0);
  return clone;
}
