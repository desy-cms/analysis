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

void CompareSigmaXBr(const vector<int>&, const vector<double>&, const vector<double>&, const string&, const string&);
double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta);
double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A);
void DrawGraphs( vector<TGraph>& graphsMSSM, vector<TGraph>& graphs2HDM, const vector<double> &tanBetas, const double& sinB_A);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&);

string boson = "H";

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	boson = "A";
	string thdm_production = "production_cosB_A_-1_1_tanB_0p5-100_COMBINATION"; //production_corseBins_cosB_A_-1_1_tanB_1-100 //
	string thdm_type = "type3";
	string thdm_fname = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/" + thdm_production + "/rootFiles/Histograms3D_" + thdm_type + "_mA_mH.root";
	TFile f(thdm_fname.c_str(),"read");
//	Define mass points:
	vector<int> mAs{300,350,400,500,600,700,900,1100};
//	Define tnaBeta points:
	vector<double> tanBetas{10,20,30,40,50,60};
//	Define sin(beta-alpha) points:
//	vector<double> sinB_As{-0.95,-0.8,-0.5,-0.3,0.,0.3,0.5,0.8,0.95};
	vector<double> sinB_As{-0.82,-0.41,-0.223634,0.,0.223634,0.41,0.82};
//	MSSM benchm,ark file
	string mssm_bench = cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root";
	CompareSigmaXBr(mAs,tanBetas,sinB_As,thdm_fname,mssm_bench);
	return 0;
}

void CompareSigmaXBr(const vector<int> &mAs, const vector<double> &tanBetas, const vector<double> &sinB_As, const string &thdm_fname, const string& mssm_benchmark){
	//	File for 2HDM scans:
	map<string,TH3F*> thdm_map;
	TFile f(thdm_fname.c_str(),"read");
	GetTHDMHistos(f,thdm_map);
	// Setup mssm_xs_tools:
	mssm_xs_tools my(mssm_benchmark.c_str(),true,0);
//	Start mA, tan(beta) and sin(beta-alpha) loops:
	for(const auto& sinB_A : sinB_As){
		// Prepare output TGraphs
		vector<TGraph> graphsMSSM(tanBetas.size());
		vector<TGraph> graphs2HDM(tanBetas.size());
		int i = 0;
		for(const auto& tanBeta : tanBetas){
			TGraph& gr_mssm = graphsMSSM.at(i);
			TGraph& gr_2hdm = graphs2HDM.at(i);
//			Set number of points for TGraph
			gr_mssm.Set((int)mAs.size());
			gr_2hdm.Set((int)mAs.size());
			++i;
			int j =0;
			for(const auto& mA : mAs){
				//Calcualte SigmaBr and fill TGraphs:
				gr_mssm.SetPoint(j,mA,MssmBr(my,mA,tanBeta));
				gr_2hdm.SetPoint(j,mA,THDMBr(thdm_map,mA,tanBeta,sinB_A));
				++j;
			}
//			string graph_name = "mssm_vs_2hdm_sinB_A_" +  to_string(sinB_A) + "_tanBeta_" + to_string(tanBeta);
//			DrawGraphs(gr_mssm,gr_2hdm,graph_name);
		}
		DrawGraphs(graphsMSSM,graphs2HDM,tanBetas,sinB_A);
	}
}

