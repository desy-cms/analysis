#include "RooAbsArg.h"
#include "RooFitResult.h"
#include "RooRealVar.h"

#include "TFile.h"
#include "TSystem.h"
#include "THStack.h"

#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TH1.h"
#include "Math/Math.h"
#include "Math/QuantFuncMathCore.h"

#include "stdlib.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options/variables_map.hpp>

#include "Analysis/MssmHbb/interface/utilLib.h"

namespace fs = boost::filesystem;

using namespace std;
using namespace boost::program_options;

double poissonError(const double & obs,const double & exp);
void computeChi2(const TH1 & observed,const TH1 & expected,const RooFitResult & fitResult);
TH1D * convertToMbb(const TH1 & hInput, const TH1 & hProperBins);
//double xsScale(const int & mass);
double illustrativeSF(const int & mass);

void postFitPlot(const std::string & input_file_fit_results,
		   const std::string & input_file_histo,
		   const int & mass,
	       const std::string & output = "",
	       const std::string & Lumi = "2.62 fb^{-1}",
	       const bool & logY = true,
	       const bool & blindData = true);

int main(int argc, char * argv[]){
	const string cmsswBase = getenv("CMSSW_BASE");

	//Declare a group of general options
	options_description generalOptions("General options");
	generalOptions.add_options()
			("help,h", "Produce help message.")
			("verbose,v", value<int>()->default_value(0), "More verbose output.")
			;

    // Declare a group of options that will be allowed only on command line
	options_description cmdLineOptions("CMD Optional arguments");
	cmdLineOptions.add_options()
		("output_file,o", value<std::string>()->default_value(""),"Output file name. Will be conbined with default 'Hbb.limits'")
		("Lumi", value<std::string>()->default_value("2.62 fb^{-1}"),"String with Lumi, i.e.: 2.62 fb^{-1}")
		("logY", value<bool>()->default_value(true),"Log Y axis? True/False")
		("blindData", value<bool>()->default_value(true),"Blinded Data? True/False")
	    ;
    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    options_description hidden("Hidden options");
    hidden.add_options()
        ("test", value<int>()->default_value(0),"if test specify 1. Default 0.")
        ;

    options_description requiredOptions("Required options");
    requiredOptions.add_options()
	    ("input_file_results,r", value<std::string>()->required(),"Input file with outputs of the Combination tool with MLL fit")
	    ("input_file_histo,i",   value<std::string>()->required(),"Input file with histograms as an output of the Combination tool")
		("mass,m", value<int>()->required(),"Mass point")
		;

    options_description all_options;
    all_options.add(generalOptions).add(cmdLineOptions).add(requiredOptions).add(hidden);

    options_description allowed_options("Allowed options");
    allowed_options.add(generalOptions).add(cmdLineOptions);

    options_description visible;
    visible.add(generalOptions).add(cmdLineOptions).add(requiredOptions);

    variables_map output_vm;
    store(command_line_parser(argc, argv).options(allowed_options).allow_unregistered().run(), output_vm);
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

  std::string  inputResultFile_ 	=  output_vm["input_file_results"].as<std::string>();
  std::string  inputHistoFile_		=  output_vm["input_file_histo"].as<std::string>();
  int mass_							=  output_vm["mass"].as<int>();
  std::string  outFileName_     	=  output_vm["output_file"].as<std::string>();
  string Lumi_ 						=  output_vm["Lumi"].as<std::string>();
  auto logY_ 						=  output_vm["logY"].as<bool>();
  auto blindData_ 					=  output_vm["blindData"].as<bool>();

  postFitPlot(inputResultFile_,inputHistoFile_,mass_,outFileName_,Lumi_,logY_,blindData_);

  return 0;
}

