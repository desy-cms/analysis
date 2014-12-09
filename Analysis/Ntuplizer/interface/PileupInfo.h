#ifndef MssmHbbAnalysis_Ntuplizer_PileupInfo_h
#define MssmHbbAnalysis_Ntuplizer_PileupInfo_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      PileupInfo
// 
/**\class PileupInfo PileupInfo.cc MssmHbbAnalysis/Ntuplizer/src/PileupInfo.cc

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
 
#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {

      class PileupInfo {
         public:
            PileupInfo();
            PileupInfo(const edm::InputTag&, TTree*);
           ~PileupInfo();
            void pileupInfo(const edm::Event&);
            void branches();
            void fill(const edm::Event&);
            void fill();
            static const int maxParticles = 1000;
      
         protected:
            // ----------member data ---------------------------
            edm::InputTag input_collection_;
            // 
            float n_true_pu_;
            float n_pu_;
            
            TTree * tree_;
            
      };
   }   
}

#endif  // MssmHbbAnalysis_Ntuplizer_PileupInfo_h
