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

void CompareBrs(const vector<double>&, const vector<double>&, const double& , const vector<string>&, const string&, const string&);
double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta);
double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& cosB_A,
				   const string& name);
void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &BRs, const double& sinB_A,const string &type);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&,const vector<string>&);

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	vector<string> types{"type1","type2","type3","type4"};
	//	Define mass points:
	vector<double> mAs{300,400,500,600,700,900,1100};
	//	Define tnaBeta points:
	vector<double> tanBetas{10,20,30,40,50,60};
	//	Define cos(beta-alpha) points:
	vector<double> cosB_As;
	for(double i = -1.001; i<= 1.001; i+=0.02) cosB_As.push_back(i);
	for(const auto& type : types){
		string thdm_fname = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/production_corseBins_cosB_A_-1_1_tanB_1-100/rootFiles/Histograms3D_" + type + "_mA.root";
		TFile f(thdm_fname.c_str(),"read");

	//	Define types of BRs:
		vector<string> BRs = {"br_Ass","br_Acc","br_Abb","br_Att","br_Atautau","br_AZh"};
		CompareBrs(cosB_As,tanBetas,500,BRs,thdm_fname,type);
	}

	return 0;
}

void CompareBrs(const vector<double> &cosB_As, const vector<double> &tanBetas, const double&mA, const vector<string>& BRs, const string &thdm_fname,const string &type){
	//	File for 2HDM scans:
	map<string,TH3F*> thdm_map;
	TFile f(thdm_fname.c_str(),"read");
	GetTHDMHistos(f,thdm_map,BRs);
//	Start mA, tan(beta) and cos(beta-alpha) loops:
	for(const auto& tanBeta : tanBetas){
		// Prepare output TGraphs
		vector<TGraph> graphs2HDM(BRs.size());
		vector<TGraph> graphsMSSM(BRs.size());
		int i = 0;
		for(const auto& BR : BRs){
			TGraph& gr_2hdm = graphs2HDM.at(i);
//			Set number of points for TGraph
			gr_2hdm.Set((int)cosB_As.size());
			++i;
			int j =0;
			for(const auto& cosB_A : cosB_As){
				++j;
				//Calcualte SigmaBr and fill TGraphs:
				gr_2hdm.SetPoint(j,cosB_A,THDMBr(thdm_map,mA,tanBeta,cosB_A,BR));
			}
		}
		DrawGraphs(graphs2HDM,BRs,tanBeta,type);
	}
}

void DrawGraphs(vector<TGraph>& graphs2HDM, const vector<string> &BRs, const double& tanBeta, const string &type){
	// setup canvas
	TCanvas can("can","can",800,600);
	can.SetGrid();
	double xmin, xmax,ymin,ymax;
	xmin = 0.6;xmax = 0.9;
	ymin = 0.15; ymax = 0.4;
	TLegend leg(xmin,ymin,xmax,ymax);
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	// Loop over the tanBetas
	for (unsigned int i = 0; i != BRs.size(); ++i){
		TGraph& thdm = graphs2HDM.at(i);
		auto br = BRs.at(i);

		//Cosmetics:
		thdm.SetMarkerStyle(20 + i);
		thdm.SetMarkerSize(1.02);
		thdm.SetMarkerColor(colours.at(i));
		thdm.SetTitle( ("2HDM(" + type + ", tan#beta = " + to_string_with_precision(tanBeta,2) + ")" + ";cos(#beta-#alpha); BR").c_str());

		leg.AddEntry(&thdm,("2HDM , " + br).c_str(),"p");

		if(i==0){
			thdm.Draw("AP");
		}
		else thdm.Draw("Psame");
		if(i==0) thdm.GetYaxis()->SetRangeUser(1e-09,1);
		if(i==0) thdm.GetXaxis()->SetRangeUser(-1.,1.);
		can.SetLogy();
		can.Update();
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/" + type + "_thdm_BRs_cosB_A_mA-500_tanB-" + to_string_with_precision(tanBeta,2) + ".pdf").c_str() );
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
				   const double& cosB_A,
				   const string& name){
	double BrAbb = f[name]->Interpolate(mA,tanBeta,cosB_A);

	return BrAbb;
}

void GetTHDMHistos(TFile& f, map<string,TH3F*>& histos, const vector<string>& BRs){
	CheckZombie(f);
	CheckZombieObjectInTFile(f,"xs_bbA");
	histos["xs_bbA"] = (TH3F*) f.Get("xs_bbA");
	for(const auto& br: BRs){
		CheckZombieObjectInTFile(f,br);
		histos[br] = (TH3F*) f.Get(br.c_str());
	}

}
