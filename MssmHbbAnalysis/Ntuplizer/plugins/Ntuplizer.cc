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
#include <boost/algorithm/string.hpp>
#include <type_traits>

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

#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "MssmHbbAnalysis/Ntuplizer/interface/EventInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/PileupInfo.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/Candidates.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/JetsTags.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/TriggerAccepts.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/PrimaryVertices.h"

#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/Common/interface/MergeableCounter.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "MssmHbbAnalysis/Ntuplizer/interface/FilterEfficiency.h"

#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

using namespace boost;
using namespace boost::algorithm;


typedef std::vector<edm::InputTag> InputTags;
typedef std::vector<std::string> strings;

// Alias to the collections classes of candidates for the ntuple
typedef mssmhbb::ntuple::EventInfo EventInfo;
typedef mssmhbb::ntuple::PileupInfo PileupInfo;
typedef mssmhbb::ntuple::Candidates<l1extra::L1JetParticle> L1JetCandidates;
typedef mssmhbb::ntuple::Candidates<l1extra::L1MuonParticle> L1MuonCandidates;
typedef mssmhbb::ntuple::Candidates<reco::CaloJet> CaloJetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::PFJet> PFJetCandidates;
typedef mssmhbb::ntuple::Candidates<pat::Jet> PatJetCandidates;
typedef mssmhbb::ntuple::Candidates<pat::Muon> PatMuonCandidates;
typedef mssmhbb::ntuple::Candidates<reco::GenJet> GenJetCandidates;
typedef mssmhbb::ntuple::Candidates<reco::GenParticle> GenParticleCandidates;
typedef mssmhbb::ntuple::Candidates<pat::TriggerObject> TriggerObjectCandidates;
typedef mssmhbb::ntuple::JetsTags JetsTags;
typedef mssmhbb::ntuple::TriggerAccepts TriggerAccepts;
typedef mssmhbb::ntuple::PrimaryVertices PrimaryVertices;

typedef mssmhbb::ntuple::FilterEfficiency<GenFilterInfo> GeneratorFilterEfficiency;
typedef mssmhbb::ntuple::FilterEfficiency<edm::MergeableCounter> EventFilterEfficiency;


// Alias to the pointers to the above classes
typedef std::unique_ptr<EventInfo> pEventInfo;
typedef std::unique_ptr<PileupInfo> pPileupInfo;
typedef std::unique_ptr<L1JetCandidates> pL1JetCandidates;
typedef std::unique_ptr<L1MuonCandidates> pL1MuonCandidates;
typedef std::unique_ptr<CaloJetCandidates> pCaloJetCandidates;
typedef std::unique_ptr<PFJetCandidates> pPFJetCandidates;
typedef std::unique_ptr<PatJetCandidates> pPatJetCandidates;
typedef std::unique_ptr<PatMuonCandidates> pPatMuonCandidates;
typedef std::unique_ptr<GenJetCandidates> pGenJetCandidates;
typedef std::unique_ptr<GenParticleCandidates> pGenParticleCandidates;
typedef std::unique_ptr<TriggerObjectCandidates> pTriggerObjectCandidates;
typedef std::unique_ptr<JetsTags> pJetsTags;
typedef std::unique_ptr<TriggerAccepts> pTriggerAccepts;
typedef std::unique_ptr<PrimaryVertices> pPrimaryVertices;

