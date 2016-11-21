// Script to fit differnet topologies
// and compare fit results, to
// estimate the knowledge of bg shape

#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <vector>

#include <TFile.h>
#include <TH1.h>
#include <TLegend.h>
#include <RooFit.h>

#include "Analysis/BackgroundModel/interface/FitContainer.h"

using namespace std;
namespace ab = analysis::backgroundmodel;

typedef unique_ptr<TH1D> pTH1D;
typedef unique_ptr<TFile> pTFile;
typedef unique_ptr<RooFitResult> pRooFitResult;

struct Topology{
	Topology(const string& name,  const string& description = "", const string& path = ""): name(name), path(path), description(description) {};
	const string name;
	const string path;
	const string description;
};

struct MyFitWorkspace : Topology{
	MyFitWorkspace(const string& name, const RooWorkspace& container)
	: Topology(name), fit(container) {};
	MyFitWorkspace(const Topology& top, const RooWorkspace& container)
	: Topology(top), fit(container) {};
	RooWorkspace fit;
};

void print(const string& );
unique_ptr<TFile> openRootFile(const string& name);
pTH1D getTH1Object(const string& file_name, const string& hist_name);
void FitModel(ab::FitContainer & );
void CompareFitparameters(const auto& );
void makeParCompPlot(const auto& par, const auto& fits);
void plotFitCurves(const auto& fits);

int main(){
	string hist_name = "general/diJet_m";
	vector<Topology> topologies{
		Topology("bb","Madgraph_MC","/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/DataMC_lowM_QCD_rew2b_scaled.root"),
		Topology("bb","Data","/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/DataMC_lowM_Run2015D-16Dec2015-v1.root"),
		Topology("bbnb","Madgraph_MC","/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/bbx_lowM_QCD_rew2b_scaled.root"),
		Topology("bbnb","Data","/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/bbx_lowM_Run2015D-16Dec2015-v1.root"),
		Topology("bbb","Madgraph_MC","/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/DataMC_3b_lowM_QCD_rew2b_scaled.root")
	};
	vector<MyFitWorkspace> vec;

	for(const auto& topology : topologies){
		pTH1D h = getTH1Object(topology.path,hist_name);
//		h->Rebin(2);
		ab::FitContainer container(h.get(),"../plots/fit_results_" + topology.name + "_" + topology.description,"background");
		FitModel(container);
		vec.push_back(MyFitWorkspace(topology,container.getWorkspace()));
	}
	CompareFitparameters(vec);


//		Create FitContainer

////		Fit model:

//		fits.push_back(MyFitResult("name",container.getNormChi2BkgOnly(),*fit.get()));

	return 0;
}

void CompareFitparameters(const auto& fits){
//	Compare par-by-par
	array<string,3> pars{{"peak","width","tail"} };
	for(const auto& par : pars){
		print("***********Parameter " + par + "************\n");
		for(const auto& fit : fits){
			print(fit.name + " " + fit.description + "\n");
			( fit.fit).var(par.c_str())->Print();
		}
		makeParCompPlot(par,fits);
	}
	plotFitCurves(fits);

}

void makeParCompPlot(const auto& par, const auto& fits){
	TCanvas can(("can_" + par).c_str(),("can_" + par).c_str(),1000,800);
	TH1D h(("h_"+par).c_str(),("h_"+par).c_str(),fits.size(),0,fits.size());
	h.SetCanExtend(TH1::kAllAxes);
	h.SetStats(0);
	h.SetMarkerColor(kBlack);
	h.SetMarkerStyle(21);
	h.SetMarkerSize(1.1);
	int i = 1;
	h.GetYaxis()->SetTitle(par.c_str());
	for(const auto& fit: fits){
		h.GetXaxis()->SetBinLabel(i,("#splitline{" + fit.name + "}{"  + fit.description + "}").c_str());
		h.SetBinContent(i,fit.fit.var(par.c_str())->getValV());
		h.SetBinError(i,fit.fit.var(par.c_str())->getError());
		++i;
	}
//	if(par == "peak") h.GetYaxis()->SetRangeUser(0,270.);
//	else if (par == "width") h.GetYaxis()->SetRangeUser(0.,65.);
//	else if (par == "tail") h.GetYaxis()->SetRangeUser(-1,1);
	h.Draw("E");
	can.Print(("../plots/"+par+".pdf").c_str());
}

void plotFitCurves(const auto& fits){
	TCanvas can(("can_" + to_string(fits.size()) + "fits").c_str(),("can_" + to_string(fits.size()) + "fits").c_str(),1000,800);
	TLegend leg(0.6,0.6,0.9,0.85);
//	colors:
	vector<int> v{1,2,3,4,6,7,94};

//	Construct frame
	RooRealVar& x = *fits.at(0).fit.var("mbb");
	RooPlot& frame = *x.frame();
//	frame.GetYaxis()->SetRangeUser(1e04,0.1);

	int i = 0;
	for(const auto& fit: fits){
		RooAbsPdf& pdf = *fit.fit.pdf("background");
		if(fit.name == "bbb") {
			fit.fit.var("tail")->setVal(-0.777574);
//			fit.fit.var("width")->setVal(55.1822);
//			fit.fit.var("peak")->setVal(255.135);
		}
		pdf.plotOn(&frame,RooFit::LineColor(v.at(i)),RooFit::LineWidth(1.1),RooFit::Name(fit.name.c_str()));
		leg.AddEntry(frame.findObject(fit.name.c_str()), (fit.name + fit.description).c_str(),"l" );
		++i;
	}
	frame.Draw();
	leg.Draw();
	can.Print( ("../plots/fit_curves_" + to_string(fits.size()) + ".pdf").c_str() );
	gPad->SetLogy();
	can.Print( ("../plots/fit_curves_" + to_string(fits.size()) + "_log.pdf").c_str() );

}

pTH1D getTH1Object(const string& file_name, const string& hist_name){
	//	Get root TFile
	pTFile f = openRootFile(file_name);
	//	Get histogram from TFile
	pTH1D h = std::unique_ptr<TH1D>(static_cast<TH1D*>(f->Get(hist_name.c_str())));
	h->SetDirectory(0);
	if(!h) throw invalid_argument("ERROR in fitM12Shape: Wrong histo name " + hist_name + " in TFile: " + file_name);
	return h;
}

void FitModel(ab::FitContainer & container){
	string model = "novopsprod";
	container.fitRangeMin(238.);
	container.fitRangeMax(1700.);
	container.setModel(ab::FitContainer::Type::background,model);
	pRooFitResult r = container.backgroundOnlyFit(model);
}


unique_ptr<TFile> openRootFile(const string& name){
	unique_ptr<TFile> file = unique_ptr<TFile>(TFile::Open(name.c_str(),"READ"));
	if(!file) throw invalid_argument("ERROR in openRootFile: Wrong file name " + name);
	return file;
}

void print(const string& s){
	cout<<s;
}
