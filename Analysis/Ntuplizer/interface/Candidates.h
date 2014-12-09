#ifndef Analysis_Ntuplizer_Candidates_h
#define Analysis_Ntuplizer_Candidates_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Candidates
// 
/**\class Candidates Candidates.cc Analysis/Ntuplizer/src/Candidates.cc

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

namespace mssmhbb {
   namespace ntuple {

      template <typename T>
      class Candidates {
         public:
            Candidates();
            Candidates(const edm::InputTag&, TTree*);
           ~Candidates();
            void candidates(const edm::Event&);
            void branches();
            void fill(const edm::Event&);
            void fill();
            void kinematics();
            static const int maxCandidates = 1000;
      
         protected:
            // ----------member data ---------------------------
            std::vector<T> candidates_;
            std::string configParameter_;
            edm::InputTag input_collection_;
            
            
            // particles kinematics for the ntuple
            int   n_;
            float eta_[maxCandidates];
            float phi_[maxCandidates];
            float pt_[maxCandidates];
            float px_[maxCandidates];
            float py_[maxCandidates];
            float pz_[maxCandidates];
            float e_[maxCandidates];
            
            TTree * tree_;
            
         private:
            bool is_l1jet_;
            bool is_calojet_;
            bool do_kinematics_;
   
            
      };
   
   }
}

#endif  // Analysis_Ntuplizer_Candidates_h
