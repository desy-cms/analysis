#ifndef MssmHbbAnalysis_Ntuplizer_FilterEfficiency_h
#define MssmHbbAnalysis_Ntuplizer_FilterEfficiency_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      FilterEfficiency
// 
/**\class FilterEfficiency FilterEfficiency.cc MssmHbbAnalysis/Ntuplizer/src/FilterEfficiency.cc

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
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {
      
      struct FilterResults {
         unsigned int total;
         unsigned int filtered;
         double efficiency;
      };

      template <typename T>
      class FilterEfficiency {
         public:
            FilterEfficiency(edm::Service<TFileService> &, const std::vector<edm::InputTag> & collections = std::vector<edm::InputTag>() );
            FilterEfficiency(TFileDirectory &, const std::vector<edm::InputTag> & collections = std::vector<edm::InputTag>());
           ~FilterEfficiency();
            void SetCollections(const std::vector<edm::InputTag> &);
            void Increment(edm::LuminosityBlock const& );
            FilterResults Results();
            TTree * Tree();
            void Fill();
      
         private:
            // ----------member data ---------------------------
            unsigned int    nTotal_;
            unsigned int    nFiltr_;
            double          efficiency_;
            std::vector<edm::InputTag> collections_;
            
            // Output tree
            TTree * tree_;
      };         
   }
}

#endif  // MssmHbbAnalysis_Ntuplizer_FilterEfficiency_h
