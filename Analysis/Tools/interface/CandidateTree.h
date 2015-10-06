#ifndef Analysis_Tools_CandidateTree_h
#define Analysis_Tools_CandidateTree_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      CandidateTree
// 
/**\class CandidateTree CandidateTree.cc Analysis/Tools/src/CandidateTree.cc

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
#include "Analysis/Tools/interface/CandidateCollection.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      template <typename T>
      class CandidateTree {
         public:
            CandidateTree();
            CandidateTree(TChain * tree);
           ~CandidateTree();
           
           int size();
           void event(const int & event);
      
            // ----------member data ---------------------------
         protected:
            static const int max_ = 1000;
            
            // 
            int   n_   ;
            float pt_  [max_];
            float eta_ [max_];
            float phi_ [max_];
            float e_   [max_];
            int   q_   [max_];
            
            float px_  [max_];
            float py_  [max_];
            float pz_  [max_];
            
            TChain * tree_;
         private:

      };
   }
}

#endif  // Analysis_Tools_CandidateTree_h
