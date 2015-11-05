#include <iostream>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "TLatex.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooAbsPdf.h"
#include "RooEffProd.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"
#include "RooBernstein.h"
#include "RooChebychev.h"
#include "RooNovosibirsk.h"
#include "RooCBShape.h"
#include "Analysis/BackgroundModel/interface/RooDoubleCB.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;


FitContainer::FitContainer(TH1& data, TH1& signal, TH1& background) :
  plotDir_(getOutputPath_("plots")),
  workspace_(RooWorkspace("workspace")),
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
  prepareCanvas_(canvas);
  prepareFrame_(*frame);
  frame->Draw();
  canvas.SaveAs((plotDir_+"input_data.pdf").c_str());
}


FitContainer::FitContainer(const DataContainer& container) :
  FitContainer(*(container.data()), *(container.bbH()), *(container.background())) {
}


FitContainer::~FitContainer() {
  workspace_.writeToFile((getOutputPath_("workspace")+"workspace.root").c_str());
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

  int numCoeffs = defaultNumberOfCoefficients_;
  std::vector<std::string> nameSplitted;
  boost::split(nameSplitted, name, boost::is_any_of(","));
  switch (nameSplitted.size()) {
  case 1: break;
  case 2:
    numCoeffs = std::stoi(nameSplitted[1]);
    break;
  default:
    std::cerr << ">>> Unsupported number of arguments for fit model: "
	      << nameSplitted.size() << std::endl;
    throw std::exception();
  }

  if (nameSplitted[0] == "novosibirsk") setNovosibirsk_(type);
  else if (nameSplitted[0] == "crystalball") setCrystalBall_(type);
  else if (nameSplitted[0] == "novoeffprod") setNovoEffProd_(type);
  else if (nameSplitted[0] == "expeffprod") setExpEffProd_(type);
  else if (nameSplitted[0] == "doublecb") setDoubleCB_(type);
  else if (nameSplitted[0] == "bernstein") setBernstein_(type, numCoeffs);
  else if (nameSplitted[0] == "chebychev") setChebychev_(type, numCoeffs);
  else if (nameSplitted[0] == "berneffprod") setBernEffProd_(type, numCoeffs);
  else {
    std::cerr << ">>> Model '" << name << "' not implemented!" << std::endl;
    std::cerr << ">>> Choose one of the following: "
	      << boost::algorithm::join(availableModels_, ", ") << std::endl;
    throw std::exception();
  }

  applyModifiers_(*(workspace_.pdf(type.c_str())), modifiers);
}


std::unique_ptr<RooFitResult> FitContainer::backgroundOnlyFit() {
  RooAbsPdf& bkg = *(workspace_.pdf("background"));
  std::unique_ptr<RooFitResult> fitResult(bkg.fitTo(data_, RooFit::Save()));

  // some preliminary test code
  std::cout << "\nconstant parameters:" << std::endl;
  fitResult->constPars().Print("v");
  std::cout << "\nfloating parameters (init):" << std::endl;
  fitResult->floatParsInit().Print("v");
  std::cout << "\nfloating parameters (final):" << std::endl;
  fitResult->floatParsFinal().Print("v");

  std::unique_ptr<RooPlot> frame(mbb_.frame());
  data_.plotOn(frame.get(), RooFit::Name("data_curve"));
  bkg.plotOn(frame.get(), RooFit::Name("background_curve"));
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  prepareCanvas_(canvas);
  prepareFrame_(*frame);
  frame->Draw();

  int nPars = fitResult->floatParsFinal().getSize();
  int ndf = getNonZeroBins_(data_) - nPars;
  double normChi2 = frame->chiSquare("background_curve", "data_curve", nPars);
  std::string chi2str(Form("%.1f/%d = %.1f",normChi2 * ndf, ndf, normChi2));
  std::cout << "\nNormalized chi^2: " << chi2str << std::endl;
  TLatex latex;
  latex.SetTextFont(43);
  latex.SetTextSize(20);
  latex.SetTextAlign(33);
  latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.98-canvas.GetTopMargin(),
		     (std::string("#chi^{2}/ndf = ")+chi2str).c_str());

  canvas.SaveAs((plotDir_+"backgroundOnlyFit.pdf").c_str());

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


