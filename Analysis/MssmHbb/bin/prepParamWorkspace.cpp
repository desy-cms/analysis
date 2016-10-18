#include "boost/program_options.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <memory>

#include <vector>

//Root includes
#include "TH1.h"
#include "TFile.h"
#include "TSystem.h"
#include "TDirectory.h"

//RooFit includes
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataHist.h"

#include "Analysis/BackgroundModel/interface/RooQuadGausExp.h"

namespace
{
  const size_t ERROR_IN_COMMAND_LINE = 1;
  const size_t SUCCESS = 0;
  const size_t ERROR_UNHANDLED_EXCEPTION = 2;
  const size_t ERROR_BG_WORKSPACE = 3;
  const size_t ERROR_SIGNAL_WORKSPACE = 4;
} // namespace

using namespace std;
using namespace boost::program_options;

//defin >> stream operator to be used in boost::po
namespace std {
	istream& operator>>(istream& in, pair<string,string>& ss) {
	string s;
  	  in >> s;
  	  const size_t sep = s.find(':');
  	  if(sep ==string::npos) s.find(',');
  	  if (sep==string::npos) {
	  ss.first = string();
    	ss.second = s;
  	  } else {
	  ss.first  = s.substr(0,sep);
    	ss.second = s.substr(sep+1);
  	  }
  	  return in;
	}
}

typedef unique_ptr<TFile> pTFile;
typedef map<string, map<string, pair<string,string> > > myMap;

void setup_bg(const string& in_path,const string& out_path, TH1& data_obs);
void setup_signal(const string& in_path, const vector<string>& syst);

int main(int argc, char ** argv){
	try {

		//list of the inputs:
		const string cmsswBase = getenv("CMSSW_BASE");
		string input_background;
		string data_obs = cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
		vector<string> input_signal = {
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_250",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_300",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_350",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_400",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_500",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_600",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_700",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_900",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_1100",
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_1300",
								};
		vector<string> systematics = {"JES","JER"};
		string output = cmsswBase + "/src/Analysis/MssmHbb/output/bg_fit/workspace/background_workspace.root";

		options_description desc("Options");
		desc.add_options()
				("help,h", "Print help messages")
				("background,b", value<string>(&input_background)->default_value(cmsswBase + "/src/Analysis/MssmHbb/output/bg_fit/workspace/bg.root"), "Input background workspace")
				("signal,s", value<vector<string> >(&input_signal)->multitoken(),"Input std::vector<signal folder>" )
				("data_obs,d", value<string>(&data_obs),"Input file with observed data")
				("syst_errors,e",value<vector<string> >(&systematics)->multitoken(),"std::vector<systematic error>" )
				("output,o", value<string>(&output),"Output for background file.root" )
				;

		//Boost container for input arguments.
		variables_map vm;
		try {
			store(parse_command_line(argc,argv,desc), vm);

			// --help option
			if(vm.count("help")){
				cout<<"Basic Command Line App to prepare signal/bg workspaces for parametric combine tool fitting"<<endl;
				cout<<desc<<endl;
				return SUCCESS;
			}

			notify(vm);
		} catch (error& e) {
			//Command line parcing can throw exception
			// should be catched
			cerr<<"ERROR: "<<e.what()<< endl << endl;
			cerr<< desc;
			return ERROR_IN_COMMAND_LINE;
		}

		//----------Main procedure----------
		TFile temp(data_obs.c_str(),"read");
		//Adjust bg workspace
		try {
			setup_bg(input_background,output,*((TH1D*) temp.Get("data_obs")));
		} catch (exception& e) {
			cerr<<e.what()<<endl;
			return ERROR_BG_WORKSPACE;
		}

		//Create signal workspace
		for(const auto& in : input_signal){
			try {
				setup_signal(in,systematics);
			} catch (exception& e) {
				cerr<<e.what()<<endl;
				return ERROR_SIGNAL_WORKSPACE;
			}
		}

	} catch (exception& e) {
		cerr<<"Unhandled Exception reached the top of main: "<<e.what()<<", application will now exit";
		return ERROR_UNHANDLED_EXCEPTION;
	}
	return SUCCESS;
}

void setup_bg(const string& in_path,const string& out_path, TH1& data_obs){
	if(gSystem->AccessPathName(in_path.c_str()) ) throw invalid_argument("Error: no bg file " + in_path);
	//Read input file
	TFile fIn(in_path.c_str(),"read");
	//Input workspace
	RooWorkspace& w = *( (RooWorkspace*)fIn.Get("workspace") );
	//Prepare RooDataHist for data_obs;
	RooRealVar mbb("mbb","mbb",data_obs.GetXaxis()->GetXmin(),data_obs.GetXaxis()->GetXmax());
	RooDataHist data("data_obs","data_obs",mbb,RooFit::Import(data_obs));
	//Prepare Bg normalisation variable WARNING - can be wrong!!!
	string bg_pdf_name = "background";
	if(w.pdf(bg_pdf_name.c_str()) == nullptr) throw invalid_argument("Error: no <background> pdf has been found in bg workspace");
	RooRealVar bg_norm((bg_pdf_name+"_norm").c_str(),"background_norm",data_obs.Integral());
	bg_norm.setConstant();

	w.import(data);
	w.import(bg_norm);
	w.Print("v");
	w.writeToFile(out_path.c_str());
}

