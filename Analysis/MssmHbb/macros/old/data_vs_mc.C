#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/macros/Drawer/SystematicErrorsCalculator.cpp"
#include "TROOT.h"

using namespace std;

void renormalise(TH1*);
void cosmetics(TH1* h);
void blind(TH1* h);

const bool findStrings(const std::string & input, const std::string & needful)
{
	std::string input1 = input;
	std::string input2 = needful;
	std::transform(input1.begin(),input1.end(),input1.begin(),::tolower);
	std::transform(input2.begin(),input2.end(),input2.begin(),::tolower);
	if(input1.find(input2) != std::string::npos) return true;
	else return false;
}

int data_vs_mc(){
	gROOT->Reset();

	std::string selection = "bbx";
	std::string lowM = "lowM";
	std::string MCsample = "QCD_rew2b_scaled";
	TFile *data_low_m = new TFile( ("../output/" + selection + "_" + lowM + "_Run2015D-16Dec2015-v1.root").c_str() );
	TFile *mc_low_m 	 = new TFile( ("../output/" + selection + "_" + lowM + "_" + MCsample + ".root").c_str() );
	
//	TFile * Ht_correction = new TFile( (selection + "_HT_Correction_" + lowM + "_" + MCsample + ".root").c_str(),"RECREATE" );

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
	vector<string> Var={"jet_b_Ht"};
//	vector<string> Var={"NumberOfJets_b","jet_b_pt1","jet_b_pt2","jet_b_Ht","jet_b_eta1","jet_b_eta2","jet_b_phi1","jet_b_phi2","jet12_b_assym","jet_b_btag_csv1",
//						"jet_b_btag_csv2","jet_b_btag_cmva1","jet_b_btag_cmva2","jet_b_deta12","jet_b_dR12", "jet_b_dphi12",
//						"diJet_b_pt","diJet_b_eta","diJet_b_phi","diJet_b_m"};
						//,"jet_b_dphi12"};
	//Systematic list:
	vector<string> Syst = {"JEC","JES","PU","SFb","SFl","PtEff"};

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
		h_data_low_m[v]	= (TH1D*) data_low_m->Get(v.c_str());
		h_mc_low_m[v]	= (TH1D*) mc_low_m->Get(name.c_str());
		std::cout<<"Scale = "<<h_data_low_m[v]->Integral()/h_mc_low_m[v]->Integral()<<std::endl;
//		h_mc_low_m[v] ->Scale(h_data_low_m[v]->Integral()/h_mc_low_m[v]->Integral());
//		if(MCsample == "BGenFilter")  h_mc_low_m[v] ->Scale(1./0.3);
		h_syst_low_m[v]	= (TH1D*) h_mc_low_m[v]->Clone();
		h_data_low_m[v] -> SetMinimum(0.);
		h_data_low_m[v] ->SetTitle("");
		if(findStrings(v,"pt")) h_data_low_m[v]->GetXaxis()->SetRangeUser(0.,h_data_low_m[v]->GetXaxis()->GetXmax());

		for(const string & s : Syst){
			TH1D *up 	= (TH1D*) mc_low_m->Get((name + "_" + s + "_up").c_str());
			TH1D *down 	= (TH1D*) mc_low_m->Get((name + "_" + s + "_down").c_str());
			if(!up || !down) continue;
			systCalc.CalculateSingleSystError(up, down);
		}
		systCalc.AddUncorelatedSystErrors(h_syst_low_m[v]);

		leg[v] = (TLegend*) style.legend("top,right",3,0.3);
		leg[v]->AddEntry(h_data_low_m[v],"BTagCSV Data","p");
		leg[v]->AddEntry(h_mc_low_m[v],"MC","p");
		renormalise(h_data_low_m[v]);
		renormalise(h_mc_low_m[v]);
		renormalise(h_syst_low_m[v]);

		if( findStrings(v,"diJet") && findStrings(v,"m")) blind(h_data_low_m[v]);

		h_ratio_low_m[v] = (TH1D*) ratio.DrawRatio(h_data_low_m[v],h_mc_low_m[v],can_low_m[v],leg[v],h_syst_low_m[v]);
		h_ratio_low_m[v] -> SetAxisRange(0.2,1.8,"Y");
		ratio.GetTopPad();		style.drawStandardTitle();
		if(findStrings(v,"pt") || findStrings(v,"dphi") || findStrings(v,"Ht")) {
			h_data_low_m[v]->SetMinimum(0.01);
			gPad->SetLogy();
		}
//		if (findStrings(v,"Ht")){
//			TH1D *h_temp = (TH1D*) h_ratio_low_m[v] -> Clone("h_clone");
//			h_temp->GetYaxis()->SetRangeUser(0,5);
//			h_temp->Scale(1./h_temp->Integral());
//			h_temp -> Write();
//		}
		can_low_m[v]->Print(("pictures/DataVsMC/" + v + "_" + lowM + "_" + MCsample + ".pdf").c_str());
		/**/
	}
//	Ht_correction->Close();
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
