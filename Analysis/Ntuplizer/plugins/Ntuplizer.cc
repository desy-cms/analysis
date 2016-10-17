// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Ntuplizer
// 
/**\class Ntuplizer Ntuplizer.cc Analysis/Ntuplizer/plugins/Ntuplizer.cc

 Description: EDAnalyzer to produce ntuples for the CMS Higgs Analysis, meant for MSSM Hbb analysis, but it can be used in other analysis.

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
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/PatCandidates/interface/TriggerObject.h"

#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "Analysis/Ntuplizer/interface/EventInfo.h"
#include "Analysis/Ntuplizer/interface/Definitions.h"
#include "Analysis/Ntuplizer/interface/Metadata.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "Analysis/Ntuplizer/interface/PileupInfo.h"
#include "Analysis/Ntuplizer/interface/Candidates.h"
#include "Analysis/Ntuplizer/interface/JetsTags.h"
#include "Analysis/Ntuplizer/interface/TriggerAccepts.h"
#include "Analysis/Ntuplizer/interface/Vertices.h"

#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "DataFormats/Common/interface/MergeableCounter.h"

#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


#include "Analysis/Ntuplizer/interface/EventFilter.h"
#include "Analysis/Ntuplizer/interface/Utils.h"

#include <TH1.h>
#include <TFile.h>
#include <TTree.h>




#ifndef CMSSWOLD
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#endif

using namespace boost;
using namespace boost::algorithm;

typedef analysis::ntuple::TitleIndex TitleIndex;
typedef analysis::ntuple::TitleAlias TitleAlias;

typedef std::vector<edm::InputTag> InputTags;
typedef std::vector<std::string> strings;

// Alias to the collections classes of candidates for the ntuple
typedef analysis::ntuple::EventInfo EventInfo;
typedef analysis::ntuple::Metadata Metadata;
typedef analysis::ntuple::Definitions Definitions;
typedef analysis::ntuple::PileupInfo PileupInfo;
typedef analysis::ntuple::Candidates<l1extra::L1JetParticle> L1JetCandidates;
typedef analysis::ntuple::Candidates<l1extra::L1MuonParticle> L1MuonCandidates;
typedef analysis::ntuple::Candidates<reco::CaloJet> CaloJetCandidates;
typedef analysis::ntuple::Candidates<reco::PFJet> PFJetCandidates;
typedef analysis::ntuple::Candidates<pat::Jet> PatJetCandidates;
typedef analysis::ntuple::Candidates<pat::MET> PatMETCandidates;
typedef analysis::ntuple::Candidates<pat::Muon> PatMuonCandidates;
typedef analysis::ntuple::Candidates<reco::GenJet> GenJetCandidates;
typedef analysis::ntuple::Candidates<reco::GenParticle> GenParticleCandidates;
typedef analysis::ntuple::Candidates<pat::TriggerObject> TriggerObjectCandidates;
typedef analysis::ntuple::JetsTags JetsTags;
typedef analysis::ntuple::TriggerAccepts TriggerAccepts;
typedef analysis::ntuple::Vertices PrimaryVertices;


// Alias to the pointers to the above classes
typedef std::unique_ptr<EventInfo> pEventInfo;
typedef std::unique_ptr<Metadata> pMetadata;
typedef std::unique_ptr<Definitions> pDefinitions;
typedef std::unique_ptr<PileupInfo> pPileupInfo;
typedef std::unique_ptr<L1JetCandidates> pL1JetCandidates;
typedef std::unique_ptr<L1MuonCandidates> pL1MuonCandidates;
typedef std::unique_ptr<CaloJetCandidates> pCaloJetCandidates;
typedef std::unique_ptr<PFJetCandidates> pPFJetCandidates;
typedef std::unique_ptr<PatJetCandidates> pPatJetCandidates;
typedef std::unique_ptr<PatMETCandidates> pPatMETCandidates;
typedef std::unique_ptr<PatMuonCandidates> pPatMuonCandidates;
typedef std::unique_ptr<GenJetCandidates> pGenJetCandidates;
typedef std::unique_ptr<GenParticleCandidates> pGenParticleCandidates;
typedef std::unique_ptr<TriggerObjectCandidates> pTriggerObjectCandidates;
typedef std::unique_ptr<JetsTags> pJetsTags;
typedef std::unique_ptr<TriggerAccepts> pTriggerAccepts;
typedef std::unique_ptr<PrimaryVertices> pPrimaryVertices;

//
// class declaration
//

class Ntuplizer : public edm::EDAnalyzer {
   public:
      explicit Ntuplizer(const edm::ParameterSet&);
      ~Ntuplizer();

      //! Member function
      /*! To state exactly what you do use, even if it is no parameters. Required by EDAnalyzer?
      */
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
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
      bool do_patmets_;
      bool do_patmuons_;
      bool do_genjets_;
      bool do_genparticles_;
      bool do_jetstags_;
      bool do_pileupinfo_;
      bool do_geneventinfo_;
      bool do_triggeraccepts_;
      bool do_primaryvertices_;
      bool do_eventfilter_;
      bool do_genfilter_;
      bool do_triggerobjects_;
      bool do_genruninfo_;
      
      bool testmode_;
      
      std::vector< std::string > inputTagsVec_;
      std::vector< std::string > inputTags_;
      std::vector< std::string > btagAlgos_;
      std::vector< std::string > btagAlgosAlias_;
      std::vector< std::string > triggerObjectLabels_;
      std::vector< TitleAlias >  btagVars_;
      std::vector< std::string > jecRecords_;
      std::vector< std::string > jerRecords_;
      
      std::map<std::string, edm::EDGetTokenT<l1extra::L1JetParticleCollection> > l1JetTokens_;
      std::map<std::string, edm::EDGetTokenT<l1extra::L1MuonParticleCollection> > l1MuonTokens_;
      std::map<std::string, edm::EDGetTokenT<reco::CaloJetCollection> > caloJetTokens_;
      std::map<std::string, edm::EDGetTokenT<reco::PFJetCollection> > pfJetTokens_;
      std::map<std::string, edm::EDGetTokenT<pat::JetCollection> > patJetTokens_;
      std::map<std::string, edm::EDGetTokenT<pat::METCollection> > patMETTokens_;
      std::map<std::string, edm::EDGetTokenT<pat::MuonCollection> > patMuonTokens_;
      std::map<std::string, edm::EDGetTokenT<reco::GenJetCollection> > genJetTokens_;
      std::map<std::string, edm::EDGetTokenT<reco::GenParticleCollection> > genPartTokens_;
      std::map<std::string, edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> > triggerObjTokens_;
      std::map<std::string, edm::EDGetTokenT<edm::TriggerResults> > triggerResultsTokens_;
      std::map<std::string, edm::EDGetTokenT<reco::VertexCollection> > primaryVertexTokens_;

