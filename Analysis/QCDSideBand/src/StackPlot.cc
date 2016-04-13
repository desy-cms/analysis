#include "Analysis/QCDSideBand/interface/StackPlot.h" 

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLegendEntry.h"
#include "TH1.h"
#include "TGraphErrors.h"
#include <TStyle.h>
#include <TROOT.h>

#include <iostream>
#include <sstream>
#include <algorithm>    // std::reverse

// Required by ROOT when when compiling an object to be used in a macro - please don't remove - used in scripts/plotting
// ClassImp(StackPlot)


//////////////////
// CONSTRUCTORS //
//////////////////
StackPlot::StackPlot() {
  std::cout << " No input or output file directory specified!" << std::endl;
  std::cout << " Please specify at least and input directory!" << std::endl;
  std::cout << " Exiting program." << std::endl;
  exit(EXIT_FAILURE);
}


StackPlot::StackPlot(std::string dir) {
  std::cout << " No output directory specified - will use input file directory as default" << std::endl;
  init(dir,dir);
}

StackPlot::StackPlot(std::string dir, std::string pdfDir) {
  init(dir,pdfDir);
}

// Common constructor block
void StackPlot::init(std::string dir, std::string pdfDir) {
  dir_ = dir;
  pdfDir_ = pdfDir;
  title_ = "";
  xTitle_ = "";
  yTitle_ = "";
  label_ = "";
  xMin_ = 0.;
  xMax_ = 0.;
  yMin_ = 0.;
  yMax_ = 0.;
  legX1_ = 0.63;
  legX2_ = 0.93;
  legY1_ = 0.63;
  legY2_ = 0.89;
  textX1_ = 0.;
  textX2_ = 0.;
  textY1_ = 0.;
  textY2_ = 0.;
  lumi_ = 2.3;
  rescaleMC_ = false;
  rescaleMCfactor_ = 1.0;
}

StackPlot::~StackPlot() {

}

//////////////////////////////////
// Set legend position manually //
//////////////////////////////////
void StackPlot::setLegPos(float x1, float y1, float x2, float y2) {
  legX1_ = x1;
  legY1_ = y1;
  legX2_ = x2;
  legY2_ = y2;
}

///////////////////////////////////////
// Set position of CMS text manually //
///////////////////////////////////////
void StackPlot::setTextPos(float x1, float y1, float x2, float y2){
  textX1_ = x1;
  textX2_ = x2;
  textY1_ = y1;
  textY2_ = y2;
}

