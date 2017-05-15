/*
 * normToTanB30.cpp
 *
 *  Created on: 7 Mar 2017
 *      Author: shevchen
 */

/*
 * Macro to produce signal PDFs normalised to
 * the MSSM cross-section at 13 TeV for fixed value
 * of tan Beta. In particular value of tanB=30
 * at mhmax mu200 scenario is taken to be compatible
 * to the 8 and 7 TeV results
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

//RooFit includes
#include "RooFit.h"
#include "RooWorkspace.h"
#include "RooAbsFunc.h"
#include "RooFormulaVar.h"
#include "RooAbsPdf.h"
//#include "TIter.h"

//user libs
#include "Analysis/MssmHbb/interface/utilLib.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"

using namespace std;
namespace fs = boost::filesystem;

void ScaleSignalPDFs(const map<int,string>& signal_templates, const std::string& path_to_mssm_xsections,const double& tanB = 30, const std::string& output_dir = "");
void ScaleSignalPDF(const std::string& in_path,const std::string& path, const double& xsection);
string GetOutputName(const std::string& input_template, const std::string& output_dir , const double& tanB );
double GetRefCrossSection(mssm_xs_tools& xs_tool,const double& mA,const double& tanBref );

int main(int argc, const char ** argv){

	//value of tanB to be used
	auto tanB = 30;
	//output dir path
	auto output_dir = "";
	//path to mssm xsections
	auto path_xsections = mssmhbb::cmsswBase + "/src/Analysis/MssmHbb/macros/signal/newmhmax_mu200_13TeV.root";
	//signal templates map
	auto signal_templates = mssmhbb::signal_workspaces;

	ScaleSignalPDFs(signal_templates,path_xsections,tanB,output_dir);

	return 0;
}

void ScaleSignalPDFs(const map<int,string>& signal_templates,const std::string& path_to_mssm_xsections, const double& tanB, const std::string& output_dir){
	/*
	 * Method to copy and scale signal templates
	 * accroding to tanB to output_dir
	 */
	mssm_xs_tools xs(path_to_mssm_xsections.c_str(),true);
	for(const auto& m : signal_templates){
		auto xreftot = GetRefCrossSection(xs,m.first,tanB);
		//output name
		auto output_name = GetOutputName(m.second, output_dir, tanB);
		//scale new templates
		ScaleSignalPDF(m.second,output_name,xreftot);
	}
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

void ScaleSignalPDF(const std::string& in_path,const std::string& path, const double& xsection){
	/*
	 * Method to scale signal template to xsection
	 */
	TFile fIn(in_path.c_str(),"READ");

	//input workspace
	const RooWorkspace& wIn = *(RooWorkspace*) fIn.Get("workspace");
	//output workspace
	RooWorkspace wOut(wIn);
	wOut.SetName("workspace");
	//signal normlaisation
	RooFormulaVar & s_norm = *(RooFormulaVar*)wOut.function("signal_norm");
	s_norm.SetName("signal_norm_NotNormalised");
	RooFormulaVar new_s_norm("signal_norm","signal_norm",("signal_norm_NotNormalised * " + to_string(xsection)).c_str(),RooArgSet(s_norm));
	cout<<"Norm: "<<s_norm.getValV()<<" "<<new_s_norm.getValV()<<endl;

	wOut.import(new_s_norm);
	wOut.writeToFile(path.c_str());

	cout<<path<<" : DONE"<<endl;


}

double GetRefCrossSection(mssm_xs_tools& xs_tool,const double& mA,const double& tanBref ){
	/*
	 * Function to get reference cross-section
	 * According to 8+7 TeV receipe it is
	 * cross-section from mhmax+mu=200 scenario
	 */
	double xtot = 0;

	double bbAG = xs_tool.bbH5F_A(mA, tanBref);
	double AbbBr= xs_tool.br_Abb(mA, tanBref);

	double bbHG = xs_tool.bbH5F_H(mA, tanBref);
	double HbbBr= xs_tool.br_Hbb(mA, tanBref);

	xtot = AbbBr*bbAG + bbHG*HbbBr;
	return xtot;





}
