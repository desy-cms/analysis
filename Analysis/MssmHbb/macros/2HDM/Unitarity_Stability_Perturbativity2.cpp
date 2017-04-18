#include <TH3F.h>
#include <TH2D.h>
#include <TH3.h>
#include <TGraph.h>
#include <TFile.h>
#include "TSystem.h"
#include <TCanvas.h>
#include <TLegend.h>
#include <TTree.h>
#include <TStyle.h>

#include <string>
#include <vector>

// Tools
#include "Analysis/MssmHbb/interface/utilLib.h"
#include "Analysis/MssmHbb/macros/Drawer/HbbStylesNew.C"

using namespace std;

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

void TwoDPlot_tanB_vs_MA(TTree& tree, const vector<double>& sinB_As, const string& options,const string& type = "type3");
void TwoDPlot_tanB_vs_sinB_A(TTree& tree, const vector<double>& sinB_As, const string& options,const string& type = "type3");
void TwoDPlot_tanB_vs_cosB_A(TTree& tree, const vector<double>& sinB_As, const string& type = "type3");

void TwoDPlot_tanB_vs_cosB_A(TFile& file, const double& mA, const string& type = "type3");
void SetTHDMHistos(TFile& f,std::map<std::string,TH3D*>& histos);

double tanAlpha(const double& mA, const double& tanB);
double tanBeta_Alpha(const double& tanB, const double& tanA);

string boson = "H";

int main(){
//	Comparison should be done
//	Histo with 2HDM results:
	boson = "A";
	string thdm_production = "production_cosB_A_-1_1_tanB_0p5-100_COMBINATION"; //production_corseBins_cosB_A_-1_1_tanB_1-100 //
	string thdm_type = "type3";
	string thdm_fname = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/" + thdm_production + "/rootFiles/Histograms3D_" + thdm_type + "_m" + boson + ".root";
	TFile f(thdm_fname.c_str(),"read");
	HbbStylesNew* style = new HbbStylesNew();
	style->SetStyle();
	//	Define sin(beta-alpha) points:
	vector<double> sinB_As;//{0.8,0.802,0.9,0.99};
	for(double i = -1.00; i<= 1.00; i+=0.005) sinB_As.push_back(i);
	vector<double> mAs{300,350,400,500,600,700,900,1100,1300};
	for(const auto& mA : mAs){
		TwoDPlot_tanB_vs_cosB_A(f,mA,thdm_type);
	}

	return 0;
}

void TwoDPlot_tanB_vs_cosB_A(TFile& f, const double& mA,const string& type){
	CheckZombie(f);
	std::map<std::string,TH3D*> histos;
	std::map<std::string,TH2D*> projs;
	SetTHDMHistos(f,histos);

	int bin = -10;
	int i = 0;
	TCanvas *can = new TCanvas("can","can",800,600);
	can->SetGrid();
	gPad->SetLogy();
	for(auto& h : histos){
		bin = h.second->GetXaxis()->FindBin(mA);
		h.second->GetXaxis()->SetRange(bin,bin);
		projs[h.first] = (TH2D*) h.second->Project3D("yz");
		projs[h.first]->SetStats(0);
		projs[h.first]->SetTitle((h.first + ", m" + boson + " = " + to_string_with_precision(mA,4)).c_str());
		projs[h.first]->GetXaxis()->SetNdivisions(20);

		projs[h.first]->Draw("COL");
		can->Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/2HDM/" + boson + "_" + type + "_" + h.first + "_mA_" + to_string_with_precision(mA,4) + ".pdf").c_str() );
		++i;
	}

}

void SetTHDMHistos(TFile& f,std::map<std::string,TH3D*>& histos){
	/*
	 * Method to set map of 2HDM histograms according to boson name
	 */
	CheckZombieObjectInTFile(f,"stability");
	histos["stability"] = (TH3D*) f.Get("stability");

	CheckZombieObjectInTFile(f,"unitarity");
	histos["unitarity"] = (TH3D*) f.Get("unitarity");

	CheckZombieObjectInTFile(f,"perturbativity");
	histos["perturbativity"] = (TH3D*) f.Get("perturbativity");
}

void TwoDPlot_tanB_vs_MA(TTree& tree, const vector<double>& sinB_As, const string& options="",const string& type){
	//Vector of histograms:
//	vector<TH2D&> histo(sinB_As.size());
	int colors[] = {0, 1, 3, 4, 5, 6, 2};
	gStyle->SetPalette((sizeof(colors)/sizeof(int)), colors);
	for(unsigned int i = 0; i < sinB_As.size();++i){
		double sinB_A = sinB_As.at(i);
		TCanvas can( ("can_sinB_A_" + to_string_with_precision(sinB_A,3)).c_str(),"can",800,600);
		string draw_opt = "sinB_A >= " + to_string_with_precision(sinB_A - 0.001,5) + "&& sinB_A < " + to_string_with_precision(sinB_A + 0.001,5);
		if(options != "") draw_opt += " && ";
		tree.Draw("mA:tanBeta>>hTemp(720,10,100,112,0,1400)",(draw_opt + options).c_str());
		TH2D& h = (TH2D&)* gDirectory->Get("hTemp");
		cout<<sinB_A<<endl;
		h.SetTitle(("sin(#beta-#alpha) = " + to_string_with_precision(sinB_A,5) + "; tan#beta; M_{A}, GeV").c_str());
		h.SetStats(kFALSE);
		h.Draw("COL");
		if(h.GetEntries() != 0) can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/Check_2hdm_uni_pert_stab_sinB_A_" + to_string_with_precision(sinB_A,3) + "_" + options + ".pdf").c_str() );
	}
}

