#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/macros/Drawer/SystematicErrorsCalculator.cpp"
#include "TROOT.h"

using namespace std;

void renormalise(TH1*);
void cosmetics(TH1* h);
void blind(TH1* h);

int data_vs_mc(){
	gROOT->Reset();

	std::string lowM = "highM";
	TFile *data_low_m = new TFile( ("../output/DataMC_" + lowM + "_Run2015.root").c_str() );
	TFile *mc_low_m 	 = new TFile( ("../output/DataMC_" + lowM + "_QCD.root").c_str() );

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
//	vector<string> Var={"NumberOfJets_b","jet_b_btag_csv2","jet_b_btag_cmva1","jet_b_btag_cmva2","jet_b_deta12","jet_b_dR12","diJet_b_pt","diJet_b_eta","diJet_b_phi","diJet_b_m"};
	vector<string> Var={"NumberOfJets_b","jet_b_pt1","jet_b_pt2","jet_b_Ht","jet_b_eta1","jet_b_eta2","jet_b_phi1","jet_b_phi2","jet12_b_assym","jet_b_btag_csv1",
						"jet_b_btag_csv2","jet_b_btag_cmva1","jet_b_btag_cmva2","jet_b_deta12","jet_b_dR12", "jet_b_dphi12",
						"diJet_b_pt","diJet_b_eta","diJet_b_phi","diJet_b_m"};
						//,"jet_b_dphi12"};
	//Systematic list:
	vector<string> Syst = {"PtEff","PU","SFb","SFl","JES","JER"};

	//histo list:
	map<string,TH1D*> h_data_low_m;
	map<string,TH1D*> h_mc_low_m;
	map<string,TH1D*> h_syst_low_m;
	map<string,TH1D*> h_ratio_low_m;

	//TCanvas list
	map<string,TCanvas*> can_low_m;

	//Pt1
	for(const string &v : Var){
		string name = (v+"/"+v);
		cout<<"Current histo: "<<name<<endl;

		can_low_m[v] 	= new TCanvas((v+"_lowM").c_str(),(v+"_lowM").c_str(),1000,800);
		h_data_low_m[v]	= (TH1D*) data_low_m->Get(name.c_str());
		h_mc_low_m[v]	= (TH1D*) mc_low_m->Get(name.c_str());
		h_syst_low_m[v]	= (TH1D*) h_mc_low_m[v]->Clone();
		h_data_low_m[v] -> SetMinimum(0.);
		h_data_low_m[v] ->SetTitle("");
		if(v.find("pt") != string::npos) h_data_low_m[v]->GetXaxis()->SetRangeUser(0.,h_data_low_m[v]->GetXaxis()->GetXmax());

		for(const string & s : Syst){
			systCalc.CalculateSingleSystError( (TH1D*) mc_low_m->Get((name + "_"+s+"_up").c_str()), (TH1D*) mc_low_m->Get((name +"_"+s+"_down").c_str()));
		}
		systCalc.AddUncorelatedSystErrors(h_syst_low_m[v]);

		leg[v] = (TLegend*) style.legend("top,right",3,0.3);
		leg[v]->AddEntry(h_data_low_m[v],"BTagCSV Data","p");
		leg[v]->AddEntry(h_mc_low_m[v],"MC","p");
		renormalise(h_data_low_m[v]);
		renormalise(h_mc_low_m[v]);
		renormalise(h_syst_low_m[v]);

		if(v.find("diJet") != std::string::npos && v.find("m") != std::string::npos) blind(h_data_low_m[v]);

		h_ratio_low_m[v] = (TH1D*) ratio.DrawRatio(h_data_low_m[v],h_mc_low_m[v],can_low_m[v],leg[v],h_syst_low_m[v]);
		h_ratio_low_m[v] -> SetAxisRange(0.2,1.8,"Y");
		ratio.GetTopPad();		style.drawStandardTitle();
		if(v.find("pt") != string::npos || v.find("dphi") != std::string::npos || v.find("Ht") != string::npos) {
			h_data_low_m[v]->SetMinimum(0.01);
			gPad->SetLogy();
		}
		can_low_m[v]->Print(("pictures/DataVsMC/" + v + "_" + lowM + ".pdf").c_str());
		/**/
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
