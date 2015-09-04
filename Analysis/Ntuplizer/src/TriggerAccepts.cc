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
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Ntuplizer/interface/TriggerAccepts.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

//
// constructors and destructor
//
TriggerAccepts::TriggerAccepts()
{
   // default constructor
}

TriggerAccepts::TriggerAccepts(const edm::InputTag& tag, TTree* tree, const std::vector<std::string>& paths)
{
   input_collection_ = tag;
   tree_ = tree;
   paths_ = paths;
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
void TriggerAccepts::Fill(const edm::Event& event)
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
         if ( hlt_config_.triggerName(j).find(paths_[i]) == 0 && triggers.accept(j) )
         {
            accept_[i] = true;
            break;
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
   }
   // std::cout << "TriggerAccepts Branches ok" << std::endl;
}

void TriggerAccepts::LumiBlock(edm::LuminosityBlock const & lumi, edm::EventSetup const& setup)
{
   bool changed;
   hlt_config_.init(lumi.getRun(), setup, input_collection_.process(), changed);

}
