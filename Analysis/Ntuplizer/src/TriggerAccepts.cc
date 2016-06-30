/**\class TriggerAccepts TriggerAccepts.cc Analysis/Ntuplizer/src/TriggerAccepts.cc

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
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Ntuplizer/interface/TriggerAccepts.h"

//
// class declaration
//

using namespace analysis;
using namespace analysis::ntuple;

//
// constructors and destructor
//
TriggerAccepts::TriggerAccepts()
{
   // default constructor
}

#ifndef CMSSWOLD
TriggerAccepts::TriggerAccepts(const edm::InputTag& tag, TTree* tree, const std::vector<std::string>& inpaths, const std::shared_ptr<HLTPrescaleProvider> hltPrescale , const bool & testmode)
{
   hlt_prescale_ = hltPrescale;
   input_collection_ = tag;
   tree_ = tree;
   inpaths_ = inpaths;
   paths_.clear();
   first_ = true;
   testmode_ = testmode;
   if ( inpaths_.size() == 0 ) testmode_ = true;
}
#endif

TriggerAccepts::TriggerAccepts(const edm::InputTag& tag, TTree* tree, const std::vector<std::string>& inpaths, const bool & testmode)
{
   input_collection_ = tag;
   tree_ = tree;
   inpaths_ = inpaths;
   paths_.clear();
   first_ = true;
   testmode_ = testmode;
   if ( inpaths_.size() == 0 ) testmode_ = true;
}

TriggerAccepts::~TriggerAccepts()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void TriggerAccepts::Fill(const edm::Event& event, const edm::EventSetup & setup)
{
   using namespace edm;
   
   // reset trigger accepts
   for (size_t i = 0; i < paths_.size() ; ++i )
      accept_[i] = false;

   Handle<TriggerResults> handler;
   event.getByLabel(input_collection_, handler);
   const TriggerResults & triggers = *(handler.product());
   
   for ( size_t j = 0 ; j < hlt_config_.size() ; ++j )
   {
      for (size_t i = 0; i < paths_.size() ; ++i )
      {
         if ( hlt_config_.triggerName(j).find(paths_[i]) == 0 )
         {
#ifndef CMSSWOLD          
            const std::pair<std::vector<std::pair<std::string,int> >,int> ps = hlt_prescale_->prescaleValuesInDetail(event,setup,hlt_config_.triggerName(j));
            psl1_[i] = ps.first[0].second;
            pshlt_[i] = ps.second;
#else
            std::pair< int, int > ps = hlt_config_.prescaleValues (event, setup, hlt_config_.triggerName(j));
            psl1_[i] = ps.first;
            pshlt_[i] = ps.second;
#endif            
//            psl1_[i]  = 1;
//            pshlt_[i] = 1;
            if ( triggers.accept(j) ) accept_[i] = true;
         }
      }
   }
   
   tree_ -> Fill();
   
}

// ------------ method called once each job just before starting event loop  ------------
void TriggerAccepts::Branches()
{
   for (size_t i = 0; i < paths_.size() ; ++i )
   {
      tree_->Branch(paths_[i].c_str(), &accept_[i], (paths_[i]+"/O").c_str());
      tree_->Branch(("psl1_"+paths_[i]).c_str(), &psl1_[i], ("psl1_"+paths_[i]+"/I").c_str());
      tree_->Branch(("pshlt_"+paths_[i]).c_str(), &pshlt_[i], ("pshlt_"+paths_[i]+"/I").c_str());
   }
   // std::cout << "TriggerAccepts Branches ok" << std::endl;
}

void TriggerAccepts::LumiBlock(edm::LuminosityBlock const & lumi, edm::EventSetup const& setup)
{
   bool changed;
#ifndef CMSSWOLD          
   hlt_prescale_->init(lumi.getRun(), setup, input_collection_.process(), changed);
   hlt_config_ = hlt_prescale_->hltConfigProvider();
#else
   hlt_config_.init(lumi.getRun(), setup, input_collection_.process(), changed);
#endif            
   
   std::vector<std::string> names = hlt_config_.triggerNames();
   
   if ( first_ )  // using this kind of resource to get the names from the configuration, when needed.
   {
      if ( ! testmode_ )
         paths_ = inpaths_;
      else
      {
         if ( inpaths_.size() == 0 ) // means all paths will be considered
            paths_ = names;
         else
         {
            for ( size_t i = 0 ; i < inpaths_.size() ; ++i )
            {
               for ( size_t j = 0 ; j < hlt_config_.size() ; ++j )
               {
                  if ( paths_.size() >= 1000 )
                  {
                     std::cout << "analysis::ntuple::TriggerAccepts::LumiBlock - Number of trigger paths is larger than 1000." << std::endl;
                     std::cout << "                                              Not all trigger paths will be considered."    << std::endl;
                     break;
                  }
                  if ( hlt_config_.triggerName(j).find(inpaths_[i]) == 0 )
                     paths_.push_back(hlt_config_.triggerName(j));
               }
            }
         }
      }
      this->Branches();
      first_ = false;
   }

}

void TriggerAccepts::Run(edm::Run const & run, edm::EventSetup const& setup)
{
   bool changed;
#ifndef CMSSWOLD          
   hlt_prescale_->init(run, setup, input_collection_.process(), changed);
   hlt_config_ = hlt_prescale_->hltConfigProvider();
#else
   hlt_config_.init(lumi.getRun(), setup, input_collection_.process(), changed);
#endif            

   std::vector<std::string> names = hlt_config_.triggerNames();
   
   if ( first_ )  // using this kind of resource to get the names from the configuration, when needed.
   {
      if ( ! testmode_ )
         paths_ = inpaths_;
      else
      {
         if ( inpaths_.size() == 0 ) // means all paths will be considered
            paths_ = names;
         else
         {
            for ( size_t i = 0 ; i < inpaths_.size() ; ++i )
            {
               for ( size_t j = 0 ; j < hlt_config_.size() ; ++j )
               {
                  if ( paths_.size() >= 1000 )
                  {
                     std::cout << "analysis::ntuple::TriggerAccepts::Run - Number of trigger paths is larger than 1000." << std::endl;
                     std::cout << "                                              Not all trigger paths will be considered."    << std::endl;
                     break;
                  }
                  if ( hlt_config_.triggerName(j).find(inpaths_[i]) == 0 )
                     paths_.push_back(hlt_config_.triggerName(j));
               }
            }
         }
      }
      this->Branches();
      first_ = false;
   }

}
