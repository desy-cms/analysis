#ifndef Analysis_Ntuplizer_TriggerAccepts_h
#define Analysis_Ntuplizer_TriggerAccepts_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      TriggerAccepts
// 
/**\class TriggerAccepts TriggerAccepts.cc Analysis/Ntuplizer/src/TriggerAccepts.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 20 Oct 2014 14:24:08 GMT
//
//

// system include files
#include <memory>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace ntuple {

      class TriggerAccepts {
         public:
            TriggerAccepts();
            TriggerAccepts(const edm::InputTag&, TTree*, const std::vector<std::string> &, const bool & testmode = false);
           ~TriggerAccepts();
            void Fill(const edm::Event & event, const edm::EventSetup & setup);
            void Branches();
            void LumiBlock(edm::LuminosityBlock const & , edm::EventSetup const& );
      
         private:
            // ----------member data ---------------------------
            
            edm::InputTag input_collection_;
            HLTConfigProvider hlt_config_;
            std::vector<std::string> paths_;
            std::vector<std::string> inpaths_;
            bool accept_[1000];
            int psl1_[1000];
            int pshlt_[1000];
            
            bool first_;
            bool testmode_;
            
            TTree * tree_;
            
      };
   }
}

#endif  // Analysis_Ntuplizer_TriggerAccepts_h