typedef std::unique_ptr<GeneratorFilterEfficiency> pGeneratorFilterEfficiency;
typedef std::unique_ptr<EventFilterEfficiency> pEventFilterEfficiency;

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
      virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      edm::ParameterSet config_;
      
      bool is_mc_;
      bool use_full_name_;
      bool do_l1jets_;
      bool do_l1muons_;
      bool do_calojets_;
      bool do_pfjets_;
      bool do_patjets_;
      bool do_patmuons_;
      bool do_genjets_;
      bool do_genparticles_;
      bool do_jetstags_;
      bool do_pileupinfo_;
      bool do_triggeraccepts_;
      bool do_primaryvertices_;
      bool do_eventfilter_;
      bool do_genfilter_;
      bool do_triggerobjects_;
      std::vector< std::string > inputTagsVec_;
      std::vector< std::string > inputTags_;
      std::vector< std::string > btagAlgos_;
      std::vector< std::string > btagAlgosAlias_;
      std::vector< std::string > triggerObjectLabels_;
      
      
      edm::InputTag genFilterInfo_;
      InputTags eventCounters_;
      
      std::map<std::string, TTree*> tree_; // using pointers instead of smart pointers, could not Fill() with smart pointer???

      // Ntuple stuff
      pEventInfo eventinfo_;
      pPileupInfo pileupinfo_;
      
      // Collections for the ntuples (vector)
      std::vector<pL1JetCandidates> l1jets_collections_;
      std::vector<pL1MuonCandidates> l1muons_collections_;
      std::vector<pCaloJetCandidates> calojets_collections_;
      std::vector<pPFJetCandidates> pfjets_collections_;
      std::vector<pPatJetCandidates> patjets_collections_;
      std::vector<pPatMuonCandidates> patmuons_collections_;
      std::vector<pGenJetCandidates> genjets_collections_;
      std::vector<pGenParticleCandidates> genparticles_collections_;
      std::vector<pJetsTags> jetstags_collections_;
      std::vector<pPrimaryVertices> primaryvertices_collections_;
      std::vector<pTriggerAccepts> triggeraccepts_collections_;
      std::vector<pTriggerObjectCandidates> triggerobjects_collections_;
      
      pGeneratorFilterEfficiency genfilter_collection_;
      pEventFilterEfficiency eventfilter_collection_;
      
      
      // Collections for the ntuples (single)
      
      // metadata
      double xsection_;
      
      mssmhbb::ntuple::FilterResults eventFilterResults_;
      mssmhbb::ntuple::FilterResults genFilterResults_;
      
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
   is_mc_         = config.getParameter<bool> ("MonteCarlo");
   use_full_name_ = true;
   inputTagsVec_ = config.getParameterNamesForType<InputTags>();
   inputTags_    = config.getParameterNamesForType<edm::InputTag>();
   
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

   if ( is_mc_ )
   {
      // MC only stuff
   }
   
   // L1 jets
      for ( auto & collection : l1jets_collections_ )
         collection -> Fill(event);
   
   // L1 muons
      for ( auto & collection : l1muons_collections_ )
         collection -> Fill(event);
   
   // Calo jets (reco)
      for ( auto & collection : calojets_collections_ )
         collection -> Fill(event);

   // PF jets (reco)
      for ( auto & collection : pfjets_collections_ )
         collection -> Fill(event);

      // Pat jets (pat)
      for ( auto & collection : patjets_collections_ )
         collection -> Fill(event);
   
      // Pat muon (pat)
      for ( auto & collection : patmuons_collections_ )
         collection -> Fill(event);
   
      // Gen jets (reco)
      for ( auto & collection : genjets_collections_ )
         collection -> Fill(event);
      
      // Gen particles (reco)
      for ( auto & collection : genparticles_collections_ )
         collection -> Fill(event);
      
      // jets tags
      for ( auto & collection : jetstags_collections_ )
         collection -> Fill(event);
      
      // trigger accecpts
      for ( auto & collection : triggeraccepts_collections_ )
         collection -> Fill(event);
      
      // primary vertices
      for ( auto & collection : primaryvertices_collections_ )
         collection -> Fill(event);
   
      // trigger objects
      for ( auto & collection : triggerobjects_collections_ )
         collection -> Fill(event);
}


