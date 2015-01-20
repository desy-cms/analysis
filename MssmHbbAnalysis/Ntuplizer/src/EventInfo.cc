/**\class EventInfo EventInfo.cc MssmHbbAnalysis/Ntuplizer/src/EventInfo.cc

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
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "MssmHbbAnalysis/Ntuplizer/interface/EventInfo.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

//
// constructors and destructor
//
EventInfo::EventInfo()
{
   // default constructor
   do_trigger_ = false;
}

EventInfo::EventInfo(TTree* tree)
{
   tree_ = tree;
   do_trigger_ = false;
   
}

EventInfo::EventInfo(const edm::InputTag& tag, TTree* tree, const std::vector<std::string>& paths)
{
   input_collection_ = tag;
   tree_ = tree;
   paths_ = paths;
   do_trigger_ = true;
   
}

EventInfo::~EventInfo()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void EventInfo::Fill(const edm::Event& event)
{
   using namespace edm;
   
   const edm::EventAuxiliary evt = event.eventAuxiliary();
   event_ = evt.event();
   run_   = evt.run();
   lumi_  = evt.luminosityBlock();
   orbit_ = evt.orbitNumber();
   bx_    = evt.bunchCrossing();
   
   if ( do_trigger_ )
   {
      Handle<TriggerResults> handler;
      event.getByLabel(input_collection_, handler);
      const TriggerResults & triggers = *(handler.product());
      
//      bool fltrAccepted = false;
      
      for ( size_t j = 0 ; j < hlt_config_.size() ; ++j )
      {
         for (std::vector<std::string>::iterator p = paths_.begin() ; p != paths_.end(); ++p)
         {
            bool found_path = ( hlt_config_.triggerName(j).find(*p) == 0 );
            if ( found_path )
            {
//               if ( HLTRFltr.accept(j) ) fltrAccepted = true;
            }
         }
      }
   }
   
   tree_ -> Fill();
   
}

// ------------ method called once each job just before starting event loop  ------------
void EventInfo::Branches()
{
   
   // event output info
   tree_->Branch("event", &event_, "event/I");
   tree_->Branch("run"  , &run_  , "run/I");
   tree_->Branch("lumisection" , &lumi_ , "lumisection/I");
   tree_->Branch("bx"   , &bx_   , "bx/I");
   tree_->Branch("orbit", &orbit_, "orbit/I");
   if ( do_trigger_ )
   {
      for (size_t i = 0; i < paths_.size() ; ++i )
         tree_->Branch(paths_[i].c_str(), &trigger_, (paths_[i]+"/O").c_str());
   }
}

void EventInfo::LumiBlock(edm::LuminosityBlock const & lumi, edm::EventSetup const& setup)
{
   bool changed;
   hlt_config_.init(lumi.getRun(), setup, input_collection_.process(), changed);

}
