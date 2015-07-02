#ifndef MssmHbbAnalysis_Ntuplizer_Metadata_h
#define MssmHbbAnalysis_Ntuplizer_Metadata_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      Metadata
// 
/**\class Metadata Metadata.cc MssmHbbAnalysis/Ntuplizer/src/Metadata.cc

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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {

      class Metadata {
         public:
//            Metadata();
            Metadata(edm::Service<TFileService> & );
            Metadata(TFileDirectory & );
           ~Metadata();
            void Fill();
            TTree * Tree();
            TFileDirectory TreeDir();

         private:
            // ----------member data ---------------------------
            
            TTree * tree_;
            TFileDirectory treeDir_;
            
      };
   }
}

#endif  // MssmHbbAnalysis_Ntuplizer_Metadata_h
