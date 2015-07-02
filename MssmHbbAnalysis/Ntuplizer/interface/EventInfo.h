#ifndef MssmHbbAnalysis_Ntuplizer_EventInfo_h
#define MssmHbbAnalysis_Ntuplizer_EventInfo_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      EventInfo
// 
/**\class EventInfo EventInfo.cc MssmHbbAnalysis/Ntuplizer/src/EventInfo.cc

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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {

      class EventInfo {
         public:
            EventInfo();
            EventInfo(edm::Service<TFileService> &);
           ~EventInfo();
            void Fill(const edm::Event&);
            void Init();
            TTree * Tree();
      
         private:
            // ----------member data ---------------------------
            
            // event information
            int event_;
            int run_;
            int lumi_;
            int bx_;
            int orbit_;
            
            // Output tree
            TTree * tree_;
            
      };
   }
}

#endif  // MssmHbbAnalysis_Ntuplizer_EventInfo_h
