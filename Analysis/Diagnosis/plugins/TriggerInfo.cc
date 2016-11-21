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
      bool first_;
      std::string pathName_;
      edm::InputTag triggerResults_;
      edm::InputTag triggerObjectsStandAlone_;
   
      edm::EDGetTokenT<edm::TriggerResults> triggerResultsTokens_;
      edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> triggerObjectsStandAloneTokens_;
      
      HLTPrescaleProvider hltPrescaleProvider_;
      HLTConfigProvider hltConfig_;
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
TriggerInfo::TriggerInfo(const edm::ParameterSet& config):
   first_(true),
   pathName_(config.getParameter<std::string> ("PathName")),
   triggerResults_(config.getParameter<edm::InputTag> ("TriggerResults")),
   triggerObjectsStandAlone_ (config.getParameter<edm::InputTag> ("TriggerObjectsStandAlone")),
   triggerResultsTokens_(consumes<edm::TriggerResults>(triggerResults_)),
   triggerObjectsStandAloneTokens_(consumes<pat::TriggerObjectStandAloneCollection>(triggerObjectsStandAlone_)),
   hltPrescaleProvider_(config, consumesCollector(), *this)
{
   //now do what ever initialization is needed
   
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
void TriggerInfo::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   Handle<TriggerResults> triggerResultsHandler;
   event.getByLabel(triggerResults_, triggerResultsHandler);
   const TriggerResults & triggers = *(triggerResultsHandler.product());   
   
   // Loop over all triggers
   std::vector<std::string> triggerNames = hltConfig_.triggerNames();
   for ( size_t i = 0 ; i < triggerNames.size() ; ++i )
   {
      if ( triggerNames[i] == pathName_ )
      {
         std::cout << "Path " << triggerNames[i];
         if ( triggers.accept(i) ) std::cout << " has fired!" << std::endl;
         else                      std::cout << " has not fired!" << std::endl;
         const std::pair<std::vector<std::pair<std::string,int> >,int> ps = hltPrescaleProvider_.prescaleValuesInDetail(event,setup,triggerNames[i]);
         std::cout << "     L1 prescales (in details) " << std::endl;
         for ( size_t j = 0; j < ps.first.size() ; ++j )
         {
            std::cout << "         " << ps.first[j].first << "  " << ps.first[j].second << std::endl;
         }
         std::cout << "     HLT prescale (in details) " << ps.second << std::endl;
         
      }
   }
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
   // MAIN STUFF
   // to be called every run
   bool changed(true);
   // initialise the holders of trigger information
   hltPrescaleProvider_.init(run, setup, triggerResults_.process(), changed);
   hltConfig_ = hltPrescaleProvider_.hltConfigProvider();
   // -------------
   
   // PRINT NAMES AND OBJECTS FOR ALL TRIGGERS
   // be careful! not supposed to be used directly in an analysis, need further changes in the code
   // this info may change if config changes but it will be printed for the first event only 
   if ( first_ )
   {
   
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
         // L1 seeds (be aware that for 2016 prompt reco the L1 information is not correct, check WBM)
         const std::vector< std::pair< bool, std::string > > & l1GtSeeds = hltConfig_.hltL1GTSeeds(triggerNames.at(i));
         const std::vector< std::string > & l1TSeeds = hltConfig_.hltL1TSeeds(triggerNames.at(i));
         
         // trigger objects
         const std::vector<std::string> & saveTags = hltConfig_.saveTagsModules(triggerNames.at(i));
         outfile << triggerNames.at(i) << std::endl;
         
         for ( size_t j = 0; j < l1GtSeeds.size(); ++j )
         {
            outfile << "   L1GT Seed: " << l1GtSeeds.at(j).second << ","<< std::endl;
         }
         for ( size_t j = 0; j < l1TSeeds.size(); ++j )
         {
            outfile << "   L1T Seed: " << l1TSeeds.at(j) << ","<< std::endl;
         }
         for ( size_t j = 0; j < saveTags.size() ; ++j )
         {
            outfile << "      Trigger Object:    '" << saveTags.at(j) << "'," << std::endl;
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
