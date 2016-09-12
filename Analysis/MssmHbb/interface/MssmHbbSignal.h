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
#include "Analysis/MssmHbb/interface/utilLib.h"

namespace analysis{
	namespace mssmhbb{
		class MssmHbbSignal : public analysis::mssmhbb::selectionDoubleB {
			public:
				MssmHbbSignal(const std::string & inputFilelist,
								const double & dataLumi,
								const bool & lowM = true,
								const bool & test = true);
				virtual ~MssmHbbSignal();

				//Method to add background template
				void addBackgroundTemplate(const std::string & bg_template, const std::string & bgHisto);
				//method to add Statistical uncertainties templates
				void addStatErrorsTemplates(const double & relative_threshold = 0.05);
				void addStatErrorsTemplates(const int & nbins = 10);

				//Overwrite Leading jet selection from JetAnalysisBase class
				const bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
				//Overwrite Methods to work with histograms:
				void fillHistograms(const std::shared_ptr<tools::Collection<tools::Jet> > &offlineJets, const double & weight);
				//Overwrite assignWeight method:
				const double assignWeight();
				//Overwrite writeHistograms method
				void writeHistograms();
				//Overwrite runner
				void runAnalysis(const std::string &json, const std::string &output = "", const int &size = 100);
				//Calculate Signal Efficiency
				void signalEfficiency();


			protected:
				//Overwrite jet modification
		        std::shared_ptr<tools::Collection<tools::Jet> > modifyJetCollection( tools::Jet & jet,
		        																	  std::shared_ptr<tools::Collection<tools::Jet> > & initialJets
																					  );

		        double pt3_  = 40;
		        double eta3_ = 2.2;
		        double btag3_;

			private:

		};
	}
}

#endif /* MSSMHBB_INTERFACE_MSSMHBBSIGNAL_H_ */
