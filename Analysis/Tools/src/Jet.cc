/**\class Jet Jet.cc Analysis/Tools/src/Jet.cc

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
 
#include "Analysis/Tools/interface/Jet.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Jet::Jet() : Candidate() 
{
   extendedFlavour_ = "?";
}
Jet::Jet(const float & pt, const float & eta, const float & phi, const float & e) : Candidate(pt,eta,phi,e,0.) 
{
   extendedFlavour_ = "?";
}
Jet::~Jet()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// Gets
float Jet::btag()                                  const { return btag_;                   }                   
int   Jet::flavour()                               const { return flavour_.at("Hadron");   }                   
int   Jet::flavour(const std::string & definition) const { return flavour_.at(definition); }                   
bool  Jet::idLoose()                               const { return idloose_;                }                   
bool  Jet::idTight()                               const { return idtight_;                }         
float Jet::jecUncert()                             const { return jecUnc_;                 }                   
std::vector<int> Jet::flavours()                   const { return flavours_;               }
std::vector< std::shared_ptr<GenParticle> > Jet::partons() const { return partons_;        }
std::string Jet::extendedFlavour()                 const { return extendedFlavour_;        }

// Sets                                                             
void Jet::btag     (const float & btag)                               { btag_    = btag; } 
void Jet::flavour  (const int   & flav)                               { flavour_["Hadron"] = flav; } 
void Jet::flavour  (const std::string & definition, const int & flav) { flavour_[definition] = flav; } 
void Jet::idLoose  (const bool  & loos)                               { idloose_ = loos; } 
void Jet::idTight  (const bool  & tigh)                               { idtight_ = tigh; } 
void Jet::jecUncert(const float & ju)                                 { jecUnc_  = ju; } 
void Jet::addParton(const std::shared_ptr<GenParticle> & parton)      { partons_.push_back(parton);
                                                                        flavours_.push_back(parton->pdgId());  }

// ------------ methods  ------------
void Jet::associatePartons(const std::vector< std::shared_ptr<GenParticle> > & particles, const float & dRmax, const bool & pythia8 )
{
   int flavour = this->flavour();
   extendedFlavour_ = "udsg";
   if ( abs(flavour) == 5 ) extendedFlavour_ = "b";
   if ( abs(flavour) == 4 ) extendedFlavour_ = "c";
   
   int flavCounter = 0;
   for ( auto & particle : particles )
   {
      int pdg = particle->pdgId();
      int status = particle->status();
      if ( pythia8 )
      {
         if ( status != 71 && status != 72 ) continue;
      }
      else
      {
         if ( status != 3 ) continue;
      }
      if ( abs(pdg) > 5 && pdg != 21 ) continue;
      if ( this->p4().DeltaR(particle->p4()) > dRmax ) continue;
      
      addParton (particle);
      
      if ( abs(pdg) == abs(flavour) ) ++flavCounter;
      
   }
   
   // Need to check for ambiguities!!!
   
   // extendedFlavour re-definition
   if ( flavour == 4 && flavCounter > 1 ) extendedFlavour_ == "cc";
   if ( flavour == 5 && flavCounter > 1 ) extendedFlavour_ == "bb";
   
}
                                                                        
                                                                        
                                                                        
void Jet::id      (const float & nHadFrac,
                   const float & nEmFrac ,
                   const float & nMult   ,
                   const float & cHadFrac,
                   const float & cEmFrac ,
                   const float & cMult   ,
                   const float & muFrac  )
{
   // Jet ID
   int nM = (int)round(nMult);
   int cM = (int)round(cMult);
   int numConst = nM + cM;
   if ( fabs(p4_.Eta()) <= 3. )
   {
      idloose_ = ((nHadFrac<0.99 && nEmFrac<0.99 && numConst>1) && ((abs(p4_.Eta())<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || fabs(p4_.Eta())>2.4) && fabs(p4_.Eta())<=3.0);
      idtight_ = ((nHadFrac<0.90 && nEmFrac<0.90 && numConst>1) && ((abs(p4_.Eta())<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || fabs(p4_.Eta())>2.4) && fabs(p4_.Eta())<=3.0);
   }
   else
   {
      idloose_ = (nEmFrac<0.90 && nM>10 && fabs(p4_.Eta())>3.0);
      idtight_ = (nEmFrac<0.90 && nM>10 && fabs(p4_.Eta())>3.0);
   }   
}
      
      
