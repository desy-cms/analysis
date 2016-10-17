/*
 * HbbStylesNew.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */
#ifndef MSSMHBB_MACROS_DRAWER_HBBSTYLESNEW_C_
#define MSSMHBB_MACROS_DRAWER_HBBSTYLESNEW_C_

#include "TPad.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"

class HbbStylesNew {
public:

	/// define common styles
	void SetStyle();
	/// create a canveas in common style (default is square)
	TCanvas* MakeCanvas(const char* name, const char* title, int dX=600, int dY=600);
	/// define common style of pads
	void InitSubPad(TPad* pad, int i);
	/// define a histogram file (not yet including poly markers, which are used fro data)
	void InitHist(TH1* hist, const char* xtit, const char* ytit="Number of Entries", int color=kBlack, int style=0);
	/// define poly markers and line width for data
	void InitData(TH1* hist);
	/// define line style, width and color for signal sample
	void InitSignal(TH1* hist);
	/// define common legend style
	void SetLegendStyle(TLegend* leg);
	/// add CMSPreliminary logo, lumi and channel
	void CMSPrelim(const char* dataset, double lowX=0.4, double lowY=0.74);
	void plotchannel(TString channel);

};

void HbbStylesNew::SetStyle()
{
  TStyle *HbbStyle = new TStyle("Htt-Style","The Perfect Style for Plots ;-)");
  gStyle = HbbStyle;

  // Canvas
  HbbStyle->SetCanvasColor     (0);
  HbbStyle->SetCanvasBorderSize(10);
  HbbStyle->SetCanvasBorderMode(0);
  HbbStyle->SetCanvasDefH      (700);
  HbbStyle->SetCanvasDefW      (700);
  HbbStyle->SetCanvasDefX      (100);
  HbbStyle->SetCanvasDefY      (100);

  // color palette for 2D temperature plots
  HbbStyle->SetPalette(1,0);

  // Pads
  HbbStyle->SetPadColor       (0);
  HbbStyle->SetPadBorderSize  (10);
  HbbStyle->SetPadBorderMode  (0);
  HbbStyle->SetPadBottomMargin(0.13);
  HbbStyle->SetPadTopMargin   (0.08);
  HbbStyle->SetPadLeftMargin  (0.15);
  HbbStyle->SetPadRightMargin (0.05);
  HbbStyle->SetPadGridX       (0);
  HbbStyle->SetPadGridY       (0);
  HbbStyle->SetPadTickX       (1);
  HbbStyle->SetPadTickY       (1);

  // Frames
  HbbStyle->SetLineWidth(3);
  HbbStyle->SetFrameFillStyle ( 0);
  HbbStyle->SetFrameFillColor ( 0);
  HbbStyle->SetFrameLineColor ( 1);
  HbbStyle->SetFrameLineStyle ( 0);
  HbbStyle->SetFrameLineWidth ( 2);
  HbbStyle->SetFrameBorderSize(10);
  HbbStyle->SetFrameBorderMode( 0);

  // Histograms
  HbbStyle->SetHistFillColor(2);
  HbbStyle->SetHistFillStyle(0);
  HbbStyle->SetHistLineColor(1);
  HbbStyle->SetHistLineStyle(0);
  HbbStyle->SetHistLineWidth(3);
  HbbStyle->SetNdivisions(505);

  // Functions
  HbbStyle->SetFuncColor(1);
  HbbStyle->SetFuncStyle(0);
  HbbStyle->SetFuncWidth(2);

  // Various
  HbbStyle->SetMarkerStyle(20);
  HbbStyle->SetMarkerColor(kBlack);
  HbbStyle->SetMarkerSize (1.4);

  HbbStyle->SetTitleBorderSize(0);
  HbbStyle->SetTitleFillColor (0);
  HbbStyle->SetTitleX         (0.2);

  HbbStyle->SetTitleSize  (0.055,"X");
  HbbStyle->SetTitleOffset(1.200,"X");
  HbbStyle->SetLabelOffset(0.005,"X");
  HbbStyle->SetLabelSize  (0.050,"X");
  HbbStyle->SetLabelFont  (42   ,"X");

  HbbStyle->SetStripDecimals(kFALSE);
  HbbStyle->SetLineStyleString(11,"20 10");

  HbbStyle->SetTitleSize  (0.055,"Y");
  HbbStyle->SetTitleOffset(1.600,"Y");
  HbbStyle->SetLabelOffset(0.010,"Y");
  HbbStyle->SetLabelSize  (0.050,"Y");
  HbbStyle->SetLabelFont  (42   ,"Y");

  HbbStyle->SetTextSize   (0.055);
  HbbStyle->SetTextFont   (42);

  HbbStyle->SetStatFont   (42);
  HbbStyle->SetTitleFont  (42);
  HbbStyle->SetTitleFont  (42,"X");
  HbbStyle->SetTitleFont  (42,"Y");

  HbbStyle->SetOptStat    (0);
  return;
}

