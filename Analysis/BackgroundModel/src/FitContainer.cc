#include <iostream>
#include <sstream>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TLatex.h"
#include "RooAbsReal.h"
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
#include "RooProdPdf.h"
#include "Analysis/BackgroundModel/interface/RooDoubleCB.h"
#include "Analysis/BackgroundModel/interface/RooExpGausExp.h"
#include "Analysis/BackgroundModel/interface/RooGausExp.h"
#include "Analysis/BackgroundModel/interface/RooExpBWExp.h"
#include "Analysis/BackgroundModel/interface/RooPhaseSpace.h"
#include "Analysis/BackgroundModel/interface/RooPhaseSpacePol4.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/Tools.h"


using namespace analysis::backgroundmodel;

//FitContainer::FitContainer(const TH1& data, const TH1& signal, const TH1& bkg,
//			   const std::string& outputDir) :
FitContainer::FitContainer(const TH1& data, const std::string& outputDir) :
  initialized_(false),
  splitrange_(true),
  outputDir_(outputDir+"/"),
  plotDir_(getOutputPath_("plots")),
  workspaceDir_(getOutputPath_("workspace")),
  fullRangeId_("full_range"),
  fitRangeId_("fit_range"),
  fitRangeLowId_("fit_range_low"),
  fitRangeHighId_("fit_range_high"),
  fitSplRangeId_("fit_range_low,fit_range_high"),
  blind_lowEdge_(650.),
  blind_highEdge_(650.),
  verbosity_(1),
  workspace_(RooWorkspace("workspace")),
  outRootFileName_(getOutputPath_("workspace")+"workspace.root"),
  mbb_("mbb"),
  data_("data_container"),
  //signal_("signal_container"),
  //bkg_("background_container"),
  bkgOnlyFit_("fit_b", "fit_b"),
  chi2BkgOnly_(-10000.0),
  normChi2BkgOnly_(-10000.0),
  ndfBkgOnly_(-10000) {
  RooRealVar mbb(mbb_.c_str(), "M_{12}",
                 data.GetXaxis()->GetXmin(), data.GetXaxis()->GetXmax(), "GeV");
  fitRangeMin_ = mbb.getMin();
  fitRangeMax_ = mbb.getMax();
  workspace_.import(mbb);

  // Name and title of the dataset MUST be identical (see initialize() method).
  RooDataHist dataContainer(data_.c_str(), data_.c_str(), mbb, &data);
  //RooDataHist signalContainer(signal_.c_str(), signal_.c_str(), mbb, &signal);
  //RooDataHist bkgContainer(bkg_.c_str(), bkg_.c_str(), mbb, &bkg);
  workspace_.import(dataContainer);
  //workspace_.import(signalContainer);
  //workspace_.import(bkgContainer);
}


FitContainer::FitContainer(const HistContainer& container,
			   const std::string& outputDir) :
//  FitContainer(*container.data(), *container.bbH(), *container.background(),
//	       outputDir) {
  FitContainer(*container.data(), outputDir) {
}


FitContainer::FitContainer(TTree& data, const std::string& outputDir) :
  initialized_(false),
  splitrange_(true),
  outputDir_(outputDir+"/"),
  plotDir_(getOutputPath_("plots")),
  workspaceDir_(getOutputPath_("workspace")),
  fullRangeId_("full_range"),
  fitRangeId_("fit_range"),
  fitRangeLowId_("fit_range_low"),
  fitRangeHighId_("fit_range_high"),
  fitSplRangeId_("fit_range_low,fit_range_high"), 
  blind_lowEdge_(650.),
  blind_highEdge_(650.),
  verbosity_(1),
  workspace_(RooWorkspace("workspace")),
  outRootFileName_(getOutputPath_("workspace")+"workspace.root"),
  mbb_("mbb"),
  weight_("weight"),
  data_("data_container"),
  bkgOnlyFit_("fit_b", "fit_b"),
  chi2BkgOnly_(-10000.0),
  normChi2BkgOnly_(-10000.0),
  ndfBkgOnly_(-10000) {
  RooRealVar mbb(mbb_.c_str(), "M_{12}",
                 0.0, data.GetMaximum(mbb_.c_str()), "GeV");
  fitRangeMin_ = mbb.getMin();
  fitRangeMax_ = mbb.getMax();
  RooRealVar weight(weight_.c_str(), "weight", 0.0, 1000.0);
  workspace_.import(mbb);
  workspace_.import(weight);

  // Name and title of the dataset MUST be identical (see initialize() method).
  RooDataSet dataContainer(data_.c_str(), data_.c_str(), RooArgSet(mbb, weight),
                           RooFit::Import(data),
                           RooFit::WeightVar(weight_.c_str()));
  workspace_.import(dataContainer);
}


