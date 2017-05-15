/*
 * FitTest.cpp
 *
 *  Created on: 27 Apr 2017
 *      Author: shevchen
 *      Macro to save only converged fits (Minuit status == 1)
 */
#include "TTree.h"
#include "TBranch.h"
#include "TObject.h"
#include "TCanvas.h"

#include "Analysis/MssmHbb/interface/utilLib.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"

using namespace std;

//inputs = ['berneff8_200_to_650_10GeV','berneff9_200_to_650_10GeV_G4','extnovoeffprod_200to650_10GeV_G4','superdijeteffprod2_200_to_650_10GeV_G4']

int main(){
	string fit_function = "berneff8_200_to_650_10GeV";
	string additional_folder = "03052017/";
	int nfits = 2;
	int nsigmas = 10;
	bool addSignal = true;
	int signal = 300;
	bool fixed_turnon = true;

	string folder_name = "FitTest_" + to_string(nfits) + "fits_" + to_string(nsigmas) + "sigma";
	if(addSignal) folder_name += "_signal" + to_string(signal);
	if(fixed_turnon) folder_name += "_fixedTurnOn";
	folder_name += "_" + fit_function;
	string file_path = mssmhbb::cmsswBase + "/src/Analysis/MssmHbb/bin/" + folder_name + "/" + folder_name + ".root";
	string output_folder_path = mssmhbb::cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/Background/";
	string output_pics_folder = output_folder_path + additional_folder + fit_function;
	if(fixed_turnon) output_pics_folder+= "_fixedTurnOn";
	CheckOutputDir(output_pics_folder);
	TFile f(file_path.c_str());
	auto &tree = *GetFromTFile<TTree>(f, "Tree");
	auto &branches = *tree.GetListOfBranches();
	for(const auto& obj : branches){
		TCanvas can("can","can",800,600);
		auto &br = *static_cast<TBranch*>(obj);
		string branch_name = br.GetName();
		tree.Draw(branch_name.c_str(),"chi_2 < 100");
		can.Print( (output_pics_folder + "/" + branch_name + ".pdf").c_str());
	}

	return 0;
}


