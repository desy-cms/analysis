/**\class Candidate Candidate.cc Analysis/Tools/src/Candidate.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 09 Sep 2015 09:24:08 GMT
//
//

// system include files
//
#include <iostream>
// user include files
#include "Analysis/Tools/interface/Candidate.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Candidate::Candidate()
{
   q_ = 0;
   p4_.SetPtEtaPhiE(0.,0.,0.,0.);
}

Candidate::Candidate(const float & pt, const float & eta, const float & phi, const float & e, const float & q)
{
   q_ = q;
   p4_.SetPtEtaPhiE(pt,eta,phi,e);
}

Candidate::Candidate(const float & px, const float & py, const float & pz)
{
   q_ = 0;
   p4_.SetXYZM(px,py,pz,0.);
}


Candidate::~Candidate()
{
}


//
// member functions
//
bool Candidate::matchTo(const std::vector<Candidate> * cands, const std::string & name, const float & deltaR)
{
   this -> matched_[name] = NULL;
   if ( ! cands )
      return false;

   const Candidate * cand = NULL;
   const Candidate * nearcand = NULL;
   float minDeltaR = 100.;
   for ( size_t i = 0; i < cands->size() ; ++i )
   {
      cand = &(cands->at(i));
      if(this->deltaR(*cand) < minDeltaR)
      {
         minDeltaR = this->deltaR(*cand);
         nearcand = cand;
      }
   }

   if(minDeltaR < deltaR)
   {
     this->matched_[name]=nearcand;
     return true;
   }

   else {
     return false;
   }
}
