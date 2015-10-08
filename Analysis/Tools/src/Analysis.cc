/**\class Analysis Analysis.cc Analysis/Tools/src/Analysis.cc

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
#include "Analysis/Tools/interface/Analysis.h"

using namespace analysis;
using namespace analysis::tools;

//
// class declaration
//

//
// constructors and destructor
//

Analysis::Analysis(const std::string & inputFilelist, const std::string & evtinfo)
{
   inputFilelist_  = inputFilelist;
   fileCollection_ = new TFileCollection("fileCollection","",inputFilelist.c_str());
   fileList_ = (TCollection*) fileCollection_->GetList();

   // event info (must be in the tree always)
   t_event_ = new TChain(evtinfo.c_str());
   t_event_ -> AddFileInfoList(fileList_);
   t_event_ -> SetBranchAddress("event", &event_);
   t_event_ -> SetBranchAddress("run", &run_);
   t_event_ -> SetBranchAddress("lumisection", &lumi_);

   nevents_ = t_event_ -> GetEntries();

   t_event_ -> GetEntry(1); // Check whether it's mc simulation
   if (run_ == 1) {         // A bit stupid, but it's only solution that I found for the moment
     is_mc_ = true;
   } else {
     is_mc_ = false;
   }

}

Analysis::~Analysis()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//
// ------------ method called for each event  ------------

int Analysis::numberEvents() { return nevents_; }
int Analysis::size() { return nevents_; }

void Analysis::event(const int & event){ t_event_ -> GetEntry(event); }

int Analysis::event()       { return event_; }
int Analysis::run()         { return run_  ; }
int Analysis::lumiSection() { return lumi_ ; }
bool Analysis::isMC() { return is_mc_ ; }


// TREES
void Analysis::treeInit_(const std::string & unique_name, const std::string & path)
{
   std::string treeTitle = ((TTree*) t_event_->GetFile()->Get(path.c_str())) -> GetTitle();
   tree_[unique_name] = new TChain(path.c_str(),treeTitle.c_str());
   tree_[unique_name] -> AddFileInfoList(fileList_);
   t_event_ -> AddFriend(tree_[unique_name]);

   treeTitle.erase(std::remove(treeTitle.begin(),treeTitle.end(),' '),treeTitle.end());
   std::string classname = treeTitle.substr(0,treeTitle.find_first_of("|"));
   std::string inputTag  = treeTitle.substr(treeTitle.find_first_of("|")+1);

}
// JETS
template<> pJetTree Analysis::addTree(const std::string & unique_name, const std::string & path) // a bit stupid but I could not make template work here
{
   this->treeInit_(unique_name,path);
   t_jets_[unique_name] = pJetTree( new PhysicsObjectTree<Jet>(tree_[unique_name], unique_name) );
   return t_jets_[unique_name];
}
template<> pJetTree    Analysis::tree(const std::string & unique_name) { return t_jets_    [unique_name]; }

// METS
template<> pMETTree Analysis::addTree(const std::string & unique_name, const std::string & path) // a bit stupid but I could not make template work here
{
   this->treeInit_(unique_name,path);
   t_mets_[unique_name] = pMETTree( new PhysicsObjectTree<MET>(tree_[unique_name], unique_name) );
   return t_mets_[unique_name];
}
template<> pMETTree    Analysis::tree(const std::string & unique_name) { return t_mets_    [unique_name]; }

// MUONS
template<> pMuonTree Analysis::addTree(const std::string & unique_name, const std::string & path) // a bit stupid but I could not make template work here
{
   this->treeInit_(unique_name,path);
   t_muons_[unique_name] = pMuonTree( new PhysicsObjectTree<Muon>(tree_[unique_name], unique_name) );
   return t_muons_[unique_name];
}
template<> pMuonTree   Analysis::tree(const std::string & unique_name) { return t_muons_   [unique_name]; }

// VERTICES
template<> pVertexTree Analysis::addTree(const std::string & unique_name, const std::string & path) // a bit stupid but I could not make template work here
{
   this->treeInit_(unique_name,path);
   t_vertices_[unique_name] = pVertexTree( new PhysicsObjectTree<Vertex>(tree_[unique_name], unique_name) );
   return t_vertices_[unique_name];
}
template<> pVertexTree Analysis::tree(const std::string & unique_name) { return t_vertices_[unique_name]; }

// ===========================================================
// =================   METADATA   ============================
// ===========================================================
// ===========================================================
// ------------ methods called for metadata  ------------
void Analysis::crossSections(const std::string & path)
{
   t_xsection_  = new TChain(path.c_str());
   int ok = t_xsection_ -> AddFileInfoList(fileList_);
   if ( ok == 0 )
   {
      std::cout << "tree does not exist" << std::endl;
      return;
   }
   TObjArray * xsecBranches = t_xsection_->GetListOfBranches();
   for ( int i = 0 ; i < xsecBranches->GetEntries() ; ++i )
   {
      std::string branch = xsecBranches->At(i)->GetName();
      if ( branch == "run" ) continue;
      xsections_[branch] = 0;
      t_xsection_ -> SetBranchAddress(branch.c_str(), &xsections_[branch]);
   }
   t_xsection_ -> GetEntry(0);
}

double Analysis::crossSection()
{
   return this -> crossSection("crossSection");
}
double Analysis::crossSection(const std::string & xs)
{
   if ( t_xsection_ == NULL ) return -1.;
   return xsections_[xs];
}

void   Analysis::listCrossSections()
{
   std::cout << "=======================================================" << std::endl;
   std::cout << "  CROSS SECTIONS" << std::endl;
   std::cout << "=======================================================" << std::endl;
   if ( t_xsection_ == NULL )
   {
      std::cout << "No cross section tree has been declared." << std::endl;
      std::cout << "=======================================================" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
      return;
   }

   for ( auto& xs : xsections_ )
   {
      std::cout << xs.first << " = " << xs.second << " pb " << std::endl;
   }
   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
}

FilterResults Analysis::generatorFilter(const std::string & path)
{
   t_genfilter_  = new TChain(path.c_str());
   t_genfilter_ -> AddFileInfoList(fileList_);

   unsigned int ntotal;
   unsigned int nfiltered;
   unsigned int sumtotal = 0;
   unsigned int sumfiltered = 0;

   t_genfilter_ -> SetBranchAddress("nEventsTotal", &ntotal);
   t_genfilter_ -> SetBranchAddress("nEventsFiltered", &nfiltered);

   for ( int i = 0; i < t_genfilter_->GetEntries(); ++i )
   {
      t_genfilter_ -> GetEntry(i);
      sumtotal += ntotal;
      sumfiltered += nfiltered;
   }


   genfilter_.total = sumtotal;
   genfilter_.filtered = sumfiltered;
   genfilter_.efficiency = float(sumfiltered)/sumtotal;

   return genfilter_;
}

void Analysis::listGeneratorFilter()
{
   std::cout << "=======================================================" << std::endl;
   std::cout << "  GENERATOR FILTER" << std::endl;
   std::cout << "=======================================================" << std::endl;
   if ( t_genfilter_ == NULL )
   {
      std::cout << "No generator tree has been declared." << std::endl;
      std::cout << "=======================================================" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
      return;
   }
   std::cout << "Total generated events      = " << genfilter_.total << std::endl;
   std::cout << "Filtered generated events   = " << genfilter_.filtered << std::endl;
   std::cout << "Generator Filter Efficiency = " << genfilter_.efficiency << std::endl;

   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;


}
