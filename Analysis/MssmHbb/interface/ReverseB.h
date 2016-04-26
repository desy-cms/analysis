/*
 * ReverseB.h
 *
 *  Created on: Apr 24, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_REVERSEB_H_
#define MSSMHBB_INTERFACE_REVERSEB_H_

#include <iostream>
#include <memory>
#include <string>

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"
#include "Analysis/MssmHbb/interface/selectionDoubleB.h"

typedef std::unique_ptr<analysis::mssmhbb::selectionDoubleB> pDoubleB;

namespace analysis{
	namespace mssmhbb{
	class ReverseB : public analysis::mssmhbb::JetAnalysisBase {
	public:
		ReverseB(const std::string & inputFilelist,
				const bool & lowM = true,
					const bool & test = true);
		virtual ~ReverseB();

		//Overwrite Leading jet selection from JetAnalysisBase class
		const bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
		//Overwrite Methods to work with histograms:
		void fillHistograms(const std::shared_ptr<tools::Collection<tools::Jet> > &offlineJets);

	private:

        const double pt1_ = 100.;
        const double pt2_ = 100.;
        const double pt3_ = 40.;
        const double eta1_ = 2.2;
        const double eta2_ = 2.2;
        const double eta3_ = 2.2;
        const double btag1_ = 0.935;
        const double btag2_ = 0.935;
        const double btag3_ = 0.8;
        const double dEta_ = 1.6;
        const double dR_ = 1.;

        pDoubleB doubleBanalysis;

	};
	}
}

#endif /* MSSMHBB_INTERFACE_REVERSEB_H_ */
