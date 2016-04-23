/*
 * Histograms.h
 *
 *  Created on: 16 апр. 2016 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_MSSMHBB_SRC_HISTOGRAMS_H_
#define ANALYSIS_MSSMHBB_SRC_HISTOGRAMS_H_

#include <iostream>		// standard in/out
#include <memory> 		// for std::shared_ptr
#include <string>
#include <map>
#include "TMath.h"

#include "TH1.h"
#include "TH2.h"
#include "TEfficiency.h"

#include "stdlib.h"

	class Histograms {
	public:
		Histograms();
		virtual ~Histograms();

		void Make(const int &size = 100);

		std::map<std::string, TEfficiency* >& getEff();
		std::map<std::string, TH1 * >& getHisto();

	protected:
		std::map<std::string,TH1* > histo_;
		std::map<std::string,TEfficiency* > eff_;

	};

#endif /* ANALYSIS_MSSMHBB_SRC_HISTOGRAMS_H_ */
