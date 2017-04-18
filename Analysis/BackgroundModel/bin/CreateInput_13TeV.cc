#include <iostream>
#include <string>
#include <memory>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TRandom3.h"

using namespace std;

int main(int /* argc */, char* /* argv */[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  const std::string inputFileName =
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_26_04_2016_unblind.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_26_04_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_06_05_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_12_05_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_14_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_15_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_lowMTrigger_Pythia8_25_04_2016.root";
    //"/nfs/dust/cms/user/chayanit/DoubleBTagSelection/DoubleBTagSelection_76X_lowMTrigger_Pythia8_16_03_2016.root";
    //"/nfs/dust/cms/user/chayanit/DoubleBTag13Selection/DoubleBTagSelection_76X_lowMTrigger_21_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/DoubleBTag23Selection/DoubleBTagSelection_76X_lowMTrigger_21_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagMMLSelection/TripleBTagMMLSelection_76X_lowMTrigger_27_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_highMTrigger_15_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagReverseSelection/TripleBTagReverseSelection_76X_highMTrigger_28_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/DoubleBTag13Selection_76X_highMTrigger_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/DoubleBTag13Selection_76X_lowMTrigger_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/DoubleBTag23Selection_76X_highMTrigger_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/DoubleBTag23Selection_76X_lowMTrigger_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/TripleBTagMMLSelection_76X_lowMTrigger_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/TripleBTagReverseSelection_76X_highMTrigger_blinded_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/TripleBTagReverseSelection_76X_highMTrigger_unblinded_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/TripleBTagReverseSelection_76X_lowMTrigger_blinded_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_29_06_2016/TripleBTagReverseSelection_76X_lowMTrigger_unblinded_29_06_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagSelection/TripleBTagSelection_76X_lowMTrigger_05_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/TripleBTagSelection/TripleBTagSelection_76X_highMTrigger_05_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_08_07_2016/DoubleBTag13Selection_76X_lowMTrigger_08_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_08_07_2016/DoubleBTag23Selection_76X_lowMTrigger_08_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_08_07_2016/TripleBTagReverseBXBSelection_76X_lowMTrigger_08_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_08_07_2016/TripleBTagReverseXBBSelection_76X_lowMTrigger_08_07_2016.root";
    //Final ntuples for pre-approval
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/DoubleBTag13Selection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/DoubleBTag13Selection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/DoubleBTag23Selection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/DoubleBTag23Selection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseSelection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagMMLSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagSelection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseBXBSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseBXBSelection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseXBBSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/UnbinnedData_13_07_2016/TripleBTagReverseXBBSelection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_13_07_2016.root";
    //"/nfs/dust/cms/user/chayanit/Optimization_18_08_2016/TripleBTagReverseSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_18_08_2016.root";
    //"/nfs/dust/cms/user/chayanit/Optimization_18_08_2016/TripleBTagSelection_76X_lowMTrigger_Run2015C_25ns-16Dec2015-v1_18_08_2016.root";
    //"/nfs/dust/cms/user/chayanit/Optimization_18_08_2016/TripleBTagReverseSelection_80X_lowMTrigger_Run2016B-PromptReco-v1_22_08_2016.root";
    //"/nfs/dust/cms/user/chayanit/Optimization_18_08_2016/TripleBTagSelection_76X_highMTrigger_Run2015C_25ns-16Dec2015-v1_18_08_2016.root";
    //"/nfs/dust/cms/user/chayanit/2016Data_15_09_2016/TripleBTagReverseSelection_80X_lowMTrigger_Run2016B-PromptReco-v1_15_09_2016.root";
    //"/nfs/dust/cms/user/chayanit/Optimization_18_08_2016/TripleBTagSelection_80X_lowMTrigger_Run2016B-PromptReco-v1_25_08_2016.root";
    //"/nfs/dust/cms/user/chayanit/2016Data_24_11_2016/TripleBTagReverseSelection_80X_lowMTrigger_Run2016BtoH-23Sep2016-v2_25_11_2016.root";
    //"/nfs/dust/cms/user/chayanit/2016Data_24_11_2016/TripleBTagSelection_80X_lowMTrigger_Run2016BtoH-23Sep2016-v2_25_11_2016.root";
    "/nfs/dust/cms/user/chayanit/2016Data_05_01_2017/TripleBTagReverseSelection_80X_lowMTrigger_Run2016BtoH-23Sep2016-v2_05_01_2017.root";
    //"/nfs/dust/cms/user/chayanit/2016Data_05_01_2017/TripleBTagSelection_80X_lowMTrigger_Run2016BtoH-23Sep2016-v2_05_01_2017.root";
   
  const std::string outputFileName =
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseSelection_highM_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseSelection_highM_13TeV_unblinded.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag13Selection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag13Selection_highM_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag23Selection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag23Selection_highM_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagMMLSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseSelection_13TeV_unblind.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagSelection_13TeV_240to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagSelection_13TeV_highM_350to3000.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag13Selection_13TeV_onlineBTag.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/DoubleBTag23Selection_13TeV_onlineBTag.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseBXBSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Nofilter_29_06_2016/TripleBTagReverseXBBSelection_13TeV.root";
    //Final ntuples for pre-approval 
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/DoubleBTag13Selection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/DoubleBTag13Selection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/DoubleBTag23Selection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/DoubleBTag23Selection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseSelection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagMMLSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagSelection_13TeV_240to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagSelection_13TeV_highM_350to3000.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagSelection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseBXBSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseBXBSelection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseXBBSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/FinalNtuples_13_07_2016/TripleBTagReverseXBBSelection_13TeV_highM.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Optimization_18_08_2016/TripleBTagReverseSelection_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Optimization_18_08_2016/TripleBTagSelection_13TeV_240to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Optimization_18_08_2016/TripleBTagReverseSelection2016_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/Optimization_18_08_2016/TripleBTagSelection_13TeV_350to3000.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016Data_15_09_2016/TripleBTagReverseSelection2016_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016Data_15_09_2016/TripleBTagSelectionBtoD2016_13TeV_200to650.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016Data_15_09_2016/TripleBTagSelectionBtoD2016_13TeV_350to1190.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016Data_15_09_2016/TripleBTagSelectionBtoD2016_13TeV_500to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagReverseSelectionBtoH2016_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_200to650.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_350to1190.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_500to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_320to860.root"; 
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_400to1200.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_600to1800.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagSelectionBtoH2016_13TeV_200to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagReverseSelectionBtoH2016_prescale_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_01_12_2016/TripleBTagReverseSelection2015_prescale_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_prescale_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_13TeV.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_13TeV_200to650.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagSelectionBtoH2016_13TeV_200to650.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagSelectionBtoH2016_13TeV_350to1190.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_13TeV_350to1190.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_13TeV_500to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagSelectionBtoH2016_13TeV_500to1700.root";
    //cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_oldBtagprescale_13TeV_seed0.root";
    cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_prescale_13TeV_G9.root";

    //cmsswBase+"/src/Analysis/BackgroundModel/data/TripleBTagReverseSelection_QCD_13TeV.root";
    
  
  TFile input(inputFileName.c_str(), "read");
  std::unique_ptr<TTree> inputTree(static_cast<TTree*>(input.Get("MssmHbb")));

  TFile output(outputFileName.c_str(), "recreate");
  TTree outputTree("MssmHbb_13TeV", "MssmHbb_13TeV");

  // could think of using float, but better keeping it in agreement with input:
  double mbb;
  double weight = 1.0;  // make this maybe later a product of some other weights

  TRandom3 *r = new TRandom3(0); //seed 0 or 4357 default
  double val = 1./9.;

  //std::cout<<"val = "<<val<<std::endl;
/*
  double WeightPileUp[3];
  double LumiWeight;
  double TwoDPtWeight;
  double dEtaWeight;
  double BTagWeight;
  double BTagSFcentral[20];
  double LeadPt[20],LeadEta[20],LeadPhi[20],LeadBTag[20];
  int Njets;
  TVector3 Jet1,Jet2,Jet3;
*/
  inputTree->SetBranchAddress("ObjM12", &mbb);
/*  inputTree->SetBranchAddress("WeightPileUp", WeightPileUp);
  inputTree->SetBranchAddress("BTagSFcentral", BTagSFcentral);
  inputTree->SetBranchAddress("LumiWeight", &LumiWeight);
  inputTree->SetBranchAddress("TwoDPtWeight", &TwoDPtWeight);
  inputTree->SetBranchAddress("dEtaWeight", &dEtaWeight);
  inputTree->SetBranchAddress("BTagWeight", &BTagWeight);
  inputTree->SetBranchAddress("LeadPt", LeadPt);
  inputTree->SetBranchAddress("LeadEta", LeadEta);
  inputTree->SetBranchAddress("LeadPhi", LeadPhi);
  inputTree->SetBranchAddress("LeadBTag", LeadBTag);
  inputTree->SetBranchAddress("Njets", &Njets);  
*/
  outputTree.Branch("mbb", &mbb, "mbb/D");
  outputTree.Branch("weight", &weight, "weight/D");

  for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
    inputTree->GetEntry(i);

    double random = r->Rndm(); 

    //2015
    //if(random > 0.125 && random < 1.0) continue;
    //2016 
    //if(random > 0.1428 && random < 1.0) continue; 	//old b-tagged WP
    //if(random > 1/9 && random < 1.0) continue; 	//new b-tagged WP
    //if(random < val) {
    //if(random > val && random < 2*val) {
    //if(random > 2*val && random < 3*val) {
    //if(random > 3*val && random < 4*val) {
    //if(random > 4*val && random < 5*val) {
    //if(random > 5*val && random < 6*val) {
    //if(random > 6*val && random < 7*val) {
    //if(random > 7*val && random < 8*val) {
    if(random > 8*val) {

    	if(mbb > 0.)	outputTree.Fill();
    	//if(mbb >= 200. && mbb <= 650.) outputTree.Fill();
    	//if(mbb >= 350 && mbb <= 1190.) outputTree.Fill();
    	//if(mbb >= 500 && mbb <= 1700.)  outputTree.Fill();
    }
  }
  outputTree.Write();

  return 0;
}
