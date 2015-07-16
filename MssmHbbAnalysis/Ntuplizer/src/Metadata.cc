/**\class Metadata Metadata.cc MssmHbbAnalysis/Ntuplizer/src/Metadata.cc

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
#include <exception>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "MssmHbbAnalysis/Ntuplizer/interface/Metadata.h"


//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

//
// constructors and destructor
//
Metadata::Metadata()
{
   // default constructor
}

Metadata::Metadata(edm::Service<TFileService> & fs, const std::string & dir ) 
{
   vdefinitions_.clear();
   mainDir_ = fs->mkdir(dir);
   
   fs_ = &fs;
   
   isGenFilter_ = false;
   isEvtFilter_ = false;
}

Metadata::~Metadata()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void Metadata::Fill()
{
   for ( auto & definitions : vdefinitions_ )
      definitions -> Fill();
   
   if ( isGenFilter_ ) genfilter_ -> Fill();
   if ( isEvtFilter_ ) evtfilter_ -> Fill();
   
}

// ------------ method called once each job just before starting event loop  ------------
void Metadata::AddDefinitions(const std::vector<std::string> & names, const std::vector<std::string> & aliases)
{
   vdefinitions_.push_back(pDefinitions ( new Definitions(mainDir_) ));
   vdefinitions_.back() -> Add(names,aliases);
}

void Metadata::AddDefinitions(const std::vector<std::string> & names, const std::vector<std::string> & aliases, const std::string & category)
{
   vdefinitions_.push_back(pDefinitions ( new Definitions(mainDir_, category) ));
   vdefinitions_.back() -> Add(names,aliases);
}

void Metadata::SetGeneratorFilter(const edm::InputTag & genFilterInfo )
{
   genfilter_ = pGenFilter( new GenFilter(mainDir_, {genFilterInfo} ));
   isGenFilter_ = true;
}

void Metadata::SetEventFilter(const std::vector<edm::InputTag> & filterInfos )
{
   evtfilter_ = pEvtFilter( new EvtFilter(mainDir_, filterInfos ));
   isEvtFilter_ = true;
}


void Metadata::IncrementEventFilters( edm::LuminosityBlock const& lumi )
{
   
   if ( isGenFilter_ ) genfilter_ -> Increment(lumi);
   if ( isEvtFilter_ ) evtfilter_ -> Increment(lumi);
}

GenFilter & Metadata::GetGeneratorFilter()
{
   // genfilter_ should not go out of scope after returning the reference, should be safe(?)
   return *genfilter_;
}
EvtFilter & Metadata::GetEventFilter()
{
   return *evtfilter_;
}