FitContainer::FitContainer(const TreeContainer& container,
			   const std::string& outputDir) :
  FitContainer(*container.data(), outputDir) {
}


FitContainer::~FitContainer() {
  workspace_.Print();
  workspace_.writeToFile(outRootFileName_.c_str());
  TFile out(outRootFileName_.c_str(), "update");
  bkgOnlyFit_.SetDirectory(&out);
  bkgOnlyFit_.Write();
  out.Close();
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

  // perform split range simultaneously fit to blinded data by CA
  mbb.setRange(fitRangeLowId_.c_str(), fitRangeMin_, blind_lowEdge_);  //always have to give input of --fit_min
  //mbb.setRange(fitRangeMedId_.c_str(), blind_lowEdge_, blind_highEdge_);
  mbb.setRange(fitRangeHighId_.c_str(), blind_highEdge_, fitRangeMax_);
  
  // set fit bins
  mbb.setBins(73);

  // plot the input data:
  RooAbsData& data = *workspace_.data(data_.c_str());
  std::unique_ptr<RooPlot> frame(mbb.frame());
  data.plotOn(frame.get(),RooFit::DataError(RooAbsData::Auto));
  TCanvas canvas("canvas", "", 600, 600);
  canvas.cd();
  prepareCanvas_(canvas);
  prepareFrame_(*frame);
  frame->Draw();
  canvas.SaveAs((plotDir_+"input_data.pdf").c_str());
  canvas.SetLogy();
  canvas.SaveAs((plotDir_+"input_data_log.pdf").c_str());

  // initialize background-only fit result tree:
  bkgOnlyFit_.Branch("chi2", &chi2BkgOnly_, "chi2/F");
  bkgOnlyFit_.Branch("normChi2", &normChi2BkgOnly_, "normChi2/F");
  bkgOnlyFit_.Branch("ndf", &ndfBkgOnly_, "ndf/I");
  bkgOnlyFit_.Branch("covMatrix", covMatrix_, "covMatrix[20]/D");
  bkgOnlyFit_.Branch("eigenVector", eigenVector_, "eigenVector[20]/D"); 

  for(int i = 0; i < 20; i++)
  {   	covMatrix_[i] = -100.;
	eigenVector_[i] = -100.;
  }	

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
  mbb.setRange(fitRangeLowId_.c_str(), fitRangeMin_, blind_lowEdge_);
  mbb.setRange(fitRangeHighId_.c_str(), blind_highEdge_, fitRangeMax_);

  return *this;
}


FitContainer& FitContainer::fitRangeMax(float max) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  fitRangeMax_ = max;
  mbb.setMax(max);
  mbb.setRange(fullRangeId_.c_str(), mbb.getMin(), mbb.getMax());
  mbb.setRange(fitRangeId_.c_str(), fitRangeMin_, fitRangeMax_);
  mbb.setRange(fitRangeLowId_.c_str(), fitRangeMin_, blind_lowEdge_);
  mbb.setRange(fitRangeHighId_.c_str(), blind_highEdge_, fitRangeMax_);

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

  if (nameSplitted[0] == "novosibirsk") setNovosibirsk_(type);			//Novosibirsk (3)
  else if (nameSplitted[0] == "novopsprod") setNovoPSProd_(type);		//PS x Novosibirsk (3)
  else if (nameSplitted[0] == "novoeffprod") setNovoEffProd_(type);             //Eff x Novosibirsk (5)
  else if (nameSplitted[0] == "novopshighMpol4") setNovoPSHighMPol4_(type);	//PS x Novosibirsk (3) HighM Poly[4]
  else if (nameSplitted[0] == "crystalball") setCrystalBall_(type);
  else if (nameSplitted[0] == "crystalpsprod") setCrystalPSProd_(type);		//PS x CrystalBall (4)
  else if (nameSplitted[0] == "crystaleffprod") setCrystalEffProd_(type);       //Eff x CrystalBall (6)
  else if (nameSplitted[0] == "expeffprod") setExpEffProd_(type);
  else if (nameSplitted[0] == "doublecb") setDoubleCB_(type);
  else if (nameSplitted[0] == "dijetv1") setDijetv1_(type);                     //Dijet (EXO-16-020) (3)
  else if (nameSplitted[0] == "dijetv1psprod") setDijetv1PSProd_(type);         //PS x Dijet (3)
  else if (nameSplitted[0] == "dijetv2") setDijetv2_(type); 			//Dijet X(750) (3)
  else if (nameSplitted[0] == "dijetv2psprod") setDijetv2PSProd_(type);		//PS x Dijet (3)
  else if (nameSplitted[0] == "expgausexp") setExpGausExp_(type);
  else if (nameSplitted[0] == "gausexp") setGausExp_(type);
  else if (nameSplitted[0] == "gausexppsprod") setGausExpPSProd_(type);		//PS x GausExp (3)
  else if (nameSplitted[0] == "expbwexp") setExpBWExp_(type);
  else if (nameSplitted[0] == "bukin") setBukin_(type);
  else if (nameSplitted[0] == "bukinpsprod") setBukinPSProd_(type);		//PS x Bukin (5)
  else if (nameSplitted[0] == "bernstein") setBernstein_(type, numCoeffs);
  else if (nameSplitted[0] == "chebychev") setChebychev_(type, numCoeffs);
  else if (nameSplitted[0] == "berneffprod") setBernEffProd_(type, numCoeffs);
  else if (nameSplitted[0] == "bernpsprod") setBernPSProd_(type, numCoeffs);
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


