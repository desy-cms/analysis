#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/Tools/interface/Analysis.h"

#include "Analysis/MssmHbb/interface/BTagCalibrationStandalone.h"


using namespace std;
using namespace analysis;
using namespace analysis::tools;

template<typename  T>
   struct GreaterByPt {
   bool operator()( const T & t1, const T & t2 ) const {
     return t1->pt() > t2->pt();
   }
};

// Functions declarations
void CreateHistograms();
void ScaleHistograms();
void WriteHistograms(const std::string & );

std::vector<float> Btag3Weight(const std::vector<Jet*> &, const BTagCalibrationReader &);

float SignalCrossSection();
std::string Process();

// Histograms
std::map<string, TH1F*> h1_;

// auxiliar variables
bool isMC_;
std::string inputList_;
int nGenTotal_;
std::string selection_;
float bbbData_;

float lumi_  = 2690.496; // inb pb-1
float btagcut_[3] = {0.46,0.8,0.935};

bool isbbb_;
int btagWP_;

// =============================================================================================   
int main(int argc, char * argv[])
{   
// EVENT SELECTION   

   // signal or control region?   
   isbbb_ = true;                                  // <<<<===== CMSDAS
   
   // btag WP
   btagWP_ = 1;  // 0:LOOSE, 1:MEDIUM, 2:TIGHT     // <<<<===== CMSDAS
   
   // normalisation for data control region   
   bbbData_ = 20691;    // WP MMM                  // <<<<===== CMSDAS
   //bbbData_ = 6385;   // WP TTT
   
   // Cuts                                         // <<<<===== CMSDAS
   float ptmin[3]   = { 100.0, 100.0, 40.0 };
   float etamax[3]  = {   2.2,   2.2 , 2.2 };
   float btagmin[3] = { btagcut_[btagWP_], btagcut_[btagWP_], btagcut_[btagWP_]};
   float nonbtag    = 0.46;
   float dRmin      = 1.;
   float detamax    = 1.55;
      
   std::string btagSel;
   if ( btagWP_ == 0 ) btagSel = "LLL";
   if ( btagWP_ == 1 ) btagSel = "MMM";
   if ( btagWP_ == 2 ) btagSel = "TTT";
   // name of this selection
   if ( isbbb_ ) selection_ = "selection_bbb_" + btagSel; // <<<<===== CMSDAS
   else selection_ = "selection_bbnb_" + btagSel;         // <<<<===== CMSDAS
   
// -----------------
   
   // Trigger
   std::string hltPath = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
   // Trigger objects
   std::vector<std::string> triggerObjects;
   triggerObjects.push_back("hltL1sL1DoubleJetC100");
   triggerObjects.push_back("hltDoubleJetsC100");
   triggerObjects.push_back("hltDoubleBTagCSV0p9");
   triggerObjects.push_back("hltDoublePFJetsC100");
   
   // Btag SF
   BTagCalibration calib("csvv2", "SFbLib.csv");
   BTagCalibrationReader readerBSF(&calib,                              // calibration instance
                                   (BTagEntry::OperatingPoint)btagWP_,  // operating point          // <<<<===== CMSDAS
                                   "mujets",                            // measurement type
                                   "central");                          // systematics type
   
   
// ====================================   
   
// Creat histograms
   CreateHistograms();
   
// Input files list
   inputList_ = "rootFileList.txt";
   if ( argc == 2 ) inputList_ = std::string(argv[1]);

   std::string process = Process();
   if ( process == "null" )
   {
      std::cout << "Cannot identify a process. Finished" << std::endl;
      return -1;
   }
      
// Log settings
   // create directory
   boost::filesystem::path dir(selection_);
   boost::filesystem::create_directory(dir);
   std::string basename =  std::string(boost::filesystem::basename(inputList_));
   std::string logname = selection_ + std::string("/") + basename + std::string("_analysis.log");
   std::ofstream logfile;
   logfile.open (logname.c_str());
   logfile << "SimpleMssmHbb - CMSDAS 2016" << std::endl;
   logfile << "===========================" << std::endl;
   logfile << "Running on " << basename;
   if ( isbbb_ ) logfile << "  in the signal region" << std::endl;
   else          logfile << "  in the control region" << std::endl;
   logfile << std::endl;
   logfile << "Trigger path: " << hltPath << std::endl;
   logfile << "Jet 1: pT > " << ptmin[0] << ",  |eta| < " << etamax[0] << ",, btag > " << btagmin[0] << std::endl;
   logfile << "Jet 2: pT > " << ptmin[1] << ",  |eta| < " << etamax[1] << ",, btag > " << btagmin[1] << std::endl;
   logfile << "Jet 3: pT > " << ptmin[2] << ",   |eta| < " << etamax[2] << ",, btag > " << btagmin[2] << std::endl;
   logfile << "delta_eta_12 < " << detamax << "  and  delta _R_ij > " << dRmin << std::endl;
   logfile << std::endl;
   logfile << "Normalisation for data control region: " << bbbData_ << std::endl;
   logfile << "===========================" << std::endl;
   
   logfile.close();
   
// Construct the Analysis object
   Analysis analysis(inputList_);
   
// If this is data
   isMC_ = (process != "data");
   
   if ( isMC_ && !isbbb_ ) std::cout << "Warning: you are running blinding policy in MC." << std::endl;
   if ( !isMC_ && isbbb_ ) std::cout << "Bliding policy will be enforced for data in bbb selection" << std::endl;

   
// Physics Objects Collections
   // Jets
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // Trigger
   analysis.triggerResults("MssmHbb/Events/TriggerResults");
   // Trigger objects
   for ( size_t i = 0; i < triggerObjects.size() ; ++i )
      analysis.addTree<TriggerObject> (triggerObjects[i],"MssmHbb/Events/selectedPatTrigger/"+triggerObjects[i]);

   if ( ! isMC_ ) analysis.processJsonFile("Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON_Silver_v2.txt");
   
   
   
// Analysis of events
//   int bbbCount = 0;
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // Event weight
      float evtWeight = 1;
      
      if ( !isMC_ && ! analysis.selectJson() ) continue; 
      
      // check if trigger fired
      int triggerFired = analysis.triggerResult(hltPath);
      if ( ! triggerFired ) continue;
      
      // Jets - std::shared_ptr< Collection<Jet> >
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

      bool goodEvent = true;
      
      // Kinematic selection - 2 leading jets
      for ( int j = 0; j < 2; ++j )
      {
         if ( selJets[j]->pt() < ptmin[j] || fabs(selJets[j]->eta()) > etamax[j] || selJets[j]->btag() < btagmin[j] )
         {
            goodEvent = false;
            break;
         }
      }
      
      if ( ! goodEvent ) continue;
      
      // BLINDING POLICY!
      // Kinematic selection - 3. leading jet - non b for data (blind policy)
      if ( !isbbb_ )
      {
         if ( selJets[2]->pt() < ptmin[2] || fabs(selJets[2]->eta()) > etamax[2] || selJets[2]->btag() > nonbtag ) goodEvent = false;
      }
      else
      {
         if ( selJets[2]->pt() < ptmin[2] || fabs(selJets[2]->eta()) > etamax[2] || selJets[2]->btag() < btagmin[2] ) goodEvent = false;
      }
         
      if ( ! goodEvent ) continue;
      
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
      float deta12 =  fabs(selJets[0]->eta() - selJets[1]->eta()) ;
      if ( deta12 > detamax ) continue;
      
      // 
      float weightBTag = 1;
      
      if ( isMC_ )
      {
         // 3 leading jets with 3 btags
         weightBTag = Btag3Weight(selJets,readerBSF)[3];
      }
      
      evtWeight *= weightBTag;
      
      // YEAH! Event passed selections!
      // Now let's fill some histograms!
      for ( int j = 0; j < 3; ++j )
      {
         h1_[Form("pt_jet%i",j)]   -> Fill(selJets[j]->pt(),evtWeight);
         h1_[Form("eta_jet%i",j)]  -> Fill(selJets[j]->eta(),evtWeight);
         h1_[Form("btag_jet%i",j)] -> Fill(selJets[j]->btag(),evtWeight);
      }
      h1_["deta12"] -> Fill(deta12,evtWeight);
      
      if ( isMC_ || (!isMC_ && !isbbb_) ) // blinding policy for data
      {
         float m12 = (selJets[0]->p4() + selJets[1]->p4()).M();
         h1_["m12"] -> Fill(m12,evtWeight);
      }
      else
      {
//         std::cout << "Blinding policy: m12 will not be filled for data in bbb selection." << std::endl;
      }
   }
   
