#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <TMatrixDSym.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TStyle.h>
#include <TH1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TTree.h>
#include <TChain.h>
#include <TFileCollection.h>
#include <TGraph2D.h>
#include <TLatex.h>
#include <TMarker.h>
#include <math.h>
#include <TMinuit.h>
#include <TMath.h>
#include <TFileInfo.h>

class splineFitter;
splineFitter* gSplineFitter;  // pointer needs to be global, to be called in fcn

// polynomial degree is an overall constant
int theDegree = 3;
int theSplineNumber = 0;


void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);  // declaration is needed in splineFitter

class splineFitter {
  
public:
  TH1D* data;
  int fitLow;
  int fitHigh;
  double lambda;  // weight for penalty term
  double fitChi2;
  int fitNdf;
  int fitStat;
  TMinuit* theMinuit;
  double* nodes;
  int numberOfNodes;
  splineFitter(TH1D* theData,double theLambda=0) {
    data = theData;
    lambda= theLambda;
    fitLow = -1;
    fitHigh = -1;
    
    fitLow = 1;  // TH convention, count from 1...nbins
    fitHigh = theData->GetNbinsX();
    theMinuit = NULL;
    nodes = NULL;
    numberOfNodes = 0;
  }
  ~splineFitter() {
    std::cout << "SplineFitter destructor called " << std::endl;
    if (theMinuit != NULL) delete theMinuit;
    std::cout << "SplineFitter destructor done " << std::endl;
  }
  void SetRangeX(int theLow,int theHigh) {
    fitLow = theLow;
    fitHigh = theHigh;
  }
  void SetKnots(const double* theNodes) {
    numberOfNodes = 0;
    for (int iKnot=0; iKnot<999; ++iKnot) {
      if (theNodes[iKnot] == -1) break;
      if (theNodes[iKnot] <-10000) break;
      if (theNodes[iKnot] >10000) break;
      ++numberOfNodes;
    }
    std::cout << "The number of knots is: " << numberOfNodes << std::endl;
    nodes = new double[numberOfNodes];
    for (int iKnot=0; iKnot<numberOfNodes; ++iKnot) {
      nodes[iKnot] = theNodes[iKnot];
//       std::cout << "Knot is: " << nodes[iKnot] << std::endl;
    }
    
  }
  double* GetKnots() {  return nodes; };
  TH1D* GetPlot() {
    return GetData();
  }
  int GetNKnots() { return numberOfNodes; };
  TH1D* GetData() {
    return data;
  }
  double GetMeanBinContent() {
    double M = data->GetSum() / data->GetNbinsX();
    return M;
  }
  void GetResult(int iPar, double& theValue, double& theError){
    // parameter numbering from zero
    double value;
    double error;
    int getpar = theMinuit->GetParameter(iPar,value,error);
    theValue = value;
    theError = error;
  };
  Double_t GetParameter(int iPar){
    // parameter numbering from zero
    double value;
    double error;
    int getpar = theMinuit->GetParameter(iPar,value,error);
    return value;
  };
  double GetLambda() {
    return lambda;
  };
  double GetChisquare() {
    return fitChi2;
  };
  int GetNDF() {
    return fitNdf;
  };
  double GetProb() { 
    return TMath::Prob(fitChi2,fitNdf);
  };

  TMatrixDSym & GetCovMatrix() {
    Int_t npars = theMinuit->GetNumPars();
    TMatrixDSym* _corrMatrix = new TMatrixDSym(npars);
    cout<<"External correlation Matrix from TMinuit"<<"\n";
    theMinuit->mnemat(_corrMatrix->GetMatrixArray(),npars );
    for (int i=0;i<npars;i++){
      for (int j=0;j<npars;j++) {
        std::cout<<"a"<<i<<j<<"="<<(*_corrMatrix)(i,j)<<"   ";
      }
      std::cout<< std::endl;
    }
    return *_corrMatrix;
  }