void DrawGraphs( vector<TGraph>& graphsMSSM, vector<TGraph>& graphs2HDM, const vector<double> &tanBetas, const double& sinB_A){
	// setup canvas
	TCanvas can("can","can",800,600);
	can.SetGrid();
	TLegend leg(0.6,0.6,0.9,0.9);
//	leg.SetHeader(("2HDM(sin(#beta-#alpha) = " + to_string_with_precision(sinB_A,2) + ") vs MSSM(m_h^{mod+}, #mu = 200)").c_str());
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	// Loop over the tanBetas
	for (unsigned int i = 0; i != tanBetas.size(); ++i){
		TGraph& mssm = graphsMSSM.at(i);
		TGraph& thdm = graphs2HDM.at(i);
		auto tanBeta = tanBetas.at(i);

		//Cosmetics:
		mssm.SetLineColor(colours.at(i));
		mssm.SetLineWidth(2.);

		thdm.SetMarkerStyle(20 + i);
		thdm.SetMarkerSize(1.1);
		thdm.SetMarkerColor(colours.at(i));
		thdm.SetTitle( ("2HDM(cos(#beta-#alpha) = " + to_string_with_precision(sinB_A,3) + ") vs MSSM(m_{h}^{mod+}, #mu = 200)" + ";M_{12}, GeV; #sigma x BR [pb]").c_str());

		leg.AddEntry(&mssm,("MSSM, tan#beta = " + to_string_with_precision(tanBeta,2)).c_str(),"al");
		leg.AddEntry(&thdm,("2HDM, tan#beta = " + to_string_with_precision(tanBeta,2)).c_str(),"p");

		if(i==0){
			thdm.Draw("AP");
		}
		else thdm.Draw("Psame");
		mssm.Draw("Csame");
		if(i==0) thdm.GetYaxis()->SetRangeUser(0.0001,150);
		can.SetLogy();
		can.Update();
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/2HDM/" + boson + "_thdm_vs_mssm_" + to_string_with_precision(sinB_A,3) + ".pdf").c_str() );
}

//void CompareSigmaXBr(const vector<int> &mAs, const vector<double> &tanBetas, const vector<double> &sinB_As, const string &thdm_fname, const string& mssm_benchmark){
//	//	File for 2HDM scans:
//	map<string,TH3F*> thdm_map;
//	TFile f(thdm_fname.c_str(),"read");
//	GetTHDMHistos(f,thdm_map);
//	// Setup mssm_xs_tools:
//	mssm_xs_tools my(mssm_benchmark.c_str(),true,0);
////	Start mA, tan(beta) and sin(beta-alpha) loops:
//	for(const auto& sinB_A : sinB_As){
//		// Prepare output TGraphs
//		vector<TGraph> graphsMSSM(tanBetas.size());
//		vector<TGraph> graphs2HDM(tanBetas.size());
//		int i = 0;
//		for(const auto& tanBeta : tanBetas){
//			TGraph& gr_mssm = graphsMSSM.at(i);
//			TGraph& gr_2hdm = graphs2HDM.at(i);
////			Set number of points for TGraph
//			gr_mssm.Set((int)mAs.size());
//			gr_2hdm.Set((int)mAs.size());
//			++i;
//			int j =0;
//			for(const auto& mA : mAs){
//				++j;
//				//Calcualte SigmaBr and fill TGraphs:
//				gr_mssm.SetPoint(j,mA,MssmSigmaBr(my,mA,tanBeta));
//				gr_2hdm.SetPoint(j,mA,THDMSigmaBr(thdm_map,mA,tanBeta,sinB_A));
//			}
//			string graph_name = "mssm_vs_2hdm_sinB_A_" +  to_string(sinB_A) + "_tanBeta_" + to_string(tanBeta);
//			DrawGraphs(gr_mssm,gr_2hdm,graph_name);
//		}
//	}
//}
//
//void DrawGraphs( TGraph& mssm, TGraph& thdm, const string& plot_name){
//	// setup canvas
//	TCanvas can("can","can",800,600);
//
//	// some cosmetics:
//	mssm.SetTitle(";M_{12}, GeV; #sigma x BR [pb]");
//	mssm.SetMarkerSize(1.1);
//	mssm.SetMarkerStyle(20);
//	thdm.SetMarkerStyle(21);
//	thdm.SetMarkerSize(1.1);
//	thdm.SetMarkerColor(kRed);
//	// Draw
//	mssm.Draw("AP");
//	thdm.Draw("P");
//	// set maximum:
//	double max = 0, min = 0;
//	if(mssm.GetYaxis()->GetXmax() > thdm.GetYaxis()->GetXmax()) max = mssm.GetYaxis()->GetXmax() + 0.1 * mssm.GetYaxis()->GetXmax();
//	else max = thdm.GetYaxis()->GetXmax() + 0.1 * thdm.GetYaxis()->GetXmax();
//	// set minimum:
//	if(mssm.GetYaxis()->GetXmin() < thdm.GetYaxis()->GetXmin()) min = mssm.GetYaxis()->GetXmin() - 0.1 * mssm.GetYaxis()->GetXmin();
//	else min = thdm.GetYaxis()->GetXmin() - 0.1 * thdm.GetYaxis()->GetXmin();
//	if(min == 0) min += 1e-04;
//	mssm.GetYaxis()->SetRangeUser(min,max);
//	mssm.Draw("AP");
//	thdm.Draw("P");
//	can.SetLogy();
//	can.Update();
//	// Legend
//	TLegend leg(0.6,0.6,0.9,0.85);
//	leg.AddEntry(&mssm,"MSSM","p");
//	leg.AddEntry(&thdm,"2HDM","p");
//	leg.Draw();
//
//	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/" + plot_name + ".pdf").c_str() );
//}

double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta) {
	double totXSec = -1, BrAbb, sigmaBBA, sigmaBBH, BrHbb;
	if(boson == "A"){
		//bbHSantander_A vs bbH5F_A
		sigmaBBA = my.bbH5F_A(mA,tanBeta);
		BrAbb = my.br_Abb(mA,tanBeta);
		totXSec = sigmaBBA*BrAbb;
	}
	else if (boson == "H"){
		sigmaBBH = my.bbH5F_H(mA,tanBeta);
		BrHbb = my.br_Hbb(mA,tanBeta);
		totXSec = sigmaBBH*BrHbb;
	}
	else if (boson == "both"){
		sigmaBBA = my.bbH5F_A(mA,tanBeta);
		BrAbb = my.br_Abb(mA,tanBeta);

		sigmaBBH = my.bbH5F_H(mA,tanBeta);
		BrHbb = my.br_Hbb(mA,tanBeta);

		totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
	}

    return totXSec;
}

