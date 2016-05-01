#include "Analysis/QCDSideBand/interface/ProgramOptions.h"
#include "Analysis/QCDSideBand/interface/Datasets.h"
#include "Analysis/QCDSideBand/interface/StackPlot.h"
#include "Analysis/QCDSideBand/interface/SumDatasets.h"

#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCut.h"
#include "TVector3.h"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>

#include <sys/stat.h>

int main(int argc, char* argv[]) {

  TH1::SetDefaultSumw2();
 
  ProgramOptions options(argc, argv);

  // input datasets
  Datasets datasets(options.iDir);
  datasets.readFile(options.datasetFile);

  //std::string oDir_Plot = options.oDir+std::string("/DoubleBcheck"); 
  std::string oDir_Plot = options.oDir+std::string("/QCDBackground");

  boost::filesystem::path opath(oDir_Plot);
  if (!exists(opath)) {
    std::cout << "Creating output directory : " << oDir_Plot << std::endl;
    boost::filesystem::create_directory(opath);
  }

  // output file
  //TFile* ofile = TFile::Open( (options.oDir+std::string("/QCDBackground.root")).c_str(), "RECREATE");

  // weight applied to MC
  TCut puWeight("WeightPileUp[0]");				//PU weight 
  TCut lumiWeight("LumiWeight");				//data luminosity
  TCut trigWeight("TwoDPtWeight * dEtaWeight");			//trigger efficiency vs pT and eta
  TCut btagWeight("BTagWeight");				//online BTag efficiency from B-particle Gun MC
  TCut btagSFWeight("BTagSFcentral[0] * BTagSFcentral[1]");	//offline BTag scale factor from BTV POG 
 
  // loop over MC datasets
  for (unsigned i=0; i<datasets.size(); ++i) {

    Dataset dataset = datasets.getDataset(i);
    //std::cout << "Opening dataset : " << dataset.name << std::endl; 

    // check datasets
    if (dataset.isData) {
      std::cout << "Analysing Data      : " << dataset.name << std::endl;
    }
    else if(dataset.name.compare(36,4,"SUSY")==0) {
      std::cout << "Analysing Sig MC    : " << dataset.name << std::endl;
    }
    else {
      std::cout << "Analysing Bg MC     : " << dataset.name << std::endl;
    }
   
    // apply weight to MC
    TCut mcWeight = puWeight * btagWeight * lumiWeight * trigWeight * btagSFWeight;
 
    // get file & tree
    TFile* file = datasets.getTFile(dataset.name);
    TTree* tree;
    file->GetObject("MssmHbb",tree);

    // output file
    TFile* ofile_Plot = TFile::Open((oDir_Plot+std::string("/")+dataset.name+std::string(".root")).c_str(), "RECREATE");

    // list of histograms
    TH1D* Mbb_L 	= new TH1D("Mbb_L","Mbb Sideband LowM",60,0,1200);   
    TH1D* Mbb_M		= new TH1D("Mbb_M","Mbb Sideband LowM",60,0,1200);
 
    // triple btag reverse region
    //double leadPt[20],leadEta[20],leadPhi[20];
    //tree->SetBranchAddress("LeadPt"	,leadPt);
    //tree->SetBranchAddress("LeadEta"    ,leadEta);
    //tree->SetBranchAddress("LeadPhi"    ,leadPhi);

    //TVector3 Jet1,Jet2,Jet3;
    //Jet1.SetPtEtaPhi(leadPt[0],leadEta[0],leadPhi[0]);
    //Jet2.SetPtEtaPhi(leadPt[1],leadEta[1],leadPhi[1]);
    //Jet3.SetPtEtaPhi(leadPt[2],leadEta[2],leadPhi[2]);

    //TCut Jet3pT		= "";
    TCut Jet3pT  	= "LeadPt[2] > 30 && abs(LeadEta[2]) < 2.2";
    TCut Jet3dR  	= "TMath::Sqrt((LeadEta[2]-LeadEta[0])*(LeadEta[2]-LeadEta[0]) + fabs(fabs(fabs(LeadPhi[2]-LeadPhi[0])-TMath::Pi())-TMath::Pi())*fabs(fabs(fabs(LeadPhi[2]-LeadPhi[0])-TMath::Pi())-TMath::Pi())) > 1.0 && TMath::Sqrt((LeadEta[2]-LeadEta[1])*(LeadEta[2]-LeadEta[1]) + fabs(fabs(fabs(LeadPhi[2]-LeadPhi[1])-TMath::Pi())-TMath::Pi())*fabs(fabs(fabs(LeadPhi[2]-LeadPhi[1])-TMath::Pi())-TMath::Pi())) > 1.0";
    TCut Jet3bTag_L 	= "LeadBTag[2] < 0.46";
    TCut Jet3bTag_M     = "LeadBTag[2] < 0.80";

    //tree->Draw("ObjM12>>Mbb", Jet3pT + Jet3dR ,"E");

    if(dataset.isData) {
    	tree->Draw("ObjM12>>Mbb_L",(Jet3pT + Jet3dR + Jet3bTag_L),"E");
	tree->Draw("ObjM12>>Mbb_M",(Jet3pT + Jet3dR + Jet3bTag_M),"E");
    }
    else {
        tree->Draw("ObjM12>>Mbb_L",(Jet3pT + Jet3dR + Jet3bTag_L) * mcWeight,"E");
	tree->Draw("ObjM12>>Mbb_M",(Jet3pT + Jet3dR + Jet3bTag_M) * mcWeight,"E");
    }

    //Mbb_L->Scale(1/Mbb_L->Integral()); //normalization to 1
    //Mbb_M->Scale(1/Mbb_M->Integral());

    // write histogram in output file
    ofile_Plot->cd();
    Mbb_L->Write("",TObject::kOverwrite);
    Mbb_M->Write("",TObject::kOverwrite);

    ofile_Plot->Close();

    // clean up
    delete tree;
    file->Close();

  }  

  // list histograms for dataset summing
  std::vector<std::string> hists;
  hists.push_back("Mbb_L");
  hists.push_back("Mbb_M");

  // sum signal datasets
  std::vector<std::string> sig250Datasets;
  sig250Datasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8_29_03_2016"));
  SumDatasets(oDir_Plot, sig250Datasets, hists, "Signal250");

  std::vector<std::string> sig300Datasets;
  sig300Datasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8_17_03_2016"));
  SumDatasets(oDir_Plot, sig300Datasets, hists, "Signal300");

  std::vector<std::string> sig350Datasets;
  sig350Datasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8_29_03_2016"));
  SumDatasets(oDir_Plot, sig350Datasets, hists, "Signal350");

  std::vector<std::string> sig400Datasets;
  sig400Datasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8_29_03_2016"));
  SumDatasets(oDir_Plot, sig400Datasets, hists, "Signal400"); 

  std::vector<std::string> qcdDatasets;
  qcdDatasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_Pythia8_16_03_2016"));
  SumDatasets(oDir_Plot, qcdDatasets, hists, "QCD");

  std::vector<std::string> realDatasets;
  realDatasets.push_back(std::string("DoubleBTagSelection_76X_lowMTrigger_13_04_2016"));
  SumDatasets(oDir_Plot, realDatasets, hists, "Data");

  // making plots here
  std::cout << "Making plots" << std::endl;
  StackPlot plots(oDir_Plot);
  plots.setLegPos(0.65,0.62,0.88,0.83);	//reset legend position on plots

  plots.addDataset("QCD", "QCD Pythia8", kAzure-2, 0);
  plots.addDataset("Signal250", "Signal 250", kRed, 2);
  plots.addDataset("Signal300", "Signal 300", kBlue, 2);
  plots.addDataset("Signal350", "Signal 350", kViolet, 2);
  plots.addDataset("Signal400", "Signal 400", kGreen+3, 2);
  plots.addDataset("Data", kBlack, 1);
  
  plots.setLumi(2.3); //just to add text on plot
  plots.draw("Mbb_L","M_{bb} [GeV]","Events / 20 GeV",0,"NONE");
  //plots.draw("Mbb_L","M_{bb} [GeV]","Events / 20 GeV",1,"NONE");
  plots.draw("Mbb_L","M_{bb} [GeV]","Events / 20 GeV",1,"RATIO");
  plots.draw("Mbb_M","M_{bb} [GeV]","Events / 20 GeV",0,"NONE");
  plots.draw("Mbb_M","M_{bb} [GeV]","Events / 20 GeV",1,"NONE");

  // Fitting Data

}




 
