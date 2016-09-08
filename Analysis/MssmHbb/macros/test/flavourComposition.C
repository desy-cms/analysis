#include "TSystem.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

int flavourComposition(){

	gStyle->SetOptFit(1111);

	std::string name;
	std::string lowM = "highM";
	std::string full_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/";
	TFile *file = nullptr;

	name = full_path + "output/JetAnalysisBase_falvourComposition_" + lowM + "_QCD_pythia8.root";
	if(gSystem->AccessPathName(name.c_str())){
		std::cout<<"No file with name: "<<name<<std::endl;
		exit(-1);
	}
	file = new TFile(name.c_str(),"READ");

	std::array<std::string,3> wp = {"T","M","L"};
	std::array<std::string,2> trigger = {"","_triggered"};
	std::array<std::string,6> flavours = {"bb","cc","ll","bc","cl","bl"};
	std::array<double,6> integral;
	std::array<double,6> percent;

	std::string hName;
	std::map<std::string,TH1*>	hist;
	std::map<std::string,THStack*> stack;
	std::map<std::string, TLegend*> leg;
	std::map<std::string, TCanvas*> can;
	int colors[]= {kCyan+2,kYellow-6,kMagenta+2,kRed+2,kYellow+2,kGreen+2,kBlue-6};
	int counter = 0;
	double total = 0;

	for(const auto & tr : trigger){
		for(const auto & p : wp){
			stack["Flav_stack_" + p + tr] = new THStack();
			leg["Leg_" + p + tr] = new TLegend(0.5,0.5,0.9,0.9);
			counter = 0;
			total = 0;
			for(const auto & fl : flavours){
				hName = "M12_" + fl + "_" + p + tr;
				hist[hName] = (TH1D*) file->Get( hName.c_str() );
				if(hist[hName] == nullptr){
					std::cerr<<"No histogram: "<<hName<<" at file "<<file->GetName()<<" was found"<<std::endl;
					exit(-1);
				}
				integral[counter] = hist[hName]->Integral();
				total += integral[counter];

				++counter;
			}

			counter = 0;
			for(const auto & fl : flavours){
				hName = "M12_" + fl + "_" + p + tr;
				hist[hName] = (TH1D*) file->Get( hName.c_str() );
				if(hist[hName] == nullptr){
					std::cerr<<"No histogram: "<<hName<<" at file "<<file->GetName()<<" was found"<<std::endl;
					exit(-1);
				}
				hist[hName]->SetFillColor(colors[counter]);
				hist[hName]->SetLineWidth(1.5);
				hist[hName]->SetLineColor(1);

				percent[counter] = integral[counter]/total;
				leg["Leg_" + p + tr]->AddEntry(hist[hName], (p + " WP, " + fl + tr + " ; Amount: " + to_string_with_precision(percent[counter],2)).c_str(),"fl");
				stack["Flav_stack_" + p + tr]->Add(hist[hName]);
				++counter;
			}

			can["can_" + p + tr] = new TCanvas( ("can_" + p + tr).c_str(),("can_" + p + tr).c_str(), 1000, 800 );
			stack["Flav_stack_" + p + tr] ->Draw("hist");
			leg["Leg_" + p + tr]->Draw();

			can["can_" + p + tr]->Print( (full_path + "macros/pictures/" + "flavourComposition_" + lowM + "_" + p + tr + ".pdf").c_str() );
		}
	}

	return 0;
}
