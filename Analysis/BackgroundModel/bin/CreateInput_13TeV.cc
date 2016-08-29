#include <string>
#include <memory>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"


int main(int /* argc */, char* /* argv */[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  const std::string inputFileName =
    "/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_18_08_2016.root";
  const std::string outputFileName =
    cmsswBase+"/src/Analysis/BackgroundModel/data/TripleBTagReverseSelection_13TeV.root";

  TFile input(inputFileName.c_str(), "read");
  std::unique_ptr<TTree> inputTree(static_cast<TTree*>(input.Get("MssmHbb")));

  TFile output(outputFileName.c_str(), "recreate");
  TTree outputTree("MssmHbb_13TeV", "MssmHbb_13TeV");

  // could think of using float, but better keeping it in agreement with input:
  double mbb;
  double weight = 1.0;  // make this maybe later a product of some other weights

  inputTree->SetBranchAddress("ObjM12", &mbb);
  outputTree.Branch("mbb", &mbb, "mbb/D");
  outputTree.Branch("weight", &weight, "weight/D");

  for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
    inputTree->GetEntry(i);
    outputTree.Fill();
  }
  outputTree.Write();

  return 0;
}
