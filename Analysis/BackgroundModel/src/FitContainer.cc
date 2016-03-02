#include <iostream>
#include <sstream>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "TLatex.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooFormulaVar.h"
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


FitContainer::FitContainer(const TH1& data, const TH1& signal, const TH1& bkg) :
  initialized_(false),
  plotDir_(getOutputPath_("plots")),
  workspaceDir_(getOutputPath_("workspace")),
  fullRangeId_("full_range"),
  fitRangeId_("fit_range"),
  verbosity_(1),
  workspace_(RooWorkspace("workspace")),
  mbb_("mbb"),
  data_("data_container"),
  signal_("signal_container"),
  bkg_("background_container") {
  RooRealVar mbb(mbb_.c_str(), "m_{bb}",
                 data.GetXaxis()->GetXmin(), data.GetXaxis()->GetXmax(), "GeV");
  fitRangeMin_ = mbb.getMin();
  fitRangeMax_ = mbb.getMax();
  workspace_.import(mbb);

  // Name and title of the dataset MUST be identical (see initialize() method).
  RooDataHist dataContainer(data_.c_str(), data_.c_str(), mbb, &data);
  RooDataHist signalContainer(signal_.c_str(), signal_.c_str(), mbb, &signal);
  RooDataHist bkgContainer(bkg_.c_str(), bkg_.c_str(), mbb, &bkg);
  workspace_.import(dataContainer);
  workspace_.import(signalContainer);
  workspace_.import(bkgContainer);
}


FitContainer::FitContainer(const HistContainer& container) :
  FitContainer(*container.data(), *container.bbH(), *container.background()) {
}


FitContainer::FitContainer(TTree& data) :
  initialized_(false),
  plotDir_(getOutputPath_("plots")),
  workspaceDir_(getOutputPath_("workspace")),
  fullRangeId_("full_range"),
  fitRangeId_("fit_range"),
  verbosity_(1),
  workspace_(RooWorkspace("workspace")),
  mbb_("mbb"),
  weight_("weight"),
  data_("data_container") {
  RooRealVar mbb(mbb_.c_str(), "m_{bb}",
                 0.0, data.GetMaximum(mbb_.c_str()), "GeV");
  fitRangeMin_ = mbb.getMin();
  fitRangeMax_ = mbb.getMax();
  RooRealVar weight(weight_.c_str(), "weight", 0.0, 1.0);
  workspace_.import(mbb);
  workspace_.import(weight);

  // Name and title of the dataset MUST be identical (see initialize() method).
  RooDataSet dataContainer(data_.c_str(), data_.c_str(), RooArgSet(mbb, weight),
                           RooFit::Import(data),
                           RooFit::WeightVar(weight_.c_str()));
  workspace_.import(dataContainer);
}


FitContainer::FitContainer(const TreeContainer& container) :
  FitContainer(*container.data()) {
}


FitContainer::~FitContainer() {
  workspace_.writeToFile((workspaceDir_+"workspace.root").c_str());
}


void FitContainer::initialize() {
  // Get back the name of the imported datasets. For some reason these are
  // deleted in the workspace after the constructor ends.
  // To get this hack here working, name and title of the dataset MUST be set
  // identical (see constructor methods).
  for (const auto& d: workspace_.allData()) d->SetName(d->GetTitle());

  // clean up possible pre-existing output:
  gSystem->Exec((std::string("rm -f "+plotDir_+"*").c_str()));
  gSystem->Exec((std::string("rm -f "+workspaceDir_+"*").c_str()));

  // set range used for normalization of the pdf and a default fit range:
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  mbb.setRange(fullRangeId_.c_str(), mbb.getMin(), mbb.getMax());
  mbb.setRange(fitRangeId_.c_str(), fitRangeMin_, fitRangeMax_);

  // plot the input data:
  RooAbsData& data = *workspace_.data(data_.c_str());
  std::unique_ptr<RooPlot> frame(mbb.frame());
  data.plotOn(frame.get());
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  prepareCanvas_(canvas);
  prepareFrame_(*frame);
  frame->Draw();
  canvas.SaveAs((plotDir_+"input_data.pdf").c_str());
  canvas.SetLogy();
  canvas.SaveAs((plotDir_+"input_data_log.pdf").c_str());

  initialized_ = true;
}


FitContainer& FitContainer::verbosity(int level) {
  verbosity_ = level;
  return *this;
}


