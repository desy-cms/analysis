#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "RooMsgService.h"

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
#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"


using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;


// =============================================================================================
int main(int argc, char * argv[])
{
	TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

	std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/BTagCSVData/Run2015C_25ns-16Dec2015-v1.txt";

	JetAnalysisBase analysis(inputList);
	analysis.setupAnalysis("Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt");
	analysis.applySelection();


	/*
	//Initialisation of TriggerEfficiency class
	TriggerEfficiency analysis(argc,argv);

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

   //Setup output file name
   //name can me specified explicitly with method: createOutputFile(fileName);
   std::string fileName = "/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_BTag0p7";
   analysis.SetupStandardOutputFile(fileName);
   //Setup Branches
   analysis.setBranches();

//   int counter = 0;
//   bool goodLeadingJets = true;
   // Analysis of events


   std::cout<<"Number of Entries: "<<analysis.size()<<std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {

   }
   std::cout<<"Number of Candidates: "<<analysis.getNumberOfCandidates()<<std::endl;

 /*
 */

	return 0;
}


/*
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-PromptReco-v4.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015C_25ns-05Oct2015-v1.root
/nfs/dust/cms/user/shevchen/samples/miniaod/BTagCSVData/Run2015D-05Oct2015-v1.root
*/