#ifndef CMSSWOLD
      std::shared_ptr<HLTPrescaleProvider> hltPrescaleProvider_;
#endif
           
      edm::InputTag genFilterInfo_;
      edm::InputTag totalEvents_;
      edm::InputTag filteredEvents_;
      edm::InputTag filteredMHatEvents_;
      edm::InputTag genRunInfo_;
      
      edm::InputTag pileupInfo_;
      edm::InputTag genEventInfo_;
      
      edm::InputTag fixedGridRhoAll_;
     
      edm::EDGetTokenT<GenFilterInfo> genFilterInfoToken_;      
      edm::EDGetTokenT<edm::MergeableCounter> totalEventsToken_;      
      edm::EDGetTokenT<edm::MergeableCounter> filteredEventsToken_;
      edm::EDGetTokenT<edm::MergeableCounter> filteredMHatEventsToken_;
      edm::EDGetTokenT<GenRunInfoProduct> genRunInfoToken_;
           
      edm::EDGetTokenT<std::vector<PileupSummaryInfo> > pileupInfoToken_;      
      edm::EDGetTokenT<GenEventInfoProduct> genEventInfoToken_;      
      
      edm::EDGetTokenT<double> fixedGridRhoAllToken_;

      
      InputTags eventCounters_;
      InputTags mHatEventCounters_;
      
      std::map<std::string, TTree*> tree_; // using pointers instead of smart pointers, could not Fill() with smart pointer???

      // Ntuple stuff
      pEventInfo eventinfo_;
      pMetadata  metadata_;
      pPileupInfo pileupinfo_;
      
      // Collections for the ntuples (vector)
      std::vector<pL1JetCandidates> l1jets_collections_;
      std::vector<pL1MuonCandidates> l1muons_collections_;
      std::vector<pCaloJetCandidates> calojets_collections_;
      std::vector<pPFJetCandidates> pfjets_collections_;
      std::vector<pPatJetCandidates> patjets_collections_;
      std::vector<pPatMETCandidates> patmets_collections_;
      std::vector<pPatMuonCandidates> patmuons_collections_;
      std::vector<pGenJetCandidates> genjets_collections_;
      std::vector<pGenParticleCandidates> genparticles_collections_;
      std::vector<pJetsTags> jetstags_collections_;
      std::vector<pPrimaryVertices> primaryvertices_collections_;
      std::vector<pTriggerAccepts> triggeraccepts_collections_;
      std::vector<pTriggerObjectCandidates> triggerobjects_collections_;
      
      
      
      // Collections for the ntuples (single)
      
      // metadata
      double xsection_;
      
      analysis::ntuple::FilterResults eventFilterResults_;
      analysis::ntuple::FilterResults genFilterResults_;
      
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
   testmode_      = false;
   inputTagsVec_ = config.getParameterNamesForType<InputTags>();
   inputTags_    = config.getParameterNamesForType<edm::InputTag>();
   
   config_  = config;
   for ( auto & inputTags : inputTagsVec_ )
   {
      InputTags collections = config_.getParameter<InputTags>(inputTags);
      for ( auto & collection : collections )
      {
         std::string label = collection.label();
         std::string inst  = collection.instance();
         std::string proc  = collection.process();
         std::string collection_name = label+"_"+inst+"_"+proc;
         if ( inputTags == "L1ExtraJets" ) l1JetTokens_[collection_name] = consumes<l1extra::L1JetParticleCollection>(collection);
         if ( inputTags == "L1ExtraMuons" ) l1MuonTokens_[collection_name] = consumes<l1extra::L1MuonParticleCollection>(collection);
         if ( inputTags == "CaloJets" ) caloJetTokens_[collection_name] = consumes<reco::CaloJetCollection>(collection);
         if ( inputTags == "PFJets" ) pfJetTokens_[collection_name] = consumes<reco::PFJetCollection>(collection);
         if ( inputTags == "PatJets" ) patJetTokens_[collection_name] = consumes<pat::JetCollection>(collection);
         if ( inputTags == "PatMETs" ) patMETTokens_[collection_name] = consumes<pat::METCollection>(collection);
         if ( inputTags == "PatMuons" ) patMuonTokens_[collection_name] = consumes<pat::MuonCollection>(collection);
         if ( inputTags == "GenJets" ) genJetTokens_[collection_name] = consumes<reco::GenJetCollection>(collection);
         if ( inputTags == "GenParticles" ) genPartTokens_[collection_name] = consumes<reco::GenParticleCollection>(collection);
         if ( inputTags == "TriggerObjectStandAlone"  ) triggerObjTokens_[collection_name] = consumes<pat::TriggerObjectStandAloneCollection>(collection);
         if ( inputTags == "PrimaryVertices"  ) primaryVertexTokens_[collection_name] = consumes<reco::VertexCollection>(collection);
         if ( inputTags == "TriggerResults"  ) triggerResultsTokens_[collection_name] = consumes<edm::TriggerResults>(collection);

         
     }
   }
   
