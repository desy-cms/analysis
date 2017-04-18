#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <memory>
#include <fstream>
#include <ostream>
#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TArray.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
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
#include "RooAddPdf.h"

using namespace std;
using namespace RooFit;

void QCD_HistSyst(bool up, int iPar, double init_3[3], double para_a_array[3], double para_b_array[3], double dpara_b_array[3], TMatrixD para_b, TMatrixD eigenVa_inverse, TFile* &outputFile)
{
  	TString outname; 
  	TMatrixD para_a_sys(3,1,init_3); // final parameters used for systematics
  	TMatrixD para_b_sys(3,1,init_3);

	para_b.GetMatrix2Array(para_b_array);

	if(up)	para_b_array[iPar]  += abs(dpara_b_array[iPar]);
	else 	para_b_array[iPar]  -= abs(dpara_b_array[iPar]);

	para_b_sys.SetMatrixArray(para_b_array);
	para_b_sys.Print("v");
	para_a_sys.Mult(eigenVa_inverse,para_b_sys);
 	para_a_sys.Print("v");
 	para_a_sys.GetMatrix2Array(para_a_array);

	switch(iPar)
	{
	case 0: if(up) outname = "PAR0_13TeVUp"; else outname = "PAR0_13TeVDown"; break;
	case 1: if(up) outname = "PAR1_13TeVUp"; else outname = "PAR1_13TeVDown"; break;
	case 2: if(up) outname = "PAR2_13TeVUp"; else outname = "PAR2_13TeVDown"; break;
	}

	RooRealVar mbb("mbb","mbb", 230.0, 1750.0) ;
  	RooRealVar peak("peak", "peak", para_a_array[0], 50.0, 500.0, "GeV");
  	RooRealVar tail("tail", "tail", para_a_array[1], -1.0, 1.0);
  	RooRealVar width("width", "width", para_a_array[2], 5.0, 875.0, "GeV");

 	// Build PS*Novosibirsk p.d.f in terms of mbb,peak,width and tail

  	RooNovosibirsk novo("novosibirsk","novosibirsk PDF", mbb, peak, width, tail);
  	RooFormulaVar phasespace("phasespace", "(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * mbb)))) - (1.326-1.)", RooArgList(mbb));
	RooEffProd novopsprod("novopsprod","novopsprod PDF", novo, phasespace);

	RooDataSet* data = novopsprod.generate(mbb,9070) ;

	TH1* h1 = data->createHistogram("QCD", mbb, Binning(76,230.,1750.));
	h1->SetDrawOption("hist");
	h1->SetName("QCD_Mbb_CMS_"+outname);

  	// F i t   m o d e l   t o   d a t a ,   p l o t   m o d e l 
  	// ---------------------------------------------------------
  	// Make a second plot frame in x and draw both the 
  	// data and the p.d.f in the frame
  	RooPlot* xframe = mbb.frame(Title("PS x Novosibirsk p.d.f. with data")) ;
  	data->plotOn(xframe,RooFit::Name("data_curve"));
  	novopsprod.plotOn(xframe,RooFit::Name("fit_curve"));


  	// F i t   m o d e l   t o   d a t a
  	// -----------------------------
  	novopsprod.fitTo(*data) ;
  
  	double chi2 = xframe->chiSquare("fit_curve", "data_curve", 3);
  	std::cout << "Chi^2 = " << chi2 << std::endl;

  	// Draw all frames on a canvas
  	TCanvas* c = new TCanvas("rf503_basics","rf503_basics",800,400) ;
  	gPad->SetLeftMargin(0.15); xframe->GetYaxis()->SetTitleOffset(1.6); xframe->Draw();

	outputFile->cd();
 	h1->Write();

}

