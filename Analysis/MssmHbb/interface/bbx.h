/*
 * bbx.h
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_BBX_H_
#define MSSMHBB_INTERFACE_BBX_H_

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"
#include "Analysis/MssmHbb/interface/DataMcComparison.h"

namespace analysis{
	namespace mssmhbb{
		class bbx : public analysis::mssmhbb::DataMcComparison {
			public:
				bbx(const std::string & inputFilelist,
								const double & dataLumi,
								const bool & lowM = true,
								const bool & test = true);
				virtual ~bbx();

				//Overwrite Leading jet selection from JetAnalysisBase class
				const bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);

			protected:

		        double pt3_  = 40;
		        double eta3_ = 2.2;
		        double btag3_;

		};
	}
}

#endif /* MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_ */
