/**\class JetsTags JetsTags.cc Analysis/Ntuplizer/src/JetsTags.cc

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
#include "DataFormats/Provenance/interface/Provenance.h"

#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "Analysis/Ntuplizer/interface/JetsTags.h"

#include "TTree.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::ntuple;

//
// constructors and destructor
//
JetsTags::JetsTags()
{
   // default constructor
}

JetsTags::JetsTags(const edm::InputTag& btag, TTree* tree, float minPt, float maxEta) :
      minPt_(minPt), maxEta_(maxEta)
{
   input_collection_ = btag;
   tree_ = tree;
   
}

JetsTags::~JetsTags()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void JetsTags::ReadFromEvent(const edm::Event& event)
{
   using namespace edm;
   
//    typedef std::vector<Provenance const*> Provenances;
//    Provenances provenances;
//    event.getAllProvenance(provenances);
//    std::string modLabel;
//    std::string instanceName;
//    std::string processName;
//    for(Provenances::iterator itProv = provenances.begin(), itProvEnd = provenances.end();
//                              itProv != itProvEnd;
//                            ++itProv) {
//       std::cout << (*itProv)->moduleLabel() << ", " << (*itProv)->productInstanceName() << ", " << (*itProv)->processName() << std::endl;
//       std::cout << (*itProv)->className() << std::endl;
//       if ( (*itProv)->moduleLabel() == "hltL3CombinedSecondaryVertexBJetTags" )
//       {
//          modLabel = (*itProv)->moduleLabel();
//          instanceName = (*itProv)->productInstanceName();
//          processName  = (*itProv)->processName();
//       }
//    }
   
   // Particles
//   edm::Handle<std::vector<reco::JetTag> > handler;
   edm::Handle<reco::JetTagCollection> handler;
   event.getByLabel(this->input_collection_, handler);
//   event.getByLabel(InputTag(modLabel,instanceName,processName), handler);
   candidates_ = *(handler.product());
}

void JetsTags::Tags()
{
   using namespace edm;
   
   int n = 0;
   for ( size_t i = 0 ; i < candidates_.size(); ++i )
   {
      if ( n >= maxCandidates ) break;
      
      RefToBase<reco::Jet> jet = candidates_[i].first;
      float btag = candidates_[i].second;
      
      if ( jet -> pt() < minPt_ || fabs (jet -> eta()) > maxEta_ ) continue;
      
      this->pt_[n] = jet -> pt();
      this->eta_[n]= jet -> eta();
      this->phi_[n]= jet -> phi();
      this->e_[n]  = jet -> energy();
      this->btag_[n]= btag;
      ++n;
      
   }
   this->n_ = n;

}

void JetsTags::MinPt(const float& minPt)
{
   minPt_ = minPt;
}

void JetsTags::MaxEta(const float& maxEta)
{
   maxEta_ = maxEta;
}


void JetsTags::Fill()
{
   tree_->Fill();
}

void JetsTags::Fill(const edm::Event& event)
{
   this->ReadFromEvent(event);
   this->Tags();
   this->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void JetsTags::Branches()
{
   
   tree_->Branch("n",  &this->n_, "n/I");
   tree_->Branch("pt", this->pt_, "pt[n]/F");
   tree_->Branch("eta",this->eta_,"eta[n]/F");
   tree_->Branch("phi",this->phi_,"phi[n]/F");
   tree_->Branch("e",  this->e_,  "e[n]/F");
   tree_->Branch("btag",this->btag_,"btag[n]/F");
   
}
