#ifndef Analysis_Ntuplizer_Vertices_h
#define Analysis_Ntuplizer_Vertices_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Vertices
// 
/**\class Vertices Vertices.cc Analysis/Ntuplizer/src/Vertices.cc

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
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace ntuple {

      class Vertices {
         public:
            Vertices();
            Vertices(const edm::InputTag&, TTree*);
           ~Vertices();
            void ReadFromEvent(const edm::Event&);
            void Fill(const edm::Event&);
            static const int maxPVs = 100;
      
         protected:
            // ----------member data ---------------------------
            reco::VertexCollection candidates_;
            std::string configParameter_;
            edm::InputTag input_collection_;
            
            
            // particles kinematics for the ntuple
            int   n_;
            float x_[maxPVs];
            float y_[maxPVs];
            float z_[maxPVs];
            float xe_[maxPVs];
            float ye_[maxPVs];
            float ze_[maxPVs];
            bool  fake_[maxPVs];
            float chi2_[maxPVs];
            float ndof_[maxPVs];
            float rho_[maxPVs];
            
            TTree * tree_;
            
         private:
      };
   
   }
}

#endif  // Analysis_Ntuplizer_Vertices_h
