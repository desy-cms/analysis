/*
 * JetAnalysisBase.h
 *
 *  Created on: 17 апр. 2016 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_
#define ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH2D.h"

// system include files
#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "TSystem.h"
//
// user include files

#include "Analysis/Tools/interface/Analysis.h"
#include "Analysis/Tools/interface/Collection.h"
#include "Analysis/Tools/interface/Jet.h"

#include "Analysis/MssmHbb/interface/Weights.h"
#include "Analysis/MssmHbb/interface/Histograms.h"
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"


//TODO: Add template arguments to unique_ptr depending on the TH type
typedef std::unique_ptr<TFile> pTFile;
typedef std::unique_ptr<BTagCalibrationReader> pSFReader;

namespace analysis{
	namespace mssmhbb{
	class JetAnalysisBase : public analysis::tools::Analysis {
	public:

		//Default constructor
		JetAnalysisBase(const std::string & inputFilelist, const bool & lowM = true, const bool & test = true);
		virtual ~JetAnalysisBase();

        //Add trigger Object Trees
        void addTriggerObjects(const std::vector<std::string> & triggerObjectName = {"hltL1sL1DoubleJetC100",
                   																	"hltDoubleJetsC100",
                   																	"hltDoublePFJetsC100",
                   																	"hltDoubleBTagCSV0p9",
                   																	"hltDoublePFJetsC100MaxDeta1p6"},
     		   	   	   	   	  const std::string & path = "MssmHbb/Events/selectedPatTrigger/");

        void loadCorrections();

        //Method that make an event loop
        void applySelection();
        //setup all corrections, ttrees etc
        virtual void setupAnalysis(const std::string & json);
        virtual void runAnalysis(const std::string &json, const std::string &output = "", const int &size = 100);
        // virtual method that create histogrmas
        virtual void makeHistograms(const int &size = 100);
        //Create standart output file name
        void SetupStandardOutputFile(const std::string & outputFileName = "");
        void createOutputFile(const std::string &name );

        struct ScaleFactor{
        	ScaleFactor() : central(0), up(0), down (0) {};
        	ScaleFactor(const double & centr, const double & upper, const double & lower) :
        		central(centr),
        		up(upper),
        		down(lower) {};
        	~ScaleFactor(){};
        	double central;
        	double up;
        	double down;
        };

        //Return std::vector that contains names of all trigger objects
        std::vector<std::string> getTriggerObjectNames();

	protected:

        const bool lowM_;
        std::string triggerLogicName_;
        int nJets_;
        Histograms histo_;
        const bool TEST;
        std::map<std::string, double > weight_;
        std::vector<std::string> triggerObjectName_;
        bool signalMC_ = false;
        pTFile outputFile_;

        //Protected methods
        virtual void fillHistograms(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
//        virtual void fillHistograms(const tools::Jet &LeadingJet, const tools::Jet &sub_LeadingJet);
        virtual void writeHistograms();
//        const virtual bool leadingJetSelection(const int & iJet, const tools::Jet & Jet);
        const virtual bool leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets);
        const virtual bool OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,const analysis::tools::Jet &sLeadOfflineJet);

	private:

        std::unique_ptr<Weights> pWeight_;

        std::map<std::string,pTFile> fCorrections_;
        std::map<std::string,TH1D *>  hCorrections1D_;
        std::map<std::string,TH2D *>  hCorrections2D_;
        std::map<std::string,pSFReader > SFb_;
        std::unique_ptr<BTagCalibration> BTagCalibrationLib_;

        const double pt1_ = 80.;
        const double pt3_ = 30.;
        const double eta1_ = 2.4;
        const double dEta_ = 100.;
        const double dR_ = 1.;

        //Scale Factors calculation
        const ScaleFactor calculateBTagSF(const tools::Jet & Jet, const bool & thirdJ);


	};

	inline std::vector<std::string> JetAnalysisBase::getTriggerObjectNames() {return triggerObjectName_;}

	}
}



#endif /* ANALYSIS_MSSMHBB_SRC_JETANALYSISBASE_H_ */