FitContainer& FitContainer::fitRangeMin(float min) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  fitRangeMin_ = min;
  mbb.setMin(min);
  mbb.setRange(fullRangeId_.c_str(), mbb.getMin(), mbb.getMax());
  mbb.setRange(fitRangeId_.c_str(), fitRangeMin_, fitRangeMax_);
  return *this;
}


FitContainer& FitContainer::fitRangeMax(float max) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  fitRangeMax_ = max;
  mbb.setMax(max);
  mbb.setRange(fullRangeId_.c_str(), mbb.getMin(), mbb.getMax());
  mbb.setRange(fitRangeId_.c_str(), fitRangeMin_, fitRangeMax_);
  return *this;
}


void FitContainer::setModel(const Type& type, const std::string& name) {
  const std::vector<ParamModifier> modifiers; // empty modifier list
  setModel(type, name, modifiers);
}


void FitContainer::setModel(const Type& type, const std::string& name,
                            const std::vector<ParamModifier>& modifiers) {
  if (!initialized_) initialize();

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
    try {
      numCoeffs = std::stoi(nameSplitted[1]);
    } catch (const std::invalid_argument& ex) {
      if (std::string(ex.what()) == "stoi") {
        std::cerr << "Conversion of '" << nameSplitted[1]
                  << "' to an integral value for the number of coefficients "
                  << "failed. Using the default value of " << numCoeffs << "."
                  << std::endl;
      } else {
        throw;
      }
    }
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
  else if (nameSplitted[0] == "chebeffprod") setChebEffProd_(type, numCoeffs);
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
  if (!initialized_) initialize();

  RooAbsPdf& bkg = *(workspace_.pdf(toString(Type::background).c_str()));
  if (&bkg == nullptr) {
    throw std::logic_error("No background model has been set.");
  }

  // get the objects from the workspace:
  RooAbsData& data = *workspace_.data(data_.c_str());
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());

  std::unique_ptr<RooFitResult>
    fitResult(bkg.fitTo(data,
                        RooFit::Save(),
                        RooFit::PrintLevel(verbosity_),
                        RooFit::Range(fitRangeId_.c_str())));

  std::cout << "\nconstant parameters:" << std::endl;
  fitResult->constPars().Print("v");
  std::cout << "\nfloating parameters (init):" << std::endl;
  fitResult->floatParsInit().Print("v");
  std::cout << "\nfloating parameters (final):" << std::endl;
  fitResult->floatParsFinal().Print("v");

  std::unique_ptr<RooPlot> frame(mbb.frame());
  data.plotOn(frame.get(), RooFit::Name("data_curve"));
  bkg.plotOn(frame.get(),
             RooFit::Name("background_curve"),
             RooFit::NormRange(fullRangeId_.c_str()),
             RooFit::Range(fitRangeId_.c_str()),
             RooFit::Normalization(data.sumEntries("1", fitRangeId_.c_str()),
                                   RooAbsReal::NumEvent));

  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  prepareCanvas_(canvas);
  prepareFrame_(*frame);
  frame->Draw();

  TLatex latex;
  latex.SetTextFont(43);
  latex.SetTextSize(30);
  latex.SetTextAlign(11);
  latex.DrawLatexNDC(canvas.GetLeftMargin()-0.004, 1.02-canvas.GetTopMargin(),
                     "Background-only fit");

  int nPars = fitResult->floatParsFinal().getSize();
  int ndf = getNonZeroBins_(data) - nPars;
  double normChi2 = frame->chiSquare("background_curve", "data_curve", nPars);
  std::string chi2str(Form("%.1f/%d = %.1f", normChi2 * ndf, ndf, normChi2));
  std::cout << "\nNormalized chi^2: " << chi2str << std::endl;
  latex.SetTextSize(20);
  latex.SetTextAlign(33);
  latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.98-canvas.GetTopMargin(),
                     (std::string("#chi^{2}_{RooFit}/ndf = ")+chi2str).c_str());
  double myChi2 = chiSquare_(data,
                             *(frame->getCurve("background_curve")));
  std::string myChi2str(Form("%.1f/%d = %.1f", myChi2, ndf, myChi2/ndf));
  std::cout << "\nMy normalized chi^2: " << myChi2str << std::endl;
  latex.SetTextSize(20);
  latex.SetTextAlign(33);
  latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.93-canvas.GetTopMargin(),
                     (std::string("#chi^{2}/ndf = ")+myChi2str).c_str());

  canvas.SaveAs((plotDir_+"background_only_fit.pdf").c_str());
  canvas.SetLogy();
  frame->GetYaxis()->SetRangeUser(0.2, 2.0*frame->GetMaximum());
  canvas.Modified();
  canvas.Update();
  canvas.SaveAs((plotDir_+"background_only_fit_log.pdf").c_str());

  return fitResult;
}


