/**\class Metadata Metadata.cc MssmHbbAnalysis/Ntuplizer/src/Metadata.cc

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
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "MssmHbbAnalysis/Ntuplizer/interface/Metadata.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

//
// constructors and destructor
//
// Metadata::Metadata()
// {
//    // default constructor
// }

Metadata::Metadata(edm::Service<TFileService> & ) 
{
}

Metadata::~Metadata()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void Metadata::Fill()
{
}

// ------------ method called once each job just before starting event loop  ------------
