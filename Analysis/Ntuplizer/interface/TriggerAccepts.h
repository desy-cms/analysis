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
            TriggerAccepts(const edm::InputTag&, TTree*, const std::vector<std::string> &);
           ~TriggerAccepts();
            void Fill(const edm::Event&);
            void Branches();
            void LumiBlock(edm::LuminosityBlock const & , edm::EventSetup const& );
      
         private:
            // ----------member data ---------------------------
            
            edm::InputTag input_collection_;
            HLTConfigProvider hlt_config_;
            std::vector<std::string> paths_;
            bool accept_[32];
            
            TTree * tree_;
            
      };
   }
}

#endif  // Analysis_Ntuplizer_TriggerAccepts_h
