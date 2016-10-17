// -*- C++ -*-
//
// Package:    Analysis/kinematicJetFilter
// Class:      kinematicJetFilter
// 
/**\class kinematicJetFilter kinematicJetFilter.cc Analysis/kinematicJetFilter/plugins/kinematicJetFilter.cc

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

class kinematicJetFilter : public edm::stream::EDFilter<> {
   public:
      explicit kinematicJetFilter(const edm::ParameterSet&);
      ~kinematicJetFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;

      // ----------member data ---------------------------
      const edm::InputTag src_;
      const edm::EDGetTokenT<pat::JetCollection> jetToken_;
      const std::vector<double> pt_;
      const std::vector<double> eta_;
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
kinematicJetFilter::kinematicJetFilter(const edm::ParameterSet& iConfig) :
	src_(iConfig.getParameter<edm::InputTag>("src")),
	jetToken_(consumes<pat::JetCollection>(src_)),
	pt_(iConfig.getParameter<std::vector<double> >("pt")),
	eta_(iConfig.getParameter<std::vector<double> >("eta"))
{
   //now do what ever initialization is needed

}


kinematicJetFilter::~kinematicJetFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
kinematicJetFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<pat::JetCollection> jetsHandle;
   iEvent.getByToken(jetToken_, jetsHandle);
   if(pt_.size() == 0 && eta_.size() == 0){
	   std::cerr<<"Error: No Pt/Eta/Btag values were specified. Interrupt."<<std::endl;
	   exit(1);
   }

   for(size_t i = 0; i< pt_.size();++i)	{
	   if(jetsHandle->at(i).pt() < pt_.at(i)) return false;
   }
   for(size_t i = 0; i< eta_.size();++i)	{
	   if(std::abs(jetsHandle->at(i).eta()) > eta_.at(i)) return false;
   }

   return true;
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
kinematicJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setComment("Filters events.");
  desc.add<edm::InputTag>("src");
  desc.add<std::vector<double> >("pt");
  desc.add<std::vector<double> >("eta");
}
//define this as a plug-in
DEFINE_FWK_MODULE(kinematicJetFilter);