// Some useful info from metadata
   // Event filter
//   FilterResults evtFilter = analysis.eventFilter("MssmHbb/Metadata/EventFilter"); 
//   h1_["evtTotal"]  -> SetBinContent(1,evtFilter.total);
//   h1_["evtFilter"] -> SetBinContent(1,evtFilter.filtered);
   if ( isMC_ )
   {
      // Gen filter
      FilterResults genFilter = analysis.generatorFilter("MssmHbb/Metadata/GeneratorFilter");
      nGenTotal_ = genFilter.total;
      h1_["nGen"] -> SetBinContent(1,nGenTotal_);
   }
   
   // Scale histograms
   ScaleHistograms();
   
   // Finish by writing the histograms into a file
   std::string outFile = selection_ + std::string("/") + basename + std::string("_analysis_histograms.root");
   WriteHistograms(outFile);
   
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
   h1_["deta12"] = new TH1F("deta12","delta eta between the two leading jets",32,0,1.6);
   h1_["m12"] = new TH1F("m12","invariant mass of the leading jets",75,0,1500);
   h1_["nGen"] = new TH1F("nGen","total number of generated events",1,0,1);
   
   for ( auto & h : h1_ )
   {
      h.second -> Sumw2();
   }
   
   
}

void ScaleHistograms()
{
   float sf = 1;
   
   // scale data bbnonb to bbb
   if ( !isMC_ && !isbbb_ )
   {
      std::cout << "Scaling bb-nonb to bbb in data" << std::endl;
      sf = bbbData_/h1_["m12"]->GetEntries();
   }
   
   
   if ( Process() == "mssmhbb" )
   {
      float genLumi = nGenTotal_/SignalCrossSection();
      sf = lumi_/genLumi;
   }
   
   for ( auto & h : h1_ )
   {
      if ( h.first != "nGen" )
      {
         h.second->Scale(sf);
      }
   }

}