std::unique_ptr<RooFitResult> FitContainer::backgroundOnlyFit(const std::string& name) {
  if (!initialized_) initialize();

  RooAbsPdf& bkg = *(workspace_.pdf(toString(Type::background).c_str()));
  if (&bkg == nullptr) {
    throw std::logic_error("No background model has been set.");
  }

  // get the objects from the workspace:
  RooAbsData& data = *workspace_.data(data_.c_str());
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());

  //RooDataHist datahist("data_curve","data_curve", mbb, data);  

  //std::unique_ptr<RooFitResult>
  //  fitResult(bkg.fitTo(data,
  //  fitResult(bkg.chi2FitTo(datahist,
  //                      RooFit::Save(),
  //                      RooFit::PrintLevel(verbosity_),
                        //RooFit::SumW2Error(kTRUE),
  //                      RooFit::Range(fitRangeId_.c_str())
  //			));

  // Split Range for blinded data and perform simultaneous fit by CA
  std::unique_ptr<RooFitResult>
    fitResult(bkg.fitTo(data,
                        RooFit::Save(),
                        RooFit::PrintLevel(verbosity_),
                        RooFit::Range(fitSplRangeId_.c_str()),
                        RooFit::SplitRange()
                        ));

  std::cout << "\nconstant parameters:" << std::endl;
  fitResult->constPars().Print("v");
  std::cout << "\nfloating parameters (init):" << std::endl;
  fitResult->floatParsInit().Print("v");
  std::cout << "\nfloating parameters (final):" << std::endl;
  fitResult->floatParsFinal().Print("v");

  // Top frame
  std::unique_ptr<RooPlot> frame(mbb.frame());
  data.plotOn(frame.get(),
	     RooFit::MarkerSize(0.8),	//0.8 for lowM
	     //RooFit::DataError(RooAbsData::Auto), 
	     RooFit::Name("data_curve"));
  bkg.plotOn(frame.get(),
	     //RooFit::VisualizeError(*fitResult,1,kFALSE),
             //RooFit::DrawOption("L"),
             RooFit::LineColor(kRed),
             //RooFit::LineWidth(2),
	     //RooFit::LineStyle(kDashed),
	     //RooFit::FillColor(kOrange),
             RooFit::Name("background_curve"),
             RooFit::NormRange(fullRangeId_.c_str()),
             RooFit::Range(fitRangeId_.c_str()),
             RooFit::Normalization(data.sumEntries("1", fitRangeId_.c_str()),
                                   RooAbsReal::NumEvent));

  int nPars = fitResult->floatParsFinal().getSize();
 
  // Get Covariance Matrix for diagonalisation 
  TMatrixDSymEigen CM = fitResult->covarianceMatrix();
  TMatrixD covVec = CM.GetEigenVectors();
  TMatrixD covMat = fitResult->covarianceMatrix(); 
  covMat.GetMatrix2Array(covMatrix_);
  covVec.GetMatrix2Array(eigenVector_);

  ////////////////////////////////////////////////////////////////////////////////
  //////////////////////// ChiSquare by Roofit ///////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  if (!splitrange_) {	
  	ndfBkgOnly_ = getNonZeroBins_(data) - nPars;
  	normChi2BkgOnly_ = frame->chiSquare("background_curve", "data_curve", nPars); //chi^2 from RooFit (RooPlot::chiSquare())
  	chi2BkgOnly_ = normChi2BkgOnly_ * ndfBkgOnly_;
  	bkgOnlyFit_.Fill();
  }

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////ChiSquare by Gregor/////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  //double myChi2 = chiSquare_(data,
  //                           *(frame->getCurve("background_curve")),
  //				blind_lowEdge_, blind_highEdge_, nPars);
  //std::string myChi2str(Form("%.1f/%d = %.1f", myChi2, ndfBkgOnly_,
  //			     myChi2/ndfBkgOnly_));
  //std::cout << "\nMy normalized chi^2: " << myChi2str << std::endl;
  //latex.DrawLatexNDC(0.98-canvas.GetRightMargin(), 0.93-canvas.GetTopMargin(),
  //                   (std::string("#chi^{2}/ndf = ")+myChi2str).c_str());

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////ChiSquare by Chayanit///////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  else {
  	ndfBkgOnly_ = getNonZeroBins_(data) - nPars - getBlindedBins_(data,blind_lowEdge_,blind_highEdge_); // excluded blinded region
  	normChi2BkgOnly_ = chiSquare_CA(*frame, "background_curve", "data_curve", nPars, blind_lowEdge_, blind_highEdge_);
	chi2BkgOnly_ = normChi2BkgOnly_ * ndfBkgOnly_;
	bkgOnlyFit_.Fill();
  }
  std::string chi2str(Form("%.1f/%d = %.1f", chi2BkgOnly_,
		 	   ndfBkgOnly_, normChi2BkgOnly_));
  std::cout << "\nNormalized chi^2: " << chi2str << std::endl;

  // TMath::Prob p-value by CA
  double prob = TMath::Prob(chi2BkgOnly_,ndfBkgOnly_);
  std::string probstr(Form("%.2f", prob));
  std::cout << "\nTMath::Prob(#chi^{2}_{RooFit},ndf) : " << probstr << std::endl;
 
  // Bottom frame
  RooHist* hpull;
  hpull = frame->pullHist();
  hpull->SetMarkerSize(0.8);	//0.8 for lowM
  std::unique_ptr<RooPlot> frame2(mbb.frame());
  frame2->addPlotable(hpull,"P");
  //frame2->addObject(frame->pullHist()); 
  frame2->SetMinimum(-5.); 
  frame2->SetMaximum(+5.); 

  //TCanvas canvas("canvas", "", 600, 600);
  TCanvas canvas;
  canvas.SetCanvasSize(500,500);
  canvas.cd();
  //prepareCanvas_(canvas);
  prepareFrame_(*frame);
  prepareFrame_(*frame2);

  TPad* pad1;
  pad1 = new TPad("pad1","",0,0.1,1,1);
  pad1->SetBottomMargin(0.2);
  pad1->SetRightMargin(0.05); 
  pad1->SetLeftMargin(0.16); 
  pad1->Draw();
  pad1->cd();
  frame->GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
  frame->GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
  frame->GetYaxis()->SetTitleSize(0.038);
  frame->GetYaxis()->SetTitleOffset(1.6);
  frame->GetYaxis()->SetLabelSize(0.033);
  //frame->GetYaxis()->SetRangeUser(frame->GetMinimum(), frame->GetMaximum()+200);
  frame->Draw();

  TLatex latex;
  latex.SetTextFont(43);
  latex.SetTextSize(17);
  latex.SetTextAlign(11);
  latex.DrawLatexNDC(pad1->GetLeftMargin(), 1.02-canvas.GetTopMargin(),
  //                   "CMS Preliminary #sqrt{s} = 13 TeV, L = 2.69 fb^{-1}");
		     "CMS Preliminary #sqrt{s} = 13 TeV, L = 12.89 fb^{-1}");
  latex.SetTextSize(15);
  latex.SetTextAlign(33);
  latex.SetTextColor(kBlue+2);
  latex.DrawLatexNDC(0.98-pad1->GetRightMargin(), 0.98-pad1->GetTopMargin(),
                     (std::string("#chi^{2}_{RooFit}/ndf = ")+chi2str).c_str());
  latex.SetTextColor(kGreen+2);
  latex.DrawLatexNDC(0.98-pad1->GetRightMargin(), 0.93-pad1->GetTopMargin(),
  		       (std::string("p-value = ")+probstr).c_str());
  latex.SetTextColor(kOrange+2);
  std::string minstr(Form("%.0f", fitRangeMin_));
  std::string maxstr(Form("%.0f", fitRangeMax_));
  latex.DrawLatexNDC(0.98-pad1->GetRightMargin(), 0.88-pad1->GetTopMargin(), 
  		       (minstr+std::string(" < M_{12} < ")+maxstr).c_str());

  canvas.cd();
  TPad *pad2 = new TPad("pad2","",0,0.0,1,0.265);
  pad2->SetTopMargin(1);
  pad2->SetBottomMargin(0.33);
  pad2->SetLeftMargin(pad1->GetLeftMargin());
  pad2->SetRightMargin(pad1->GetRightMargin());
  pad2->SetGridy();
  pad2->Draw();
  pad2->cd();
  frame2->SetTitle("");
  frame2->GetXaxis()->SetTitleSize(0.15);
  frame2->GetXaxis()->SetTitleOffset(0.9);
  frame2->GetXaxis()->SetLabelSize(0.115);
  frame2->GetXaxis()->SetLabelOffset(0.010);
  frame2->SetYTitle("Pulls");
  frame2->GetYaxis()->CenterTitle(kTRUE);
  frame2->GetYaxis()->SetTitleSize(0.14);
  frame2->GetYaxis()->SetTitleOffset(0.4);
  frame2->GetYaxis()->SetNdivisions(3,5,0);
  frame2->GetYaxis()->SetLabelSize(0.115);
  frame2->GetYaxis()->SetLabelOffset(0.011);
  frame2->Draw();

  canvas.SaveAs((plotDir_+name+"_lowM_linear.pdf").c_str());
  pad1->SetLogy();
  frame->GetYaxis()->SetRangeUser(0.1, frame->GetMaximum()*5);
  canvas.Modified();
  canvas.Update();
  canvas.SaveAs((plotDir_+name+"_lowM_log.pdf").c_str());

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