void TwoDPlot_tanB_vs_sinB_A(TTree& tree, const vector<double>& mAs, const string& options,const string& type){
	int colors[] = {0, 1, 3, 4, 5, 6, 2};
	gStyle->SetPalette((sizeof(colors)/sizeof(int)), colors);
	for(unsigned int i = 0; i < mAs.size();++i){
		double mA = mAs.at(i);
		TCanvas can( ("can_mA_" + to_string_with_precision(mA,3)).c_str(),"can",800,600);
//		string draw_opt = "mA >= " + to_string_with_precision(mA - 0.001,5) + "&& sinB_A < " + to_string_with_precision(sinB_A + 0.001,5);
		string draw_opt = "mA == " + to_string_with_precision(mA,4);
		if(options != "") draw_opt += " && ";
		tree.Draw("tanBeta:sinB_A>>hTemp(201,0.98,1.001,720,10,100)",(draw_opt + options).c_str());
		TH2D& h = (TH2D&)* gDirectory->Get("hTemp");
		cout<<mA<<endl;
		h.SetTitle(("M_{A} = " + to_string_with_precision(mA,5) + ", GeV; sin(#beta-#alpha); tan#beta").c_str());
		h.SetStats(kFALSE);
		h.Draw("COL");
		if(h.GetEntries() != 0) can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/Check_2hdm_uni_pert_stab_mA_" + to_string_with_precision(mA,3) + "_" + options + ".pdf").c_str() );
	}
}

//void TwoDPlot_tanB_vs_cosB_A(TFile& f, const vector<double>& mAs,const string& type){
//
//}

void TwoDPlot_tanB_vs_cosB_A(TTree& tree, const vector<double>& mAs,const string& type){
	int colors[] = {0, 1, 3, 4, 5, 6, 2};
	gStyle->SetPalette((sizeof(colors)/sizeof(int)), colors);
	for(unsigned int i = 0; i < mAs.size();++i){
		double mA = mAs.at(i);
		TCanvas can( ("can_mA_" + to_string_with_precision(mA,3)).c_str(),"can",800,600);
		TH2D h("hTemp","",200,-0.5,0.5,396,0.5,50);//720: 10 - 100
		Float_t sinB_A;
		tree.SetBranchAddress("sinB_A",&sinB_A);
		Float_t tanB;
		tree.SetBranchAddress("tanBeta",&tanB);
		Float_t MA;
		tree.SetBranchAddress("mA",&MA);
		Float_t uni;
		tree.SetBranchAddress("unitarity",&uni);
		Float_t stab;
		tree.SetBranchAddress("stability",&stab);
		Float_t pert;
		tree.SetBranchAddress("perturbativity",&pert);

		auto nentries = tree.GetEntries();
		for(auto i = 0;i < nentries; ++i){
			tree.GetEntry(i);
//			cout<<"TEST: Uni = "<<uni<<" Stab = "<<stab<<" Pert = "<<pert<<" MA = "<<MA<<" tanB = "<<tanB<<endl;
			if(MA != mA) continue;
			if(uni != 1 || stab != 1 || pert != 1) continue;
//			double tanA = tanAlpha(MA,tanB);
//			double tanB_A = tanBeta_Alpha(tanB,tanA);
			double cosB_A = sqrt(1. - sinB_A*sinB_A);
			if(sinB_A < 0) cosB_A *= -1; // Doesn't work right now.
			h.Fill(cosB_A,tanB);
		}

		cout<<mA<<endl;
		h.SetTitle((type +  ", M_{A} = " + to_string_with_precision(mA,5) + ", GeV; cos(#beta-#alpha); tan#beta").c_str());
		h.SetStats(kFALSE);
		can.SetLogy();
		h.Draw("COL");
		if(h.GetEntries() != 0) can.Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/" + type + "_Check_2hdm_uni_pert_stab_cosB_A_mA_" + to_string_with_precision(mA,3) + ".pdf").c_str() );
	}
}


double tanAlpha(const double& mA, const double& tanB){
	double tanA = 0;
	double sin2B = 2. * tanB / (1. + tanB*tanB);
	double cos2B = 2./(tanB*tanB + 1.) - 1;
	double mZ = 91.1876; //from PDG
	double m2Z = mZ * mZ;
	double m2A = mA * mA;
	tanA = (-1. * (m2A + m2Z) * sin2B)/( (m2Z - m2A)*cos2B + sqrt( (m2A+m2Z)*(m2A+m2Z) - 4.*m2A*m2Z*cos2B*cos2B ) );

	return tanA;
}

double tanBeta_Alpha(const double& tanB, const double& tanA){
	return (tanB - tanA)/(1. + tanB*tanA);
}