  int Fit(){
    int nSpline = numberOfNodes-1-theDegree;
    int nFitPar = nSpline;
    theMinuit = new TMinuit(nFitPar);
    theMinuit->SetFCN(fcn);
    Double_t arglist[10];
    arglist[0] = 1;
    Int_t ierflg = 0;
    // set error definition to 1 (correct for chi2 fits)
    theMinuit->mnexcm("SET ERR",arglist,1,ierflg);
    Double_t* vstart = new Double_t[nFitPar];
    Double_t* vstep = new Double_t[nFitPar];
    for (int i=0; i<nFitPar; ++i) {
      vstart[i] = 1 / double( nFitPar );
      vstep[i] = vstart[i] / 100;
      theMinuit->mnparm(i,Form("par%d",i),vstart[i],vstep[i],0.,0.,ierflg);
    }

//     // tell minuit to use gradient
//     arglist[0] = 1;
//     arglist[1] = 1;
//     theMinuit->mnexcm("SET GRA",arglist,2,ierflg);
#ifdef INIHESSE
    arglist[0] = 0;
    arglist[1] = 0;
    theMinuit->mnexcm("HESSE",arglist,0,ierflg);
#endif
    arglist[0] = 50000;
    arglist[1] = .1;
    theMinuit->mnexcm("MIGRAD",arglist,2,ierflg);
    // get statistics from minuit
    Double_t fmin; 
    Double_t fedm; 
    Double_t errdef; 
    Int_t npari; 
    Int_t nparx; 
    Int_t istat;
    theMinuit->mnstat(fmin,fedm,errdef,npari,nparx,istat);
    fitChi2 = fmin;
    fitNdf = (gSplineFitter->fitHigh - gSplineFitter->fitLow + 1) - npari;
    fitStat = istat;
    std::cout << " fmin=" << fmin << " fedm=" << fedm << " errdef=" << errdef
              << " npari=" << npari << " nparx=" << nparx << " istat=" << istat << std::endl;
    std::cout << " ndf=" << fitNdf << std::endl;

//     arglist[0] = 0;
//     arglist[1] = 0;
//     theMinuit->mnexcm("HESSE",arglist,0,ierflg);


    return fitStat;
  };
};


Double_t mySpline(Double_t x,int degree,int i) {
  double* xKnots = gSplineFitter->GetKnots();
  int nKnots = gSplineFitter->GetNKnots();
  if (i < 0 || i>=(nKnots-1)) {
    std::cout << "mySpline: bad spline number " << i << endl;
    return -1;
  }
  if (degree == 0) {
    if (x >= xKnots[i] && x < xKnots[i+1]) {
//       std::cout << "mySpline called for x=" << x << " Degree " << degree << "  piece " << i
//                 << "  return 1" << std::endl;
      return 1;
    } else {
//       std::cout << "mySpline called for x=" << x << " Degree " << degree << "  piece " << i
//                 << "  return 0" << std::endl;
      return 0;
    }
  } else {
    // recursion
    int p = degree;
    Double_t Spl1 = mySpline(x,p-1,i);
    Double_t Spl2 = mySpline(x,p-1,i+1);
    Double_t fac1 = (x - xKnots[i]) / (xKnots[i+p] - xKnots[i]);
    Double_t fac2 = (xKnots[i+p+1] - x) / (xKnots[i+p+1] - xKnots[i+1]);
//     std::cout << "Spl1 =" << Spl1 << "  Spl2 =" << Spl2
//               << "  fac1 =" << fac1 << "  fac2 =" << fac2 << std::endl;
    Double_t Nip = fac1 * Spl1 + fac2 * Spl2;
//     std::cout << "mySpline called for x=" << x << " Degree " << degree << "  piece " << i
//               << "  return " << Nip << std::endl;
    return Nip;
  }
  return 0;
}
  

Double_t fitf(Double_t *v, Double_t *par) {

  // the fitting function is a linear combination of b splines
  int nSpline = gSplineFitter->GetNKnots()-1-theDegree;
  Double_t theFunc = 0;
  for (int iSpline=0; iSpline<nSpline; ++iSpline) {
    theFunc += par[iSpline] * mySpline(v[0],theDegree,iSpline);
  }
  return theFunc;
}




