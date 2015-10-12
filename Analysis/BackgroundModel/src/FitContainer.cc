#include <iostream>
#include <exception>
#include <boost/assign/list_of.hpp>
#include "TSystem.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooNovosibirsk.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"


namespace bass = boost::assign;

using namespace analysis::backgroundmodel;


FitContainer::FitContainer(TH1* data, TH1* signal, TH1* background)
  : plotDir_(getOutputPath_("plots")), workspace_(RooWorkspace("workspace")),
    mbb_("mbb", "m_{bb}",
	 data->GetXaxis()->GetXmin(), data->GetXaxis()->GetXmax(), "GeV"),
    data_("data_container", "", mbb_, RooFit::Import(*data)),
    signal_("signal_container", "", mbb_, RooFit::Import(*signal)),
    background_("background_container", "", mbb_, RooFit::Import(*background)) {

  // some preliminary test code
  RooPlot* frame = mbb_.frame();
  data_.plotOn(frame);
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  frame->Draw();
  canvas.SaveAs((plotDir_+"input_data.pdf").c_str());
}


FitContainer::~FitContainer() {
}


void FitContainer::setModel(const std::string& type, const std::string& name) {
  if (!checkType_(type)) throw std::exception();
  if (name == "novosibirsk") {
    setNovosibirsk_(type);
  } else {
    std::cerr << ">>> Model '" << name << "' not implemented!" << std::endl;
    throw std::exception();
  }
}


RooFitResult* FitContainer::backgroundOnlyFit() {
  RooAbsPdf* bkg = workspace_.pdf("background");
  RooFitResult* fitResult = bkg->fitTo(data_, RooFit::Save());
  
  // some preliminary test code
  RooPlot* frame = mbb_.frame();
  data_.plotOn(frame);
  bkg->plotOn(frame);
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  frame->Draw();
  canvas.SaveAs((plotDir_+"backgroundOnlyFit.pdf").c_str());

  return fitResult;
}


void FitContainer::setNovosibirsk_(const std::string& type) {
  double peakStart = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") peakStart = getMaxPosition_(signal_);
  else if (type == "background") peakStart = getMaxPosition_(background_);
  RooRealVar peak("peak", "peak", peakStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar width("width", "width", 0.0, 200.0, "GeV");
  RooRealVar tail("tail", "tail", 0.0, 50.0);
  RooNovosibirsk novo(type.c_str(), (type+"_novosibirsk").c_str(),
		      mbb_, peak, width, tail);
  workspace_.import(novo);
}


std::string FitContainer::getOutputPath_(const std::string& subdirectory) {
  std::string path(gSystem->Getenv("CMSSW_BASE"));
  path += "/src/Analysis/BackgroundModel/test/" + subdirectory + "/";
  gSystem->mkdir(path.c_str(), true);
  return path;
}


bool FitContainer::checkType_(const std::string& type) {
  // update this list if needed
  const std::vector<std::string> allowedTypes = bass::list_of
    ("signal")("background");
  for (auto t = allowedTypes.cbegin(); t != allowedTypes.cend(); ++t) {
    if (*t == type) return true;
  }
  std::cerr << "Type '" << type << "' is not allowed." << std::endl;
  return false;
}


double FitContainer::getMaxPosition_(const RooDataHist& data) {
  TH1* hist = data.createHistogram("mbb");
  int maximumBin = hist->GetMaximumBin();
  double maximum = hist->GetBinCenter(maximumBin);
  delete hist;
  return maximum;
}
