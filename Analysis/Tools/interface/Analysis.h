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
#include <typeinfo>
#include <boost/any.hpp>
#include <boost/core/demangle.hpp>
#include <boost/algorithm/string.hpp>
#include "stdlib.h"
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

      class Analysis {
         public:
            Analysis(const std::string & inputFilelist, const std::string & evtinfo = "MssmHbb/Events/EventInfo");
           ~Analysis();

            // Event
            int  numberEvents();
            int  size();
            void event(const int & event, const bool & addCollections = true);
            int  event();
            int  run();
            int  lumiSection();
            bool isMC();

            // Trees
            template<class Object>
            std::shared_ptr< PhysicsObjectTree<Object> > addTree(const std::string & unique_name, const std::string & path );
            template<class Object>
            std::shared_ptr< PhysicsObjectTree<Object> > tree(const std::string & unique_name);
            
            // Collections
            template<class Object>
            std::shared_ptr< Collection<Object> > addCollection(const std::string & unique_name);
            template<class Object>
            std::shared_ptr< Collection<Object> > collection(const std::string & unique_name);
            
            // Cross sections
            void   crossSections(const std::string & path);
            double crossSection();
            double crossSection(const std::string & title);
            void   listCrossSections();
            
            // Luminosity
            double luminosity();
            double luminosity(const std::string & title);

            // Trigger results
            void triggerResults(const std::string & path);
            int triggerResult(const std::string & trig);

            // Generator Filter
            FilterResults generatorFilter(const std::string & path);
            void listGeneratorFilter();
            
            // Matching to trigger objects
            template <class Object1, class Object2>
            void match(const std::string & collection, const std::string & match_collection);
            template <class Object1, class Object2>
            void match(const std::string & collection, const std::vector<std::string> & match_collections);
            
            // good Json files
            void processJsonFile(const std::string & fileName = "goodJson.txt");
            bool selectJson();

            // ----------member data ---------------------------
         protected:

            TFileCollection * fileCollection_;
            TCollection * fileList_;
            std::string inputFilelist_;


            std::map<std::string, double> xsections_;
            std::map<std::string, bool> triggerResults_;
            std::map<int,std::vector<std::string> > goodLumi_;
            FilterResults genfilter_;

            int event_;
            int run_;
            int lumi_;
            bool is_mc_;

            int nevents_;

            // TREES
            void treeInit_(const std::string & unique_name, const std::string & path);
            TChain * t_xsection_;
            TChain * t_genfilter_;
            TChain * t_event_;
            TChain * t_triggerResults_;

         // Physics objects
            // root trees
            std::map<std::string, TChain*> tree_;

            // Framework trees and types
            std::map<std::string, boost::any  > t_any_;
            std::map<std::string, std::string > t_type_;
            
            // Collections
            std::map<std::string, boost::any > c_any_;
            
         private:


      }; // END OF CLASS DECLARATIONS!!!

// ========================================================
//                         IMPLEMENTATIONS!
// ========================================================

// +++++++++++++++++++++++ IMPORTANT ++++++++++++++++++++++
// Need to put the implementations in the header file when using template!!!
// otherwise would have to specialize, like the other functions.
// This explains the problems I have been getting.
// -------------------------------------------------------
      // TREES
      template <class Object>
      std::shared_ptr< PhysicsObjectTree<Object> >  Analysis::addTree(const std::string & unique_name, const std::string & path)
      {
         this->treeInit_(unique_name,path);
         t_any_[unique_name] = std::shared_ptr< PhysicsObjectTree<Object> > ( new PhysicsObjectTree<Object>(tree_[unique_name], unique_name) );
         std::string type = boost::core::demangle(typeid(Object).name());
         std::vector<std::string> tmp;
         boost::split( tmp, type, boost::is_any_of("::"));
         t_type_[unique_name] = tmp.back();
         return boost::any_cast< std::shared_ptr< PhysicsObjectTree<Object> > > (t_any_[unique_name]);
      }
      // --
      template <class Object>
      std::shared_ptr< PhysicsObjectTree<Object> >  Analysis::tree(const std::string & unique_name)
      {
         // If tree does not exist, return NULL
         std::map<std::string, boost::any >::iterator it = t_any_.find(unique_name);
         if ( it == t_any_.end() )
            return nullptr;
         
         return boost::any_cast< std::shared_ptr< PhysicsObjectTree<Object> > > (t_any_[unique_name]);
      }
// -------------------------------------------------------
      // COLLECTIONS
      template <class Object>
      std::shared_ptr< Collection<Object> >  Analysis::addCollection(const std::string & unique_name)
      {
         // Still need to see how to deal with collections not originating from the ntuple,
         // e.g. a selected jets collection from the ntuple jets collection.
         
         // If tree does not exist, return NULL
         std::map<std::string, boost::any >::iterator it = t_any_.find(unique_name);
         if ( it == t_any_.end() )
            return nullptr;
         
         auto tree = boost::any_cast< std::shared_ptr< PhysicsObjectTree<Object> > > (t_any_[unique_name]);
         c_any_[unique_name] = std::shared_ptr< Collection<Object> > ( new Collection<Object>(tree -> collection()));
         
         std::shared_ptr< Collection<Object> > ret = boost::any_cast< std::shared_ptr< Collection<Object> > > (c_any_[unique_name]);
         
         return ret;
      }
      template <class Object>
      std::shared_ptr< Collection<Object> >  Analysis::collection(const std::string & unique_name)
      {
         std::shared_ptr< Collection<Object> > ret = boost::any_cast< std::shared_ptr< Collection<Object> > > (c_any_[unique_name]);
         return ret;
      }
      //--
      template <class Object1, class Object2>
      void Analysis::match(const std::string & collection, const std::string & match_collection)
      {
         auto o1 = boost::any_cast< std::shared_ptr< Collection<Object1> > > (c_any_[collection]);
         auto o2 = boost::any_cast< std::shared_ptr< Collection<Object2> > > (c_any_[match_collection]);
         o1->matchTo(o2);
      }
      //--
      template <class Object1, class Object2>
      void Analysis::match(const std::string & collection, const std::vector<std::string> & match_collections)
      {
         for ( auto & mc : match_collections )
            this->match<Object1,Object2>(collection,mc);
      }
      
// ========================================================

      inline int  Analysis::numberEvents() { return nevents_; }
      inline int  Analysis::size()         { return nevents_; }
      inline int  Analysis::event()        { return event_; }
      inline int  Analysis::run()          { return run_  ; }
      inline int  Analysis::lumiSection()  { return lumi_ ; }
      inline bool Analysis::isMC()         { return is_mc_ ; }

   }
}

#endif  // Analysis_Tools_Analysis_h