double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A){
	double totXSec, BrAbb, sigmaBBA, sigmaBBH, BrHbb;
	if(boson != "both"){
		sigmaBBA = f["xs_bb" + boson]		->Interpolate(mA,tanBeta,sinB_A);
		BrAbb 	= f["br_" + boson + "bb"]	->Interpolate(mA,tanBeta,sinB_A);
		totXSec = sigmaBBA*BrAbb;
	}
	else {
		sigmaBBA = f["xs_bbA"]->Interpolate(mA,tanBeta,sinB_A);
		sigmaBBH = f["xs_bbH"]->Interpolate(mA,tanBeta,sinB_A);

		BrAbb = f["br_Abb"]->Interpolate(mA,tanBeta,sinB_A);
		BrHbb = f["br_Hbb"]->Interpolate(mA,tanBeta,sinB_A);

		totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
	}

	return totXSec;
}

void GetTHDMHistos(TFile& f, map<string,TH3F*>& histos){
	CheckZombie(f);
	if(boson != "both"){
		CheckZombieObjectInTFile(f,"xs_bb" + boson);
		histos["xs_bb" + boson] = (TH3F*) f.Get(("xs_bb" + boson).c_str());

		CheckZombieObjectInTFile(f,"br_" + boson + "bb");
		histos["br_" + boson + "bb"] = (TH3F*) f.Get(("br_" + boson + "bb").c_str());
	}
	else{
		CheckZombieObjectInTFile(f,"xs_bbA");
		histos["xs_bbA"] = (TH3F*) f.Get("xs_bbA");
		CheckZombieObjectInTFile(f,"br_Abb");
		histos["br_Abb"] = (TH3F*) f.Get("br_Abb");

		CheckZombieObjectInTFile(f,"xs_bbH");
		histos["xs_bbH"] = (TH3F*) f.Get("xs_bbH");
		CheckZombieObjectInTFile(f,"br_Hbb");
		histos["br_Hbb"] = (TH3F*) f.Get("br_Hbb");
	}

}
