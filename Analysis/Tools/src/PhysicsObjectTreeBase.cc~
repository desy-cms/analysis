/**\class PhysicsObjectTreeBase PhysicsObjectTreeBase.cc Analysis/Tools/src/PhysicsObjectTreeBase.cc

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
#include "Analysis/Tools/interface/PhysicsObjectTreeBase.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class Object>
PhysicsObjectTreeBase<Object>::PhysicsObjectTreeBase() : TreeBase()
{
}
template <class Object>
PhysicsObjectTreeBase<Object>::PhysicsObjectTreeBase(TChain * tree, const std::string & name) : TreeBase(tree, name)
{
   tree_  -> SetBranchAddress( "n"  , &n_   );
   tree_  -> SetBranchAddress( "pt" ,  pt_  );
   tree_  -> SetBranchAddress( "eta",  eta_ );
   tree_  -> SetBranchAddress( "phi",  phi_ );
   tree_  -> SetBranchAddress( "e"  ,  e_   );
   tree_  -> SetBranchAddress( "q"  ,  q_   );
   tree_  -> SetBranchAddress( "px" ,  px_  );
   tree_  -> SetBranchAddress( "py" ,  py_  );
   tree_  -> SetBranchAddress( "pz" ,  pz_  );

}

template <class Object>
PhysicsObjectTreeBase<Object>::~PhysicsObjectTreeBase()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------

// ===================================
//  Specialized for vertex

PhysicsObjectTreeBase<Vertex>::PhysicsObjectTreeBase() : TreeBase()
{
}

PhysicsObjectTreeBase<Vertex>::PhysicsObjectTreeBase(TChain * tree, const std::string & name) : TreeBase(tree, name)
{
   tree_  -> SetBranchAddress( "n"   , &n_    );
   tree_  -> SetBranchAddress( "x"   ,  x_    );
   tree_  -> SetBranchAddress( "y"   ,  y_    );
   tree_  -> SetBranchAddress( "z"   ,  z_    );
   tree_  -> SetBranchAddress( "xe"  ,  xe_   );
   tree_  -> SetBranchAddress( "ye"  ,  ye_   );
   tree_  -> SetBranchAddress( "ze"  ,  ze_   );
   tree_  -> SetBranchAddress( "fake",  fake_ );
   tree_  -> SetBranchAddress( "chi2",  chi2_ );
   tree_  -> SetBranchAddress( "ndof",  ndof_ );
   tree_  -> SetBranchAddress( "rho" ,  rho_  );
}

PhysicsObjectTreeBase<Vertex>::~PhysicsObjectTreeBase()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


template class PhysicsObjectTreeBase<Candidate>;
template class PhysicsObjectTreeBase<Jet>;
template class PhysicsObjectTreeBase<MET>;
template class PhysicsObjectTreeBase<Muon>;
template class PhysicsObjectTreeBase<Vertex>;
template class PhysicsObjectTreeBase<Trigger>;