// ------------ method called once each job just before starting event loop  ------------
void 
Ntuplizer::beginJob()
{
   do_pileupinfo_       = config_.exists("PileupInfo");
   do_l1jets_           = config_.exists("L1ExtraJets");
   do_l1muons_          = config_.exists("L1ExtraMuons");
   do_calojets_         = config_.exists("CaloJets");
   do_pfjets_           = config_.exists("PFJets");
   do_patjets_          = config_.exists("PatJets");
   do_patmuons_         = config_.exists("PatMuons");
   do_genjets_          = config_.exists("GenJets");
   do_genparticles_     = config_.exists("GenParticles");
   do_jetstags_         = config_.exists("JetsTags");
   do_triggeraccepts_   = config_.exists("TriggerResults") && config_.exists("TriggerPaths");
   do_primaryvertices_  = config_.exists("PrimaryVertices");
   do_eventfilter_      = config_.exists("EventFilter");
   do_genfilter_        = config_.exists("GenFilterInfo");
   do_triggerobjects_   = config_.exists("TriggerObjectStandAlone") &&  config_.exists("TriggerObjectLabels");
   
   if ( config_.exists("UseFullName") )
      use_full_name_ = config_.getParameter<bool> ("UseFullName");

   
   edm::Service<TFileService> fs;
//   TFileDirectory triggerObjectsDir;
//    std::cout << std::boolalpha;
//    std::cout << "oioioi  rvalue " << std::is_rvalue_reference< edm::Service<TFileService> >::value << std::endl;
//    std::cout << "oioioi  lvalue " << std::is_lvalue_reference< edm::Service<TFileService> >::value << std::endl;
//    std::cout << "oioioi  object " << std::is_object< edm::Service<TFileService> >::value << std::endl;

   
   std::string name;
   std::string fullname;
   
   // Metadata 
   name = "Metadata";
   tree_[name] = fs -> make<TTree>(name.c_str(),name.c_str());
   // cross section
   tree_[name] -> Branch("xsection"        , &xsection_        , "xsection/D");
   // event filter
   tree_[name] -> Branch("nEventsTotal"    , &eventFilterResults_.total     , "nEventsTotal/i");
   tree_[name] -> Branch("nEventsFiltered" , &eventFilterResults_.filtered  , "nEventsFiltered/i");
   tree_[name] -> Branch("filterEfficiency", &eventFilterResults_.efficiency, "filterEfficiency/D");
   // generator filter
   tree_[name] -> Branch("nGenEventsTotal"    , &genFilterResults_.total     , "nGenEventsTotal/i");
   tree_[name] -> Branch("nGenEventsFiltered" , &genFilterResults_.filtered  , "nGenEventsFiltered/i");
   tree_[name] -> Branch("genFilterEfficiency", &genFilterResults_.efficiency, "genFilterEfficiency/D");
   
   xsection_ = -1.0;
   
   genFilterResults_  = {};
   eventFilterResults_ = {};
   
   if ( config_.exists("CrossSection") )
      xsection_ = config_.getParameter<double>("CrossSection");
   
   // Btagging algorithms
   // Will set one default
   btagAlgos_.clear();
   btagAlgosAlias_.clear();
   btagAlgos_.push_back("pfCombinedInclusiveSecondaryVertexV2BJetTags");
   btagAlgosAlias_.push_back("btag_csvivf");
   if ( config_.exists("BTagAlgorithmsAlias") )
   {
      btagAlgosAlias_.clear();
      btagAlgosAlias_ = config_.getParameter< std::vector<std::string> >("BTagAlgorithmsAlias");
   }
   if ( config_.exists("BTagAlgorithms") )
   {
      btagAlgos_.clear();
      btagAlgos_ = config_.getParameter< std::vector<std::string> >("BTagAlgorithms");
   }
   if ( btagAlgos_.size() != btagAlgosAlias_.size() )
   {
      // if user put the wrong number of alias, then use the algo name as alias
      btagAlgosAlias_.clear();
      for ( auto& it : btagAlgos_ )
         btagAlgosAlias_.push_back(it);
   }
   
   // Definitions
   name = "Definitions";
   tree_[name] = fs -> make<TTree>(name.c_str(),name.c_str());
   // btag algorihtms
   for ( size_t i = 0; i < btagAlgos_.size() ; ++i )
   {
      tree_[name] ->Branch(btagAlgosAlias_[i].c_str(),(void*)btagAlgos_[i].c_str(),"string/C",1024);
   }
   tree_[name] -> Fill();

   
   // Event info tree
   eventinfo_ = pEventInfo (new EventInfo(fs));
   
   // Input tags (vector)
   for ( auto & inputTags : inputTagsVec_ )
   {
      InputTags collections = config_.getParameter<InputTags>(inputTags);
      for ( auto & collection : collections )
      {
         // Names for the trees, from inputs
         std::string label = collection.label();
         std::string inst  = collection.instance();
         std::string proc  = collection.process();
         name = label;
         if ( find_first(inputTags,"L1Extra") )
         {
            // renaming tree for L1 jest as there is no explicit indication those are L1 jets objects
            std::string l1obj = inputTags;
            erase_first(l1obj,"L1Extra");
            name += l1obj;
         }
         fullname = name + "_" + inst + "_" + proc;
         name += inputTags == "L1ExtraJets" && ! use_full_name_ ? "_" + inst : "";
         if ( collection.instance() != "" && collections.size() > 1 )
            name += "_" + inst;
         if ( use_full_name_ ) name = fullname;
         
         // Initialise trees
         if ( inputTags != "TriggerObjectStandAlone" )
            tree_[name] = fs->make<TTree>(name.c_str(),fullname.c_str());
         
         // Pileup Info
         if ( inputTags == "PileupInfo" )
         {
            pileupinfo_ = pPileupInfo( new PileupInfo(collection, tree_[name]) );
            pileupinfo_ -> Branches();
         }
         
         // L1 Jets
         if ( inputTags == "L1ExtraJets" )
         {
            l1jets_collections_.push_back( pL1JetCandidates( new L1JetCandidates(collection, tree_[name], is_mc_, 5.,5. ) ));
            l1jets_collections_.back() -> Init();
         }
         
         // L1 Muons
         if ( inputTags == "L1ExtraMuons" )
         {
            l1muons_collections_.push_back( pL1MuonCandidates( new L1MuonCandidates(collection, tree_[name], is_mc_, 0.,2.5 ) ));
            l1muons_collections_.back() -> Init();
         }
         
         // Calo Jets
         if ( inputTags == "CaloJets" )
         {
            calojets_collections_.push_back( pCaloJetCandidates( new CaloJetCandidates(collection, tree_[name], is_mc_, 10.,5. ) ));
            calojets_collections_.back() -> Init();
         }
         // PF Jets
         if ( inputTags == "PFJets" )
         {
            pfjets_collections_.push_back( pPFJetCandidates( new PFJetCandidates(collection, tree_[name], is_mc_, 10.,5. ) ));
            pfjets_collections_.back() -> Init();
         }
         // Pat Jets
         if ( inputTags == "PatJets" )
         {
            patjets_collections_.push_back( pPatJetCandidates( new PatJetCandidates(collection, tree_[name], is_mc_, 10, 5. ) ));
            patjets_collections_.back() -> Init(btagAlgos_, btagAlgosAlias_);
         }
         // Pat Muons
         if ( inputTags == "PatMuons" )
         {
            patmuons_collections_.push_back( pPatMuonCandidates( new PatMuonCandidates(collection, tree_[name], is_mc_ ,5., 2.5) ));
            patmuons_collections_.back() -> Init();
         }
         // Gen Jets
         if ( inputTags == "GenJets" )
         {
            genjets_collections_.push_back( pGenJetCandidates( new GenJetCandidates(collection, tree_[name], is_mc_, 10., 5. ) ));
            genjets_collections_.back() -> Init();
         }
         // Gen Particles
         if ( inputTags == "GenParticles" )
         {
            genparticles_collections_.push_back( pGenParticleCandidates( new GenParticleCandidates(collection, tree_[name], is_mc_ ) ));
            genparticles_collections_.back() -> Init();
        }
         // Jets Tags
         if ( inputTags == "JetsTags" )
         {
            jetstags_collections_.push_back( pJetsTags( new JetsTags(collection, tree_[name]) ));
            jetstags_collections_.back() -> Branches();
         }
         
               // Trigger Objects
         if ( do_triggerobjects_ && inputTags == "TriggerObjectStandAlone"  )
         {
            if ( triggerObjectLabels_.empty() )
               triggerObjectLabels_ = config_.getParameter< std::vector<std::string> >("TriggerObjectLabels");
            std::string dir = name;
            TFileDirectory triggerObjectsDir = fs -> mkdir(dir);
      
            for ( auto & triggerObjectLabel : triggerObjectLabels_ )
            {
               name = triggerObjectLabel;
               if ( use_full_name_ ) name += "_" + dir;
               tree_[name] = triggerObjectsDir.make<TTree>(name.c_str(),name.c_str());
               triggerobjects_collections_.push_back(pTriggerObjectCandidates( new TriggerObjectCandidates(collection, tree_[name], is_mc_ ) ));
               triggerobjects_collections_.back() -> Init();
            }
         }
         
         // Trigger Accepts
         if ( do_triggeraccepts_ && inputTags == "TriggerResults" )
         {
            // TriggerResults collections names differ by the process, so add it to the name
            std::vector< std::string> trigger_paths = config_.getParameter< std::vector< std::string> >("TriggerPaths");
            triggeraccepts_collections_.push_back( pTriggerAccepts( new TriggerAccepts(collection, tree_[name], trigger_paths) ));
            triggeraccepts_collections_.back() -> Branches();
         }
         // Primary Vertices
         if ( inputTags == "PrimaryVertices" )
         {
            primaryvertices_collections_.push_back( pPrimaryVertices( new PrimaryVertices(collection, tree_[name]) ));
            primaryvertices_collections_.back() -> Branches();
         }
         
         // Event filter
         if ( do_eventfilter_ && inputTags == "EventFilter" )
         {
            eventCounters_.push_back(collection);
            if ( eventCounters_.size() > 1 )
            {
               std::cout << "Ntuplizer::BeginJob() - Warning: you gave more than two collections for the event filter calculation." << std::endl;
               std::cout << "                                 Only the first two collections will be used." << std::endl;
               eventfilter_collection_ = pEventFilterEfficiency( new EventFilterEfficiency(eventCounters_));
               break;
            }
         }
         
      }
   }
      
   
   // InputTag (single, i.e. not vector)
   
   for ( auto & inputTag : inputTags_ )
   {
      edm::InputTag collection = config_.getParameter<edm::InputTag>(inputTag);
      std::string label = collection.label();
         
      // Generator filter
      if ( do_genfilter_ && inputTag == "GenFilterInfo" )
      {
         genFilterInfo_  = config_.getParameter<edm::InputTag> ("GenFilterInfo");
         genfilter_collection_ = pGeneratorFilterEfficiency( new GeneratorFilterEfficiency({genFilterInfo_} ));
      }

   } 

}

// ------------ method called once each job just after ending the event loop  ------------
void 
Ntuplizer::endJob() 
{
   if ( do_genfilter_ )
      genFilterResults_   = genfilter_collection_->Results();
   if ( do_eventfilter_ )
      eventFilterResults_ = eventfilter_collection_->Results();
   
   tree_["Metadata"] -> Fill();
     
}

// ------------ method called when starting to processes a run  ------------
/*
void 
Ntuplizer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------

void 
Ntuplizer::endRun(edm::Run const& run, edm::EventSetup const& setup)
{
   if ( xsection_ < 0. )
   {
      edm::Handle<GenRunInfoProduct> genRunInfo;
      run.getByLabel( "generator", genRunInfo );
      xsection_ = genRunInfo->internalXSec().value();
   }
}

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

void 
Ntuplizer::endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
   if ( do_eventfilter_ )
      eventfilter_collection_ -> Increment(lumi);
   
   if ( do_genfilter_ )
      genfilter_collection_ -> Increment(lumi);
   
}


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
