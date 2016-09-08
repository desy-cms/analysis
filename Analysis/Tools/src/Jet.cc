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
   btagAlgo_ = "btag_csvivf";
}
Jet::Jet(const float & pt, const float & eta, const float & phi, const float & e) : Candidate(pt,eta,phi,e,0.) 
{
   extendedFlavour_ = "?";
   btagAlgo_ = "btag_csvivf";
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
float Jet::btag()                                  const { return btags_.at(btagAlgo_);    }                   
float Jet::btag(const std::string & algo)          const { return btags_.at(algo);         }                   
int   Jet::flavour()                               const { return flavour_.at("Hadron");   }                   
int   Jet::flavour(const std::string & definition) const { return flavour_.at(definition); }                   
bool  Jet::idLoose()                               const { return idloose_;                }                   
bool  Jet::idTight()                               const { return idtight_;                }         
float Jet::jecUncert()                             const { return jecUnc_;                 }                   
std::vector<int> Jet::flavours()                   const { return flavours_;               }
std::vector< std::shared_ptr<GenParticle> >\
      Jet::partons()                               const { return partons_;        }
std::string Jet::extendedFlavour()                 const { return extendedFlavour_;        }
float Jet::JerResolution()							const { return jerResolution_;}
float Jet::JerSf() 									const {	return jerSF_; }
float Jet::JerSfDown() 								const { return jerSFDown_; }
float Jet::JerSfUp() 								const { return jerSFUp_; }

// Sets                                                             
void Jet::btag     (const float & btag)                               { btag_    = btag; } 
void Jet::btag     (const std::string & algo, const float & btag)     { btags_[algo]  = btag; } 
void Jet::flavour  (const int   & flav)                               { flavour_["Hadron"] = flav; } 
void Jet::flavour  (const std::string & definition, const int & flav) { flavour_[definition] = flav; } 
void Jet::idLoose  (const bool  & loos)                               { idloose_ = loos; } 
void Jet::idTight  (const bool  & tigh)                               { idtight_ = tigh; } 
void Jet::jecUncert(const float & ju)                                 { jecUnc_  = ju; } 
void Jet::addParton(const std::shared_ptr<GenParticle> & parton)      { partons_.push_back(parton);
                                                                        flavours_.push_back(parton->pdgId());  }
void Jet::btagAlgo (const std::string & algo )                        { btagAlgo_ = algo; }                                                                        
void Jet::JerResolution(const float & jerResolution) 					{ jerResolution_ = jerResolution; }
void Jet::JerSf(const float & jerSf) 									{ jerSF_ = jerSf; }
void Jet::JerSfDown(const float & jerSfDown) 							{ jerSFDown_ = jerSfDown; }
void Jet::JerSfUp(const float & jerSfUp) 								{ jerSFUp_ = jerSfUp; }

int Jet::removeParton(const int & i)
{
   if ( partons_.size() == 1 )
   {
      partons_.clear();
      flavours_.clear();
   }
   else
   {
      partons_.erase(partons_.begin()+i);
      flavours_.erase(flavours_.begin()+i);
   }
   
   // re-do the extendedFlavour
   int flavour = abs(this->flavour());
   int flavCounter = 0;
   
   for ( auto & flav : flavours_ )
      if ( abs(flav) == flavour ) ++flavCounter;
   
//    if (( flavour == 4 || flavour == 5 ) && flavCounter == 0 )
//    {
//       std::cout << "ERROR!!! Jet::removeParton => Original flavour has changed!? " << flavour << "->" << extendedFlavour_ << std::endl;
//       return -1;
//    }

   if ( flavour == 4 && flavCounter > 1 ) extendedFlavour_ == "cc";
   if ( flavour == 5 && flavCounter > 1 ) extendedFlavour_ == "bb";
   
   return 0;
   
}

// ------------ methods  ------------
void Jet::associatePartons(const std::vector< std::shared_ptr<GenParticle> > & particles, const float & dRmax, const float & ptMin,  const bool & pythia8 )
{
   int flavour = abs(this->flavour());
   extendedFlavour_ = "udsg";
   if ( flavour == 5 ) extendedFlavour_ = "b";
   if ( flavour == 4 ) extendedFlavour_ = "c";
   
   int flavCounter = 0;
   for ( auto & particle : particles )
   {
      int pdg = particle->pdgId();
      int status = particle->status();
      float pt = particle->pt();
      if ( pt < ptMin ) continue;
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
      
      if ( abs(pdg) == flavour ) ++flavCounter;
      
   }
   
   // Need to check for ambiguities!!!
   
   // extendedFlavour re-definition
   if ( flavour == 4 && flavCounter > 1 ) extendedFlavour_ = "cc"; 
   if ( flavour == 5 && flavCounter > 1 ) extendedFlavour_ = "bb";
   
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
