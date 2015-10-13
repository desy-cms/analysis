#include <iostream>
#include <exception>
#include <boost/assign/list_of.hpp>
#include "TSystem.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooBernstein.h"
#include "RooChebychev.h"
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
  const std::vector<ParamModifier> modifiers; // empty modifier list
  setModel(type, name, modifiers);
}


void FitContainer::setModel(const std::string& type, const std::string& name,
			    const std::vector<ParamModifier>& modifiers) {
  if (!checkType_(type)) throw std::exception();
  if (workspace_.allPdfs().find(type.c_str())) {
    std::cerr << ">>> Model for " << type << " has already been set!" << std::endl;
    throw std::exception();
  }
  
  if (name == "novosibirsk") {
    setNovosibirsk_(type, modifiers);
  } else if (name == "bernstein") {
    setBernstein_(type, modifiers);
  } else if (name == "chebychev") {
    setChebychev_(type, modifiers);
  } else {
    std::cerr << ">>> Model '" << name << "' not implemented!" << std::endl;
    throw std::exception();
  }
}


RooFitResult* FitContainer::backgroundOnlyFit() {
  RooAbsPdf* bkg = workspace_.pdf("background");
  RooFitResult* fitResult =
    bkg->fitTo(data_, RooFit::Save()// , RooFit::Range(0.0, 1500.0)
	       );
  
  // some preliminary test code
  RooPlot* frame = mbb_.frame();
  data_.plotOn(frame);
  bkg->plotOn(frame);
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  frame->Draw();
  canvas.SaveAs((plotDir_+"backgroundOnlyFit.pdf").c_str());

  std::cout << "\nconstant parameters:" << std::endl;
  fitResult->constPars().Print("v");
  std::cout << "\nfloating parameters (init):" << std::endl;
  fitResult->floatParsInit().Print("v");
  std::cout << "\nfloating parameters (final):" << std::endl;
  fitResult->floatParsFinal().Print("v");

  return fitResult;
}


void FitContainer::setNovosibirsk_(const std::string& type,
				   const std::vector<ParamModifier>& modifiers) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") peakStart = getMaxPosition_(signal_);
  else if (type == "background") peakStart = getMaxPosition_(background_);
  RooRealVar peak("peak", "peak", peakStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb_.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo(type.c_str(), (type+"_novosibirsk").c_str(),
		      mbb_, peak, width, tail);
  applyModifiers_(novo, modifiers);
  workspace_.import(novo);
}


void FitContainer::setBernstein_(const std::string& type,
				 const std::vector<ParamModifier>& modifiers) {
  const unsigned int numCoeff = 7;
  double upperBound[numCoeff] = {10.0, 20.0, 20.0, 10.0, 10.0, 10.0, 10.0};
  RooArgList coefficients("bernstein_coefficients");
  for (unsigned int c = 0; c < numCoeff; ++c) {
    RooRealVar* coefficient =
      new RooRealVar(Form("bernstein_coefficient_%02d", c), "", 0.0, upperBound[c]);
    coefficients.add(*coefficient);
  }
  RooBernstein bern(type.c_str(), (type+"_bernstein").c_str(), mbb_, coefficients);
  applyModifiers_(bern, modifiers);
  workspace_.import(bern);
}


void FitContainer::setChebychev_(const std::string& type,
				 const std::vector<ParamModifier>& modifiers) {
  const unsigned int numCoeff = 7;
  double upperBound[numCoeff] = {10.0, 20.0, 20.0, 10.0, 10.0, 10.0, 10.0};
  RooArgList coefficients("chebychev_coefficients");
  for (unsigned int c = 0; c < numCoeff; ++c) {
    RooRealVar* coefficient =
      new RooRealVar(Form("chebychev_coefficient_%02d", c), "", 0.0, upperBound[c]);
    coefficients.add(*coefficient);
  }
  RooChebychev cheb(type.c_str(), (type+"_chebychev").c_str(), mbb_, coefficients);
  applyModifiers_(cheb, modifiers);
  workspace_.import(cheb);
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


bool FitContainer::applyModifiers_(RooAbsPdf& pdf,
				   const std::vector<ParamModifier>& modifiers) {
  bool modified = false;
  RooArgSet* parameters = pdf.getParameters(mbb_);
  TIterator* iter = parameters->createIterator();
  RooRealVar* parameter = static_cast<RooRealVar*>(iter->Next());
  while (parameter) {
    for (auto m = modifiers.cbegin(); m != modifiers.cend(); ++m) {
      if (m->modify(*parameter)) {
	modified = true;
	break;
      }
    }
    parameter = static_cast<RooRealVar*>(iter->Next());
  }
  if (modifiers.size() > 0 && !modified) {
    std::cerr << ">>> None of the modifiers provided to '" << pdf.GetName()
	      << "' pdf could be applied." << std::endl;
    std::cerr << ">>> Provided modifiers :";
    for (auto m = modifiers.cbegin(); m != modifiers.cend(); ++m) {
      if (m != modifiers.cbegin()) std::cerr << ",";
      std::cerr << " " << m->name();
    }
    std::cerr << std::endl;
    std::cerr << ">>> Found in pdf:";
    parameters->Print();
  }
  return modified;
}
