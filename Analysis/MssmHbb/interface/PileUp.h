#ifndef PileUp_h
#define PileUp_h

#include "TROOT.h"
#include "TH1F.h"
#include <iostream>


class PileUp {

public:

		PileUp() : h_data(0), h_MC(0), h_ratio(0) {}; // constructor
		~ PileUp() {
			delete h_data;
			delete h_MC;
			delete h_ratio;
		}; //destructor

		void setDataHisto (TH1F * data_h);
		void setMCHisto (TH1F * mc_h);

		TH1F* getNormalisedDataHisto();
		TH1F* getNormalisedMCHisto();

		double getPUweight (const double & Nvertex_MC);



	private:
		TH1F * h_data;
		TH1F * h_MC;
		TH1F * h_ratio;

	protected:

		void make_ratio(); 


};

inline TH1F* PileUp::getNormalisedDataHisto () { return h_data; }
inline TH1F* PileUp::getNormalisedMCHisto () { return h_MC; }

#endif


