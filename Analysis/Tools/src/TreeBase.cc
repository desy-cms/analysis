/**\class TreeBase TreeBase.cc Analysis/Tools/src/TreeBase.cc

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
#include <string>
#include <algorithm> 
// 
// user include files
#include "Analysis/Tools/interface/TreeBase.h"


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

//
// constructors and destructor
//
// template <typename Object>
TreeBase::TreeBase() : TChain()
{
}
//template <typename Object>
TreeBase::TreeBase(TChain * tree, const std::string & name) : TChain()
{
   tree_ = tree;
   name_ = name;
   
   std::string treeTitle = std::string(tree_->GetTitle());
   treeTitle.erase(std::remove(treeTitle.begin(),treeTitle.end(),' '),treeTitle.end());
   className_ = treeTitle.substr(0,treeTitle.find_first_of("|"));
   inputTag_  = treeTitle.substr(treeTitle.find_first_of("|")+1);
   
   TObjArray * treeBranches = tree_->GetListOfBranches();
   for ( int i = 0 ; i < treeBranches->GetEntries() ; ++i )
      branches_.push_back(treeBranches->At(i)->GetName());
   
}


TreeBase::~TreeBase()
{
}


//
// member functions
//

void TreeBase::event(const int & event) { tree_ -> GetEntry(event); }
TChain * TreeBase::tree() { return tree_; }

