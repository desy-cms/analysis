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
#include "TBranch.h"
#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace mssmhbb {

      class TriggerEfficiency : public analysis::tools::Analysis {
         public:
            TriggerEfficiency(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~TriggerEfficiency();
           
           void addTriggerObjects(const std::vector<std::string> & triggerObjectName = {"hltL1sL1ZeroBias",
           																				"hltSingleCaloJet10",
           																				"hltSinglePFJet40",
           																				"hltL1sL1SingleJet36",
           																				"hltSingleCaloJet40",
           																				"hltSinglePFJet60",
           																				"hltL1sL1SingleJet52",
           																				"hltSingleCaloJet50",
           																				"hltSinglePFJet80"}, const std::string & path = "MssmHbb/Events/selectedPatTrigger/");
           																				
           void setBranches();
           void zeroingBranches();
           void fillTree();
           void writeTree();

           bool matchToPF100(const analysis::tools::Jet & jet);
           bool matchToPF80(const analysis::tools::Jet & jet);
           bool matchToPF60(const analysis::tools::Jet & jet);

           //returns:
           std::vector<std::string> triggerObjectNames();
           TTree *OutTree;


           // Jet variables:
           double LeadPt[4];
           double LeadEta[4];
           double LeadPhi[4];
           double LeadBTag[4];

           double dPhiFS 	= 0;
           double dEtaFS 	= 0;
           int Njets = 0;
           int Njets80 = 0;

           // Reference efficiency variables:
           double hpf60for100_Num = 0;
           double hpf60for100_Denum = 0;
           double hpf60for80_Num = 0;
           double hpf60for80_Denum = 0;
           double hpf100_Num[4];
           double hpf100_Denum[4];

           // T&P efficiency variables:
           double TnP40_probe = 0;
           double TnP40_tag = 0;
           double TnP60_tag = 0;
           double TnP60_probe = 0;
           double TnP80_tag = 0;
           double TnP80_probe = 0;
           double TnP100_tag[4];
           double TnP100_probe[4];

           //Trigger Matching variables:
           int LeadMatch60[4];
           int LeadMatch80[4];
           int LeadMatch100[4];

           //Trigger Object variables:
           	   //L1
           int NL1Object = 0;
           	   //L2
           int NL2Object = 0;
           	   //L3
           int NL3Object = 0;
           //Other variables
           double ptVeto = 0;
           double M12 = 0;

           //Only MC variables:
           double mcModel_Num[4];
           double mcModel_Denum[4];
           double weightPtTnP = 0;
           double weightPtRef = 0;
           double weightPtComb = 0;
           double weightLumi = 0;


         
            // ----------member data ---------------------------
         protected:
         	std::vector<std::string> triggerObjectName_;
         	
         private:
         
         
         

      };
      
      //................................
      inline std::vector<std::string> TriggerEfficiency::triggerObjectNames() {return triggerObjectName_;}
      
   }
}

#endif  // Analysis_MssmHbb_TriggerEfficiency_h
