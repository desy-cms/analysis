#ifndef Analysis_Tools_Candidate_h
#define Analysis_Tools_Candidate_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Candidate
// 
/**\class Candidate Candidate.cc Analysis/Tools/src/Candidate.cc

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
#include "TLorentzVector.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Candidate {
         public:
            Candidate();
           ~Candidate();
           
           float pt();
           float eta();
           float phi();
           float e();
           int   q();
           TLorentzVector p4();
           
           void set(const float & pt, const float & eta, const float & phi, const float & e, const float & q);

      
         protected:
            // ----------member data ---------------------------
            
            // 
            float pt_ ;
            float eta_;
            float phi_;
            float e_  ;
            int   q_  ;
            
            TLorentzVector p4_;

         private:
      };
   }
}

#endif  // Analysis_Tools_Candidate_h
