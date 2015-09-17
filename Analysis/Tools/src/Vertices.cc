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
#include <iostream>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Tools/interface/Vertices.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
Vertices::Vertices(TChain * tree)
{
   tree_ = tree;
   tree_  -> SetBranchAddress( "n"      ,    &n_     );
   tree_  -> SetBranchAddress( "x"      ,     x_     );
   tree_  -> SetBranchAddress( "y"      ,     y_     );
   tree_  -> SetBranchAddress( "z"      ,     z_     );
   tree_  -> SetBranchAddress( "xe"     ,     xe_    );
   tree_  -> SetBranchAddress( "ye"     ,     ye_    );
   tree_  -> SetBranchAddress( "ze"     ,     ze_    );
   tree_  -> SetBranchAddress( "fake"   ,     fake_  );
   tree_  -> SetBranchAddress( "chi2"   ,     chi2_  );
   tree_  -> SetBranchAddress( "ndof"   ,     ndof_  );
   tree_  -> SetBranchAddress( "rho"    ,     rho_   );
}

Vertices::~Vertices()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
Vertex Vertices::at(const int& i)
{
   using namespace edm;
   
   Vertex vtx;
   
   if ( i >= n_ )
   {
      return vtx;
   }
   
   vtx.set(x_[i],y_[i],z_[i]);
   vtx.xe(  xe_  [i]);
   vtx.ye  (ye_  [i]);
   vtx.ze  (ze_  [i]);
   vtx.chi2(chi2_[i]);
   vtx.ndof(ndof_[i]);
   vtx.rho (rho_ [i]);
   vtx.fake(fake_[i]);
   
   return vtx;
   
}

int Vertices::size()
{
   return n_;
}

void Vertices::entry(const int & i)
{
   tree_ -> GetEntry(i);
}
