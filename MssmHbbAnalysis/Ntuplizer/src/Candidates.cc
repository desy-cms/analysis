/**\class Candidates Candidates.cc MssmHbbAnalysis/Ntuplizer/src/Candidates.cc

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

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

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

#include "CommonTools/Utils/interface/PtComparator.h"

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "MssmHbbAnalysis/Ntuplizer/interface/Candidates.h"

#include "TTree.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

// member functions specialization - needed to be declared in the same namespace as the class
namespace mssmhbb {
   namespace ntuple {
      template <> void Candidates<pat::TriggerObject>::ReadFromEvent(const edm::Event& event);
 }
}   
//
// constructors and destructor
//
template <typename T>
Candidates<T>::Candidates()
{
   // default constructor
}

template <typename T>
Candidates<T>::Candidates(const edm::InputTag& tag, TTree* tree, const bool & mc, float minPt, float maxEta ) :
      minPt_(minPt), maxEta_(maxEta)
{
   this->input_collection_ = tag;
   this->tree_ = tree;
   
   is_l1jet_        = std::is_same<T,l1extra::L1JetParticle>::value;
   is_l1muon_       = std::is_same<T,l1extra::L1MuonParticle>::value;
   is_calojet_      = std::is_same<T,reco::CaloJet>::value;
   is_pfjet_        = std::is_same<T,reco::PFJet>::value;
   is_patjet_       = std::is_same<T,pat::Jet>::value;
   is_patmuon_      = std::is_same<T,pat::Muon>::value;
   is_genjet_       = std::is_same<T,reco::GenJet>::value;
   is_genparticle_  = std::is_same<T,reco::GenParticle>::value;
   is_trigobject_   = std::is_same<T,pat::TriggerObject>::value;
   is_mc_           = mc;
   
//   do_kinematics_ = ( is_l1jet_ || is_l1muon_ || is_calojet_ || is_pfjet_ || is_patjet_ || is_patmuon_ || is_genjet_ || is_genparticle_ );
   do_kinematics_ = true;
   do_generator_  = ( is_mc_ && is_genparticle_ );
   
   higgs_pdg_ = 36;
   
}

template <typename T>
Candidates<T>::~Candidates()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
template <typename T>
void Candidates<T>::ReadFromEvent(const edm::Event& event)
{
   using namespace edm;

   // Candidates
   candidates_.clear();
   edm::Handle<std::vector<T> > handler;
   event.getByLabel(this->input_collection_, handler);
   candidates_ = *(handler.product());
}

// Specialization for trigger objects
template <>
void Candidates<pat::TriggerObject>::ReadFromEvent(const edm::Event& event)
{
   using namespace edm;
   
   candidates_.clear();
   // The stand alone collection
   edm::Handle<pat::TriggerObjectStandAloneCollection> handler;
   event.getByLabel(this->input_collection_, handler);
   
   const std::string label = tree_ -> GetName(); // using the label to name the tree
   for ( auto ito : *handler )
   {
//      const std::vector< std::string > & filterLabels = ito.filterLabels();
//      for ( auto & l : filterLabels )
//         std::cout << l << std::endl;
      if ( ito.filter(label) )
         candidates_.push_back(ito.triggerObject());
   }
   
   // Sort the objects by pt
   NumericSafeGreaterByPt<pat::TriggerObject> triggerObjectGreaterByPt;
   std::sort (candidates_.begin(), candidates_.end(),triggerObjectGreaterByPt);
}

template <typename T>
void Candidates<T>::Kinematics()
{
   using namespace edm;

   int n = 0;
   for ( size_t i = 0 ; i < candidates_.size(); ++i )
   {
      if ( n >= maxCandidates ) break;
      
      if ( do_generator_ )
      {
         int pdg    = candidates_[i].pdgId();
         int status = candidates_[i].status();
         
         this->pdg_[n]   = pdg;
         this->status_[n]= status;
         const reco::Candidate * mother = candidates_[i].mother(0);
         this->higgs_dau_[n] = false;
         if ( mother != NULL )  // initial protons are orphans
         {
            if ( mother->pdgId() == higgs_pdg_ )
               this->higgs_dau_[n] = true;
         }
         
         if ( !(abs(pdg) <= 6 || (abs(pdg) >= 11 && abs(pdg) <= 16) || pdg == 36 || this->higgs_dau_[n] ) ) continue; 
         
      }
      
//      if ( !do_generator_ )
//         if ( candidates_[i].pt() < minPt_ || fabs (candidates_[i].eta()) > maxEta_ ) continue;
      
      if ( minPt_  >= 0. && candidates_[i].pt()  < minPt_  ) continue;
      if ( maxEta_ >= 0. && candidates_[i].eta() > maxEta_ ) continue;
      
      this->pt_[n]  = candidates_[i].pt();
      this->eta_[n] = candidates_[i].eta();
      this->phi_[n] = candidates_[i].phi();
      this->px_[n]  = candidates_[i].px();
      this->py_[n]  = candidates_[i].py();
      this->pz_[n]  = candidates_[i].pz();
      this->e_[n]   = candidates_[i].energy();
      this->et_[n]  = candidates_[i].et();
      this->q_[n]   = candidates_[i].charge();
      
      if ( is_patjet_ )
      {
         pat::Jet * jet = dynamic_cast<pat::Jet*> (&candidates_[i]);
         for ( size_t it = 0 ; it < btagAlgos_.size() ;  ++it )
         {
            if ( it >= 15 ) break;
            this->btag_[it][n]  = jet->bDiscriminator(btagAlgos_[it]);
         }
         if ( is_mc_ )
         {
            this->flavour_[n]  = jet->partonFlavour();
         }
      }
      
      ++n;
      
   }
   this->n_ = n;

}

template <typename T>
void Candidates<T>::MinPt(const float& minPt)
{
   minPt_ = minPt;
}

template <typename T>
void Candidates<T>::MaxEta(const float& maxEta)
{
   maxEta_ = maxEta;
}


template <typename T>
void Candidates<T>::Fill()
{
   tree_->Fill();
}

template <typename T>
void Candidates<T>::Fill(const edm::Event& event)
{
   this->ReadFromEvent(event);
   if ( do_kinematics_ ) this->Kinematics();
   this->Fill();
}

// ------------ method called once each job just before starting event loop  ------------

template <typename T>
void Candidates<T>::Branches()
{
   // kinematics output info
   if ( do_kinematics_ )
   {
      tree_->Branch("n",  &this->n_, "n/I");
      tree_->Branch("pt", this->pt_, "pt[n]/F");
      tree_->Branch("eta",this->eta_,"eta[n]/F");
      tree_->Branch("phi",this->phi_,"phi[n]/F");
      tree_->Branch("px", this->px_, "px[n]/F");
      tree_->Branch("py", this->py_, "py[n]/F");
      tree_->Branch("pz", this->pz_, "pz[n]/F");
      tree_->Branch("e",  this->e_,  "e[n]/F");
      tree_->Branch("et", this->et_, "et[n]/F");
      tree_->Branch("q",  this->q_,  "q[n]/I");
      if ( do_generator_ )
      {
         tree_->Branch("pdg",   this->pdg_,   "pdg[n]/I");
         tree_->Branch("status",this->status_,"status[n]/I");
         tree_->Branch("higgs_dau",this->higgs_dau_,"higgs_dau[n]/O");

      }
      if ( is_patjet_ )
      {
//         for ( auto& it : btagAlgosAlias_ )
         for ( size_t it = 0 ; it < btagAlgos_.size() ;  ++it )
         {
            if ( it >= 15 ) break;
            std::string alias = btagAlgosAlias_[it];
            std::string aliasv = alias + "[n]/F";
            tree_->Branch(alias.c_str(),   this->btag_[it], aliasv.c_str());
         }
         if ( is_mc_ )
         {
            tree_->Branch("flavour",   this->flavour_,   "flavour[n]/I");
         }
      }
   }
   
}

template <typename T>
void Candidates<T>::Init( const std::vector<std::string> & btagAlgos, const std::vector<std::string> & btagAlgosAlias )
{
   btagAlgos_ = btagAlgos;
   btagAlgosAlias_ = btagAlgosAlias;
   Branches();
   
}

// Need to declare all possible template classes here
template class Candidates<l1extra::L1JetParticle>;
template class Candidates<l1extra::L1MuonParticle>;
template class Candidates<reco::CaloJet>;
template class Candidates<reco::PFJet>;
template class Candidates<pat::Jet>;
template class Candidates<pat::Muon>;
template class Candidates<reco::GenJet>;
template class Candidates<reco::GenParticle>;
template class Candidates<pat::TriggerObject>;
