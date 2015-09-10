#ifndef Analysis_Ntuplizer_Definitions_h
#define Analysis_Ntuplizer_Definitions_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Definitions
// 
/**\class Definitions Definitions.cc Analysis/Ntuplizer/src/Definitions.cc

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

namespace analysis {
   namespace ntuple {

      class Definitions {
         public:
            Definitions();
            Definitions(edm::Service<TFileService> &);
            Definitions(edm::Service<TFileService> &, const std::string &);
            Definitions(TFileDirectory &);
            Definitions(TFileDirectory &, const std::string &);
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

#endif  // Analysis_Ntuplizer_Definitions_h
