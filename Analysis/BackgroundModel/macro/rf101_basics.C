#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
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
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooList.h"
#include "TAxis.h"

using namespace RooFit ;


void rf101_basics()
{
  // S e t u p   m o d e l 
  // ---------------------

  // Declare variables mbb,peak,width and tail with associated name, title, initial value and allowed range
  RooRealVar mbb("mbb","mbb", 230.0, 1750.0) ;
  RooRealVar peak("peak", "peak", 246.82, 50.0, 500.0, "GeV");
  RooRealVar width("width", "width", 63.21, 5.0, 875.0, "GeV");
  RooRealVar tail("tail", "tail", -0.6245, -1.0, 1.0);

  // Build PS*Novosibirsk p.d.f in terms of mbb,peak,width and tail
  RooNovosibirsk novo("novosibirsk","novosibirsk PDF", mbb, peak, width, tail);
  RooFormulaVar phasespace("phasespace", "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)", RooArgList(mbb));

  RooEffProd novopsprod("novopsprod","novopsprod PDF", novo, phasespace);

  // Construct plot frame in 'x'
  RooPlot* xframe = mbb.frame(Title("Phasespace x Novosibirsk p.d.f.")) ;


  // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
  // ---------------------------------------------------------------------------

  // Plot novopsprod in frame (i.e. in mbb) 
  novopsprod.plotOn(xframe) ;

  // Change the value of sigma to 3
  //sigma.setVal(3) ;

  // Plot gauss in frame (i.e. in x) and draw frame on canvas
  //gauss.plotOn(xframe,LineColor(kRed)) ;
  

  // G e n e r a t e   e v e n t s 
  // -----------------------------

  // Generate a dataset of 9070 events in mbb from novopsprod PDF
  RooDataSet* data = novopsprod.generate(mbb,9070) ;  
  //RooDataHist* data = novopsprod.generateBinned(mbb,9070) ;
  TH1* h1 = data->createHistogram("QCD", mbb, Binning(76,230.,1750.));
  h1->SetName("QCD");
  h1->Draw();
  
  // Make a second plot frame in x and draw both the 
  // data and the p.d.f in the frame
  RooPlot* xframe2 = mbb.frame(Title("PS x Novosibirsk p.d.f. with data")) ;
  data->plotOn(xframe2) ;
  novopsprod.plotOn(xframe2) ;
  

  // F i t   m o d e l   t o   d a t a
  // -----------------------------

  // Fit pdf to data
  novopsprod.fitTo(*data) ;

  // Print values of mean and sigma (that now reflect fitted values and errors)
  peak.Print() ;
  width.Print() ;
  tail.Print() ;

  // Draw all frames on a canvas
  TCanvas* c = new TCanvas("rf101_basics","rf101_basics",800,400) ;
  c->Divide(2) ;
  c->cd(1) ; gPad->SetLeftMargin(0.15) ; xframe->GetYaxis()->SetTitleOffset(1.6) ; xframe->Draw() ;
  c->cd(2) ; gPad->SetLeftMargin(0.15) ; xframe2->GetYaxis()->SetTitleOffset(1.6) ; xframe2->Draw() ;
  
 
}

