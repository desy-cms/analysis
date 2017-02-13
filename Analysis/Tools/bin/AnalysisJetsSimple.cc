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
   double ptmin = 20.;
   
   bool isMC = false;
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);
   
   // Physics Objects Collections
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // Trigger results
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   std::string hltPath = "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v";
   
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sDoubleJetC100");
   triggerObjects.push_back("hltDoubleJetsC100");
   triggerObjects.push_back("hltBTagCaloCSVp014DoubleWithMatching");
   triggerObjects.push_back("hltDoublePFJetsC100");
   triggerObjects.push_back("hltDoublePFJetsC100MaxDeta1p6");
   
   std::string trgobj_path = "MssmHbb/Events/selectedPatTrigger/";
   for ( auto & obj : triggerObjects )
      analysis.addTree<TriggerObject>(obj,trgobj_path+obj);
   
   // Certified lumis
   if ( ! isMC ) analysis.processJsonFile("json.txt");
   

   TFile hout("histograms_rereco_ptmin20.root","recreate");
   
   std::map<std::string, TH1F*> h1;
   h1["n"]        = new TH1F("n" , "" , 30, 0, 30);
   h1["pt"]       = new TH1F("pt" , "" , 500, 0, 100);
   h1["eta"]      = new TH1F("eta" , "" , 100, -5, 5);
   h1["phi"]      = new TH1F("phi" , "" , 100, -4, 4);
   
   h1["nHadFrac"] = new TH1F("nHadFrac" , "" , 100, 0, 1);
   h1["nEmFrac"]  = new TH1F("nEmFrac"  , "" , 100, 0, 1);
   h1["nMult"]    = new TH1F("nMult"    , "" , 40, 0, 40);
   h1["cHadFrac"] = new TH1F("cHadFrac" , "" , 100, 0, 1);
   h1["cEmFrac"]  = new TH1F("cEmFrac"  , "" , 100, 0, 1);
   h1["cMult"]    = new TH1F("cMult"    , "" , 40, 0, 40);
   h1["muFrac"]   = new TH1F("muFrac"   , "" , 100, 0, 1);
   h1["nConst"]   = new TH1F("nConst"   , "" , 40, 0, 40);
         
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;
   int nevents =  analysis.size();
//   nevents = 10000;
//   std::string prevFile = "";

   int nFired = 0;
   for ( int i = 0 ; i < nevents ; ++i )
   {
      if ( i > 0 && i%100000 == 0 ) std::cout << i << " events processed..." << std::endl;
      
      analysis.event(i);
      
      if ( !isMC )
      {
//          if ( analysis.run() == 275782)
//          {
//             if ( prevFile == "" || prevFile != analysis.fileName() )
//             {
//                prevFile = analysis.fileName();
//                std::cout << prevFile << std::endl;
//             }
//          }
         if ( !analysis.selectJson() ) continue;
      }

      // Trigger results
      // hltPath
      int trgFired = analysis.triggerResult(hltPath);
      if ( ! trgFired ) continue;
      ++nFired;
            
//       std::cout << "++++++    ENTRY  " << i;
//       std::cout << std::endl;
      
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      int njets = 0;
      for ( int j = 0 ; j < jets->size() ; ++j )
      {
         Jet jet = jets->at(j);
         if ( ! jet.idLoose() )  continue;
         if ( jet.pt() < ptmin ) continue;
         ++njets;
         h1["pt"]        -> Fill(jet.pt());
         h1["eta"]       -> Fill(jet.eta());
         h1["phi"]       -> Fill(jet.phi());
         h1["nHadFrac"]  -> Fill(jet.neutralHadronFraction());
         h1["nEmFrac"]   -> Fill(jet.neutralEmFraction()    );
         h1["nMult"]     -> Fill(jet.neutralMultiplicity()  );
         h1["cHadFrac"]  -> Fill(jet.chargedHadronFraction());
         h1["cEmFrac"]   -> Fill(jet.chargedEmFraction()    );
         h1["cMult"]     -> Fill(jet.chargedMultiplicity()  );
         h1["muFrac"]    -> Fill(jet.muonFraction()         );
         h1["nConst"]    -> Fill(jet.constituents()         );
         
      }
      h1["n"] -> Fill(njets);
      
   }
   
   for (auto & ih1 : h1)
   {
      ih1.second -> Write();
   }
   
   std::cout << "Number of triggered events = " << nFired << std::endl;
   
//    
}

