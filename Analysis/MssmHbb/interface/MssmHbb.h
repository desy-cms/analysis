#ifndef Analysis_MssmHbb_MssmHbb_h
#define Analysis_MssmHbb_MssmHbb_h 1

// -*- C++ -*-
//
// Package:    Analysis/MssmHbb
// Class:      Analysis
// 
/**\class Analysis MssmHbb.cc Analysis/MssmHbb/src/MssmHbb.cc

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
   namespace mssmhbb {

      class MssmHbb : public analysis::tools::Analysis {
         public:
            MssmHbb(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~MssmHbb();
           
         
            // ----------member data ---------------------------
         protected:
               
         private:

      };
   }
}

#endif  // Analysis_MssmHbb_MssmHbb_h
