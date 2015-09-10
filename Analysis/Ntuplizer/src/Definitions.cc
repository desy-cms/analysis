/**\class Definitions Definitions.cc Analysis/Ntuplizer/src/Definitions.cc

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
#include "FWCore/Framework/interface/Event.h"
// 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "Analysis/Ntuplizer/interface/Definitions.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::ntuple;

//
// constructors and destructor
//
Definitions::Definitions()
{
   // default constructor
}

Definitions::Definitions(edm::Service<TFileService> & fs)
{
   std::string category = "Definitions";
   tree_ = fs->make<TTree>(category.c_str(),category.c_str());
   
}

Definitions::Definitions(edm::Service<TFileService> & fs, const std::string & category )
{
   TFileDirectory subDir = fs->mkdir( "Definitions" );
   tree_ = subDir.make<TTree>(category.c_str(),category.c_str());
}

Definitions::Definitions(TFileDirectory & subDir)
{
   std::string category = "Definitions";
   tree_ = subDir.make<TTree>(category.c_str(),category.c_str());
   
}

Definitions::Definitions(TFileDirectory & subDir, const std::string & category )
{
   TFileDirectory subSubDir = subDir.mkdir( "Definitions" );
   tree_ = subSubDir.make<TTree>(category.c_str(),category.c_str());
   
}

Definitions::~Definitions()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void Definitions::Fill()
{
   tree_ -> Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void Definitions::Init()
{
}

void Definitions::Add(const std::vector<std::string> & names, const std::vector<std::string> & aliases)
{
   for ( size_t i = 0 ; i < names.size() ; ++i )
      this -> Add(names[i],aliases[i]);
}

void Definitions::Add(const std::string & name, const std::string & alias)
{
   tree_ ->Branch(alias.c_str(),(void*)name.c_str(),"string/C",1024);
}

// ------------ other methods ----------------
TTree * Definitions::Tree()
{
   return tree_;
}