void postFitPlot(const std::string & input_file_name,
		   const std::string & input_file_histo,
		   const int & mass,
	       const std::string & output,
	       const std::string & Lumi,
	       const bool & logY,
	       const bool & blindData){

	std::cout<<"In postFitPlot "<<std::endl;

	// Check input files
	TFile *fInput = new TFile(input_file_name.c_str(),"READ");
	if(!fInput){ std::cerr<<"No file: "<<input_file_name<<" was found!"<<std::endl; exit(-1); }

	TFile *fHInput = new TFile(input_file_histo.c_str(),"READ");
	if(!fHInput){ std::cerr<<"No file: "<<input_file_histo<<" was found!"<<std::endl; exit(-1);	}

	//Output file
	TFile fOut( ("postfit_plot_M_" + std::to_string(mass)+"_"+ output + ".root").c_str(),"RECREATE");

//	RooRealVar *r = (RooRealVar*) ((RooFitResult*) fInput.Get("fit_s"))->floatParsFinal().find("r");

	// Input strings
	string stages[3] = {"prefit", "fit_s", "fit_b"};
	string process = "bbHTo4b";

	TH1D * data_obs = (TH1D*) fHInput->Get("data_obs");
	if(!data_obs){
		std::cerr<<"No data_obs histo was found"<<std::endl;
		exit(-1);
	}
	data_obs->SetMarkerStyle(20);
	data_obs->SetMarkerSize(0.95);
	data_obs->SetLineWidth(2);

	for(const string & stage : stages){

		//Chi2 calculation
		if(stage != "prefit"){
			cout<<"Fit mode: "<<stage<<std::endl;
			computeChi2(*data_obs,*(TH1D*) fInput->Get( ("shapes_"+stage + "/" + process + "/total").c_str() ),* (RooFitResult*) fInput->Get(stage.c_str()));
		}
		data_obs = convertToMbb(*data_obs,*data_obs);

		string resPath = "shapes_"+stage + "/" + process + "/";

		//Translate output of the combination tool to the human readable histograms :D
		TH1D *tmp = (TH1D*) fInput->Get( (resPath+"total").c_str());
		if(!tmp){
			std::cerr<<"No histogram: "<<(resPath+"total")<<" at file: "<<fInput->GetName()<<std::endl;
			exit(-1);
		}
		TH1D * total = convertToMbb(*tmp, *data_obs); //total: S+Bg

		tmp = (TH1D*) fInput->Get( (resPath+"total_background").c_str());
		if(!tmp) {
			std::cerr<<"No histogram: "<<(resPath+"total_background")<<" at file: "<<fInput->GetName()<<std::endl;
			exit(-1);
		}
		TH1D * totalBkg = convertToMbb(*tmp, *data_obs);
		totalBkg->SetFillColor(kTeal+3);
		totalBkg->SetLineColor(kBlack);

		tmp = (TH1D*) fInput->Get( (resPath+"total_signal").c_str());
		if(!tmp) {
			std::cerr<<"No histogram: "<<(resPath+"total_signal")<<" at file: "<<fInput->GetName()<<std::endl;
			exit(-1);
		}
		TH1D * totalSig = convertToMbb(*tmp, *data_obs);
		//totalSig->Scale(xsScale(mass));					// Scale to the XSection (because numbers were provided for 1 pb)
		if(stage == "fit_s"){
			totalSig->SetFillColor(kRed);
			totalSig->SetLineColor(kBlack);
		}

		//Add Tempr. THstack for Bg+Signal
		THStack * totalStack = new THStack();
		totalStack->Add(totalBkg);						// Add background
		if(stage == "fit_s") totalStack->Add(totalSig); // Add Signal

		THStack *stack = new THStack();					//Stack that will be drawn
		stack = totalStack;
		if(stage != "fit_s") total = totalBkg;

		TCanvas *c = new TCanvas(("can_"+stage).c_str(),"",600,600);
        TPad *pad1 = new TPad("pad1", "", 0, 0.25, 1, 1);
        pad1->SetTopMargin(0.08);
        pad1->SetBottomMargin(0.019);
        pad1->SetLeftMargin(0.15);
        pad1->SetRightMargin(0.05);
        pad1->SetTicks(1, 1);
        pad1->Draw();

        c->cd();
        TPad *pad2 = new TPad("pad2", "", 0, 0.0, 1, 0.25);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.35);
        pad2->SetLeftMargin(0.15);
        pad2->SetRightMargin(0.05);
        pad2->SetGrid(0,1);
        pad2->SetTicks(1, 1);
        pad2->Draw();
        c->cd();

        pad1->cd();
        double maxEntries = std::max(data_obs->GetMaximum(),total->GetMaximum());
        stack->SetMaximum(maxEntries + 0.05*maxEntries);
        stack->SetMinimum(0);
        stack->Draw("hist");
        stack->GetHistogram()->SetLabelSize(0);
        stack->GetYaxis()->SetTitle("a.u.");//"Events/(bin width)");
        stack->GetYaxis()->SetLabelSize(0.05);
        stack->GetYaxis()->SetTitleOffset(1.4);
        stack->GetYaxis()->SetTitleSize(0.05);
        total->SetFillStyle(3444);
        total->SetFillColor(kGray+2);
        total->SetLineColor(kBlack);
        total->Draw("same e2");
        data_obs->Draw("same p");
	if(stage == "fit_s"){        
		TH1D *scaleSig = (TH1D*) totalSig->Clone("scaleSig");
        	scaleSig->Scale(illustrativeSF(mass));
        	scaleSig->Draw("same hist");
	//        totalSig->Draw("same hist");
	}

        pad1->RedrawAxis();
        pad2->cd();
        TH1D *ratio = (TH1D*) data_obs->Clone();
        ratio->Add(total,-1);
        ratio->Divide(total);

//        for(int i=1;i<ratio->GetNbinsX();++i){
//        	std::cout<<"Bin: "<<i<<" Data: "<<data_obs->GetBinContent(i)<<" +/- "<<data_obs->GetBinError(i)<<
//        			   " Data-Pred: "<<data_obs->GetBinContent(i) - total->GetBinContent(i)<<" +/- "<<std::sqrt(data_obs->GetBinError(i)*data_obs->GetBinError(i) + total->GetBinError(i)*total->GetBinError(i))<<
//					   " (Data-Pred)/Pred: "<<(data_obs->GetBinContent(i) - total->GetBinContent(i))/total->GetBinContent(i)<<" +/- "<<std::sqrt( pow(1./total->GetBinContent(i) * data_obs->GetBinError(i),2) + pow(data_obs->GetBinContent(i)* total->GetBinError(i)/(total->GetBinContent(i)*total->GetBinContent(i)),2.) )<<
//					   " From TH1: "<<ratio->GetBinContent(i)<<" +/- "<<ratio->GetBinError(i)<<std::endl;
//
//        }

        ratio->SetStats(kFALSE);
        ratio->SetTitle("");
        ratio->SetMinimum(-0.1);
        ratio->SetMaximum(0.1);
        ratio->GetYaxis()->SetNdivisions(6,0,0);
        ratio->SetLineStyle(0);
        ratio->GetXaxis()->SetLabelSize(0.15);
        ratio->GetXaxis()->SetTitle("#bf{m_{bb} (GeV)}");
        ratio->GetXaxis()->SetTitleSize(0.17);
        ratio->GetXaxis()->SetTitleOffset(0.9);
        ratio->GetYaxis()->SetLabelSize(0.14);
        ratio->GetYaxis()->SetTitle("#bf{#frac{(Data-Pred)}{Pred}}");
        ratio->GetYaxis()->CenterTitle(kTRUE);
        ratio->GetYaxis()->SetTitleSize(0.12);
        ratio->GetYaxis()->SetTitleOffset(0.6);
        ratio->Draw();

        TH1D *ratioBkg = (TH1D*) total->Clone();
        ratioBkg->Add(total,-1);
        ratioBkg->Divide(total);
        ratioBkg->SetFillStyle(3244);
        ratioBkg->SetFillColor(kGray+2);
        ratioBkg->SetMarkerSize(0);
        for(int i = 1; i < ratioBkg->GetNbinsX() + 1; ++i){
        	if(ratioBkg->GetBinError(i) == 0) ratioBkg->SetBinError(i,999.);
        	if(ratioBkg->GetBinContent(i) == -1){
        		ratioBkg->SetBinContent(i,-999.);
        		ratioBkg->SetBinError(i,0.);
        	}
//        	std::cout<<"Bin: "<<i<<" total: "<<total->GetBinContent(i)<<" +/- "<<total->GetBinError(i)<<
//        			   " total-total: "<<total->GetBinContent(i) - total->GetBinContent(i)<<" +/- "<<
//					   " (total-total)/total: "<<(total->GetBinContent(i) - total->GetBinContent(i))/total->GetBinContent(i)<<" +/- "<<
//					   " From TH1: "<<ratioBkg->GetBinContent(i)<<" +/- "<<ratioBkg->GetBinError(i)<<std::endl;
        }
        ratioBkg->Draw("same e2");
        pad2->RedrawAxis("g");
        ratio->Draw("same p e0");
        pad2->RedrawAxis();
        pad1->cd();

		// Create a legend
		TLegend leg(0.6,0.4,0.9,0.9);
		leg.SetLineColor(0);
		leg.AddEntry(data_obs,"Observed","pl"); // Add observed data
		leg.AddEntry(totalBkg,"QCD multijet prediction","f"); //Add QCD background
		if(stage == "fit_s") leg.AddEntry(totalSig,("H_{1}/A_{1} #times " +to_string_with_precision(illustrativeSF(mass),1) + " (m = "+to_string(mass) + " GeV)").c_str(),"f"); // Add Signal Model
		leg.AddEntry(total, "syst. + stat. uncertainty", "f"); // Add stat. + syst. uncertainties
        leg.Draw();

        c->cd();
        c->Print( ("postfit_M_"+std::to_string(mass)+"_"+stage+".pdf").c_str() );
        c->Print( ("postfit_M_"+std::to_string(mass)+"_"+stage+".png").c_str() );

        fOut.cd();
        c->Write( ("canva_"+stage).c_str() );
        ratio->Write( ("ratio_"+stage).c_str() );
        ratioBkg->Write( ("ratioBkg_"+stage).c_str() );
        data_obs->Write( ("data_obs_"+stage).c_str() );
        total->Write( ("total_"+stage).c_str() );
	}
	fOut.Close();
}