void setup_signal(const string& in_folder, const vector<string>& syst){
	//Sanity check of input folder
	if(gSystem->AccessPathName(in_folder.c_str())) throw invalid_argument("Error in <prepParamWorkspace::setup_signal>: Wrong path: " + in_folder);
	//Sanity check of central WP
	string central = (in_folder + "/workspace/FitContainer_workspace.root");
	if(gSystem->AccessPathName(central.c_str())) throw invalid_argument("Error in <prepParamWorkspace::setup_signal>: file /workspace/FitContainer_workspace.root was not found in " + in_folder);
	//Read central workspace
	TFile fInCentral( (in_folder + "/workspace/FitContainer_workspace.root").c_str(),"read");
	RooWorkspace& wInCentral = *( (RooWorkspace*) fInCentral.Get("workspace") );
	RooRealVar& mbb = *( (RooRealVar*) wInCentral.var("mbb") );
	//Output workspace
	RooWorkspace w("workspace");
	//Signal shape parameters list
	vector<string> parameters = {"signal_norm","mean","sigmaL1","sigmaR1","sigmaL2","sigmaR2","tail_shift","tail_sigma","norm_g1","norm_g2"};
	//Form strings for RooFormulaVars
	map<string,string> formula;
	RooArgList nuisance_list; // Needed to add nuisances
	for(const auto &par : parameters){
		RooRealVar& vCent = (RooRealVar&) *wInCentral.var(par.c_str());
		formula[par] = to_string(vCent.getValV()) + " + ";
	}
	//Loop over the system. sources
	for(const auto& s : syst){
		//Read Up and Down TFiles
		string dnName = (in_folder + "/" + s + "_Up/workspace/FitContainer_workspace.root");
		if(gSystem->AccessPathName(dnName.c_str()) ) throw invalid_argument("Error in <prepParamWorkspace::setup_signal>: No /" + s + "_Up/workspace/FitContainer_workspace.root in " + in_folder);
		TFile fInUp( (dnName).c_str(),"read" );
		string upName = in_folder + "/" + s + "_Down/workspace/FitContainer_workspace.root";
		if(gSystem->AccessPathName(upName.c_str()) ) throw invalid_argument("Error in <prepParamWorkspace::setup_signal> No /" + s + "_Down/workspace/FitContainer_workspace.root in " + in_folder);
		TFile fInDown( (upName).c_str(),"read");
		//Get workspaces:
		RooWorkspace& wInUp = *( (RooWorkspace*) fInUp.Get("workspace") );
		RooWorkspace& wInDown = *( (RooWorkspace*) fInDown.Get("workspace") );
		//RooRealVar for nuisance:
		RooRealVar nuisance(s.c_str(),s.c_str(),0,-4,4);
		for(const auto &par : parameters){
			RooRealVar& vUp   = (RooRealVar&) *wInUp.var(par.c_str());
			RooRealVar& vDown = (RooRealVar&) *wInDown.var(par.c_str());
			string f = "("+ to_string(vUp.getValV()) + " - " + to_string(vDown.getValV()) +") * 0.25 * " + s + " ";
			formula[par] += f;
			if(s != syst.back()) formula[par] += "+ ";
		}
		w.import(nuisance);
		nuisance_list.add(*w.var(nuisance.GetName()));
	}
	//Finaly make a RooFormulaVars:
	for(const auto &par : parameters){
		RooFormulaVar fPar( (par).c_str(),(par).c_str(),formula[par].c_str(),nuisance_list);
		w.import(fPar);
	}
	//Create new RooQuadGausExp as a function of Nuisnace 1 : JES
	analysis::backgroundmodel::RooQuadGausExp func("signal","signal",mbb,(RooFormulaVar&) *w.function("mean"), (RooFormulaVar&) *w.function("sigmaL1"),
			(RooFormulaVar&) *w.function("sigmaL2"), (RooFormulaVar&) *w.function("sigmaR1"),
			(RooFormulaVar&) *w.function("sigmaR2"), (RooFormulaVar&) *w.function("tail_shift"),
			(RooFormulaVar&) *w.function("tail_sigma"), (RooFormulaVar&) *w.function("norm_g1"), (RooFormulaVar&) *w.function("norm_g2"));
	w.import(func);
	w.Print("v");
	string out_path = in_folder + "/workspace/signal_workspace.root";
	w.writeToFile(out_path.c_str());
	cout<<"INFO: file " + out_path +" was created"<<endl;
}
