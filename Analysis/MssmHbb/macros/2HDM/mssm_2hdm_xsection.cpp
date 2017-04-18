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

namespace mssm_2hdm_xsec{

void CompareXss(const vector<double>&, const vector<double>&, const double& , const vector<string>&, const vector<string>&, const string&, const string&);
double MssmXs(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta,
			 const string& name);
double THDMXs(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A,
				   const string& name);
void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &Xss, const double& sinB_A, const string& model = "thdm");
void DrawGraphs(vector<TGraph>& graphs2HDM, vector<TGraph>& graphsMSSM, const vector<string> &Xss, const double& sinB_A);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&);
}

string boson = "H";
string cB_A = "0.";
using namespace mssm_2hdm_xsec;

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	string thdm_production = "production_cosB_A_-1_1_tanB_0p5-100_COMBINATION"; //production_corseBins_cosB_A_-1_1_tanB_1-100 //
	string thdm_type = "type3";
	string thdm_fname = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/" + thdm_production + "/rootFiles/Histograms3D_" + thdm_type + "_mA_mH.root";
	TFile f(thdm_fname.c_str(),"read");
//	Define mass points:
	vector<double> mAs{300,400,500,600,700,900,1100};
//	Define tnaBeta points:
	vector<double> tanBetas;
	for(double i = 10; i <= 60; i+=0.5) tanBetas.push_back((double)i);
//	Define sin(beta-alpha) points:
	vector<double> sinB_As{-0.95,-0.8,-0.5,-0.3,0.,0.3,0.5,0.8,0.95};
//	Define types of Xss:
	vector<string> Xss = {"xs_bb" + boson};
	vector<string> Xss_mssm = {"bb5F->" + boson};

//	MSSM benchm,ark file
	string mssm_bench = cmsswBase + "/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root";
	CompareXss(mAs, tanBetas, stod(cB_A),Xss, Xss_mssm, thdm_fname, mssm_bench);
	return 0;
}

