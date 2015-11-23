#include <iostream>
#include <sstream>
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
#include "RooBukinPdf.h"
#include "Analysis/BackgroundModel/interface/RooDoubleCB.h"
#include "Analysis/BackgroundModel/interface/RooExpGausExp.h"
#include "Analysis/BackgroundModel/interface/RooExpBWExp.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;


FitContainer::FitContainer(TH1& data, TH1& signal, TH1& background) :
  verbosity_(1),
  plotDir_(getOutputPath_("plots")),
  workspaceDir_(getOutputPath_("workspace")),
  workspace_(RooWorkspace("workspace")),
  mbb_("mbb", "m_{bb}",
       data.GetXaxis()->GetXmin(), data.GetXaxis()->GetXmax(), "GeV"),
  data_("data_container", "", mbb_, RooFit::Import(data)),
  signal_("signal_container", "", mbb_, RooFit::Import(signal)),
  background_("background_container", "", mbb_, RooFit::Import(background)) {

  gSystem->Exec((std::string("rm -f "+plotDir_+"*").c_str()));
  gSystem->Exec((std::string("rm -f "+workspaceDir_+"*").c_str()));

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
  workspace_.writeToFile((workspaceDir_+"workspace.root").c_str());
}


FitContainer& FitContainer::verbosity(int level) {
  verbosity_ = level;
  return *this;
}


void FitContainer::setModel(const Type& type, const std::string& name) {
  const std::vector<ParamModifier> modifiers; // empty modifier list
  setModel(type, name, modifiers);
}


void FitContainer::setModel(const Type& type, const std::string& name,
                            const std::vector<ParamModifier>& modifiers) {
  if (workspace_.allPdfs().find(toString(type).c_str())) {
    std::stringstream msg;
    msg << "Model for " << toString(type) << " has already been set!";
    throw std::runtime_error(msg.str());
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
    std::stringstream msg;
    msg << "Unsupported number of arguments for fit model: "
	<< nameSplitted.size();
    throw std::runtime_error(msg.str());
  }

  if (nameSplitted[0] == "novosibirsk") setNovosibirsk_(type);
  else if (nameSplitted[0] == "crystalball") setCrystalBall_(type);
  else if (nameSplitted[0] == "novoeffprod") setNovoEffProd_(type);
  else if (nameSplitted[0] == "cbeffprod") setCBEffProd_(type);
  else if (nameSplitted[0] == "expeffprod") setExpEffProd_(type);
  else if (nameSplitted[0] == "doublecb") setDoubleCB_(type);
  else if (nameSplitted[0] == "expgausexp") setExpGausExp_(type);
  else if (nameSplitted[0] == "expbwexp") setExpBWExp_(type);
  else if (nameSplitted[0] == "bukin") setBukin_(type);
  else if (nameSplitted[0] == "bernstein") setBernstein_(type, numCoeffs);
  else if (nameSplitted[0] == "chebychev") setChebychev_(type, numCoeffs);
  else if (nameSplitted[0] == "berneffprod") setBernEffProd_(type, numCoeffs);
  else {
    std::stringstream msg;
    msg << "Model '" << name
	<< "' not implemented! Choose one of the following: "
	<< boost::algorithm::join(availableModels_, ", ");
    throw std::runtime_error(msg.str());
  }

  applyModifiers_(*(workspace_.pdf(toString(type).c_str())), modifiers);
}


std::unique_ptr<RooFitResult> FitContainer::backgroundOnlyFit() {
  RooAbsPdf& bkg = *(workspace_.pdf("background"));
  if (&bkg == nullptr) {
    throw std::logic_error("No background model has been set.");
  }
  std::unique_ptr<RooFitResult>
    fitResult(bkg.fitTo(data_, RooFit::Save(), RooFit::PrintLevel(verbosity_)));

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


void FitContainer::profileModel(const Type& type) {
  RooAbsPdf& model= *(workspace_.pdf(toString(type).c_str()));
  if (&model == nullptr) {
    std::stringstream msg;
    msg << "No model of type '" << toString(type) << "' is set, yet.";
    throw std::logic_error(msg.str());
  }
  std::unique_ptr<RooAbsReal> nll(model.createNLL(data_));
  std::unique_ptr<RooArgSet> parameters(model.getParameters(mbb_));
  std::unique_ptr<TIterator> iter(parameters->createIterator());
  // use raw pointer for 'parameter' because 'pdf' owns the object it points to:
  RooRealVar* parameter = static_cast<RooRealVar*>(iter->Next());
  while (parameter) {
    std::unique_ptr<RooAbsReal> profile(nll->createProfile(*parameter));
    std::unique_ptr<RooPlot> frame(parameter->frame());
    if (frame == nullptr) {
      std::stringstream msg;
      msg << "Problems creating the frame for '" << parameter->GetName() << "'.";
      throw std::runtime_error(msg.str());
    }
    profile->plotOn(frame.get());
    TCanvas canvas("canvas", "", 600, 600);
    canvas.cd();
    prepareCanvas_(canvas);
    prepareFrame_(*frame);
    frame->Draw();
    canvas.SaveAs((plotDir_+toString(type)+"_profile_"+
		   parameter->GetName()+".pdf").c_str());
    canvas.SetLogy();
    canvas.SaveAs((plotDir_+toString(type)+"_profile_"+
		   parameter->GetName()+"_log.pdf").c_str());
    parameter = static_cast<RooRealVar*>(iter->Next());
  }
}


void FitContainer::setNovosibirsk_(const Type& type) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: peakStart = getMaxPosition_(signal_); break;
  case Type::background: peakStart = getMaxPosition_(background_); break;
  }
  RooRealVar peak("peak", "peak", peakStart, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb_.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo(toString(type).c_str(),
		      (toString(type)+"_novosibirsk").c_str(),
                      mbb_, peak, width, tail);
  workspace_.import(novo);
}


