/**\class Vertex Vertex.cc Analysis/Tools/src/Vertex.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh Bastos Rangel
//         Created:  Mon, 09 Sep 2015 09:24:08 GMT
//
//

// system include files
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/Vertex.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Vertex::Vertex() : x_(0.), y_(0.), z_(0.)
{
}

Vertex::Vertex(const float & x, const float & y, const float & z) : x_(x), y_(y), z_(z)
{
}

Vertex::~Vertex()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ methods  ------------
// Get
float Vertex::x()  { return x_ ; }
float Vertex::y()  { return y_ ; }
float Vertex::z()  { return z_ ; }
float Vertex::xe() { return xe_; }
float Vertex::ye() { return ye_; }
float Vertex::ze() { return ze_; }

float Vertex::chi2() { return chi2_; }
float Vertex::ndof() { return ndof_; }
float Vertex::rho()  { return rho_ ; }

bool  Vertex::fake() { return fake_; }

// Set
void  Vertex::x   (const float & x ) { x_  = x ; }
void  Vertex::y   (const float & y ) { y_  = y ; }
void  Vertex::z   (const float & z ) { z_  = z ; }
void  Vertex::xe  (const float & xe) { xe_ = xe; }
void  Vertex::ye  (const float & ye) { ye_ = ye; }
void  Vertex::ze  (const float & ze) { ze_ = ze; }

void  Vertex::chi2 (const float & chi2 ) { chi2_  = chi2 ; }
void  Vertex::ndof (const float & ndof ) { ndof_  = ndof ; }
void  Vertex::rho  (const float & rho  ) { rho_   = rho  ; }

void  Vertex::fake (const bool  & fake ) { fake_  = fake ; }
