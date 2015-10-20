#include <string>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>

#include "TFile.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h"

#include "Analysis/MssmHbb/interface/MssmHbb.h"

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;


// =============================================================================================
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

   // Input files list MonteCarloStudies
   std::string inputList = "rootFileList.txt";
   MssmHbb analysis(inputList,"MssmHbb/Events/EventInfo");
   
   if(analysis.isMC())
   {
     // cross sections
     analysis.crossSections("MonteCarloStudies/Metadata/CrossSections");
     analysis.listCrossSections();

     //generator filter
     analysis.generatorFilter("MonteCarloStudies/Metadata/GeneratorFilter");
     analysis.listGeneratorFilter();
   }

   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsPuppi");
   analysis.addTree<TriggerObject> ("hltDoubleBTagCSV0p9","MssmHbb/Events/selectedPatTrigger/hltDoubleBTagCSV0p9");
   analysis.addTree<TriggerObject> ("hltL1sL1DoubleJetC100","MssmHbb/Events/selectedPatTrigger/hltL1sL1DoubleJetC100");
   analysis.addTree<TriggerObject> ("hltDoublePFJetsC100","MssmHbb/Events/selectedPatTrigger/hltDoublePFJetsC100");
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   std::string triggerLogicName = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v1";
   
   int nCand = 0;
   std::vector <int> leadJetsNumber;
   
   
   std::map < std::string, TH1F *> h1;
   h1["h_jet_N"]         = new TH1F("h_jet_N" , "", 20, 0., 20.);
	
	h1["h_jet1_Pt"]       = new TH1F("h_jet1_Pt" , "", 100, 0., 1000.);
	h1["h_jet1_Eta"]      = new TH1F("h_jet1_Eta", "", 100, -5, 5.);
	h1["h_jet1_Phi"]      = new TH1F("h_jet1_Phi", "", 100, -3.2, 3.2);
	h1["h_jet1_BTag"]     = new TH1F("h_jet1_BTag", "", 100, 0., 1.);
	
   	h1["h_jet2_Pt"]       = new TH1F("h_jet2_Pt" , "", 100, 0., 1000.);
   	h1["h_jet2_Eta"]      = new TH1F("h_jet2_Eta", "", 100, -5, 5.);
  	h1["h_jet2_Phi"]      = new TH1F("h_jet2_Phi", "", 100, -3.2, 3.2);
  	h1["h_jet2_BTag"]     = new TH1F("h_jet2_BTag", "", 100, 0., 1.);
  	
	h1["h_obj_M"]  		  = new TH1F("h_obj_M12","",100,0.,1000.);	
	h1["h_obj_Pt"]		  = new TH1F("h_obj_Pt","",100,0.,1000.);
	h1["h_obj_Eta"]		  = new TH1F("h_obj_Eta","",100,-5.,5.);
	h1["h_obj_Phi"]       = new TH1F("h_obj_Phi","",100,-3.2,3.2);
	
	h1["h_djet_Pt12"]     = new TH1F("h_djet_Pt12","",100,0.,200);
	h1["h_djet_Eta12"]    = new TH1F("h_djet_Eta12","",100,-5.,5.);
	h1["h_djet_Phi12"]    = new TH1F("h_djet_Phi12","",100,-3.2,3.2);
	h1["h_djet_PtTOT"]	  = new TH1F("h_djet_PtTOT","",100,0,1);
   
   // Analysis of events
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // Selection of double b-tag sample:
      Collection<Jet> jets = analysis.collection<Jet>("Jets");
      Collection<TriggerObject> hltDoublePFJetsC100 = analysis.collection<TriggerObject>("hltDoublePFJetsC100");
      Collection<TriggerObject> hltDoubleBTagCSV0p9 = analysis.collection<TriggerObject>("hltDoubleBTagCSV0p9");
      Collection<TriggerObject> hltL1sL1DoubleJetC100 = analysis.collection<TriggerObject>("hltL1sL1DoubleJetC100");
      Collection<Jet> leadJets;
      
      leadJetsNumber.clear();
      int bTagged = 0;
      for( int iJet = 0; iJet < jets.size(); ++iJet)
      {
      	Jet jet = jets.at(iJet);
		if(jet.idLoose() ) leadJetsNumber.push_back(iJet);
		if(jet.btag() > 0.9 && jet.matchTo(hltDoubleBTagCSV0p9.vectorCandidates(),"hltDoubleBTagCSV0p9") ) bTagged ++;
		
      }
      if( leadJetsNumber.size() < 2) continue;
       
      leadJets.add(jets.at(leadJetsNumber[0]));
      leadJets.add(jets.at(leadJetsNumber[1]));
      
      if ( !analysis.triggerResult(triggerLogicName) ) continue;
      if( leadJets.at(0).pt() < 100 || leadJets.at(1).pt() < 100 ) continue;
      if( std::abs(leadJets.at(0).eta()) > 2.4 || std::abs(leadJets.at(1).eta()) > 2.4 ) continue;
      if( std::abs( leadJets.at(0).eta() - leadJets.at(1).eta() ) > 1.4 ) continue;
      if( leadJets.at(0).deltaR(leadJets.at(1)) <= 1) continue;
      if( leadJets.at(0).btag() < 0.9 || leadJets.at(1).btag() < 0.9 ) continue;
      
      //Trigger matching:
      //Two Leading jets matchs online objects
      if( !leadJets.at(0).matchTo(hltDoublePFJetsC100.vectorCandidates(),"hltDoublePFJetsC100")) continue;
      if( !leadJets.at(1).matchTo(hltDoublePFJetsC100.vectorCandidates(),"hltDoublePFJetsC100")) continue;
      if( !leadJets.at(0).matchTo(hltL1sL1DoubleJetC100.vectorCandidates(),"hltL1sL1DoubleJetC100")) continue;
      if( !leadJets.at(1).matchTo(hltL1sL1DoubleJetC100.vectorCandidates(),"hltL1sL1DoubleJetC100")) continue;
      
      //Two any jets mutch b-tagged objects:
      if(bTagged < 2) continue;
      
      h1["h_jet1_Pt"] -> Fill(leadJets.at(0).pt());
     h1["h_jet1_Eta"] -> Fill(leadJets.at(0).eta());
     h1["h_jet1_Phi"] -> Fill(leadJets.at(0).phi());
     h1["h_jet1_BTag"] -> Fill(leadJets.at(0).btag());
     
     h1["h_jet2_Pt"] -> Fill(leadJets.at(1).pt());
     h1["h_jet2_Eta"] -> Fill(leadJets.at(1).eta());
     h1["h_jet2_Phi"] -> Fill(leadJets.at(1).phi());
     h1["h_jet2_BTag"] -> Fill(leadJets.at(1).btag());
     
     TLorentzVector obj;
     obj = leadJets.at(0).p4() + leadJets.at(1).p4();
     
     h1["h_obj_Pt"] -> Fill(obj.Pt());
     h1["h_obj_Eta"] -> Fill(obj.Eta());
     h1["h_obj_Phi"] -> Fill(obj.Phi());
     h1["h_obj_M"] -> Fill(obj.M());
     
     h1["h_djet_Pt12"] -> Fill(leadJets.at(0).pt() - leadJets.at(1).pt() );
     h1["h_djet_Eta12"] -> Fill( leadJets.at(0).eta() - leadJets.at(1).eta() );
     h1["h_djet_Phi12"] -> Fill( leadJets.at(0).phi() - leadJets.at(1).phi() );
     h1["h_djet_PtTOT"] -> Fill( (leadJets.at(0).pt() - leadJets.at(1).pt() )/ (leadJets.at(0).pt() + leadJets.at(1).pt()) );
      
      ++nCand;

   }
   std::cout<<"Number of Candidates: "<<nCand<<std::endl;
   std::string fileName = "output.root";
   fileName = analysis.eventTree()->GetFile()->GetName();
   //fileName = "outputFiles/"+fileName;
   fileName = "outputFiles/Results" + boost::filesystem::basename(fileName) + ".root";
   //if(analysis.isMC()) fileName = "outputFiles/" + boost::filesystem::basename(fileName) + ".root";
   TFile * outFile = new TFile(fileName.c_str(),"RECREATE");
   for ( auto& ih1 : h1 )
   {
   	if(analysis.isMC()) ih1.second->Scale(594.65/analysis.crossSection());
   	ih1.second -> Write();
   }
   outFile -> Close();
   
   return 0;
//
}
