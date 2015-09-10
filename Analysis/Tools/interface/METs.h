#ifndef Analysis_Tools_METs_h
#define Analysis_Tools_METs_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      METs
// 
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
#include <memory>
#include <vector>
// 
// user include files

#include "TTree.h"
#include "TChain.h"
#include "Analysis/Tools/interface/Candidates.h"
#include "Analysis/Tools/interface/MET.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class METs : public Candidates {
         public:
            METs(TChain *);
           ~METs();
           
            MET at(const int &);
           
         private:
            // ----------member data ---------------------------
            float sigxx_ [max_];
            float sigxy_ [max_];
            float sigyx_ [max_];
            float sigyy_ [max_];
         
            float gen_px_ [max_];
            float gen_py_ [max_];
            float gen_pz_ [max_];
         
         
               
      };
   }
}

#endif  // Analysis_Tools_METs_h
