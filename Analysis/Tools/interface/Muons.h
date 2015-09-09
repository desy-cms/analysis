#ifndef Analysis_Tools_Muons_h
#define Analysis_Tools_Muons_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Muons
// 
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
#include <memory>
#include <vector>
// 
// user include files

#include "TTree.h"
#include "TChain.h"
#include "Analysis/Tools/interface/Candidates.h"
#include "Analysis/Tools/interface/Muon.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Muons : public Candidates {
         public:
            Muons(TChain *);
           ~Muons();
           
            Muon at(const int &);
           
         private:
            // ----------member data ---------------------------
      };
   }
}

#endif  // Analysis_Tools_Muons_h
