#ifndef Analysis_Tools_Analysis_h
#define Analysis_Tools_Analysis_h 1

// -*- C++ -*-
//
// Package:    Analysis/Tools
// Class:      Analysis
//
/**\class Analysis Analysis.cc Analysis/Tools/src/Analysis.cc

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
#include <string>
//
// user include files

#include "TChain.h"
#include "TFile.h"
#include "TFileCollection.h"
#include "Analysis/Tools/interface/Utils.h"

#include "Analysis/Tools/interface/PhysicsObjectTree.h"
#include "Analysis/Tools/interface/Collection.h"

//
// class declaration
//

namespace analysis {
   namespace tools {

      typedef std::shared_ptr< PhysicsObjectTree<Jet> >    pJetTree;
      typedef std::shared_ptr< PhysicsObjectTree<MET> >    pMETTree;
      typedef std::shared_ptr< PhysicsObjectTree<Muon> >   pMuonTree;
      typedef std::shared_ptr< PhysicsObjectTree<Vertex> > pVertexTree;
      typedef std::shared_ptr< PhysicsObjectTree<TriggerObject> > pTriggerObjectTree;

      class Analysis {
         public:
            Analysis(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~Analysis();

            // Event
            int  numberEvents();
            int  size();
            void event(const int & event);
            int  event();
            int  run();
            int  lumiSection();
            bool isMC();

            // Trees
            template<class Object>
            std::shared_ptr< PhysicsObjectTree<Object> > addTree(const std::string & unique_name, const std::string & path );
            template<class Object>
            std::shared_ptr< PhysicsObjectTree<Object> > tree(const std::string & unique_name);
            template<class Object>
            Collection<Object> collection(const std::string & unique_name);

            // Cross sections
            void   crossSections(const std::string & path);
            double crossSection();
            double crossSection(const std::string & title);
            void   listCrossSections();

            // Trigger results
            void addTriggerResultTree(const std::string & unique_name, const std::string & path = "MssmHbb/Events/TriggerResults");
            bool HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160();
            bool HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6();

            // Generator Filter
            FilterResults generatorFilter(const std::string & path);
            void listGeneratorFilter();

            // ----------member data ---------------------------
         protected:

            TFileCollection * fileCollection_;
            TCollection * fileList_;
            std::string inputFilelist_;


            std::map<std::string, double> xsections_;
            FilterResults genfilter_;

            int event_;
            int run_;
            int lumi_;
            bool is_mc_;

            int nevents_;
            int HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_;
            int HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_;

            // TREES
            void treeInit_(const std::string & unique_name, const std::string & path);
            TChain * t_xsection_;
            TChain * t_genfilter_;
            TChain * t_event_;


         // Physics objects
            // root trees
            std::map<std::string, TChain*> tree_;

            // analysis trees (in C++14 one can define a variable template member somehow, but I did not manage to do this)
            std::map<std::string, pJetTree>    t_jets_;
            std::map<std::string, pMETTree>    t_mets_;
            std::map<std::string, pMuonTree>   t_muons_;
            std::map<std::string, pVertexTree> t_vertices_;
            std::map<std::string, pTriggerObjectTree> t_TriggerObjects_;

         private:

          static bool checker_;

      }; // END OF CLASS DECLARATIONS!!!

      // stupid: I could not make function template without specialisation
      // it should not work wiht implementation in the header because the
      // returned values are different.
     template <> pJetTree    Analysis::tree(const std::string & unique_name);
     template <> pMETTree    Analysis::tree(const std::string & unique_name);
     template <> pMuonTree   Analysis::tree(const std::string & unique_name);
     template <> pVertexTree Analysis::tree(const std::string & unique_name);
     template <> pTriggerObjectTree Analysis::tree(const std::string & unique_name);

     template <> pTriggerObjectTree Analysis::addTree(const std::string & unique_name, const std::string & path);
     template <> pJetTree    Analysis::addTree(const std::string & unique_name, const std::string & path);
     template <> pMETTree    Analysis::addTree(const std::string & unique_name, const std::string & path);
     template <> pMuonTree   Analysis::addTree(const std::string & unique_name, const std::string & path);
     template <> pVertexTree Analysis::addTree(const std::string & unique_name, const std::string & path);

// ========================================================
//                         IMPLEMENTATIONS!
// ========================================================

// +++++++++++++++++++++++ IMPORTANT ++++++++++++++++++++++
// Need to put the implementations in the header file when using template!!!
// This explains the problems I have been getting.
// ========================================================

      // COLLECTIONS
      template <class Object>
      Collection<Object>  Analysis::collection(const std::string & unique_name)
      {
         Collection<Object> col = this -> tree<Object>(unique_name) -> collection();
         return col;
      }

      inline int Analysis::numberEvents() { return nevents_; }
      inline int Analysis::size() { return nevents_; }
      inline int Analysis::event()       { return event_; }
      inline int Analysis::run()         { return run_  ; }
      inline int Analysis::lumiSection() { return lumi_ ; }
      inline bool Analysis::isMC() { return is_mc_ ; }

      inline bool Analysis::HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160() { return HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_; }
      inline bool Analysis::HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6() { return HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_; }



   }
}

#endif  // Analysis_Tools_Analysis_h
