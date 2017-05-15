#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <memory>
#include <ostream>
#include <iostream>
#include <string>
#include <exception>

//Root includes
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TIterator.h"

//RooFit includes
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooHist.h"

//include boost
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "Analysis/MssmHbb/interface/utilLib.h"
#include "Analysis/Tools/interface/RooFitUtils.h"
#include "Analysis/BackgroundModel/interface/RooFitQuality.h"
#include "Analysis/MssmHbb/src/namespace_mssmhbb.cpp"

namespace fs = boost::filesystem;
using namespace boost::program_options;
using namespace analysis;

using namespace std;
using namespace RooFit;

void PrintRooFitResults(RooFitResult & r);
void Fit_study(const string& input_file,
		const int & max_fits,
		const std::string & pdf_name,
		const std::string & output,
		const bool & store_plots,
		const string& workspace_name,
		const int& nsigma,
		const int& mass,
		const bool& add_signal,
		const bool& fix_turnon);
void make_Plot(RooPlot &xframe1, RooPlot &xframe2, const int& i, const std::string & output);

const boost::program_options::variables_map ParseUserInput(const int& argc, const char* argv[]);
const std::string SetupOutputDir(const variables_map& user_input);

std::vector<RooRealVar> GetRooRealVars(RooWorkspace& w, const std::vector<std::string>& names);
void AddOutBranches(TTree &outtree,
					int&    minuit_status,
					double& edm,
					double& minNLL,
					double& chi_2,
					vector<string>& par_names,
					int& counter,
					double *par_in,
					double *par_out,
					double *epar_out,
					double& ns_postfit,
					double& ns_err_postfit,
					double& nbg_prefit,
					double& nbg_postfit
					);
vector<string> GetRooRealVarNames(RooWorkspace & w);
void FillArrayOfPdfPars(const vector<string>& names, RooWorkspace & w, double *pars, double *epars);
void FillArrayOfPdfPars(const vector<string>& names, RooWorkspace & w, double *pars);
void RandomiseVarsInWorkspace(RooWorkspace &w, const std::vector<RooRealVar>& init_vars, const int& nsigma, const bool& rand_turn_on);
double GetGausRandomisation(TRandom3& r, const double& mean, const double&sigma, const double& max, const double& min);
void SetVarsInWorkspace(RooWorkspace &w, const std::vector<RooRealVar>& vars);
std::string GetSignalPdfPath(const std::string & mass);
void SetConstantVars(RooWorkspace& w, const std::string& x_name);
void FixTurnOn(RooWorkspace &w);

int main(int argc, const char * argv[])
{

	//Parse User input
	variables_map user_input = ParseUserInput(argc,argv);
	//Setup output file
	string output_file = SetupOutputDir(user_input);

	int max_fits_		= user_input["max_fits"].as<int>();
	string input_file_	= user_input["input_file"].as<std::string>();
	string pdf_name_ 	= user_input["pdf_name"].as<std::string>();
	string output_		= user_input["output_file"].as<std::string>();
	string wspace_name_	= user_input["workspace_name"].as<std::string>();
	int nsigma_			= user_input["nsigma"].as<int>();
	bool plots_     	= user_input["plots"].as<bool>();
	bool add_signal_	= user_input["add_signal"].as<bool>();
	int mass_			= user_input["mass"].as<int>();
	bool fix_turnon_	= user_input["fix_turnon"].as<bool>();

	Fit_study(input_file_,max_fits_,pdf_name_,output_file,plots_,wspace_name_,nsigma_,mass_,add_signal_,fix_turnon_);

	return  0;
}