int main(int argc, char* argv[])
{
  // R e a d   w o r k s p a c e   f r o m   f i l e
  // -----------------------------------------------

  // Open input file with workspace
  TFile *f = new TFile("FitContainer_workspace.root") ;

  // Retrieve workspace from file
  RooWorkspace* w = (RooWorkspace*) f->Get("workspace") ;

  // R e t r i e v e   p d f ,   d a t a   f r o m   w o r k s p a c e
  // -----------------------------------------------------------------

  // Retrieve x,model and data from workspace
  RooRealVar* mbb = w->var("mbb") ;
  RooRealVar* peak = w->var("peak");
  RooRealVar* tail = w->var("tail");
  RooRealVar* width = w->var("width");
  RooAbsPdf* novopsprod = w->pdf("background") ;
  //int nPars = novopsprod->getParameters().getSize();

  peak->Print();
  tail->Print();
  width->Print();

  RooDataSet* data = novopsprod->generate(*mbb, 9070);

  // Create Histogram from function
  TH1* h_central = data->createHistogram("QCD", *mbb, Binning(76,230.,1750.));
  h_central->SetName("QCD");
  h_central->Draw("hist");

  // Output file to store all QCD Templates
  TFile* outputFile = new TFile("QCD_Templates.root");
  outputFile->cd();
  h_central->Write();

  // Diagonalization of Correlation Matrix
  // -------------------------------------
  double peak_ = peak->getVal();
  double tail_ = tail->getVal();
  double width_ = width->getVal();

  double dpeak_ = peak->getError();
  double dtail_ = tail->getError();
  double dwidth_ = width->getError();

  double init_9[9] = {1.,1.,1.,1.,1.,1.,1.,1.,1.};
  double cm_elements[9] = {1.,-0.6704,-0.782,-0.6704,1.,0.9579,-0.782,0.9579,1.}; //hardcoded
  double eigen_elements[9] = {-0.5376,0.8235,-0.1813,0.5844,0.5189,0.6238,0.6078,0.2294,-0.7602}; //hardcoded

  double init_3[3] = {1.,1.,1.};
  double para_a_array[3] = {peak_,tail_,width_};
  double dpara_a_array[3] = {dpeak_,dtail_,dwidth_};
 
  // Matrix a //
  // -----------
  TMatrixD corMatrixa(3,3,cm_elements);
  TMatrixD eigenVa(3,3,eigen_elements);
  TMatrixD eigenVa_inverse(3,3,eigen_elements);
  eigenVa_inverse.Invert();
  std::cout << "\nEigenvectors of Correlation Matrix a " << std::endl;
  eigenVa.Print("v");
  std::cout << "\nInverse Eigenvectors of Correlation Matrix a " << std::endl;
  eigenVa_inverse.Print("v");
  //TMatrixD unit(3,3,elements);
  //unit.Mult(eigenVa,eigenVa_inverse);
  //unit.Print("v");
  TMatrixD para_a(3,1,para_a_array);
  TMatrixD dpara_a(3,1,dpara_a_array);
  std::cout << "\nParameters Matrix a " << std::endl;
  para_a.Print("v");
  std::cout << "\nError of Parameters (symmetric) Matrix a " << std::endl;
  dpara_a.Print("v");

  // Matrix b (diagonal of a) //
  // ---------------------------
  TMatrixD corMatrixb(3,3,init_9);
  TMatrixD eigenVb(3,3,init_9);
  eigenVb.Mult(eigenVa_inverse,corMatrixa);
  corMatrixb.Mult(eigenVb,eigenVa);
  std::cout << "\nCorrelation Matrix b (after diagonalization) " << std::endl;
  corMatrixb.Print("v");

  TMatrixD para_b(3,1,init_3);
  TMatrixD dpara_b(3,1,init_3);
  para_b.Mult(eigenVa,para_a);
  dpara_b.Mult(eigenVa,dpara_a);
  std::cout << "\nParameters Matrix b " << std::endl;
  para_b.Print("v");  
  std::cout << "\nError of Parameters (symmetric) Matrix b " << std::endl;
  dpara_b.Print("v");
 
  double para_b_array[3] = {1.,1.,1.};
  double dpara_b_array[3] = {1.,1.,1.};  

  // Get Matrix to Array for varying up and down
  // -------------------------------------------
  dpara_b.GetMatrix2Array(dpara_b_array);

  QCD_HistSyst(1,0,init_3,para_a_array,para_b_array,dpara_b_array,para_b,eigenVa_inverse,outputFile);

  outputFile->Close();
  cout << "End of code :)" << endl;


}






