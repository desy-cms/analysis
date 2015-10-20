/**\class TriggerObject TriggerObject.cc Analysis/Tools/src/TriggerObject.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Rostyslav Shevchenko
//         Created:  Fri, 08 Oct 2015 17:56:18 GMT
//
//

// system include files
//
// user include files
#include "FWCore/Framework/interface/Event.h"
//
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Analysis/Tools/interface/TriggerObject.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
TriggerObject::TriggerObject() : Candidate()
{
}
TriggerObject::TriggerObject(const float & pt, const float & eta, const float & phi, const float & e) :
      Candidate(pt,eta,phi,e,0)
{
}
TriggerObject::~TriggerObject()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
