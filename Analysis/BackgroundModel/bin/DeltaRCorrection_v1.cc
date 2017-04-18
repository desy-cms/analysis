#include <iostream>
#include <string>
#include <memory>
#include "TSystem.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TRandom3.h"

using namespace std;

int main(int /* argc */, char* /* argv */[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  const std::string inputFileName =
    cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_prescale_13TeV_G4.root";   

  const std::string outputFileName =
    cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_prescale_dRcorrection_13TeV_G4.root";
  
  TFile input(inputFileName.c_str(), "read");
  std::unique_ptr<TTree> inputTree(static_cast<TTree*>(input.Get("MssmHbb_13TeV")));

  TFile output(outputFileName.c_str(), "recreate");
  TTree outputTree("MssmHbb_13TeV", "MssmHbb_13TeV");

  // could think of using float, but better keeping it in agreement with input:
  double mbb;
  double weight = 1.0;  // make this maybe later a product of some other weights

  TRandom3 *r = new TRandom3(0); //seed 0 or 4357 default
  TF1 *f1 = new TF1("f1","[0]*x*x*x+[1]*x*x+[2]*x+[3]",1,3.5);
  f1->SetParameter(0,-7.19293e-02/1.016381);
  f1->SetParameter(1,4.53292e-01/1.016381);
  f1->SetParameter(2,-8.19299e-01/1.016381);
  f1->SetParameter(3,1.33441e+00/1.016381);
 
  inputTree->SetBranchAddress("mbb", &mbb);
  //inputTree->SetBranchAddress("weight", &weight);

  outputTree.Branch("mbb", &mbb, "mbb/D");
  outputTree.Branch("weight", &weight, "weight/D");

  for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
    inputTree->GetEntry(i);

    double random = r->Rndm(); 

    if(random > f1->GetRandom()) continue;
    outputTree.Fill();  

  }
  outputTree.Write();

  return 0;
}
