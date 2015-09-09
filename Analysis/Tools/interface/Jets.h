#ifndef Analysis_Tools_Jets_h
#define Analysis_Tools_Jets_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Jets
// 
/**\class Jets Jets.cc Analysis/Tools/src/Jets.cc

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
#include "Analysis/Tools/interface/Jet.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Jets : public Candidates {
         public:
            Jets(TChain *, std::string btagging = "btag_csvivf");
           ~Jets();
           
            Jet at(const int &);
           
            float btag();
           
         private:
            // ----------member data ---------------------------
            std::string btagging_;
            float btag_    [max_];
            int   flavour_ [max_];
            float nHadFrac_[max_];
            float nEmFrac_ [max_];
            float nMult_   [max_];
            float cHadFrac_[max_];
            float cEmFrac_ [max_];
            float cMult_   [max_];
            float muFrac_  [max_];
         
         
         
               
      };
   }
}

#endif  // Analysis_Tools_Jets_h