#ifndef CMSSWOLD  
   hltPrescaleProvider_ = std::shared_ptr<HLTPrescaleProvider>(new HLTPrescaleProvider(config, consumesCollector(), *this));;
#endif
   
   // Single InputTag
   for ( auto & inputTag : inputTags_ )
   {
      edm::InputTag collection = config_.getParameter<edm::InputTag>(inputTag);
      
      // Lumi products
      if ( inputTag == "GenFilterInfo" )  { genFilterInfoToken_    = consumes<GenFilterInfo,edm::InLumi>(collection);         genFilterInfo_   = collection;}
      if ( inputTag == "TotalEvents" )    { totalEventsToken_      = consumes<edm::MergeableCounter,edm::InLumi>(collection); totalEvents_     = collection;}
      if ( inputTag == "FilteredEvents" ) { filteredEventsToken_   = consumes<edm::MergeableCounter,edm::InLumi>(collection); filteredEvents_  = collection;}
      if ( inputTag == "FilteredMHatEvents" ) { filteredMHatEventsToken_ = consumes<edm::MergeableCounter,edm::InLumi>(collection); filteredMHatEvents_  = collection;}
      if ( inputTag == "GenRunInfo" )     { genRunInfoToken_       = consumes<GenRunInfoProduct,edm::InRun>(collection);      genRunInfo_      = collection;}

      if ( inputTag == "PileupInfo" )     { pileupInfoToken_       = consumes<std::vector<PileupSummaryInfo> >(collection);   pileupInfo_      = collection;}
      if ( inputTag == "GenEventInfo" )   { genEventInfoToken_     = consumes<GenEventInfoProduct>(collection);               genEventInfo_    = collection;}
      if ( inputTag == "FixedGridRhoAll" ){ fixedGridRhoAllToken_  = consumes<double>(collection);                            fixedGridRhoAll_ = collection;}
 
   }

   
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
   
