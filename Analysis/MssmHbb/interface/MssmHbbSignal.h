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

#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"


namespace analysis{
	namespace mssmhbb{
		class MssmHbbSignal : public analysis::mssmhbb::JetAnalysisBase {
			public:
				MssmHbbSignal(const std::string & inputFilelist,
							const std::string & evtinfo = "MssmHbb/Events/EventInfo",
								const bool & lowM = true,
									const bool & test = true);
					virtual ~MssmHbbSignal();
		};
	}
}

#endif /* MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_ */
