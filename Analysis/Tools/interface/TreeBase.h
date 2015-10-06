#ifndef Analysis_Tools_TreeBase_h
#define Analysis_Tools_TreeBase_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      TreeBase
// 
/**\class TreeBase TreeBase.cc Analysis/Tools/src/TreeBase.cc

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
// #include "Analysis/Tools/interface/Jet.h"
// #include "Analysis/Tools/interface/MET.h"
// #include "Analysis/Tools/interface/Muon.h"
// #include "Analysis/Tools/interface/Vertex.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

//       template <typename Object>
      class TreeBase {
         public:
            TreeBase();
            TreeBase(TChain * tree, const std::string & name);
           ~TreeBase();
           
           void event(const int & event);
           TChain * tree();
           
            // ----------member data ---------------------------
         protected:
            TChain * tree_;
            std::string className_;
            std::string inputTag_;
            
            std::vector<std::string> branches_;
            
            std::string name_;
            
         private:

      };
         
   }
}

#endif  // Analysis_Tools_TreeBase_h
