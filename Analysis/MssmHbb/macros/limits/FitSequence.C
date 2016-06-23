#include "FitMassDistribution.C"
void FitSequence(TString trigScenario = "lowM") {

  TFile * file = new TFile("SignalPDFs_"+trigScenario+".root","recreate");

  TString mH[14] = {"100","120","160","200","250","300","350","400",
		    "500","600","700","900","1100","1300"};


  TString sys[2] = {"CMS_PtEff_13TeV","CMS_JES_13TeV"};
  TString dir[2] = {"Up","Down"};

  int iStart = 1;
  if (trigScenario=="highM")
    iStart = 4;

  for (int i=iStart; i<14; ++i) {
    int iopt = 2;
    int rebin = 1;
    if (i<8) iopt = 1;
    if (trigScenario=="highM") {
      if (i<6) rebin = 2;
    }
    if (trigScenario=="lowM") {
      if (i>8&&i<12) rebin = 4;
      if (i>11) rebin = 2;
    }
    TString fileName = "MssmHbbSignal_"+trigScenario+"_SUSYGluGluToBBHToBB_M-"+mH[i]+"_TuneCUETP8M1_13TeV-pythia8.root";
    TFile * fileInput = new TFile(fileName);
    std::cout << "input file : " << fileInput << std::endl;
    TString histName = "bbH_Mbb";
    if (trigScenario=="highM") histName = "Mbb";
    TH1D * histCentral = (TH1D*)fileInput->Get(histName);
    std::cout << "Central hist : " << histCentral << std::endl;
    TF1 * funcCentral = (TF1*)FitMass(histCentral,"Mbb_"+mH[i]+"_"+trigScenario,iopt,rebin);
    file->cd("");
    funcCentral->Write("Mbb_"+mH[i]);
    for (int iSys=0; iSys<2; ++iSys) {
      for (int idir=0; idir<2; ++idir) {
	histName = "bbH_Mbb_"+sys[iSys]+dir[idir];
	TH1D * histSys = (TH1D*)fileInput->Get(histName);
	std::cout << histName << " : " << histSys << std::endl;
	TF1 * funcSys = (TF1*)FitMass(histSys,"Mbb_"+mH[i]+"_"+trigScenario+"_"+sys[iSys]+dir[idir],iopt,rebin);
	file->cd("");
	funcSys->Write("Mbb_"+mH[i]+"_"+sys[iSys]+dir[idir]);
      }
    }

  }

  file->Close();

}
