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

class RatioPlots: public HbbStyle {
public:
	RatioPlots();
	virtual ~RatioPlots();
	void DrawRatio(TH1 *numerator, TH1 *denumerator, TLegend *leg, TCanvas *can);

	//Set
	void SetRatioTitle(const std::string &title = "Ratio Data/MC");
	void SetRatioRange(const double &ymin, const double &ymax);

private:

	std::string ratioTitle_;
	std::pair<double,double> ratioRange_;

};

inline void RatioPlots::SetRatioTitle(const std::string &title){ ratioTitle_ = title;}

inline void RatioPlots::SetRatioRange(const double &ymin, const double &ymax){ ratioRange_.first = ymin;
																		ratioRange_.second = ymax;}

RatioPlots::RatioPlots() {
	// TODO Auto-generated constructor stub

	// Setup style file
	TH1::SetDefaultSumw2();

}

RatioPlots::~RatioPlots() {
	// TODO Auto-generated destructor stub
}

void RatioPlots::DrawRatio(TH1 *numerator, TH1 *denumerator, TLegend *leg, TCanvas *can){

	//Create Top pad for this canva
	TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
	pad1 -> SetBottomMargin(0.05);
	pad1 -> Draw();
	pad1 -> cd();

	numerator -> Draw();
	denumerator -> Draw("same");
	leg->Draw();

	can->cd();
	TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
	pad2 -> SetTopMargin(0);
	pad2 -> SetBottomMargin(0.2);
	pad2 -> Draw();
	pad2 -> cd();

	TH1F * hRatio = (TH1F*)numerator->Clone();
	hRatio -> Sumw2();
	hRatio -> Divide(denumerator);
	hRatio -> Draw();

	TLine *horizLine = new TLine(numerator->GetXaxis()->GetXmin(),1,numerator->GetXaxis()->GetXmax(),1);
	horizLine -> SetLineStyle(2);
	horizLine -> Draw();

	TAxis *axis = new TAxis();
	axis = hRatio->GetYaxis();
	hRatio -> GetYaxis() -> SetRangeUser(ratioRange_.first,ratioRange_.second);
	hRatio -> GetYaxis() -> SetTitle(ratioTitle_.c_str());

	// X axis ratio plot settings
	hRatio -> GetXaxis() -> SetTitle(numerator->GetXaxis() -> GetTitle());
}

#endif /* ANALYSIS_DRAWER_SRC_RatioPlots_H_ */
