#ifndef Analysis_Tools_PhysicsObjectTree_h
#define Analysis_Tools_PhysicsObjectTree_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      PhysicsObjectTree
//
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
#include <memory>
#include <vector>
//
// user include files

#include "TTree.h"
#include "TChain.h"
#include "Analysis/Tools/interface/PhysicsObjectTreeBase.h"
#include "Analysis/Tools/interface/Collection.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      template <class Object>
      class PhysicsObjectTree : public PhysicsObjectTreeBase<Object> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Object> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for JET
      template <>
      class PhysicsObjectTree<Jet> : public PhysicsObjectTreeBase<Jet> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Jet> collection();

            // ----------member data ---------------------------
         protected:
            // PatJets
            float btag_    [max_];
            int   flavour_ [max_];
            int   hadrflavour_ [max_];
            int   partflavour_ [max_];
            int   physflavour_ [max_];
            float nHadFrac_[max_];
            float nEmFrac_ [max_];
            float nMult_   [max_];
            float cHadFrac_[max_];
            float cEmFrac_ [max_];
            float cMult_   [max_];
            float muFrac_  [max_];
            bool  idLoose_ [max_];
            bool  idTight_ [max_];
            float jecUnc_  [max_];

         private:

      };
         
      // Specialization for Candidate
      template <>
      class PhysicsObjectTree<Candidate> : public PhysicsObjectTreeBase<Candidate> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Candidate> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };  

      // Specialization for GENPARTICLE
      template <>
      class PhysicsObjectTree<GenParticle> : public PhysicsObjectTreeBase<GenParticle> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<GenParticle> collection();

            // ----------member data ---------------------------
         protected:
            // PatJets
            int   pdgid_    [max_];
            int   status_   [max_];
            bool  higgs_dau_[max_];

         private:

      };

      // Specialization for MET
      template <>
      class PhysicsObjectTree<MET> : public PhysicsObjectTreeBase<MET>{
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<MET> collection();
            // ----------member data ---------------------------
         protected:
            // METs
            float sigxx_  [max_];
            float sigxy_  [max_];
            float sigyx_  [max_];
            float sigyy_  [max_];

            float gen_px_ [max_];
            float gen_py_ [max_];
            float gen_pz_ [max_];

         private:

      };

      // Specialization for Muon
      template <>
      class PhysicsObjectTree<Muon> : public PhysicsObjectTreeBase<Muon> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Muon> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for Vertex
      template <>
      class PhysicsObjectTree<Vertex> : public PhysicsObjectTreeBase<Vertex> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<Vertex> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };

      // Specialization for TriggerObject
      template <>
      class PhysicsObjectTree<TriggerObject> : public PhysicsObjectTreeBase<TriggerObject> {
         public:
            PhysicsObjectTree();
            PhysicsObjectTree(TChain * tree, const std::string & name);
           ~PhysicsObjectTree();

            Collection<TriggerObject> collection();

            // ----------member data ---------------------------
         protected:

         private:

      };
   }
}

#endif  // Analysis_Tools_PhysicsObjectTree_h