void Fit_study(	const string& input_file,
				const int & max_fits,
				const std::string & pdf_name,
				const std::string & output,
				const bool & store_plots,
				const string& workspace_name,
				const int& nsigma,
				const int& mass,
				const bool& add_signal,
				const bool& fix_turnon){
	/*
	 * Function to perform a fit study.
	 */
	std::cout << std::fixed;
	TH1::SetDefaultSumw2();
	//Get workspace
	auto &w 		= *GetFromTFile<RooWorkspace>(input_file, workspace_name);
	auto &w_signal	= *GetFromTFile<RooWorkspace>(GetSignalPdfPath(to_string(mass)),"workspace");
	//Get pdf and data
	auto &pdf 	= *GetFromRooWorkspace<RooAbsPdf>(w, pdf_name);
	auto &x		= *GetFromRooWorkspace<RooRealVar>(w, "mbb");
	auto &data	= *GetFromRooWorkspace<RooAbsData>(w, "data_container");
	auto &signal_pdf = *GetFromRooWorkspace<RooAbsPdf>(w_signal,"signal");
	//Make signal vars constant:
	SetConstantVars(w_signal,"mbb");
	//Setup output TTree
	TFile outputFile((output+ "/" + output + ".root").c_str(), "RECREATE");
	TTree outtree("Tree","Tree");
	backgroundmodel::RooFitQuality q;

	double chi_2=0.;
	double edm = -100, minNLL = 100;
	int minuit_status = -100;
	int counter = 0;
	double pars_out[10], epars_out[10], pars_in[10];
	double ns_prefit, ns_postfit, nbg_prefit, nbg_postfit, ns_err_postfit;
	vector<string> par_names = GetRooRealVarNames(w);
	//Get initial values of the parameters - needed to reset value at each step of the Toy production
	vector<RooRealVar> roovars = GetRooRealVars(w,par_names);
	//Output branches
	AddOutBranches(outtree,minuit_status,edm,minNLL,chi_2,par_names,counter,pars_in,pars_out,epars_out,ns_postfit,ns_err_postfit,nbg_prefit,nbg_postfit);
	x.setRange("fit_range",200,650);
	for(int i =1; i<=max_fits;++i){
		/*
		 * Toy is produced by alternative function
		 * and fitted with backgorund pdf
		 */

		//Fit loop
		cout<<"\nFit number "<<i<<"\n\n"<<endl;
		string fit_number = to_string(i);

		//Set to initial (JUST TO bE SAFE)
		SetVarsInWorkspace(w, roovars);
		//Randomise initial parameters
		RandomiseVarsInWorkspace(w, roovars,nsigma,false);
		//Fix turn-on?
		if(fix_turnon) FixTurnOn(w);
		//And write them to the tree
		FillArrayOfPdfPars(par_names,w,pars_in);

		ns_prefit = 0;
		RooRealVar roo_nsignal 			("roo_nsignal","roo_nsignal",ns_prefit); roo_nsignal.setConstant(false);
		RooRealVar roo_ntot     		("roo_ntot","roo_ntot",data.sumEntries()); roo_ntot.setConstant();
		RooFormulaVar roo_nbkg 			("roo_nbkg","roo_nbkg","roo_ntot - roo_nsignal",RooArgList(roo_ntot,roo_nsignal));
		RooAddPdf  pdf_SpBg             ("pdf_SpBg","pdf_SpBg",RooArgList(signal_pdf,pdf),RooArgList(roo_nsignal,roo_nbkg));
		RooFitResult *fitResult;

		auto &xframe = *x.frame();	//Top frame
		auto &xframe2 = *x.frame();	//Bottom frame

		//Fit
		if(add_signal){
			fitResult = pdf_SpBg.fitTo(data,RooFit::Range(200,650), RooFit::Save(), RooFit::PrintLevel(3), RooFit::Verbose(1), RooFit::Minimizer("Minuit2", "migrad"), RooFit::Hesse(1) , RooFit::Offset(1) );
		}
		else
			fitResult = pdf.fitTo(data, RooFit::Range("fit_range"), RooFit::Save(), RooFit::PrintLevel(3), RooFit::Verbose(1), RooFit::Minimizer("Minuit2", "migrad"), RooFit::Hesse(1) , RooFit::Offset(1) );

		data.plotOn(&xframe,RooFit::MarkerSize(0.8), RooFit::Name("data_curve"));
		if(add_signal){
			pdf_SpBg.plotOn(&xframe,RooFit::Name("fit_curve"),RooFit::LineColor(kRed));
	  		pdf_SpBg.plotOn(&xframe, RooFit::Name("bkg"), Components("background"), LineStyle(kDashed), LineColor(kRed), LineWidth(4));
	  		pdf_SpBg.plotOn(&xframe, RooFit::Name("sgn"), Components("signal"), LineStyle(kDashed), LineColor(kGreen+2), LineWidth(4));
		}
		else pdf.plotOn (&xframe,RooFit::Name("fit_curve"),RooFit::LineColor(kRed));
		make_Plot(xframe,xframe2,i,output);

		auto chi2ndf = q.chiSquare(xframe,"fit_curve","data_curve",fitResult->floatParsFinal().getSize());
		PrintRooFitResults(*fitResult);
		chi2ndf.Print();

		//Fill output variables
		chi_2 = chi2ndf.chi2;
		FillArrayOfPdfPars(par_names,w,pars_out,epars_out);
	  	minuit_status = fitResult->status();
		edm = fitResult->edm();
		minNLL = fitResult->minNll();
		ns_postfit = roo_nsignal.getValV();
		nbg_postfit = roo_nbkg.getValV();
		ns_err_postfit = roo_nsignal.getError();


		std::cout << "ns_prefit      = " << ns_prefit << std::endl; 	// number of signal before the fit
		std::cout << "nbg_prefit     = " << roo_ntot.getValV() - ns_prefit<< std::endl;
		std::cout <<" ntot_pretfit   = " << roo_ntot.getValV()<<endl;
	  	std::cout << "ns_postfit     = " << ns_postfit << std::endl; 	// number of signal after the fit
	  	std::cout << "nbg_postfit    = " << nbg_postfit<< std::endl;
	  	std::cout <<" ntot_postfit   = "<<roo_nbkg.getValV() + roo_nsignal.getValV()<<endl;
	  	std::cout << "err_s_postfit  = " << ns_err_postfit << std::endl; // error of n_s from the fit

		outtree.Fill();
		++counter;
	}


	outputFile.cd();
	outtree.Write();
	outputFile.Close();
	cout << "End of code :)" << endl;

}

