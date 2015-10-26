#include <iostream>
#include <exception>
#include "TSystem.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooAbsPdf.h"
#include "RooProdPdf.h"
#include "RooEffProd.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"
#include "RooBernstein.h"
#include "RooChebychev.h"
#include "RooNovosibirsk.h"
#include "RooCBShape.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;


FitContainer::FitContainer(TH1& data, TH1& signal, TH1& background)
  : plotDir_(getOutputPath_("plots")), workspace_(RooWorkspace("workspace")),
    mbb_("mbb", "m_{bb}",
	 data.GetXaxis()->GetXmin(), data.GetXaxis()->GetXmax(), "GeV"),
    data_("data_container", "", mbb_, RooFit::Import(data)),
    signal_("signal_container", "", mbb_, RooFit::Import(signal)),
    background_("background_container", "", mbb_, RooFit::Import(background)) {

  // some preliminary test code
  std::unique_ptr<RooPlot> frame(mbb_.frame());
  data_.plotOn(frame.get());
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  frame->Draw();
  canvas.SaveAs((plotDir_+"input_data.pdf").c_str());
}


FitContainer::FitContainer(const DataContainer& container)
  : FitContainer(*(container.data()), *(container.bbH()), *(container.background())) {
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

  if (name == "novosibirsk") setNovosibirsk_(type);
  else if (name == "crystalball") setCrystalBall_(type);
  else if (name == "bernstein") setBernstein_(type);
  else if (name == "chebychev") setChebychev_(type);
  else if (name == "berneffprod") setBernEffProd_(type);
  else if (name == "expeffprod") setExpEffProd_(type);
  else {
    std::cerr << ">>> Model '" << name << "' not implemented!" << std::endl;
    throw std::exception();
  }

  applyModifiers_(*(workspace_.pdf(type.c_str())), modifiers);
}


std::unique_ptr<RooFitResult> FitContainer::backgroundOnlyFit() {
  RooAbsPdf& bkg = *(workspace_.pdf("background"));
  std::unique_ptr<RooFitResult> fitResult(bkg.fitTo(data_, RooFit::Save()));

  // some preliminary test code
  std::unique_ptr<RooPlot> frame(mbb_.frame());
  data_.plotOn(frame.get(), RooFit::Name("data_curve"));
  bkg.plotOn(frame.get(), RooFit::Name("background_curve"));
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

  int nPars = fitResult->floatParsFinal().getSize();
  int ndf = getNonZeroBins_(data_) - nPars;
  double normChi2 = frame->chiSquare("background_curve", "data_curve", nPars);
  std::cout << "\nNormalized chi^2: "
	    << normChi2 * ndf << "/" << ndf << " = " << normChi2
	    << std::endl;

  return fitResult;
}


void FitContainer::setNovosibirsk_(const std::string& type) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") peakStart = getMaxPosition_(signal_);
  else if (type == "background") peakStart = getMaxPosition_(background_);
  RooRealVar peak("peak", "peak", peakStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb_.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo(type.c_str(), (type+"_novosibirsk").c_str(),
		      mbb_, peak, width, tail);
  workspace_.import(novo);
}


void FitContainer::setCrystalBall_(const std::string& type) {
  double mStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") mStart = getMaxPosition_(signal_);
  else if (type == "background") mStart = getMaxPosition_(background_);
  RooRealVar m0("m0", "", mStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar sigma("sigma", "", 35.0, 5.0, 100.0, "GeV");
  RooRealVar alpha("alpha", "", -1.0, 0.0);
  RooRealVar n("n", "", 1.0); n.setConstant(false);
  RooCBShape cb(type.c_str(), (type+"_crystalball").c_str(),
		mbb_, m0, sigma, alpha, n);
  workspace_.import(cb);
}


void FitContainer::setBernstein_(const std::string& type) {
  const unsigned int numCoeff = 7;
  double upperBound[numCoeff] = {10.0, 20.0, 20.0, 10.0, 10.0, 10.0, 10.0};
  RooArgList coefficients("bernstein_coefficients");
  for (unsigned int c = 0; c < numCoeff; ++c) {
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(Form("bernstein_coefficient_%02d", c), "", 0.0, upperBound[c]));
    coefficients.addClone(*coefficient);
  }
  RooBernstein bern(type.c_str(), (type+"_bernstein").c_str(), mbb_, coefficients);
  workspace_.import(bern);
}


