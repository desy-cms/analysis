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
#include <map>
#include <iostream>
//
// user include files
#include "TLorentzVector.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Candidate {
         typedef std::vector<Candidate> Candidates;
         public:
            Candidate();
            Candidate(const float & pt, const float & eta, const float & phi, const float & e, const float & q = 0);
            Candidate(const float & px, const float & py, const float & pz);
           ~Candidate();

           float px()   const;
           float py()   const;
           float pz()   const;
           float pt()   const;
           float eta()  const;
           float phi()  const;
           float e()    const;
           float m()    const;
           float mass() const;
           inline float deltaR(const Candidate & cand) const;
           int   q()   const;
           void  q(const float & q);
           TLorentzVector p4() const;
           TVector3       p3() const;

           Candidate candidate();

           // made below virtual as this may be different for MET, or vertex
           virtual bool matchTo(const std::vector<Candidate> * cands, const std::string & name, const float & deltaR = 0.5);
           const Candidate * matched(const std::string & name);
           const Candidate * matched(const std::string & name) const;
         protected:
            // ----------member data ---------------------------

            //
            int   q_  ;
            std::string name;
            TLorentzVector p4_;

            std::map<std::string, const Candidate * > matched_;

         private:
      };

      // ===============================================
      // INLINE IMPLEMENTATIONS

      // Gets
      inline float Candidate::px()   const { return p4_.Px() ; }
      inline float Candidate::py()   const { return p4_.Py() ; }
      inline float Candidate::pz()   const { return p4_.Pz() ; }
      inline float Candidate::pt()   const { return p4_.Pt() ; }
      inline float Candidate::eta()  const { return p4_.Eta(); }
      inline float Candidate::phi()  const { return p4_.Phi(); }
      inline float Candidate::e()    const { return p4_.E()  ; }
      inline float Candidate::m()    const { return p4_.M()  ; }
      inline float Candidate::mass() const { return p4_.M()  ; }
      inline int   Candidate::q()    const { return q_;   }
      inline float Candidate::deltaR(const Candidate &cand) const { return p4_.DeltaR(cand.p4()) ;}

      inline TLorentzVector Candidate::p4() const { return p4_; }
      inline TVector3       Candidate::p3() const { return p4_.Vect(); }

      inline Candidate Candidate::candidate()
      {
         Candidate cand(this->pt(), this->eta(), this->phi(), this->e(), this->q());
         return cand;
      }

      inline const Candidate * Candidate::matched(const std::string & name) { return matched_[name]; }
      inline const Candidate * Candidate::matched(const std::string & name) const { return matched_.find(name) != matched_.end() ? matched_.find(name)->second : 0; }

      // Sets
      inline void  Candidate::q(const float & q) { q_ = q; }

   }
}

#endif  // Analysis_Tools_Candidate_h
