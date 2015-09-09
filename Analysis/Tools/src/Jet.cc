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
}
Jet::~Jet()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ methods  ------------
float Jet::btag()    { return btag_;    }
int   Jet::flavour() { return flavour_; }
bool  Jet::idLoose() { return idLoose_; }
bool  Jet::idTight() { return idTight_; }

void Jet::btag    (const float & btag) { btag_    = btag; }
void Jet::flavour (const int   & flav) { flavour_ = flav; }
void Jet::idLoose (const bool  & loos) { idLoose_ = loos; }
void Jet::idTight (const bool  & tigh) { idTight_ = tigh; }
