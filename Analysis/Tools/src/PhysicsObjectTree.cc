/**\class PhysicsObjectTree PhysicsObjectTree.cc Analysis/Tools/src/PhysicsObjectTree.cc

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
#include "Analysis/Tools/interface/PhysicsObjectTree.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class Object>
      PhysicsObjectTree<Object>::PhysicsObjectTree() : PhysicsObjectTreeBase<Object>()     {}
template <class Object>
      PhysicsObjectTree<Object>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Object>(tree, name) {}
template <class Object>
      PhysicsObjectTree<Object>::~PhysicsObjectTree()                                                   {}

// member function
template <class Object> Collection<Object>  PhysicsObjectTree<Object>::collection()
{
   // DUMMY
   Collection<Object> obj;
   return obj;
}

// JETS
// Constructors and destructor
PhysicsObjectTree<Jet>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Jet>(tree, name)
{
   tree_  -> SetBranchAddress( "btag_csvivf", btag_    );
   tree_  -> SetBranchAddress( "flavour"        , flavour_ );
   tree_  -> SetBranchAddress( "hadronFlavour"  , hadrflavour_ );
   tree_  -> SetBranchAddress( "partonFlavour"  , partflavour_ );
   tree_  -> SetBranchAddress( "physicsFlavour" , physflavour_ );
   tree_  -> SetBranchAddress( "id_nHadFrac", nHadFrac_);
   tree_  -> SetBranchAddress( "id_nEmFrac" , nEmFrac_ );
   tree_  -> SetBranchAddress( "id_nMult"   , nMult_   );
   tree_  -> SetBranchAddress( "id_cHadFrac", cHadFrac_);
   tree_  -> SetBranchAddress( "id_cEmFrac" , cEmFrac_ );
   tree_  -> SetBranchAddress( "id_cMult"   , cMult_   );
   tree_  -> SetBranchAddress( "id_muonFrac", muFrac_  );
}
PhysicsObjectTree<Jet>::~PhysicsObjectTree() {}

// Member functions
Collection<Jet>  PhysicsObjectTree<Jet>::collection()
{
   std::vector<Jet> jets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Jet jet(pt_[i], eta_[i], phi_[i], e_[i]);
      jet.btag(btag_[i]);
      jet.flavour(flavour_[i]);
      jet.flavour("Hadron",hadrflavour_[i]);
      jet.flavour("Parton",partflavour_[i]);
      jet.flavour("Physics",physflavour_[i]);
      jet.id(nHadFrac_[i],
             nEmFrac_[i] ,
             nMult_[i]   ,
             cHadFrac_[i],
             cEmFrac_[i] ,
             cMult_[i]   ,
             muFrac_[i]  );
      jets.push_back(jet);
   }
   Collection<Jet> jetCollection(jets, name_);
   return jetCollection;

}


// MET
// Constructors and destructor
PhysicsObjectTree<MET>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<MET>(tree, name)
{
   tree_  -> SetBranchAddress( "sigxx" , sigxx_ );
   tree_  -> SetBranchAddress( "sigxy" , sigxy_ );
   tree_  -> SetBranchAddress( "sigyx" , sigyx_ );
   tree_  -> SetBranchAddress( "sigyy" , sigyy_ );
   // Exists in MC; check if available and set
   // Maybe it's better to use analysis::isMC() for this perpouse?
   std::vector<std::string>::iterator it;
   it = std::find(branches_.begin(),branches_.end(),"gen_px");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_px_);
   it = std::find(branches_.begin(),branches_.end(),"gen_py");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_py_);
   it = std::find(branches_.begin(),branches_.end(),"gen_pz");  if ( it != branches_.end() ) tree_  -> SetBranchAddress( (*it).c_str(), gen_pz_);
}
PhysicsObjectTree<MET>::~PhysicsObjectTree() {}

// Member function
Collection<MET>  PhysicsObjectTree<MET>::collection()
{
   std::vector<MET> mets;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      MET met(px_[i], py_[i], pz_[i]);
      met.significanceMatrix(sigxx_[i],sigxy_[i],sigyx_[i],sigyy_[i]);
      met.genP(gen_px_[i],gen_py_[i],gen_pz_[i]);
      mets.push_back(met);
   }
   Collection<MET> metCollection(mets, name_);
   return metCollection;

}

// MUON
// Constructors and destructor
PhysicsObjectTree<Muon>::PhysicsObjectTree() : PhysicsObjectTreeBase<Muon>()
{
}
PhysicsObjectTree<Muon>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Muon>(tree, name)
{
}
PhysicsObjectTree<Muon>::~PhysicsObjectTree()
{
}
// Member functions
Collection<Muon>  PhysicsObjectTree<Muon>::collection()
{
   std::vector<Muon> muons;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Muon muon(pt_[i], eta_[i], phi_[i], e_[i], q_[i]);
      muons.push_back(muon);
   }
   Collection<Muon> muonCollection(muons, name_);
   return muonCollection;
}

// VERTEX
// Constructors and destructor
PhysicsObjectTree<Vertex>::PhysicsObjectTree()              : PhysicsObjectTreeBase<Vertex>()     {}
PhysicsObjectTree<Vertex>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<Vertex>(tree, name) {}
PhysicsObjectTree<Vertex>::~PhysicsObjectTree()                                                   {}

// Member functions
Collection<Vertex>  PhysicsObjectTree<Vertex>::collection()
{
   std::vector<Vertex> vertices;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      Vertex vertex(x_[i], y_[i], z_[i]);
      vertex.chi2(chi2_[i]);
      vertex.ndof(ndof_[i]);
      vertex.rho(rho_[i]);
      vertex.fake(fake_[i]);

      vertices.push_back(vertex);
   }
   Collection<Vertex> vertexCollection(vertices, name_ );
   return vertexCollection;
}

// TriggerObjects
// Constructors and destructor
PhysicsObjectTree<TriggerObject>::PhysicsObjectTree() : PhysicsObjectTreeBase<TriggerObject>()
{
}
PhysicsObjectTree<TriggerObject>::PhysicsObjectTree(TChain * tree, const std::string & name) : PhysicsObjectTreeBase<TriggerObject>(tree, name)
{
}
PhysicsObjectTree<TriggerObject>::~PhysicsObjectTree() {}

// Member functions
Collection<TriggerObject>  PhysicsObjectTree<TriggerObject>::collection()
{
   std::vector<TriggerObject> triggers;
   for ( int i = 0 ; i < n_ ; ++i )
   {
      TriggerObject trig(pt_[i], eta_[i], phi_[i], e_[i]);
      triggers.push_back(trig);
   }
   Collection<TriggerObject> TriggerObjectCollection(triggers, name_);
   return TriggerObjectCollection;

}

// ======================================
// Templates declarations
template class PhysicsObjectTree<Candidate>;
template class PhysicsObjectTree<Jet>;
template class PhysicsObjectTree<MET>;
template class PhysicsObjectTree<Muon>;
template class PhysicsObjectTree<Vertex>;
template class PhysicsObjectTree<TriggerObject>;