double poissonError(const double & obs,const double & exp){
	gSystem->Load("libMathCore");
	double sigma = 1.0;
	double alpha = 1.0 - TMath::Erf(sigma/TMath::Sqrt(2));
	if(obs < exp){
		return ROOT::Math::gamma_quantile_c(alpha/2,obs+1,1);
	}
	else {
		if(obs == 0) return 0;
		else return ROOT::Math::gamma_quantile(alpha/2,obs,1.);
	}
}

void computeChi2(const TH1 & observed,const TH1 & expected,const RooFitResult & fitResult){
    double chi2 = 0.0;
    int ndof = 0;

    for(int iBin = 1; iBin < observed.GetNbinsX() + 1; ++ iBin){
    	double obs 		= observed.GetBinContent(iBin);
    	double exp 		= expected.GetBinContent(iBin);
    	double statErr  = poissonError(obs,exp);
    	chi2 += (obs-exp)*(obs-exp)/(statErr*statErr);
    	ndof += 1;
//	std::cout<<"Chi: "<<obs<<" "<<exp<<" "<<chi2<<std::endl;
    }
    RooArgList floatPars = (RooArgList) fitResult.floatParsFinal();
    int rIndex = floatPars.index("r");
    int indexRange = floatPars.getSize();
    if(rIndex != -1) ndof -= 1;

    for(int i = 0; i < indexRange; ++i ){
    	if(i == rIndex) continue;
    	double val = ((RooRealVar*) floatPars.at(i))->getVal();
    	chi2 += val*val;
    }

    cout<<"chi2/ndf = "<<chi2<<" / "<<ndof<<" = "<<chi2/ndof<<endl;
    cout<<"prob = "<<TMath::Prob(chi2, ndof)<<std::endl;
}

