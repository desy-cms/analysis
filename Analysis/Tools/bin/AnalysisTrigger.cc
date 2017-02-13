#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);
   
   // Physics Objects Collections
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   std::string hltPath1 = "HLT_DoubleJetsC100_SingleBTagCSV_p014_v";
   std::string hltPath2 = "HLT_DoubleJetsC100_SingleBTagCSV_p014_SinglePFJetC350_v";
   
   // Trigger objects
   std::vector<std::string> jetTriggerObjects;
   jetTriggerObjects.push_back("hltL1sDoubleJetC100");
   jetTriggerObjects.push_back("hltDoubleJetsC100");
   jetTriggerObjects.push_back("hltSingleBTagCSV0p84");
   jetTriggerObjects.push_back("hltJetC350");
   
   std::string trgobj_path = "MssmHbb/Events/selectedPatTrigger/";
   for ( auto & obj : jetTriggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i;
      std::cout << std::endl;
      
      // Trigger results: fired and prescales
      // hltPath1
      int trg1_fired = analysis.triggerResult(hltPath1);
      int trg1_l1ps  = analysis.triggerL1Prescale(hltPath1);
      int trg1_hltps = analysis.triggerHLTPrescale(hltPath1);
      std::string s_accept1 = " fired ";
      if ( ! trg1_fired ) s_accept1 = " did not fire ";
      std::cout << "The path " << hltPath1 << s_accept1 << " and has L1 PS = " << trg1_l1ps << " and HLT PS = " << trg1_hltps << std::endl; 
      
      // hltPath2
      int trg2_fired = analysis.triggerResult(hltPath2);
      int trg2_l1ps  = analysis.triggerL1Prescale(hltPath2);
      int trg2_hltps = analysis.triggerHLTPrescale(hltPath2);
      std::string s_accept2 = " fired ";
      if ( ! trg2_fired ) s_accept2 = " did not fire ";
      std::cout << "The path " << hltPath2 << s_accept2 << " and has L1 PS = " << trg2_l1ps << " and HLT PS = " << trg2_hltps << std::endl; 
   }
   
//    
}

