/**\class EventFilter EventFilter.cc Analysis/Ntuplizer/src/EventFilter.cc

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
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "Analysis/Ntuplizer/interface/EventFilter.h"



//
// class declaration
//

using namespace mssmhbb;
using namespace mssmhbb::ntuple;

// member functions specialization - needed to be declared in the same namespace as the class
namespace mssmhbb {
   namespace ntuple {
      template <> void EventFilter<edm::MergeableCounter>::Increment(edm::LuminosityBlock const&);
 }
}   

//
// constructors and destructor
//
// EventFilter::EventFilter()
// {
//    // default constructor
// }

template <typename T>
EventFilter<T>::EventFilter(edm::Service<TFileService> & fs, const std::vector<edm::InputTag> & collections) :
      nTotal_(0), nFiltr_(0), collections_(collections)
{
   std::string category = "GeneratorFilter";
   if ( std::is_same<T,edm::MergeableCounter>::value )
      category = "EventFilter";
   
   tree_ = fs->make<TTree>(category.c_str(),category.c_str());
   
   tree_ -> Branch("nEventsTotal"    , &this->nTotal_     , "nEventsTotal/i");
   tree_ -> Branch("nEventsFiltered" , &this->nFiltr_     , "nEventsFiltered/i");
   tree_ -> Branch("filterEfficiency", &this->efficiency_ , "filterEfficiency/D");

   
}

template <typename T>
EventFilter<T>::EventFilter(TFileDirectory & subDir, const std::vector<edm::InputTag> & collections) :
      nTotal_(0), nFiltr_(0), collections_(collections)
{
   std::string category = "GeneratorFilter";
   if ( std::is_same<T,edm::MergeableCounter>::value )
      category = "EventFilter";

   tree_ = subDir.make<TTree>(category.c_str(),category.c_str());
   
   tree_ -> Branch("nEventsTotal"    , &this->nTotal_     , "nEventsTotal/i");
   tree_ -> Branch("nEventsFiltered" , &this->nFiltr_     , "nEventsFiltered/i");
   tree_ -> Branch("filterEfficiency", &this->efficiency_ , "filterEfficiency/D");

}

template <typename T>
EventFilter<T>::~EventFilter()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}

// ------------ other methods ----------------
template <typename T>
TTree * EventFilter<T>::Tree()
{
   return tree_;
}

// ------------ method called once each job just before starting event loop  ------------
template <typename T>
void EventFilter<T>::SetCollections(const std::vector<edm::InputTag> & collections)
{
   collections_ = collections;
}

// ------------ method called at the end of the EDAnalyzer job  ------------
template <typename T>
void EventFilter<T>::Fill()
{
   efficiency_ = this -> Results().efficiency;
   tree_ -> Fill();
}


//
// member functions
//
template <typename T>
FilterResults EventFilter<T>::Results()
{
   FilterResults res;
   double eff = nTotal_ > 0  ? (double) nFiltr_ / (double) nTotal_ : 0.;

   res.total = nTotal_;
   res.filtered =  nFiltr_;
   res.efficiency = eff;
   
   return res;
}


// ------------ method called for each lumi  ------------
template <typename T>
void EventFilter<T>::Increment(edm::LuminosityBlock const& lumi)
{
   using namespace edm;
   
   if ( collections_.size() > 0 )
   {
      Handle<T> handler;
      lumi.getByLabel(collections_[0], handler);
       
      nTotal_  += handler->sumWeights();
      nFiltr_  += handler->sumPassWeights();
   }
   
}

template <>
void EventFilter<edm::MergeableCounter>::Increment(edm::LuminosityBlock const& lumi)
{
   using namespace edm;
   
   if ( collections_.size() > 1 )
   {
      Handle <MergeableCounter> totalHandler;
      lumi.getByLabel(collections_[0],totalHandler);
      nTotal_ += totalHandler -> value;
      
      Handle <MergeableCounter> filtrHandler;
      lumi.getByLabel(collections_[1],filtrHandler);
      nFiltr_ += filtrHandler -> value;
   }
   
}

// Need to declare all possible template classes here
template class EventFilter<edm::MergeableCounter>;
template class EventFilter<GenFilterInfo>;
