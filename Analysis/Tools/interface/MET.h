#ifndef Analysis_Tools_MET_h
#define Analysis_Tools_MET_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      MET
// 
/**\class MET MET.cc Analysis/Tools/src/MET.cc

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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
// 
// user include files
#include "Analysis/Tools/interface/Candidate.h"
//
// class declaration
//
using namespace boost::numeric::ublas;

namespace analysis {
   namespace tools {

      class MET : public Candidate {
         public:
            MET();
            MET(const float & px, const float & py, const float & pz);
           ~MET();
            matrix<float> significanceMatrix();
            float * genP();
            
            void significanceMatrix(const float &, const float &,const float &,const float &);
            void genP(const float &, const float &,const float &);
            
            
//            using Candidate::set; // in case needed to overload the function set
      
         private:
            // ----------member data ---------------------------
            
            // 
            matrix<float> sig_;
            float gen_p_[3];
            
      };
   }
}

#endif  // Analysis_Tools_MET_h
