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
#include <fstream>
#include <cstdlib>
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
   
   //if(is_mc_) crossSection();

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
void Analysis::event(const int & event, const bool & addCollections)
{
   t_event_ -> GetEntry(event);
   if ( !addCollections) return;
   
   for ( auto & tree : t_any_ )
   {
      std::string name = tree.first;
      std::string type = t_type_[name];
      if ( type == "Jet" )            this->addCollection<Jet>(name);
      if ( type == "Muon" )           this->addCollection<Muon>(name);
      if ( type == "MET" )            this->addCollection<MET>(name);
      if ( type == "Vertex" )         this->addCollection<Vertex>(name);
      if ( type == "TriggerObject" )  this->addCollection<TriggerObject>(name);
   }
   
}



// ===========================================================
// ===============         Trees             =================
// ===========================================================
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
// See Analysis.h for the implementations related to template trees


// ===========================================================
// ===============       Collections         =================
// ===========================================================

// See also Analysis.h for the implementations related to template collections

//}
// ===========================================================
// =============== Method for Trigger Results=================
// ===========================================================

void Analysis::triggerResults(const std::string & path)
{
   t_triggerResults_  = new TChain(path.c_str());
   int ok = t_triggerResults_ -> AddFileInfoList(fileList_);
   t_event_ -> AddFriend(t_triggerResults_);
   if ( ok == 0 )
   {
      std::cout << "tree does not exist" << std::endl;
      return;
   }
   TObjArray * triggerBranches = t_triggerResults_ -> GetListOfBranches();
   for ( int i = 0 ; i < triggerBranches->GetEntries() ; ++i )
   {
      std::string branch = triggerBranches->At(i)->GetName();
      triggerResults_[branch] = 0;
      t_triggerResults_ -> SetBranchAddress(branch.c_str(), &triggerResults_[branch]);
   }
}

int Analysis::triggerResult(const std::string & trig)
{
   if ( t_triggerResults_ == NULL ) return -1.;
   return triggerResults_[trig];
}


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

double Analysis::luminosity()
{
	return (nevents_ / this -> crossSection() );
}

double Analysis::luminosity(const std::string & xs)
{
	if ( t_xsection_ == NULL ) return -1.;
	return (nevents_ / this -> crossSection(xs));
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

void Analysis::processJsonFile(const std::string & fileName)
{
	std::string scriptName = "source $CMSSW_BASE/src/Analysis/Tools/interface/strip.sh ";
	std::system((scriptName + fileName).c_str());
	const std::string modifidedJsonFileName("temp");
    std::ifstream fileStream(modifidedJsonFileName, std::ifstream::in);
    if (!fileStream.good()) 
    {
    	std::cerr<<"Error in Analysis.cc! Cannot find file with name: "<< fileName <<"\n...break\n"<<std::endl;
        exit(12);
    }
    // Loop over all lines in ccFile
    int checker = 0;
    while(fileStream.good())
    {
    	// Read input File
    	std::string line;
    	std::getline(fileStream, line);
    	// Loop over words in cc-File line and fill vWord
    	std::vector<std::string> vWord;
    	std::string word;
    	for (std::stringstream ss(line); ss >> word; )
    	{
    	    vWord.push_back(word);
    	}
            goodLumi_[checker] = vWord;
            checker ++;
    }
}

bool Analysis::selectJson()
{
	bool lumi = false;
    for( std::map<int, std::vector<std::string> >::iterator it = goodLumi_.begin(); it != --goodLumi_.end(); ++it)
    {
      if(std::stoi(it->second.at(0)) == run_)
      {
      	for(size_t lumiIt = 1; lumiIt < it->second.size()-1;  lumiIt = lumiIt + 2)
      	{
      		int lower = std::stoi(it->second.at(lumiIt));
      		int bigger = std::stoi(it->second.at(lumiIt+1));
      		if(lumi_ >= lower && lumi_ <= bigger ) lumi = true;
      	}
      }
      else continue;
    }
    return lumi;
}

// Way to get the Trigger names independent of Run period
/*
void triggerNames(std::string &trueTriggerNames,const char *myTriggerNames, TTree * t_Trig)
{
	TObjArray *mycopy = (TObjArray *)t_Trig->GetListOfBranches()->Clone();
	TString names;
	
	for (int i = 0; i < mycopy -> GetEntries(); ++i)
	{
		names = mycopy->At(i)->GetName();
		if( names.Contains(myTriggerNames) ) trueTriggerNames = (std::string)mycopy->At(i)->GetName();
		std::cout<<"name = "<<names<<std::endl;
	}
	
}
*/

 
