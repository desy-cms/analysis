#include <vector>
#include <array>
#include <map>

#include "stdlib.h"
#include <memory>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/join.hpp>
#include "TSystem.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"

#include "TFormula.h"

#include "RooDataHist.h"
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooWorkspace.h"

#include "TFile.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"

#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/FitContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"
#include "Analysis/BackgroundModel/interface/Tools.h"
#include "Analysis/BackgroundModel/interface/ProbabilityDensityFunctions.h"
#include "Analysis/BackgroundModel/interface/RooQuadGausExp.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

// hi my name is Rostyslav and I like bananas

namespace po = boost::program_options;
namespace ab = analysis::backgroundmodel;

typedef std::unique_ptr<TFile> pTFile;

using namespace analysis::backgroundmodel;
using namespace std;

void drawSystFits(const int & mass,const std::string &syst, const std::string &output);
void plotShapeParameters(const int & mass,const vector<string> &syst, const std::string &folder);

int main(int argc, char* argv[]) {
  const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	std::map<int,std::string> signal;
	signal[700] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-700_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[900] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-900_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[1100]= "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_chayanit-SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[1300]= "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-1300_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[500] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
	signal[600] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
//	signal[200] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8.root";
	signal[250] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8.root";
	signal[300] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root";
	signal[350] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root";
	signal[400] = "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root";
/**/
  const int verbosity = 2;
  std::string histo_name = "bbH_Mbb";
  //List of shape uncertainties
  std::vector<std::string> shape_unc = {"JER","JES"};
  std::string CMS = "CMS";
  std::string energy = "13TeV";
  std::array<std::string,2> sign = { {"Up","Down"} };
  std::string model = "quadgausexp";
  for(const auto & mass : signal){
	  TFile f((cmsswBase + mass.second).c_str(),"read");
	  ab::HistContainer histContainer(cmsswBase + mass.second);//MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
	  histContainer.Rebin(1);
	  std::string name = cmsswBase + "/src/Analysis/MssmHbb/output/" + "mass_point_" + std::to_string(mass.first);
	  std::string hname = histo_name;
	  //Central value
	  TH1D* hSignal = (TH1D*) f.Get(histo_name.c_str());
          if(hSignal == nullptr) throw std::logic_error("Wrong histo name: " + histo_name + " has been provided");
          ab::FitContainer fitter(hSignal,name,"signal");
          fitter.verbosity(verbosity - 1);
          fitter.setModel(ab::FitContainer::Type::signal,model);
          std::unique_ptr<RooFitResult> Signalfit = fitter.FitSignal(model);
    	  RooRealVar signal_norm("signal_norm","signal_norm",hSignal->Integral());
    	  signal_norm.setConstant(true);
    	  fitter.Import(signal_norm);
    	  fitter.Write();
	  //Syst variation 
	  for(const auto & syst : shape_unc){
		for(const auto & s : sign){
               	hname = histo_name + "_" + CMS + "_" + syst + "_" + energy + s;
               	TH1D* hSignal = (TH1D*) f.Get(hname.c_str());
               	if(hSignal == nullptr) throw std::logic_error("Wrong histo name: " + hname + " has been provided");
               	ab::FitContainer fit_syst(hSignal,name + "/" + syst + "_" + s,"signal");
               	fit_syst.verbosity(verbosity - 1);
               	fit_syst.setModel(ab::FitContainer::Type::signal,model);
               	std::unique_ptr<RooFitResult> Signalfit_syst = fit_syst.FitSignal(model);
               	RooRealVar signal_norm("signal_norm","signal_norm",hSignal->Integral());
               	signal_norm.setConstant(true);
               	fit_syst.Import(signal_norm);
               	fit_syst.Write();
	        }
		//Draw plots
		drawSystFits(mass.first,syst,name);
	  }
	  //Plot shape parameters
	  plotShapeParameters(mass.first,shape_unc,name);
  }

  return 0;

}