void FitContainer::setNovoPSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak_novops", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width_novops", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  RooRealVar tail("tail_novops", "tail", -0.1, -1.0, 1.0);

  //RooNovosibirsk novo((toString(type)+"_novosibirsk").c_str(),
  RooNovosibirsk novo((toString(type)+"_novops").c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb, peak, width, tail);

  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
			  "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
   		      	  RooArgList(mbb));
  RooEffProd novopsprod(toString(type).c_str(),
                        (toString(type)+"_novopsprod").c_str(), novo, phasespace);
  workspace_.import(novopsprod);
}


void FitContainer::setNovoEffProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak_novoeff", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width_novoeff", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  RooRealVar tail("tail_novoeff", "tail", -0.1, -1.0, 1.0);

  RooNovosibirsk novo((toString(type)+"_novoeff").c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb, peak, width, tail);

  RooRealVar slope_novoeff("slope_novoeff", "slope_novoeff", 0.01, 0.0, 0.1);
  RooRealVar turnon_novoeff("turnon_novoeff", "turnon_novoeff",
                    mbb.getMin()+ 5.0, mbb.getMin(), mbb.getMax());

  RooFormulaVar eff((toString(type)+"_novosibirskeff").c_str(),
                    "0.5*(TMath::Erf(slope_novoeff*(mbb-turnon_novoeff)) + 1)",
                    RooArgSet(mbb, slope_novoeff, turnon_novoeff));

  RooEffProd novoEffProd(toString(type).c_str(),
                         (toString(type)+"_novoeffprod").c_str(), novo, eff);
  workspace_.import(novoEffProd);
}


