#include <iostream>
#include <string>
#include <memory>
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TVector3.h"
#include "TRandom3.h"

using namespace std;

int main(int /* argc */, char* /* argv */[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

  const std::string inputFileName =
    "/nfs/dust/cms/user/chayanit/2016Data_05_01_2017/TripleBTagReverseSelection_80X_lowMTrigger_Run2016BtoH-23Sep2016-v2_05_01_2017.root";

  const std::string outputFileName =
    cmsswBase+"/src/Analysis/BackgroundModel/data/2016DataRereco_05_01_2017/TripleBTagReverseSelectionBtoH2016_prescale_dR_13TeV_G4.root";  
  
  TFile input(inputFileName.c_str(), "read");
  std::unique_ptr<TTree> inputTree(static_cast<TTree*>(input.Get("MssmHbb")));

  TFile output(outputFileName.c_str(), "recreate");
  //TTree outputTree("MssmHbb_13TeV", "MssmHbb_13TeV");

  // could think of using float, but better keeping it in agreement with input:
  double mbb,dEtaFS,dPhiFS;
  double weight = 1.0;  // make this maybe later a product of some other weights
  
  //TRandom3 *r = new TRandom3(0); //seed 0 or 4357 default
  //double val = 1./9.;
  TF1 *f1 = new TF1("f1","[0]*x*x*x+[1]*x*x+[2]*x+[3]",1,3.5);
  f1->SetParameter(0,-7.19293e-02);
  f1->SetParameter(1,4.53292e-01);
  f1->SetParameter(2,-8.19299e-01);
  f1->SetParameter(3,1.33441e+00);

  inputTree->SetBranchAddress("ObjM12", &mbb);
  inputTree->SetBranchAddress("dEtaFS", &dEtaFS);
  inputTree->SetBranchAddress("dPhiFS", &dPhiFS);

  //outputTree.Branch("mbb", &mbb, "mbb/D");
  //outputTree.Branch("weight", &weight, "weight/D");
  TH1D *Mbb = new TH1D("mbb","mbb;M_{12} [GeV];Events/20 GeV",60,0.,1200.);

  for (int i = 0; i < inputTree->GetEntriesFast(); ++i) {
    inputTree->GetEntry(i);

    double dR = sqrt(dEtaFS*dEtaFS+dPhiFS*dPhiFS);
    weight = f1->Eval(dR);
    //if(weight > 1.) weight = 1.;

    //double random = r->Rndm(); 

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
    //if(random > 8*val) {

	Mbb->Fill(mbb,weight);
 
    	//if(mbb > 0.)	outputTree.Fill();
    	//if(mbb >= 200. && mbb <= 650.) outputTree.Fill();
    	//if(mbb >= 350 && mbb <= 1190.) outputTree.Fill();
    	//if(mbb >= 500 && mbb <= 1700.)  outputTree.Fill();
    //}
  }
  //outputTree.Write();
  output.cd();
  Mbb->Write();
  output.Close();
  return 0;
}
