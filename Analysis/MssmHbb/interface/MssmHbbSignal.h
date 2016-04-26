/*
 * MssmHbbSignal.h
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_
#define MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"
#include "Analysis/MssmHbb/interface/selectionDoubleB.h"

typedef std::unique_ptr<analysis::mssmhbb::selectionDoubleB> pDoubleB;

namespace analysis{
	namespace mssmhbb{
		class MssmHbbSignal : public analysis::mssmhbb::selectionDoubleB {
			public:
				MssmHbbSignal(const std::string & inputFilelist,
								const bool & lowM = true,
									const bool & test = true);
				virtual ~MssmHbbSignal();

				//Overwrite Leading jet selection from JetAnalysisBase class
				const bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
				//Overwrite Methods to work with histograms:
				void fillHistograms(const std::shared_ptr<tools::Collection<tools::Jet> > &offlineJets);
				//Overwrite writeHistograms method
				void writeHistograms();

			private:

		        double pt1_;
		        double pt2_;
		        double pt3_;
		        double eta1_ = 2.2;
		        double eta2_ = 2.2;
		        double eta3_ = 2.2;
		        double btag1_;
		        double btag2_;
		        double btag3_;
		        double dEta_ = 1.6;
		        double dR_ = 1.;

		        pDoubleB doubleBanalysis;

		};
	}
}

#endif /* MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_ */
