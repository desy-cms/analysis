#include <string>
#include <iostream>
#include <vector>

#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Jets.h"
#include "Analysis/Tools/interface/Muons.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


void PrintCrossSections(const TFileCollection &);

// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt"; 
   TFileCollection fc("dum","",inputList.c_str());
   
   // Main event tree
   TChain * t_Event   = new TChain("MonteCarloStudies/Events/EventInfo");
   t_Event->AddFileInfoList((TCollection*) fc.GetList());
   
   // Other event trees
   TChain * t_Jets    = new TChain("MonteCarloStudies/Events/slimmedJetsPuppi");
   TChain * t_Muons   = new TChain("MonteCarloStudies/Events/slimmedMuons");
   t_Jets   -> AddFileInfoList((TCollection*) fc.GetList());
   t_Muons  -> AddFileInfoList((TCollection*) fc.GetList());
   
   // Friendship (DON'T FORGET!!!)
   t_Event -> AddFriend(t_Jets);
   t_Event -> AddFriend(t_Muons);
   
   // Create objects collections
   Jets  jets (t_Jets);
   Muons muons(t_Muons);
   
   // Number of loop events
   int nEvents = t_Event->GetEntries();
   
   int eventCounter = 0;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
//      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      ++eventCounter;
      
      t_Event->GetEntry(i);
      
      if ( jets.size() > 0 )
      {
         Jet jet = jets.at(0);
         std::cout << jet.pt() << "  " << jet.btag() << std::endl;
      }
      
      if ( muons.size() > 0 )
      {
         Muon muon = muons.at(0);
         std::cout << "Muon pt = " << muon.pt() << std::endl;
      }
   }
      
//    
}

void PrintCrossSections(const TFileCollection & fc)
{
//       // Cross Sections - print on the screen
//    std::cout << "=======================================================" << std::endl;
//    std::cout << "  CROSS SECTIONS" << std::endl;
//    std::cout << "=======================================================" << std::endl;
//    
//    TChain * t_xsection  = new TChain("MonteCarloStudies/Metadata/CrossSections");
//    t_xsection -> AddFileInfoList((TCollection*) fc.GetList());
//    TObjArray * xsecBranches = t_xsection->GetListOfBranches();
//    std::map<std::string, double> xsections;
//    for ( int i = 0 ; i < xsecBranches->GetEntries() ; ++i )
//    {
//       std::string branch = xsecBranches->At(i)->GetName();
//       if ( branch == "run" ) continue;
//       xsections[branch] = 0;
//       t_xsection -> SetBranchAddress(branch.c_str(), &xsections[branch]);
//    }
//    t_xsection -> GetEntry(0);
//    for ( auto& xs : xsections )
//    {
//       std::cout << xs.first << " = " << xs.second << " pb " << std::endl;
//    }

}