void FitContainer::setNovoPSHighMPol4_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar peak("peak", "peak", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 50.0, 5.0, mbb.getMax()/2.0, "GeV");
  RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
  RooNovosibirsk novo((toString(type)+"_novosibirsk").c_str(),
                      (toString(type)+"_novosibirsk").c_str(),
                      mbb, peak, width, tail);

  RooPhaseSpacePol4 phasespace((toString(type)+"_pspol4").c_str(),
			       (toString(type)+"_phasespace").c_str(),mbb);

  RooEffProd novopsprod(toString(type).c_str(),
                        (toString(type)+"_novopsprod").c_str(), novo, phasespace);
  workspace_.import(novopsprod);
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


void FitContainer::setCrystalPSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar m0("m0_cbps", "m0", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma_cbps", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha_cbps", "alpha", -1.0, -0.1);
  RooRealVar n("n_cbps", "n", 20.0, 3.0, 100.0);
  RooCBShape cb((toString(type)+"_cbps").c_str(), 
		(toString(type)+"_crystalball").c_str(),
                mbb, m0, sigma, alpha, n);

  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd crystalpsprod(toString(type).c_str(),
                          (toString(type)+"_crystalpsprod").c_str(), cb, phasespace);
  workspace_.import(crystalpsprod);
}


