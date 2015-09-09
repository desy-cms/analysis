/**\class Muons Muons.cc Analysis/Tools/src/Muons.cc

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
#include <iostream>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/Muons.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Muons::Muons(TChain * tree) : Candidates(tree)
{
}

Muons::~Muons()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------

Muon Muons::at(const int& i)
{
   using namespace edm;
   
   Muon mu;
   
   if ( i >= n_ || pt_ < 0 )
   {
      return mu;
   }
   
   mu.set(pt_[i],eta_[i],phi_[i],e_[i],q_[i]);
   
   return mu;
   
}
