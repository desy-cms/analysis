#ifndef Analysis_Tools_Vertices_h
#define Analysis_Tools_Vertices_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Vertices
// 
/**\class Vertices Vertices.cc Analysis/Tools/src/Vertices.cc

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
#include "Analysis/Tools/interface/Vertex.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Vertices {
         public:
            Vertices(TChain *);
           ~Vertices();
           
           Vertex at(const int &);
           int size();
           void entry(const int &);
      
            // ----------member data ---------------------------
         protected:
            static const int max_ = 100;
            
            // 
            int   n_   ;
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
            
            
            std::vector<Vertex> Vertices_;
            
            TChain * tree_;
         private:

      };
   }
}

#endif  // Analysis_Tools_Vertices_h
