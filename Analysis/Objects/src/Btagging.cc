/**\class MssmHbb Btagging.cc Analysis/Objects/src/Btagging.cc

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
#include <iostream>
// 
// user include files
#include "Analysis/Objects/interface/Btagging.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::objects;

//
// constructors and destructor
//
Btagging::Btagging(const std::string & inputFilelist, const std::string & evtinfo) : Object(inputFilelist,evtinfo)
{
   jets_    = "Jets";
   flavdef_ = "Hadron";
   balgo_   = "btag_csvivf";
   ptmin_   = 20.;
   etamax_  = 2.5;
   xfrmax_  = 0.4;
   xfptmin_ = 5.;
}

Btagging::~Btagging()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

// void Btagging::efficiencies()
// {
//    this->efficiencies(jets_);
// }

//void Btagging::efficiencies(const std::string & coll)
void Btagging::efficiencies()
{
   int nentries = this->size();
   
   efficiencyHistograms();
   
   std::cout << "*** Btagging::efficiencies() running on " << nentries << " events." << std::endl; 
   
   for ( int i = 0 ; i < nentries ; ++i )
   {

      if ( i > 0 && i % 100000 == 0 )
      std::cout << i << " events processed..." << std::endl;
      
      this->event(i);
      
      auto jets = this->collection<Jet>(jets_);
      
      // GenParticles
      std::shared_ptr< Collection<GenParticle> > particles; 
      if ( genparticles_ == "GenParticles" )
      {
         particles = this->collection<GenParticle>(genparticles_);
         if ( flavdef_ == "Extended" )
         {
            jets->associatePartons(particles,xfrmax_,xfptmin_);
         }
      }
      else
      {
         // by accident the flavour definition is extended must use the default
         if ( flavdef_ == "Extended" )
         {
            this->flavourDefinition();
            std::cout << "*** Btagging::efficiencies() trying to run Extended flavour without GenParticles." << std::endl; 
            std::cout << "                             will switch to default definition." << std::endl; 
         }
      }
      
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         auto jet = jets->at(j);
         if ( !jet.idLoose() || jet.pt() < ptmin_ || fabs(jet.eta()) > etamax_ ) continue;
         
         std::string flavour;

         if ( flavdef_ == "Extended" )
         {
            flavour = jet.extendedFlavour();
            if ( flavour == "udsg" ) flavour = "l";

         }
         else
         {
            if ( abs(jet.flavour(flavdef_)) == 5 ) flavour = "b";
            if ( abs(jet.flavour(flavdef_)) == 4 ) flavour = "c";
            if ( abs(jet.flavour(flavdef_)) != 5 && abs(jet.flavour(flavdef_)) != 4 ) flavour = "l";
         }
         
         h2d_eff_["h_"+flavour+"jet_pt_eta"]   ->Fill(jet.pt(),fabs(jet.eta()));
         
         // Working point histograms
         if ( jet.btag(balgo_) > wp_ )
         {
            h2d_eff_["h_"+flavour+"jet_pt_eta_wp"]->Fill(jet.pt(),fabs(jet.eta()));
         }

      }
      
      
   }
   
   // Efficiency histograms
   // B JETS
   h2d_eff_["h_bjet_eff_pt_eta"] = (TH2F*) h2d_eff_["h_bjet_pt_eta_wp"] -> Clone("h_bjet_eff_pt_eta");
   h2d_eff_["h_bjet_eff_pt_eta"] -> Divide(h2d_eff_["h_bjet_pt_eta"]);
   // C JETS
   h2d_eff_["h_cjet_eff_pt_eta"] = (TH2F*) h2d_eff_["h_cjet_pt_eta_wp"] -> Clone("h_cjet_eff_pt_eta");
   h2d_eff_["h_cjet_eff_pt_eta"] -> Divide(h2d_eff_["h_cjet_pt_eta"]);
   // UDSG JETS
   h2d_eff_["h_ljet_eff_pt_eta"] = (TH2F*) h2d_eff_["h_ljet_pt_eta_wp"] -> Clone("h_ljet_eff_pt_eta");
   h2d_eff_["h_ljet_eff_pt_eta"] -> Divide(h2d_eff_["h_ljet_pt_eta"]);
   
   if ( genparticles_ == "GenParticles" && flavdef_ == "Extended" )
   {
      // B JETS
      h2d_eff_["h_bbjet_eff_pt_eta"] = (TH2F*) h2d_eff_["h_bbjet_pt_eta_wp"] -> Clone("h_bbjet_eff_pt_eta");
      h2d_eff_["h_bbjet_eff_pt_eta"] -> Divide(h2d_eff_["h_bbjet_pt_eta"]);
      // C JETS
      h2d_eff_["h_ccjet_eff_pt_eta"] = (TH2F*) h2d_eff_["h_ccjet_pt_eta_wp"] -> Clone("h_ccjet_eff_pt_eta");
      h2d_eff_["h_ccjet_eff_pt_eta"] -> Divide(h2d_eff_["h_ccjet_pt_eta"]);
      
   }      
   
   // Output
   TFile out(Form("BtagEfficiencies_%s_%1.3f.root",balgo_.c_str(),wp_),"recreate");
   for ( auto & histo : h2d_eff_ )
   {
      histo.second->SetDrawOption("LEGO");
      histo.second->GetXaxis()->SetTitle("pT (GeV)");
      histo.second->GetYaxis()->SetTitle("eta");
      histo.second->GetZaxis()->SetTitle("entries");
      if ( histo.first.find("eff") != std::string::npos )
         histo.second->GetZaxis()->SetTitle("efficiency");
      
      histo.second->Write();
   }
   out.Close();
   
   
}

// void Btagging::jetsCollection(const std::string & unique_name)
// {
//    jets_ = unique_name;
// }
// 
// void Btagging::jetsCollection(const std::string & unique_name, const std::string & path)
// {
//    jets_ = "Jets";
//    this->addTree<Jet>(jets_,path);
// }

void Btagging::jetsCollection(const std::string & path)
{
   this->addTree<Jet>(jets_,path);
}

void Btagging::genParticlesCollection(const std::string & path)
{
   genparticles_ = "GenParticles";
   this->addTree<GenParticle>(genparticles_,path);
}



void Btagging::efficiencyHistograms()
{
   if ( ! h2d_eff_.empty() ) return;  // histograms have been defined
   
   // B JETS
   h2d_eff_["h_bjet_pt_eta"]    = new TH2F("h_bjet_pt_eta"   ,"",nptbins_,ptbins_,netabins_,etabins_);
   h2d_eff_["h_bjet_pt_eta_wp"] = new TH2F("h_bjet_pt_eta_wp","",nptbins_,ptbins_,netabins_,etabins_);
   
   // C JETS
   h2d_eff_["h_cjet_pt_eta"]    = new TH2F("h_cjet_pt_eta"   ,"",nptbins_,ptbins_,netabins_,etabins_);
   h2d_eff_["h_cjet_pt_eta_wp"] = new TH2F("h_cjet_pt_eta_wp","",nptbins_,ptbins_,netabins_,etabins_);
   
   // C JETS
   h2d_eff_["h_ljet_pt_eta"]    = new TH2F("h_ljet_pt_eta"   ,"",nptbins_,ptbins_,netabins_,etabins_);
   h2d_eff_["h_ljet_pt_eta_wp"] = new TH2F("h_ljet_pt_eta_wp","",nptbins_,ptbins_,netabins_,etabins_);
   
   
   if ( genparticles_ == "GenParticles" && flavdef_ == "Extended" )
   {
      // B JETS
      h2d_eff_["h_bbjet_pt_eta"]    = new TH2F("h_bbjet_pt_eta"   ,"",nptbins_,ptbins_,netabins_,etabins_);
      h2d_eff_["h_bbjet_pt_eta_wp"] = new TH2F("h_bbjet_pt_eta_wp","",nptbins_,ptbins_,netabins_,etabins_);
      
      // C JETS
      h2d_eff_["h_ccjet_pt_eta"]    = new TH2F("h_ccjet_pt_eta"   ,"",nptbins_,ptbins_,netabins_,etabins_);
      h2d_eff_["h_ccjet_pt_eta_wp"] = new TH2F("h_ccjet_pt_eta_wp","",nptbins_,ptbins_,netabins_,etabins_);
      
   }
      
   
}
