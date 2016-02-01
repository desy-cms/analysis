#ifndef Analysis_Objects_Btagging_h
#define Analysis_Objects_Btagging_h 1

// -*- C++ -*-
//
// Package:    Analysis/Objects
// Class:      Analysis
// 
/**\class Analysis Btagging.cc Analysis/Objects/src/Btagging.cc

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
#include <memory>
#include <vector>
#include <string>
// 
// user include files

#include "Analysis/Tools/interface/Analysis.h"

//
// class declaration
//

namespace analysis {
   namespace objects {

      class Btagging : public analysis::tools::Analysis {
         public:
            Btagging(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~Btagging();
           
         
            // ----------member data ---------------------------
         protected:
               
         private:

      };
   }
}

#endif  // Analysis_Objects_Btagging_h
