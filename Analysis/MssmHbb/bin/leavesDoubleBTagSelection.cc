#include <string>
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

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;

double BTagWeight(TH2F*,TH2F*,TH2F*,const double &, const double &);

// =============================================================================================
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   //For btag efficiency:

   //Online BTag Trigger Efficiency produced by Ye Chen
   TFile * fileBTagEff = new TFile("~/Downloads/efficiency_online_btagcsv0p9.root");
   TH2F *btagEff0p9 = (TH2F*) fileBTagEff ->Get("heh4");	 // eta <0.9
   TH2F *btagEff0p9_1p4 = (TH2F*) fileBTagEff ->Get("heh3"); // 1.4 > eta >0.9
   TH2F *btagEff1p4_2p5 = (TH2F*) fileBTagEff ->Get("heh2"); // 2.5 > eta > 1.4

   //Online Pt trigger efficiency:
   TFile * filePtEff = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/macros/triggerEfficiency/latest/TwoDimEff.root");
   TH2F *PtEff = (TH2F*) filePtEff ->Get("TwoDEffRefMC_Num"); // 2D

   // Input files list
   //std::string inputList = "rootFileListBTagCSV.txt";
   std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/HT_QCD/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8.txt";
   MssmHbb analysis(inputList);

   // Process selected JSON file
   if(!analysis.isMC()) analysis.processJsonFile("goodJson.txt");

   //Setup output file name
   //name can me specified explicitly with method: createOutputFile(fileName);
   std::string fileName = "/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionHT300500";
   analysis.SetupStandardOutputFile(fileName);

   // Add std::vector<std::string> of the Trigger Objects that you would like to apply.
   // Also Trigger Results name will be stored, according to the trigger objects names
   // By default - LowM MssmHbb trigger objects is used
   if(!analysis.isMC()) analysis.addTriggerObjects();

   //Add BTagCalibration calculators needed for Offline BTag SF:
   BTagCalibration calib("csvv2", "SFbLib.csv");
   BTagCalibrationReader reader(&calib,               // calibration instance
                                BTagEntry::OP_TIGHT,  // operating point
                                "mujets",               // measurement type
                                "central");           // systematics type
   BTagCalibrationReader reader_up(&calib, BTagEntry::OP_TIGHT, "mujets", "up");  // sys up
   BTagCalibrationReader reader_down(&calib, BTagEntry::OP_TIGHT, "mujets", "down");  // sys down

   int counter = 0;
   bool goodLeadingJets = true;

   // Analysis of events

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

		//Selection cuts for first two leading jets
		if(counter == 1 || counter == 2){
			if(jet.pt() < 100) break;
			if(abs(jet.eta()) > 2.2) break;
			if(jet.btag() < 0.941) break;
			if(analysis.isMC()) analysis.calculateBTagSF(reader,reader_up,reader_down);

			if(counter == 2){
				if(LeadJet[0].deltaR(LeadJet[1]) <= 1) break;
				if(abs(LeadJet[0].eta() - LeadJet[1].eta()) > 1.6) break;
				if(!analysis.isMC() && !analysis.OnlineSelection(LeadJet[0],LeadJet[1])) break;
				goodLeadingJets = true;
			}
		}
      }
      if(!goodLeadingJets) continue;

      //Online BTag Efficiency were calculated only up to 1 TeV
      if(LeadJet[0].pt() > 1000) continue;

      // Method that calculates dEta, dPhi between frist t2o leading jets
      // Also all parameters of di-jet object is calculated
      analysis.calculateJetVariables();

      if(analysis.isMC()){
    	  analysis.setBTagWeight(BTagWeight(btagEff0p9,btagEff0p9_1p4,btagEff1p4_2p5,LeadJet[0].pt(),LeadJet[0].eta())*
    		  	  	  	  	 	 BTagWeight(btagEff0p9,btagEff0p9_1p4,btagEff1p4_2p5,LeadJet[1].pt(),LeadJet[1].eta()));
    	  analysis.setLumiWeight(2182.680439,analysis.luminosity());

    	  //Calculate other weights
    	  analysis.calculateWeights(btagEff0p9_1p4,PtEff);

    	  //Calculation of the flavour composition, based on HadronFlavours
    	  analysis.calculateFlavourComposition();
      }

      analysis.fillTree();


   }
   std::cout<<"Number of Candidates: "<<analysis.getNumberOfCandidates()<<std::endl;
   analysis.writeTree();

   std::cout<<" Output File: "<<analysis.getOutPutFileName()<<" was stored!"<<std::endl;

   return 0;
//
}

double BTagWeight(TH2F*region1,TH2F* region2,TH2F* region3, const double &pt, const double &eta){
	if(pt <1000) {
		if(eta < 0.9) return region1 -> Interpolate(pt);
		else if (eta > 0.9 &&eta < 1.4) return region2 -> Interpolate(pt);
		else return region3 -> Interpolate(pt);

	}
	else return 1;
}

/*
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-PromptReco-v4.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015C_25ns-05Oct2015-v1.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-05Oct2015-v1.root
*/
