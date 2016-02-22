#ifndef Analysis_MssmHbb_TriggerEfficiency_h
#define Analysis_MssmHbb_TriggerEfficiency_h 1

// -*- C++ -*-
//
// Package:    Analysis/MssmHbb
// Class:      TriggerEfficiency
// 
/**\class TriggerEfficiency TriggerEfficiency.cc Analysis/MssmHbb/src/TriggerEfficiency.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Rostyslav Shevchenko
//         Created:  Wed, 25 Nov 2015 14:24:08 GMT
//
//

// system include files
#include <memory>
#include <vector>
#include <string>
// 
// user include files

#include "Analysis/Tools/interface/Analysis.h"
#include "Analysis/MssmHbb/interface/MssmHbb.h"
#include "TBranch.h"
#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace mssmhbb {

      class TriggerEfficiency : public MssmHbb {
         public:
            TriggerEfficiency(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~TriggerEfficiency();
           
           //Set Output Branches
           void setBranches();

           //Clean variables
           void cleanVariables();

           //Set Number of The trigger Objects in the event
           void setTriggerObjectVars();
           bool matchToPF60(const analysis::tools::Jet &);
           bool matchToPF80(const analysis::tools::Jet &);
           bool matchToPF100(const analysis::tools::Jet &);
           bool matchToPF160(const analysis::tools::Jet &);
           bool matchToPF100dEta1p6(const analysis::tools::Jet &, const analysis::tools::Jet &);

           void setTopology(const bool & topology);

         protected:
           int NTrigObj_[20];
           int LeadMatch60_[20];
           int LeadMatch80_[20];
           int LeadMatch100_[20];
           int LeadMatch160_[20];
           int LeadMatch100dEta1p6_[20];

           int PFJet80_;
           int PFJet60_;

           int doubleJetTolopogy_;

         

      };
      
      inline void TriggerEfficiency::setTopology(const bool & topology){ doubleJetTolopogy_ = topology;}

   }
}

#endif  // Analysis_MssmHbb_TriggerEfficiency_h
