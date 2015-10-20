#ifndef Analysis_Tools_TriggerObject_h
#define Analysis_Tools_TriggerObject_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      TriggerObject
//
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
#include <memory>
//
// user include files
#include "Analysis/Tools/interface/Candidate.h"
//
// class declaration
//

namespace analysis {
   namespace tools {

      class TriggerObject : public Candidate {
         public:
            TriggerObject();
            TriggerObject(const float & pt, const float & eta, const float & phi, const float & e);
           ~TriggerObject();
           
         protected:

         private:
            // ----------member data ---------------------------

            //
      };
   }
}

#endif  // Analysis_Tools_TriggerObject_h
