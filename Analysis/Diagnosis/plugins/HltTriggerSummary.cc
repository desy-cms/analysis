// -*- C++ -*-
//
// Package:    Analysis/Diagnosis
// Class:      HltTriggerSummary
// 
/**\class HltTriggerSummary HltTriggerSummary.cc Analysis/Diagnosis/plugins/HltTriggerSummary.cc

 Description: Obtain trigger information from a EDM file. For MC it is straightforward, 
              for data be careful how you use the information!

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh
//         Created:  Tue, 18 Oct 2016 09:19:51 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Trigger related includes
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"


//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class HltTriggerSummary : public edm::EDAnalyzer
{
   public:
      explicit HltTriggerSummary(const edm::ParameterSet&);
      ~HltTriggerSummary();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual void beginRun(const edm::Run&, const edm::EventSetup&) override; 
      virtual void endRun(const edm::Run&, const edm::EventSetup&) override; 

      // ----------member data ---------------------------
      std::string pathName_;
      edm::InputTag triggerResults_;
      
      std::vector<std::string> triggerObjectsLabels_;
      edm::InputTag triggerEvent_;
   
      edm::EDGetTokenT<edm::TriggerResults>   triggerResultsTokens_;
      edm::EDGetTokenT<trigger::TriggerEvent> triggerEventTokens_;
      
      HLTPrescaleProvider hltPrescaleProvider_;
      HLTConfigProvider hltConfig_;
      
      bool readPrescale_;
      
      std::string triggerEventProcess_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HltTriggerSummary::HltTriggerSummary(const edm::ParameterSet& config):
   pathName_(config.getParameter<std::string> ("PathName")),
   triggerResults_(config.getParameter<edm::InputTag> ("TriggerResults")),
   triggerObjectsLabels_(config.getParameter< std::vector<std::string> >("TriggerObjectsLabels")),
   triggerEvent_ (config.getParameter<edm::InputTag> ("TriggerEvent")),
   triggerResultsTokens_(consumes<edm::TriggerResults>(triggerResults_)),
   triggerEventTokens_(consumes<trigger::TriggerEvent>(triggerEvent_)),
   hltPrescaleProvider_(config, consumesCollector(), *this)
{
   //now do what ever initialization is needed
   triggerEventProcess_ = triggerEvent_.process();
   readPrescale_ = false;
   if ( config.exists("ReadPrescale") )  readPrescale_ = config.getParameter<bool> ("ReadPrescale");
   

}


HltTriggerSummary::~HltTriggerSummary()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void HltTriggerSummary::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;
   
// Dealing with trigger results
   Handle<TriggerResults> triggerResultsHandler;
   event.getByLabel(triggerResults_, triggerResultsHandler);
   const TriggerResults & triggers = *(triggerResultsHandler.product());
   
   // check if the passed path fired
   const unsigned int triggerIndex(hltConfig_.triggerIndex(pathName_));
   std::cout << "Path " << pathName_;
   bool accepted = triggers.accept(triggerIndex);
   if ( accepted ) std::cout << " has fired!"     << std::endl;
   else            std::cout << " has not fired!" << std::endl;
   if ( readPrescale_ )
   {
      const std::pair<std::vector<std::pair<std::string,int> >,int> ps = hltPrescaleProvider_.prescaleValuesInDetail(event,setup,pathName_);
      std::cout << "     L1 prescales (in details) " << std::endl;
      for ( size_t j = 0; j < ps.first.size() ; ++j )
      {
         std::cout << "         " << ps.first[j].first << "  " << ps.first[j].second << std::endl;
      }
      std::cout << "     HLT prescale (in details) " << ps.second << std::endl;
   }

   
// Dealing with trigger objects from HltTriggerSummary
   Handle<trigger::TriggerEvent> triggerEventHandler;   
   event.getByLabel(triggerEvent_, triggerEventHandler);
   const trigger::TriggerEvent & triggerEvent = *(triggerEventHandler.product());
   const trigger::TriggerObjectCollection & triggerObjects = triggerEvent.getObjects();
   
   // The commented lines below are needed only if one wants to identify the trigger objects for the
   // trigger path passed as parameter. However, we want here to dump trigger objects that
   // are determined via config parameters regardless of the path, avoiding thus duplicated
   // entries that would happen when requiring two paths sharing same objects.
   
//   const unsigned int m(hltConfig_.size(triggerIndex));
//   const vector<string>& moduleLabels(hltConfig_.moduleLabels(triggerIndex));
//   const unsigned int moduleIndex(triggers.index(triggerIndex));
   
   for ( size_t j = 0 ; j < triggerObjectsLabels_.size() ; ++j )
   {
      const unsigned int filterIndex(triggerEvent.filterIndex(InputTag(triggerObjectsLabels_[j],"",triggerEventProcess_)));
      
      if ( filterIndex < triggerEvent.sizeFilters() )
      {
         std::cout << filterIndex << " : "  << triggerObjectsLabels_[j]  << std::endl;
         const trigger::Keys& KEYS(triggerEvent.filterKeys(filterIndex));
         const size_t nK(KEYS.size());
         for (size_t i=0; i!=nK; ++i)
         {
            const trigger::TriggerObject& TO(triggerObjects[KEYS[i]]);
            std::cout << "    " << KEYS[i] << ": " << TO.id() << " " << TO.pt() << " " << TO.eta() << " " << TO.phi() << " " << TO.mass() << std::endl;
         }
            
      }
   }
   
   
}


// ------------ method called once each job just before starting event loop  ------------
void HltTriggerSummary::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void HltTriggerSummary::endJob() 
{
}


void HltTriggerSummary::beginRun(const edm::Run & run, const edm::EventSetup & setup)
{
   // MAIN STUFF
   // to be called every run
   bool changed(true);
   // initialise the holders of trigger information
   hltPrescaleProvider_.init(run, setup, triggerResults_.process(), changed);
   hltConfig_ = hltPrescaleProvider_.hltConfigProvider();
   // -------------

}


// ------------ method called when ending the processing of a run  ------------

void HltTriggerSummary::endRun(edm::Run const& run, edm::EventSetup const& setup)
{
   
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HltTriggerSummary::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HltTriggerSummary);
