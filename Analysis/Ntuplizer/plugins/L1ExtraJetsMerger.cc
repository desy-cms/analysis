// -*- C++ -*-
//
// Package:    Analysis/L1ExtraJetsMerger
// Class:      L1ExtraJetsMerger
// 
/**\class L1ExtraJetsMerger L1ExtraJetsMerger.cc Analysis/L1ExtraJetsMerger/plugins/L1ExtraJetsMerger.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh
//         Created:  Mon, 03 Nov 2014 15:08:57 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

#include "CommonTools/Utils/interface/PtComparator.h"

//
// class declaration
//

class L1ExtraJetsMerger : public edm::EDProducer {
   public:
      explicit L1ExtraJetsMerger(const edm::ParameterSet&);
      ~L1ExtraJetsMerger();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      
      std::vector<edm::InputTag> input_collections_;
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
L1ExtraJetsMerger::L1ExtraJetsMerger(const edm::ParameterSet& config)
{
   input_collections_ = config.getParameter< std::vector<edm::InputTag> >("L1ExtraJets");
   
   produces<l1extra::L1JetParticleCollection>("Merged");
}


L1ExtraJetsMerger::~L1ExtraJetsMerger()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1ExtraJetsMerger::produce(edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;
   
   // L1 Jets
   std::auto_ptr<l1extra::L1JetParticleCollection> l1jets( new l1extra::L1JetParticleCollection );
   
   int counter = 0;
   int sum = 0;
   for ( size_t j = 0 ; j < input_collections_.size(); ++j )  // loop over all input l1extrajets collections
   {
      edm::Handle<l1extra::L1JetParticleCollection> handler;
      event.getByLabel(input_collections_.at(j), handler);
      const l1extra::L1JetParticleCollection & jets = *(handler.product());
      sum += jets.size();
      for ( l1extra::L1JetParticleCollection::const_iterator jet = jets.begin(); jet != jets.end(); ++jet )
      {
         l1jets -> push_back(*jet);
         ++counter;
      }
   }
  
//    // This is stupid but if it solves the problem...
//    edm::Handle<l1extra::L1JetParticleCollection> handler0;
//    event.getByLabel(input_collections_.at(0), handler0);
//    const l1extra::L1JetParticleCollection & jets0 = *(handler0.product());
//    
//    edm::Handle<l1extra::L1JetParticleCollection> handler1;
//    event.getByLabel(input_collections_.at(1), handler1);
//    const l1extra::L1JetParticleCollection & jets1 = *(handler1.product());
//    
//    edm::Handle<l1extra::L1JetParticleCollection> handler2;
//    event.getByLabel(input_collections_.at(2), handler2);
//    const l1extra::L1JetParticleCollection & jets2 = *(handler2.product());
//    
//    // reserve some space for the new data, to control the size
//    const int size = jets0.size() + jets1.size() + jets2.size();
//    
//    l1jets->reserve( size );
//    
//    for ( l1extra::L1JetParticleCollection::const_iterator jet = jets0.begin(); jet != jets0.end(); ++jet )
//       l1jets -> push_back(*jet);
//    for ( l1extra::L1JetParticleCollection::const_iterator jet = jets1.begin(); jet != jets1.end(); ++jet )
//       l1jets -> push_back(*jet);
//    for ( l1extra::L1JetParticleCollection::const_iterator jet = jets2.begin(); jet != jets2.end(); ++jet )
//       l1jets -> push_back(*jet);
  
   
   NumericSafeGreaterByPt<l1extra::L1JetParticle> compJetsPt;
   std::sort (l1jets -> begin(), l1jets -> end(), compJetsPt);

   event.put(l1jets,"Merged");
   
}

// ------------ method called once each job just before starting event loop  ------------
void 
L1ExtraJetsMerger::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1ExtraJetsMerger::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
L1ExtraJetsMerger::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
L1ExtraJetsMerger::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
L1ExtraJetsMerger::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
L1ExtraJetsMerger::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
L1ExtraJetsMerger::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(L1ExtraJetsMerger);