TH1D * convertToMbb(const TH1 & hInput, const TH1 & hProperBins){
	int nBins = hProperBins.GetNbinsX();
	TH1D *converted = (TH1D*) hProperBins.Clone();
	for(int i = 1; i < nBins +1; ++i){
    	converted->SetBinContent(i, hInput.GetBinContent(i)); // / converted->GetBinWidth(i));
    	converted->SetBinError(i, hInput.GetBinError(i)); // / converted->GetBinWidth(i));
//    	std::cout<<hInput.GetName()<<" Ev: "<<converted->GetBinContent(i)<<" "<<converted->GetBinError(i)<<std::endl;
	}
	converted->SetName(hInput.GetName());
	return converted;
}

/*
double xsScale(const int & mass){
	switch(mass){
	case 100:
		return 5.23;
	case 120:
		return 41.79;
	case 160:
		return 75.32;
	case 200:
		return 35.42;
	case 250:
		return 15.55;
	case 300:
		return 7.64;
	case 350:
		return 4.1;
	case 400:
		return 2.34;
	case 500:
		return 0.88;
	case 600:
		return 0.377;
	case 700:
		return 0.18;
	case 900:
		return 0.051;
	case 1100:
		return 0.018;
	case 1300:
		return 0.008;
	default:
		std::cerr<<"No such mass point in the list of xsections"<<std::endl;
		exit(-1);
}
}
*/

double illustrativeSF(const int & mass){
	switch(mass){
	case 100:
		return 1.;
	case 120:
		return 1.;
	case 160:
		return 1.;
	case 200:
		return 1.;
	case 250:
		return 3000.;
	case 300:
		return 3000.;
	case 350:
		return 1000.;
	case 400:
		return 1000;
	case 500:
		return 500;
	case 600:
		return 500;
	case 700:
		return 800;
	case 900:
		return 800;
	case 1100:
		return 800;
	case 1300:
		return 800;
	default:
		return -1000;
		std::cerr<<"No such mass point"<<std::endl;
		exit(-1);
	}
}
