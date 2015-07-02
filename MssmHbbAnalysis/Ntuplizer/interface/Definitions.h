#ifndef MssmHbbAnalysis_Ntuplizer_Definitions_h
#define MssmHbbAnalysis_Ntuplizer_Definitions_h 1

// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      Definitions
// 
/**\class Definitions Definitions.cc MssmHbbAnalysis/Ntuplizer/src/Definitions.cc

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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

//
// class declaration
//

namespace mssmhbb {
   namespace ntuple {

      class Definitions {
         public:
            Definitions();
            Definitions(edm::Service<TFileService> &);
           ~Definitions();
            void Fill();
            void Init();
            void Add(const std::vector<std::string> &, const std::vector<std::string> &);
            void Add(const std::string &, const std::string &);
            TTree * Tree();
      
         private:
            // ----------member data ---------------------------
            
            // Output tree
            TTree * tree_;
            
      };
   }
}

#endif  // MssmHbbAnalysis_Ntuplizer_Definitions_h