void FitContainer::setCrystalEffProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar m0("m0_cbeff", "m0", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma_cbeff", "sigma", 35.0, 10.0, 100.0, "GeV");
  RooRealVar alpha("alpha_cbeff", "alpha", -1.0, -0.1);
  RooRealVar n("n_cbeff", "n", 20.0, 3.0, 100.0);

  RooCBShape cb((toString(type)+"_cbeff").c_str(),
                (toString(type)+"_crystalball").c_str(),
                mbb, m0, sigma, alpha, n);

  RooRealVar slope_cbeff("slope_cbeff", "slope_cbeff", 0.01, 0.0, 0.1);
  RooRealVar turnon_cbeff("turnon_cbeff", "turnon_cbeff",
                    mbb.getMin()+ 5.0, mbb.getMin()- 50.0, mbb.getMax());
  RooFormulaVar eff((toString(type)+"_crystaleff").c_str(),
                    "0.5*(TMath::Erf(slope_cbeff*(mbb-turnon_cbeff)) + 1)",
                    RooArgSet(mbb, slope_cbeff, turnon_cbeff));

  RooEffProd crystalEffprod(toString(type).c_str(),
                            (toString(type)+"_crystaleffprod").c_str(), cb, eff);
  workspace_.import(crystalEffprod);
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


void FitContainer::setDijetv1_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  //RooRealVar par_a("par_a", "par_a", 1.0, 0.0, 10.0);
  RooRealVar par_b("par_b", "par_b", -13, -1000, 1000.);
  RooRealVar par_c("par_c", "par_c", -1.4, -1000, 1000.);
  RooGenericPdf dijet(toString(type).c_str(),
                     (toString(type)+"_dijet").c_str(),
                     "pow(mbb/13000. , par_b + par_c * TMath::Log(mbb/13000.))",	//(x)^(P1+P2log(x)) where x = mbb/sqrt(s)
                     RooArgList(mbb,par_b,par_c));
  workspace_.import(dijet);
}


void FitContainer::setDijetv1PSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  //RooRealVar par_a("par_a", "par_a", 1.0, 0.0, 10.0);
  RooRealVar par_b("par_b", "par_b", -13, -1000, 1000.);
  RooRealVar par_c("par_c", "par_c", -1.4, -1000, 1000.);
  RooGenericPdf dijet((toString(type)+"_dijetpdf").c_str(),
		      (toString(type)+"_dijet").c_str(),
		      "pow(mbb/13000. , par_b + par_c * TMath::Log(mbb/13000.))",
		      RooArgList(mbb,par_b,par_c));
  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd dijetpsprod(toString(type).c_str(),
                        (toString(type)+"_dijetpsprod").c_str(), dijet, phasespace);
  workspace_.import(dijetpsprod);
}


void FitContainer::setDijetv2_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar par_a("par_a", "par_a", 8.0, 0.0, 10.0);
  RooRealVar par_b("par_b", "par_b", 0.06, 0.02, 0.08);
  RooRealVar par_c("par_c", "par_c", 70., 65., 100.);			//control non-zero values when start mbb > 200 GeV
  RooGenericPdf dijet(toString(type).c_str(),				//(-1+cx)e^-alnx(1+blnx)
                     (toString(type)+"_dijet").c_str(),
                     "TMath::Exp(-par_a * TMath::Log(mbb/13000.) - par_b*par_a*(pow(TMath::Log(mbb/13000.),2)))*(-1+par_c*(mbb/13000.))",
                     RooArgList(mbb,par_a,par_b,par_c));
  workspace_.import(dijet);
}


