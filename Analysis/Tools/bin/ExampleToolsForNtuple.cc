#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/JetCollection.h"
#include "Analysis/Tools/interface/Muons.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


void PrintCrossSections(TChain *);
void PrintGeneratorFilter(TChain *);

// =============================================================================================   
int main(int argc, char * argv[])
{
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt"; 
   TFileCollection fc("dum","",inputList.c_str());
   
// =============================================================================================   
   // Acces to Metadata (outside the event loop, no friendship with event-like trees)
   TChain * t_xsection  = new TChain("MonteCarloStudies/Metadata/CrossSections");
   t_xsection -> AddFileInfoList((TCollection*) fc.GetList());
   PrintCrossSections(t_xsection);

   TChain * t_genfilter  = new TChain("MonteCarloStudies/Metadata/GeneratorFilter");
   t_genfilter -> AddFileInfoList((TCollection*) fc.GetList());
   PrintGeneratorFilter(t_genfilter);
// =============================================================================================   
   
// =============================================================================================
   // EVENT stuff   
   // Main event tree
   TChain * t_Event   = new TChain("MonteCarloStudies/Events/EventInfo");
   t_Event->AddFileInfoList((TCollection*) fc.GetList());
   
   // Other event trees
   TChain * t_Jets    = new TChain("MonteCarloStudies/Events/slimmedJetsPuppi");
   TChain * t_Muons   = new TChain("MonteCarloStudies/Events/slimmedMuons");
   t_Jets   -> AddFileInfoList((TCollection*) fc.GetList());
   t_Muons  -> AddFileInfoList((TCollection*) fc.GetList());
   
   // Friendship (DON'T FORGET!!!)
   t_Event -> AddFriend(t_Jets);
   t_Event -> AddFriend(t_Muons);
   
   // Create objects collections
   JetCollection  jets (t_Jets);
   Muons muons(t_Muons);
   
   // HISTOGRAMS
   std::map<std::string, TH1F*> h1;
   // JETS
   h1["h_jet_N"]        = new TH1F("h_jet_N" , "", 20, 0., 20.);
   h1["h_jet_Pt"]       = new TH1F("h_jet_Pt" , "", 200, 0., 1000.);
   h1["h_jet_Eta"]      = new TH1F("h_jet_Eta", "", 100, -5, 5.);
   h1["h_jet_Phi"]      = new TH1F("h_jet_Phi", "", 100, 3.2, 3.2);
   h1["h_jet_Btag"]     = new TH1F("h_jet_Btag", "", 100, 0., 1.);
   h1["h_jet_Flavour"]  = new TH1F("h_jet_Flavour", "", 40, -10., 30.);
   h1["h_jet_IdLoose"]  = new TH1F("h_jet_IdLoose", "", 2, 0, 2);
   h1["h_jet_IdTight"]  = new TH1F("h_jet_IdTight", "", 2, 0, 2);
   // MUONS
   h1["h_muon_N"]       = new TH1F("h_muon_N" , "", 20, 0., 20.);
   h1["h_muon_Pt"]      = new TH1F("h_muon_Pt" , "", 200, 0., 1000.);
   h1["h_muon_Eta"]     = new TH1F("h_muon_Eta", "", 100, -5, 5.);
   h1["h_muon_Phi"]     = new TH1F("h_muon_Phi", "", 100, 3.2, 3.2);

   
   // Number of loop events
   int nEvents = t_Event->GetEntries();
   
   int eventCounter = 0;
   
   for ( int i = 0 ; i < nEvents ; ++i )
   {
//      if ( i%10000 == 0 ) std::cout << "Processed  " << i << "  events" << "\xd";
      ++eventCounter;
      
      t_Event->GetEntry(i);
      
      
      h1["h_jet_N"]  -> Fill(jets.size());
      for ( int i = 0; i < jets.size(); ++i )
      {
         Jet jet = jets.at(i);
         h1["h_jet_Pt"]      -> Fill(jet.pt());
         h1["h_jet_Eta"]     -> Fill(jet.eta());
         h1["h_jet_Phi"]     -> Fill(jet.phi());
         h1["h_jet_Btag"]    -> Fill(jet.btag());
         h1["h_jet_Flavour"] -> Fill(jet.flavour());
         h1["h_jet_IdLoose"] -> Fill(jet.idLoose());
         h1["h_jet_IdTight"] -> Fill(jet.idTight());
      }
      
      h1["h_muon_N"]  -> Fill(muons.size());
      for ( int i = 0; i < muons.size(); ++i )
      {
         Muon muon = muons.at(i);
         h1["h_muon_Pt"]      -> Fill(muon.pt());
         h1["h_muon_Eta"]     -> Fill(muon.eta());
         h1["h_muon_Phi"]     -> Fill(muon.phi());
     }
      
   }
   
   TFile * outFile = new TFile("ExampleNtupleHistograms.root","RECREATE");
   outFile -> mkdir("Jets","Jets");
   outFile -> mkdir("Muons","Muons");
   
   for ( auto& ih1 : h1 )
   {
      if ( ih1.first.find("h_jet_") != std::string::npos )
         outFile -> cd("Jets");
      if ( ih1.first.find("h_muon_") != std::string::npos )
         outFile -> cd("Muons");
      ih1.second -> Write();
   }
   outFile -> Close();

      
//    
}

void PrintCrossSections(TChain * tree)
{
      // Cross Sections - print on the screen
   std::cout << "=======================================================" << std::endl;
   std::cout << "  CROSS SECTIONS" << std::endl;
   std::cout << "=======================================================" << std::endl;
   
   TObjArray * xsecBranches = tree->GetListOfBranches();
   std::map<std::string, double> xsections;
   for ( int i = 0 ; i < xsecBranches->GetEntries() ; ++i )
   {
      std::string branch = xsecBranches->At(i)->GetName();
      if ( branch == "run" ) continue;
      xsections[branch] = 0;
      tree -> SetBranchAddress(branch.c_str(), &xsections[branch]);
   }
   tree -> GetEntry(0);
   for ( auto& xs : xsections )
   {
      std::cout << xs.first << " = " << xs.second << " pb " << std::endl;
   }
   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;

}

void PrintGeneratorFilter(TChain * tree)
{
   std::cout << "=======================================================" << std::endl;
   std::cout << "  GENERATOR FILTER" << std::endl;
   std::cout << "=======================================================" << std::endl;
   
   unsigned int nEvtTot;
   unsigned int nEvtFlt;
   unsigned int sEvtTot = 0;
   unsigned int sEvtFlt = 0;
      
   tree -> SetBranchAddress("nEventsTotal", &nEvtTot);
   tree -> SetBranchAddress("nEventsFiltered", &nEvtFlt);
   
   for ( int i = 0; i < tree->GetEntries(); ++i )
   {
      tree -> GetEntry(i);
      sEvtTot += nEvtTot;
      sEvtFlt += nEvtFlt;
   }
   float genFilterEff = float(sEvtFlt)/sEvtTot;
   
   std::cout << "Total generated events = " << sEvtTot << std::endl;
   std::cout << "Filtered generated events = " << sEvtFlt << std::endl;
   std::cout << "Generator Filter Efficiency = " << genFilterEff << std::endl;
   
   std::cout << "=======================================================" << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;
}
