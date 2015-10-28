/**\class Collection Collection.cc Analysis/Tools/src/Collection.cc

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
#include "Analysis/Tools/interface/Jet.h"
#include "Analysis/Tools/interface/MET.h"
#include "Analysis/Tools/interface/Muon.h"
#include "Analysis/Tools/interface/Vertex.h"

#include "Analysis/Tools/interface/Collection.h"


// member functions specialization - needed to be declared in the same namespace as the class
namespace analysis {
   namespace tools {
      template <> Collection<Vertex>::Collection(const Objects & objects, const std::string & name);
      template <> std::vector<Candidate> * Collection<Vertex>::vectorCandidates() const;
      template <> void Collection<Vertex>::matchTo( const Collection<Candidate> & collection );
      template <> void Collection<Vertex>::matchTo( const Collection<TriggerObject> & collection );
      template <> void Collection<Vertex>::matchTo( const std::shared_ptr<Collection<TriggerObject> > collection );
   }
}
//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
template <class Object>
Collection<Object>::Collection()
{
}
template <class Object>
Collection<Object>::Collection(const Objects & objects, const std::string & name)
{
   objects_ = objects;
   size_ = (int) objects_.size();
   name_ = name;
   candidates_.clear();
   for ( int i = 0; i < size_ ; ++i ) candidates_.push_back(objects_[i]);
}

template <>
Collection<Vertex>::Collection(const Objects & objects, const std::string & name)
{
   objects_ = objects;
   size_ = (int) objects_.size();
   name_ = name;
}


template <class Object>
Collection<Object>::~Collection()
{
//   std::cout<< this << "  Collection destroyed" << std::endl;
   
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
template <class Object>
std::vector<Object> * Collection<Object>::vector()
{
   return &objects_;
}

template <class Object>
void Collection<Object>::matchTo( const Collection<Candidate> & collection )
{
   for ( auto & obj : objects_ )
      obj.matchTo(collection.vectorCandidates(),collection.name());
}

template <class Object>
void Collection<Object>::matchTo( const Collection<TriggerObject> & collection )
{
   for ( auto & obj : objects_ )
      obj.matchTo(collection.vectorCandidates(),collection.name());
}

template <class Object>
void Collection<Object>::matchTo( const std::shared_ptr<Collection<TriggerObject> > collection )
{
   this->matchTo(*collection);
}

template <class Object>
std::vector<Candidate>* Collection<Object>::vectorCandidates() const
{
   return &candidates_;
}

// try to find how the enable_if works to avoid this specialization
// typename std::enable_if<std::is_base_of<Candidate, Object>::value, std::vector<Candidate>* >::type
// std::is_base_of<Foo, Bar>::value
template <>
std::vector<Candidate> * Collection<Vertex>::vectorCandidates() const
{
   return nullptr;
}
// ------------ method called for each event  ------------


// ------------ declare the template classes  ------------
template class Collection<Candidate>;
template class Collection<Jet>;
template class Collection<MET>;
template class Collection<Muon>;
template class Collection<Vertex>;
template class Collection<TriggerObject>;
