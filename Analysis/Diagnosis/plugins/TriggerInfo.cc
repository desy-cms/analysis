// -*- C++ -*-
//
// Package:    Analysis/Diagnosis
// Class:      TriggerInfo
// 
/**\class TriggerInfo TriggerInfo.cc Analysis/Diagnosis/plugins/TriggerInfo.cc

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
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
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

class TriggerInfo : public edm::EDAnalyzer
{
   public:
      explicit TriggerInfo(const edm::ParameterSet&);
      ~TriggerInfo();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual void beginRun(const edm::Run&, const edm::EventSetup&) override; 
      virtual void endRun(const edm::Run&, const edm::EventSetup&) override; 

      // ----------member data ---------------------------
      edm::InputTag triggerResults_;
      edm::InputTag triggerObjectsStandAlone_;
   
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsStandAloneTokens_;
      edm::EDGetTokenT<edm::TriggerResults> triggerResultsTokens_;
      
      HLTConfigProvider hltConfig_;
      std::shared_ptr<HLTPrescaleProvider> hltPrescaleProvider_;
      
      bool first_;
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
TriggerInfo::TriggerInfo(const edm::ParameterSet& config)

{
   //now do what ever initialization is needed
   first_ = true;
   
   triggerResults_ = config.getParameter<edm::InputTag> ("TriggerResults");
   triggerObjectsStandAlone_ = config.getParameter<edm::InputTag> ("TriggerObjectsStandAlone");
   
   
   triggerResultsTokens_    = consumes<edm::TriggerResults>(triggerResults_);
   triggerObjectsStandAloneTokens_ = consumes<pat::TriggerObjectStandAloneCollection>(triggerObjectsStandAlone_);
   
   hltPrescaleProvider_ = std::shared_ptr<HLTPrescaleProvider>(new HLTPrescaleProvider(config, consumesCollector(), *this));;
}


TriggerInfo::~TriggerInfo()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void TriggerInfo::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
}


// ------------ method called once each job just before starting event loop  ------------
void TriggerInfo::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerInfo::endJob() 
{
}


void TriggerInfo::beginRun(const edm::Run & run, const edm::EventSetup & setup)
{
   bool changed;
   
   if ( first_ )
   {
   
      // initialise the holders of trigger information
      hltPrescaleProvider_ -> init(run, setup, triggerResults_.process(), changed);
      hltConfig_ = hltPrescaleProvider_ -> hltConfigProvider();
      
      std::string tablename = hltConfig_.tableName();  // menu
      std::string globaltag = hltConfig_.globalTag();  // global tag
      
      // Output the trigger paths and corresponding objects into a text file
      std::string outfileName = tablename;
      outfileName.erase(0,1);
      std::replace( outfileName.begin(), outfileName.end(), '/', '_');
      outfileName += "-";
      outfileName += globaltag;
      outfileName += ".txt";
      std::ofstream outfile(outfileName.c_str());
      
      outfile << "HLT ConfDB table name = " <<tablename << std::endl;
      outfile << "GlobalTag = " << globaltag << std::endl;
      outfile << "========================================" << std::endl;
      outfile << std::endl;
      
      // Loop over all triggers
      std::vector<std::string> triggerNames = hltConfig_.triggerNames();
      for ( size_t i = 0 ; i < triggerNames.size() ; ++i )
      {
         const std::vector< std::pair< bool, std::string > > & l1GtSeeds = hltConfig_.hltL1GTSeeds(triggerNames.at(i));
         const std::vector<std::string> & saveTags = hltConfig_.saveTagsModules(triggerNames.at(i));
         outfile << triggerNames.at(i) << std::endl;
         for ( size_t j = 0; j < l1GtSeeds.size(); ++j )
         {
            outfile << "   L1 Seed: " << l1GtSeeds.at(j).second << ","<< std::endl;
         }
         for ( size_t j = 0; j < saveTags.size() ; ++j )
         {
            outfile << "      Trigger Object:    " << saveTags.at(j) << "," << std::endl;
         }
         outfile << "----------------------------------------" << std::endl;
         outfile << std::endl;
      }
      first_ = false;
      outfile.close();
      
   }
}


// ------------ method called when ending the processing of a run  ------------

void TriggerInfo::endRun(edm::Run const& run, edm::EventSetup const& setup)
{
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerInfo::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerInfo);
