#ifndef Analysis_MssmHbb_MssmHbb_h
#define Analysis_MssmHbb_MssmHbb_h 1

// -*- C++ -*-
//
// Package:    Analysis/MssmHbb
// Class:      Analysis
// 
/**\class Analysis MssmHbb.cc Analysis/MssmHbb/src/MssmHbb.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
// Modified by: Rostyslav Shevchenko
//

// system include files
#include <memory>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
// 
// user include files

#include "Analysis/Tools/interface/Analysis.h"
namespace po = boost::program_options;

//Macros that provide information about the current CMSSW version
#define CMSSW std::string(PROJECT_VERSION)

//
// class declaration
//

using namespace analysis::tools;

namespace analysis {
		namespace mssmhbb {

		class TriggerEfficiency{
         	 public:
			TriggerEfficiency(const int & argc, char * argv[]);
			~TriggerEfficiency();

			void SetupAnalysis();
			void processJsonFile(const std::string & fileName);

         	 protected:

			po::variables_map input_map_;
			Analysis *analysis_;
			const auto cmsswBase_;
               
         	 private:

		};


	}
}

#endif  // Analysis_MssmHbb_MssmHbb_h
