#ifndef Analysis_Tools_PhysicsObjectTreeBase_h
#define Analysis_Tools_PhysicsObjectTreeBase_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      PhysicsObjectTreeBase
// 
/**\class PhysicsObjectTreeBase PhysicsObjectTreeBase.cc Analysis/Tools/src/PhysicsObjectTreeBase.cc

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
#include "Analysis/Tools/interface/TreeBase.h"
#include "Analysis/Tools/interface/Collection.h"
#include "Analysis/Tools/interface/Jet.h"
#include "Analysis/Tools/interface/MET.h"
#include "Analysis/Tools/interface/Muon.h"
#include "Analysis/Tools/interface/Vertex.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      template <typename Object>
      class PhysicsObjectTreeBase : public TreeBase {
         public:
            PhysicsObjectTreeBase();
            PhysicsObjectTreeBase(TChain * tree, const std::string & name);
           ~PhysicsObjectTreeBase();
           
            // ----------member data ---------------------------
         protected:
            static const int max_ = 1000;
            // general candidates (e.g. physics objects)
            int n_;
            float pt_  [max_];
            float eta_ [max_];
            float phi_ [max_];
            float e_   [max_];
            int   q_   [max_];
            
            float px_  [max_];
            float py_  [max_];
            float pz_  [max_];
            
         private:

      };
         
      template <>
      class PhysicsObjectTreeBase<Vertex> : public TreeBase {
         public:
            PhysicsObjectTreeBase();
            PhysicsObjectTreeBase(TChain * tree, const std::string & name);
           ~PhysicsObjectTreeBase();
           
            // ----------member data ---------------------------
         protected:
            static const int max_ = 400;
            
            // general candidates
            int n_;
            float x_   [max_];
            float y_   [max_];
            float z_   [max_];
            float xe_  [max_];
            float ye_  [max_];
            float ze_  [max_];
            bool  fake_[max_];
            float chi2_[max_];
            float ndof_[max_];
            float rho_ [max_];
            
         private:

      };
   }
}

#endif  // Analysis_Tools_PhysicsObjectTreeBase_h