/////////////////////////////////////////////////////////////////////
// Draw a histogram. Args:                                         
//  hname         = name of hist in ROOT files, used for output filename
//  xTitle        = title on x-axis                                
//  yTitle        = title on y-axis                                
//  logy          = draw with log Y axis                           
//  drawRatioPlot = draw with ratio plot below main plot, or not   
//                  "RATIO" draw ratio plot below,      
//                  "NONE" doesn't draw ratio plot, is short and wide,
//                  "SIG" doesn't draw ratio plot, but main plot is same size as in RATIO
//
// You can only draw once you have used addDataset to add datasets 
// 
// Rough layout of flow:
//  - Setup canvas, pads, etc
//  - Scan through all files for a given hist to determine 
//    min & max for y axis, and count for data/mc normalisation
//  - Scan through again, set colours, add to stack
//  - Draw the hist stack, and draw statistic errors
//  - Draw signal & data on top
//  - Draw CMS text, legend
//  - Draw ratio plot if requested
/////////////////////////////////////////////////////////////////////
void StackPlot::draw(std::string hname, std::string xTitle, std::string yTitle, bool logy, std::string ratioPlotOption) {

  std::cout << "Drawing " << hname << " " << std::endl;

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat("");
  gStyle->SetLegendBorderSize(0);

  TH1::SetDefaultSumw2();

  TCanvas canvas;

  bool drawRatioPlot; 
  bool drawSignal; 

  if (ratioPlotOption == "RATIO") {
    drawRatioPlot = true; // draw a ratio plot below
    drawSignal = false;
  } 
  else {
    drawRatioPlot = false;
    if (ratioPlotOption == "SIG")
      drawSignal = true; // signal plot
    else if (ratioPlotOption == "NONE")
      drawSignal = false; // in which case we must be dealing with N-1 plots
    else {
      std::cerr << "Please choose a valid ratio plot option in your plots.draw() call!" << std::endl;
      std::cerr << "Options are \"RATIO\", \"NONE\" or \"SIG\"" << std::endl;
      std::cerr << "Exiting without drawing" << std::endl;
      return;
    }
  }

  TPad *pad1; // This TPad is for the main plot. There's another one for the ratio plot
  if (drawRatioPlot) {
    canvas.SetCanvasSize(500, 600);
    pad1 = new TPad("pad1","",0,0.30,1,1);
    pad1->SetBottomMargin(0.02);
    pad1->SetRightMargin(0.05); // The ratio plot below inherits the right and left margins settings here!
    pad1->SetLeftMargin(0.16);
  } else {
    if (drawSignal){
      // How I calculated these size so identitcal to ratio plots, but without ratio bit:
      // Canvas size: 420+51-> 420 = 0.7*600, 51 = (0.33*0.3*600) [space required for x title and labels on ratio plot pad2]-(0.02*420)[space taken up by bottom of hist on pad1]
      // Pad bottom margin [space from bottom of hist to bottom of pad = bottom of pdf if pad goes down to y=0] = (0.33*0.3*600)/(420+51)
      // Right & left margins are the same as before
      canvas.SetCanvasSize(500, 420+51); 
      pad1 = new TPad("pad1","",0,0,1,1);
      pad1->SetBottomMargin(0.126); // = correct space for axis labels and title. 
      pad1->SetRightMargin(0.05); 
      pad1->SetLeftMargin(0.16);
    } else
      pad1 = new TPad("pad1","",0,0,1,1);
  }
  pad1->Draw();
  pad1->cd();
  if (logy) pad1->SetLogy();

  std::vector<TLegendEntry*> entries; // To hold legend entries, so can put in proper order later (otherwise it comes out upside down!)

  std::vector<TFile*>::iterator file = files_.begin();
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  // Some method of scanning all input files and determining the min y value needed for y axis
  // Uses the first BG file to determine minimum
  // Maximum is normally determined by the stack hist
  // BIT BROKEN - if you get a seg fault when drawing, you prob have no events in the first file and it gets angry
  // 
  // Also use this loop to get the sum of all BG MC contributions to allow rescaling to data
  // Note that this INCLUDES under & overflow bins. If you don't want those ones (ie, just what's shown on the plots)
  // then change  
  //     h->Integral(0,(h->GetNbinsX())+1)
  // to
  //      h->Integral(1,h->GetNbinsX()) 
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  int i = 0; // file counter
  double ymin=0;
  double xMin(0.),xMax(0.);
  double mcIntegral(0.),dataIntegral(0.); //for rescaling MC

  for (; file!=files_.end(); ++file, ++i) {

    // Do some checks to see if files & hists exist
    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }
    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());
    if (h==0) {
      std::cout << "No histogram " << hname << " in file " << labels_.at(i) << std::endl;
      continue;
    }

    if (styles_.at(i) == 0 || styles_.at(i) == 3) {
      if (h->GetEntries()==0) {
        std::cout << "No entries in histogram " << labels_.at(i) << std::endl;
        continue;
      }
    }

    // Get min of BG hist to determine y axis min
    if (styles_.at(i) == 0) {
      if (h->GetEntries() == 0) continue;
      double min = h->GetMinimum(0.); 
      
      if(fabs(h->GetMaximum(0)) == min ) continue; // Occasionally it does something odd and min = 3.4e38, max = -3.4e38. I guess it's and overflow thing. This stops it
      
      // Get integral including under & overflow, for renorm MC to data
      mcIntegral += h->Integral(0,(h->GetNbinsX())+1);

      // Skip the min/max finding if we've already found a decent one
      if (logy && (ymin > 0)) continue;
      else if (!logy && (ymin != 0)) continue;

      if (logy) {
        if( min > 0 ) ymin = min;
      } else 
        if (min != 0.) ymin = min;

      if (xMin_ != 0.)
        xMin = xMin_;
      else
        xMin = h->GetXaxis()->GetXmin();
      
      if (xMax_ != 0.)
        xMax = xMax_;
      else
        xMax = h->GetXaxis()->GetXmax();
    } else if (styles_.at(i) == 1){ // For data
      dataIntegral =   h->Integral(0,(h->GetNbinsX())+1);
    }
  } // end loop over files_

  if (rescaleMC_){
    rescaleMCfactor_ = dataIntegral/mcIntegral;
    std::cout << "data integral: " << dataIntegral << std::endl;
    std::cout << "mc integral: " << mcIntegral << std::endl;
    std::cout << "rescaleMCfactor_: " << rescaleMCfactor_ << std::endl;
  }

  /////////////////////////////////
  // Make the stacked histogram  //
  /////////////////////////////////
  THStack stack("hs", (hname+std::string(" stack")).c_str());

  bool drawStack=false;

  i=0;
  file = files_.begin();
  for (; file!=files_.end(); ++file, ++i) {
    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << " in file " << labels_.at(i) << std::endl;
      break;
    }

    //include only backgrounds and signal in stack
    if (styles_.at(i) == 0 || styles_.at(i) == 3) {
      if (h->GetEntries()==0) {
        std::cout << "No entries in histogram " << labels_.at(i) << std::endl;
        continue;
      }
      
      drawStack=true;
      h->SetLineColor(kBlack);
      h->SetLineWidth(1);
      h->SetFillColor(cols_.at(i));
      
      // For stack with filled color
      if(styles_.at(i) == 0)
        h->Scale(rescaleMCfactor_);

      // For stack with line color
      if(styles_.at(i) == 3) {
        h->SetLineStyle(1);
        h->SetLineWidth(3);
        h->SetLineColor(cols_.at(i));
        h->SetFillColor(0);
      } 
      h->SetAxisRange(xMin,xMax,"X");
      stack.Add(h);
      
      // Add entry to legend vector
      TLegendEntry *legE;
      if(styles_.at(i) == 3)
        legE = new TLegendEntry(h, labels_.at(i).c_str(), "L");
      else 
        legE = new TLegendEntry(h, labels_.at(i).c_str(), "F");
      entries.push_back(legE);
      
    }

  } // End of loop over files_ for TStack
  

  // Little hack here to get sum of hists in the THStack
  // Since apparently there isn't a method for that... (If there is please fix me!)
  // Used for ratio plot
  TList *histList = stack.GetHists();
  TIter next(histList); // Get list of hists in Stack
  TH1* hMC  = (TH1*) histList->First()->Clone();
  TObject *obj;
  while ((obj = next())) // Loop through all hists in stack
  {
    // skip first object since it's used by creating the histogram                               
    if(obj == histList->First()) continue;
    if(drawSignal && obj == histList->Last())  continue; //don't account signal histogram
    hMC->Add((TH1*)obj);
  }

  //////////////////////////////
  // Draw the histogram stack //
  //////////////////////////////
  if (drawStack) {
    stack.Draw("HIST"); // Draw first so can change axes

    // Some auto-axis ranging, doesn't work brilliantly, try tweaking the factors
    if (logy){
      stack.SetMaximum(stack.GetMaximum()*100);
      // stack.SetMinimum(ymin*0.1);
      stack.SetMinimum(ymin);
    } else {
      stack.SetMaximum(stack.GetMaximum()*1.25);
    }

    // BUT if user has set axis range, use that preferentially
    if (yMax_ != 0.) stack.SetMaximum(yMax_);
    if (yMin_ != 0.) stack.SetMinimum(yMin_);

    stack.GetXaxis()->SetRangeUser(xMin, xMax);

    stack.Draw("HIST"); //redraw to update axes 

    // Make axis labels nice
    // Change size depending on if ratio plot or not (ratio plot shrinks actual sizes, so need to account for this)
    if (drawRatioPlot){
      stack.GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
      stack.GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot

      stack.GetYaxis()->SetTitleSize(0.07);
      stack.GetYaxis()->SetTitleOffset(1.);
      stack.GetYaxis()->SetLabelSize(0.05);
    } else{
      if (drawSignal){
        // These are magic values. ie. found after some calculations/trial and error
        stack.GetXaxis()->SetTitleSize(0.057);
        stack.GetXaxis()->SetTitleOffset(0.9);
        stack.GetXaxis()->SetTitle(xTitle.c_str());
        stack.GetXaxis()->SetLabelSize(0.046);

        stack.GetYaxis()->SetTitleSize(0.061); 
        stack.GetYaxis()->SetTitleOffset(1.08);
        stack.GetYaxis()->SetLabelSize(0.05);
      } else {
        stack.GetXaxis()->SetTitleSize(0.045);
        stack.GetXaxis()->SetTitleOffset(0.9);
        stack.GetXaxis()->SetTitle(xTitle.c_str());

        stack.GetYaxis()->SetTitleSize(0.045);
        // stack.GetYaxis()->SetTitleOffset(1.2);    
      }
    }
    stack.GetYaxis()->SetTitle(yTitle.c_str());

  }

  ////////////////////////////////////
  // draw statistic errors on stack //
  ////////////////////////////////////

  TH1D* ErrComp = (TH1D *) hMC->Clone();
  ErrComp->SetMarkerStyle(0);
  ErrComp->SetMarkerSize(0);
  ErrComp->SetLineColor(kGray+3);
  ErrComp->SetLineWidth(0);
  ErrComp->SetFillColor(kGray+3);
  ErrComp->SetFillStyle(3013);
  ErrComp->Draw("E2same");

  ///////////////////////////////////////////////////
  // Draw non-stacked histograms (data, signal MC) //
  ///////////////////////////////////////////////////

  TH1D* hData = (TH1D*) malloc(sizeof(TH1D)); //to allocate a certain amount of memory for gcc compilation
  file = files_.begin();
  i=0;
  for (; file!=files_.end(); ++file, ++i) {

    if (*file==0) {
      std::cerr << "No file for " << labels_.at(i) << std::endl;
      continue;
    }

    TH1D* h = (TH1D*) (*file)->Get(hname.c_str());

    if (h==0) {
      std::cout << "No histogram " << hname << std::endl;
      break;
    }

    // data
    if (styles_.at(i) == 1) {

      h->SetMarkerStyle(8);
      h->SetLineColor(kBlack);
      h->SetMarkerColor(cols_.at(i));
      TLegendEntry *legE = new TLegendEntry(h, "Observed", "PL");
      entries.push_back(legE);
      h->Draw("PE SAME");
      h->SetAxisRange(xMin,xMax,"X");
      h->Draw("PE SAME");
      hData = (TH1D*)h->Clone("hData");
    }

    // signal MC (non-stacked)
    if (styles_.at(i) == 2) {
      h->SetLineStyle(1);
      h->SetLineWidth(3);
      h->SetLineColor(cols_.at(i));
      TLegendEntry *legE = new TLegendEntry(h, labels_.at(i).c_str(), "L");
      entries.push_back(legE);
      h->Draw("HISTE SAME");
    }

  }

  // draw the axes again over the top of block colours
  canvas.RedrawAxis();  

  // Draw line on plot in case of specifying our cut selection
  /*
  double cutVal = 0.;
  if (!hname.compare("hDijetNM1")) cutVal = 50.;
  else if (!hname.compare("hDEtaJJNM1")) cutVal = 4.2;
  else if (!hname.compare("hMjjNM1")) cutVal = 1100.;
  else if (!hname.compare("hMETNM1")) cutVal = 130.;
  else if (!hname.compare("hDPhiJJNM1") || !hname.compare("hDPhiJJSigNoRatio")) cutVal = 1.0;
  else if (!hname.compare("hCenEtNM1")) cutVal = 30.0;

  if (cutVal != 0) {
    TLine *cutLine = new TLine(cutVal,0,cutVal,stack.GetMaximum()*1.5);
    cutLine->SetLineColor(kBlack);
    cutLine->SetLineWidth(3);
    cutLine->SetLineStyle(7);
    cutLine->Draw();
  }
  */

  /////////////////////////
  // Draw text on plot   //
  /////////////////////////
  // First, CMS text in top left of plot
  TPaveText *cms;
  if(drawRatioPlot){
    // Optimised for ratio plots
    cms = new TPaveText(0.18, 0.60, 0.6, 0.89, "NDC");
  } else {
    // Optimised for non-ratio plots
    if (drawSignal)
      cms = new TPaveText(0.18, 0.60, 0.6, 0.89, "NDC");
    else
      cms = new TPaveText(0.12, 0.60, 0.45, 0.9, "NDC");
  }
  

  cms->SetFillColor(0);
  cms->SetFillStyle(4000);
  cms->SetBorderSize(0);
  cms->SetLineColor(0);
  cms->SetTextAlign(12);
  cms->AddText("CMS Preliminary");
  std::stringstream s; // convert float to string to make concatenation easier
  s << lumi_;
  cms->AddText(("#sqrt{s} = 13 TeV, L = "+s.str()+" fb^{-1}").c_str());
  //if(ratioPlotOption == "RATIO_Z") 	cms->AddText("VBF H(inv) Z(ll) enriched");
  //else 				cms->AddText("VBF H(inv)");
  cms->AddText(label_.c_str());   // any other text user has specified

  // This works. DON'T use SetX1NDC EVEN THOUGH it says NDC above.
  // This is because ROOT is silly.
  if (textX1_ != 0.) cms->SetX1(textX1_);
  if (textX2_ != 0.) cms->SetX2(textX2_);
  if (textY1_ != 0.) cms->SetY1(textY1_);
  if (textY2_ != 0.) cms->SetY2(textY2_);
  cms->Draw();

  //////////////////////////////////////////////
  // Draw legend, with entries in right order //
  //////////////////////////////////////////////
  TLegend leg(legX1_, legY1_, legX2_, legY2_);
  leg.SetFillColor(0);

  std::reverse(entries.begin(),entries.end());
  for (unsigned int n = 0;n < entries.size(); n++) {
    leg.AddEntry(entries.at(n)->GetObject(), entries.at(n)->GetLabel(), entries.at(n)->GetOption());
  }
  if (drawRatioPlot || drawSignal) leg.SetBorderSize(0);
  leg.Draw();

  canvas.cd();

  //////////////////////
  // Draw ratio plot  //
  //////////////////////

  if (drawRatioPlot){
    TPad *pad2 = new TPad("pad2","",0,0.0,1,0.30);
    pad2->SetTopMargin(1);
    pad2->SetBottomMargin(0.33);
    pad2->SetLeftMargin(pad1->GetLeftMargin());
    pad2->SetRightMargin(pad1->GetRightMargin());
    // pad2->SetTopMargin(0);

    // pad2->SetFillStyle(4000); // For transparent pad?
    // pad2->SetFillColor(0);
    // pad2->SetFrameFillStyle(4000);
    pad2->Draw();
    pad2->cd();
 
    hData->Divide(hMC);
    hData->SetMarkerStyle(8);

    // BE CAREFUL WHEN EDITING THESE VALUES
    // They *should* work to give nice sizes & offsets 
    // - ignore the fact that some of the Offsets are odd numbers like 0.011
    // It involved a lot of trial and error, so edit at your own risk...
    // ROOT does some magic to convert these to actual distances...

    hData->SetXTitle(xTitle.c_str());
    hData->SetYTitle("#frac{Data}{MC}");
    hData->GetXaxis()->SetTitleSize(0.15);
    hData->GetXaxis()->SetTitleOffset(0.85);
    hData->GetXaxis()->SetLabelSize(0.12);
    hData->GetXaxis()->SetLabelOffset(0.008);

    hData->GetYaxis()->CenterTitle(kTRUE);
    hData->GetYaxis()->SetTitleSize(0.12);
    hData->GetYaxis()->SetTitleOffset(0.5);
    hData->GetYaxis()->SetNdivisions(3,5,0);
    hData->GetYaxis()->SetLabelSize(0.12);
    hData->GetYaxis()->SetLabelOffset(0.011);

    // remove any points if no data ie data/mc = 0
    for(int nbin = 1; nbin<= hData->GetNbinsX(); nbin++) {
      if(hData->GetBinContent(nbin) == 0)
        hData->SetBinContent(nbin,1000);
    }

    hData->Draw("ep");
    hData->SetMaximum(2);
    hData->SetMinimum(0);

      // h->SetAxisRange(xMin,xMax,"X");
      // h->GetXaxis()->SetLimits(xMin, xMax);
    hData->Draw("ep");

    // Do MC errors on ratio plot
    // the bars it puts are of size = err_MC/MC,
    // ie the relative error on MC. 
    int NBins = hMC->GetNbinsX();
    TGraphErrors * Erreff = new TGraphErrors(NBins);
    for(int iBin = 1; iBin <= NBins; ++iBin) {   
      double x = hMC->GetBinCenter(iBin);
      double y_smSum = hMC->GetBinContent(iBin); 
      if(!(y_smSum > 0.)) continue;
      double relerr = hMC->GetBinError(iBin)/hMC->GetBinContent(iBin);
      Erreff->SetPoint(iBin-1, x, 1);
      Erreff->SetPointError(iBin-1, 0.5*hMC->GetBinWidth(iBin),relerr);
    }
    Erreff->SetMarkerStyle(0);
    Erreff->SetMarkerSize(0);
    Erreff->SetLineColor(kGray+3);
    Erreff->SetLineWidth(0);
    Erreff->SetFillColor(kGray+3);
    Erreff->SetFillStyle(3013);
    Erreff->Draw("2");

    hData->Draw("ep same");

    // Because ROOT is so stupid, if you use SetRangeUser or SetAxisRange, it doesn't affect the GetXmin or GetXmax
    // You could use SetLimits instead of SetRangeUser, which DOES update GetXmin, but DOESN'T set the x axis to plot between the limits 
    // - instead it literally plots the full range, and just renames the max and min to whatever you passed in SetLimits
    // And there's no documentation either!!!
    // So instead I have to manually find the bin edge based on the bin number that contains the user's value
    // EXCEPT
    // You use GetBinUpEdge if user sets it, or GetBinLowEdge if not.
    // AND
    // this all goes to hell if the user sets xMin_ or xMax_ to = bin edge value. Ack.
    // Occasionally you get a lien which goess off the edge of the ratio box, sorry.
    
    double lineMin = hData->GetXaxis()->GetBinLowEdge(hData->GetXaxis()->FindBin(xMin));
    double lineMax = hData->GetXaxis()->GetBinLowEdge(hData->GetXaxis()->FindBin(xMax));
    if (xMax_ != 0.)
      lineMax = hData->GetXaxis()->GetBinUpEdge(hData->GetXaxis()->FindBin(xMax));
    TLine *line = new TLine(lineMin,1,lineMax,1);
    line->SetLineColor(kBlack);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->Draw();
    canvas.cd();
  } // end of drawing ratio plot bit
  
  // Reset setYmax and min here, so you don't have to do it each time!
  xMin_ = 0.;
  xMax_ = 0.;
  yMin_ = 0.;
  yMax_ = 0.;
  canvas.RedrawAxis();  

  /////////////////
  // Save as PDF //
  /////////////////
  std::string filename;
  if(logy) filename = pdfDir_+std::string("/")+hname+std::string("_log")+".pdf";
  else	   filename = pdfDir_+std::string("/")+hname+".pdf";	
  std::cout << "Writing pdf file " << filename << std::endl;
  canvas.Print( filename.c_str() ,"pdf");
  // canvas.Print( (dir_+std::string("/")+hname+".png").c_str(),"png");

}


void StackPlot::dumpInfo(std::ostream &o) {

  o << "StackPlot" << std::endl;
  for (unsigned i=0; i<files_.size(); ++i) {
    o << "  dataset : " << labels_.at(i) << ", " << cols_.at(i) << ", " << styles_.at(i) << std::endl;
  }
  o << std::endl;

}