void make_Plot(RooPlot &xframe1, RooPlot &xframe2, const int& i, const std::string & output){
	std::string ntoy = std::to_string(i);
	RooHist &hpull = *xframe1.pullHist();
	hpull.SetMarkerSize(0.1);
	xframe2.addPlotable(&hpull,"P");
	xframe2.SetMinimum(-5.);
	xframe2.SetMaximum(+5.);

	TCanvas canvas;
        canvas.SetCanvasSize(500,500);
        canvas.cd();

	TPad* pad1;
	pad1 = new TPad("pad1","",0,0.1,1,1);
	pad1->SetBottomMargin(0.2);
        pad1->SetRightMargin(0.05);
        pad1->SetLeftMargin(0.16);
        pad1->Draw();
        pad1->cd();
        xframe1.GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
        xframe1.GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
        xframe1.GetYaxis()->SetTitleSize(0.038);
        xframe1.GetYaxis()->SetTitleOffset(1.6);
        xframe1.GetYaxis()->SetLabelSize(0.033);
	xframe1.Draw();
        TLegend *leg1 = new TLegend(0.67,0.63,0.93,0.89);
        leg1->SetFillColor(kWhite);
        leg1->SetLineColor(kWhite);
        leg1->AddEntry("data_curve","Toy Data", "P");
        leg1->AddEntry("fit_curve","Signal + background","L");
        leg1->AddEntry("bkg","Background only", "L");
        leg1->AddEntry("sgn","Signal only", "L");
        leg1->Draw("same");
	
	canvas.cd();
        TPad *pad2 = new TPad("pad2","",0,0.0,1,0.265);
        pad2->SetTopMargin(1);
        pad2->SetBottomMargin(0.33);
        pad2->SetLeftMargin(pad1->GetLeftMargin());
        pad2->SetRightMargin(pad1->GetRightMargin());
        pad2->SetGridy();
        pad2->Draw();
        pad2->cd();
        xframe2.SetTitle("");
        xframe2.GetXaxis()->SetTitleSize(0.15);
        xframe2.GetXaxis()->SetTitleOffset(0.9);
        xframe2.GetXaxis()->SetLabelSize(0.115);
        xframe2.GetXaxis()->SetLabelOffset(0.010);
        xframe2.SetYTitle("Pulls");
        xframe2.GetYaxis()->CenterTitle(kTRUE);
        xframe2.GetYaxis()->SetTitleSize(0.14);
        xframe2.GetYaxis()->SetTitleOffset(0.4);
        xframe2.GetYaxis()->SetNdivisions(3,5,0);
        xframe2.GetYaxis()->SetLabelSize(0.115);
        xframe2.GetYaxis()->SetLabelOffset(0.011);
        xframe2.Draw();

	canvas.SaveAs((output+std::string("/")+std::string("Fit_")+ntoy+std::string("_linear.pdf")).c_str());
	pad1->SetLogy();
        xframe1.GetYaxis()->SetRangeUser(0.1, xframe1.GetMaximum()*100);
        canvas.Modified();
        canvas.Update();
        canvas.SaveAs((output+std::string("/")+std::string("Fit_")+ntoy+std::string("_log.pdf")).c_str());
}

