/*
 * MssmHbbSignal.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

MssmHbbSignal::MssmHbbSignal(const std::string & inputFilelist, const std::string & evtinfo, const bool & lowM, const bool & test) :
							JetAnalysisBase(inputFilelist,evtinfo,lowM,test)
{
	// TODO Auto-generated constructor stub
	nJets_ = 3;
	triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
}

MssmHbbSignal::~MssmHbbSignal() {
	// TODO Auto-generated destructor stub
}

