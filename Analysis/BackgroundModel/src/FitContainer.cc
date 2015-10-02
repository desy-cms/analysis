#include <iostream>
#include <boost/assign/list_of.hpp>
#include "TSystem.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"


namespace bass = boost::assign;

using namespace analysis::backgroundmodel;


FitContainer::FitContainer(TH1* data, TH1* signal, TH1* background)
  : mbb_("mbb", "m_{bb}", data->GetXaxis()->GetXmin(), data->GetXaxis()->GetXmax()),
    data_("data_container", "", mbb_, RooFit::Import(*data)),
    signal_("signal_container", "", mbb_, RooFit::Import(*signal)),
    background_("background_container", "", mbb_, RooFit::Import(*background)) {

  // some preliminary test code
  RooPlot* frame = mbb_.frame();
  data_.plotOn(frame);
  TCanvas canvas("canvas", "", 600,600);
  canvas.cd();
  frame->Draw();
  std::string path(gSystem->Getenv("CMSSW_BASE"));
  path += "/src/Analysis/BackgroundModel/test/plots/";
  gSystem->mkdir(path.c_str(), true);
  canvas.SaveAs((path+"input_data.pdf").c_str());
}


FitContainer::~FitContainer() {
}