const boost::program_options::variables_map ParseUserInput(const int& argc, const char* argv[]){
	options_description generalOptions("General options");
    	generalOptions.add_options()
				("help,h", "Produce help message.");

        // Declare a group of options that will be allowed only on command line
		options_description cmdLineOptions("CMD Optional arguments");
		cmdLineOptions.add_options()
			("max_fits,n",value<int>()->required(),"Number of fits")
			("input_file,i",value<std::string>()->required(),"Input rooworkspace file ")
			("pdf_name,f",value<std::string>()->default_value("background"),"pdf name in the workspace")
			("workspace_name,w",value<std::string>()->default_value("workspace"),"Name of the RooWorkspace in the input file")
			("nsigma,s",value<int>()->default_value(10),"Number of sigmas to be used in randomisation")
			("output_file,o", value<std::string>()->default_value(""), "Output file name, if not specified will be created automatically")
			("plots,p", bool_switch()->default_value(false),"if specified - no plots will be stored")
			("mass,m", value<int>()->default_value(300),"mass point")
			("add_signal", bool_switch()->default_value(false),"if specified - Signal + Bg fit will be performed")
			("fix_turnon", bool_switch()->default_value(false),"if specified - Turn-on parameters will be fixed")
			;

        options_description all_options;
        all_options.add(generalOptions).add(cmdLineOptions);

   	variables_map output_vm;
    	store(command_line_parser(argc, argv).options(generalOptions).allow_unregistered().run(), output_vm);
    	notify(output_vm);

    	if (output_vm.count("help")) {
	    std::cout << all_options << std::endl;
	    exit(0);
	}

    	store(parse_command_line(argc, argv,all_options), output_vm);
    	try {
    		notify(output_vm);
    	}
    	catch (const required_option& ex) {
    		std::cerr << ex.what() << std::endl;
        	exit(-1);
    	}

    	return output_vm;
}

const std::string SetupOutputDir(const variables_map& user_input){
	/*
	 * Function to setup output firectory path.
	 *
	 * Setup is done according to the user input AND
	 * defined template
	 */
	auto user_path  = user_input["output_file"].as<string>();
	auto nsigma		= user_input["nsigma"].as<int>();
	auto nfits		= user_input["max_fits"].as<int>();
	auto add_signal = user_input["add_signal"].as<bool>();
	auto mass		= user_input["mass"].as<int>();
	auto fix_turnon = user_input["fix_turnon"].as<bool>();
	std::string oDir = user_path;
	//Pre-defined template is used
	oDir = "FitTest_" + to_string(nfits) + "fits_" + to_string(nsigma) + "sigma";
	if(add_signal) oDir += "_signal" + to_string(mass);
	if(fix_turnon) oDir += "_fixedTurnOn";
	if(user_path != "") oDir = oDir + "_" + user_path;
	CheckOutputDir(oDir);
	std::cout << "Writing results to " << oDir << std::endl;
	return oDir;
}

