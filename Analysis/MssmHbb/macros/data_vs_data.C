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

void data_vs_data(){
	gROOT->Reset();

	std::string selection = "DataMC";
	std::string name = "test_2016_pt_20_lowM";
	std::string samplename1 = "Run2016E-23Sep2016-v1";
	std::string samplename2 = "Run2016E-PromptReco-v2";
	TFile *fdata1 	= new TFile( ("../output/" + selection + "_" + name + "_" + samplename1 + ".root").c_str() );
	TFile *fdata2 	= new TFile( ("../output/" + selection + "_" + name + "_" + samplename2 + ".root").c_str() );
	
	//Style
	HbbStyle style;
	style.set(PRIVATE);

	//Draw ratio objects;
	RatioPlots ratio(PRIVATE);

	//Declare list of Legends
	map<string,TLegend*> leg;

	//variables vector:
	vector<string> Var={"NumberOfJets_b","jet_b_pt1","jet_b_pt2","jet_b_Ht","jet_b_eta1","jet_b_eta2","jet_b_phi1","jet_b_phi2","jet12_b_assym","jet_b_btag_csv1",
						"jet_b_btag_csv2","jet_b_btag_cmva1","jet_b_btag_cmva2","jet_b_deta12","jet_b_dR12", "jet_b_dphi12",
						"diJet_b_pt","diJet_b_eta","diJet_b_phi","diJet_b_m"};

	//histo list:
	map<string,TH1D*> h_data1;
	map<string,TH1D*> h_data2;
	map<string,TH1D*> h_ratio;

	//TCanvas list
	map<string,TCanvas*> can_low_m;

	//Pt1
	for(const string &v : Var){
		string name = (v);
		cout<<"Current histo: "<<name<<endl;

		can_low_m[v] 	= new TCanvas((v+"_lowM").c_str(),(v+"_lowM").c_str(),1000,800);
		h_data1[v]	= (TH1D*) fdata1->Get(name.c_str());
		h_data2[v]	= (TH1D*) fdata2->Get(name.c_str());
		std::cout<<"Scale = "<<h_data1[v]->Integral()/h_data2[v]->Integral()<<std::endl;
		h_data1[v] -> SetMinimum(0.);
		h_data1[v] ->SetTitle("");
		if(findStrings(v,"pt")) h_data1[v]->GetXaxis()->SetRangeUser(0.,h_data1[v]->GetXaxis()->GetXmax());

		leg[v] = (TLegend*) style.legend("top,right",3,0.3);
		leg[v]->AddEntry(h_data1[v],"ReReco","p");
		leg[v]->AddEntry(h_data2[v],"PromptReco-v1","p");
		renormalise(h_data1[v]);
		renormalise(h_data2[v]);

		h_ratio[v] = (TH1D*) ratio.DrawRatio(h_data1[v],h_data2[v],can_low_m[v],leg[v],nullptr);
		h_ratio[v] -> SetAxisRange(0.2,1.8,"Y");
		ratio.GetTopPad();		style.drawStandardTitle();
		if(findStrings(v,"pt") || findStrings(v,"dphi") || findStrings(v,"Ht")) {
			h_data1[v]->SetMinimum(0.01);
			gPad->SetLogy();
		}
		can_low_m[v]->Print(("pictures/DataVsData/" + samplename1 + "_vs_" + samplename2 + "_var_" + v + "_selection_" + selection + ".pdf").c_str());
		/**/
	}
//	Ht_correction->Close();
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
