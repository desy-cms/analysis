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
#include <iostream>
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
//
// class declaration
//
int returnMassPoint(const std::string & inputFilelist_);

class mHatGenFilter : public edm::stream::EDFilter<> {
   public:
      explicit mHatGenFilter(const edm::ParameterSet&);
      ~mHatGenFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;

      // ----------member data ---------------------------
      const edm::InputTag src_;
      const edm::EDGetTokenT<GenEventInfoProduct> genToken_;
      const double mHat_;
      const std::string mCentral_;
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
mHatGenFilter::mHatGenFilter(const edm::ParameterSet& iConfig) :
	src_(iConfig.getParameter<edm::InputTag>("src")),
	genToken_(consumes<GenEventInfoProduct>(src_)),
	mHat_(iConfig.getParameter<double>("mHat")),
	mCentral_(iConfig.getParameter<std::string>("mCentral"))
{
   //now do what ever initialization is needed

}


mHatGenFilter::~mHatGenFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
mHatGenFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<GenEventInfoProduct> GenEventInfoHandle;
   iEvent.getByToken(genToken_, GenEventInfoHandle);
   if (GenEventInfoHandle.isValid()){
	   double p_prot = 13000. /2.;
   	   double p1 = GenEventInfoHandle->pdf()->x.first * p_prot;
   	   double p2 = GenEventInfoHandle->pdf()->x.second * p_prot;
   	   double mHat = std::sqrt((p1+p2)*(p1+p2) - (p1-p2)*(p1-p2));
   	   if(mHat < mHat_ * returnMassPoint(mCentral_)) return false;
   	   return true;
   }
   else {
	   edm::LogWarning("gg Ntuplizer") << "no GenEventInfoProduct in event";
	   return false;
   }
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
mHatGenFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setComment("Filter events according to mHat.");
  desc.add<edm::InputTag>("src");
  desc.add<double>("mHat");
  desc.add<std::string>("mCentral");
}

int returnMassPoint(const std::string & inputFilelist_) {
	int Mpoint = 0;

	std::string MassPos = "_M-";
	auto p1 = inputFilelist_.find(MassPos) + 3;
	if(p1 == std::string::npos) {
		std::cerr<<"FileNames were cahnged!!!!"<<std::endl;
		exit(1);
	}
	auto p2 = inputFilelist_.find("_",p1);
	std::string MpointString = inputFilelist_.substr(p1,size_t(p2-p1));
	Mpoint = std::stoi(MpointString);
	return Mpoint;
}
//define this as a plug-in
DEFINE_FWK_MODULE(mHatGenFilter);