TCanvas* HbbStylesNew::MakeCanvas(const char* name, const char *title, int dX, int dY)
{
  // Start with a canvas
  TCanvas *canvas = new TCanvas(name,title,0,0,dX,dY);
  // General overall stuff
  canvas->SetFillColor      (0);
  canvas->SetBorderMode     (0);
  canvas->SetBorderSize     (10);
  // Set margins to reasonable defaults
  canvas->SetLeftMargin     (0.18);
  canvas->SetRightMargin    (0.05);
  canvas->SetTopMargin      (0.08);
  canvas->SetBottomMargin   (0.15);
  // Setup a frame which makes sense
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);

  return canvas;
}

void HbbStylesNew::InitSubPad(TPad* pad, int i)
{
  pad->cd(i);
  TPad *tmpPad = (TPad*) pad->GetPad(i);
  tmpPad->SetLeftMargin  (0.18);
  tmpPad->SetTopMargin   (0.05);
  tmpPad->SetRightMargin (0.07);
  tmpPad->SetBottomMargin(0.15);
  return;
}

void HbbStylesNew::InitSignal(TH1 *hist)
{
  hist->SetFillStyle(0.);
  hist->SetLineStyle(11);
  hist->SetLineWidth(2.);
  hist->SetLineColor(kBlue+3);
}

void HbbStylesNew::InitHist(TH1 *hist, const char *xtit, const char *ytit, int color, int style)
{
  hist->SetXTitle(xtit);
  hist->SetYTitle(ytit);
  hist->SetLineColor(kBlack);
  hist->SetLineWidth(    2.);
  hist->SetFillColor(color );
  hist->SetFillStyle(style );
  hist->SetTitleSize  (0.055,"Y");
  hist->SetTitleOffset(1.600,"Y");
  hist->SetLabelOffset(0.014,"Y");
  hist->SetLabelSize  (0.040,"Y");
  hist->SetLabelFont  (42   ,"Y");
  hist->SetTitleSize  (0.055,"X");
  hist->SetTitleOffset(1.300,"X");
  hist->SetLabelOffset(0.014,"X");
  hist->SetLabelSize  (0.050,"X");
  hist->SetLabelFont  (42   ,"X");
  hist->SetMarkerStyle(20);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize (0.6);
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleFont(42);
  hist->SetTitle("");
  return;
}

void HbbStylesNew::InitData(TH1* hist)
{
  hist->SetMarkerStyle(20.);
  hist->SetMarkerSize (1.3);
  hist->SetLineWidth  ( 2.);
}

void HbbStylesNew::SetLegendStyle(TLegend* leg)
{
  leg->SetFillStyle (0);
  leg->SetFillColor (0);
  leg->SetBorderSize(0);
}

void CMSPrelim(const char* dataset, double lowX, double lowY)
{
  TPaveText* cmsprel  = new TPaveText(lowX, lowY+0.06, lowX+0.30, lowY+0.16, "NDC");
  cmsprel->SetBorderSize(   0 );
  cmsprel->SetFillStyle(    0 );
  cmsprel->SetTextAlign(   12 );
  cmsprel->SetTextSize ( 0.05 );
  cmsprel->SetTextColor(    1 );
  cmsprel->SetTextFont (   62 );
  cmsprel->AddText("CMS");
  cmsprel->Draw();

  TPaveText* lumi     = new TPaveText(lowX+0.08, lowY+0.061, lowX+0.45, lowY+0.161, "NDC");
  lumi->SetBorderSize(   0 );
  lumi->SetFillStyle(    0 );
  lumi->SetTextAlign(   12 );
  lumi->SetTextSize ( 0.05 );
  lumi->SetTextColor(    1 );
  lumi->SetTextFont (   62 );
  lumi->AddText(dataset);
  lumi->Draw();

}
void plotchannel(TString channel) {

   TLatex * tex = new TLatex(0.2,0.94,channel);
   tex->SetNDC();
   tex->SetTextSize(0.06);
   tex->SetLineWidth(2);
   tex->Draw();

}

#endif /* MSSMHBB_MACROS_DRAWER_HBBSTYLESNEW_H_ */
