#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "stdlib.h"

#include "Analysis/MssmHbb/interface/MssmHbb.h"
#include "Analysis/MssmHbb/interface/json.h"
#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"
#include "Analysis/MssmHbb/interface/Weights.h"

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;

double BTagWeight(TH2F*,TH2F*,TH2F*,const double &, const double &);

// =============================================================================================
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

   // Input files list
   //std::string inputList = "rootFileListBTagCSV.txt";
   std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/BTagCSVData/Run2015C_25ns-16Dec2015-v1.txt";

   // Initialisation of MssmHbb class
   MssmHbb analysis(inputList);

   // Process selected JSON file
   if(!analysis.isMC()) analysis.processJsonFile("Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt");

   // Add std::vector<std::string> of the Trigger Objects that you would like to apply.
   // Also Trigger Results name will be stored, according to the trigger objects names
   // By default - LowM MssmHbb trigger objects is used
   std::vector<std::string> triggerObjects = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
   if(!analysis.isMC()) analysis.addTriggerObjects();

   //Select which sets of cuts should be applied:for Low mass selection = true; for High M = false.
   if(analysis.isMC()) analysis.setLowMSelection(true);

   //Setup output file name
   //name can me specified explicitly with method: createOutputFile(fileName);
   std::string fileName = "/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_versionComparison_";
   analysis.SetupStandardOutputFile(fileName);

   //Setup Branches
   analysis.setBranches();

   int counter = 0;
   bool goodLeadingJets = true;
   // Analysis of events

   //Output Histograms:
   std::map<std::string,TH1F*> h;
   h["TriggerSelection"] 	= new TH1F("TriggerSelection","TriggerSelection",100,0.,1000);
   h["TS+Matching"] 		= new TH1F("TS+Matching","TriggerSelection",100,0.,1000);
   h["TS+M+Eta"] 			= new TH1F("TS+M+Eta","TriggerSelection",100,0.,1000);
   h["TS+M+Eta+Pt"] 		= new TH1F("TS+M+Eta+Pt","TriggerSelection",100,0.,1000);
   h["TS+M+Eta+Pt+Btag"] 	= new TH1F("TS+M+Eta+Pt+Btag","TriggerSelection",100,0.,1000);
   h["All"]					= new TH1F("All","TriggerSelection",100,0.,1000);


   std::cout<<"Number of Entries: "<<analysis.size()<<std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {

      analysis.event(i);

      //Select only good JSon runs
      if (!analysis.isMC() && !analysis.selectJson()) continue;

      //Trigger Selection
      if( !analysis.isMC() && !analysis.getTriggerResult()) continue;

      //Used to clean values of the variables for the new event
      analysis.cleanVariables();

      //Define Jet Collection
      auto offlineJets = analysis.collection<Jet>("Jets");
      //Define Vertex collection
      auto offlinePrimaryVertices = analysis.collection<Vertex>("Vertices");

      if (offlineJets -> size() < 2) continue;

      //Match offline Jets to online Objects
      if (!analysis.isMC()) analysis.match<Jet,TriggerObject>("Jets",analysis.getTriggerObjectNames());

      //Set Total Number of Jets
      analysis.setNjets(offlineJets->size());

      counter = 0;
      goodLeadingJets = false;
      Jet LeadJet[20];
      // Selection of double b-tag sample:
      for( int iJet = 0; iJet < offlineJets -> size(); ++iJet)
      {
      	Jet jet = offlineJets -> at(iJet);

      	//Calculate HT
      	analysis.addHt(jet.pt());


      	//Only jets that pass Loose identification will be considered
		if(!jet.idLoose()) continue;
		counter++;

		//Only first 5 jets is taken into account
		//To reduce size of the output file
		if(counter > 5) break;
		LeadJet[counter - 1] = jet;

		analysis.setJetCounter(counter-1);
		analysis.setJetVariables(jet);

      }

      if(analysis.getTriggerResult()) {
    	  h["TriggerSelection"]->Fill(LeadJet[0].pt());
    	  if(analysis.OnlineSelection(LeadJet[0],LeadJet[1])) {
    		  h["TS+Matching"]->Fill(LeadJet[0].pt());
    		  if(LeadJet[0].eta() < 2.2 && LeadJet[1].eta() < 2.2) {
    			  h["TS+M+Eta"]->Fill(LeadJet[0].pt());
    			  if(LeadJet[0].pt() > 100 && LeadJet[1].pt() > 100) {
    				  h["TS+M+Eta+Pt"]->Fill(LeadJet[0].pt());
    				  if(LeadJet[0].btag() > 0.941 && LeadJet[1].btag() > 0.941) h["TS+M+Eta+Pt+Btag"]->Fill(LeadJet[0].pt());
    			  }
    		  }
    	  }
      }



   }
   std::cout<<"Number of Candidates: "<<analysis.getNumberOfCandidates()<<std::endl;
   for(const auto &ih : h){
	   ih.second->Write();
   }

   analysis.writeTree();

   return 0;
//
 /*
 */
}

/*
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-PromptReco-v4.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015C_25ns-05Oct2015-v1.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-05Oct2015-v1.root
*/
