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
   coll_ = "Jets";
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

void Btagging::efficiencies()
{
   this->efficiencies(coll_);
}

void Btagging::efficiencies(const std::string & coll)
{
   int nentries = this->size();
   
   efficiencyHistograms();
   
   for ( int i = 0 ; i < nentries ; ++i )
   {
      this->event(i);
      
      auto jets = this->collection<Jet>(coll);
      
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         auto jet = jets->at(j);
         h2d_eff_["h_pt_eta_bjet"]   ->Fill(jet.pt(),fabs(jet.eta()));
         
         // Working point histograms
         if ( jet.btag() > wp_ )
         {
            h2d_eff_["h_pt_eta_bjet_wp"]->Fill(jet.pt(),fabs(jet.eta()));
         }
      }
      
      
   }
   
   TFile out("BtagEfficiencies.root","recreate");
   for ( auto & histo : h2d_eff_ )
   {
      histo.second->Write();
   }
   out.Close();
   
   
}

void Btagging::efficiencyHistograms()
{
   if ( ! h2d_eff_.empty() ) return;  // histograms have been defined
   
//   h2d_eff_["h_pt_eta_bjet"]    = new TH2F("h_pt_eta_bjet"   ,"",50,0,1000,4,0,2.5);
//   h2d_eff_["h_pt_eta_bjet_wp"] = new TH2F("h_pt_eta_bjet_wp","",50,0,1000,4,0,2.5);
   h2d_eff_["h_pt_eta_bjet"]    = new TH2F("h_pt_eta_bjet"   ,"",nptbins_,ptbins_,netabins_,etabins_);
   h2d_eff_["h_pt_eta_bjet_wp"] = new TH2F("h_pt_eta_bjet_wp","",nptbins_,ptbins_,netabins_,etabins_);
   
}
