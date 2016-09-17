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

template<typename  T>
   struct GreaterByPt {
   bool operator()( const T & t1, const T & t2 ) const {
     return t1->pt() > t2->pt();
   }
};

// Histograms
void CreateHistograms();
void WriteHistograms(const char *);

std::map<string, TH1F*> h1_;


// =============================================================================================   
int main(int argc, char * argv[])
{   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

// Creat histograms
   CreateHistograms();
   
// Input files list
   std::string inputList = "rootFileList.txt";
   if ( argc == 2 ) inputList = std::string(argv[1]);
   
   Analysis analysis(inputList);
   
// Some useful info from metadata
   // Event filter
   FilterResults evtFilter = analysis.generatorFilter("MssmHbb/Metadata/EventFilter");
   h1_["evtTotal"]  -> SetBinContent(1,evtFilter.total);
   h1_["evtFilter"] -> SetBinContent(1,evtFilter.filtered);
   
// Physics Objects Collections
   // Jets
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // Trigger
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sL1DoubleJetC100");
   triggerObjects.push_back("hltDoubleJetsC100");
   triggerObjects.push_back("hltDoubleBTagCSV0p9");
   triggerObjects.push_back("hltDoublePFJetsC100");
   for ( size_t i = 0; i < triggerObjects.size() ; ++i )
      analysis.addTree<TriggerObject> (triggerObjects[i],"MssmHbb/Events/selectedPatTrigger/"+triggerObjects[i]);

// EVENT SELECTION   
   // Cuts
   float ptmin[3]   = { 100.0, 100.0, 40.0 };
   float etamax[3]  = {   2.2,   2.2 , 2.2 };
   float btagmin[3] = {   0.8,   0.8,  0.8 };  // medium WP = 0.8; tight WP = 0.935
   float dRmin      = 1.;
   float detamax    = 1.55;
   
// Analysis of events
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // check if trigger fired
      int triggerFired = analysis.triggerResult("HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v");
      if ( ! triggerFired ) continue;
      
      // Jets - std::shared_ptr< Collection<Object> >
      auto jets = analysis.collection<Jet>("Jets");
      
//       // Example: If need to change something and reorder the jets
//       std::vector< std::shared_ptr<Jet> >  sortedJets = jets->vector();
//       GreaterByPt< std::shared_ptr<Jet> > jetGreaterPt;
//       std::sort (sortedJets.begin(), sortedJets.end(),jetGreaterPt);
//       for ( int j = 0 ; j < jets->size() ; ++j )
//       {
//          std::cout << "Jet " << j << "  "  << jets->at(j).pt() << "   " << sortedJets[j]->pt()<< std::endl;
//       }
//       std::cout << "========" << std::endl;
      
      // select good jets - passed jet id loose
      std::vector<Jet*> selJets;
      for ( int j = 0 ; j < jets->size() ; ++j )
         if ( jets->at(j).idLoose() ) selJets.push_back(&jets->at(j));

      // need at least 3 good jets
      if ( selJets.size() < 3 ) continue;      
      
      // Kinematic selection
      bool goodEvent = true;
      for ( int j = 0; j < 3; ++j )
      {
         if ( selJets[j]->pt() < ptmin[j] || fabs(selJets[j]->eta()) > etamax[j] || selJets[j]->btag() < btagmin[j] )
         {
            goodEvent = false;
            break;
         }
      }
      
      // try to match the offline to the online objects
      analysis.match<Jet,TriggerObject>("Jets",triggerObjects,0.5);
      // do trigger objects match with offline?
      for ( size_t to = 0; to < triggerObjects.size() ; ++to )
      {
         if ( ! selJets[0]->matched(triggerObjects[to]) || !  selJets[1]->matched(triggerObjects[to]) )
         {
            goodEvent = false;
            break;
         }
      }
      if ( ! goodEvent ) continue;
      
      // Additional kinematic cuts
      // delta R between the leading jets
      if ( selJets[0]->deltaR(*selJets[1]) < dRmin ) continue;
      if ( selJets[0]->deltaR(*selJets[2]) < dRmin ) continue;
      if ( selJets[1]->deltaR(*selJets[2]) < dRmin ) continue;
      // delta eta between two leading jets
      if ( fabs(selJets[0]->eta() - selJets[1]->eta()) > detamax ) continue;
      
      
      // YEAH! Event passed selections!
      // Now let's fill some histograms!
      for ( int j = 0; j < 3; ++j )
      {
         h1_[Form("pt_jet%i",j)]   -> Fill(selJets[j]->pt());
         h1_[Form("eta_jet%i",j)]  -> Fill(selJets[j]->eta());
         h1_[Form("btag_jet%i",j)] -> Fill(selJets[j]->btag());
      }
      float m12 = (selJets[0]->p4() + selJets[1]->p4()).M();
      h1_["m12"] -> Fill(m12);
   }
   
   // Finish by writing the histograms into a file
   WriteHistograms("analysis_histograms.root");
   
//    
}

void CreateHistograms()
{
   for ( int j = 0; j < 3 ; ++j )
   {
      h1_[Form("pt_jet%i",j)]  = new TH1F(Form("pt_jet%i",j) ,Form("pt jet%i",j),100,0.,1000.);
      h1_[Form("eta_jet%i",j)] = new TH1F(Form("eta_jet%i",j) ,Form("eta jet%i",j),120,-3.,3.);
      h1_[Form("btag_jet%i",j)] = new TH1F(Form("btag_jet%i",j),Form("btag jet%i",j),50,0.5,1.);
   }
   h1_["m12"] = new TH1F("m12","invariant mass of the leading jets",20,0,1000);
   h1_["evtTotal"] = new TH1F("evtTotal","total number of events",1,0,1);
   h1_["evtFilter"] = new TH1F("evtFilter","number filtered events",1,0,1);
   
}

void WriteHistograms(const char * filename)
{
   TFile * outFile = new TFile(filename,"RECREATE");
   for ( auto & h : h1_ )
   {
      h.second->Write();
   }
   outFile -> Close();
   delete outFile;
}
