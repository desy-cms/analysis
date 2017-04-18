/*
 * normToTanB30.cpp
 *
 *  Created on: 7 Mar 2017
 *      Author: shevchen
 */

/*
 * Macro to produce signal templates normalised to
 * the MSSM cross-section at 13 TeV for fixed value
 * of tan Beta. In particular value of tanB=30 is taken
 * to be compatible to the 8 and 7 TeV results
 */

//system includes
#include <iostream>				// std::cout
#include "boost/filesystem.hpp"	// boost::filesystem::path

//ROOT includes
#include "TClass.h"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TKey.h"
//#include "TIter.h"

//user libs
#include "Analysis/MssmHbb/interface/utilLib.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"

using namespace std;
namespace fs = boost::filesystem;

void ScaleSignalTemplates(const map<int,string>& signal_templates, const std::string& path_to_mssm_xsections,const double& tanB = 30, const std::string& output_dir = "");
void ScaleSignalPDF(const std::string& path, const double& xsection);
void CopySignalFile(const std::string& path_from, const std::string& path_to);
string GetOutputName(const std::string& input_template, const std::string& output_dir , const double& tanB );

int main(int argc, const char ** argv){

	//value of tanB to be used
	const double tanB = 30;
	//output dir path
	const string output_dir = "";
	//path to mssm xsections
	const string path_xsections = mssmhbb::path_mssm_xsections;
	//signal templates map
	const map<int,string> signal_templates = mssmhbb::signal_templates;

	ScaleSignalTemplates(signal_templates,path_xsections,tanB,output_dir);

	return 0;
}

void ScaleSignalTemplates(const map<int,string>& signal_templates,const std::string& path_to_mssm_xsections, const double& tanB, const std::string& output_dir){
	/*
	 * Method to copy and scale signal templates
	 * accroding to tanB to output_dir
	 */
	mssm_xs_tools xs(path_to_mssm_xsections.c_str(),true);
	for(const auto& m : signal_templates){
		double bbASantander = xs.bbHSantander_A(m.first, tanB);
		//output name
		string output_name = GetOutputName(m.second, output_dir, tanB);
		//copy template
		CopySignalFile(m.second, output_name);
		//scale new templates
		ScaleSignalPDF(output_name,bbASantander);
	}
}

void CopySignalFile(const std::string& path_from, const std::string& path_to){
	/*
	 * Method to copy signal templates
	 */

	fs::copy_file(path_from,path_to,fs::copy_option::overwrite_if_exists);

}

string GetOutputName(const std::string& input_template, const std::string& output_dir, const double& tanB){
	/*
	 * Method to get name of the output updated signal template
	 */
	//input path
	fs::path p(input_template.c_str());
	//input file name WITHOUT extension
	string input_name = p.stem().string();
	//output directory
	string out_dir;
	out_dir = (output_dir != "") ? output_dir : p.parent_path().string();
	//output name
	string output_name = out_dir + "/" + input_name + "_NormToTanB" + to_string_with_precision(tanB, 2) + p.extension().string();

	return output_name;
}

void ScaleSignalPDF(const std::string& path, const double& xsection){
	/*
	 * Method to scale signal template to xsection
	 */
	TFile f(path.c_str(),"UPDATE");

	//Read all objects from file
	TIter next(f.GetListOfKeys());
	TKey *key;
	//loop over the keys
	while((key = (TKey*)next())){
		TClass &cl = *gROOT->GetClass(key->GetClassName());
		//only TH1 type classes
		if(!cl.InheritsFrom("TH1")) continue;
		TH1 &h = *(TH1*)key->ReadObj();
		string name = key->GetName();
//		if(name.find("QCD_Mbb") != string::npos || name == "data_obs" || name == "NumberOfGenEvents_afterMHat" ||
//				name == "NumberOfGenEvents_afterMHat_rewPU" || name == "TotalNumberOfGenEvents" || name == "xsection") continue;
//		else{
		if(name.find("bbH_Mbb") != string::npos){
			cout<<"weight before = "<<h.Integral()<<" xsec = "<<xsection<<" after = ";
			h.Scale(1./xsection);
			cout<<h.Integral()<<endl;
			h.Write(name.c_str(),TObject::kWriteDelete);
		}
	}
}