void FitContainer::setDijetv2PSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar par_a("par_a", "par_a", 8.0, 0.0, 10.0);
  RooRealVar par_b("par_b", "par_b", 0.06, 0.02, 0.08);
  RooRealVar par_c("par_c", "par_c", 70., 65., 100.);
  RooGenericPdf dijet((toString(type)+"_dijetpdf").c_str(),
                      (toString(type)+"_dijet").c_str(),
                      "TMath::Exp(-par_a * TMath::Log(mbb/13000.) - par_b*par_a*(pow(TMath::Log(mbb/13000.),2)))*(-1+par_c*(mbb/13000.))",
                      RooArgList(mbb,par_a,par_b,par_c));
  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd dijetpsprod(toString(type).c_str(),
                        (toString(type)+"_dijetpsprod").c_str(), dijet, phasespace);
  workspace_.import(dijetpsprod);
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


void FitContainer::setGausExp_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar mean("mean", "mean", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
  RooRealVar right("right", "right", 0.4, 0.1, 15.0);
  RooGausExp gausExp(toString(type).c_str(),
                           (toString(type)+"_gausexp").c_str(),
                           mbb, mean, sigma, right);
  workspace_.import(gausExp);
}


void FitContainer::setGausExpPSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar mean("mean", "mean", getPeakStart_(type, 500.0), 50.0, 500.0, "GeV");
  RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
  RooRealVar right("right", "right", 0.4, 0.1, 15.0);
  RooGausExp gausExp((toString(type)+"_gausexp").c_str(),
                     (toString(type)+"_gausexp").c_str(),
                     mbb, mean, sigma, right);

  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd gausExppsprod(toString(type).c_str(),
                          (toString(type)+"_gausExppsprod").c_str(), gausExp, phasespace);
  workspace_.import(gausExppsprod);
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
  RooRealVar rho1("rho1", "rho1", 0.05, -0.1, 0.1); 
  RooRealVar rho2("rho2", "rho2", -0.05, -0.07, 0.045);
  RooBukinPdf bukin(toString(type).c_str(),
                    (toString(type)+"_bukin").c_str(),
                    mbb, Xp, sigp, xi, rho1, rho2);
  workspace_.import(bukin);
}


void FitContainer::setBukinPSProd_(const Type& type) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  RooRealVar Xp("Xp", "Xp", getPeakStart_(type, 350.0), 50.0, 350.0, "GeV");
  RooRealVar sigp("sigp", "sigp", 20.0, 85.0, "GeV");
  RooRealVar xi("xi", "xi", 0.0, 0.55);
  RooRealVar rho1("rho1", "rho1", 0.05, -0.1, 0.2);
  RooRealVar rho2("rho2", "rho2", -0.05, -0.07, 0.045);
  RooBukinPdf bukin((toString(type)+"_bukin").c_str(),
                    (toString(type)+"_bukin").c_str(),
                    mbb, Xp, sigp, xi, rho1, rho2);

  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd bukinpsprod(toString(type).c_str(),
                          (toString(type)+"_bukinpsprod").c_str(), bukin, phasespace);
  workspace_.import(bukinpsprod);
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


void FitContainer::setBernPSProd_(const Type& type, const int numCoeffs) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
  RooBernstein bern((toString(type)+"_bernstein").c_str(),
                    (toString(type)+"_bernstein").c_str(),
                     mbb, *coeffs);

  RooFormulaVar phasespace((toString(type)+"_phasespace").c_str(),
                          "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)",
                          RooArgList(mbb));
  RooEffProd bernPSProd(toString(type).c_str(),
                        (toString(type)+"_bernPSProd").c_str(), bern, phasespace);

  workspace_.import(bernPSProd);
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
  std::string path = outputDir_ + subdirectory;
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
  std::cout << "number of Non-Zero bins : " << nonZeroBins << std::endl;
  return nonZeroBins;
}


int FitContainer::getBlindedBins_(const RooAbsData& data, double blind_lowEdge, double blind_highEdge) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<TH1> hist(data.createHistogram(mbb_.c_str(), mbb));
  int blindedBins = 0;
  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    double center = hist->GetBinCenter(i);
    if (hist->GetBinContent(i) > 0. && center > blind_lowEdge && center < blind_highEdge) ++blindedBins;
  }
  std::cout << "number of blinded bins : " << blindedBins << std::endl;
  return blindedBins;
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
      std::cout << "chi^2 at bin " << i << " : " << pull*pull << std::endl;
      chi2 += pull*pull;
    }
  }
  return chi2;
}