void FitContainer::setNovoEffProd_(const std::string& type) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") peakStart = getMaxPosition_(signal_);
  else if (type == "background") peakStart = getMaxPosition_(background_);
  RooRealVar peak("peak", "peak", peakStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb_.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo((type+"_novosibirsk").c_str(),
		      (type+"_novosibirsk").c_str(),
		      mbb_, peak, width, tail);

  RooRealVar slope("slope", "", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "", 5.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((type+"_eff").c_str(),
  		    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
  		    RooArgSet(mbb_, slope, turnon));

  RooEffProd novoEffProd(type.c_str(), (type+"_novoeffprod").c_str(), novo, eff);
  workspace_.import(novoEffProd);
}


void FitContainer::setExpEffProd_(const std::string& type) {
  RooRealVar tau("tau", "", -0.1);
  tau.setConstant(false);
  RooExponential exp((type+"_exp").c_str(), (type+"_exp").c_str(), mbb_, tau);

  RooRealVar slope("slope", "", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "", 5.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((type+"_eff").c_str(),
  		    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
  		    RooArgSet(mbb_, slope, turnon));

  RooEffProd expEffProd(type.c_str(), (type+"_expeffprod").c_str(), exp, eff);
  workspace_.import(expEffProd);
}


void FitContainer::setDoubleCB_(const std::string& type) {
  double meanStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  if (type == "signal") meanStart = getMaxPosition_(signal_);
  else if (type == "background") meanStart = getMaxPosition_(background_);
  RooRealVar mean("mean", "", meanStart, mbb_.getMin(), mbb_.getMax(), "GeV");
  RooRealVar width("width", "", 35.0, 5.0, 100.0, "GeV");
  RooRealVar alpha1("alpha1", "", -1.0, 0.0);
  RooRealVar n1("n1", "", 1.0);
  RooRealVar alpha2("alpha2", "", -1.0, 0.0);
  RooRealVar n2("n2", "", 1.0);
  n1.setConstant(false);
  n2.setConstant(false);
  RooDoubleCB doubleCB(type.c_str(), (type+"_doublecb").c_str(),
                      mbb_, mean, width, alpha1, n1, alpha2, n2);
  workspace_.import(doubleCB);
}


void FitContainer::setBernstein_(const std::string& type, const int numCoeffs) {
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern(type.c_str(), (type+"_bernstein").c_str(), mbb_, *coeffs);
  workspace_.import(bern);
}


void FitContainer::setChebychev_(const std::string& type, const int numCoeffs) {
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "chebychev"));
  RooChebychev cheb(type.c_str(), (type+"_chebychev").c_str(), mbb_, *coeffs);
  workspace_.import(cheb);
}


void FitContainer::setBernEffProd_(const std::string& type, const int numCoeffs) {
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern((type+"_bernstein").c_str(), (type+"_bernstein").c_str(),
		    mbb_, *coeffs);

  RooRealVar slope1("slope_1", "", 0.01, 0.0, 0.1);
  RooRealVar turnon1("turnon_1", "", 5.0, mbb_.getMin(), 100.0);
  RooRealVar slope2("slope_2", "", 0.01, 0.0, 0.1);
  RooRealVar turnon2("turnon_2", "", 10.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((type+"_eff").c_str(),
  		    "0.5*(TMath::Erf(slope_1*mbb-turnon_1) + 1)*"
  		    "0.5*(TMath::Erf(slope_2*mbb-turnon_2) + 1)",
  		    RooArgSet(mbb_, slope1, turnon1, slope2, turnon2));

  RooEffProd bernEffProd(type.c_str(), (type+"_berneffprod").c_str(), bern, eff);
  workspace_.import(bernEffProd);
}


std::string FitContainer::getOutputPath_(const std::string& subdirectory) {
  std::string path(gSystem->Getenv("CMSSW_BASE"));
  path += "/src/Analysis/BackgroundModel/test/" + subdirectory + "/";
  gSystem->mkdir(path.c_str(), true);
  return path;
}


void FitContainer::prepareCanvas_(TCanvas& raw) {
  raw.SetLeftMargin(0.15);
  raw.SetRightMargin(0.05);
}


void FitContainer::prepareFrame_(RooPlot& raw) {
  raw.GetYaxis()->SetTitleOffset(2);
  raw.SetTitle("");
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
  // use raw pointer for 'parameter' because 'pdf' owns the object it points to:
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


std::unique_ptr<RooArgList>
FitContainer::getCoefficients_(const int numCoeffs, const std::string& name) {
  std::unique_ptr<RooArgList> coefficients
    (new RooArgList((name+"_coefficients").c_str()));
  for (int c = 0; c < numCoeffs; ++c) {
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(Form((name+"_coefficient_%02d").c_str(), c), "", 0.0, 10.0));
    coefficients->addClone(*coefficient);
  }
  return coefficients;
}


const std::vector<std::string> FitContainer::availableModels_ =
  {"novosibirsk",
   "crystalball",
   "novoeffprod",
   "expeffprod",
   "doublecb",
   "bernstein",
   "chebychev",
   "berneffprod"};

const int FitContainer::defaultNumberOfCoefficients_ = 7;
