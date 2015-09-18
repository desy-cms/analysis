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


//
// class declaration
//

using namespace analysis;
using namespace analysis::tools;

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

int Analysis::NumberOfEvents() { return nevents_; }
void Analysis::Event(const int & i){ t_event_ -> GetEntry(i); }

int Analysis::Event()       { return event_; }
int Analysis::Run()         { return run_  ; }
int Analysis::LumiSection() { return lumi_ ; }

void Analysis::AddPhysicsObject(const std::string & unique_name, const std::string & path, const std::string & type)
{
   t_physobjs_[unique_name] = new TChain(path.c_str(),type.c_str());
   t_physobjs_[unique_name] -> AddFileInfoList(fileList_);
   t_event_ -> AddFriend(t_physobjs_[unique_name]);
}

TChain * Analysis::PhysicsObject(const std::string & unique_name)
{
   return t_physobjs_[unique_name];
}

// ------------ methods called for metadata  ------------
void Analysis::CrossSections(const std::string & path)
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

double Analysis::CrossSection()
{
   return this -> CrossSection("crossSection");
}
double Analysis::CrossSection(const std::string & xs) 
{
   if ( t_xsection_ == NULL ) return -1.;
   return xsections_[xs];
}

void   Analysis::ListCrossSections()
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
   
FilterResults Analysis::GeneratorFilter(const std::string & path) 
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

void Analysis::ListGeneratorFilter() 
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
