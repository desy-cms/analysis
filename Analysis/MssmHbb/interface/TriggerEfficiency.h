/*
 * JetAnalysisBase.h
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_
#define ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_

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

namespace analysis{
	namespace mssmhbb{
	template<class Object>
	class JetAnalysisBase {
	public:

		//Default constructor
		JetAnalysisBase(const int & argc, char * argv[]);

		//Template constructors
		template <> JetAnalysisBase<"TriggerEfficiency">(const int & argc, char * argv[]);
		virtual ~JetAnalysisBase();

	protected:

		boost::program_options::variables_map input_map_;
		Analysis *analysis_;
		const auto cmsswBase_;
	};

	}
}



#endif /* ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_ */
