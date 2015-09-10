/**\class METs METs.cc Analysis/Tools/src/METs.cc

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
 
#include "Analysis/Tools/interface/METs.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
METs::METs(TChain * tree) : Candidates(tree)
{
   tree  -> SetBranchAddress( "sigxx",   sigxx_ );
   tree  -> SetBranchAddress( "sigxy",   sigxy_ );
   tree  -> SetBranchAddress( "sigyx",   sigyx_ );
   tree  -> SetBranchAddress( "sigyy",   sigyy_ );

}

METs::~METs()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------

MET METs::at(const int& i)
{
   using namespace edm;
   
   MET met;
   
   if ( i >= 1 || pt_ < 0 )
   {
      return met;
   }
   
   met.momentum3(px_[i],py_[i],pz_[i]);
   met.pt(pt_[i]);
   met.phi(phi_[i]);
   met.significanceMatrix(sigxx_[i],sigxy_[i],sigyx_[i],sigyy_[i]);
   
   return met;
   
}
