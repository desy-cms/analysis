// -*- C++ -*-
//
// Package:    MssmHbbAnalysis/Ntuplizer
// Class:      Ntuplizer
// 
/**\class Ntuplizer Ntuplizer.cc MssmHbbAnalysis/Ntuplizer/plugins/Ntuplizer.cc

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
#include "DataFormats/Provenance/interface/Provenance.h"

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

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/JetReco/interface/GenJet.h"

#include "MssmHbbAnalysis/Ntuplizer/interface/EventInfo.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/PileupInfo.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/Candidates.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/JetsTags.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/TriggerAccepts.h"

#include "DataFormats/Common/interface/OwnVector.h"

#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

typedef std::vector<edm::InputTag> InputTags;
typedef std::vector<std::string> strings;

// Alias to the collections classes of candidates for the ntuple
typedef mssmhbb::ntuple::EventInfo EventInfo;
typedef mssmhbb::ntuple::PileupInfo PileupInfo;
typedef mssmhbb::ntuple::Candidates<l1extra::L1JetParticle> L1JetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::CaloJet> CaloJetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::PFJet> PFJetCandidates;
typedef mssmhbb::ntuple::Candidates<pat::Jet> PatJetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::GenJet> GenJetCandidates;
typedef mssmhbb::ntuple::JetsTags JetsTags;
typedef mssmhbb::ntuple::TriggerAccepts TriggerAccepts;

// Alias to the pointers to the above classes
typedef std::unique_ptr<EventInfo> pEventInfo;
typedef std::unique_ptr<PileupInfo> pPileupInfo;
typedef std::unique_ptr<L1JetCandidates> pL1JetCandidates;
typedef std::unique_ptr<CaloJetCandidates> pCaloJetCandidates;
typedef std::unique_ptr<PFJetCandidates> pPFJetCandidates;
typedef std::unique_ptr<PatJetCandidates> pPatJetCandidates;
typedef std::unique_ptr<GenJetCandidates> pGenJetCandidates;
typedef std::unique_ptr<JetsTags> pJetsTags;
typedef std::unique_ptr<TriggerAccepts> pTriggerAccepts;

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
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      edm::ParameterSet config_;
      
      bool do_mc_;
      bool do_l1jets_;
      bool do_calojets_;
      bool do_pfjets_;
      bool do_patjets_;
      bool do_genjets_;
      bool do_jetstags_;
      bool do_pileupinfo_;
      bool do_triggeraccepts_;
      std::vector< std::string > inputTags_;
      
      std::map<std::string, TTree*> trees_; // using pointers instead of smart pointers, could not Fill() with smart pointer???

      // Ntuple stuff
      pEventInfo eventinfo_;
      pPileupInfo pileupinfo_;
      
      // Collections for the ntuples
      std::vector<pL1JetCandidates> l1jets_collections_;
      std::vector<pCaloJetCandidates> calojets_collections_;
      std::vector<pPFJetCandidates> pfjets_collections_;
      std::vector<pPatJetCandidates> patjets_collections_;
      std::vector<pGenJetCandidates> genjets_collections_;
      std::vector<pJetsTags> jetstags_collections_;
      std::vector<pTriggerAccepts> triggeraccepts_collections_;
      
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
   
//    typedef std::vector<Provenance const*> Provenances;
//    Provenances provenances;
//    event.getAllProvenance(provenances);
//    
//    for(Provenances::iterator itProv = provenances.begin(), itProvEnd = provenances.end();
//                              itProv != itProvEnd;
//                            ++itProv) {
//       std::cout << (*itProv)->moduleLabel() << std::endl;
//                            }
//    
   // Event info
   eventinfo_ -> Fill(event);
   
   if ( do_pileupinfo_ )
      pileupinfo_ -> Fill(event);

   if ( do_mc_ )
   {
      // MC only stuff
   }
   
   // L1 jets
   if ( do_l1jets_ )
   {
      for ( size_t i = 0; i < l1jets_collections_.size() ; ++i )
      {
         l1jets_collections_[i]  -> Fill(event);
      }
   }
   
   // Calo jets (reco)
   if ( do_calojets_ )
   {
      for ( size_t i = 0; i < calojets_collections_.size() ; ++i )
      {
         calojets_collections_[i]  -> Fill(event);
      }
   }

   // PF jets (reco)
   if ( do_pfjets_ )
   {
      for ( size_t i = 0; i < pfjets_collections_.size() ; ++i )
      {
         pfjets_collections_[i]  -> Fill(event);
      }
   }

      // Pat jets (pat)
   if ( do_patjets_ )
   {
      for ( size_t i = 0; i < patjets_collections_.size() ; ++i )
      {
         patjets_collections_[i]  -> Fill(event);
      }
   }
   
      // Gen jets (reco)
   if ( do_genjets_ )
   {
      for ( size_t i = 0; i < genjets_collections_.size() ; ++i )
      {
         genjets_collections_[i]  -> Fill(event);
      }
   }
      // jets tags
   if ( do_jetstags_ )
   {
      for ( size_t i = 0; i < jetstags_collections_.size() ; ++i )
      {
         jetstags_collections_[i]  -> Fill(event);
      }
   }
      // trigger accecpts
   if ( do_triggeraccepts_ )
   {
      for ( size_t i = 0; i < triggeraccepts_collections_.size() ; ++i )
      {
         triggeraccepts_collections_[i]  -> Fill(event);
      }
   }
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
Ntuplizer::beginJob()
{
   do_pileupinfo_     = config_.exists("PileupInfo");
   do_l1jets_         = config_.exists("L1ExtraJets");
   do_calojets_       = config_.exists("CaloJets");
   do_pfjets_         = config_.exists("PFJets");
   do_patjets_        = config_.exists("PatJets");
   do_genjets_        = config_.exists("GenJets");
   do_jetstags_       = config_.exists("JetsTags");
   do_triggeraccepts_ = config_.exists("TriggerResults") && config_.exists("TriggerPaths");
   
   
   edm::Service<TFileService> fs;
   
   // Metadata 
   std::string name = "Metadata";
   
   
   // Event info tree
   name = "EventInfo";
   trees_[name] = fs->make<TTree>(name.c_str(),name.c_str());
   eventinfo_ = pEventInfo (new EventInfo(trees_[name]));
   eventinfo_ -> Branches();
  
   // Input tags 
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
         if ( do_triggeraccepts_ && (*inputTag) == "TriggerResults" )
            name  += "_" + (*collection).process();
         
         
         // Initialise trees
         trees_[name] = fs->make<TTree>(name.c_str(),name.c_str());
         
         // Pileup Info
         if ( (*inputTag) == "PileupInfo" )
         {
            pileupinfo_ = pPileupInfo( new PileupInfo((*collection), trees_[name]) );
            pileupinfo_ -> Branches();
         }
         // L1 Jets
         if ( (*inputTag) == "L1ExtraJets" )
         {
            // renaming tree for L1 jest as there is no explicit indication those are L1 jets objects
            std::string l1name = name + "Jets";
            trees_[name]->SetNameTitle(l1name.c_str(),l1name.c_str());
            l1jets_collections_.push_back( pL1JetCandidates( new L1JetCandidates((*collection), trees_[name]) ));
            l1jets_collections_.back() -> Branches();
         }
         
         // Calo Jets
         if ( (*inputTag) == "CaloJets" )
         {
            calojets_collections_.push_back( pCaloJetCandidates( new CaloJetCandidates((*collection), trees_[name]) ));
            calojets_collections_.back() -> Branches();
         }
         // PF Jets
         if ( (*inputTag) == "PFJets" )
         {
            pfjets_collections_.push_back( pPFJetCandidates( new PFJetCandidates((*collection), trees_[name]) ));
            pfjets_collections_.back() -> Branches();
         }
         // Pat Jets
         if ( (*inputTag) == "PatJets" )
         {
            patjets_collections_.push_back( pPatJetCandidates( new PatJetCandidates((*collection), trees_[name]) ));
            patjets_collections_.back() -> Branches();
         }
         // Gen Jets
         if ( (*inputTag) == "GenJets" )
         {
            genjets_collections_.push_back( pGenJetCandidates( new GenJetCandidates((*collection), trees_[name]) ));
            genjets_collections_.back() -> Branches();
         }
         // Jets Tags
         if ( (*inputTag) == "JetsTags" )
         {
            jetstags_collections_.push_back( pJetsTags( new JetsTags((*collection), trees_[name]) ));
            jetstags_collections_.back() -> Branches();
         }
         // Trigger Accepts
         if ( do_triggeraccepts_ && (*inputTag) == "TriggerResults" )
         {
            // TriggerResults collections names differ by the process, so add it to the name
            std::vector< std::string> trigger_paths = config_.getParameter< std::vector< std::string> >("TriggerPaths");
            triggeraccepts_collections_.push_back( pTriggerAccepts( new TriggerAccepts((*collection), trees_[name], trigger_paths) ));
            triggeraccepts_collections_.back() -> Branches();
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

void  Ntuplizer::beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
   // Initialize HLTConfig every lumi block
   if ( do_triggeraccepts_ )
   {
      for ( size_t i = 0; i < triggeraccepts_collections_.size() ; ++i )
      {
         triggeraccepts_collections_[i]  -> LumiBlock(lumi,setup);
      }
   }
}


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
