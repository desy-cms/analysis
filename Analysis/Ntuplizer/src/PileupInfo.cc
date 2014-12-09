/**\class PileupInfo PileupInfo.cc MssmHbbAnalysis/Ntuplizer/src/PileupInfo.cc

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
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "MssmHbbAnalysis/Ntuplizer/interface/PileupInfo.h"
 
//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;
//
// constructors and destructor
//
PileupInfo::PileupInfo()
{
   // default constructor
}

PileupInfo::PileupInfo(const edm::InputTag& tag, TTree* tree)
{
   input_collection_ = tag;
   tree_ = tree;
   
}

PileupInfo::~PileupInfo()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void PileupInfo::pileupInfo(const edm::Event& event)
{
   using namespace edm;
   
   // 
   edm::Handle<std::vector<PileupSummaryInfo> > handler;
   event.getByLabel(input_collection_, handler);
   std::vector<PileupSummaryInfo> pileup_infos = *(handler.product());
   
   for ( size_t i = 0 ; i < pileup_infos.size() ; ++i )
   {
      PileupSummaryInfo pileup_info = pileup_infos.at(i);
      
      if ( pileup_info.getBunchCrossing() != 0 ) continue;
      
      n_true_pu_ = pileup_info.getTrueNumInteractions();
      n_pu_ = pileup_info.getPU_NumInteractions();
      break;
   }
   
   
}


void PileupInfo::fill()
{
   tree_->Fill();
}

void PileupInfo::fill(const edm::Event& event)
{
   pileupInfo(event);
   fill();
}

// ------------ method called once each job just before starting event loop  ------------
void PileupInfo::branches()
{
   tree_->Branch("nTruePileup",  &this->n_true_pu_, "nTruePileup/F");
   tree_->Branch("nPileup",  &this->n_pu_, "nPileup/F");
}