//    if ( do_pileupinfo_ )
//       pileupinfo_ -> Fill(event);

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
         collection -> Fill(event, iSetup);
   
      // Pat mets (pat)
      for ( auto & collection : patmets_collections_ )
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
         collection -> Fill(event, iSetup);
      
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
   do_pileupinfo_       = config_.exists("PileupInfo") && is_mc_;
   do_geneventinfo_     = config_.exists("GenEventInfo") && is_mc_;
   do_l1jets_           = config_.exists("L1ExtraJets");
   do_l1muons_          = config_.exists("L1ExtraMuons");
   do_calojets_         = config_.exists("CaloJets");
   do_pfjets_           = config_.exists("PFJets");
   do_patjets_          = config_.exists("PatJets");
   do_patmets_          = config_.exists("PatMETs");
   do_patmuons_         = config_.exists("PatMuons");
   do_genjets_          = config_.exists("GenJets");
   do_genparticles_     = config_.exists("GenParticles");
   do_jetstags_         = config_.exists("JetsTags");
//   do_triggeraccepts_   = config_.exists("TriggerResults") && config_.exists("TriggerPaths");
   do_triggeraccepts_   = config_.exists("TriggerResults");
   do_primaryvertices_  = config_.exists("PrimaryVertices");
//   do_eventfilter_      = config_.exists("EventFilter");
   do_eventfilter_      = config_.exists("TotalEvents")  && config_.exists("FilteredEvents");
   do_genfilter_        = config_.exists("GenFilterInfo");
   do_triggerobjects_   = config_.exists("TriggerObjectStandAlone") &&  config_.exists("TriggerObjectLabels");
   do_genruninfo_       = config_.exists("GenRunInfo") && is_mc_ ;
   
   if ( config_.exists("TestMode") ) // This is DANGEROUS! but can be useful. So BE CAREFUL!!!!
      testmode_ = config_.getParameter<bool> ("TestMode");
   
   if ( config_.exists("UseFullName") )
      use_full_name_ = config_.getParameter<bool> ("UseFullName");

   
   edm::Service<TFileService> fs;
   
   TFileDirectory eventsDir = fs -> mkdir("Events");
   
   std::string name;
   std::string fullname;
   
   genFilterResults_  = {};
   eventFilterResults_ = {};
   
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
   
   btagVars_.clear();
   for ( size_t it = 0 ; it < btagAlgos_.size() ;  ++it )
   {
      btagVars_.push_back({btagAlgos_[it],btagAlgosAlias_[it]});
//      btagVars_[btagAlgosAlias_[it]] = {btagAlgos_[it],(unsigned int)it};
   }
   
   // JEC Record (from TXT files)
   std::vector<std::string > jec_files;
   // JEC Record (from CondDB)
   jecRecords_.clear();
   if ( do_patjets_ && config_.exists("JECRecords") )
   {
      jecRecords_ = config_.getParameter< std::vector<std::string> >("JECRecords");
      if(config_.exists("JECUncertaintyFiles"))
      {
         jec_files = config_.getParameter< std::vector<std::string > >("JECUncertaintyFiles");
      }
   }
   // JER Record (from TXT files)
   std::vector<std::string > jer_files;
   std::vector<std::string > jersf_files;
   // JER Record (from CondDB)
   jerRecords_.clear();
   if ( do_patjets_ && config_.exists("JERRecords") )
   {
      jerRecords_ = config_.getParameter< std::vector<std::string> >("JERRecords");
      if(config_.exists("JERResFiles"))
      {
      	jer_files = config_.getParameter< std::vector<std::string > >("JERResFiles");
      }
      if(config_.exists("JERSfFiles"))
      {
      	jersf_files = config_.getParameter< std::vector<std::string > >("JERSfFiles");
      }
      
   }
   //
   size_t nPatJets = 0;
   if ( do_patjets_ )
      nPatJets = config_.getParameter<InputTags>("PatJets").size();
   
   if ( nPatJets > jecRecords_.size() && jecRecords_.size() != 0 )
   {
      std::cout << "*** ERROR ***  Ntuplizer: Number of JEC Records less than the number of PatJet collections." << std::endl;;
      exit(-1);
   }
   if ( nPatJets > jerRecords_.size() && jerRecords_.size() != 0 )
   {
      std::cout << "*** ERROR ***  Ntuplizer: Number of JER Records less than the number of PatJet collections." << std::endl;;
      exit(-1);
   }
   if ( jerRecords_.size() != 0 && jer_files.size() != 0 && jersf_files.size()!=0 &&(jerRecords_.size() != jer_files.size() || jerRecords_.size() != jersf_files.size()) )
   {
   		std::cerr << "*** ERROR *** Ntuplizer: Number of JER Records are not the same as number of provided input files. " <<std::endl;
   		exit(-1);
   }
   
   
   // Event info tree
   eventinfo_ = pEventInfo (new EventInfo(eventsDir));
   if ( do_pileupinfo_ )
      eventinfo_ -> PileupInfo(config_.getParameter<edm::InputTag>("PileupInfo"));
   if ( do_geneventinfo_ )
      eventinfo_ -> GenEventInfo(config_.getParameter<edm::InputTag>("GenEventInfo"));
   
    // Metadata 
   metadata_ = pMetadata (new Metadata(fs,is_mc_));
   metadata_ -> AddDefinitions(btagVars_,"btagging");
   // My cross section  value for the metadata
   xsection_ = -1.0;
   if ( config_.exists("CrossSection") )
      xsection_ = config_.getParameter<double>("CrossSection");
   
   
  // Input tags (vector)
   for ( auto & inputTags : inputTagsVec_ )
   {
      InputTags collections = config_.getParameter<InputTags>(inputTags);
      int patJetCounter = 0;
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
            tree_[name] = eventsDir.make<TTree>(name.c_str(),fullname.c_str());
         
         // L1 Jets
         if ( inputTags == "L1ExtraJets" )
         {
            l1jets_collections_.push_back( pL1JetCandidates( new L1JetCandidates(collection, tree_[name], is_mc_ ) ));
            l1jets_collections_.back() -> Init();
         }
         
         // L1 Muons
         if ( inputTags == "L1ExtraMuons" )
         {
            l1muons_collections_.push_back( pL1MuonCandidates( new L1MuonCandidates(collection, tree_[name], is_mc_ ) ));
            l1muons_collections_.back() -> Init();
         }
         
         // Calo Jets
         if ( inputTags == "CaloJets" )
         {
            calojets_collections_.push_back( pCaloJetCandidates( new CaloJetCandidates(collection, tree_[name], is_mc_ ) ));
            calojets_collections_.back() -> Init();
         }
         // PF Jets
         if ( inputTags == "PFJets" )
         {
            pfjets_collections_.push_back( pPFJetCandidates( new PFJetCandidates(collection, tree_[name], is_mc_ ) ));
            pfjets_collections_.back() -> Init();
         }
         // Pat Jets
         if ( inputTags == "PatJets" )
         {
            patjets_collections_.push_back( pPatJetCandidates( new PatJetCandidates(collection, tree_[name], is_mc_ ) ));
            patjets_collections_.back() -> Init(btagVars_);
            
            if ( patJetCounter == 0 && jecRecords_.size() > 0  )  std::cout << "*** Jet Energy Corrections Records - PatJets ***" << std::endl;
            if ( jecRecords_.size() > 0  )
            {
               if ( jec_files.size() > 0 && jec_files[patJetCounter] != "" )
                  patjets_collections_.back() -> AddJecInfo(jecRecords_[patJetCounter],jec_files[patJetCounter]);  // use txt file
               else
                  patjets_collections_.back() -> AddJecInfo(jecRecords_[patJetCounter]);                           // use confdb

            }
            
            if ( patJetCounter == 0 && jerRecords_.size() > 0  ) std::cout << "*** Jet Energy Resolutions Records - PatJets ***" << std::endl;
            if ( jerRecords_.size() > 0 && is_mc_  )
            {
               if ( jer_files.size() > 0 && jer_files[patJetCounter] != "" )
                  patjets_collections_.back() -> AddJerInfo(jerRecords_[patJetCounter],jer_files[patJetCounter], jersf_files[patJetCounter],fixedGridRhoAll_);  // use txt file
               else
                  patjets_collections_.back() -> AddJerInfo(jerRecords_[patJetCounter],fixedGridRhoAll_);  // use txt file

            }
            
//             if ( jecRecords_.size() > 0 && jerRecords_.size() > 0 )
//             {
//                if (jer_files.size() != 0 && jer_files[patJetCounter] != "" && jersf_files[patJetCounter] != "")
//                {
//             		patjets_collections_.back() -> Init(btagVars_,jecRecords_[patJetCounter],jerRecords_[patJetCounter],jer_files[patJetCounter],jersf_files[patJetCounter],fixedGridRhoAll_);
//             	}
//             	else
//                {
//                   patjets_collections_.back() -> Init(btagVars_,jecRecords_[patJetCounter],jerRecords_[patJetCounter],fixedGridRhoAll_);
//                }
//                if ( jecRecords_[patJetCounter] != "" )  std::cout << name << " => "  << jecRecords_[patJetCounter] << std::endl;
//             }
//             else
//             {
//                patjets_collections_.back() -> Init(btagVars_);
//             }
            ++patJetCounter;
         }
         // Pat METs
         if ( inputTags == "PatMETs" )
         {
            patmets_collections_.push_back( pPatMETCandidates( new PatMETCandidates(collection, tree_[name], is_mc_) ));
            patmets_collections_.back() -> Init();
         }
         // Pat Muons
         if ( inputTags == "PatMuons" )
         {
            patmuons_collections_.push_back( pPatMuonCandidates( new PatMuonCandidates(collection, tree_[name], is_mc_ ) ));
            patmuons_collections_.back() -> Init();
         }
         // Gen Jets
         if ( inputTags == "GenJets" )
         {
            genjets_collections_.push_back( pGenJetCandidates( new GenJetCandidates(collection, tree_[name], is_mc_, 10., 6. ) ));
            genjets_collections_.back() -> Init();
         }
         // Gen Particles
         if ( inputTags == "GenParticles" )
         {
            genparticles_collections_.push_back( pGenParticleCandidates( new GenParticleCandidates(collection, tree_[name], is_mc_, 0.1, 6. ) ));
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
            TFileDirectory triggerObjectsDir = eventsDir.mkdir(dir);
      
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
            std::vector< std::string> trigger_paths;
            trigger_paths.clear();
            if ( config_.exists("TriggerPaths") ) trigger_paths = config_.getParameter< std::vector< std::string> >("TriggerPaths");
#ifndef CMSSWOLD           
            triggeraccepts_collections_.push_back( pTriggerAccepts( new TriggerAccepts(collection, tree_[name], trigger_paths, hltPrescaleProvider_, testmode_) ));
#else
            triggeraccepts_collections_.push_back( pTriggerAccepts( new TriggerAccepts(collection, tree_[name], trigger_paths, testmode_) ));  
#endif                      
//            triggeraccepts_collections_.back() -> Branches();
         }
         // Primary Vertices
         if ( inputTags == "PrimaryVertices" )
         {
            primaryvertices_collections_.push_back( pPrimaryVertices( new PrimaryVertices(collection, tree_[name]) ));
         }
         
      }
   }
      
   
   // InputTag (single, i.e. not vector)
   
   int nCounters = 0;
   int nMHatCounters = 0;
   for ( auto & inputTag : inputTags_ )
   {
      edm::InputTag collection = config_.getParameter<edm::InputTag>(inputTag);
      
         // Names for the trees, from inputs
         std::string label = collection.label();
         std::string inst  = collection.instance();
         std::string proc  = collection.process();
         name = label;
         fullname = name + "_" + inst + "_" + proc;
         if ( use_full_name_ ) name = fullname;
         
         
      // Generator filter
      if ( do_genfilter_ && inputTag == "GenFilterInfo" && is_mc_ )
      {
         metadata_ -> SetGeneratorFilter(config_.getParameter<edm::InputTag> ("GenFilterInfo"));
      }
      // Event filter
      if ( do_eventfilter_ )
      {
         eventCounters_.resize(2);
         mHatEventCounters_.resize(2);
         if ( inputTag == "TotalEvents" )     { eventCounters_[0] = totalEvents_; mHatEventCounters_[0] = totalEvents_; ++nCounters; ++nMHatCounters; }
         if ( inputTag == "FilteredEvents" )  { eventCounters_[1] = filteredEvents_; ++nCounters; }
         if ( inputTag == "FilteredMHatEvents" )  { mHatEventCounters_[1] = filteredMHatEvents_; ++nMHatCounters; }

         if ( nCounters == 2 ) 		metadata_ -> SetEventFilter(eventCounters_);
         if ( nMHatCounters == 2)	metadata_ -> SetMHatEventFilter(mHatEventCounters_);
//         std::cout<<nMHatCounters<<std::endl;
      }
      // Pileup Info
//       if ( inputTag == "PileupInfo" && is_mc_ )
//       {
//          tree_[name] = eventsDir.make<TTree>(name.c_str(),fullname.c_str());
//          pileupinfo_ = pPileupInfo( new PileupInfo(collection, tree_[name]) );
//          pileupinfo_ -> Branches();
// 
//       }
         


   } 

}

