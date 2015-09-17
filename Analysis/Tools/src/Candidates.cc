/**\class Candidates Candidates.cc Analysis/Tools/src/Candidates.cc

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
 
#include "Analysis/Tools/interface/Candidates.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Candidates::Candidates(TChain * tree)
{
   tree_ = tree;
   tree_  -> SetBranchAddress( "n"      ,    &n_     );
   tree_  -> SetBranchAddress( "pt"     ,     pt_    );
   tree_  -> SetBranchAddress( "eta"    ,     eta_   );
   tree_  -> SetBranchAddress( "phi"    ,     phi_   );
   tree_  -> SetBranchAddress( "e"      ,     e_     );
   tree_  -> SetBranchAddress( "q"      ,     q_     );
   tree_  -> SetBranchAddress( "px"     ,     px_    );
   tree_  -> SetBranchAddress( "py"     ,     py_    );
   tree_  -> SetBranchAddress( "pz"     ,     pz_    );
}

Candidates::~Candidates()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
Candidate Candidates::at(const int& i)
{
   using namespace edm;
   
   Candidate cand;
   
   if ( i >= n_ || pt_ < 0 )
   {
      return cand;
   }
   
   cand.set(pt_[i],eta_[i],phi_[i],e_[i],q_[i]);
   cand.momentum3(px_[i],py_[i],pz_[i]);
   cand.phi(phi_[i]);
   cand.pt(pt_[i]);
   
   return cand;
   
}

int Candidates::size()
{
   return n_;
}

void Candidates::entry(const int & i)
{
   tree_ -> GetEntry(i);
}