void FitContainer::setCrystalBall_(const Type& type) {
  double mStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: mStart = getMaxPosition_(signal_); break;
  case Type::background: mStart = getMaxPosition_(background_); break;
  }
  RooRealVar m0("m0", "m0", mStart, 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha", "alpha", -1.0, -0.1);
  RooRealVar n("n", "n", 20.0, 3.0, 100.0);
  RooCBShape cb(toString(type).c_str(), (toString(type)+"_crystalball").c_str(),
                mbb_, m0, sigma, alpha, n);
  workspace_.import(cb);
}


void FitContainer::setNovoEffProd_(const Type& type) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: peakStart = getMaxPosition_(signal_); break;
  case Type::background: peakStart = getMaxPosition_(background_); break;
  }
  RooRealVar peak("peak", "peak", peakStart, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb_.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo((toString(type)+"_novosibirsk").c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb_, peak, width, tail);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon", 5.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
                    RooArgSet(mbb_, slope, turnon));

  RooEffProd novoEffProd(toString(type).c_str(),
			 (toString(type)+"_novoeffprod").c_str(), novo, eff);
  workspace_.import(novoEffProd);
}


void FitContainer::setCBEffProd_(const Type& type) {
  double mStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: mStart = getMaxPosition_(signal_); break;
  case Type::background: mStart = getMaxPosition_(background_); break;
  }
  RooRealVar m0("m0", "m0", mStart, 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha", "alpha", -1.0, -0.1);
  RooRealVar n("n", "n", 20.0, 3.0, 100.0);
  RooCBShape cb((toString(type)+"_crystalball").c_str(),
		(toString(type)+"_crystalball").c_str(),
                mbb_, m0, sigma, alpha, n);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon", 5.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
                    RooArgSet(mbb_, slope, turnon));

  RooEffProd cbEffProd(toString(type).c_str(),
		       (toString(type)+"_cbeffprod").c_str(), cb, eff);
  workspace_.import(cbEffProd);
}


void FitContainer::setExpEffProd_(const Type& type) {
  RooRealVar tau("tau", "tau", -0.1);
  tau.setConstant(false);
  RooExponential exp((toString(type)+"_exp").c_str(),
		     (toString(type)+"_exp").c_str(), mbb_, tau);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon", 5.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*mbb-turnon) + 1)",
                    RooArgSet(mbb_, slope, turnon));

  RooEffProd expEffProd(toString(type).c_str(),
			(toString(type)+"_expeffprod").c_str(), exp, eff);
  workspace_.import(expEffProd);
}


void FitContainer::setDoubleCB_(const Type& type) {
  double meanStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: meanStart = getMaxPosition_(signal_); break;
  case Type::background: meanStart = getMaxPosition_(background_); break;
  }
  RooRealVar mean("mean", "mean", meanStart, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 35.0, 5.0, 100.0, "GeV");
  RooRealVar alpha1("alpha1", "alpha1", -1.0, -0.1);
  RooRealVar n1("n1", "n1", 20.0, 3.0, 100.0);
  RooRealVar alpha2("alpha2", "alpha2", 0.1, 1.0);
  RooRealVar n2("n2", "n2", 20.0, 3.0, 100.0);
  RooDoubleCB doubleCB(toString(type).c_str(),
		       (toString(type)+"_doublecb").c_str(),
		       mbb_, mean, width, alpha1, n1, alpha2, n2);
  workspace_.import(doubleCB);
}


