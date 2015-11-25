#ifndef Analysis_Tools_GenParticle_h
#define Analysis_Tools_GenParticle_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      GenParticle
// 
/**\class GenParticle GenParticle.cc Analysis/Tools/src/GenParticle.cc

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
#include "Analysis/Tools/interface/Candidate.h"
//
// class declaration
//

namespace analysis {
   namespace tools {

      class GenParticle : public Candidate {
         public:
            GenParticle();
            GenParticle(const float & pt, const float & eta, const float & phi, const float & e, const float & q);
           ~GenParticle();
//            using Candidate::set; // in case needed to overload the function set
            void pdgId(const int & pdgId);
            void status(const int & status);
            void higgsDaughter(const bool & higgs_dau);
            int pdgId();
            int status();
            bool higgsDaughter();
      
         private:
            // ----------member data ---------------------------
            int  pdgid_;
            int  status_;
            bool higgs_dau_;
            // 
      };
      // ===============================================
      // INLINE IMPLEMENTATIONS
         
      // Gets
      inline int  GenParticle::pdgId()    { return pdgid_;    }                   
      inline int  GenParticle::status()   { return status_;    }                   
      inline bool GenParticle::higgsDaughter()   { return higgs_dau_;    }                   
                
      // Sets                                                             
      inline void GenParticle::pdgId  (const int & pdgId)  { pdgid_  = pdgId; } 
      inline void GenParticle::status (const int & status) { status_ = status; } 
      inline void GenParticle::higgsDaughter (const bool & higgs_dau) { higgs_dau_ = higgs_dau; } 
   }
}

#endif  // Analysis_Tools_GenParticle_h
