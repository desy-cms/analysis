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
#include "RooBukinPdf.h"
#include "RooEffProd.h"
#include "RooNovosibirsk.h"

#include "Analysis/BackgroundModel/src/classes.h"
#include "Analysis/BackgroundModel/interface/RooDoubleGausExp.h"

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
void setup_bg2(const string& in_path,const string& out_path, TH1& data_obs);
void setup_signal(const string& in_path, const vector<string>& syst);
string SignalModel(const vector<string>& parameters);

int main(int argc, char ** argv){
	try {

		//list of the inputs:
		const string cmsswBase = getenv("CMSSW_BASE");
		string input_background;
		string data_obs = cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_clange-SUSYGluGluToBBHToBB_M-600_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";
		vector<string> input_signal = {
				cmsswBase + "/src/Analysis/MssmHbb/output/mass_point_200",
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
		vector<string> systematics = {"JES","JER","PtEff"};
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
//			setup_bg2(input_background,output,*((TH1D*) temp.Get("data_obs")));
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
	w.var("mbb")->setBins(100000);
	//Prepare RooDataHist for data_obs;
	RooRealVar mbb("mbb","mbb",data_obs.GetXaxis()->GetXmin(),data_obs.GetXaxis()->GetXmax());
	mbb.setBins(100000);
//	RooDataHist data("data_obs","data_obs",mbb,RooFit::Import(data_obs));

	//Prepare Bg normalisation variable WARNING - can be wrong!!!
	string bg_pdf_name = "background";
	if(w.pdf(bg_pdf_name.c_str()) == nullptr) throw invalid_argument("Error: no <background> pdf has been found in bg workspace");
	RooRealVar bg_norm((bg_pdf_name+"_norm").c_str(),"background_norm",data_obs.Integral());
	bg_norm.setConstant();

//	w.var("peak_novops")->setConstant();
//	w.var("tail_novops")->setConstant();
//	w.var("width_novops")->setConstant();
//	w.var("peak_novops")->Print();
//	w.var("tail_novops")->Print();
//	w.var("width_novops")->Print();
//	w.var("peak_novops")->removeRange();
//	w.var("tail_novops")->removeRange();
//	w.var("width_novops")->removeRange();
//	w.var("peak_novops")->removeError();
//	w.var("tail_novops")->removeError();
//	w.var("width_novops")->removeError();
//        w.var("peak_novops")->Print();
//        w.var("tail_novops")->Print();
//        w.var("width_novops")->Print();

//	w.import(data);
//	w.import(bg_norm);
//	w.Print("v");
//	w.writeToFile(out_path.c_str());

	//Try another workspace
	RooWorkspace wOut("workspace");
	wOut.import(*(RooAbsPdf*)w.pdf(bg_pdf_name.c_str()));
//	wOut.import(*(RooFormulaVar*)w.function("background_phasespace"));
//	wOut.import(*(RooAbsPdf*)w.pdf("background_novops"));

	TH1& h = *w.pdf(bg_pdf_name.c_str())->createHistogram("QCD",mbb,RooFit::Binning(100000,240.,1700));
	h.Scale(data_obs.Integral() / h.Integral());
	h.SetTitle("data_obs");
	h.SetName("data_obs");
	RooDataHist h_data("data_obs","data_obs",mbb,RooFit::Import(h));
	wOut.import(h_data);
	wOut.import(bg_norm);
	wOut.Print("v");
	wOut.writeToFile(out_path.c_str());
}

void setup_bg2(const string& in_path,const string& out_path, TH1& data_obs){
	if(gSystem->AccessPathName(in_path.c_str()) ) throw invalid_argument("Error: no bg file " + in_path);
	//Read input file
	TFile fIn(in_path.c_str(),"read");
	//Input workspace
	RooWorkspace& w = *( (RooWorkspace*)fIn.Get("workspace") );
	//Prepare RooDataHist for data_obs;
	RooRealVar& mbb = *w.var("mbb");
	RooDataHist data("data_obs","data_obs",mbb,RooFit::Import(data_obs));
	//Read shape variables
	RooRealVar& peak_novops =  *w.var("peak_novops");
	RooRealVar& tail_novops = *w.var("tail_novops");
	RooRealVar& width_novops = *w.var("width_novops");
	RooWorkspace wOut("workspace");
	//Prepare Bg normalisation variable WARNING - can be wrong!!!
	string bg_pdf_name = "background";
	if(w.pdf(bg_pdf_name.c_str()) == nullptr) throw invalid_argument("Error: no <background> pdf has been found in bg workspace");
	RooRealVar bg_norm((bg_pdf_name+"_norm").c_str(),"background_norm",data_obs.Integral());
	bg_norm.setConstant();

	RooRealVar peak("peak","peak",0,-50,50);
	RooFormulaVar f_peak("f_peak","f_peak", (to_string(peak_novops.getValV()) + " + " + to_string(peak_novops.getError()) + " * @0").c_str(),RooArgSet(peak) );
	RooRealVar tail("tail","tail",0,-50,50);
	RooFormulaVar f_tail("f_tail","f_tail", (to_string(tail_novops.getValV()) + " + " + to_string(tail_novops.getError()) + " * @0").c_str(),RooArgSet(tail) );
	RooRealVar width("width","width",0,-50,50);
	RooFormulaVar f_width("f_width","f_width", (to_string(width_novops.getValV()) + " + " + to_string(width_novops.getError()) + " * @0").c_str(),RooArgSet(width) );

	RooFormulaVar phasespace((bg_pdf_name + "_phasespace").c_str(),
				  ("(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * " + string(mbb.GetName()) + ")))) - (1.326-1.)").c_str(),RooArgList(mbb));
	RooNovosibirsk novo((bg_pdf_name + "_novosibirsk").c_str(),(bg_pdf_name + "_novosibirsk").c_str(),mbb,f_peak,f_width,f_tail);
	RooEffProd novopsprod(bg_pdf_name.c_str(), bg_pdf_name.c_str(), novo, phasespace);
	wOut.import(mbb);
	wOut.import(data);
	wOut.import(bg_norm);
	wOut.import(peak);
	wOut.import(width);
	wOut.import(tail);
//	wOut.import(f_peak);
//	wOut.import(f_width);
//	wOut.import(f_tail);
//	wOut.import(phasespace);
//	wOut.import(novo);
	wOut.import(novopsprod);
	wOut.Print("v");
	wOut.writeToFile(out_path.c_str());
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
	mbb.setRange(240,1700);	//needed to define the function
	mbb.setBins(100000);
	//Output workspace
	RooWorkspace w("workspace");
	//Signal shape parameters list
	vector<string> parameters;
	vector<string> var_parameters; //parameters list that are not constant
	RooArgList vars = wInCentral.allVars();
	auto iter = vars.createIterator();
	RooRealVar *v;
	while ((v = (RooRealVar*) iter->Next()) ){
		if(string(v->GetName()) == string(mbb.GetName())) continue;
		parameters.push_back( string(v->GetName()) );
	}
	//Form strings for RooFormulaVars
	map<string,string> formula;
	RooArgList nuisance_list; // Needed to add nuisances
	for(const auto &par : parameters){
		RooRealVar& vCent = (RooRealVar&) *wInCentral.var(par.c_str());
		formula[par] = to_string(vCent.getValV());
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
			if(!wInUp.var(par.c_str())->hasError()  && par != "signal_norm" ) continue;
			RooRealVar& vUp   = (RooRealVar&) *wInUp.var(par.c_str());
			RooRealVar& vDown = (RooRealVar&) *wInDown.var(par.c_str());
			string f = "+ ("+ to_string(vUp.getValV()) + " - " + to_string(vDown.getValV()) +") * 0.25 * " + s + " ";
			formula[par] += f;
			var_parameters.push_back(par);
		}
		w.import(nuisance);
		nuisance_list.add(*w.var(nuisance.GetName()));
	}
	//Finaly make a RooFormulaVars:
	for(const auto &par : parameters){
		if(find(var_parameters.begin(),var_parameters.end(),par) != var_parameters.end()){	//If parameter is var = f(nuisances)
			RooFormulaVar fPar( (par).c_str(),(par).c_str(),formula[par].c_str(),nuisance_list);
			w.import(fPar);
		}
		else {
//			if(par == "signal_norm") continue;
			RooRealVar constant(("f_"+par).c_str(),("f_"+par).c_str(),stod(formula[par].c_str()));
			constant.setConstant();
			w.import(constant);
			RooFormulaVar fconstant(par.c_str(),par.c_str(),"@0",RooArgSet(constant));
			w.import(fconstant);
		}
	}
	/* I believe there should me more elegant way to determine a type
	 * of RooFit Pdf. But curently - this is the inly solution that I see
	 */
	//Create new RooQuadGausExp as a function of Nuisnace 1 : JES
	string model = SignalModel(parameters);
	unique_ptr<RooAbsPdf> func;
	if(model == "bukin"){
		func = make_unique<RooBukinPdf>("signal","signal",mbb,(RooFormulaVar&) *w.function("Xp"),(RooFormulaVar&) *w.function("sigp"),
												(RooFormulaVar&)*w.function("xi"),(RooFormulaVar&) *w.function("rho1"),
												(RooFormulaVar&)*w.function("rho2"));
	}
	else if (model == "doublegausexp"){
		func = make_unique<analysis::backgroundmodel::RooDoubleGausExp>("signal","signal",mbb,(RooFormulaVar&) *w.function("mean"),(RooFormulaVar&) *w.function("sigmaL"),
													(RooFormulaVar&) *w.function("sigmaR"),(RooFormulaVar&)*w.function("tail_shift"),
													(RooFormulaVar&) *w.function("tail_sigma"));
	}
	else throw logic_error("WRROR");
//	analysis::backgroundmodel::RooQuadGausExp func("signal","signal",mbb,(RooFormulaVar&) *w.function("mean"), (RooFormulaVar&) *w.function("sigmaL1"),
//			(RooFormulaVar&) *w.function("sigmaL2"), (RooFormulaVar&) *w.function("sigmaR1"),
//			(RooFormulaVar&) *w.function("sigmaR2"), (RooFormulaVar&) *w.function("tail_shift"),
//			(RooFormulaVar&) *w.function("tail_sigma"), (RooFormulaVar&) *w.function("norm_g1"), (RooFormulaVar&) *w.function("norm_g2"));
	w.import(*func.get());
	w.Print("v");
	string out_path = in_folder + "/workspace/signal_workspace.root";
	w.writeToFile(out_path.c_str(),1);
	cout<<"INFO: file " + out_path +" was created"<<endl;
}

string SignalModel(const vector<string>& parameters){
	map<string, vector<string> > models;
	vector<string> inserter;
	models["bukin"] = {"signal_norm","Xp","sigp","xi","rho1","rho2"};
	models["doublegausexp"] = {"signal_norm","mean","sigmaL","sigmaR","tail_shift","tail_sigma"};
	set<string> s1(parameters.begin(),parameters.end());
	for(const auto& model : models){
		inserter.clear();
		set<string> s2(model.second.begin(),model.second.end());
		set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),back_inserter(inserter)); //compare this vectors
		if(inserter.size() != model.second.size()) continue;
		else return model.first;
	}
	throw logic_error("Error in <SignalModel> : Couldn't match available parameters to any models specified");
}