void FitContainer::setExpGausExp_(const Type& type) {
  double meanStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: meanStart = getMaxPosition_(signal_); break;
  case Type::background: meanStart = getMaxPosition_(background_); break;
  }
  RooRealVar mean("mean", "mean", meanStart, 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
  RooRealVar left("left", "left", 0.1, 15.0);
  RooRealVar right("right", "right", 0.1, 15.0);
  RooExpGausExp expGausExp(toString(type).c_str(),
			   (toString(type)+"_expgausexp").c_str(),
			   mbb_, mean, sigma, left, right);
  workspace_.import(expGausExp);
}


void FitContainer::setExpBWExp_(const Type& type) {
  double peakStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: peakStart = getMaxPosition_(signal_); break;
  case Type::background: peakStart = getMaxPosition_(background_); break;
  }
  RooRealVar peak("peak", "peak", peakStart, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 35.0, 5.0, 150.0, "GeV");
  RooRealVar left("left", "left", 0.1, 15.0);
  RooRealVar right("right", "right", 0.1, 15.0);
  RooExpBWExp expBWExp(toString(type).c_str(),
		       (toString(type)+"_expbwexp").c_str(),
		       mbb_, peak, width, left, right);
  workspace_.import(expBWExp);
}


void FitContainer::setBukin_(const Type& type) {
  double XpStart  = (mbb_.getMin() + mbb_.getMax()) / 2.0;
  switch (type) {
  case Type::signal: XpStart = getMaxPosition_(signal_); break;
  case Type::background: XpStart = getMaxPosition_(background_); break;
  }
  RooRealVar Xp("Xp", "Xp", XpStart, 50.0, 350.0, "GeV");
  RooRealVar sigp("sigp", "sigp", 15.0, 85.0, "GeV");
  RooRealVar xi("xi", "xi", 0.0, 0.55);
  RooRealVar rho1("rho1", "rho1", 0.05, -0.1, 0.2);
  RooRealVar rho2("rho2", "rho2", -0.05, -0.07, 0.045);
  RooBukinPdf bukin(toString(type).c_str(),
		    (toString(type)+"_bukin").c_str(),
		    mbb_, Xp, sigp, xi, rho1, rho2);
  workspace_.import(bukin);
}


void FitContainer::setBernstein_(const Type& type, const int numCoeffs) {
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern(toString(type).c_str(),
		    (toString(type)+"_bernstein").c_str(), mbb_, *coeffs);
  workspace_.import(bern);
}


void FitContainer::setChebychev_(const Type& type, const int numCoeffs) {
  if (numCoeffs > 7) {
    throw std::invalid_argument
      ("Chebychev polynomials support not more than 7 coefficients.");
  }
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "chebychev"));
  RooChebychev cheb(toString(type).c_str(),
		    (toString(type)+"_chebychev").c_str(), mbb_, *coeffs);
  workspace_.import(cheb);
}


void FitContainer::setBernEffProd_(const Type& type, const int numCoeffs) {
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern((toString(type)+"_bernstein").c_str(),
		    (toString(type)+"_bernstein").c_str(),
                    mbb_, *coeffs);

  RooRealVar slope1("slope1", "slope1", 0.01, 0.0, 0.1);
  RooRealVar turnon1("turnon1", "turnon1", 5.0, mbb_.getMin(), 100.0);
  RooRealVar slope2("slope2", "slope2", 0.01, 0.0, 0.1);
  RooRealVar turnon2("turnon2", "turnon2", 10.0, mbb_.getMin(), 100.0);
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope_1*mbb-turnon_1) + 1)*"
                    "0.5*(TMath::Erf(slope_2*mbb-turnon_2) + 1)",
                    RooArgSet(mbb_, slope1, turnon1, slope2, turnon2));

  RooEffProd bernEffProd(toString(type).c_str(),
			 (toString(type)+"_berneffprod").c_str(), bern, eff);
  workspace_.import(bernEffProd);
}


std::string FitContainer::getOutputPath_(const std::string& subdirectory) {
  std::string path(gSystem->Getenv("CMSSW_BASE"));
  path += "/src/Analysis/BackgroundModel/test/"+subdirectory;
  gSystem->mkdir(path.c_str(), true);
  path += "/FitContainer_";
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
    std::string id(Form((name+"_coefficient_%02d").c_str(), c));
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(id.c_str(), id.c_str(), 0.0, 10.0));
    coefficients->addClone(*coefficient);
  }
  return coefficients;
}


const std::vector<std::string> FitContainer::availableModels_ =
  {"novosibirsk",
   "crystalball",
   "novoeffprod",
   "cbeffprod",
   "expeffprod",
   "doublecb",
   "expgausexp",
   "expbwexp",
   "bukin",
   "bernstein",
   "chebychev",
   "berneffprod"};

const int FitContainer::defaultNumberOfCoefficients_ = 7;