double FitContainer::chiSquare_(const RooAbsData& data, const RooCurve& fit, double blind_lowEdge, double blind_highEdge, int nFitParam) {
  RooRealVar& mbb = *workspace_.var(mbb_.c_str());
  std::unique_ptr<TH1> hist(data.createHistogram(mbb_.c_str(), mbb));
  double chi2 = 0.0;
  double avgBinSize = hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin();
  avgBinSize /= hist->GetNbinsX();
  int nbin = 0;

  std::cout << "blind_lowEdge  = " << blind_lowEdge  << std::endl;
  std::cout << "blind_highEdge = " << blind_highEdge << std::endl;

  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
    double center = hist->GetBinCenter(i);  	// x value
    double content = hist->GetBinContent(i);	// y value
    if (content > 0.0 && center > fitRangeMin_ && center < fitRangeMax_ ) {
      std::cout << "center = " << center << std::endl; 
      double densityCorrection = avgBinSize/hist->GetBinWidth(i);
      double model = fit.interpolate(center);
      double dataPoint = content*densityCorrection;
      double pull = (dataPoint - model)/hist->GetBinError(i)/densityCorrection;
      std::cout << "pull^2 at bin " << i << " : " << pull*pull << std::endl;
      chi2 += pull*pull;
      nbin++;
    }
  }
  std::cout << "chi^2 = " << chi2 << std::endl;
  std::cout << "nbin  = " << nbin << std::endl;
  std::cout << "chi^2/ndf = " << chi2 / (nbin-nFitParam) << std::endl;
  return chi2 / (nbin-nFitParam);
}


 
double FitContainer::chiSquare_CA(const RooPlot& frame, const char* curvename, const char* histname, int nFitParam, double blind_lowEdge, double blind_highEdge)
{
  // Find curve object
  RooCurve* curve = (RooCurve*) frame.findObject(curvename,RooCurve::Class());  
  if (!curve) {
  	std::cout << "RooPlot::chiSquare(" << curvename << ") cannot find curve" << std::endl;
  	return -1.;
  }
  // Find histogram object
  RooHist* hist = (RooHist*) frame.findObject(histname,RooHist::Class());
  if (!hist) {
  	std::cout << "RooPlot::chiSquare(" << histname << ") cannot find histogram" << std::endl;
  	return -1.;
  }

  int np = hist->GetN();
  double x,y,eyl,eyh,exl,exh;
   
  // Find starting and ending bin of histogram based on range of RooCurve
  double xstart,xstop; 
  curve->GetPoint(0,xstart,y);
  curve->GetPoint(curve->GetN()-1,xstop,y);

  std::cout << "xstart = " << xstart << std::endl;
  std::cout << "xstop  = " << xstop << std::endl;
 
  int nbin = 0;
  double chisq = 0;
  for(int i = 0; i < np; i++){

 	// Retrieve histogram contents
	hist->GetPoint(i,x,y);
    	// Check if point is in range of curve
   	if (x < xstart || x > xstop) continue ;
	// Chece if point is in blinded region
	if (x > blind_lowEdge && x < blind_highEdge) continue;

	eyl = hist->GetEYlow()[i] ;
	eyh = hist->GetEYhigh()[i] ;
	exl = hist->GetEXlow()[i] ;
	exh = hist->GetEXhigh()[i] ;

        //std::cout << "x at bin " << i << " : " << x << " +/- " << exh << std::endl;
	// Integrate function over this bin
	double avg = curve->average(x-exl,x+exh);

	// Add pull^2 to chisq
	if (y!=0) {      
		double pull = (y>avg) ? ((y-avg)/eyl) : ((y-avg)/eyh) ;
		std::cout << "chi^2 at bin " << i << " : " << pull*pull << std::endl;
		chisq += pull*pull ;
		nbin++;
	}
  }
  std::cout << "chisq = " << chisq << std::endl;
  std::cout << "ndof  = " << nbin-nFitParam << std::endl;

  return chisq / (nbin-nFitParam);
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
   "novopsprod",
   "novoeffprod",
   "novopshighMpol4",
   "crystalball",
   "crystalpsprod",
   "crystaleffprod",
   "cbeffprod",
   "expeffprod",
   "doublecb",
   "dijetv1",
   "dijetv1psprod",
   "dijetv2",
   "dijetv2psprod",
   "expgausexp",
   "gausexp",
   "gausexppsprod",
   "expbwexp",
   "bukin",
   "bukinpsprod",
   "bernstein",
   "chebychev",
   "berneffprod",
   "bernpsprod",
   "chebeffprod"};

const int FitContainer::defaultNumberOfCoefficients_ = 4;

const std::string FitContainer::defaultOutputDir_ =
  std::string(gSystem->Getenv("CMSSW_BASE"))+"/src/Analysis/BackgroundModel/test/";
