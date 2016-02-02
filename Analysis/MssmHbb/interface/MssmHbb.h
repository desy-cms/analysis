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
#include "time.h"
// 
// user include files

#include "Analysis/Tools/interface/Analysis.h"
#include "Analysis/MssmHbb/interface/BasicTree.h"

//
// class declaration
//

namespace analysis {
   namespace mssmhbb {

      class MssmHbb : public analysis::tools::Analysis, public BasicTree {
         public:
            MssmHbb(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~MssmHbb();
           
           bool lowMOnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,const analysis::tools::Jet &sLeadOfflineJet);
           //For double BTag study
           bool OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,const analysis::tools::Jet &sLeadOfflineJet);

           //For triple BTag study
           bool OnlineSelection(const analysis::tools::Jet &fLeadOfflineJet,const analysis::tools::Jet &sLeadOfflineJet,const analysis::tools::Jet &thLeadOfflineJet);
           void addTriggerObjects(const std::vector<std::string> & triggerObjectName = {"hltL1sL1DoubleJetC100",
                      																	"hltDoubleJetsC100",
                      																	"hltDoublePFJetsC100",
                      																	"hltDoubleBTagCSV0p9",
                      																	"hltDoublePFJetsC100MaxDeta1p6"},
        		   	   	   	   	  const std::string & path = "MssmHbb/Events/selectedPatTrigger/");
           void SetupStandardOutputFile(const std::string & outputFileName);

           //returns
           void ShowMCInformation(); // Show MC information
           std::vector<std::string> getTriggerObjectNames();
           std::string getOutPutFileName();
           const char * getTriggerLogicName();
           int getTriggerResult();

         
            // ----------member data ---------------------------
         protected:
           std::vector<std::string> triggerObjectName_;
           std::string outPutName_;
           std::string triggerLogicName_;
           bool lowMSelection_ = false;
               
         private:

           std::string get_date();

      };

      inline std::vector<std::string> MssmHbb::getTriggerObjectNames() {return triggerObjectName_;}
      inline const char * MssmHbb::getTriggerLogicName(){ return triggerLogicName_.c_str();	}
      inline std::string MssmHbb::getOutPutFileName(){ return outPutName_; }
      inline int MssmHbb::getTriggerResult(){ return this->triggerResult(triggerLogicName_.c_str());}
   }
}

#endif  // Analysis_MssmHbb_MssmHbb_h
