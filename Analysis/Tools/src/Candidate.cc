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
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/Candidate.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Candidate::Candidate() : pt_(-1.), eta_(0.), phi_(0.), e_(0.), q_(0.) 
{
   p4_.SetPtEtaPhiE(0,0,0,0);
}

Candidate::~Candidate()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ methods  ------------
float Candidate::pt()
{
   return pt_;
}
float Candidate::eta()
{
   return eta_;
}
float Candidate::phi()
{
   return phi_;
}
float Candidate::e()
{
   return e_;
}
int   Candidate::q()
{
   return q_;
}
TLorentzVector Candidate::p4()
{
   return p4_;
}

void Candidate::set(const float & pt, const float & eta, const float & phi, const float & e, const float & q)
{
   pt_  = pt;
   eta_ = eta;
   phi_ = phi;
   e_   = e;
   q_   = q;
   
   if ( pt_ < 0 )
      p4_.SetPtEtaPhiE(0,0,0,0);
   else
      p4_.SetPtEtaPhiE(pt_,eta_,phi_,e_);

}
