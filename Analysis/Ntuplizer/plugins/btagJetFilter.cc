// -*- C++ -*-
//
// Package:    Analysis/btagJetFilter
// Class:      btagJetFilter
// 
/**\class btagJetFilter btagJetFilter.cc Analysis/btagJetFilter/plugins/btagJetFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Rostyslav Shevchenko
//         Created:  Thu, 21 Apr 2016 12:40:20 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include <vector>

//
// class declaration
//

class btagJetFilter : public edm::stream::EDFilter<> {
   public:
      explicit btagJetFilter(const edm::ParameterSet&);
      ~btagJetFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;

      // ----------member data ---------------------------
      const edm::InputTag src_;
      const edm::EDGetTokenT<pat::JetCollection> jetToken_;
      const std::vector<double> btag_;
      const std::string algo_;
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
btagJetFilter::btagJetFilter(const edm::ParameterSet& iConfig) :
	src_(iConfig.getParameter<edm::InputTag>("src")),
	jetToken_(consumes<pat::JetCollection>(src_)),
	btag_(iConfig.getParameter<std::vector<double> >("btag")),
	algo_(iConfig.getParameter<std::string >("algo"))
{
   //now do what ever initialization is needed

}


btagJetFilter::~btagJetFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
btagJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<pat::JetCollection> jetsHandle;
   iEvent.getByToken(jetToken_, jetsHandle);
   if(btag_.size() == 0 || algo_ == ""){
	   std::cerr<<"Error: No Btag/Algo values were specified. Interrupt."<<std::endl;
	   exit(1);
   }

   for(size_t i = 0; i< btag_.size();++i)	{
	   if(jetsHandle->at(i).bDiscriminator(algo_) < btag_.at(i)) return false;
   }

   return true;
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
btagJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setComment("Filter events according to bDescriminator.");
  desc.add<edm::InputTag>("src");
  desc.add<std::vector<double> >("btag");
}
//define this as a plug-in
DEFINE_FWK_MODULE(btagJetFilter);
