#ifndef Analysis_Tools_Vertex_h
#define Analysis_Tools_Vertex_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Vertex
// 
/**\class Vertex Vertex.cc Analysis/Tools/src/Vertex.cc

 Description: Reconstructed Vertex

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
//Original

namespace analysis {
   namespace tools {

      class Vertex {
         public:
            /// default constructor
            Vertex();
            /// constructor with defined position x, y, z
            Vertex(const float &,const float &,const float &);
            /// destructor
           ~Vertex();
           
           /// returns x coordinate of vertex position
           float x   () const;  
           /// returns y coordinate of vertex position
           float y   () const;  
           /// returns z coordinate of vertex position
           float z   () const;  
           /// returns error on x coordinate of vertex position
           float xe  () const;  
           /// returns error on y coordinate of vertex position
           float ye  () const;  
           /// returns error on z coordinate of vertex position
           float ze  () const;  
           /// returns chi^2 of the vertex fit
           float chi2() const;  
           /// returns number of degrees of freedom of the vertex fit
           float ndof() const;  
           /// returns rho coordinate of vertex position
           float rho () const;  
           /// returns if vertex is fake or not
           bool  fake() const;  
           
           /// sets x coordinate of vertex position
           void x   (const float &);
           /// sets y coordinate of vertex position
           void y   (const float &);
           /// sets z coordinate of vertex position
           void z   (const float &); 
           /// sets error on x coordinate of vertex position
           void xe  (const float &);
           /// sets error on y coordinate of vertex position
           void ye  (const float &);
           /// sets error on z coordinate of vertex position
           void ze  (const float &);
           /// sets chi^2 of the vertex fit
           void chi2(const float &);
            /// sets number of degrees of freedom of the vertex fit
          void ndof(const float &);
           /// sets rho coordinate of vertex position
           void rho (const float &);
           /// sets if vertex is fake or not
           void fake(const bool  &);
           
         protected:
            // ----------member data ---------------------------
            
           // 
           /// x coordinate of vertex position
           float x_   ;
           /// y coordinate of vertex position
           float y_   ;
           /// z coordinate of vertex position
           float z_   ;
           /// error on x coordinate of vertex position
           float xe_  ;
           /// error on y coordinate of vertex position
           float ye_  ;
           /// error on z coordinate of vertex position
           float ze_  ;
           /// chi^2 of the vertex fit
           float chi2_;
            /// number of degrees of freedom of the vertex fit
           float ndof_;
           /// rho coordinate of vertex position
           float rho_ ;
           /// if vertex is fake or not
           bool  fake_;

         private:
      };
   }
}

#endif  // Analysis_Tools_Vertex_h