void WriteHistograms(const std::string & filename)
{
   
   TFile * outFile = new TFile(filename.c_str(),"RECREATE");
      
   for ( auto & h : h1_ )
   {
      h.second->Write();
   }
   outFile -> Close();
   delete outFile;
}

float SignalCrossSection()  // in pb
{
   std::string basename =  std::string(boost::filesystem::basename(inputList_));
   std::string mass = basename.substr(8,basename.length());
   
   std::map<std::string,float> xs;
   xs["100"] =  365.04 * 0.889297        +   5.21588 * 0.829672      ;
   xs["120"] =  207.345 * 0.886169       +   41.7379 * 0.88271       ;
   xs["160"] =  79.5406 * 0.88124        +   75.3253 * 0.879037      ;
   xs["200"] =  36.1034 * 0.857523       +   35.4616 * 0.869342      ;
   xs["250"] =  15.8387 * 0.811031       +   15.6594 * 0.85409       ;
   xs["300"] =  7.7196 * 0.705892        +   7.65395 * 0.809695      ;
   xs["350"] =  4.14873 * 0.564904       +   4.13288 * 0.708487      ;
   xs["400"] =  2.36367 * 0.515287       +   2.36207 * 0.624898      ;
   xs["500"] =  0.887828 * 0.435839      +   0.889008 * 0.415354     ;
   xs["600"] =  0.380582 * 0.350134      +   0.381322 * 0.346045     ;
   xs["700"] =  0.180691 * 0.311139      +   0.181021 * 0.309895     ;
   xs["900"] =  0.0497564 * 0.27335      +   0.0497311 * 0.272931    ;
   xs["1100"] =  0.0164384 * 0.254142    +   0.0164648 * 0.253845    ;
   xs["1300"] =  0.00616693 * 0.242023   +   0.00617024 * 0.241754   ;

   return xs[mass];
}

std::string Process()
{
   std::string basename =  std::string(boost::filesystem::basename(inputList_));
   
   if (basename.find("mssmhbb") != std::string::npos) return std::string("mssmhbb");
   if (basename.find("qcd") != std::string::npos) return std::string("qcd");
   if (basename.find("data") != std::string::npos) return std::string("data");
   
   return std::string("null");
   
}

// method that calculates the event weight based on the number of b-tagged jets in MC and the expected number of b-tags among the three leading jets
// https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1c_Event_reweighting_using_scale
std::vector<float> Btag3Weight(const std::vector<Jet*> & jets, const BTagCalibrationReader & reader)
{
   std::vector<float> weights;
      
   if ( jets.size() < 3 ) return weights;
   
   // BTag Scale Factor
   float sf[3];
   for ( int j = 0; j < 3; ++j )
   {
      float pt = jets[j]->pt();
      float eta = jets[j]->eta();
      if ( pt > 660 ) pt = 660;
      sf[j] = reader.eval(BTagEntry::FLAV_B, eta, pt);
   }
   // bin0
   weights.push_back((1-sf[0])*(1-sf[1])*(1-sf[2]));
   // bin1 
   weights.push_back((1-sf[0])*(1-sf[1])*sf[2] + (1-sf[0])*(1-sf[2])*sf[1] + (1-sf[1])*(1-sf[2])*sf[0]);
   // bin2 
   weights.push_back((1-sf[0])*sf[1]*sf[2] + (1-sf[1])*sf[0]*sf[2] + (1-sf[2])*sf[0]*sf[1]);
   // bin3 
   weights.push_back(sf[0]*sf[1]*sf[2]);
      
   return weights;
   
}

