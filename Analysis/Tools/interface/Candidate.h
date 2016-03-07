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
            /// default constructor
            Candidate();
            /// constructor from 4-momentum information
            Candidate(const float & pt, const float & eta, const float & phi, const float & e, const float & q = 0);
            /// constructor from 3-momentum information
            Candidate(const float & px, const float & py, const float & pz);
            /// destructor
           ~Candidate();
            
           // Get
           /// returns the x component of the momentum
           float px()          const;
           /// returns the y component of the momentum
           float py()          const;
           /// returns the z component of the momentum
           float pz()          const;
           /// returns the transverse momentum
           float pt()          const;
           /// returns the pseudorapidity
           float eta()         const;
           /// returns the azimuthal angle
           float phi()         const;
           /// returns the energy
           float e()           const;
           /// returns the mass
           float m()           const;
           /// returns the mass
           float mass()        const;
           /// returns the charge
           int   q()           const;
           /// returns the 4-momentum (TLorentzVector)
           TLorentzVector p4() const;
           /// returns the 4-momentum (TVector3)
           TVector3       p3() const;
           
           // Set
           /// sets the 4-momentum (TLorentzVector)
           void p4  (const TLorentzVector &);
           /// sets the x component of the momentum
           void px  (const float &);
           /// sets the y component of the momentum
           void py  (const float &);
           /// sets the z component of the momentum
           void pz  (const float &);
           /// sets the energy
           void e   (const float &);
           /// sets the charge
           void q   (const float &);
           
           /// returns the deltaR between this and another candidate
           float deltaR(const Candidate & ) const;
           

           // made below virtual as this may be different for MET, or vertex
           /// function to match this candidate to another object from a list of pointers with a name
           virtual bool matchTo(const std::vector<Candidate> * cands, const std::string & name, const float & deltaR = 0.5);
           /// returns the pointer to the matched candidate object
           const Candidate * matched(const std::string & name);
           /// returns the pointer to the matched candidate object
           const Candidate * matched(const std::string & name) const;
         protected:
            // ----------member data ---------------------------

            //
            /// the charge
            float   q_  ;
            /// the 4-momentum
            TLorentzVector p4_;
            /// map of matched candidates
            std::map<std::string, const Candidate * > matched_;

         private:
      };
   }
}

#endif  // Analysis_Tools_Candidate_h
