// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Ntuplizer
// 
/**\class Ntuplizer Ntuplizer.cc Analysis/Ntuplizer/plugins/Ntuplizer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh
//         Created:  Mon, 20 Oct 2014 11:54:54 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Analysis/Ntuplizer/interface/EventInfo.h"
#include "Analysis/Ntuplizer/interface/PileupInfo.h"
#include "Analysis/Ntuplizer/interface/Candidates.h"

#include "DataFormats/Common/interface/OwnVector.h"

#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

typedef std::vector<edm::InputTag> InputTags;
typedef std::vector<std::string> strings;

typedef mssmhbb::ntuple::EventInfo EventInfo;
typedef mssmhbb::ntuple::PileupInfo PileupInfo;
typedef mssmhbb::ntuple::Candidates<l1extra::L1JetParticle> L1JetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::CaloJet> CaloJetCandidates;

typedef std::unique_ptr<EventInfo> pEventInfo;
typedef std::unique_ptr<PileupInfo> pPileupInfo;
typedef std::unique_ptr<L1JetCandidates> pL1JetCandidates;
typedef std::unique_ptr<CaloJetCandidates> pCaloJetCandidates;

//
// class declaration
//

class Ntuplizer : public edm::EDAnalyzer {
   public:
      explicit Ntuplizer(const edm::ParameterSet&);
      ~Ntuplizer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      edm::ParameterSet config_;
      
      bool do_mc_;
      bool do_l1jets_;
      bool do_calojets_;
      bool do_pileupinfo_;
      std::vector< std::string > inputTags_;
      
      std::map<std::string, TTree*> trees_; // using pointers instead of smart pointers, could not Fill() with smart pointer???

      // Ntuple stuff
      pEventInfo eventinfo_;
      pPileupInfo pileupinfo_;
      
      std::vector<pL1JetCandidates> l1jets_collections_;
      std::vector<pCaloJetCandidates> calojets_collections_;
      
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
Ntuplizer::Ntuplizer(const edm::ParameterSet& config) //:   // initialization of ntuple classes
{
   //now do what ever initialization is needed
   do_mc_     = config.getParameter<bool> ("MonteCarlo");
   inputTags_ = config.getParameterNamesForType<InputTags>();
   
   config_  = config;
   
}


Ntuplizer::~Ntuplizer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void Ntuplizer::analyze(const edm::Event& event, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   // Event info
   eventinfo_ -> fill(event);
   
   if ( do_pileupinfo_ )
      pileupinfo_ -> fill(event);

   if ( do_mc_ )
   {
      // MC only stuff
   }
   
   // L1 jets
   if ( do_l1jets_ )
   {
      for ( size_t i = 0; i < l1jets_collections_.size() ; ++i )
      {
         l1jets_collections_[i]  -> fill(event);
      }
   }
   
   // Calo jets (reco)
   if ( do_calojets_ )
   {
      for ( size_t i = 0; i < calojets_collections_.size() ; ++i )
      {
         calojets_collections_[i]  -> fill(event);
      }
   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
Ntuplizer::beginJob()
{
   do_pileupinfo_ = false;
   do_l1jets_     = false;
   do_calojets_   = false;
   
   edm::Service<TFileService> fs;
   
   // Event info tree
   std::string name = "EventInfo";
   trees_[name] = fs->make<TTree>(name.c_str(),name.c_str());
   eventinfo_ = pEventInfo (new EventInfo(trees_[name]));
   eventinfo_ -> branches();
  
   // Input tags trees
   for ( strings::iterator inputTag = inputTags_.begin(); inputTag != inputTags_.end() ; ++inputTag )
   {
      InputTags collections = config_.getParameter<InputTags>((*inputTag));
      for ( InputTags::iterator collection = collections.begin(); collection != collections.end() ; ++collection)
      {
         // Names for the trees, from inputs
         std::string label = (*collection).label();
         name  = label;
         if ( (*collection).instance() != "" && collections.size() > 1 )
            name += "_" + (*collection).instance();
         
         // Initialise trees
         trees_[name] = fs->make<TTree>(name.c_str(),name.c_str());
         
         // Pileup Info
         if ( (*inputTag) == "PileupInfo" )
         {
            do_pileupinfo_ = true;
            pileupinfo_ = pPileupInfo( new PileupInfo((*collection), trees_[name]) );
            pileupinfo_ -> branches();
         }
         // L1 Jets
         if ( (*inputTag) == "L1ExtraJets" )
         {
            do_l1jets_ = true;
            // renaming tree for L1 jest as there is no explicit indication those are L1 jets objects
            std::string l1name = name + "Jets";
            trees_[name]->SetNameTitle(l1name.c_str(),l1name.c_str());
            l1jets_collections_.push_back( pL1JetCandidates( new L1JetCandidates((*collection), trees_[name]) ));
            l1jets_collections_.back() -> branches();
         }
         
         // Calo Jets
         if ( (*inputTag) == "CaloJets" )
         {
            do_calojets_ = true;
            calojets_collections_.push_back( pCaloJetCandidates( new CaloJetCandidates((*collection), trees_[name]) ));
            calojets_collections_.back() -> branches();
         }
      }
   }
   
   // MC
   if ( do_mc_ )
   {
      // MC-only stuff
   }
   
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Ntuplizer::endJob() 
{
   
}

// ------------ method called when starting to processes a run  ------------
/*
void 
Ntuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
Ntuplizer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
Ntuplizer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
Ntuplizer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Ntuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Ntuplizer);
