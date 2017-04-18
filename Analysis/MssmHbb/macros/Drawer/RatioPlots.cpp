/*
 * RatioPlots.cpp
 *
 *  Created on: 24 дек. 2015 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_DRAWER_SRC_RatioPlots_CPP_
#define ANALYSIS_DRAWER_SRC_RatioPlots_CPP_

#include "TCut.h"
#include "TH1.h"
#include "TLegend.h"
#include "TAxis.h"
#include <string>
#include <utility>
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TExec.h"
#include "TF1.h"

#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

class RatioPlots: public HbbStyle {
public:
	RatioPlots(const PublicationStatus status);
	RatioPlots();
	virtual ~RatioPlots();

	TH1 *DrawRatio(TH1 *numerator, TH1 *denumerator, TCanvas *can = nullptr, TLegend *leg = nullptr, TH1 *systErr = nullptr, TH1 *mc_stat_err = nullptr);
	TH1 *DrawRatio(TH1 *histo, TF1 *fit, std::string fitName, TCanvas *can = 0);
	TH1 *DrawRatio(TH1 *numerator, TH1 *denumerator, TGraphAsymmErrors *systErr = nullptr, TLegend *leg = nullptr, TCanvas *can = nullptr);
	TGraphAsymmErrors *DrawRatio(TGraphAsymmErrors *numerator, TGraphAsymmErrors *denumerator, TCanvas *can = nullptr, TLegend *leg = nullptr, TGraphAsymmErrors *systErr = nullptr);
	void DrawPhaseSpaceDescription(float x1, float y1, float x2, float y2);
	bool FindMaximum(TH1*,TH1*);
	bool FindMaximum(TGraphAsymmErrors*,TGraphAsymmErrors*);
	TGraphAsymmErrors * CreateSystTGraph(TH1 *central, TH1 *up, TH1 *down);


	//Set
	void SetRatioTitle(const std::string &title = "Ratio Data/MC");
	void SetRatioRange(const double &ymin, const double &ymax);

	//Get
	TPad *GetTopPad();
	TPad *GetBottomPad();
	void SetBottomStyle(TH1*);
	void SetTopStyle(TH1*);
	void SetBottomStyle(TGraph*);

private:

	std::string ratioTitle_;
	std::pair<double,double> ratioRange_;
	HbbStyle style_;

	TPad *pad1_ = NULL;
	TPad *pad2_ = NULL;

	TExec *er_0;
	TExec *er_1;

};

inline void RatioPlots::SetRatioTitle(const std::string &title){ ratioTitle_ = title;}

inline void RatioPlots::SetRatioRange(const double &ymin, const double &ymax){ ratioRange_.first = ymin;
																		ratioRange_.second = ymax;}

inline TPad *RatioPlots::GetTopPad(){ return pad1_;}
inline TPad *RatioPlots::GetBottomPad(){ return pad2_;}

RatioPlots::RatioPlots(const PublicationStatus status) {
	// TODO Auto-generated constructor stub
	style_.set(status);
	er_0 = new TExec("er_0","gStyle->SetErrorX(0)");
	er_1 = new TExec("er_1","gStyle->SetErrorX(0.5)");
	// Setup style file
	TH1::SetDefaultSumw2();
}

RatioPlots::RatioPlots() {
	// TODO Auto-generated constructor stub
	er_0 = new TExec("er_0","gStyle->SetErrorX(0)");
	er_1 = new TExec("er_1","gStyle->SetErrorX(0.5)");
	// Setup style file
	TH1::SetDefaultSumw2();

}

RatioPlots::~RatioPlots() {
	// TODO Auto-generated destructor stub
}

TGraphAsymmErrors * RatioPlots::CreateSystTGraph(TH1 *central, TH1 *up, TH1* down){
	TGraphAsymmErrors *result = new TGraphAsymmErrors(central);
	for(int i = 1; i <= central->GetNbinsX(); ++i){
		double eyl = down->GetBinContent(i);
		double eyh = up->GetBinContent(i);
		//Add stat errors:
		eyl = std::sqrt(central->GetBinError(i)*central->GetBinError(i) + eyl*eyl);
		eyh = std::sqrt(central->GetBinError(i)*central->GetBinError(i) + eyh*eyh);
		result->SetPointError(i-1,0,0,eyl,eyh);
	}

	return result;
}

TH1* RatioPlots::DrawRatio(TH1 *numerator, TH1 *denumerator, TCanvas *can, TLegend *leg, TH1 *systErr, TH1 *mc_stat_err){

	TH1::SetDefaultSumw2();
	//Create Top pad for this canva
	pad1_ = new TPad("pad1","pad1",0,0.1,1,1);
	pad1_ -> SetBottomMargin(0.2);
	pad1_ -> SetRightMargin(0.05);
	pad1_ -> SetLeftMargin(0.16);
	pad1_ -> Draw();
	pad1_ -> cd();

	numerator -> SetMarkerStyle(20);
	numerator -> SetMarkerSize(gStyle->GetMarkerSize());

	if(!FindMaximum(numerator,denumerator))
	{
		if(systErr) numerator->SetMaximum(denumerator->GetMaximum() + .2* denumerator->GetMaximum());
		else numerator->SetMaximum(denumerator->GetMaximum() + .1* denumerator->GetMaximum());
	}

	numerator->SetMaximum(numerator->GetMaximum()*1.1);
	numerator->GetXaxis()->SetNdivisions(505);
	numerator->GetYaxis()->SetNdivisions(206);
	numerator->GetYaxis()->SetTitleOffset(1.1);
	numerator->GetXaxis()->SetTitleOffset(999);
	numerator->GetXaxis()->SetLabelOffset(999);
	numerator->GetYaxis()->SetTitleSize(0.048);
	numerator->SetStats(0);
	numerator -> Draw("E");

	if(systErr){
		denumerator->SetMarkerStyle(21);
		denumerator->SetMarkerColor(2);
		systErr->SetLineColor(2);
		systErr->SetLineWidth(2);
		systErr->SetFillColor(kMagenta-10);
		systErr->SetFillStyle(1001);
		er_1->Draw();
		systErr -> Draw("E2 same");
		er_0->Draw();
	}

	if(systErr) er_1->Draw();
	denumerator -> Draw("hist same");
	if(systErr) er_0->Draw();

	//Create stat. MC errors:
	if(mc_stat_err){
		mc_stat_err->SetLineColor(kBlack);
		mc_stat_err->SetFillColor(kBlack);
		mc_stat_err->SetFillStyle(3013); //3002
		er_1->Draw();
		mc_stat_err->Draw("E2 same");
		er_0->Draw();
	}

	numerator -> Draw("Esame");

	if(leg) leg->Draw();

	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.265);
	pad2_ -> SetTopMargin(0);
	pad2_ -> SetLeftMargin(pad1_->GetLeftMargin());
	pad2_ -> SetRightMargin(pad1_->GetRightMargin());
	pad2_ -> SetBottomMargin(0.28);
	pad2_ -> Draw();
	pad2_ -> cd();

	TH1 * hRatio = (TH1*)numerator->Clone("hRatio");
	hRatio -> Sumw2();
	for(int i = 0; i  < numerator->GetNbinsX(); ++i){
		if(denumerator->GetBinContent(i+1) != 0){
			hRatio->SetBinContent(i+1, hRatio->GetBinContent(i+1) / denumerator->GetBinContent(i + 1));
			hRatio->SetBinError(i+1, hRatio->GetBinError(i+1) / denumerator->GetBinContent(i + 1));
		}
		else {
			hRatio->SetBinContent(i+1,0);
			hRatio->SetBinError(i+1,0);
		}
	}
//	hRatio -> Divide(denumerator);

	hRatio->GetXaxis()->SetTitle(numerator->GetXaxis()->GetTitle());
	hRatio->GetXaxis()->SetTickLength(numerator->GetXaxis()->GetTickLength()*3);
	SetBottomStyle(hRatio);
	hRatio -> Draw();

	//MC stat errors:
	TH1 *hRatio_MCStat;
	if(mc_stat_err){
		hRatio_MCStat = (TH1*) mc_stat_err->Clone("hRatio_MCStat");
//		hRatio_MCStat->Divide(mc_stat_err);
		//Set proper errors:
		for(int i = 0; i<mc_stat_err->GetNbinsX();++i){
			hRatio_MCStat->SetBinContent(i+1,1);
			hRatio_MCStat->SetBinError(i+1, mc_stat_err->GetBinError(i+1) / mc_stat_err->GetBinContent(i+1));
			if(mc_stat_err->GetBinContent(i+1) == 0) hRatio_MCStat->SetBinError(i+1,999.);
			if(mc_stat_err->GetBinContent(i+1) == 0 && numerator->GetBinContent(i+1) == 0) hRatio_MCStat->SetBinError(i+1,0);
		}
		er_1->Draw();
		hRatio_MCStat->Draw("E2 same");
		er_0->Draw();
	}

	TLine *horizLine = new TLine(hRatio->GetXaxis()->GetXmin(),1,hRatio->GetXaxis()->GetXmax(),1);
	horizLine -> SetLineStyle(2);
	horizLine -> Draw();

	//Add Systematic errors:
	if(systErr){
	TH1 *RatioSyst = (TH1*) systErr->Clone("RatioSyst");
		RatioSyst->Divide(numerator,RatioSyst);
		RatioSyst->SetFillColor(kMagenta-10);
		RatioSyst->SetFillStyle(1001);

		RatioSyst->Draw("E5 same");
		hRatio->Draw("same");
		horizLine -> Draw();
	}

	pad1_ -> cd();
	return hRatio;
}

TH1* RatioPlots::DrawRatio(TH1 *histo, TF1 * fit, std::string fitName, TCanvas *can){

	TH1::SetDefaultSumw2();
	//Create Top pad for this canva
	pad1_ = new TPad("pad1","pad1",0,0.3,1,1);
	pad1_ -> SetBottomMargin(0.0);
	pad1_ -> Draw();
	pad1_ -> cd();

	TH1 * hRatio = (TH1*)histo->Clone("hRatio");
	histo -> Fit(fit,"R+");
	char name[200];
	sprintf(name,"}{#chi^{2}/ndf = %.2f}",fit->GetChisquare()/fit->GetNDF());
	fitName = "#splitline{ " + fitName + name;
	TLegend *fitLegend = new TLegend(0.7,0.45,0.9,0.65);
	fitLegend -> SetTextSize(0.04);
	fitLegend -> AddEntry(fit,fitName.c_str(),"l");
	histo -> Draw();
	fitLegend->Draw();
//	style_.standardTitle()->Draw();
//	std::cout<<fit->GetChisquare()<<" "<<fit->GetNDF()<<" "<<fit->GetChisquare()/fit->GetNDF()<<std::endl;

	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2_ -> SetTopMargin(0.0);
	pad2_ -> SetBottomMargin(0.35);
	pad2_ -> Draw();
	pad2_ -> cd();

	hRatio -> Sumw2();
	hRatio -> Divide(fit);
	hRatio -> Draw();

	TLine *horizLine = new TLine(histo->GetXaxis()->GetXmin(),1,histo->GetXaxis()->GetXmax(),1);
	horizLine -> SetLineStyle(2);
	horizLine -> Draw();

	SetBottomStyle(hRatio);

	hRatio -> GetYaxis() -> SetRangeUser(ratioRange_.first,ratioRange_.second);
	hRatio -> GetYaxis() -> SetTitle(ratioTitle_.c_str());
	hRatio -> GetXaxis() -> SetTitle(histo->GetXaxis()->GetTitle());

	return hRatio;
}

TH1* RatioPlots::DrawRatio(TH1 *numerator, TH1 *denumerator, TGraphAsymmErrors *systErr, TLegend *leg, TCanvas *can){

	//Create Top pad for this canva
	pad1_ = new TPad("pad1","pad1",0,0.3,1,1);
	pad1_ -> SetBottomMargin(0.0);
	pad1_ -> Draw();
	pad1_ -> cd();

	numerator -> SetMarkerStyle(20);

	denumerator->SetMarkerStyle(21);
	denumerator->SetMarkerColor(2);

	if(!FindMaximum(numerator,denumerator))
	{
		if(systErr) numerator->SetMaximum(denumerator->GetMaximum() + .2* denumerator->GetMaximum());
		else numerator->SetMaximum(denumerator->GetMaximum() + .1* denumerator->GetMaximum());
	}
	numerator -> Draw();
	denumerator -> Draw("same");

	if(systErr){
	systErr->SetLineColor(2);
	systErr->SetLineWidth(2);
	systErr->SetLineStyle(3);
	systErr -> Draw("p");
	}
	if(leg) leg->Draw();
	style_.standardTitle()->Draw();

	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2_ -> SetTopMargin(0.0);
	pad2_ -> SetBottomMargin(0.35);
	pad2_ -> Draw();
	pad2_ -> cd();

	TH1 * hRatio = (TH1*)numerator->Clone("hRatio");
	hRatio -> Divide(denumerator);
	hRatio -> Draw();

	TLine *horizLine = new TLine(numerator->GetXaxis()->GetXmin(),1,numerator->GetXaxis()->GetXmax(),1);
	horizLine -> SetLineStyle(2);
	horizLine -> Draw();

	SetBottomStyle(hRatio);

	hRatio -> GetYaxis() -> SetRangeUser(ratioRange_.first,ratioRange_.second);
	hRatio -> GetYaxis() -> SetTitle(ratioTitle_.c_str());
	hRatio -> GetXaxis() -> SetTitle(numerator->GetXaxis()->GetTitle());

	//Add Systematic errors:
	if(systErr){
	TGraphAsymmErrors *RatioSyst = (TGraphAsymmErrors*) systErr->Clone("RatioSyst");
	for (int i = 1; i<=numerator->GetNbinsX();++i){
		double x,y;
		RatioSyst->GetPoint(i-1,x,y);
		RatioSyst->SetPoint(i-1,x,numerator->GetBinContent(i)/y);
		double eyh = numerator->GetBinContent(i)/y * std::sqrt( pow(numerator->GetBinError(i)/numerator->GetBinContent(i),2) + pow(RatioSyst->GetErrorYhigh(i-1)/y,2) );
		double eyl = numerator->GetBinContent(i)/y * std::sqrt( pow(numerator->GetBinError(i)/numerator->GetBinContent(i),2) + pow(RatioSyst->GetErrorYlow(i-1)/y,2) );
		RatioSyst->SetPointError(i-1,0,0,eyl,eyh);

	}
	RatioSyst->SetFillColor(kGreen-9);
	RatioSyst->SetFillStyle(3001);
	RatioSyst->Draw("3");
	}
	return hRatio;
}

TGraphAsymmErrors * RatioPlots::DrawRatio(TGraphAsymmErrors *numerator, TGraphAsymmErrors *denumerator, TCanvas *can,TLegend *leg, TGraphAsymmErrors *systErr){
	TH1::SetDefaultSumw2();
	//Create Top pad for this canva
	pad1_ = new TPad("pad1","pad1",0,0.3,1,1);
	pad1_ -> SetBottomMargin(0.0);
	pad1_ -> Draw();
	pad1_ -> cd();
//	numerator -> SetMarkerStyle(20);

//	denumerator->SetMarkerStyle(21);
//	denumerator->SetMarkerColor(2);

	numerator -> Draw("AP");
	denumerator -> Draw("P same");

	if(leg) leg->Draw();
	style_.standardTitle()->Draw();
	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2_ -> SetTopMargin(0.0);
	pad2_ -> SetBottomMargin(0.35);
	pad2_ -> Draw();
	pad2_ -> cd();

	TGraphAsymmErrors * hRatio = (TGraphAsymmErrors*)numerator->Clone("hRatio");
//	TGraphAsymmErrors * hRatio = new TGraphAsymmErrors(*numerator);
//	hRatio->Divide(numerator->GetHistogram(),denumerator->GetHistogram(),"pois");

	double *num_x = numerator->GetX();
	double *num_y = numerator->GetY();
//	double *den_x = denumerator->GetX();
	double *den_y = denumerator->GetY();
	for( int i = 0; i < numerator->GetN(); ++i){
	
		double val = num_y[i]/den_y[i];
		hRatio->SetPoint(i,num_x[i],val);
//		double el = std::sqrt(numerator->GetErrorYlow(i)*numerator->GetErrorYlow(i) + denumerator->GetErrorYhigh(i)*denumerator->GetErrorYhigh(i));
		double el = val * std::sqrt( std::pow(numerator->GetErrorYlow(i)/(num_y[i]),2) + std::pow(denumerator->GetErrorYhigh(i)/(den_y[i]),2) );
		hRatio->SetPointEYlow(i,el);
//		double eu = std::sqrt(numerator->GetErrorYhigh(i)*numerator->GetErrorYhigh(i) + denumerator->GetErrorYlow(i)*denumerator->GetErrorYlow(i));
		double eu = val* std::sqrt( std::pow(numerator->GetErrorYhigh(i)/(num_y[i]),2) + std::pow(denumerator->GetErrorYlow(i)/(den_y[i]),2) );
		hRatio->SetPointEYhigh(i,eu);
	}
	hRatio -> Draw("AP");

//	TLine *horizLine = new TLine(numerator->GetXaxis()->GetXmin(),1,numerator->GetXaxis()->GetXmax(),1);
//	horizLine -> SetLineStyle(2);
//	horizLine -> Draw();

//	SetBottomStyle(hRatio);

	hRatio -> GetYaxis() -> SetRangeUser(ratioRange_.first,ratioRange_.second);
	hRatio -> GetXaxis() -> SetRangeUser(numerator->GetXaxis()->GetXmin(),numerator->GetXaxis()->GetXmax());
	hRatio -> GetYaxis() -> SetTitle(ratioTitle_.c_str());
	hRatio -> GetXaxis() -> SetTitle(numerator->GetXaxis()->GetTitle());

	can->cd();
	return hRatio;
}



bool RatioPlots::FindMaximum(TH1* first, TH1 *second){
	if(first->GetMaximum() > second->GetMaximum()) return true;
	else return false;
}

bool RatioPlots::FindMaximum(TGraphAsymmErrors* first, TGraphAsymmErrors *second){
	if(first->GetMaximum() > second->GetMaximum()) return true;
	else return false;
}

void RatioPlots::SetBottomStyle(TH1 *hRatio){

	hRatio->GetXaxis()->SetTitleSize(0.13);
	hRatio->GetXaxis()->SetTitleOffset(0.93);
	hRatio->GetXaxis()->SetLabelOffset(0.010);
	hRatio->GetXaxis()->SetNdivisions(505);
	hRatio->GetYaxis()->CenterTitle(kTRUE);
	hRatio->GetYaxis()->SetTitleSize(0.14);
	hRatio->GetYaxis()->SetTitleOffset(0.4);
	hRatio->GetYaxis()->SetNdivisions(206);
	hRatio->GetYaxis()->SetLabelSize(0.115);
	hRatio->GetYaxis()->SetLabelOffset(0.011);
	hRatio->GetXaxis()->SetLabelSize(0.115);


}

void RatioPlots::SetTopStyle(TH1 *numerator){

	numerator->SetMaximum(numerator->GetMaximum()*1.1);
	numerator->GetXaxis()->SetNdivisions(505);
	numerator->GetYaxis()->SetNdivisions(206);
	numerator->GetYaxis()->SetTitleOffset(1.1);
	numerator->GetXaxis()->SetTitleOffset(999);
	numerator->GetXaxis()->SetLabelOffset(999);
	numerator->GetYaxis()->SetTitleSize(0.048);
	numerator->SetStats(0);


}

void RatioPlots::SetBottomStyle(TGraph *hRatio){

	hRatio ->GetXaxis()-> SetNdivisions(510);
	hRatio ->GetYaxis()-> SetNdivisions(505);
	hRatio ->GetXaxis()->SetTickLength(0.07);
	hRatio ->GetYaxis()->SetTickLength(0.04);
	hRatio ->GetXaxis()->SetTitleSize(0.06*7/3);
	hRatio ->GetYaxis()->SetTitleSize(0.06*7/3);
	hRatio ->GetXaxis()->SetTitleFont(42);
	hRatio ->GetYaxis()->SetTitleFont(42);
	hRatio ->GetXaxis()->SetLabelSize(0.05*7/3);
	hRatio ->GetYaxis()->SetLabelSize(0.05*7/3);
	hRatio ->GetXaxis()->SetLabelOffset(0.01);
	hRatio ->GetYaxis()->SetLabelOffset(0.01);

	hRatio -> GetYaxis() -> SetTitleOffset(0.6);
	hRatio -> GetXaxis() -> SetTitleOffset(1.);
}

void RatioPlots::DrawPhaseSpaceDescription(float x1, float y1, float x2, float y2){
	pad1_->cd();
	TPaveText *description = new TPaveText(x1,y1,x2,y2);
	description->AddText("At least 2 b jets");
	description->AddText("p^{jet}_{T} > 100 GeV, |#eta^{jet}| < 2.2");
	description->AddText("|#Delta#eta_{1-2}| < 1.6, CSVv2T (BTag Discr. > 0.935)");
	description->SetFillColor(0);
	description->SetBorderSize(0);
	description->Draw();
}

#endif /* ANALYSIS_DRAWER_SRC_RatioPlots_H_ */