void FitContainer::profileModel(const Type& type) {
  RooAbsPdf& model= *(workspace_.pdf(toString(type).c_str()));
  if (&model == nullptr) {
    std::stringstream msg;
    msg << "No model of type '" << toString(type) << "' is set, yet.";
    throw std::logic_error(msg.str());
  }

  // get the objects from the workspace:
  RooAbsData& data = *workspace_.data(data_.c_str());
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());

  std::unique_ptr<RooAbsReal> nll(model.createNLL(data));
  std::unique_ptr<RooArgSet> parameters(model.getParameters(mbb));
  std::unique_ptr<TIterator> iter(parameters->createIterator());
  // use raw pointer for 'parameter' because 'model' owns the object it points to:
  auto parameter = static_cast<RooRealVar*>(iter->Next());
  while (parameter) {
    if (!(parameter->isConstant())) {
      std::unique_ptr<RooAbsReal> profile(nll->createProfile(*parameter));
      std::unique_ptr<RooPlot> frame(parameter->frame());
      if (frame == nullptr) {
        std::stringstream msg;
        msg << "Problems creating frame for '" << parameter->GetName() << "'.";
        throw std::runtime_error(msg.str());
      }
      profile->plotOn(frame.get());
      TCanvas canvas("canvas", "", 600, 600);
      prepareCanvas_(canvas);
      prepareFrame_(*frame);
      frame->Draw();
      canvas.SaveAs((plotDir_+toString(type)+"_profile_"+
                     parameter->GetName()+".pdf").c_str());
      canvas.SetLogy();
      canvas.SaveAs((plotDir_+toString(type)+"_profile_"+
                     parameter->GetName()+"_log.pdf").c_str());
    }
    parameter = static_cast<RooRealVar*>(iter->Next());
  }
}


void FitContainer::showModels() const {
  std::cout << "\n=============================================" << std::endl;
  std::cout << "Defined Models" << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooArgSet models(workspace_.allPdfs());
  std::unique_ptr<TIterator> itModel(models.createIterator());
  // use raw pointer for 'model' because 'models' owns the object it points to:
  auto model = static_cast<RooAbsPdf*>(itModel->Next());
  while (model) {
    model->Print();
    std::unique_ptr<RooArgSet> parameters(model->getParameters(mbb));
    std::unique_ptr<TIterator> itPar(parameters->createIterator());
    // use raw pointer for 'parameter' because 'model' owns the object it points to:
    auto parameter = static_cast<RooRealVar*>(itPar->Next());
    while (parameter) {
      parameter->Print();
      parameter = static_cast<RooRealVar*>(itPar->Next());
    }
    model = static_cast<RooAbsPdf*>(itModel->Next());
    std::cout << "---------------------------------------------" << std::endl;
  }
  std::cout << std::endl;
}


void FitContainer::setNovosibirsk_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo(toString(type).c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb, peak, width, tail);
  workspace_.import(novo);
}


void FitContainer::setCrystalBall_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar m0("m0", "m0", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha", "alpha", -1.0, -0.1);
  RooRealVar n("n", "n", 20.0, 3.0, 100.0);
  RooCBShape cb(toString(type).c_str(), (toString(type)+"_crystalball").c_str(),
                mbb, m0, sigma, alpha, n);
  workspace_.import(cb);
}


void FitContainer::setNovoEffProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo((toString(type)+"_novosibirsk").c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb, peak, width, tail);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon",
                    mbb.getMin()+ 5.0, mbb.getMin(), mbb.getMax());
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*(mbb-turnon)) + 1)",
                    RooArgSet(mbb, slope, turnon));

  RooEffProd novoEffProd(toString(type).c_str(),
                         (toString(type)+"_novoeffprod").c_str(), novo, eff);
  workspace_.import(novoEffProd);
}


void FitContainer::setCBEffProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar m0("m0", "m0", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha", "alpha", -1.0, -0.1);
  RooRealVar n("n", "n", 20.0, 3.0, 100.0);
  RooCBShape cb((toString(type)+"_crystalball").c_str(),
                (toString(type)+"_crystalball").c_str(),
                mbb, m0, sigma, alpha, n);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon",
                    mbb.getMin()+ 5.0, mbb.getMin(), mbb.getMax());
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*(mbb-turnon)) + 1)",
                    RooArgSet(mbb, slope, turnon));

  RooEffProd cbEffProd(toString(type).c_str(),
                       (toString(type)+"_cbeffprod").c_str(), cb, eff);
  workspace_.import(cbEffProd);
}


