/**\class Jet Jet.cc Analysis/Tools/src/Jet.cc

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
 
#include "Analysis/Tools/interface/Jet.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Jet::Jet() : Candidate() 
{
}
Jet::Jet(const float & pt, const float & eta, const float & phi, const float & e) : Candidate(pt,eta,phi,e,0.) 
{
}
Jet::~Jet()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ methods  ------------
void Jet::id      (const float & nHadFrac,
                   const float & nEmFrac ,
                   const float & nMult   ,
                   const float & cHadFrac,
                   const float & cEmFrac ,
                   const float & cMult   ,
                   const float & muFrac  )
{
   // Jet ID
   int nM = (int)round(nMult);
   int cM = (int)round(cMult);
   int numConst = nM + cM;
   if ( fabs(p4_.Eta()) <= 3. )
   {
      idloose_ = ((nHadFrac<0.99 && nEmFrac<0.99 && numConst>1) && ((abs(p4_.Eta())<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || fabs(p4_.Eta())>2.4) && fabs(p4_.Eta())<=3.0);
      idtight_ = ((nHadFrac<0.90 && nEmFrac<0.90 && numConst>1) && ((abs(p4_.Eta())<=2.4 && cHadFrac>0 && cM>0 && cEmFrac<0.99) || fabs(p4_.Eta())>2.4) && fabs(p4_.Eta())<=3.0);
   }
   else
   {
      idloose_ = (nEmFrac<0.90 && nM>10 && fabs(p4_.Eta())>3.0);
      idtight_ = (nEmFrac<0.90 && nM>10 && fabs(p4_.Eta())>3.0);
   }   
}
      
      