namespace mssm_2hdm_xsec{

void CompareXss(const vector<double> &mAs, const vector<double> &tanBetas, const double&sinB_A, const vector<string>& Xss, const vector<string>& Xss_mssm,  const string &thdm_fname,const string &mssm_benchmark){
	//	File for 2HDM scans:
	map<string,TH3F*> thdm_map;
	TFile f(thdm_fname.c_str(),"read");
	GetTHDMHistos(f,thdm_map);
	// Setup mssm_xs_tools:
	mssm_xs_tools my(mssm_benchmark.c_str(),true,0);
	// Setup mssm_xs_tools:
//	Start mA, tan(beta) and sin(beta-alpha) loops:
	for(const auto& mA : mAs){
		// Prepare output TGraphs
		vector<TGraph> graphs2HDM(Xss.size());
		vector<TGraph> graphsMSSM(Xss_mssm.size());
		int i = 0, j =0;
		for(const auto& Xs : Xss){
			TGraph& gr_2hdm = graphs2HDM.at(i);
//			Set number of points for TGraph
			gr_2hdm.Set((int)tanBetas.size());
			++i;
			j = 0;
			for(const auto& tanBeta : tanBetas){
				++j;
				//Calcualte SigmaXs and fill TGraphs:
//				gr_mssm.SetPoint(j,tanBeta,MssmXs(my,mA,tanBeta));
				gr_2hdm.SetPoint(j,tanBeta,THDMXs(thdm_map,mA,tanBeta,sinB_A,Xs));
			}
		}
		//MSSM part
		i = 0;
		j = 0;
		for (const auto& Xs : Xss_mssm){
			TGraph& gr_mssm = graphsMSSM.at(i);
			gr_mssm.Set((int)tanBetas.size());
			++i;
			j=0;
			for(const auto& tanBeta : tanBetas){
				gr_mssm.SetPoint(j,tanBeta,MssmXs(my,mA,tanBeta,Xs));
				++j;
			}
		}

		cout<<"mA = "<<mA<<endl;
//		DrawGraphs(graphs2HDM,Xss,mA,"2HDM");
//		DrawGraphs(graphsMSSM,Xss_mssm,mA,"MSSM");
		DrawGraphs(graphs2HDM,graphsMSSM,Xss,mA);
	}
}

void DrawGraphs(vector<TGraph>& graphs2HDM, vector<TGraph>& graphsMSSM, const vector<string> &Xss, const double& mA){
	TCanvas can("can","can",800,600);
	can.SetGrid();
	double xmin, xmax,ymin,ymax;
	xmin = 0.6;xmax = 0.9;
	ymin = 0.15; ymax = 0.4;
	TLegend leg(xmin,ymin,xmax,ymax);
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	for (unsigned int i = 0; i != Xss.size(); ++i){
		TGraph& thdm = graphs2HDM.at(i);
		TGraph& mssm = graphsMSSM.at(i);

		auto Xs = Xss.at(i);

		//Cosmetics:
		mssm.SetLineColor(colours.at(i));
		mssm.SetLineWidth(2.);

		thdm.SetMarkerStyle(20 + i);
		thdm.SetMarkerSize(1.1);
		thdm.SetMarkerColor(colours.at(i));
		thdm.SetTitle( ("2HDM(cos(#beta-#alpha) = " + cB_A + ") vs MSSM(m_{h}^{mod+}, #mu = 200)" + ";tan(#beta); #sigma, [pb]").c_str());

		leg.AddEntry(&mssm,"MSSM","al");
		leg.AddEntry(&thdm,("2HDM, cos(#beta-#alpha) = " + cB_A).c_str(),"p");

		if(i==0){
			thdm.Draw("AP");
		}
		else thdm.Draw("Psame");
		mssm.Draw("Csame");
//		if(i==0) thdm.GetYaxis()->SetRangeUser(0.0001,150);
		can.SetLogy();
		can.Update();
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/2HDM/" + boson + "_2HDMvsMSSM" + "_Xs_tanB_mA-" + to_string_with_precision(mA,4) + "_cB_A-" + cB_A + ".pdf").c_str() );

}

void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &Xss, const double& mA, const string& model){
	// setup canvas
	TCanvas can("can","can",800,600);
	can.SetGrid();
	double xmin, xmax,ymin,ymax;
	xmin = 0.6;xmax = 0.9;
	ymin = 0.15; ymax = 0.4;
	TLegend leg(xmin,ymin,xmax,ymax);
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	// Loop over the tanBetas
	for (unsigned int i = 0; i != Xss.size(); ++i){
		TGraph& thdm = graphs2HDM.at(i);
		auto Xs = Xss.at(i);
		//Cosmetics:
		thdm.SetMarkerStyle(20 + i);
		thdm.SetMarkerSize(1.02);
		thdm.SetMarkerColor(colours.at(i));
		thdm.SetTitle( (model + "(m_{A} = " + to_string_with_precision(mA,4) + ")" + ";tan#beta; #Sigma").c_str());

		leg.AddEntry(&thdm,(model + " , " + Xs).c_str(),"p");

		if(i==0){
			thdm.Draw("AP");
		}
		else thdm.Draw("Psame");
//		if(i==0) thdm.GetYaxis()->SetRangeUser(0.00000001,1);
//		if(i==0) thdm.GetXaxis()->SetRangeUser(0.8,1.);
		can.SetLogy();
		can.Update();
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/2HDM/" + boson + "_" + model + "_Xs_tanB_mA-" + to_string_with_precision(mA,4) + "_cB_A-" + cB_A + ".pdf").c_str() );
}

double MssmXs(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta,
			 const string& name) {
    double Xs = my.xsec(name.c_str(),mA,tanBeta);

    return Xs;
}

double THDMXs(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& sinB_A,
				   const string& name){
	double XsAbb = f[name]->Interpolate(mA,tanBeta,sinB_A);

	return XsAbb;
}

void GetTHDMHistos(TFile& f, map<string,TH3F*>& histos){
	CheckZombie(f);
	CheckZombieObjectInTFile(f,"xs_bb" + boson);
	histos["xs_bb" + boson] = (TH3F*) f.Get(("xs_bb" + boson).c_str());
}

}
