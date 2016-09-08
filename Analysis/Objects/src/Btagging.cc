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
   lumiTarget_= -1.;
   njetsmin_ = 1;
   njetsmax_ = -1;
   nbtags_   = njetsmin_;
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

   std::cout << "*** Btagging::efficiencies() " << std::endl;
   std::cout << "    - Total number of events  = " << nentries << " events" << std::endl; 
   
   
   // number of jets and btags: setting proper values   
   // in case user set "wrong" inputs, reset parameters
   if ( njetsmin_  < 1 )                            njetsmin_ =  1;      // at least one jet must be selected 
   if ( nbtags_   == 0 )                            nbtags_   = -1;      // just avoiding dealing with zero
   if ( njetsmax_ == 0 )                            njetsmax_ = -1;      // just avoiding dealing with zero
   
   // avoiding particular situations
   if ( njetsmin_  < 2 )                            drmin_    = -1.;     // at least two jets are needed for the deltaR cut
   if ( nbtags_    < 0 && njetsmin_ != njetsmax_ )  drmin_    = -1.;     // no deltaR cut when not exclusive btagging
   if ( njetsmax_  > 0 && njetsmin_ > njetsmax_ )   njetsmax_ = -1;      // max cannot be less than min, so make no max selection
   if ( njetsmax_  > 0 && nbtags_   > njetsmax_ )   nbtags_ = njetsmax_; // cannot btag more jets than max, nbtags = max
//   if ( nbtags_ > njetsmin_ || nbtags_ < 1 )      nbtags_ = njetsmin_;
   
   
   std::cout << "    - nJetsMin  = " << njetsmin_ << std::endl;
   std::cout << "    - nJetsMax  = " << njetsmax_ << std::endl;
   std::cout << "    - nbtags    = " << nbtags_   << std::endl;
   std::cout << "    - deltaRMin = " << drmin_    << std::endl;

   
   // histograms
   histograms();
   
   float lumiScale = 1.;
   if ( lumiTarget_ > 0 )
   {
      float sampleLumi = nentries/this->crossSection();
      lumiScale  = lumiTarget_/sampleLumi;
      std::cout << "    - Target luminosity       = " << lumiTarget_ << " pb-1" << std::endl;
      std::cout << "    - Sample cross section    = " << this->crossSection() << " pb" << std::endl;
      std::cout << "    - Luminosity scale factor = " << lumiScale << std::endl;
   }
   else
   {
      std::cout << "    - No luminosity scaling will be performed." << std::endl;
   }
   
   
   for ( int i = 0 ; i < nentries ; ++i )
   {

      if ( (i+1) % 100000 == 0 )
      std::cout << i+1 << " events processed..." << std::endl;
      
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

      // Select jets
      std::vector<Jet> selJetsId;
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         if ( !jet.idLoose() ) continue;
         selJetsId.push_back(jet);
      }
      // Leading jets must fulfil the pt and eta requirements?, once a jet does not fulfill the requirements ignore the other jets
      // This is what we do when selecting signal events MSSM Hbb!
      // Larger fraction of jets from the hard-process?
      std::vector<Jet> selJets;
      for ( int j = 0 ; j < int(selJetsId.size()) ; ++j )
      {
         Jet jet = selJetsId.at(j);
         if ( jet.pt() > ptmin_ && fabs(jet.eta()) < etamax_ )
            selJets.push_back(jet);
         else
            break;
      }
      
      // At least the minimum number of jets
      if ( int(selJets.size()) < njetsmin_ ) continue;
      
      // At most the maximum number of jets
      if ( njetsmax_ > 0 && int(selJets.size()) > njetsmax_ ) continue;
      
      // For an exclusive selection make njtesmin = njetsmax
      
      
      if ( drmin_ > 0 ) // select only events in which the minimmum number of selected jets have deltaR above certain value
      {
         bool ok = true;
         for ( int j1 = 0 ; j1 < njetsmin_-1 ; ++j1 )
         {
            Jet jet1 = selJets.at(j1);
            for ( int j2 = j1+1 ; j2 < njetsmin_ ; ++j2 )
            {
               Jet jet2 = selJets.at(j2);
               if ( jet1.deltaR(jet2) < drmin_ ) ok = false;
            }
         }
         if ( ! ok ) continue;
      }
      
      // deal with negative nbtags, i.e. all selected jets undergo btagging (inclusive btagging)
      int nbtags = nbtags_;
      if ( nbtags < 0 ) nbtags = int(selJets.size());
      
//      std::cout << nbtags << " leading jets will be analysed from " << int(selJets.size()) << " jets..." << std::endl;
      
      // btagging on selected jets, but only for nbtags
      for ( int j = 0 ; j < nbtags ; ++j )
      {
         Jet jet = selJets.at(j);
//         std::cout << "     jet: " << j << " pt = " << jet.pt() << "  , |eta| = " << fabs(jet.eta()) << std::endl; 
         
         std::string flavour = flavour_(jet);
         
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
   if ( lumiTarget_ > 0. )
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
      // deal with negative nbtags
      int nbtags = nbtags_;
      if ( nbtags < 0 ) nbtags = njetsmin_;
      
      for ( int i = 0; i < nbtags ; ++i )
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
   lumiTarget_ = lumi;
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
   
   // histograms per jet
   histogramsPerJet();
   
   // histograms settings
   histogramsSettings();
      
}

void Btagging::histogramsPerJet()
{

   if ( (nbtags_ > 1 && nbtags_ == njetsmin_) || (nbtags_ < 0 && njetsmin_ == njetsmax_) )
      doperjet_ = true;

   if ( ! doperjet_ ) return;
   
   // deal with negative nbtags
   int nbtags = nbtags_;
   if ( nbtags < 0 ) nbtags = njetsmin_;
      
   std::cout << "    - Btagging::histogramsPerJet() - for " << nbtags << " leading jets" << std::endl;
   
   for ( int i = 0; i < nbtags ; ++i )
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

std::string Btagging::flavour_(const Jet & jet)
{
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
   
   return flavour;
}
