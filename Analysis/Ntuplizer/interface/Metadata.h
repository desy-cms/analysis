#ifndef Analysis_Ntuplizer_Metadata_h
#define Analysis_Ntuplizer_Metadata_h 1

// -*- C++ -*-
//
// Package:    Analysis/Ntuplizer
// Class:      Metadata
// 
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
#include <memory>
// 
// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "Analysis/Ntuplizer/interface/Definitions.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/MergeableCounter.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "Analysis/Ntuplizer/interface/EventFilter.h"

#include "Analysis/Ntuplizer/interface/Utils.h"



#include "TTree.h"


typedef analysis::ntuple::Definitions Definitions;
typedef std::unique_ptr<Definitions> pDefinitions;

typedef analysis::ntuple::EventFilter<GenFilterInfo> GenFilter;
typedef analysis::ntuple::EventFilter<edm::MergeableCounter> EvtFilter;

typedef std::unique_ptr<GenFilter> pGenFilter;
typedef std::unique_ptr<EvtFilter> pEvtFilter;


typedef std::unique_ptr<edm::Service<TFileService> > pTFileService;


//
// class declaration
//

namespace analysis {
   namespace ntuple {

      class Metadata {
         public:
            Metadata();
            Metadata(edm::Service<TFileService> &, const bool & is_mc = false, const std::string & dir = "Metadata" );
            Metadata(TFileDirectory & );
           ~Metadata();
            void Fill();
            TTree * Tree();
            TFileDirectory TreeDir();
            void AddDefinitions(const std::vector<std::string> &, const std::vector<std::string> &);
            void AddDefinitions(const std::vector<std::string> &, const std::vector<std::string> &, const std::string &);
            void AddDefinitions(const std::vector<TitleAlias> &, const std::string &);
            
            void SetGeneratorFilter(const edm::InputTag & );
            void SetEventFilter(const std::vector<edm::InputTag> &);
            void SetMHatEventFilter(const std::vector<edm::InputTag> &);
            void IncrementEventFilters( edm::LuminosityBlock const& );
            
            void SetCrossSections( const edm::Run  &, const edm::InputTag &, const double & myxs = -1. );
            
            
            GenFilter & GetGeneratorFilter();
            EvtFilter & GetEventFilter();

         private:
            // ----------member data ---------------------------
            
//            edm::Service<TFileService> * fs_;
            TFileDirectory mainDir_;
            TFileDirectory mHatDir_;
            
            std::vector<pDefinitions> vdefinitions_;
            
            bool isGenFilter_;
            bool isEvtFilter_;
            bool isMHatEvtFilter_;
            pGenFilter  genfilter_;
            pEvtFilter  evtfilter_;
            pEvtFilter  mHatEvtFilter_;
            
            
            pTFileService pfs_;
            
            // Cross sections tree
            TTree * treeXS_;
            double myXSec_;
            double XSec_;
            double internalXSec_;
            double externalXSecLO_;
            double externalXSecNLO_;
            unsigned int runXS_;
            
            // Dataset tree
            TTree * treeDS_;
            bool is_mc_;

            
            
            
      };
   }
}

#endif  // Analysis_Ntuplizer_Metadata_h
