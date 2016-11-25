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
#include "TKey.h"
#include "Analysis/Tools/interface/Analysis.h"
#include <boost/algorithm/string.hpp>

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
   
   std::vector<std::string> branches;
   TObjArray * treeBranches = t_event_->GetListOfBranches();
   for ( int i = 0 ; i < treeBranches->GetEntries() ; ++i )
      branches.push_back(treeBranches->At(i)->GetName());
   
   t_event_ -> SetBranchAddress("event", &event_);
   t_event_ -> SetBranchAddress("run", &run_);
   t_event_ -> SetBranchAddress("lumisection", &lumi_);
   
   // For backward compatibility
   std::vector<std::string>::iterator it;
   it = std::find(branches.begin(),branches.end(),"nPileup");      if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &n_pu_);
   it = std::find(branches.begin(),branches.end(),"nTruePileup");  if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &n_true_pu_);
   
   it = std::find(branches.begin(),branches.end(),"genWeight");    if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &genWeight_);
   it = std::find(branches.begin(),branches.end(),"genScale");     if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &genScale_);
   it = std::find(branches.begin(),branches.end(),"pdfid1");       if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &pdf_.id.first);
   it = std::find(branches.begin(),branches.end(),"pdfid2");       if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &pdf_.id.second);
   it = std::find(branches.begin(),branches.end(),"pdfx1");        if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &pdf_.x.first);
   it = std::find(branches.begin(),branches.end(),"pdfx2");        if ( it != branches.end() ) t_event_  -> SetBranchAddress( (*it).c_str(), &pdf_.x.second);
   
   
//   t_event_ -> SetBranchAddress("nPileup", &n_pu_);
//   t_event_ -> SetBranchAddress("nTruePileup", &n_true_pu_);

   nevents_ = t_event_ -> GetEntries();

   t_event_ -> GetEntry(1); // Check whether it's mc simulation
   if (run_ == 1) {         // A bit stupid, but it's only solution that I found for the moment
     is_mc_ = true;
   } else {
     is_mc_ = false;
   }
   
   btageff_algo_    = "";
   btageff_flavour_ = "";
   
   mylumi_= -1.;

   
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
   // Initialisation for backward compatibility
   n_pu_ = -1;
   n_true_pu_ = -1;
   
   genWeight_ = -1.;
   genScale_  = -1.;
   pdf_.id.first  = 0;
   pdf_.id.second = 0;
   pdf_.x.first  = -1.;
   pdf_.x.second = -1.;
   
   t_event_ -> GetEntry(event);
   if ( !addCollections) return;
   
   for ( auto & tree : t_any_ )
   {
      std::string name = tree.first;
      std::string type = t_type_[name];
      if ( type == "Jet" )            this->addCollection<Jet>(name);
      if ( type == "Muon" )           this->addCollection<Muon>(name);
      if ( type == "GenJet" )         this->addCollection<GenJet>(name);
      if ( type == "MET" )            this->addCollection<MET>(name);
      if ( type == "Vertex" )         this->addCollection<Vertex>(name);
      if ( type == "TriggerObject" )  this->addCollection<TriggerObject>(name);
      if ( type == "GenParticle" )    this->addCollection<GenParticle>(name);
      if ( type == "Candidate" )      this->addCollection<Candidate>(name);
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
      if ( TString(branch).BeginsWith("ps") )
      {
         triggerResultsPS_[branch] = 1;
         t_triggerResults_ -> SetBranchAddress(branch.c_str(), &triggerResultsPS_[branch]);
      }
      else
      {
         triggerResults_[branch] = 0;
         t_triggerResults_ -> SetBranchAddress(branch.c_str(), &triggerResults_[branch]);
      }
   }
}

int Analysis::triggerResult(const std::string & trig)
{
   if ( t_triggerResults_ == NULL ) return -1.;
   return triggerResults_[trig];
}

int Analysis::triggerL1Prescale(const std::string & trig)
{
//   if ( t_triggerResults_ == NULL ) return -1.;
   return triggerResultsPS_["psl1_"+trig];
}
int Analysis::triggerHLTPrescale(const std::string & trig)
{
//   if ( t_triggerResults_ == NULL ) return -1.;
   return triggerResultsPS_["pshlt_"+trig];
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

float Analysis::scaleLuminosity(const float & lumi)
{
   float lumiScale = 1.;
   mylumi_ = lumi;
   if ( mylumi_ < 0 ) return lumiScale;
   
   lumiScale  = mylumi_/this->luminosity();
   return lumiScale;
   
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

FilterResults Analysis::eventFilter(const std::string & path)
{
   t_evtfilter_  = new TChain(path.c_str());
   t_evtfilter_ -> AddFileInfoList(fileList_);

   unsigned int ntotal;
   unsigned int nfiltered;
   unsigned int sumtotal = 0;
   unsigned int sumfiltered = 0;

   t_evtfilter_ -> SetBranchAddress("nEventsTotal", &ntotal);
   t_evtfilter_ -> SetBranchAddress("nEventsFiltered", &nfiltered);

   for ( int i = 0; i < t_evtfilter_->GetEntries(); ++i )
   {
      t_evtfilter_ -> GetEntry(i);
      sumtotal += ntotal;
      sumfiltered += nfiltered;
   }


   evtfilter_.total = sumtotal;
   evtfilter_.filtered = sumfiltered;
   evtfilter_.efficiency = float(sumfiltered)/sumtotal;

   return evtfilter_;
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

void Analysis::addBtagEfficiencies(const std::string & filename)
{
   fileBtagEff_ = new TFile(filename.c_str(),"OLD");
   std::string ftitle = fileBtagEff_->GetTitle();
   std::vector<std::string> field;
   if ( ftitle != "" )
   {
      boost::split(field, ftitle, boost::is_any_of(":"));
      btageff_flavour_ = field[0];
      btageff_algo_    = field[1];
   }
   
   TList * mylist = fileBtagEff_->GetListOfKeys();
   for ( int i = 0 ; i < mylist->GetSize() ; ++i  )
   {
      std::string className = ((TKey*) mylist -> At(i)) -> GetClassName();
      std::string objName   = ((TKey*) mylist -> At(i)) -> GetName();
      if ( className == "TH2F" )
         h2_btageff_[objName] = (TH2F*) fileBtagEff_->Get(objName.c_str());
//      if ( className == "TH2D" )
//         h2_btageff_[objName] = (TH2D*) fileBtagEff_->Get(objName.c_str());
   }
   
}

float Analysis::btagEfficiency(const analysis::tools::Jet & jet, const int & rank)
{
   float eff = 0.;
   std::string flav;
   std::string srank;
   if ( rank < 1 ) srank = "";
   else            srank = Form("%i",rank);
   if ( btageff_flavour_ == "Extended" || btageff_flavour_ == "extended" )
   {
      flav = jet.extendedFlavour();
      if ( flav == "udsg" ) flav = "l";
   }
   else
   {
      int iflav = jet.flavour(btageff_flavour_);
      if ( abs(iflav) == 5 )                flav = "b";
      if ( abs(iflav) == 4 )                flav = "c";
      if ( abs(iflav) < 4 || iflav == 21 )  flav = "l";
   }
   float pt = jet.pt();
   float eta = fabs(jet.eta());
   
   std::string hname = Form("h_%sjet%s_eff_pt_eta",flav.c_str(),srank.c_str());

   int bin = h2_btageff_[hname] -> FindBin(pt,eta);
   
   eff = h2_btageff_[hname] -> GetBinContent(bin);
         
   return eff;
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

 
