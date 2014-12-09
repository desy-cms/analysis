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
 
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "MssmHbbAnalysis/Ntuplizer/interface/Candidates.h"

#include "TTree.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

//
// constructors and destructor
//
template <typename T>
Candidates<T>::Candidates()
{
   // default constructor
}

template <typename T>
Candidates<T>::Candidates(const edm::InputTag& tag, TTree* tree)
{
   this->input_collection_ = tag;
   this->tree_ = tree;
   
   is_l1jet_   = std::is_same<T,l1extra::L1JetParticle>::value;
   is_calojet_ = std::is_same<T,reco::CaloJet>::value;
   do_kinematics_ = ( is_l1jet_ || is_calojet_ );
   
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
void Candidates<T>::candidates(const edm::Event& event)
{
   using namespace edm;
   
   // Particles
   edm::Handle<std::vector<T> > handler;
   event.getByLabel(this->input_collection_, handler);
   candidates_ = *(handler.product());
}

template <typename T>
void Candidates<T>::kinematics()
{
   using namespace edm;
   
   int n = 0;
   for ( size_t i = 0 ; i < candidates_.size(); ++i )
   {
      if ( n < maxCandidates )
      {
         this->pt_[n] = candidates_[i].pt();
         this->eta_[n]= candidates_[i].eta();
         this->phi_[n]= candidates_[i].phi();
         this->px_[n] = candidates_[i].px();
         this->py_[n] = candidates_[i].py();
         this->pz_[n] = candidates_[i].pz();
         this->e_[n]  = candidates_[i].energy();
         ++n;
      }
      this->n_ = n;

   }
}


template <typename T>
void Candidates<T>::fill()
{
   tree_->Fill();
}

template <typename T>
void Candidates<T>::fill(const edm::Event& event)
{
   this->candidates(event);
   if ( do_kinematics_ ) this->kinematics();
   this->fill();
}

// ------------ method called once each job just before starting event loop  ------------
template <typename T>
void Candidates<T>::branches()
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
   }
}

// Need to declare all possible template classes here
template class Candidates<l1extra::L1JetParticle>;
template class Candidates<reco::CaloJet>;