void FitContainer::setExpEffProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar tau("tau", "tau", -0.1);
  tau.setConstant(false);
  RooExponential exp((toString(type)+"_exp").c_str(),
                     (toString(type)+"_exp").c_str(), mbb, tau);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon",
                    mbb.getMin() + 5.0, mbb.getMin(), mbb.getMax());
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*(mbb-turnon)) + 1)",
                    RooArgSet(mbb, slope, turnon));

  RooEffProd expEffProd(toString(type).c_str(),
                        (toString(type)+"_expeffprod").c_str(), exp, eff);
  workspace_.import(expEffProd);
}


void FitContainer::setDoubleCB_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar mean("mean", "mean", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 35.0, 5.0, 100.0, "GeV");
  RooRealVar alpha1("alpha1", "alpha1", -1.0, -0.1);
  RooRealVar n1("n1", "n1", 20.0, 3.0, 100.0);
  RooRealVar alpha2("alpha2", "alpha2", 0.1, 1.0);
  RooRealVar n2("n2", "n2", 20.0, 3.0, 100.0);
  RooDoubleCB doubleCB(toString(type).c_str(),
                       (toString(type)+"_doublecb").c_str(),
                       mbb, mean, width, alpha1, n1, alpha2, n2);
  workspace_.import(doubleCB);
}


void FitContainer::setExpGausExp_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar mean("mean", "mean", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
  RooRealVar left("left", "left", 0.1, 15.0);
  RooRealVar right("right", "right", 0.1, 15.0);
  RooExpGausExp expGausExp(toString(type).c_str(),
                           (toString(type)+"_expgausexp").c_str(),
                           mbb, mean, sigma, left, right);
  workspace_.import(expGausExp);
}


void FitContainer::setExpBWExp_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 35.0, 5.0, 150.0, "GeV");
  RooRealVar left("left", "left", 0.1, 15.0);
  RooRealVar right("right", "right", 0.1, 15.0);
  RooExpBWExp expBWExp(toString(type).c_str(),
                       (toString(type)+"_expbwexp").c_str(),
                       mbb, peak, width, left, right);
  workspace_.import(expBWExp);
}


void FitContainer::setBukin_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar Xp("Xp", "Xp", getPeakStart_(type, 350.0), 50.0, 350.0, "GeV");
  RooRealVar sigp("sigp", "sigp", 20.0, 85.0, "GeV");
  RooRealVar xi("xi", "xi", 0.0, 0.55);
  RooRealVar rho1("rho1", "rho1", 0.05, -0.1, 0.2);
  RooRealVar rho2("rho2", "rho2", -0.05, -0.07, 0.045);
  RooBukinPdf bukin(toString(type).c_str(),
                    (toString(type)+"_bukin").c_str(),
                    mbb, Xp, sigp, xi, rho1, rho2);
  workspace_.import(bukin);
}


void FitContainer::setBernstein_(const Type& type, const int numCoeffs) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern(toString(type).c_str(),
                    (toString(type)+"_bernstein").c_str(), mbb, *coeffs);
  workspace_.import(bern);
}


void FitContainer::setChebychev_(const Type& type, const int numCoeffs) {
  if (numCoeffs > 7) {
    throw std::invalid_argument
      ("Chebychev polynomials support not more than 7 coefficients.");
  }
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "chebychev"));
  RooChebychev cheb(toString(type).c_str(),
                    (toString(type)+"_chebychev").c_str(), mbb, *coeffs);
  workspace_.import(cheb);
}


void FitContainer::setBernEffProd_(const Type& type, const int numCoeffs) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern((toString(type)+"_bernstein").c_str(),
                    (toString(type)+"_bernstein").c_str(),
                    mbb, *coeffs);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon",
                     mbb.getMin() + 10.0, mbb.getMin(), mbb.getMax());
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*(mbb-turnon)) + 1)",
                    RooArgSet(mbb, slope, turnon));

  RooEffProd bernEffProd(toString(type).c_str(),
                         (toString(type)+"_berneffprod").c_str(), bern, eff);
  workspace_.import(bernEffProd);
}


