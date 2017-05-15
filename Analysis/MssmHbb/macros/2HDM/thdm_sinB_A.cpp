#include <TH3F.h>
#include <TH3.h>
#include <TGraph.h>
#include <TFile.h>
#include "TSystem.h"
#include <TCanvas.h>
#include <TLegend.h>


#include <string>
#include <vector>

// xsection estimation for MSSM
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"
// Tools
#include "Analysis/MssmHbb/interface/utilLib.h"

using namespace std;

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

void CompareBrs(const vector<double>&, const vector<double>&, const vector<double>&, const string&, const string&);
double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta);
double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A);
void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<double> &tanBetas, const double& sinB_A);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&);

string boson = "A";

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	boson = "H";
	string thdm_fname = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/production_corseBins_cosB_A_-1_1_tanB_1-100/rootFiles/Histograms3D_type3_mA_mH.root";
	TFile f(thdm_fname.c_str(),"read");
//	Define mass points:
	vector<double> mAs{300,400,500,600,700,900,1100};
//	Define tnaBeta points:
	vector<double> tanBetas{10,20,30,40,50,60};
//	Define sin(beta-alpha) points:
	vector<double> sinB_As;
	for(double i = 0.805; i< 1.0; i+=0.005) sinB_As.push_back(i);
//	MSSM benchm,ark file
	string mssm_bench = cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root";
	CompareBrs(mAs,tanBetas,sinB_As,thdm_fname,mssm_bench);
	return 0;
}

void CompareBrs(const vector<double> &mAs, const vector<double> &tanBetas, const vector<double> &sinB_As, const string &thdm_fname, const string& mssm_benchmark){
	//	File for 2HDM scans:
	map<string,TH3F*> thdm_map;
	TFile f(thdm_fname.c_str(),"read");
	GetTHDMHistos(f,thdm_map);
	// Setup mssm_xs_tools:
	mssm_xs_tools my(mssm_benchmark.c_str(),true,0);
//	Start mA, tan(beta) and sin(beta-alpha) loops:
	for(const auto& mA : mAs){
		// Prepare output TGraphs
		vector<TGraph> graphsMSSM(tanBetas.size());
		vector<TGraph> graphs2HDM(tanBetas.size());
		int i = 0;
		for(const auto& tanBeta : tanBetas){
			TGraph& gr_mssm = graphsMSSM.at(i);
			TGraph& gr_2hdm = graphs2HDM.at(i);
//			Set number of points for TGraph
			gr_mssm.Set((int)sinB_As.size());
			gr_2hdm.Set((int)sinB_As.size());
			++i;
			int j =0;
			for(const auto& sinB_A : sinB_As){
				++j;
				//Calcualte SigmaBr and fill TGraphs:
				gr_2hdm.SetPoint(j,sinB_A,THDMBr(thdm_map,mA,tanBeta,sinB_A));
			}
		}
		DrawGraphs(graphs2HDM,tanBetas,mA);
	}
}

void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<double> &tanBetas, const double& mA){
	// setup canvas
	TCanvas can("can","can",800,600);
	can.SetGrid();
	double xmin, xmax,ymin,ymax;
	xmin = 0.6;xmax = 0.9;
	if(mA<800){
		ymin = 0.15; ymax = 0.4;
	}
	else {
		ymin = 0.65; ymax = 0.9;
	}
	TLegend leg(xmin,ymin,xmax,ymax);
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	// Loop over the tanBetas
	for (unsigned int i = 0; i != tanBetas.size(); ++i){
		TGraph& thdm = graphs2HDM.at(i);
		auto tanBeta = tanBetas.at(i);

		//Cosmetics:
		thdm.SetMarkerStyle(20 + i);
		thdm.SetMarkerSize(1.02);
		thdm.SetMarkerColor(colours.at(i));
		thdm.SetTitle( ("2HDM(m_{A} = " + to_string_with_precision(mA,4) + ")" + ";sin(#beta - #alpha); #sigma x BR [pb]").c_str());

		leg.AddEntry(&thdm,("2HDM, tan#beta = " + to_string_with_precision(tanBeta,2)).c_str(),"p");

		if(i==0){
			thdm.Draw("AP");
		}
		else thdm.Draw("Psame");
		if(i==0) thdm.GetYaxis()->SetRangeUser(0.00001,150);
		if(i==0) thdm.GetXaxis()->SetRangeUser(0.8,1.);
		can.SetLogy();
		can.Update();
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/thdm_vs_mssm_sinB_A_" + to_string_with_precision(mA,4) + ".pdf").c_str() );
}

double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta) {
	double sigmaBBA = my.bbHSantander_A(mA,tanBeta);
//    double sigmaBBH = my.bbHSantander_H(mA,tanBeta);

    double BrAbb = my.br_Abb(mA,tanBeta);
//    double BrHbb = my.br_Hbb(mA,tanBeta);

    double totXSec = sigmaBBA*BrAbb;// + sigmaBBH*BrHbb;

    return totXSec;
}

double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A){
	double sigmaBBA = f["xs_bbA"]->Interpolate(mA,tanBeta,sinB_A);
//	double sigmaBBH = f["xs_bbH"]->Interpolate(mA,tanBeta,sinB_A);

	double BrAbb = f["br_Abb"]->Interpolate(mA,tanBeta,sinB_A);
//	double BrHbb = f["br_Hbb"]->Interpolate(mA,tanBeta,sinB_A);

	double totXSec = sigmaBBA*BrAbb;// + sigmaBBH*BrHbb;

	return totXSec;
}

void GetTHDMHistos(TFile& f, map<string,TH3F*>& histos){
	CheckZombie(f);
	CheckZombieObjectInTFile(f,"xs_bbA");
	histos["xs_bbA"] = (TH3F*) f.Get("xs_bbA");
//	CheckZombieObjectInTFile(f,"xs_bbH");
//	histos["xs_bbH"] = (TH3F*) f.Get("xs_bbH");
	CheckZombieObjectInTFile(f,"br_Abb");
	histos["br_Abb"] = (TH3F*) f.Get("br_Abb");
//	CheckZombieObjectInTFile(f,"br_Hbb");
//	histos["br_Abb"] = (TH3F*) f.Get("br_Hbb");
}
