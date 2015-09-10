#ifndef Analysis_Ntuplizer_JetsTags_h
#define Analysis_Ntuplizer_JetsTags_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      JetsTags
// 
/**\class JetsTags JetsTags.cc Analysis/Ntuplizer/src/JetsTags.cc

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

#include "TTree.h"

//
// class declaration
//

namespace analysis {
   namespace ntuple {

      class JetsTags {
         public:
            JetsTags();
            JetsTags(const edm::InputTag&, TTree*, float minPt = 20., float minEta = 10.);
           ~JetsTags();
            void ReadFromEvent(const edm::Event&);
            void Branches();
            void Fill(const edm::Event&);
            void Fill();
            void Tags();
            void MinPt(const float&);
            void MaxEta(const float&);
            static const int maxCandidates = 100;
      
         protected:
            // ----------member data ---------------------------
            reco::JetTagCollection candidates_;
            std::string configParameter_;
            edm::InputTag input_collection_;
            
            
            // particles kinematics for the ntuple
            int   n_;
            float eta_[maxCandidates];
            float phi_[maxCandidates];
            float pt_[maxCandidates];
            float e_[maxCandidates];
            
            // Tag
            float btag_[maxCandidates];
            
            TTree * tree_;
            
         private:
            float minPt_;
            float maxEta_;
   
            
      };
   
   }
}

#endif  // Analysis_Ntuplizer_JetsTags_h
