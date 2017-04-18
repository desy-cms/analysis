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

void CompareSigmaXBr(const vector<int>&, const vector<double>&, const vector<double>&, const vector<string>&);
double MssmBr(mssm_xs_tools& my,
		     const double& mA,
		     const double& tanBeta);
double THDMBr(map<string,TH3F*>& f,
				   const double& mA,
				   const double& tanBeta,
				   const double& cosB_A);
void DrawGraphs( map<string,TGraph*>& graphs2HDM, const double &tanBetas, const double& cosB_A);
void GetTHDMHistos(TFile& name,map<string,TH3F*>&);

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	string folder_path = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/production_corseBins_cosB_A_-1_1_tanB_1-100/rootFiles/";
	vector<string> thdm_types {folder_path + "Histograms3D_type1_mA.root",
							   folder_path + "Histograms3D_type2_mA.root",
							   folder_path + "Histograms3D_type3_mA.root",
							   folder_path + "Histograms3D_type4_mA.root"};
//	Define mass points:
	vector<int> mAs{300,350,400,500,600,700,900,1100,1299};
//	Define tnaBeta points:
	vector<double> tanBetas{10,20,30,40,50,60};
//	Define cos(beta-alpha) points:
	vector<double> cosB_As{-0.99,-0.6,-0.4,-0.05,0,0.05,0.4,0.6,0.99};
	CompareSigmaXBr(mAs,tanBetas,cosB_As,thdm_types);
	return 0;
}

void CompareSigmaXBr(const vector<int> &mAs, const vector<double> &tanBetas, const vector<double> &cosB_As, const vector<string>& thdm_types){
	//	File for 2HDM scans:
	vector<string> types;
	vector<map<string,TH3F*> > thdms_map(thdm_types.size());
	map<string,TFile*> tfiles;
	int k =0;
	for(const auto& thdm : thdm_types){
//		 = f(thdm.c_str(),"read");
		string thdm_type_string = thdm.substr(thdm.find("type"),5);
		tfiles[thdm_type_string] = new TFile(thdm.c_str(),"read");
		GetTHDMHistos(*tfiles[thdm_type_string],thdms_map.at(k));
		types.push_back(thdm_type_string);
		++k;
	}
	//	Start mA, tan(beta) and cos(beta-alpha) loops:
	for(const auto& cosB_A : cosB_As){
		for(const auto& tanBeta : tanBetas){
			map<string,TGraph*> graphsTHDM;
			int i =0;
//			for(const auto& thdm : thdms_map){
			for(unsigned int v = 0; v != thdms_map.size(); ++v){
				graphsTHDM[types.at(i)] = new TGraph(mAs.size());
				int j =0;
				for(const auto& mA : mAs){
					++j;
					double value = THDMBr(thdms_map.at(v),mA,tanBeta,cosB_A);
					cout<<"M: "<<mA<<" GxBR: "<<value<<endl;
					graphsTHDM[types.at(i)]->SetPoint(j,mA,value);
				}
				++i;
			}
			std::cout<<"DRAW"<<std::endl;
			DrawGraphs(graphsTHDM,tanBeta,cosB_A);
		}
	}
}

void DrawGraphs( map<string, TGraph*>& graphs2HDM, const double &tanBeta, const double& cosB_A){
	// setup canvas
	TCanvas can("can","can",800,600);
	can.SetGrid();
	TLegend leg(0.6,0.3,0.9,0.5);
//	leg.SetHeader(("2HDM(cos(#beta-#alpha) = " + to_string_with_precision(cosB_A,2) + ") vs MSSM(m_h^{mod+}, #mu = 200)").c_str());
	vector<int> colours = {1,2,3,4,6,9,12,28,46};
	// Loop over the tanBetas
	int i = 0;
	for (const auto& thdm : graphs2HDM){
		thdm.second->SetMarkerStyle(20 + i);
		thdm.second->SetMarkerSize(1.1);
		thdm.second->SetMarkerColor(colours.at(i));
		thdm.second->SetTitle( ("2HDM(cos(#beta-#alpha) = " + to_string_with_precision(cosB_A,3) + ", tan#beta = " + to_string_with_precision(tanBeta,3) + ";M_{12}, GeV; #sigma x BR [pb]").c_str());
		leg.AddEntry(thdm.second,("2HDM, " + thdm.first).c_str(),"p");

		if(i==0){
			thdm.second->Draw("AP");
		}
		else thdm.second->Draw("Psame");
		if(i==0) thdm.second->GetYaxis()->SetRangeUser(1e-17,150);
		can.SetLogy();
		can.Update();
		++i;
	}
	leg.Draw();
	can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/thdm_types_cosB_A-" + to_string_with_precision(cosB_A,3) + "_tanB-" + to_string_with_precision(tanBeta,3) + ".pdf").c_str() );
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
				   const double& cosB_A){
	double sigmaBBA = f["xs_bbA"]->Interpolate(mA,tanBeta,cosB_A);
	double BrAbb = f["br_Abb"]->Interpolate(mA,tanBeta,cosB_A);

	double totXSec = sigmaBBA*BrAbb;// + sigmaBBH*BrHbb;
	return totXSec;
}

void GetTHDMHistos(TFile& f,map<string,TH3F*>& histos){
	CheckZombie(f);
	CheckZombieObjectInTFile(f,"xs_bbA");
//	map<string,TH3F*> histos;
	histos["xs_bbA"] = (TH3F*) f.Get("xs_bbA");
	CheckZombieObjectInTFile(f,"br_Abb");
	histos["br_Abb"] = (TH3F*) f.Get("br_Abb");

//	return histos;
}
