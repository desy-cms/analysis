/**\class Metadata Metadata.cc Analysis/Ntuplizer/src/Metadata.cc

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
 
#include "Analysis/Ntuplizer/interface/Metadata.h"

#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::ntuple;

//
// constructors and destructor
//
Metadata::Metadata()
{
   // default constructor
}

Metadata::Metadata(edm::Service<TFileService> & fs, const bool & is_mc, const std::string & dir ) 
{
   is_mc_ = is_mc;
   
   vdefinitions_.clear();
   mainDir_ = fs->mkdir(dir);
   mHatDir_ = fs->mkdir("mHatFilter");
   
//   fs_ = &fs;
   
   isGenFilter_ = false;
   isEvtFilter_ = false;
   isMHatEvtFilter_ = false;
   
   if ( is_mc_ )
   {
      // Cross sections tree
      treeXS_ = mainDir_.make<TTree>("CrossSections","Cross Sections");
      // cross section branches
      treeXS_ -> Branch("run"            , &runXS_            , "run/i");
      treeXS_ -> Branch("myCrossSection" , &myXSec_           , "myCrossSection/D");
      treeXS_ -> Branch("crossSection"   , &XSec_             , "crossSection_generator/D");
      treeXS_ -> Branch("internalXSec"   , &internalXSec_     , "internalXSec_generator/D");
      treeXS_ -> Branch("externalXSecLO" , &externalXSecLO_   , "externalXSecLO_generator/D");
      treeXS_ -> Branch("externalXSecNLO", &externalXSecNLO_  , "externalXSecNLO_generator/D");
   }
   
   // Dataset
   treeDS_ = mainDir_.make<TTree>("Dataset","Dataset info");
   treeDS_ -> Branch("isMC"            , &is_mc_            , "isMC/O");

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
   
   if ( isGenFilter_ ) 		genfilter_ 		-> Fill();
   if ( isEvtFilter_ ) 		evtfilter_ 		-> Fill();
   if ( isMHatEvtFilter_) 	mHatEvtFilter_ 	-> Fill();
   
   treeDS_ -> Fill();
   
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

void Metadata::AddDefinitions(const std::vector<TitleAlias> & tas, const std::string & category)
{
   std::vector<std::string> names;
   std::vector<std::string> aliases;
   for ( auto & ta : tas )
   {
      names.push_back(ta.title);
      aliases.push_back(ta.alias);
   }
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

void Metadata::SetMHatEventFilter(const std::vector<edm::InputTag> & filterInfos )
{
	mHatEvtFilter_ = pEvtFilter( new EvtFilter(mHatDir_, filterInfos ));
   isMHatEvtFilter_ = true;
}


void Metadata::IncrementEventFilters( edm::LuminosityBlock const& lumi )
{
   if ( isGenFilter_ ) 		genfilter_ 		-> Increment(lumi);
   if ( isEvtFilter_ ) 		evtfilter_ 		-> Increment(lumi);
   if ( isMHatEvtFilter_) 	mHatEvtFilter_ 	-> Increment(lumi);
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

void Metadata::SetCrossSections( const edm::Run  & run, const edm::InputTag & inputTag, const double & myxs )
{
   if ( is_mc_ )
   {
      runXS_ = run.run();
      
      edm::Handle<GenRunInfoProduct> genRunInfo;
      run.getByLabel( inputTag, genRunInfo );
      myXSec_          = myxs;
      XSec_            = genRunInfo -> crossSection();
      internalXSec_    = genRunInfo -> internalXSec().value();
      externalXSecLO_  = genRunInfo -> externalXSecLO().value();
      externalXSecNLO_ = genRunInfo -> externalXSecNLO().value();
      
      treeXS_ -> Fill();
   }

}
            

