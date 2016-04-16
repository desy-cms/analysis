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

// =============================================================================================
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

   // Input files list
   //std::string inputList = "rootFileListBTagCSV.txt";
   std::string inputList = "/nfs/dust/cms/user/shevchen/samples/miniaod/76X/MSSMHbb/SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.txt";
   // Initialisation of MssmHbb class
   MssmHbb analysis(inputList);

   // Add std::vector<std::string> of the Trigger Objects that you would like to apply.
   // Also Trigger Results name will be stored, according to the trigger objects names
   // By default - LowM MssmHbb trigger objects is used
   std::vector<std::string> triggerObjects = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
   if(!analysis.isMC()) analysis.addTriggerObjects();

   //Select which sets of cuts should be applied:for Low mass selection = true; for High M = false.
   if(analysis.isMC()) analysis.setLowMSelection(true);

   //Setup output file name
   //name can me specified explicitly with method: createOutputFile(fileName);
   std::string fileName = "/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_GenLvLStudy";
   analysis.SetupStandardOutputFile(fileName);

   //gen jets:
   analysis.addTree<Jet>("GenJets","MssmHbb/Events/slimmedGenJets");

   //gen particles
   analysis.addTree<GenParticle>("GenParticles","MssmHbb/Events/prunedGenParticles");

   std::map<std::string,TH1D*> h;

   h["LeadJet_pt"]			= new TH1D("LeadJet_pt","",100,0.,1500.);
   h["LeadJet_eta"]			= new TH1D("LeadJet_eta","",100,-3.,3.);
   h["AllJet_pt"]			= new TH1D("AllJet_pt","",100,0.,1500);
   h["AllJet_eta"]			= new TH1D("AllJet_eta","",100,-3.,3);

   h["Fraction_pt"]			= new TH1D("Fraction_pt","",100,0.,1500);
   h["Fraction_eta"]		= new TH1D("Fraction_eta","",100,-3.,3.);

//   int counter = 0;
//   bool goodLeadingJets = true;
   // Analysis of events

   std::cout<<"Number of Entries: "<<analysis.size()<<std::endl;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {

      analysis.event(i);

      //Trigger Selection
      if( !analysis.isMC() && !analysis.getTriggerResult()) continue;

      //Define Jet Collection
      auto offlineJets = analysis.collection<Jet>("Jets");

      //Define Gen Jet collection
      auto genJets = analysis.collection<Jet>("GenJets"	);

      //Define Gen Particles collection
      auto genParticles = analysis.collection<GenParticle>("GenParticles");

//      if (offlineJets -> size() < 3) continue;

      //Match offline Jets to online Objects
      if (!analysis.isMC()) analysis.match<Jet,TriggerObject>("Jets",analysis.getTriggerObjectNames());

      std::vector<Jet> GenHBBJets;
      //First associate gen jets with initiaal partons (higgs daughters)
	  for(int iGenJet = 0; iGenJet < genJets->size(); ++iGenJet){
		  Jet genJ = genJets->at(iGenJet);
		  genJ.associatePartons(genParticles);
		  for(int iGenPart = 0; iGenPart < genJ.partons().size(); iGenPart++){
			  GenParticle p = genParticles->at(iGenPart);
			  if(p.higgsDaughter() && std::abs(p.pdgId()) == 5){
				  GenHBBJets.push_back(genJ);
			  }
		  }
	  }



      /*
      for(int iGenPart = 0; iGenPart < genParticles->size();++iGenPart){
    	  GenParticle p = genParticles->at(iGenPart);

    	  if(p.higgsDaughter() && (p.pdgId() == 5 || p.pdgId() == -5) ){
    		  //Find associated jet
    		  for(int iGenJet = 0; iGenJet < genJets->size(); ++iGenJet){
    			  Jet genJ = genJets->at(iGenJet);
    			  genJ.associatePartons(genParticles);
    		  }
    	  }
      }
      */

//      if(offlineJets->size() < 2 )



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
