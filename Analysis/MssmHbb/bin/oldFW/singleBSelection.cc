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
   std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/v2/Pythia8_QCD/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8.txt";

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

   //Create Object to calculate the weights:
   Weights weightCalc(analysis.getLowMSelection());
   std::map<std::string, double> weight;


   // Add PileUp reweighting

//   std::map<std::string, TFile*> fPileUpData;
//   std::map<std::string, TH1F* > hPileUpData;
//   fPileUpData["central"] = new TFile("input_corrections/Data_Pileup_2015D_Nov17.root","read");
//   hPileUpData["central"] = (TH1F*) fPileUpData["central"]->Get("pileup");
//   fPileUpData["down"] = new TFile("input_corrections/PileUpData_down_26_02_2016.root","read");
//   hPileUpData["down"] = (TH1F*) fPileUpData["down"]->Get("pileup");
//   fPileUpData["up"] = new TFile("input_corrections/PileUpData_up_26_02_2016.root","read");
//   hPileUpData["up"] = (TH1F*) fPileUpData["up"]->Get("pileup");
//   TFile *fPileUpMC = new TFile("input_corrections/PileUpMC_26_02_2016.root","read");
//   TH1F *hPileUpMC = (TH1F*) fPileUpMC->Get("pileup");


   //Add BTagCalibration calculators needed for Offline BTag SF:
   BTagCalibration calib("csvv2", "input_corrections/SFbLib.csv");
   BTagCalibrationReader reader(&calib,               // calibration instance
                                BTagEntry::OP_TIGHT,  // operating point
                                "mujets",               // measurement type
                                "central");           // systematics type
   BTagCalibrationReader reader_up(&calib, BTagEntry::OP_TIGHT, "mujets", "up");  // sys up
   BTagCalibrationReader reader_down(&calib, BTagEntry::OP_TIGHT, "mujets", "down");  // sys down

   //Setup output file name
   //name can me specified explicitly with method: createOutputFile(fileName);
   std::string fileName = "/nfs/dust/cms/user/shevchen/output/SingleBTagSelection_76X";
   analysis.SetupStandardOutputFile(fileName);

   //Setup Branches
   analysis.setBranches();

   int counter = 0;
   bool goodLeadingJets = true;
   // Analysis of events

   //Add cMVA variable
   double LeadBTagMVA_[20];
   analysis.getOutputTree()->Branch("LeadBTagMVA",LeadBTagMVA_,"LeadBTagMVA[20]/D");

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

      if (offlineJets -> size() < 1) continue;

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
		LeadBTagMVA_[counter-1] = jet.btag("btag_csvmva");

		//Selection cuts for first two leading jets
		if(counter == 1){
			if(jet.pt() < 30) break;
			if(abs(jet.eta()) > 2.2) break;
//			if(jet.btag() < analysis.BTag1Cut()) break;
			if(analysis.isMC()) analysis.calculateBTagSF(reader,reader_up,reader_down);

			goodLeadingJets = true;

		}
      }
      if(!goodLeadingJets) continue;

      // Method that calculates dEta, dPhi between frist two leading jets
      // Also all parameters of di-jet object is calculated
      analysis.calculateJetVariables();

      if(analysis.isMC()){

    	  weight["Lumi"]     = weightCalc.LumiWeight(2182.680439,analysis.luminosity());
    	  analysis.setLumiWeight(weight["Lumi"]);

      }


      // Fill the output Tree
      analysis.fillTree();


   }
   std::cout<<"Number of Candidates: "<<analysis.getNumberOfCandidates()<<std::endl;

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
