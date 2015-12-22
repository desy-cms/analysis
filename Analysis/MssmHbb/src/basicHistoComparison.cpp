/*
 * basicHistoComparison.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: Rostyslav Shevchenko
 */

#include <iostream>
#include "Analysis/MssmHbb/interface/basicHistoComparison.h"

/*basicHistoComparison::basicHistoComparison(const PublicationStatus &status){
	style_.set(status);
}*/

basicHistoComparison::basicHistoComparison(){}

basicHistoComparison::~basicHistoComparison() {
	// TODO Auto-generated destructor stub
	//	delete histo_:
	for(const auto & it : histo_){
		delete it.second;
	}

	//	delete file_:
	for(const auto & it : file_){
		delete it.second;
	}
}

void basicHistoComparison::addHistogram(const std::string &rootFilePath, const std::string &histoName){

	//take path and get only the file name from it
	boost::filesystem::path p(rootFilePath);
	std::string fileName = (p.stem()).string();
	//read input file and check if it was not opened before
	if(!file_[fileName]) file_[fileName] = new TFile(rootFilePath.c_str(),"READ");
	//check whether file exist
	if(!file_[fileName] -> IsOpen()){
		std::cerr<<"Error: Wrong File Name. Please check input tags. Interrupted."<<std::endl;
		exit(0);
	}
	histo_[fileName+"/"+histoName] = (TH1F*) file_[fileName] -> Get(histoName.c_str());

	if(!histo_[fileName+"/"+histoName]) {
		std::cerr<<"Error: TH1F "<<histoName.c_str()<<" NOT found in File "<<rootFilePath.c_str()<<std::endl;
		exit(1);
	}
}

void basicHistoComparison::drawComparison(){
	int dist = 0;
	for(const auto & h : histo_){
		if(dist != 0) h.second -> Draw("E same");
		else h.second -> Draw("E");
		dist++;
	}
}

void basicHistoComparison::draw(TCanvas *canva){

//	std::map<std::string, TH1F*>::iterator first = histo_.begin();
	TLegend *leg = new TLegend(0.6,0.6,0.85,0.85);
	int dist = 0;
	for(const auto & h : histo_)
	{
		h.second -> SetStats(kFALSE);
		h.second -> SetMarkerStyle(20+dist);
		h.second -> SetMarkerSize(1.5);
		h.second -> SetMarkerColor(1+dist);
		if(dist != 0) h.second -> Draw("E same");
		else h.second -> Draw("E");
		leg -> AddEntry(h.second,(h.first).c_str(),"p");
		dist++;
	}
	leg -> Draw();
	canva -> Draw();
}