void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag) {    
  // this is the FCN defining the objective function
  double chisq = 0;
  double M = gSplineFitter->GetMeanBinContent();
  Double_t v[1];
  // loop over the histogram bins
  for (int binx=gSplineFitter->fitLow; binx<=gSplineFitter->fitHigh; ++binx) {
    double errData = gSplineFitter->GetData()->GetBinError(binx);
    if (errData > 0) {
      v[0] = gSplineFitter->GetData()->GetBinCenter(binx);
      double delta = gSplineFitter->GetData()->GetBinContent(binx) - fitf(v,par);

      chisq += (delta*delta) / (errData * errData);
    }
  }
  // here we should add the penalty term
  double penalty = 0;
  int nSpline = gSplineFitter->GetNKnots()-1-theDegree;
  for (int iSpline=2; iSpline<nSpline; ++iSpline) {
    double Dk = par[iSpline] - 2*par[iSpline-1] + par[iSpline-2];
    penalty += Dk * Dk / (M * M);
  }
  chisq += gSplineFitter->GetLambda() * penalty;

  f = chisq;

  if (iflag == 2) {
    // compute derivative
    std::cout << "Derivative currently not operational" << std::endl;
  }
}













void customFitSpline() {

  bool writeOut = true;
  bool range1TeV = false;

  // create new canvas
  TCanvas* tabcan = new TCanvas ("cg1","mycanvas",10,10,800,600);
  gStyle->SetPadColor(0);
  tabcan->SetFillColor(0);


  string flav[5] = {"b","c","l","bb","cc"};
  gStyle->SetPadColor(0);
  // open Root file
//   TFile*f = TFile::Open("/afs/desy.de/group/cms/mssmhbb/CommonFiles/OfflineBtagEff/v127/btagefficiencies_v127_mediummass.root");
  TFile*f = TFile::Open("final_extended.root");

  // pointers to Root files

  // just for the stupidity of CINT which cannot handle const array sizes

  const int nfl = 5;
  const int neta = 5;

  int nbinsPt = 40;
  double maxPt = 2000.;
  if (range1TeV) {
    nbinsPt = 50;
    maxPt = 1000.;
  }

  TH2D** hEff = new TH2D*[nfl];
  TH1D*** hEffPt = new TH1D**[nfl];
  for (int ifl=0; ifl<nfl; ++ifl) {
    hEffPt[ifl] = new TH1D*[neta];
  }
  //TH1D* hEffPt[nfl][neta];

  // loop over flavors
  for (int ifl=0; ifl<5; ++ifl) {
    hEff[ifl] = (TH2D*) f->Get(Form("h_%sjet_eff_pt_eta",flav[ifl].c_str()));
    //hEff[ifl]->Draw("LEGO2");
    //gPad->Print(Form("pteta_CSVT_%s_eff.png",flav[ifl].c_str()));

    // loop over eta bins
    for (int ieta=0; ieta<neta; ++ieta) {
      hEffPt[ifl][ieta] = hEff[ifl]->ProjectionX(Form("h_%sjet_eff_etabin%d",flav[ifl].c_str(),ieta),ieta+1,ieta+1,"");
      hEffPt[ifl][ieta]->SetMarkerStyle(20);
      hEffPt[ifl][ieta]->SetStats(0);
      hEffPt[ifl][ieta]->GetXaxis()->SetRangeUser(0,800);
      hEffPt[ifl][ieta]->GetXaxis()->SetTitleOffset(1.1);
      hEffPt[ifl][ieta]->GetYaxis()->SetTitle("#epsilon_{btag}");
      hEffPt[ifl][ieta]->GetYaxis()->SetTitleOffset(0.9);
      //std::cout << "GetTitleSize=" << hEffPt[ifl][ieta]->GetYaxis()->GetTitleSize() << std::endl;
      hEffPt[ifl][ieta]->GetYaxis()->SetTitleSize(0.05);
      //hEffPt[ifl][ieta]->Draw("EP1");
      //gPad->Print(Form("pt_CSVT_%s_eff_etabin%d.png",flav[ifl].c_str(),ieta));
    }
  }

  //  f->Close();

#ifdef DODRAW
  // just do drawing
  TF1 *pfunc = new TF1("pfunc",fitf,0,1000,3);
  pfunc->SetParameter(0,0.6);
  pfunc->SetParName(0,"Constant");
  
  for (int iSplineNumber=0; iSplineNumber<(gSplineFitter->GetNKnots()-1-theDegree); ++iSplineNumber) {
    theSplineNumber = iSplineNumber;
    pfunc->SetParameter(1,theDegree);
    pfunc->SetParName(1,"Degree");
    pfunc->SetParameter(2,iSplineNumber);
    pfunc->SetParName(2,"SplineNumber");
    pfunc->SetLineColor(iSplineNumber+2);
    if (iSplineNumber>=3) pfunc->SetLineColor(iSplineNumber+3);
    //pfunc->Draw("");
    //gPad->Print(Form("pfunc%d.png",iSplineNumber));
                
    
    if (iSplineNumber == 0) {
      pfunc->DrawCopy("");
    } else {
      pfunc->DrawCopy("SAME");
    }
  }
  // pfunc->Draw();
  gPad->Print("pfunc.png");
#endif

  // create 2D histograms for later plotting
   const static int nptbins = 41;
   float ptbins[nptbins+1] = {0,5,10,20,40,60,80,100,120,140,160,180,200,220,240,280,320,360,400,430,460,500,520,560,620,680,700,750,850,960,1000,1300,1500,1600,1800,2000,2400,2800,3000,3300,3500,4000};
   const static int netabins = 5;
   float etabins[netabins+1] = {0.,0.5,1.0,1.5,2.2,2.4};
  TH2D** hEffSm = new TH2D*[nfl];
  for (int ifl=0; ifl<nfl; ++ifl) {
    hEffSm[ifl] = new TH2D( Form("EffSm_%s",flav[ifl].c_str()),
                            Form("%s",flav[ifl].c_str()),
                            nptbins, ptbins,netabins,etabins);
    hEffSm[ifl]->GetXaxis()->SetTitle("p_{T} [GeV]");
    hEffSm[ifl]->GetXaxis()->SetRangeUser(0,800);
    hEffSm[ifl]->GetYaxis()->SetTitle("#eta");
    hEffSm[ifl]->GetZaxis()->SetTitle("#epsilon_{btag}");
    hEffSm[ifl]->SetStats(0);
    hEffSm[ifl]->GetXaxis()->SetTitleOffset(2);
    hEffSm[ifl]->GetYaxis()->SetTitleOffset(2);
    hEffSm[ifl]->GetZaxis()->SetTitleOffset(1.4);
    //    hEffSm[ifl]->SetLeftMargin(3);
    hEffSm[ifl]->SetStats(0);
  }

  for (int ifl=0; ifl<nfl; ++ifl) {
    for (int ieta=0; ieta<neta; ++ieta) {

      // now try the fitting
      TH1D* hEffPtFit = new TH1D( *hEffPt[ifl][ieta] );
      //TH1D* hEffPtFit = hEffPt[ifl][ieta]->Clone( Form("%s_Fit",hEffPt[ifl][ieta]->GetName()) );

      hEffPtFit->SetName( Form("%s_Fit",hEffPt[ifl][ieta]->GetName()) );
      hEffPtFit->SetTitle( Form("%s_Fit",hEffPt[ifl][ieta]->GetTitle()) );
      
      // set up Minuit fitter
      if (flav[ifl] == "c" ) {
        gSplineFitter = new splineFitter(hEffPtFit,20);
        double k2[] = {0, 10,20,40, 60, 80, 100,120,140, 160,200, 220,260,300, 350,600,800,1100,1500,1900, 2800,3600, -1};
        gSplineFitter->SetKnots( k2 );
      } else if (flav[ifl] == "l") {
        gSplineFitter = new splineFitter(hEffPtFit,10);
        double k3[] = {0, 10, 20, 40, 60, 80, 110, 130, 200, 300, 400, 600,950,1200,1600, 2000,2600,3000, 4000, -1};
        gSplineFitter->SetKnots( k3 );
      } else if (flav[ifl] == "cc") {
        gSplineFitter = new splineFitter(hEffPtFit,10);
        double k4[] = {0, 10, 20, 40, 60, 80, 100,120, 150,180, 200, 300, 400, 600,950,1200,1600, 2000,2600,3000, 4000, -1};
        gSplineFitter->SetKnots( k4 );
      }
        else {
        gSplineFitter = new splineFitter(hEffPtFit,10);
        double k1[] = {0, 20, 40, 60, 80, 110,180, 200,240, 320, 400,600,1200,1900,2400, 2800,3400, 4000, -1};
        gSplineFitter->SetKnots( k1 );
      }

      Int_t status = gSplineFitter->Fit(); 
      cout << " fit status: " << status << endl;

      int nSpline = gSplineFitter->GetNKnots()-1-theDegree;

      // TFitResultPtr r = hEffPtFit->Fit("fit","S");

      // now determine the smoothed errors
      TH1D* hEffPtErr = new TH1D( *hEffPt[ifl][ieta] );
      hEffPtErr->SetName( Form("%s_Errors",hEffPt[ifl][ieta]->GetName()) );
      //hEffPtErr->SetTitle( Form("%s_Errors",hEffPt[ifl][ieta]->GetTitle()) );
      float etamin = ieta*0.5;
      float etamax = (ieta+1)*0.5;

      if( ieta == 3)
      etamax = 2.2;

      else if ( ieta == 4)
      { 
        etamin = 2.2;
        etamax = 2.4;
      }


      hEffPtErr->SetTitle(Form("Offline btag #epsilon, jet flavor %s, %3.1f #leq #eta #leq %3.1f",flav[ifl].c_str(),etamin,etamax));


      // get covariance matrix of the fit parameters
      TMatrixDSym& cov = gSplineFitter->GetCovMatrix();  //  to access the covariance matrix

      // get the fitted parameters
      Double_t* par = new Double_t[nSpline];
      for (int ipar=0; ipar<nSpline; ++ipar) {
        par[ipar] = gSplineFitter->GetParameter(ipar);
      }

      // prepare a dummy vector of parameters
      Double_t* upar = new Double_t[nSpline];

      // prepare a vector for the derivatives
      Double_t* deriv = new Double_t[nSpline];

      // loop over the bins
      for (int ibin=0; ibin < hEffPtErr->GetNbinsX(); ++ibin) {
        Double_t v[1];
        v[0] = hEffPtErr->GetXaxis()->GetBinCenter(ibin+1);
        Double_t fval = fitf( v, par );
        if (fval<0) fval = 0;      // do not allow negative efficiencies
        hEffPtErr->SetBinContent( ibin+1, fval );

        // compute the derivatives
        for (int ipar=0; ipar<nSpline; ++ipar) {
          // set dummy vector of unit parameters
          for (int jpar=0; jpar<nSpline; ++jpar) {
            upar[jpar] = (jpar == ipar)? 1 : 0;
          }
          deriv[ipar] = fitf( v, upar );
        }

        // perform error propagation
        Double_t eSqr = 0;
        for (int ipar=0; ipar<nSpline; ++ipar) {
          for (int jpar=0; jpar<nSpline; ++jpar) {
            eSqr += deriv[ipar] * deriv[jpar] * cov(ipar,jpar);
          }
        }

        Double_t err = 0;
        if (eSqr >0) err = sqrt( eSqr);
        hEffPtErr->SetBinError( ibin+1, err );

      }

      // create function for drawing
      TF1 *func = new TF1("fit",fitf,40,1000,nSpline);
      for (int ipar=0; ipar<nSpline; ++ipar) {
        func->SetParameter(ipar,par[ipar]);
        func->SetParName(ipar,Form("Spline %d",ipar));
      }

      hEffPtErr->SetFillColor(kCyan);
      hEffPtErr->SetMarkerStyle(0);
      hEffPtErr->GetXaxis()->SetRangeUser(0,800);
      hEffPtErr->Draw("E3");
      hEffPt[ifl][ieta]->Draw("EP1,SAME");  // to avoid drawing of function
      gPad->Print(Form("Splsmooth_Degree_%d_pt_CSVIVF_%s_etabin%d.png",theDegree,flav[ifl].c_str(),ieta));
      gPad->Print(Form("Splsmooth_Degree_%d_pt_CSVIVF_%s_etabin%d.pdf",theDegree,flav[ifl].c_str(),ieta));

      hEffPtErr->Draw("E3");
      hEffPtFit->Draw("EP1,SAME");  // this drawing with function
      func->Draw("SAME");
      gPad->Print(Form("Splfit_Degree_%d_pt_CSVIVF_%s_etabin%d.png",theDegree,flav[ifl].c_str(),ieta));
      gPad->Print(Form("Splfit_Degree_%d_pt_CSVIVF_%s_etabin%d.pdf",theDegree,flav[ifl].c_str(),ieta));

      // fill the eta column in the 2D smoothed efficiency histogram
      for (int ibin=0; ibin < hEffSm[ifl]->GetNbinsX(); ++ibin) {
        hEffSm[ifl]->SetBinContent(ibin+1,ieta+1, func->Eval(hEffSm[ifl]->GetXaxis()->GetBinCenter(ibin+1),0,0));
        hEffSm[ifl]->SetBinError(ibin+1,ieta+1, hEffPtErr->GetBinError( ibin+1 ));
      }
    }
    // draw the 2D histogram
    hEffSm[ifl]->Draw("LEGO2");
    gPad->Print(Form("Smooth2D_Lego_%s.png",flav[ifl].c_str()));
    gPad->Print(Form("Smooth2D_Lego_%s.pdf",flav[ifl].c_str()));
    hEffSm[ifl]->Draw("SURF1");
    gPad->Print(Form("Smooth2D_Surf_%s.png",flav[ifl].c_str()));
    gPad->Print(Form("Smooth2D_Surf_%s.pdf",flav[ifl].c_str()));

    // make interpolated 2D histogram
    TH2D* hEffInt = new TH2D( Form("EffInt_%s",flav[ifl].c_str()),
                            Form("%s",flav[ifl].c_str()),
                            nptbins,ptbins,netabins,etabins);
    hEffInt->GetXaxis()->SetTitle("p_{T} [GeV]");
    hEffInt->GetYaxis()->SetTitle("#eta");
    hEffInt->GetZaxis()->SetTitle("#epsilon_{btag}");
    hEffInt->SetStats(0);
    hEffInt->GetXaxis()->SetTitleOffset(2);
    hEffInt->GetYaxis()->SetTitleOffset(2);
    hEffInt->GetZaxis()->SetTitleOffset(1.4);
    for (int ibinpt=0; ibinpt < hEffInt->GetNbinsX(); ++ibinpt) {
      for (int ibineta=0; ibineta < hEffInt->GetNbinsY(); ++ibineta) {
        double thePt = hEffInt->GetXaxis()->GetBinCenter(ibinpt+1);
        double theEta = hEffInt->GetYaxis()->GetBinCenter(ibineta+1);
        double theInter = hEffSm[ifl]->Interpolate(thePt,theEta);
        hEffInt->SetBinContent(ibinpt+1,ibineta+1,theInter);
      }
    }
    double oldPhi = gPad->GetPhi();
    double oldTheta = gPad->GetTheta();
    gPad->SetPhi(310);
    hEffInt->Draw("SURF4");
    gPad->Print(Form("Inter2D_Surf_%s.png",flav[ifl].c_str()));
    gPad->Print(Form("Inter2D_Surf_%s.pdf",flav[ifl].c_str()));
    gPad->SetPhi(oldPhi);
  }

  // write out the smoothed btag efficiencies
  //f->Close();
  if (writeOut) {
    TFile*fout = new TFile("final_extended_smoothed.root","RECREATE");
    for (int ifl=0; ifl<nfl; ++ifl) {
      string theOldName( hEff[ifl]->GetName() );
      hEff[ifl]->Delete();
      hEffSm[ifl]->SetName( theOldName.c_str() );
      hEffSm[ifl]->Write();
      TH2D *h1 = (TH2D*) f->Get(Form("h_%sjet_pt_eta",flav[ifl].c_str()));
      TH2D *h2 = (TH2D*) f->Get(Form("h_%sjet_pt_eta_wp",flav[ifl].c_str()));

    h1->Write();
    h2->Write();
    
    }
    fout->Close();
    f->Close();
  }
}