void drawSystFits(const int & mass,const std::string &syst, const std::string &output){
	TFile fCentral( (output + "/workspace/FitContainer_workspace.root").c_str(),"read");
	TCanvas c("c","c",1000,800);
	RooWorkspace& wCentral = *( (RooWorkspace*) fCentral.Get("workspace"));

	//Extract RooRealVar for X-axis
	RooRealVar& x = *( wCentral.var("mbb"));
	RooPlot& frame = *(x.frame());
	RooAbsPdf& PdfCentral = *(wCentral.pdf("signal"));
	PdfCentral.plotOn(&frame,RooFit::LineColor(kBlack),RooFit::Name("Central"));

	//down:
	TFile fDown( (output + "/" + syst + "_" + "Down/workspace/FitContainer_workspace.root").c_str());
	RooWorkspace& wDown = *( (RooWorkspace*) fDown.Get("workspace"));
	RooAbsPdf& PdfDown = *(wDown.pdf("signal"));
	PdfDown.plotOn(&frame,RooFit::LineColor(kBlue),RooFit::Name("Down"));

	//up
	TFile fUp( (output + "/" + syst + "_" + "Up/workspace/FitContainer_workspace.root").c_str());
	RooWorkspace& wUp = *( (RooWorkspace*) fUp.Get("workspace"));
	RooAbsPdf& PdfUp = *(wUp.pdf("signal"));
	PdfUp.plotOn(&frame,RooFit::LineColor(kRed),RooFit::Name("Up"));

//	TPad pad1("pad1","",0.,0.35,1,1);
//	pad1.SetBottomMargin(0.0);
//	pad1.SetLeftMargin(0.16);
//	pad1.SetRightMargin(0.05);
//	pad1.Draw();
//	pad1.cd();

	TLegend leg(0.6,0.7,0.9,0.9);
	frame.Draw();
	leg.AddEntry(frame.findObject("Central"),"Central","l");
	leg.AddEntry(frame.findObject("Down"),("-2#sigma variation " + syst).c_str(),"l");
	leg.AddEntry(frame.findObject("Up"),("+2#sigma variation " + syst).c_str(),"l");
	leg.Draw();

//	c.cd();
//	TPad pad2("pad2","",0.,0.21,1,0.35);
//	pad2.SetBottomMargin(0.0);
//	pad2.SetTopMargin(0.);
//	pad2.SetLeftMargin(pad1.GetLeftMargin());
//	pad2.SetRightMargin(pad1.GetRightMargin());
//	pad2.Draw();
//	pad2.cd();

	c.Print( (output + "/plots/" + syst + ".png").c_str());
}

void plotShapeParameters(const int & mass,const vector<string> &syst, const std::string &folder){
	TFile fCentral( (folder + "/workspace/FitContainer_workspace.root").c_str(),"read");
	RooWorkspace& wCn = *( (RooWorkspace*) fCentral.Get("workspace"));

	for(const auto& s: syst){
		//down
		TFile fDown( (folder + "/" + s + "_" + "Down/workspace/FitContainer_workspace.root").c_str());
		RooWorkspace& wDn = *( (RooWorkspace*) fDown.Get("workspace"));
		//up
		TFile fUp( (folder + "/" + s + "_" + "Up/workspace/FitContainer_workspace.root").c_str());
		RooWorkspace& wUp = *( (RooWorkspace*) fUp.Get("workspace"));

		//Signal shape parameters list
		vector<string> parameters = {"signal_norm","mean","sigmaL1","sigmaR1","sigmaL2","sigmaR2","tail_shift","tail_sigma","norm_g1","norm_g2"};
		TCanvas c(("c" + s).c_str(),"c",1000,800);
		TH1F hUp("hUp",(s + ";Shape parameters;(#theta_{up/dn} - #theta_{0})/#theta_{0}").c_str(),parameters.size(),0,10);
		TH1F hDn("hDn","hDn",parameters.size(),0,10);
		hDn.SetCanExtend(TH1::kAllAxes);
		hUp.SetCanExtend(TH1::kAllAxes);
		hUp.SetStats(0);
		hUp.SetMarkerStyle(21);
		hUp.SetMarkerColor(kRed);
		hDn.SetMarkerStyle(21);
		hDn.SetMarkerColor(kBlue);
		hUp.GetYaxis()->SetRangeUser(-1.,1.);
		int i = 0;
		double e_up;
		double e_down;
		for(const auto& p : parameters){
			++i;
			double up = (wUp.var(p.c_str())->getValV() - wCn.var(p.c_str())->getValV()) / wCn.var(p.c_str())->getValV();
			if(wUp.var(p.c_str())->getError() != 0) e_up = wUp.var(p.c_str())->getValV()/wCn.var(p.c_str())->getValV() * sqrt( pow(wUp.var(p.c_str())->getValV()/wUp.var(p.c_str())->getError(),2) + pow(wCn.var(p.c_str())->getValV()/wCn.var(p.c_str())->getError(),2) );
			else e_up = 0;
			double down = (wDn.var(p.c_str())->getValV() - wCn.var(p.c_str())->getValV()) /wCn.var(p.c_str())->getValV();
			if(wDn.var(p.c_str())->getError() != 0) e_down = wDn.var(p.c_str())->getValV()/wCn.var(p.c_str())->getValV() * sqrt( pow(wDn.var(p.c_str())->getValV()/wDn.var(p.c_str())->getError(),2) + pow(wCn.var(p.c_str())->getValV()/wCn.var(p.c_str())->getError(),2) );
			else e_down = 0;
			cout<<p<<" val: "<<up<<" +/- "<<e_up<<" "<<down<<" +/- "<<e_down<<endl;
			hUp.GetXaxis()->SetBinLabel(i,p.c_str());
			hUp.SetBinContent(i,up);
			hUp.SetBinError(i,e_up);
			hDn.SetBinContent(i,down);
			hDn.SetBinError(i,e_down);
		}
		hUp.Draw("E");
		hDn.Draw("Esame");
		TLegend leg(0.65,0.6,0.85,0.8);
		leg.SetLineColor(0);
		leg.AddEntry(&hUp,"Up","p");
		leg.AddEntry(&hDn,"Down","p");
		leg.Draw();

		c.Print( (folder + "/plots/delta" + s + ".pdf").c_str() );
	}

}