void FitContainer::setChebychev_(const std::string& type) {
  const unsigned int numCoeff = 7;
  double upperBound[numCoeff] = {10.0, 20.0, 20.0, 10.0, 10.0, 10.0, 10.0};
  RooArgList coefficients("chebychev_coefficients");
  for (unsigned int c = 0; c < numCoeff; ++c) {
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(Form("chebychev_coefficient_%02d", c), "", 0.0, upperBound[c]));
    coefficients.addClone(*coefficient);
  }
  RooChebychev cheb(type.c_str(), (type+"_chebychev").c_str(), mbb_, coefficients);
  workspace_.import(cheb);
}


void FitContainer::setBernEffProd_(const std::string& type) {
  const unsigned int numCoeff = 7;
  double upperBound[numCoeff] = {10.0, 20.0, 20.0, 10.0, 10.0, 10.0, 10.0};
  RooArgList coefficients("bernstein_coefficients");
  for (unsigned int c = 0; c < numCoeff; ++c) {
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(Form("bernstein_coefficient_%02d", c), "", 0.0, upperBound[c]));
    coefficients.addClone(*coefficient);
  }
  RooBernstein bern((type+"_bernstein").c_str(), (type+"_bernstein").c_str(),
		    mbb_, coefficients);

  RooRealVar slope("slope", "", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "", 100.0, mbb_.getMin(), mbb_.getMax());

  RooFormulaVar eff((type+"eff").c_str(),
		    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
		    RooArgSet(mbb_, slope, turnon));

  RooEffProd bernEffProd(type.c_str(), (type+"_berneffprod").c_str(), bern, eff);

  workspace_.import(bernEffProd);
}


void FitContainer::setExpEffProd_(const std::string& type) {
  RooRealVar tau("tau", "", -0.1);
  tau.setConstant(false);
  RooExponential exp(type.c_str(), (type+"_exp").c_str(), mbb_, tau);
  // RooExponential exp((type+"_exp").c_str(), (type+"_exp").c_str(), mbb_, tau);

  // RooRealVar slope("slope", "", 0.01, 0.0, 0.1);
  // RooRealVar turnon("turnon", "", 100.0, mbb_.getMin(), mbb_.getMax());

  // RooFormulaVar eff((type+"_eff").c_str(),
  // 		    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
  // 		    RooArgSet(mbb_, slope, turnon));

  // RooEffProd expEffProd(type.c_str(), (type+"_berneffprod").c_str(), exp, eff);

  // applyModifiers_(expEffProd, modifiers);
  // workspace_.import(expEffProd);
  workspace_.import(exp);
}


std::string FitContainer::getOutputPath_(const std::string& subdirectory) {
  std::string path(gSystem->Getenv("CMSSW_BASE"));
  path += "/src/Analysis/BackgroundModel/test/" + subdirectory + "/";
  gSystem->mkdir(path.c_str(), true);
  return path;
}


bool FitContainer::checkType_(const std::string& type) {
  // update this list if needed
  const std::vector<std::string> allowedTypes = {"signal", "background"};
  for (const auto& t : allowedTypes) {
    if (t == type) return true;
  }
  std::cerr << "Type '" << type << "' is not allowed." << std::endl;
  return false;
}


double FitContainer::getMaxPosition_(const RooDataHist& data) {
  std::unique_ptr<TH1> hist(data.createHistogram("mbb"));
  int maximumBin = hist->GetMaximumBin();
  return hist->GetBinCenter(maximumBin);
}


int FitContainer::getNonZeroBins_(const RooDataHist& data) {
  std::unique_ptr<TH1> hist(data.createHistogram("mbb"));
  int nonZeroBins = 0;
  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    if (hist->GetBinContent(i) > 0) ++nonZeroBins;
  }
  return nonZeroBins;
}


bool FitContainer::applyModifiers_(RooAbsPdf& pdf,
				   const std::vector<ParamModifier>& modifiers) {
  bool modified = false;
  std::unique_ptr<RooArgSet> parameters(pdf.getParameters(mbb_));
  std::unique_ptr<TIterator> iter(parameters->createIterator());
  // use raw pointer for 'parameter' because 'pdf' owns the object it points to
  RooRealVar* parameter = static_cast<RooRealVar*>(iter->Next());
  while (parameter) {
    for (const auto& m : modifiers) {
      if (m.modify(*parameter)) {
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