// ------------ method called once each job just after ending the event loop  ------------
void 
Ntuplizer::endJob() 
{
   metadata_ -> Fill();
}

// ------------ method called when starting to processes a run  ------------
void Ntuplizer::beginRun(edm::Run const& run, edm::EventSetup const& setup)
{
   // Initialize HLTConfig every lumi block
   if ( do_triggeraccepts_ )
   {
      for ( size_t i = 0; i < triggeraccepts_collections_.size() ; ++i )
      {
         triggeraccepts_collections_[i]  -> Run(run,setup);
      }
   }
}


// ------------ method called when ending the processing of a run  ------------

void 
Ntuplizer::endRun(edm::Run const& run, edm::EventSetup const& setup)
{
   if ( do_genruninfo_ )
   {
      metadata_ -> SetCrossSections(run,genRunInfo_,xsection_);
   }
}

// ------------ method called when starting to processes a luminosity block  ------------

void  Ntuplizer::beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
//    // Initialize HLTConfig every lumi block
//    if ( do_triggeraccepts_ )
//    {
//       for ( size_t i = 0; i < triggeraccepts_collections_.size() ; ++i )
//       {
//          triggeraccepts_collections_[i]  -> LumiBlock(lumi,setup);
//       }
//    }
}


// ------------ method called when ending the processing of a luminosity block  ------------

void 
Ntuplizer::endLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup)
{
   metadata_ -> IncrementEventFilters(lumi);
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
