#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <memory>

#include <boost/filesystem.hpp>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "THStack.h"
#include "stdlib.h"


#include "Analysis/MssmHbb/interface/JetAnalysisBase.h"

typedef std::unique_ptr<TH1> pTH1;
typedef std::unique_ptr<THStack> pTHStack;

using namespace std;
using namespace analysis::mssmhbb;
using namespace analysis::tools;

// =============================================================================================
void selection(const std::string & file_list);

int main(int argc, char **argv) {

	//Check the number of input parameters
	if(argc < 2){
		std::cerr<<"Usage: "<<argv[0]<<" INPUT_FILE_LIST.txt/.root"<<std::endl;
		exit(-1);
	}

	//parse input file list to the selection
	std::ifstream ifs(argv[1]);
	std::string line;
	std::string input_list;
	if(!ifs) {
		//Check whether this list exist
		cerr<<"wrong file name. Exception."<<endl;;
		exit(-1);
	}
	else {
		while(std::getline(ifs,line)){
			boost::filesystem::path p(line);
			if(boost::filesystem::extension(p) == ".root"){
				input_list = argv[0];
			}
			else if (boost::filesystem::extension(p) == ".txt"){
				input_list = line;
			}
			else{
				std::cerr<<"Wrong input file extension. Please provide .txt/.root files"<<std::endl;
				exit(-1);
			}

			selection(input_list);
			if(boost::filesystem::extension(p) == ".root") break;
		}
	}

	return 0;
}

