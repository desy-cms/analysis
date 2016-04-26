/*
 * doubleB.h
 *
 *  Created on: Apr 24, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_SELECTIONDOUBLEB_H_
#define MSSMHBB_INTERFACE_SELECTIONDOUBLEB_H_

#include <iostream>
#include <memory>
#include <string>

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"
//#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"


namespace analysis{
	namespace mssmhbb{
		class selectionDoubleB : public analysis::mssmhbb::JetAnalysisBase {
//			friend class MssmHbbSignal;
//			friend class ReverseB;
		public:
			selectionDoubleB(const std::string & inputFilelist,
						const bool & lowM = true,
							const bool & test = true);
			virtual ~selectionDoubleB();

		protected:

			//Overwrite Leading jet selection from JetAnalysisBase class
			virtual const bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
			//Overwrite Methods to work with histograms:
			virtual void fillHistograms(const std::shared_ptr<tools::Collection<tools::Jet> > &offlineJets);

		private:

	        double pt1_;
	        double pt2_;
	        double eta1_ = 2.2;
	        double eta2_ = 2.2;
	        double btag1_;
	        double btag2_;
	        double dEta_ = 1.6;
	        double dR_ = 1.;
		};
	}
}

#endif /* MSSMHBB_INTERFACE_SELECTIONDOUBLEB_H_ */
