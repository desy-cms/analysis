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
   jets_     = "Jets";
   flavdef_  = "Hadron";
   balgo_    = "btag_csvivf";
   ptmin_    = 20.;
   etamax_   = 2.5;
   xfrmax_   = 0.4;
   xfptmin_  = 5.;
   lumi_     = -1.;
   njetsmin_ = 1;
   njetsmax_ = -1;
   drmin_    = -1.;
   doperjet_ = false;
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
void Btagging::efficiencies(const int & nevts)
{
   int nentries = this->size();
   
   if ( nevts > 0 )
      nentries = nevts;
   
   // histograms
   histograms();
   if ( njetsmin_ == njetsmax_ )
      histogramsPerJet();
   histogramsSettings();
   
   //
   if ( njetsmin_ != njetsmax_ && njetsmin_ < 2 ) // no sense to apply deltaR between jets
      drmin_ = -1.;
   
   std::cout << "*** Btagging::efficiencies() " << std::endl;
   std::cout << "    - Total number of events  = " << nentries << " events" << std::endl; 
   
   float lumiScale = 1.;
   if ( lumi_ > 0 )
   {
      float sampleLumi = nentries/this->crossSection();
      lumiScale  = lumi_/sampleLumi;
      std::cout << "    - Target luminosity       = " << lumi_ << " pb-1" << std::endl;
      std::cout << "    - Sample cross section    = " << this->crossSection() << " pb" << std::endl;
      std::cout << "    - Luminosity scale factor = " << lumiScale << std::endl;
   }
   else
   {
      std::cout << "    - No luminosity scaling will be performed." << std::endl;
   }
   
   
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
      
      std::vector<Jet> selJets;
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         if ( !jet.idLoose() || jet.pt() < ptmin_ || fabs(jet.eta()) > etamax_ ) continue;
         selJets.push_back(jet);
         if ( njetsmax_ > 0 && int(selJets.size()) == njetsmax_ ) break;
      }
      
      if ( int(selJets.size()) < njetsmin_ ) continue;
      
      if ( drmin_ > 0 ) // select only events in which the selected jets have deltaR above certain value
      {
         bool ok = true;
         for ( size_t j1 = 0 ; j1 < selJets.size()-1 ; ++j1 )
         {
            Jet jet1 = selJets.at(j1);
            for ( size_t j2 = j1+1 ; j2 < selJets.size() ; ++j2 )
            {
               Jet jet2 = selJets.at(j2);
               if ( jet1.deltaR(jet2) < drmin_ ) ok = false;
            }
         }
         if ( ! ok ) continue;
      }
      
      for ( size_t j = 0 ; j < selJets.size() ; ++j )
      {
         Jet jet = selJets.at(j);
//         if ( !jet.idLoose() || jet.pt() < ptmin_ || fabs(jet.eta()) > etamax_ ) continue;
         
         
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
         
         // Some workaround to allow negative bins of eta
         if ( etabins_[0] < 0 )
            h2d_eff_["h_"+flavour+"jet_pt_eta"]   ->Fill(jet.pt(),jet.eta());
         else
            h2d_eff_["h_"+flavour+"jet_pt_eta"]   ->Fill(jet.pt(),fabs(jet.eta()));
         
         // Working point histograms
         if ( jet.btag(balgo_) > wp_ )
         {
            if ( etabins_[0] < 0. )
               h2d_eff_["h_"+flavour+"jet_pt_eta_wp"]->Fill(jet.pt(),jet.eta());
            else
               h2d_eff_["h_"+flavour+"jet_pt_eta_wp"]->Fill(jet.pt(),fabs(jet.eta()));
         }

         if ( doperjet_ ) // this more or less repeats what was done above in case of exclusive number of jets
         {
            // Some workaround to allow negative bins of eta
            if ( etabins_[0] < 0 )
               h2d_eff_[Form("h_%sjet%i_pt_eta",flavour.c_str(),int(j+1))]   ->Fill(jet.pt(),jet.eta());
            else
               h2d_eff_[Form("h_%sjet%i_pt_eta",flavour.c_str(),int(j+1))]   ->Fill(jet.pt(),fabs(jet.eta()));
            
            // Working point histograms
            if ( jet.btag(balgo_) > wp_ )
            {
               if ( etabins_[0] < 0. )
                  h2d_eff_[Form("h_%sjet%i_pt_eta_wp",flavour.c_str(),int(j+1))]->Fill(jet.pt(),jet.eta());
               else
                  h2d_eff_[Form("h_%sjet%i_pt_eta_wp",flavour.c_str(),int(j+1))]->Fill(jet.pt(),fabs(jet.eta()));
            }
            
         }         
         
      }
      
      
   }
   
   // Scale to luminosity (need to consider possible generator filter)
   if ( lumi_ > 0. )
   {
      for ( auto & histo : h2d_eff_ )
      {
         histo.second->Scale(lumiScale);
      }
   }
   
   // Efficiency histograms (TODO: TEfficiency objects instead)
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
   
   if ( doperjet_ ) // this more or less repeats what was done above in case of exclusive number of jets
   {
      for ( int i = 0; i < njetsmax_ ; ++i )
      {
         // B JETS
         h2d_eff_[Form("h_bjet%i_eff_pt_eta",i+1)] = (TH2F*) h2d_eff_[Form("h_bjet%i_pt_eta_wp",i+1)] -> Clone(Form("h_bjet%i_eff_pt_eta",i+1));
         h2d_eff_[Form("h_bjet%i_eff_pt_eta",i+1)] -> Divide(h2d_eff_[Form("h_bjet%i_pt_eta",i+1)]);
         // C JETS
         h2d_eff_[Form("h_cjet%i_eff_pt_eta",i+1)] = (TH2F*) h2d_eff_[Form("h_cjet%i_pt_eta_wp",i+1)] -> Clone(Form("h_cjet%i_eff_pt_eta",i+1));
         h2d_eff_[Form("h_cjet%i_eff_pt_eta",i+1)] -> Divide(h2d_eff_[Form("h_cjet%i_pt_eta",i+1)]);
         // UDSG JETS
         h2d_eff_[Form("h_ljet%i_eff_pt_eta",i+1)] = (TH2F*) h2d_eff_[Form("h_ljet%i_pt_eta_wp",i+1)] -> Clone(Form("h_ljet%i_eff_pt_eta",i+1));
         h2d_eff_[Form("h_ljet%i_eff_pt_eta",i+1)] -> Divide(h2d_eff_[Form("h_ljet%i_pt_eta",i+1)]);
         
         if ( genparticles_ == "GenParticles" && flavdef_ == "Extended" )
         {
            // B JETS
            h2d_eff_[Form("h_bbjet%i_eff_pt_eta",i+1)] = (TH2F*) h2d_eff_[Form("h_bbjet%i_pt_eta_wp",i+1)] -> Clone(Form("h_bbjet%i_eff_pt_eta",i+1));
            h2d_eff_[Form("h_bbjet%i_eff_pt_eta",i+1)] -> Divide(h2d_eff_[Form("h_bbjet%i_pt_eta",i+1)]);
            // C JETS
            h2d_eff_[Form("h_ccjet%i_eff_pt_eta",i+1)] = (TH2F*) h2d_eff_[Form("h_ccjet%i_pt_eta_wp",i+1)] -> Clone(Form("h_ccjet%i_eff_pt_eta",i+1));
            h2d_eff_[Form("h_ccjet%i_eff_pt_eta",i+1)] -> Divide(h2d_eff_[Form("h_ccjet%i_pt_eta",i+1)]);
            
         }      
      }
   }
   // Output
   TFile out(Form("BtagEfficiencies_%s_%1.3f.root",balgo_.c_str(),wp_),"recreate",Form("%s:%s:%1.3f",flavdef_.c_str(),balgo_.c_str(),wp_));
   for ( auto & histo : h2d_eff_ )
   {
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

void Btagging::scaleLuminosity(const float & lumi)
{
   lumi_ = lumi;
}



void Btagging::histograms()
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

void Btagging::histogramsPerJet()
{

   if ( njetsmin_ != njetsmax_ || njetsmin_ < 2 )  return;  // for simplicity only for exclusive number of jets 
   
   doperjet_ = true;
   
   for ( int i = 0; i < njetsmax_ ; ++i )
   {
      // B JETS
      h2d_eff_[Form("h_bjet%i_pt_eta",i+1)]    = new TH2F(Form("h_bjet%i_pt_eta",i+1)   ,"",nptbins_,ptbins_,netabins_,etabins_);
      h2d_eff_[Form("h_bjet%i_pt_eta_wp",i+1)] = new TH2F(Form("h_bjet%i_pt_eta_wp",i+1),"",nptbins_,ptbins_,netabins_,etabins_);
      
      // C JETS
      h2d_eff_[Form("h_cjet%i_pt_eta",i+1)]    = new TH2F(Form("h_cjet%i_pt_eta",i+1)   ,"",nptbins_,ptbins_,netabins_,etabins_);
      h2d_eff_[Form("h_cjet%i_pt_eta_wp",i+1)] = new TH2F(Form("h_cjet%i_pt_eta_wp",i+1),"",nptbins_,ptbins_,netabins_,etabins_);
      
      // C JETS
      h2d_eff_[Form("h_ljet%i_pt_eta",i+1)]    = new TH2F(Form("h_ljet%i_pt_eta",i+1)   ,"",nptbins_,ptbins_,netabins_,etabins_);
      h2d_eff_[Form("h_ljet%i_pt_eta_wp",i+1)] = new TH2F(Form("h_ljet%i_pt_eta_wp",i+1),"",nptbins_,ptbins_,netabins_,etabins_);
      
      
      if ( genparticles_ == "GenParticles" && flavdef_ == "Extended" )
      {
         // B JETS
         h2d_eff_[Form("h_bbjet%i_pt_eta",i+1)]    = new TH2F(Form("h_bbjet%i_pt_eta",i+1)   ,"",nptbins_,ptbins_,netabins_,etabins_);
         h2d_eff_[Form("h_bbjet%i_pt_eta_wp",i+1)] = new TH2F(Form("h_bbjet%i_pt_eta_wp",i+1),"",nptbins_,ptbins_,netabins_,etabins_);
         
         // C JETS
         h2d_eff_[Form("h_ccjet%i_pt_eta",i+1)]    = new TH2F(Form("h_ccjet%i_pt_eta",i+1)   ,"",nptbins_,ptbins_,netabins_,etabins_);
         h2d_eff_[Form("h_ccjet%i_pt_eta_wp",i+1)] = new TH2F(Form("h_ccjet%i_pt_eta_wp",i+1),"",nptbins_,ptbins_,netabins_,etabins_);
         
      }
   }
   
}
void Btagging::histogramsSettings()
{
   for ( auto & histo : h2d_eff_ )
   {
      histo.second -> Sumw2();
      histo.second -> SetDrawOption("LEGO");
      histo.second -> GetXaxis() -> SetTitle("pT (GeV)");
      histo.second -> GetYaxis() -> SetTitle("eta");
      histo.second -> GetZaxis() -> SetTitle("entries");
   }
   
}