void selection(const std::string & input){

	TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms

	//Initialiation of analysis class
	bool lowM = false;
	JetAnalysisBase analysis(input,lowM);

	std::string triggerLogic;
	std::vector<std::string> triggerObjects;
	double pt_cut;

	if(lowM){
		triggerLogic = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
		triggerObjects = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
		pt_cut = 100;
	}
	else {
		triggerLogic = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
		triggerObjects = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
		pt_cut = 160;
	}

	analysis.addTriggerObjects(triggerObjects);
	std::vector<std::string> trigger = {"","_triggered"};

	std::array<std::pair<std::string,double> , 3 > wp = { { std::pair<std::string,double>("T",0.935),
														  std::pair<std::string,double>("M",0.8),
														  std::pair<std::string,double>("L",0.46) } };
	std::map<std::string,pTH1> h;
	std::map<std::string,pTHStack > stack;
	for(const auto & tr : trigger){
		for(const auto & p : wp){
			h["M12_" + p.first + tr] = std::make_unique<TH1D>( ("M12_" + p.first + tr).c_str() , (p.first + " WPs").c_str(),100,0,1500);
			h["M12_bb_" + p.first + tr] = std::make_unique<TH1D>( ("M12_bb_" + p.first + tr).c_str() , (p.first + " WPs bb").c_str() ,100,0,1500);
			h["M12_cc_" + p.first + tr] = std::make_unique<TH1D>( ("M12_cc_" + p.first + tr).c_str() , (p.first + " WPs cc").c_str() ,100,0,1500);
			h["M12_ll_" + p.first + tr] = std::make_unique<TH1D>( ("M12_ll_" + p.first + tr).c_str() , (p.first + " WPs ll").c_str() ,100,0,1500);
			h["M12_bc_" + p.first + tr] = std::make_unique<TH1D>( ("M12_bc_" + p.first + tr).c_str() , (p.first + " WPs bc").c_str() ,100,0,1500);
			h["M12_cl_" + p.first + tr] = std::make_unique<TH1D>( ("M12_cl_" + p.first + tr).c_str() , (p.first + " WPs cl").c_str() ,100,0,1500);
			h["M12_bl_" + p.first + tr] = std::make_unique<TH1D>( ("M12_bl_" + p.first + tr).c_str() , (p.first + " WPs bl").c_str() ,100,0,1500);
			stack["Flav_stack_" + p.first + tr] = std::make_unique<THStack>( ("Flav_stack_" + p.first + tr).c_str(), (p.first + " WP, Flavour stack " + tr).c_str() );
		}
	}

	int nevents = analysis.size() * 0.5;
	int ncand = 0;
	std::cout<<" Events to process: "<<nevents<<std::endl;

	//Event loop
	for(int i = 0; i < nevents ; ++i){
		analysis.event(i);

		//Reco jets collection
		auto recoJets 	= analysis.collection<Jet>("Jets");
		auto genJets	= analysis.collection<Jet>("GenJets");
		analysis.match<Jet,TriggerObject>("Jets",analysis.getTriggerObjectNames());

		//check collection size
		if(recoJets->size() < 2) continue;

		//Pick leading jets:
		Jet jet[5];
		jet[0] = recoJets->at(0);
		jet[1] = recoJets->at(1);

		//basic selection
		if(!jet[0].idLoose() || !jet[1].idLoose()) continue;
		if(jet[0].pt() < pt_cut || jet[1].pt() < pt_cut) continue;
		if(std::abs(jet[0].eta()) > 2.2 || std::abs(jet[1].eta()) > 2.2 ) continue;
		if(lowM && std::abs(jet[0].eta() - jet[1].eta()) > 1.6 ) continue;
		if(jet[0].deltaR(jet[1]) <= 1) continue;

		TLorentzVector diJet = jet[0].p4() + jet[1].p4();

		bool b[2], c[2], l[2];
		for(int i = 0; i < 2;++i) {
			b[i] = jet[i].flavour("Hadron") == 5;
			c[i] = jet[i].flavour("Hadron") == 4;
			l[i] = jet[i].flavour("Hadron") == 0;
		}
		//Now check different WPs
			for(const auto & p : wp){
				if(jet[0].btag() >= p.second && jet[1].btag() >= p.second ){
					if(b[0] && b[1]){
						h["M12_bb_"+p.first]->Fill(diJet.M());
					}
					else if (c[0] && c[1]){
						h["M12_cc_"+p.first]->Fill(diJet.M());
					}
					else if (l[0] && l[1]){
						h["M12_ll_"+p.first]->Fill(diJet.M());
					}
					else if ( (b[0] && c[1]) || (b[1] && c[0]) ){
						h["M12_bc_"+p.first]->Fill(diJet.M());
					}
					else if ( (c[0] && l[1]) || (c[1] && l[0])){
						h["M12_cl_"+p.first]->Fill(diJet.M());
					}
					else if ( (b[0] && l[1]) || (b[1] && l[0]) ){
						h["M12_bl_"+p.first]->Fill(diJet.M());
					}

					h["M12_" + p.first]->Fill(diJet.M());
				}
			}

			if(analysis.triggerResult(triggerLogic) &&
					analysis.OnlineSelection(jet[0],jet[1])){
				for(const auto & p : wp){
					if(jet[0].btag() >= p.second && jet[1].btag() >= p.second ){
						if(b[0] && b[1]){
							h["M12_bb_"+p.first + "_triggered"]->Fill(diJet.M());
						}
						else if (c[0] && c[1]){
							h["M12_cc_"+p.first + "_triggered"]->Fill(diJet.M());
						}
						else if (l[0] && l[1]){
							h["M12_ll_"+p.first + "_triggered"]->Fill(diJet.M());
						}
						else if ( (b[0] && c[1]) || (b[1] && c[0]) ){
							h["M12_bc_"+p.first + "_triggered"]->Fill(diJet.M());
						}
						else if ( (c[0] && l[1]) || (c[1] && l[0])){
							h["M12_cl_"+p.first + "_triggered"]->Fill(diJet.M());
						}
						else if ( (b[0] && l[1]) || (b[1] && l[0]) ){
							h["M12_bl_"+p.first + "_triggered"]->Fill(diJet.M());
						}

						h["M12_" + p.first + "_triggered"]->Fill(diJet.M());
					}
				}
			}


		++ncand;
	}
	std::cout<<"Number of Candidates: "<<ncand<<std::endl;
	int colors[]= {kCyan+2,kYellow-6,kMagenta+2,kRed+2,kYellow+2,kGreen+2,kBlue-6};
	for(const auto & ih : h){
		ih.second->Scale(1./analysis.luminosity());
		if(ih.first.find("bb") != std::string::npos) ih.second->SetFillColor(colors[0]);
		if(ih.first.find("cc") != std::string::npos) ih.second->SetFillColor(colors[1]);
		if(ih.first.find("ll") != std::string::npos) ih.second->SetFillColor(colors[2]);
		if(ih.first.find("bl") != std::string::npos) ih.second->SetFillColor(colors[3]);
		if(ih.first.find("cl") != std::string::npos) ih.second->SetFillColor(colors[4]);
		if(ih.first.find("bc") != std::string::npos) ih.second->SetFillColor(colors[5]);
		ih.second->SetLineWidth(1.5);
		ih.second->SetLineColor(1);
		ih.second->Draw("hist same");
	}

	for(const auto & tr : trigger){
		for(const auto & p : wp){
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_bl_" + p.first + tr].get());
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_cl_" + p.first + tr].get());
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_bc_" + p.first + tr].get());
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_ll_" + p.first + tr].get());
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_cc_" + p.first + tr].get());
			stack["Flav_stack_" + p.first + tr]->Add(h["M12_bb_" + p.first + tr].get());
		}
	}


	analysis.SetupStandardOutputFile("falvourComposition");
	analysis.getOutputFile()->cd();
	for(const auto & ih : h){
		ih.second->Write();
	}

	std::map<std::string,std::unique_ptr<TCanvas> > can;

	for(const auto & st : stack){
		st.second->Write();
//		can[st.first] = std::make_unique<TCanvas>( ("can_" + st.first).c_str(),("can_" + st.first).c_str(),1000,800 );
//		st.second->Draw();
//		can[st.first]->Write();
	}


	analysis.getOutputFile()->Close();

}


