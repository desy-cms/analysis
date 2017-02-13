// -*- C++ -*-
//
// Package:    Analysis/Diagnosis
// Class:      ListEventContent
// 
/**\class ListEventContent ListEventContent.cc Analysis/Diagnosis/plugins/ListEventContent.cc

 Description: [one line class summary]

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

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class ListEventContent : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ListEventContent(const edm::ParameterSet&);
      ~ListEventContent();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      
   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::InputTag patJets_;
      edm::EDGetTokenT<pat::JetCollection> jetsToken_;
      
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
ListEventContent::ListEventContent(const edm::ParameterSet& config):
patJets_(config.getParameter<edm::InputTag> ("PatJets")),
jetsToken_(consumes<pat::JetCollection>(patJets_))
{
   //now do what ever initialization is needed

}


ListEventContent::~ListEventContent()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ListEventContent::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;
   
   EventAuxiliary evt = event.eventAuxiliary();
  

   edm::Handle<pat::JetCollection> jetsHandler;
   event.getByLabel(patJets_, jetsHandler);
   const pat::JetCollection & jets = *(jetsHandler.product());
   
   std::cout << "Run = " << evt.run() << "    Lumi section = " << evt.luminosityBlock() << "     Event = " << evt.event() << std::endl;
   
   for ( size_t j = 0; j < jets.size() ; ++j )
   {
      auto jet = jets.at(j);
      double nHadFrac = jet.neutralHadronEnergyFraction();
      double nEmFrac  = jet.neutralEmEnergyFraction();
      int    nMult    = jet.neutralMultiplicity();
      double cHadFrac = jet.chargedHadronEnergyFraction();
      double cEmFrac  = jet.chargedEmEnergyFraction();
      int    cMult    = jet.chargedMultiplicity();
//      double muFrac   = jet.muonEnergyFraction();
      int    nConst   = nMult + cMult;
      
      bool looseId;
//      bool tightId;
      if ( fabs(jet.eta()) <= 2.7 )
      {
         looseId = (nHadFrac<0.99 && nEmFrac<0.99 && nConst>1) && ((fabs(jet.eta())<=2.4 && cHadFrac>0 && cMult>0 && cEmFrac<0.99) || fabs(jet.eta())>2.4) && fabs(jet.eta())<=2.7;
//         tightId = (nHadFrac<0.90 && nEmFrac<0.90 && nConst>1) && ((fabs(jet.eta())<=2.4 && cHadFrac>0 && cEmFrac<0.99) || fabs(jet.eta())>2.4) && fabs(jet.eta())<=2.7;
      }
      else if ( fabs(jet.eta()) > 2.7 && fabs(jet.eta()) <= 3.0 )
      {
         looseId = ( nEmFrac<0.90 && nMult>2 );
//         tightId = ( nEmFrac<0.90 && nMult>2 );
      }
      else
      {
         looseId = ( nEmFrac<0.90 && nMult>10 );
//         tightId = ( nEmFrac<0.90 && nMult>10 );
      }
      
      printf ("jet# %4d : pt = %7.2f   eta = %+5.3f   phi = %+5.3f   jetid = %1u  \n", int(j), jet.pt(), jet.eta(), jet.phi(), looseId ); 
      printf ("            neutral had frac = %5.3f   neutral em frac = %5.3f   neutral# = %3d  charged had frac = %5.3f   charged em frac = %5.3f  charged# = %3d   constituents# = %3d \n", nHadFrac, nEmFrac, nMult, cHadFrac, cEmFrac, cMult, nConst ); 
      
      
      
      
   }     // jets loop                              
  
   std::cout << "---------------" << std::endl;
}


// ------------ method called once each job just before starting event loop  ------------
void 
ListEventContent::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void 
ListEventContent::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ListEventContent::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//

//define this as a plug-in
DEFINE_FWK_MODULE(ListEventContent);
