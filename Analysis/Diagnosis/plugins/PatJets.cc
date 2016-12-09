// -*- C++ -*-
//
// Package:    Analysis/Diagnosis
// Class:      PatJets
// 
/**\class PatJets PatJets.cc Analysis/Diagnosis/plugins/PatJets.cc

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

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class PatJets : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit PatJets(const edm::ParameterSet&);
      ~PatJets();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      
      void createHistograms();


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::InputTag patJets_;
      edm::EDGetTokenT<pat::JetCollection> jetsToken_;
      double ptMin_;
      
      // histograms
      edm::Service<TFileService> fs_;
      std::map<std::string, TH1F*> h_jets_;
      std::map<std::string, TH1F*> h_jets_id_;

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
PatJets::PatJets(const edm::ParameterSet& config):
patJets_(config.getParameter<edm::InputTag> ("PatJets")),
jetsToken_(consumes<pat::JetCollection>(patJets_)),
ptMin_(config.getParameter<double> ("PTMin"))
{
   //now do what ever initialization is needed
   usesResource("TFileService");

}


PatJets::~PatJets()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
PatJets::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   edm::Handle<pat::JetCollection> jetsHandler;
   event.getByLabel(patJets_, jetsHandler);
   const pat::JetCollection & jets = *(jetsHandler.product());
   
   int njets = 0;
   int njets_id = 0;
   
   for ( auto & jet : jets )
   {
      if ( ptMin_ > 0 && jet.pt() < ptMin_ ) continue;
      
      double nHadFrac = jet.neutralHadronEnergyFraction();
      double nEmFrac  = jet.neutralEmEnergyFraction();
      int    nMult    = jet.neutralMultiplicity();
      double cHadFrac = jet.chargedHadronEnergyFraction();
      double cEmFrac  = jet.chargedEmEnergyFraction();
      int    cMult    = jet.chargedMultiplicity();
      double muFrac   = jet.muonEnergyFraction();
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
      
      
      h_jets_["pt"] -> Fill(jet.pt());
      h_jets_["eta"] -> Fill(fabs(jet.eta()));
      h_jets_["phi"] -> Fill(jet.phi());
      
      h_jets_["nHadFrac"] -> Fill(nHadFrac);
      h_jets_["nEmFrac"]  -> Fill(nEmFrac );
      h_jets_["nMult"]    -> Fill(nMult   );
      h_jets_["cHadFrac"] -> Fill(cHadFrac);
      h_jets_["cEmFrac"]  -> Fill(cEmFrac );
      h_jets_["cMult"]    -> Fill(cMult   );
      h_jets_["muFrac"]   -> Fill(muFrac  );
      h_jets_["nConst"]   -> Fill(nConst  );
      
      ++njets;
      
      if ( looseId )
      {
         h_jets_id_["pt"] -> Fill(jet.pt());
         h_jets_id_["eta"] -> Fill(fabs(jet.eta()));
         h_jets_id_["phi"] -> Fill(jet.phi());
         
         h_jets_id_["nHadFrac"] -> Fill(nHadFrac);
         h_jets_id_["nEmFrac"]  -> Fill(nEmFrac );
         h_jets_id_["nMult"]    -> Fill(nMult   );
         h_jets_id_["cHadFrac"] -> Fill(cHadFrac);
         h_jets_id_["cEmFrac"]  -> Fill(cEmFrac );
         h_jets_id_["cMult"]    -> Fill(cMult   );
         h_jets_id_["muFrac"]   -> Fill(muFrac  );
         h_jets_id_["nConst"]   -> Fill(nConst  );
         
         ++njets_id;
         
      }
      
      
      
      
   }     
   
   h_jets_["n"] -> Fill(float(njets));
   h_jets_id_["n"] -> Fill(float(njets_id));                              
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
PatJets::beginJob()
{
   createHistograms();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PatJets::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PatJets::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//
void PatJets::createHistograms()
{
   h_jets_["n"]       = fs_->make<TH1F>("n" , "" , 30, 0, 30);
   h_jets_["pt"]       = fs_->make<TH1F>("pt" , ""  , 500, 0, 100);
   h_jets_["eta"]      = fs_->make<TH1F>("eta" , "" , 100, -5, 5);
   h_jets_["phi"]      = fs_->make<TH1F>("phi" , "" , 100, -4, 4);
   
   h_jets_["nHadFrac"] = fs_->make<TH1F>("nHadFrac" , "" , 100, 0, 1);
   h_jets_["nEmFrac"]  = fs_->make<TH1F>("nEmFrac"  , "" , 100, 0, 1);
   h_jets_["nMult"]    = fs_->make<TH1F>("nMult"    , "" , 40, 0, 40);
   h_jets_["cHadFrac"] = fs_->make<TH1F>("cHadFrac" , "" , 100, 0, 1);
   h_jets_["cEmFrac"]  = fs_->make<TH1F>("cEmFrac"  , "" , 100, 0, 1);
   h_jets_["cMult"]    = fs_->make<TH1F>("cMult"    , "" , 40, 0, 40);
   h_jets_["muFrac"]   = fs_->make<TH1F>("muFrac"   , "" , 100, 0, 1);
   h_jets_["nConst"]   = fs_->make<TH1F>("nConst"   , "" , 40, 0, 40);
   
   //
   h_jets_id_["n"]       = fs_->make<TH1F>("n_idloose" , "" , 30, 0, 30);
   h_jets_id_["pt"]       = fs_->make<TH1F>("pt_idloose" , "" , 500, 0, 100);
   h_jets_id_["eta"]      = fs_->make<TH1F>("eta_idloose" , "" , 100, -5, 5);
   h_jets_id_["phi"]      = fs_->make<TH1F>("phi_idloose" , "" , 100, -4, 4);
   
   h_jets_id_["nHadFrac"] = fs_->make<TH1F>("nHadFrac_idloose" , "" , 100, 0, 1);
   h_jets_id_["nEmFrac"]  = fs_->make<TH1F>("nEmFrac_idloose"  , "" , 100, 0, 1);
   h_jets_id_["nMult"]    = fs_->make<TH1F>("nMult_idloose"    , "" , 40, 0, 40);
   h_jets_id_["cHadFrac"] = fs_->make<TH1F>("cHadFrac_idloose" , "" , 100, 0, 1);
   h_jets_id_["cEmFrac"]  = fs_->make<TH1F>("cEmFrac_idloose"  , "" , 100, 0, 1);
   h_jets_id_["cMult"]    = fs_->make<TH1F>("cMult_idloose"    , "" , 40, 0, 40);
   h_jets_id_["muFrac"]   = fs_->make<TH1F>("muFrac_idloose"   , "" , 100, 0, 1);
   h_jets_id_["nConst"]   = fs_->make<TH1F>("nConst_idloose"   , "" , 40, 0, 40);
   
}

//define this as a plug-in
DEFINE_FWK_MODULE(PatJets);
