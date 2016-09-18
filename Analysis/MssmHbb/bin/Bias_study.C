#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <memory>
#include <fstream>
#include <ostream>
#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TMath.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TCanvas.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooRandom.h"


//#include <type_traits>
#include <string>
#include <stdlib.h>
#include <locale>
#include <algorithm>
#include <functional>

//include boost
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>

namespace fs = boost::filesystem;
using namespace boost::program_options;

using namespace std;
using namespace RooFit;

const double n_obs = 10261.;
const double n_ref[4] = {88.,88.,82.,51.};	//number of events from 0.5*sig_upper

void Bias_study(const int & mass_point, const int & signal_strength, const int & toy_max, const std::string & pdf, const std::string & generator, const std::string & output, const bool & test);

std::string select_pdf(int * npar,const std::string & pdf, const bool & test);

const bool findStrings(const std::string & input, const std::string & needful);

int main(int argc, const char * argv[])
{
	// firts arg is mass point (int ), second - signal strength, third - number of toys
	// list of the pdfs to fit toy S+B MC
	// list of the pdfs to generate toy S+B MC
	
	options_description generalOptions("General options");
    	generalOptions.add_options()
				("help,h", "Produce help message.")
				("verbose,v", value<int>()->default_value(0), "More verbose output.");

        // Declare a group of options that will be allowed only on command line
		options_description cmdLineOptions("CMD Optional arguments");
		cmdLineOptions.add_options()
			("mass_point,m", value<int>()->required(),"Select mass point from the list of available: \n"
//			" \t100;\n"
//			" \t120;\n"
//			" \t160;\n"
//			" \t200;\n"
//			" \t250;\n"
			" \t300;\n"
			" \t350;\n"
			" \t400;\n"
			" \t500;\n")
//			" \t600;\n"
//			" \t700;\n"
//			" \t900;\n"
//			" \t1100;\n"
//			" \t1300;\n")
			("signal_strength,r",value<int>()->required(),"Signal strength to be injected to toys fit")
			("toy_max,n",value<int>()->required(),"Number of toys")
			("pdf,f",value<std::string>()->required(),"Select nominal PDF to fit toys data: \n"
			" \tnovops;\n"
			" \tnovoeff;\n"
			" \tcbeff;\n"
			" \tbukin;\n")
			("generator,g",value<std::string>()->required(),"Select alternative PDF to produce toys data: \n"
                        " \tnovops;\n"
                        " \tnovoeff;\n"
                        " \tcbeff;\n"
                        " \tbukin;\n")
		    	("output_file,o", value<std::string>()->default_value(""), "Output file name, if not specified will be created automatically");

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        options_description hidden("Hidden options");
        hidden.add_options()
            ("test", value<bool>()->default_value(0),"if test specify 1. Default 0.")
            ;

        options_description all_options;
        all_options.add(generalOptions).add(cmdLineOptions).add(hidden);

        options_description visible("Allowed options");
        visible.add(generalOptions).add(cmdLineOptions);


   	variables_map output_vm;
    	store(command_line_parser(argc, argv).options(generalOptions).allow_unregistered().run(), output_vm);
    	notify(output_vm);

    	if (output_vm.count("help")) {
	    std::cout << visible << std::endl;
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

	int mass_ 		= output_vm["mass_point"].as<int>();
	int signal_strength_ 	= output_vm["signal_strength"].as<int>();
	int toy_max_		= output_vm["toy_max"].as<int>();
	std::string pdf_ 	= output_vm["pdf"].as<std::string>();
	std::string generator_  = output_vm["generator"].as<std::string>();
	std::string output_	= output_vm["output_file"].as<std::string>();
	bool test_		= output_vm["test"].as<bool>();
	
	Bias_study(mass_,signal_strength_,toy_max_,pdf_,generator_,output_,test_);

	return  0;
}

std::string select_pdf(int * npar,const std::string & pdf, const bool & test){
	std::string outname;
	if(findStrings(pdf,"novops")){
		*npar = 3+2;
		outname = "workspace_novops.root";
	}
	else if(findStrings(pdf,"novoeff")) {
		*npar = 5+2;
		outname = "workspace_novoeff.root";
	}
	else if(findStrings(pdf,"cbeff")) {
		*npar = 6+2;
		outname = "workspace_cbeff.root";
	} 
	else if(findStrings(pdf,"bukin")) {
		*npar = 5+2;
		outname = "workspace_bukin.root";
	}
	else{
		std::cout<<"No such function available!"<<std::endl;
		exit(-1);
	}

	if(test){
		std::cout<<" PDF: "<<pdf<<" with npar = "<<npar<<" connect to file: "<<outname<<std::endl;
	}
	return outname;
}

void Bias_study(const int & mass, const int & signal_strength, const int & toy_max, const std::string & pdf, const std::string & generator, const std::string & output, const bool & test)
{

	// Initial values set here
	double reso  = mass*0.1;
	double n_gen = 0.;
	if(mass == 300) n_gen = n_ref[0];
	if(mass == 350) n_gen = n_ref[1];
	if(mass == 400) n_gen = n_ref[2];
	if(mass == 500) n_gen = n_ref[3];
	double n_sgn = signal_strength*n_gen;
	
	if(test){
		std::cout<<" Starting Bias test with M = "<<mass<<" and r = "<<signal_strength<<std::endl;
		std::cout<<" Selected PDF is: "<<pdf<<std::endl;
		std::cout<<" Selected Generator is: "<<generator<<std::endl;
	}
	int nparPDF, nparGen;
	std::string nPDF, nGenerator;
	nPDF = select_pdf(&nparPDF,pdf,test);
	nGenerator = select_pdf(&nparGen,generator,test);


	TFile * fPDF = new TFile(nPDF.c_str(),"READ");
	if(!fPDF){
		std::cerr<<"Error: no file "<<nPDF<<" was found. Interrupt"<<std::endl;
		exit(-1);
	}
	RooWorkspace *wPDF = (RooWorkspace*)fPDF->Get("workspace");
	if(!wPDF){
		std::cerr<<"Error: no (RooWorkspace*) with name 'workspace' was found at file "<<nPDF<<std::endl;
		exit(-1); 
	}

	TFile * fGenerator = new TFile(nGenerator.c_str(),"READ");
	if(!fGenerator){
		std::cerr<<"Error: no file "<<nGenerator<<" was found. Interrupt"<<std::endl;
		exit(-1);
	}
	RooWorkspace *wGenerator = (RooWorkspace*)fGenerator->Get("workspace");
	if(!wGenerator){
		std::cerr<<"Error: no (RooWorkspace*) with name 'workspace' was found at file "<<nGenerator<<std::endl;
		exit(-1); 
	}


	double nb_toy=0.,ns_toy=0.,n_toy=0.,ns_fit=0.,ns_err=0.,nb_fit=0.,nb_err=0.,chi_2=0.,bias=0.;
	int r = signal_strength;

	RooRandom::randomGenerator()->SetSeed(0);
	
	//RooRealVar *mbb = wPDF->var("mbb");
	RooRealVar *mbb = wGenerator->var("mbb");
	if(!mbb){
		std::cerr<<"No mbb variable in the workspace from file "<<fPDF<<std::endl;
		exit(-1);
	}

	// Output file TTree
	std::string out_name;
	if(output != "") out_name = output;
	else out_name = "BiasTest_toy"+generator+"_fit"+pdf+"_"+std::to_string(mass)+"GeV_r"+std::to_string(signal_strength)+".root";
	TFile* outputFile = new TFile(out_name.c_str(),"RECREATE");
	TTree outtree("Toys","Toys");

	outtree.Branch("n_gen",		&n_gen,		"n_gen/D");
	outtree.Branch("n_sgn", 	&n_sgn, 	"n_sgn/D");
	outtree.Branch("signal_strength",&r,		"signal_strength/I");
	outtree.Branch("nb_toy",	&nb_toy,	"nb_toy/D");
	outtree.Branch("ns_toy",	&ns_toy,	"ns_toy/D");
	outtree.Branch("n_toy",		&n_toy,		"n_toy/D"); 
	outtree.Branch("ns_fit",	&ns_fit,	"ns_fit/D");
	outtree.Branch("ns_err",	&ns_err,	"ns_err/D");
	outtree.Branch("nb_fit",	&nb_fit,	"nb_fit/D");
	outtree.Branch("nb_err",	&nb_err,	"nb_err/D");   	
	outtree.Branch("chi_2",		&chi_2,		"chi_2/D");
	outtree.Branch("bias",		&bias,		"bias/D");
		
	// produce toys from alternative PDF and fit by nominal PDF
	RooAbsPdf *pdf_bkg_fit = wPDF->pdf("background");
	if(!pdf_bkg_fit){
		std::cerr<<"No 'background' in workspace from the file "<<nPDF<<std::endl;
		exit(-1);
	}
	RooAbsPdf *pdf_bkg_alt = wGenerator->pdf("background");
	if(!pdf_bkg_alt){
		std::cerr<<"No 'background' in workspace from the file "<<generator<<std::endl;
		exit(-1);
	}
  	
	// Signal PDF
 	RooRealVar mean("gausmean","Mean of Gaussian", mass);
  	RooRealVar sigma("gaussigma","Width of Gaussian", reso); 
 	RooGaussian pdf_sgn("gaus","gaus(mbb,mean,sigma)", *mbb, mean, sigma);
	// Extended signal PDF for toy
	RooRealVar sgn_norm("sgn_norm","", n_sgn);
	RooExtendPdf pdf_sgn_ext("pdf_sgn_ext","", pdf_sgn, sgn_norm); 

	// Extended background PDF for toy
	RooRealVar bkg_norm("bkg_norm","", n_obs);
	RooExtendPdf pdf_bkg_alt_ext("pdf_bkg_alt_ext","", *pdf_bkg_alt, bkg_norm);

	// N_s and N_b in S+B Fit
	RooRealVar n_s("n_s","", 0.);
	n_s.setConstant(0);	//turn off to be constant (freely floating)
	RooRealVar n_b("n_b","", bkg_norm.getVal());
	n_b.setConstant(0);	//turn off to be constant (freely floating)
	
	// Preparing S+B PDF to fit toy
	RooAddPdf pdf_fit_ext("pdf_fit_ext","", RooArgList(pdf_sgn, *pdf_bkg_fit), RooArgList(n_s,n_b));
	RooDataSet *data_toy;
	RooDataSet *data_toy_sgn;
	//RooFitResult *res_fit;
	RooPlot* xframe = mbb->frame(Title("")) ;

	for(int i=0; i < toy_max; ++i) 
	{
	  	// Preparing S+B toys unbinned dataset
	  	data_toy = pdf_bkg_alt_ext.generate(*mbb, Extended());
	  	nb_toy = data_toy->sumEntries();
	  	data_toy_sgn = pdf_sgn_ext.generate(*mbb, Extended());
	  	ns_toy = data_toy_sgn->sumEntries();
 
	  	RooCategory index("index","index");
	  	index.defineType("signal") ;
	  	index.defineType("background") ;
	  	RooDataSet data_toy_sb("data_toy_sb","", RooArgSet(*mbb), Index(index), Import("background",*data_toy), Import("signal",*data_toy_sgn));
		
	  	n_toy = data_toy_sb.sumEntries();

	  	n_s.setVal(0.);
	  	n_b.setVal(bkg_norm.getVal());

	  	//res_fit = pdf_fit_ext.fitTo(data_toy_sb, RooFit::Save(), RooFit::PrintLevel(0), RooFit::Extended(kTRUE)); 
		pdf_fit_ext.fitTo(data_toy_sb, RooFit::Save(), RooFit::PrintLevel(0), RooFit::Extended(kTRUE)); 

	  	data_toy_sb.plotOn(xframe, RooFit::Name("data_curve"));	
	  	pdf_fit_ext.plotOn(xframe, RooFit::Name("fit_curve"));
	  	//xframe->Draw(); 

	  	chi_2 = xframe->chiSquare("fit_curve", "data_curve", nparPDF);	//parameters from sig+bkg (novops)

	  	std::cout << "Chi^2 from the fit = " << chi_2 << std::endl;

		//if(chi_2 > 1.1) continue;

	  	std::cout << "nb_toy = " << nb_toy << std::endl; 
	  	std::cout << "ns_toy = " << ns_toy << std::endl;
	  	std::cout << "n_toy  = " << n_toy << std::endl;
	  	std::cout << "n_s    = " << n_s.getVal() << std::endl;
	  	std::cout << "err_s  = " << n_s.getError() << std::endl;
	  	std::cout << "n_b    = " << n_b.getVal() << std::endl;
	  	std::cout << "err_b  = " << n_b.getError() << std::endl;	
	  	std::cout << "Bias   = " << (n_s.getVal() - ns_toy)/(n_s.getError()) << std::endl;

		ns_fit = n_s.getVal();
		ns_err = n_s.getError();
		nb_fit = n_b.getVal();
		nb_err = n_b.getError();
		bias   = (n_s.getVal() - ns_toy)/(n_s.getError());

		outtree.Fill();
	}

	outputFile->cd();
	outtree.Write();
	outputFile->Close();
        cout << "End of code :)" << endl;
}

const bool findStrings(const std::string & input, const std::string & needful){
	std::string input1 = input;
	std::string input2 = needful;
	std::transform(input1.begin(),input1.end(),input1.begin(),::tolower);
	std::transform(input2.begin(),input2.end(),input2.begin(),::tolower);
	if(input1.find(input2) != std::string::npos) return true;
	else return false;
}


