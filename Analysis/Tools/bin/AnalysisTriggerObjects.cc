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
   Analysis analysis(inputList,"MssmHbbTrigger/Events/EventInfo");
   
   // Physics Objects Collections
   analysis.triggerResults("MssmHbbTrigger/Events/TriggerResults");
   std::string hltPath[20];
   hltPath[0] = "HLT_L1SingleMu3_v1";
   hltPath[1] = "HLT_L1SingleJet20_v1";
   
   // Trigger objects
   std::vector<std::string> jetTriggerObjects;
   jetTriggerObjects.push_back("hltL1sSingleMu3");
   jetTriggerObjects.push_back("hltL1sSingleJet20");
   
   std::string trgobj_path = "MssmHbbTrigger/Events/hltTriggerSummaryAOD/";
   for ( auto & obj : jetTriggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Get generator filter information
   FilterResults genFilter = analysis.generatorFilter("MssmHbbTrigger/Metadata/GeneratorFilter");  
   
   // Get cross section
   analysis.crossSections("MssmHbbTrigger/Metadata/CrossSections");
   
   double crossSection = analysis.crossSection();
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   
   int nSelected = 0;
   int nTotal = genFilter.filtered;
   float genFilterEfficiency = genFilter.efficiency;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      if ( analysis.nPileup() != 45 ) continue;
      
      // Trigger results: fired and prescales
      // hltPath1
      int trg_fired = analysis.triggerResult(hltPath[0]) && analysis.triggerResult(hltPath[1]);
      
      if ( ! trg_fired ) continue;
      
      // muon selection at L1
      auto l1Mu3s = analysis.collection<TriggerObject>("hltL1sSingleMu3");
      
      std::vector<TriggerObject> selL1Mu10s;
      for ( int m = 0 ; m < l1Mu3s->size() ; ++m )
      {
         TriggerObject l1Mu3 = l1Mu3s->at(m);
         if ( l1Mu3.pt() >= 10 && fabs(l1Mu3.eta()) <= 2.2 ) 
         {
            selL1Mu10s.push_back(l1Mu3);
//         std::cout << l1Mu3.pt() << std::endl;
         }
         
      }
      if ( selL1Mu10s.size() < 1 ) continue;
      
      // jet selection at L1
      auto l1Jet20s = analysis.collection<TriggerObject>("hltL1sSingleJet20");
      std::vector<TriggerObject> selL1Jet30s;
      for ( int j = 0 ; j < l1Jet20s->size() ; ++j )
      {
         TriggerObject l1Jet20 = l1Jet20s->at(j);
         if ( l1Jet20.pt() >= 30 && fabs(l1Jet20.eta()) <= 2.2 ) 
         {
            selL1Jet30s.push_back(l1Jet20);
         }
      }
      std::cout << selL1Jet30s.size() <<std::endl;
      
      if ( selL1Jet30s.size() < 2 ) continue;
      
      
      ++nSelected;
      
   }
   
   std::cout << "Number of selected events : " << nSelected << std::endl;
   std::cout << "Efficiency for L1_SingleMu10Eta2p2 with PU = 45 : " << double(nSelected)/nTotal << std::endl;
   
//    
}