void AddOutBranches(TTree &outtree,
					int&    minuit_status,
					double& edm,
					double& minNLL,
					double& chi_2,
					vector<string>& par_names,
					int& counter,
					double *par_in,
					double *par_out,
					double *epar_out,
					double& ns_postfit,
					double& ns_err_postfit,
					double& nbg_prefit,
					double& nbg_postfit
					){
	/*
	 * Function to add special branches to the TTree
	 */
	outtree.Branch("minuit_status",     &minuit_status,     "minuit_status/I");
	outtree.Branch("edm",				&edm,				"edm/D");
	outtree.Branch("minNLL",			&minNLL,			"minNLL/D");
	outtree.Branch("chi_2",				&chi_2,				"chi_2/D");
	outtree.Branch("counter",			&counter,			"counter/I");
	outtree.Branch("nbg_prefit",		&nbg_prefit,		"nbg_prefit/D");
	outtree.Branch("nbg_postfit", 		&nbg_postfit, 		"nbg_postfit/D");
	outtree.Branch("ns_postfit", 		&ns_postfit, 		"ns_postfit/D");
	outtree.Branch("ns_err_postfit",	&ns_err_postfit,	"ns_err_postfit/D");

	//branches with bg and signal parameters TODO: introduce lambda-expression
	auto lambda_i = [](TTree & tree, auto vec, auto pars) { for(auto & v : vec) {auto index = &v - &vec[0]; tree.Branch((v + "_in").c_str(), &pars[index] , (v + "_in/D").c_str());}};
	auto lambda_n = [](TTree & tree, auto vec, auto pars) { for(auto & v : vec) {auto index = &v - &vec[0]; tree.Branch(v.c_str(), &pars[index] , (v + "/D").c_str());}};
//	auto lambda_din_out = [](TTree & tree, auto vec, auto pars_in, auto pars_out)  { for(auto & v : vec) {auto index = &v - &vec[0]; tree.Branch( ("d_" + v + "_in_out").c_str, &(pars_in[index] - pars_out[index]) , ("d_" + v + "_in_out/D").c_str() ); }};
//	auto lambda_e = [](TTree & tree, auto vec, auto pars) { for(auto & v : vec) {auto index = &v - &vec[0]; tree.Branch((v + "_err").c_str(), &pars[index] , (v + "_err/D").c_str());}};
	lambda_i(outtree,par_names,par_in);
	lambda_n(outtree,par_names,par_out);
//	lambda_din_out(outtree,par_names,par_in,par_out);
//	lambda_e(outtree,par_names,epar_out);
}

vector<string> GetRooRealVarNames(RooWorkspace & w){
	/*
	 * Function to get a pair of NAME : DOUBLE & from RooRealVar
	 *
	 * To be written in TTree
	 */
	vector<string> par_names;
	TIterator &s_iter = *w.componentIterator();
	auto *s_var = s_iter.Next();
		while(s_var){
			string class_name = s_var->ClassName();
			string name = s_var->GetName();
			if(class_name == "RooRealVar"){
				if(name != "mbb" && name != "weight"){
					par_names.push_back(name);
				}
			}
			s_var = s_iter.Next();
		}
		return par_names;
}

void FillArrayOfPdfPars(const vector<string>& names, RooWorkspace & w, double *pars, double *epars){
	/*
	 * Function to fill the arrays of pdf parameters
	 */
	for(const auto& v : names){
		auto index = &v - &names[0];
		pars[index]  = w.var(v.c_str())->getValV();
		epars[index] = w.var(v.c_str())->getError();
	}
}

void FillArrayOfPdfPars(const vector<string>& names, RooWorkspace & w, double *pars){
	/*
	 * Function to fill the arrays of pdf parameters
	 */
	for(const auto& v : names){
		auto index = &v - &names[0];
		pars[index]  = w.var(v.c_str())->getValV();
	}
}

std::vector<RooRealVar> GetRooRealVars(RooWorkspace& w, const std::vector<std::string>& names){
	/*
	 * Function to get copies of RooRealVars from the
	 * Workspace
	 */
	vector<RooRealVar> v;
	for(const auto& n : names){
		RooRealVar var(*w.var(n.c_str()),n.c_str());
		v.push_back(var);
	}
	return v;
}

void RandomiseVarsInWorkspace(RooWorkspace &w, const std::vector<RooRealVar>& init_vars, const int& nsigma, const bool& rand_turn_on){
	/*
	 * Function to randomise vars in workspace
	 */
	TRandom3 r(0);
	for(const auto& v : init_vars){
		string name = v.GetName();
		if( (name.find("turnon_novoeff") != string::npos || name.find("slope_novoeff") != string::npos) && !rand_turn_on) continue;
		auto init_val = v.getValV();
		auto init_err = v.getError();
		auto up_range = v.getMax();
		auto low_range= v.getMin();

		auto &var = *GetFromRooWorkspace<RooRealVar>(w,v.GetName());
		var.setVal(GetGausRandomisation(r,init_val,init_err * nsigma, up_range, low_range));
		cout<<"Randomise var: "<<var.GetName()<<" ";
		var.Print();
	}
}

