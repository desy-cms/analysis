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
   bool status = false;
   
   
   if ( ! cands )
   {
      this -> matched_[name] = nullptr;
      return status;
   }

   const Candidate * cand = nullptr;
   const Candidate * nearcand = nullptr;
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
     status = true;
   }

   else {
     this -> matched_[name] = nullptr;
   }
   
   return status;
}

bool Candidate::matchTo(const std::vector<Candidate> * cands, const std::string & name, const float & delta_pT, const float & deltaR)
{
   bool status = false;


   if ( ! cands )
   {
      this -> matched_[name] = nullptr;
      return status;
   }

   const Candidate * cand = nullptr;
   const Candidate * nearcand = nullptr;
   float minDeltaR = deltaR + 1; 		// Assign more real value;
   float dpT = 0.;
   float dpTmin = delta_pT + 1;
   for ( size_t i = 0; i < cands->size() ; ++i )
   {
      cand = &(cands->at(i));
      dpT = std::abs(this->pt() - cand->pt());
      if(this->deltaR(*cand) < minDeltaR && dpT < dpTmin)
      {
         minDeltaR = this->deltaR(*cand);
         dpTmin    = dpT;
         nearcand = cand;
      }
   }

   if(minDeltaR < deltaR && dpTmin < delta_pT)
   {
     this->matched_[name]=nearcand;
     status = true;
   }

   else {
     this -> matched_[name] = nullptr;
   }

   return status;
}

// Gets
float Candidate::px()   const { return p4_.Px() ; }
float Candidate::py()   const { return p4_.Py() ; }
float Candidate::pz()   const { return p4_.Pz() ; }
float Candidate::pt()   const { return p4_.Pt() ; }
float Candidate::eta()  const { return p4_.Eta(); }
float Candidate::phi()  const { return p4_.Phi(); }
float Candidate::e()    const { return p4_.E()  ; }
float Candidate::m()    const { return p4_.M()  ; }
float Candidate::mass() const { return p4_.M()  ; }
int   Candidate::q()    const { return q_;   }
float Candidate::deltaR(const Candidate &cand) const { return p4_.DeltaR(cand.p4()) ;}

TLorentzVector Candidate::p4() const { return p4_; }
TVector3       Candidate::p3() const { return p4_.Vect(); }


const Candidate * Candidate::matched(const std::string & name) { return matched_[name]; }
const Candidate * Candidate::matched(const std::string & name) const { return matched_.find(name) != matched_.end() ? matched_.find(name)->second : 0; }

// Sets
void  Candidate::p4(const TLorentzVector & p4) { p4_ = p4; }
void  Candidate::px(const float & px) { p4_.SetPx(px); }
void  Candidate::py(const float & py) { p4_.SetPy(py); }
void  Candidate::pz(const float & pz) { p4_.SetPy(pz); }
void  Candidate::e (const float & e ) { p4_.SetE(e);   }
void  Candidate::q (const float & q)  { q_ = q; }

