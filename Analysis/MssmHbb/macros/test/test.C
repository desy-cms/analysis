#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/macros/Drawer/SystematicErrorsCalculator.cpp"
#include "TROOT.h"

using namespace std;

void renormalise(TH1*);
void cosmetics(TH1* h);
void blind(TH1* h);

int test(){	
	gStyle->SetOptStat(0);

	std::string full_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/";
	std::string name;
	name = full_path + "output/bbx_lowM_QCD.root";
	TFile *bbx_mc 	= new TFile( (name).c_str(),"READ" );
	if(!bbx_mc) {
		std::cout<<" No file: "<<name<<" were found"<<std::endl;
		exit(-1);
	}

	name = full_path + "output/bbx_lowM_Run2015.root";
	TFile *bbx_data = new TFile( (name).c_str(),"READ" );
	if(!bbx_data){
		std::cout<<" No file: "<<name<<" were found"<<std::endl;
		exit(-1);
	}

	name = full_path + "output/DataMC_3b_NoHTrew_lowM_QCD.root";
	TFile *bbb_mc	= new TFile( (name).c_str(),"READ" );
	if(!bbb_mc){
		std::cout<<" No file: "<<name<<" were found"<<std::endl;
		exit(-1);
	}

	name = full_path + "output/DataMC_3b_lowM_Run2015.root";
	TFile *bbb_data	= new TFile( (name).c_str(),"READ" );
	if(!bbb_data){
		std::cout<<" No file: "<<name<<" were found"<<std::endl;
		exit(-1);
	}

	//Style
	HbbStyle style;
	style.set(PRIVATE);

	//Draw ratio objects;
	RatioPlots ratio(PRIVATE);

	//Declare SystematicCalc object
	SystematicErrorsCalculator systCalc;

	//Declare list of Legends
	map<string,TLegend*> leg;

	//variables vector:
	vector<string> Var={"diJet_b_m"};

	//Systematic list:
	vector<string> Syst = {"JEC","JES","PU","SFb","SFl","PtEff"};

	//histo list:
	map<string,TH1D*> hBBX_mc;
	map<string,TH1D*> hBBB_mc;
	map<string,TH1D*> hratio_mc;

	//TCanvas list
	map<string,TCanvas*> can_low_m;

	//Pt1
	for(const string &v : Var){
		string name = (v+"/"+v);
		cout<<"Current histo: "<<name<<endl;

		can_low_m[v] 	= new TCanvas((v+"_lowM").c_str(),(v+"_lowM").c_str(),1000,800);
		hBBX_mc[v]	= (TH1D*) bbx_mc->Get(name.c_str());
		hBBB_mc[v]	= (TH1D*) bbb_mc->Get(name.c_str());
		renormalise(hBBB_mc[v]);
		renormalise(hBBX_mc[v]);

		hBBX_mc[v]->Scale(1./hBBX_mc[v]->Integral());
		hBBB_mc[v]->Scale(1./hBBB_mc[v]->Integral());
		hBBB_mc[v]->SetMarkerStyle(21);
		hBBB_mc[v]->SetMarkerSize(1.5);

		leg[v] = (TLegend*) style.legend("top,right",3,0.3);
		leg[v]->AddEntry(hBBX_mc[v],"bbx - MC","p");
		leg[v]->AddEntry(hBBB_mc[v],"bbb - MC","p");

		hBBX_mc[v] ->SetTitle("");
		hratio_mc[v] = (TH1D*) ratio.DrawRatio(hBBX_mc[v],hBBB_mc[v],can_low_m[v],leg[v],nullptr);
		hratio_mc[v]->SetAxisRange(0.,3.,"Y");
		ratio.GetTopPad();		style.drawStandardTitle();
		gPad->SetLogy();


	}
	return 0;
}

void renormalise(TH1 * h){
	for(int i=1; i<= h->GetNbinsX();++i){
		h->SetBinContent(i,h->GetBinContent(i)/h->GetBinWidth(i));
		h->SetBinError(i,h->GetBinError(i)/h->GetBinWidth(i));
	}
//	h->Scale(1./h->Integral());
}

void cosmetics(TH1* h){
	if( string(h->GetName()).find("pt") != std::string::npos){
		h->SetMinimum(0.01);
		gPad->SetLogy();
	}
}

void blind(TH1* h){
	for(int i=1;i<h->GetNbinsX();++i){
		if(h->GetXaxis()->GetBinUpEdge(i) >= 650 &&
		   h->GetXaxis()->GetBinLowEdge(i) <= 850){
			h->SetBinContent(i,0);
			h->SetBinError(i,0);
		}
	}
}