double GetGausRandomisation(TRandom3& r, const double& mean, const double&sigma, const double& max, const double& min){
	/*
	 * Gaus randomisation within the limits
	 */
	double res = r.Gaus(mean,sigma);
	if(res >= max || res <= min) res = GetGausRandomisation(r,mean,sigma,max,min);
	return res;
}

void PrintRooFitResults(RooFitResult & r){
		/*
		 * Information about pre- and post-fit pars
		 */
		std::cout << "\nconstant parameters:" << std::endl;
  		r.constPars().Print("v");
  		std::cout << "\nfloating parameters (init):" << std::endl;
  		r.floatParsInit().Print("v");
  		std::cout << "\nfloating parameters (final):" << std::endl;
  		r.floatParsFinal().Print("v");
}

std::string GetSignalPdfPath(const std::string & pdf){
	/*
	 * Function to get signal root file path
	 */
	string outname = "";
	if(pdf == "300" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-300/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "350" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-350/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "400" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-400/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "500" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-500/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "600" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-600/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "700") {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-700/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "900" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-900/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "1100" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-1100/workspace/FitContainer_workspace.root";
	}
	else if(pdf == "1300" ) {
		outname = mssmhbb::cmsswBase +  "/src/Analysis/MssmHbb/output/ReReco_signal_M-1300/workspace/FitContainer_workspace.root";
	}
	else{
		throw invalid_argument("Invalid mass: " + pdf + " has been provided.");
	}

	return outname;
}

void SetConstantVars(RooWorkspace& w, const std::string& x_name){
	/*
	 * Make all vars constant
	 */
	TIterator &s_iter = *w.componentIterator();
	auto *s_var = s_iter.Next();
	while(s_var){
		string class_name = s_var->ClassName();
		string name = s_var->GetName();
		if(class_name == "RooRealVar"){
			auto &var = *GetFromRooWorkspace<RooRealVar>(w,name);
			if(name != x_name){
				var.setConstant();
			}
		}
		s_var = s_iter.Next();
	}
}

void SetVarsInWorkspace(RooWorkspace &w, const std::vector<RooRealVar>& vars){
	/*
	 * Function to set vars in the workspace according to vars
	 */
	std::cout<<"\nWorkspace content: "<<w.GetName()<<" VARS"<<std::endl;
	w.Print("v");
	for(const auto& v : vars){
		RooRealVar &init = *w.var(v.GetName());
		init.setVal(v.getValV());
		init.setError(v.getError());
		init.setMax(v.getMax());
		init.setMin(v.getMin());
		init.Print();
	}
}

void FixTurnOn(RooWorkspace &w){
	/*
	 * Function to change parameter range
	 * hard coded
	 */
	w.var("turnon_novoeff")->setConstant();
	w.var("slope_novoeff")->setConstant();
//	if(pdf_name == "extnovoeffprod_G4_R1"){
//		w.var("turnon_novoeff")->setConstant();
//		w.var("slope_novoeff")->setConstant();
//	}
//	else if (pdf_name == "berneffprod9par_G4_R1"){
//		w.var("slope_novoeff")->setConstant();
//		w.var("turnon_novoeff")->setConstant();
//	}
//	else if (pdf_name == "berneffprod8par_G4_R1"){
//		w.var("slope_novoeff")->setConstant();
//		w.var("turnon_novoeff")->setConstant();
//	}
//	else if (pdf_name == "dijetv3logprod_G4_R1"){
//		w.var("slope_bg")->setConstant();
//		w.var("turnon_bg")->setConstant();
//	}
//	else if (pdf_name == "superdijeteffprod2_G4_R1"){
//		w.var("slope_novoeff")->setConstant();
//		w.var("turnon_novoeff")->setConstant();
//	}
//	else{
//		throw std::invalid_argument("Wrong pdf name: " + pdf_name + ". Couldn't fix the turn-on as required.");
//	}
}
