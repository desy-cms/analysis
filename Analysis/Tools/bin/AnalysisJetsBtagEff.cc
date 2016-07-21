#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1.h" 
#include "TH2.h"
#include "TLorentzVector.h"

#include "Analysis/Tools/interface/Analysis.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;


// =============================================================================================   
int main(int argc, char * argv[])
{
   
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list

   std::string inputList = "rootFileList.txt";
   Analysis analysis(inputList);

   // Physics Objects Collections
   analysis.addTree<Jet> ("Jets","MssmHbb/Events/slimmedJetsReapplyJEC");
   // GenParticles (when needed)
   analysis.addTree<GenParticle> ("GenParticles","MssmHbb/Events/prunedGenParticles");

   //Cross-sections
   analysis.crossSections("MssmHbb/Metadata/CrossSections");
   //Luminosity
   //float lumiScale = analysis.scaleLuminosity(1000.);

   // Use external btagging efficiencies
   analysis.addBtagEfficiencies("final_extended_delR0.3_smoothed.root");
   analysis.btagEfficienciesAlgo("btag_csvivf");  // only needed above a certain commit where this information should be available in the title of the root file above
   analysis.btagEfficienciesFlavour("Extended");  // only needed above a certain commit where this information should be available in the title of the root file above
   
   
   // Analysis of events
   std::cout << "This analysis has " << analysis.size() << " events" << std::endl;

   const static int nptbins = 41;
   float ptbins[nptbins+1] = {0,5,10,20,40,60,80,100,120,140,160,180,200,220,240,280,320,360,400,430,460,500,520,560,620,680,700,750,850,960,1000,1300,1500,1600,1800,2000,2400,2800,3000,3300,3500,4000};
    const static int netabins = 5;
    float etabins[netabins+1] = {0.,0.5,1.0,1.5,2.2,2.4};

    TH2F * hist1b = new TH2F("hist1b" , "jet_w/o_weight_b", nptbins, ptbins, netabins, etabins);
    TH2F * hist1c = new TH2F("hist1_c" , "jet_w/o_weight_c", nptbins, ptbins, netabins, etabins);
    TH2F * hist1l = new TH2F("hist1_l" , "jet_w/o_weight_l", nptbins, ptbins, netabins, etabins);
    TH2F * hist1bb = new TH2F("hist1_bb" , "jet_w/o_weight_bb", nptbins, ptbins, netabins, etabins);
    TH2F * hist1cc = new TH2F("hist1_cc" , "jet_w/o_weight_cc", nptbins, ptbins, netabins, etabins);
    //TH2F * hist2 = new TH2F("hist2" , "jet2_w/o weight", nptbins, ptbins, netabins, etabins);
    TH2F * hist3b = new TH2F("hist3b" , "jet_weighted_b", nptbins, ptbins, netabins, etabins);
    TH2F * hist3c = new TH2F("hist3_c" , "jet_weighted_c", nptbins, ptbins, netabins, etabins);
    TH2F * hist3l = new TH2F("hist3_l" , "jet_weighted_l", nptbins, ptbins, netabins, etabins);
    TH2F * hist3bb = new TH2F("hist3_bb" , "jet_weighted_bb", nptbins, ptbins, netabins, etabins);
    TH2F * hist3cc = new TH2F("hist3_cc" , "jet_weighted_cc", nptbins, ptbins, netabins, etabins);
    //TH2F * hist4 = new TH2F("hist4" , "jet2_weighted", nptbins, ptbins, netabins, etabins);

    //TH1F * hist5 = new TH1F("hist5", "mass_w/o_weight",12,0,1200);
    //TH1F * hist6 = new TH1F("hist6", "mass_weighted",12,0,1200);


    TFile * newFile = new TFile("histograms_wo_weight_800_to_1000.root" , "RECREATE");

    for ( int i = 0 ; i < analysis.size() ; ++i )
    {
      analysis.event(i);
      
      std::cout << "++++++    ENTRY  " << i << std::endl;
      
      // GenParticles
      auto particles = analysis.collection<GenParticle>("GenParticles");
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      jets->btagAlgo("btag_csvivf");
      
      // Needed to use the extended flavour
      jets->associatePartons(particles,0.3,5);
      
      //std::ofstream output;
      //output.open("output_file.txt");

      if ( jets->size() < 1 ) continue;
      Jet jet1 = jets->at(0);
      //Jet jet2 = jets->at(1);

       if ( jet1.pt() < 40. ) continue;
       if ( (jet1.eta() > 2.2 || jet1.eta() < -2.2)  ) continue;
     // if ( ((jet1.eta()- jet2.eta()) > 1.6) || ((jet2.eta()-jet1.eta()) > 1.6) ) continue;
      //if (jet1.deltaR(jet2) > 1) continue;

//      cout << jet.pt() << "  " << jet.eta() << "  " << jet.flavour() << endl;
      double efficiency1 = analysis.btagEfficiency(jet1,0);
      //double efficiency2 = analysis.btagEfficiency(jet2,0);  

     // TLorentzVector p1 = jet1.p4();
      //TLorentzVector p2 = jet2.p4();
     // TLorentzVector p12 = p1+p2;

     // double m = p1.M();
      //double m2 = p2.M();
      //double mtot = m1+m2;
          
     if ( jet1.extendedFlavour() == "b" )
      hist3b->Fill(jet1.pt(),jet1.eta(),efficiency1);
      if ( jet1.extendedFlavour() == "c" )
      hist3c->Fill(jet1.pt(),jet1.eta(),efficiency1);
      if ( jet1.extendedFlavour() == "udsg" )
      hist3l->Fill(jet1.pt(),jet1.eta(),efficiency1);
      if ( jet1.extendedFlavour() == "bb" )
      hist3bb->Fill(jet1.pt(),jet1.eta(),efficiency1);
      if ( jet1.extendedFlavour() == "cc" )
      hist3cc->Fill(jet1.pt(),jet1.eta(),efficiency1);
    
      //hist4->Fill(jet2.pt(),jet2.eta(),(efficiency1*efficiency2));

      //hist6->Fill(m,efficiency1);

  


      if ( jet1.btag() < 0.935) continue;


      if ( jet1.extendedFlavour() == "b" )
      hist1b->Fill(jet1.pt(),jet1.eta());
      if ( jet1.extendedFlavour() == "c" )
      hist1c->Fill(jet1.pt(),jet1.eta());
      if ( jet1.extendedFlavour() == "udsg" )
      hist1l->Fill(jet1.pt(),jet1.eta());
      if ( jet1.extendedFlavour() == "bb" )
      hist1bb->Fill(jet1.pt(),jet1.eta());
      if ( jet1.extendedFlavour() == "cc" )
      hist1cc->Fill(jet1.pt(),jet1.eta());
      
     // hist2->Fill(jet2.pt() , jet2.eta());

      //hist5->Fill(m);


     

 //     for ( int j = 0 ; j < jets->size() ; ++j )
 //     {
 //        Jet jet = jets->at(j);
//         cout << "    Jet #" << j << ": ";
//         cout  << "pT  = "           << jet.pt()      << ", ";
//         cout  << "eta = "           << jet.eta()     << ", ";
//         cout  << "phi = "           << jet.phi()     << ", ";
//         cout  << "id loose = "      << jet.idLoose() << ", ";
//         cout  << "flavour = "       << jet.flavour() << ", ";
//         cout  << "extFlavour = "    << jet.extendedFlavour() << ", ";
//         cout  << "btag = "    << jet.btag()<< std::endl;
//         cout  << "efficiency = "  << analysis.btagEfficiency(jet) << std::endl;
           
//      }
     //cout.close();
           

   }
     // hist1 -> Scale(lumiScale);
      hist1b -> Write();
      hist1c -> Write();
      hist1l -> Write();
      hist1bb -> Write();
      hist1cc -> Write();

      //hist2 -> Scale(lumiScale);
      //hist2 -> Write();
     // hist5 -> Scale(lumiScale);
     // hist5 -> GetXaxis() -> SetRangeUser(0,700);
     // hist5 -> GetYaxis() -> SetRangeUser(0,1000000);
      //hist5 -> Write();
      newFile -> Close();

   TFile * newFile2 = new TFile("histograms_with_weight_800_to_1000.root", "RECREATE");
      
     // hist3 -> Scale(lumiScale);
      hist3b -> Write();
      hist3c -> Write();
      hist3l -> Write();
      hist3bb -> Write();
      hist3cc -> Write();

      //hist4 -> Scale(lumiScale);
      //hist4 -> Write();
      //hist6 -> Scale(lumiScale);
      //hist6 -> GetXaxis() -> SetRangeUser(0,700);
     // hist6 -> GetYaxis() -> SetRangeUser(0,1000000);
      //hist6 -> Write();

      newFile2 -> Close();


//    
}

