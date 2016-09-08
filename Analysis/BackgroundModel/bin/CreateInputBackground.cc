#include <string>
#include <memory>
#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"


int main(int argc, char* argv[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  // name of input is first argument
  int arg1 = argc-1;
//  TString inputFileName = TString(cmsswBase)+"/src/Analysis/MssmHbb/test/"+TString(argv[arg1]);
  TString inputFileName = TString(argv[arg1]);
  
  TString outputFileName = TString(inputFileName(0,inputFileName.Length()-5))+"_Background.root";
  
//  const std::string outputFileName =    cmsswBase+"/src/Analysis/BackgroundModel/data/TripleBTagReverseSelection_13TeV.root";
//    const std::string outputFileName =  "TripleBTagReverseSelection_13TeV.root";
    
  TFile input(inputFileName, "read");
  std::unique_ptr<TTree> inputTree(static_cast<TTree*>(input.Get("MssmHbb")));

  TFile output(outputFileName, "recreate");
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
