#ifndef Analysis_Tools_Vertex_h
#define Analysis_Tools_Vertex_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Vertex
// 
/**\class Vertex Vertex.cc Analysis/Tools/src/Vertex.cc

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
// 
// user include files
#include "TLorentzVector.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      class Vertex {
         public:
            Vertex();
            Vertex(const float &,const float &,const float &);
           ~Vertex();
           
           float x   ();
           float y   ();
           float z   ();
           float xe  ();
           float ye  ();
           float ze  ();
           float chi2();
           float ndof();
           float rho ();
           bool  fake();
           
           void x   (const float &);
           void y   (const float &);
           void z   (const float &);
           void xe  (const float &);
           void ye  (const float &);
           void ze  (const float &);
           void chi2(const float &);
           void ndof(const float &);
           void rho (const float &);
           void fake(const bool  &);
           
           void set(const float & x, const float & y, const float & z);

      
         protected:
            // ----------member data ---------------------------
            
            // 
            float x_   ;
            float y_   ;
            float z_   ;
            float xe_  ;
            float ye_  ;
            float ze_  ;
            bool  fake_;
            float chi2_;
            float ndof_;
            float rho_ ;

         private:
      };
   }
}

#endif  // Analysis_Tools_Vertex_h
