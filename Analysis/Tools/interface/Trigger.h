#ifndef Analysis_Tools_Trigger_h
#define Analysis_Tools_Trigger_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Trigger
//
/**\class Trigger Trigger.cc Analysis/Tools/src/Trigger.cc

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

      class Trigger : public Candidate {
         public:
            Trigger();
            Trigger(const float & pt, const float & eta, const float & phi, const float & e);
           ~Trigger();

           bool fired();
           void fired(const bool &);

         protected:

           bool fired_;

         private:
            // ----------member data ---------------------------

            //
      };

      // INLINE implementation

      //Get
      inline bool Trigger::fired() { return fired_; }
      //Set
      inline void Trigger::fired(const bool & fired) { fired_ = fired; }
   }
}

#endif  // Analysis_Tools_Trigger_h