void FitContainer::setChebEffProd_(const Type& type, const int numCoeffs) {
  if (numCoeffs > 7) {
    throw std::invalid_argument
      ("Chebychev polynomials support not more than 7 coefficients.");
  }
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "chebychev"));
  RooChebychev cheb((toString(type)+"_chebychev").c_str(),
                    (toString(type)+"_chebychev").c_str(), mbb, *coeffs);

  RooRealVar slope("slope", "slope", 0.01, 0.0, 0.1);
  RooRealVar turnon("turnon", "turnon",
                    mbb.getMin()+ 5.0, mbb.getMin(), mbb.getMax());
  RooFormulaVar eff((toString(type)+"_eff").c_str(),
                    "0.5*(TMath::Erf(slope*(mbb-turnon)) + 1)",
                    RooArgSet(mbb, slope, turnon));

  RooEffProd chebEffProd(toString(type).c_str(),
                         (toString(type)+"_chebeffprod").c_str(), cheb, eff);
  workspace_.import(chebEffProd);
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


double FitContainer::getPeakStart_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  return getPeakStart_(type, mbb.getMax());
}


double FitContainer::getPeakStart_(const Type& type, double max) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  double peakStart = (mbb.getMin() + max) / 2.0;
  switch (type) {
  case Type::signal:
    if (signal_ != "") {
      RooAbsData& signal = *workspace_.data(signal_.c_str());
      peakStart = getMaxPosition_(signal);
    }
    break;
  case Type::background:
    if (bkg_ != "") {
      RooAbsData& bkg = *workspace_.data(bkg_.c_str());
      peakStart = getMaxPosition_(bkg);
    }
    break;
  }
  return peakStart < max ? peakStart : max;
}


double FitContainer::getMaxPosition_(const RooAbsData& data) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<TH1> hist(data.createHistogram(mbb_.c_str(), mbb));
  int maximumBin = hist->GetMaximumBin();
  return hist->GetBinCenter(maximumBin);
}


int FitContainer::getNonZeroBins_(const RooAbsData& data) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<TH1> hist(data.createHistogram(mbb_.c_str(), mbb));
  int nonZeroBins = 0;
  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    double center = hist->GetBinCenter(i);
    if (hist->GetBinContent(i) > 0 &&
        center > fitRangeMin_ && center < fitRangeMax_) ++nonZeroBins;
  }
  return nonZeroBins;
}


double FitContainer::chiSquare_(const RooAbsData& data, const RooCurve& fit) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<TH1> hist(data.createHistogram(mbb_.c_str(), mbb));
  double chi2 = 0.0;
  double avgBinSize = hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin();
  avgBinSize /= hist->GetNbinsX();
  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    double center = hist->GetBinCenter(i);
    double content = hist->GetBinContent(i);
    if (content > 0.0 && center > fitRangeMin_ && center < fitRangeMax_) {
      double densityCorrection = avgBinSize/hist->GetBinWidth(i);
      double model = fit.interpolate(center);
      double dataPoint = content*densityCorrection;
      double pull = (dataPoint - model)/hist->GetBinError(i)/densityCorrection;
      chi2 += pull*pull;
    }
  }
  return chi2;
}


bool FitContainer::applyModifiers_(RooAbsPdf& pdf,
                                   const std::vector<ParamModifier>& modifiers) {
  bool modified = false;
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgSet> parameters(pdf.getParameters(mbb));
  std::unique_ptr<TIterator> iter(parameters->createIterator());
  // use raw pointer for 'parameter' because 'pdf' owns the object it points to:
  RooRealVar* parameter = static_cast<RooRealVar*>(iter->Next());
  while (parameter) {
    for (const auto& m : modifiers) {
      if (m.modify(*parameter)) {
        modified = true;
      }
    }
    parameter = static_cast<RooRealVar*>(iter->Next());
  }
  if (modifiers.size() > 0 && !modified) {
    std::cerr << ">>> None of the modifiers provided to '" << pdf.GetName()
              << "' pdf could be applied." << std::endl;
    std::cerr << ">>> Provided modifiers: ";
    for (auto m = modifiers.cbegin(); m != modifiers.cend(); ++m) {
      if (m != modifiers.cbegin()) std::cerr << ",";
      std::cerr << " " << m->name();
    }
    std::cerr << std::endl;
    std::cerr << ">>> Found in pdf: ";
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
   "berneffprod",
   "chebeffprod"};

const int FitContainer::defaultNumberOfCoefficients_ = 7;
