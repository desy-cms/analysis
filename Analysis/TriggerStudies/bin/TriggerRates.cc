#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <bitset>
#include <memory>

#include "TFile.h" 
#include "TH1.h" 
#include "TH2.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"
#include "TRFIOFile.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"
#include "TFileCollection.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TF1.h"

#include "NtupleVariables.h"

using namespace std;

float  pi;

int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   
   // Histograms
   
   // TTree chains and friendship?
   TChain * chainEvent    = new TChain("TriggerStudies/EventInfo");
   TChain * chainTrgResults  = new TChain("TriggerStudies/TriggerResults_HLT3PB");
   
   // Input files
   std::string inputList = "rootFileList.txt";
   cout << "List of input files: " << inputList << endl;
   TFileCollection fc("dum","",inputList.c_str());
   
   // Friendship
   chainEvent -> AddFriend(chainTrgResults);
   
   // Read from file
   chainEvent   -> AddFileInfoList((TCollection*) fc.GetList());
   chainTrgResults -> AddFileInfoList((TCollection*) fc.GetList());
   
   // Branches
   // Event Info
   chainEvent->SetBranchAddress( "run"         , &run_ );
   chainEvent->SetBranchAddress( "lumisection" , &lumi_ );
   chainEvent->SetBranchAddress( "event"       , &event_ );
   // Trigger Results
   TObjArray * trgBranches = chainTrgResults->GetListOfBranches();
   int nPaths = trgBranches -> GetEntries();
//   trgAccepts_ = new bool [nPaths];
//   trgAccepts_ = std::unique_ptr<bool[]>(new bool [nPaths]);
   std::map<std::string, int> trgCounter;
   
   for ( int i = 0 ; i < nPaths ; ++i )
   {
      std::string path = trgBranches->At(i)->GetName();
      chainTrgResults -> SetBranchAddress(path.c_str(), &trgAccepts_[path]);
      trgCounter[path] = 0;
   }
   
//    std::map<std::string, bool>::iterator it = trgAccepts_.begin();
//    for (it=trgAccepts_.begin(); it!=trgAccepts_.end(); ++it)
//    {
//       std::cout << it->first << " => " << it->second << '\n';
//    }
   
   
//    
   // Number of loop events
   int nentries = chainEvent->GetEntries();
   std::cout << nentries << std::endl;
   for ( int i = 0 ; i < nentries ; ++i )
   {
      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      
      chainEvent->GetEntry(i);
      
      for(it_trg it = trgAccepts_.begin(); it != trgAccepts_.end(); it++)
      {
         if ( it->second )
            ++trgCounter[it->first];
      }
   }
   
   for(it_cnt it = trgCounter.begin(); it != trgCounter.end(); it++)
   {
      std::cout << it->first << "  ->   " << it->second << std::endl;
   }
   
    
//    
}


