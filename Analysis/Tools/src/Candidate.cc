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
   if ( ! cands )
   {
      this -> matched_[name] = NULL;
      std::cout << "Cands are NULL" << std::endl;
      return false;
   }
   
   const Candidate * cand = NULL;
   for ( size_t i = 0; i < cands->size() ; ++i )
   {
      cand = &((*cands)[i]);
      std::cout << name << ": " << (*cands)[i].pt() << "  " << (*cands)[i].eta() << "  " << (*cands)[i].phi() << std::endl;
   }
      std::cout << this->pt() << "  " << this->eta() << "  " << this->phi() << std::endl;
   // if matched,
   this->matched_[name]=cand;
   return true;
}


