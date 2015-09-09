/**\class Jets Jets.cc Analysis/Tools/src/Jets.cc

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
#include <iostream>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/Jets.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Jets::Jets(TChain * tree, std::string btagging ) : Candidates(tree)
{
   btagging_ = btagging;
   tree  -> SetBranchAddress( btagging_.c_str(), btag_    );
   tree  -> SetBranchAddress( "flavour",         flavour_ );
   tree  -> SetBranchAddress( "id_nHadFrac",     nHadFrac_);
   tree  -> SetBranchAddress( "id_nEmFrac",      nEmFrac_ );
   tree  -> SetBranchAddress( "id_nMult",        nMult_   );
   tree  -> SetBranchAddress( "id_cHadFrac",     cHadFrac_);
   tree  -> SetBranchAddress( "id_cEmFrac",      cEmFrac_ );
   tree  -> SetBranchAddress( "id_cMult",        cMult_   );
   tree  -> SetBranchAddress( "id_muonFrac",     muFrac_  );

}

Jets::~Jets()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------

Jet Jets::at(const int& i)
{
   using namespace edm;
   
   Jet jet;
   
   if ( i >= n_ || pt_ < 0 )
   {
      return jet;
   }
   
   jet.set(pt_[i],eta_[i],phi_[i],e_[i],q_[i]);
   jet.btag(btag_[i]);
   jet.flavour(flavour_[i]);
   // Jet ID
   int nM = (int)round(nMult_[i]);
   int cM = (int)round(cMult_[i]);
   int numConst = nM + cM;
   if ( fabs(eta_[i]) <= 3. )
   {
      jet.idLoose((nHadFrac_[i]<0.99 && nEmFrac_[i]<0.99 && numConst>1) && ((abs(eta_[i])<=2.4 && cHadFrac_[i]>0 && cM>0 && cEmFrac_[i]<0.99) || fabs(eta_[i])>2.4) && fabs(eta_[i])<=3.0);
      jet.idTight((nHadFrac_[i]<0.90 && nEmFrac_[i]<0.90 && numConst>1) && ((abs(eta_[i])<=2.4 && cHadFrac_[i]>0 && cM>0 && cEmFrac_[i]<0.99) || fabs(eta_[i])>2.4) && fabs(eta_[i])<=3.0);
   }
   else
   {
      jet.idLoose(nEmFrac_[i]<0.90 && nM>10 && fabs(eta_[i])>3.0);
      jet.idTight(nEmFrac_[i]<0.90 && nM>10 && fabs(eta_[i])>3.0);
   }
   
   return jet;
   
}
