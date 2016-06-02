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

class RatioPlots: public HbbStyle {
public:
	RatioPlots(const PublicationStatus status);
	virtual ~RatioPlots();

	TH1 *DrawRatio(TH1 *numerator, TH1 *denumerator, TCanvas *can = nullptr, TH1 *systErr = nullptr, TLegend *leg = nullptr);
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

private:

	std::string ratioTitle_;
	std::pair<double,double> ratioRange_;
	void SetBottomStyle(TH1*);
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

TH1* RatioPlots::DrawRatio(TH1 *numerator, TH1 *denumerator, TCanvas *can, TH1 *systErr, TLegend *leg){

	TH1::SetDefaultSumw2();
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

	if(systErr){
	systErr->SetLineColor(2);
	systErr->SetLineWidth(2);
//	systErr->SetLineStyle(3);
	systErr->SetFillColor(kMagenta-10);
	systErr->SetFillStyle(1001);
//	systErr -> Draw("E1 same");
	er_1->Draw();
	systErr -> Draw("E2 same");
	er_0->Draw();
	}

	er_1->Draw();
	denumerator -> Draw("same");
	er_0->Draw();
	numerator -> Draw("same");

	if(leg) leg->Draw();
//	style_.standardTitle()->Draw();

	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2_ -> SetTopMargin(0.0);
	pad2_ -> SetBottomMargin(0.35);
	pad2_ -> Draw();
	pad2_ -> cd();

	TH1 * hRatio = (TH1*)numerator->Clone("hRatio");
	hRatio -> Sumw2();
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
	TH1 *RatioSyst = (TH1*) systErr->Clone("RatioSyst");
	RatioSyst->Divide(numerator,RatioSyst);
	RatioSyst->SetFillColor(kMagenta-10);
	RatioSyst->SetFillStyle(1001);

	RatioSyst->Draw("E5 same");
	hRatio->Draw("same");
	horizLine -> Draw();
	}

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

TGraphAsymmErrors * RatioPlots::DrawRatio(TGraphAsymmErrors *numerator, TGraphAsymmErrors *denumerator, TCanvas *can = nullptr,TLegend *leg = nullptr, TGraphAsymmErrors *systErr = nullptr){
	TH1::SetDefaultSumw2();
	//Create Top pad for this canva
	pad1_ = new TPad("pad1","pad1",0,0.3,1,1);
	pad1_ -> SetBottomMargin(0.0);
	pad1_ -> Draw();
	pad1_ -> cd();
	std::cout<<"WTF"<<std::endl;
	numerator -> SetMarkerStyle(20);

	denumerator->SetMarkerStyle(21);
	denumerator->SetMarkerColor(2);

	if(!FindMaximum(numerator,denumerator))
	{
		if(systErr) numerator->SetMaximum(denumerator->GetMaximum() + .2* denumerator->GetMaximum());
		else numerator->SetMaximum(denumerator->GetMaximum() + .1* denumerator->GetMaximum());
	}
	numerator -> Draw("AP");
	std::cout<<"WTF"<<std::endl;
	if(systErr){
	systErr->SetLineColor(2);
	systErr->SetLineWidth(2);
//	systErr->SetLineStyle(3);
	systErr->SetFillColor(kMagenta-10);
	systErr->SetFillStyle(1001);
//	systErr -> Draw("E1 same");
	er_1->Draw();
	systErr -> Draw("E2 same");
	er_0->Draw();
	}

	er_1->Draw();
	denumerator -> Draw("P same");
	er_0->Draw();
	numerator -> Draw("P same");

	if(leg) leg->Draw();
	style_.standardTitle()->Draw();
	std::cout<<"WTF"<<std::endl;
	can->cd();
	pad2_ = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2_ -> SetTopMargin(0.0);
	pad2_ -> SetBottomMargin(0.35);
	pad2_ -> Draw();
	pad2_ -> cd();

	std::cout<<"WTF"<<std::endl;
	TGraphAsymmErrors * hRatio = (TGraphAsymmErrors*)numerator->Clone("hRatio");
//	hRatio->Divide(numerator->GetHistogram(),denumerator->GetHistogram(),"pois");

	double *num_x = numerator->GetX();
	double *num_y = numerator->GetY();
	double *den_x = denumerator->GetX();
	double *den_y = denumerator->GetY();
	for( int i = 0; i < numerator->GetN(); ++i){
//
		double val = num_y[i-1]/den_y[i-1];
		hRatio->SetPoint(i,num_x[i-1],val);
		double el = numerator->GetErrorYlow(i)/denumerator->GetErrorYlow(i)* std::sqrt( std::pow((num_y[i-1])/numerator->GetErrorYlow(i),2) + std::pow((den_y[i-1])/denumerator->GetErrorYlow(i),2) );
		hRatio->SetPointEYlow(i,0);
		double eu = numerator->GetErrorYhigh(i)/denumerator->GetErrorYhigh(i)* std::sqrt( std::pow((num_y[i-1])/numerator->GetErrorYhigh(i),2) + std::pow((den_y[i-1])/denumerator->GetErrorYhigh(i),2) );
		hRatio->SetPointEYhigh(i,0);
//		std::cout<<i<<" "<<val<<" up+/- "<<eu<<" dn+/- "<<el<<std::endl;
//		std::cout<<"down: "<<numerator->GetErrorXlow(i)/denumerator->GetErrorXlow(i)<<" "<<std::sqrt( std::pow((num_y[i-1])/numerator->GetErrorXlow(i),2) + std::pow((den_y[i-1])/denumerator->GetErrorXlow(i),2) )<<std::endl;
	}
	hRatio -> Draw("AP");

	TLine *horizLine = new TLine(numerator->GetXaxis()->GetXmin(),1,numerator->GetXaxis()->GetXmax(),1);
	horizLine -> SetLineStyle(2);
	horizLine -> Draw();

//	SetBottomStyle(hRatio);

	hRatio -> GetYaxis() -> SetRangeUser(ratioRange_.first,ratioRange_.second);
	hRatio -> GetYaxis() -> SetTitle(ratioTitle_.c_str());
	hRatio -> GetXaxis() -> SetTitle(numerator->GetXaxis()->GetTitle());

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

	/*
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(0.13);
//   hRatio -> GetYaxis() -> SetTitleFont(60);
   hRatio -> GetYaxis() -> SetTitleOffset(0.7);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(23);

   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitleSize(0.13);
   hRatio -> GetXaxis() -> SetTitleOffset(0.85);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(23);
   */

	hRatio -> SetNdivisions(505,"YZ");
	hRatio -> SetNdivisions(510,"X");
	hRatio -> SetTickLength(0.04,"YZ");
	hRatio -> SetTickLength(0.07,"X");
	hRatio -> SetTitleSize(0.06*7/3,"XYZ");
	hRatio -> SetTitleFont(42,"XYZ");

	hRatio -> SetLabelFont(42,"XYZ");
	hRatio -> SetLabelSize(0.05*7/3,"XYZ");
	hRatio -> SetLabelOffset(0.01,"XYZ");

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
