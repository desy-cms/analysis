/**\class MET MET.cc Analysis/Tools/src/MET.cc

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
 
#include "Analysis/Tools/interface/MET.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
MET::MET() : Candidate() 
{
}
MET::~MET()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ methods  ------------
matrix<float> MET::significanceMatrix()
{
   return sig_;
}

float * MET::genP()
{
   return gen_p_;
}

void MET::significanceMatrix (const float & xx, const float & xy, const float & yx, const float & yy)
{
   sig_.resize(2,2);
   sig_(0,0) = xx;
   sig_(0,1) = xy;
   sig_(1,0) = yx;
   sig_(1,1) = yy;
}

void MET::genP (const float & px, const float & py, const float & pz)
{
   gen_p_[0] = px;
   gen_p_[1] = py;
   gen_p_[2] = pz;
}   
