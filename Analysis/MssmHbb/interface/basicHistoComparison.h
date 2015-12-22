/*
 * basicHistoComparison.h
 *
 *  Created on: Nov 27, 2015
 *      Author: Rostyslav Shevchenko
 */

#ifndef BASICHISTOCOMPARISON_H_
#define BASICHISTOCOMPARISON_H_

#include <string>
#include <memory>
#include <map>
#include <iterator>
#include <boost/filesystem.hpp>

#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"

//#include "Analysis/MssmHbb/src/HbbStyle.cc"

class basicHistoComparison {
public:
//	basicHistoComparison(const PublicationStatus &status = PUBLIC);
	basicHistoComparison();
	virtual ~basicHistoComparison();
	void addHistogram(const std::string &rootFilePath, const std::string &histoName);
	void draw(TCanvas *canva);
	void drawComparison();

	//Get  methods:
	TH1F * histo(const std::string &name);

private:
	std::string MPproductionFolder_;
	std::string rootFileName_;
	std::map<std::string, TH1F*> histo_;
	std::map<std::string,TFile *> file_;

//	HbbStyle style_;
};

inline TH1F * basicHistoComparison::histo(const std::string &name) { return histo_[name];}

#endif /* BASICHISTOCOMPARISON_H_ */
