/*
 * FTest.cpp
 *
 *  Created on: 18 Apr 2017
 *      Author: shevchen
 */
#include <iostream>

#include "TCanvas.h"
#include "RooFitResult.h"

#include "Analysis/BackgroundModel/interface/FTest.h"
#include "Analysis/BackgroundModel/interface/ProbabilityDensityFunctions.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

using namespace analysis::backgroundmodel;

FTest::FTest(TH1 & data_obs, RooRealVar & observable, const std::string& pdf_family) :
		pdf_family_(pdf_family), data_name_("data") {
	/*
	 * Constructor from TH1 and RooRealVar observable
	 */
	RooDataSet data(data_name_.c_str(),data_name_.c_str(),observable,RooFit::Import(data_obs));
	obs_name_ = observable.GetName();
	workspace_.import(data);
	workspace_.import(observable);
}

FTest::FTest(RooDataSet & data_obs, RooRealVar & observable, const std::string& pdf_family) :
		pdf_family_(pdf_family) {
	/*
	 * Constructor from RooDataSet and RooRealVar observable
	 */
	obs_name_ = observable.GetName();
	data_name_=   data_obs.GetName();
	workspace_.import(data_obs);
	workspace_.import(observable);
}

FTest::FTest(TH1 & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family) :
	pdf_family_(pdf_family), obs_name_("observable"), data_name_("data") {
	/*
	 * Constructor from TH1 and xmin and xmax of observable
	 */
	RooRealVar observable(obs_name_.c_str(),obs_name_.c_str(),xmin,xmax);
	RooDataSet data(data_name_.c_str(),data_name_.c_str(),observable,RooFit::Import(data_obs));
	workspace_.import(data);
	workspace_.import(observable);
}

FTest::FTest(RooDataSet & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family) :
	pdf_family_(pdf_family), obs_name_("observable") {
	/*
	 * Constructor from RooDataSet and xmin and xmax of observable
	 */
	RooRealVar observable(obs_name_.c_str(),obs_name_.c_str(),xmin,xmax);
	workspace_.import(data_obs);
	workspace_.import(observable);
}

void FTest::PerformFTest(const int& max_degree,const std::string& output_folder, const int& min_degree){
	/*
	 * F-test
	 */
	RooFit::SumW2Error(kTRUE);

	//Check output path
//	CheckOutputDir(output_folder);

	CheckPdfFamily();
	TCanvas can("can","can",800,600);

	auto &data = *GetFromRooWorkspace<RooDataSet>(workspace_, data_name_);
	auto &x    = *GetFromRooWorkspace<RooRealVar>(workspace_, obs_name_);

//	std::string null_pdf_name = "pdf_null";
//	int null_degree = 0;
//	if(pdf_family_.find("bern")!=std::string::npos) null_degree = 1;
//	GetPdf(null_degree,null_pdf_name);
//	auto &null_pdf = *GetFromRooWorkspace<RooAbsPdf>(workspace_,null_pdf_name);
//
//	auto &fitResult_null = *null_pdf.fitTo(data,RooFit::Save(),RooFit::PrintLevel(0),RooFit::Verbose(0),RooFit::SplitRange(),RooFit::Offset(1));
//	PrintRooFitResults(fitResult_null);
//	auto chi2ndf_null = PlotFitResult(null_pdf_name, output_folder + "/NullPdf");
//
//	auto chi2_null = chi2ndf_null.chi2;
//	std::cout<<"chi2_null/ndf = "<<chi2_null/chi2ndf_null.ndf<<std::endl;

	double chi2_null = 0;
	int begin_degree = 0;
	int null_degree = min_degree;
	double minnll_null = 0;
	if(pdf_family_.find("bern")!=std::string::npos && min_degree == 0) null_degree = 1;
	//FTest loop - each iteration number of parameters is increasing by 1
	begin_degree = null_degree;
	for(int i = null_degree; i < max_degree; ++i){
		std::string pdf_name = "pdf" + std::to_string(i);
		GetPdf(i,pdf_name);
		auto &pdf = *workspace_.pdf((pdf_name).c_str());
		AdjustPDFVars(i - begin_degree);
		auto &fitResult = *pdf.fitTo(data,RooFit::Save(),RooFit::PrintLevel(0),RooFit::Verbose(0),RooFit::Minimizer("Minuit2", "migrad"), RooFit::Hesse(1),RooFit::Offset(1));
		PrintRooFitResults(fitResult);
		auto chi2ndf = PlotFitResult(pdf_name, output_folder + "/" + pdf_name);
		auto chi2 = chi2ndf.chi2;
		auto ndf = chi2ndf.ndf;
		auto minnll = fitResult.minNll();
		std::cout<<"chi2/ndf: Iter "<<i<<" = "<<chi2<<"/"<<ndf<<" = "<<chi2/ndf<<std::endl;
		double F_stat = (chi2_null - chi2) / (i - null_degree) / (chi2 / (ndf));
		double p_value = ROOT::Math::fdistribution_cdf(F_stat,i - null_degree,(ndf));
		std::cout<<"dMinNLL = "<<minnll_null - minnll<<" F: "<<(minnll_null - minnll) / (i - null_degree) / (minnll_null / (ndf))<<" p_val = "<<ROOT::Math::fdistribution_cdf((minnll_null - minnll) / (i - null_degree) / (minnll_null / (ndf)),i - null_degree,(ndf))<<std::endl;
		std::cout<<"dChi = "<<chi2_null - chi2<<std::endl;
		std::cout<<"Degrees: "<<null_degree - i<<" "<<i<<std::endl;
		std::cout<<"F-stat = "<<F_stat<<std::endl;
		std::cout<<"F-stat_test = "<<TMath::FDistI(F_stat,i - null_degree,(x.getBins() - i))<<" "<<TMath::FDist(F_stat,i - null_degree,(x.getBins() - i))<<std::endl;

		std::cout<<"p value: "<<p_value<<" (1-p) = "<<1-p_value<<std::endl;
		chi2_null = chi2;
		null_degree = i;
		minnll_null = minnll;
	}
}

void FTest::GetPdf(const int& degree, const std::string& name){
	/*
	 * Method to get PDF according to
	 * the choosen pdf_family
	 */
	ProbabilityDensityFunctions pdfs(workspace_,obs_name_.c_str(),false);
	pdfs.setPeakStart(280);
	pdfs.setPdf(pdf_family_ + "," + std::to_string(degree) , name);
}

void FTest::CheckPdfFamily(){
	/*
	 * Method to check whether Pdf family is correct
	 */
	if(std::find(availablePDFs_.begin(), availablePDFs_.end(), pdf_family_) == availablePDFs_.end()){
		std::stringstream msg;
		msg<<"PDF family "<<pdf_family_<<" not implemented. Choose available one: "<<boost::algorithm::join(availablePDFs_,",");
		throw std::invalid_argument(msg.str());
	}
}

void FTest::PrintRooFitResults(RooFitResult& r){
	/*
	 * Method to print Parameters before and after the fit
	 */
	std::cout << "\nconstant parameters:" << std::endl;
	r.constPars().Print("v");
	std::cout << "\nfloating parameters (init):" << std::endl;
	r.floatParsInit().Print("v");
	std::cout << "\nfloating parameters (final):" << std::endl;
	r.floatParsFinal().Print("v");
}

Chi2Ndf FTest::PlotFitResult(const std::string& pdf_name, const std::string& output_name){
	/*
	 * Method to draw fit results
	 */
	// Top frame
	auto &x    = *GetFromRooWorkspace<RooRealVar>(workspace_, obs_name_);
	auto &data = *GetFromRooWorkspace<RooDataSet>(workspace_, data_name_);
	auto &pdf  = *GetFromRooWorkspace<RooAbsPdf> (workspace_, pdf_name);
	auto &frame_up = *x.frame();
	frame_up.SetTitle("");

	//Draw data points and PDF
	data.plotOn(&frame_up,RooFit::MarkerSize(0.8),RooFit::Name("data"));
	pdf.plotOn(&frame_up,RooFit::LineColor(kRed),RooFit::Name(pdf_name.c_str()));

	//Calculate chi2
	int pdf_nfloatpars = (pdf.getParameters(data)->selectByAttrib("Constant", kFALSE))->getSize();
	RooFitQuality qual;
	Chi2Ndf chi2ndf = qual.chiSquare(frame_up, pdf_name, "data", pdf_nfloatpars);
	std::string chi2str = to_string_with_precision<double>(chi2ndf.chi2, 2) + "/" + to_string_with_precision<double>(chi2ndf.ndf,2) + " = " + to_string_with_precision<double>(chi2ndf.chi2/chi2ndf.ndf,2);

	//Print PDF parameters
	double par_xmin = 0.65, par_xmax = 0.66, par_ymax = 0.8;
	pdf.paramOn(&frame_up,RooFit::Layout(par_xmin,par_xmax,par_ymax));
	frame_up.getAttText()->SetTextSize(0.02);
	frame_up.getAttLine()->SetLineWidth(0);

	// Bottom frame
	auto &hpull = *frame_up.pullHist();
	hpull.SetMarkerSize(0.8);
	auto &frame_down = *x.frame();
	frame_down.addPlotable(&hpull,"P");
	frame_down.SetMinimum(-5.);
	frame_down.SetMaximum(+5.);

	TCanvas canvas("can","can",500,500);
	canvas.cd();

	TPad pad1("pad1","",0,0.1,1,1);
	pad1.SetBottomMargin(0.2);
	pad1.SetRightMargin(0.05);
	pad1.SetLeftMargin(0.16);
	pad1.Draw();
	pad1.cd();
	frame_up.GetXaxis()->SetTitleOffset(999); //Effectively turn off x axis title on main plot
	frame_up.GetXaxis()->SetLabelOffset(999); //Effectively turn off x axis label on main plot
	frame_up.GetYaxis()->SetTitleSize(0.038);
	frame_up.GetYaxis()->SetTitleOffset(1.6);
	frame_up.GetYaxis()->SetLabelSize(0.033);
	frame_up.Draw();

	TLatex latex;
	latex.SetTextFont(43);
	latex.SetTextSize(17);
	latex.SetTextAlign(11);
	latex.DrawLatexNDC(0.31-pad1.GetRightMargin(), 1.02-canvas.GetTopMargin(),("CMS Preliminary #sqrt{s} = 13 TeV, L = "));
	latex.SetTextSize(15);
	latex.SetTextAlign(33);
	latex.SetTextColor(kBlue+2);
	latex.DrawLatexNDC(0.98-pad1.GetRightMargin(), 0.98-pad1.GetTopMargin(),(std::string("#chi^{2}_{RooFit}/ndf = ")+chi2str).c_str());
	latex.SetTextColor(kGreen+2);
	latex.DrawLatexNDC(0.98-pad1.GetRightMargin(), 0.93-pad1.GetTopMargin(),(std::string("p-value = ")+to_string_with_precision<double>(chi2ndf.GetPValue(), 2)).c_str());
//	latex.SetTextColor(kOrange+2);
//	latex.DrawLatexNDC(0.98-pad1.GetRightMargin(), 0.88-pad1.GetTopMargin(),(std::to_string(int(x.getMin()))+std::string(" < M_{12} < ")+std::to_string(int(x.getMax()))).c_str());

	canvas.cd();
	TPad pad2("pad2","",0,0.0,1,0.265);
	pad2.SetTopMargin(1);
	pad2.SetBottomMargin(0.33);
	pad2.SetLeftMargin(pad1.GetLeftMargin());
	pad2.SetRightMargin(pad1.GetRightMargin());
	pad2.SetGridy();
	pad2.Draw();
	pad2.cd();
	frame_down.SetTitle("");
	frame_down.GetXaxis()->SetTitleSize(0.15);
	frame_down.GetXaxis()->SetTitleOffset(0.9);
	frame_down.GetXaxis()->SetLabelSize(0.115);
	frame_down.GetXaxis()->SetLabelOffset(0.010);
	frame_down.SetYTitle("Pulls");
	frame_down.GetYaxis()->CenterTitle(kTRUE);
	frame_down.GetYaxis()->SetTitleSize(0.14);
	frame_down.GetYaxis()->SetTitleOffset(0.4);
	frame_down.GetYaxis()->SetNdivisions(3,5,0);
	frame_down.GetYaxis()->SetLabelSize(0.115);
	frame_down.GetYaxis()->SetLabelOffset(0.011);
	frame_down.Draw();

	canvas.Print((output_name+"_linear.pdf").c_str());
	pad1.SetLogy();
	double log_min = frame_up.GetMinimum()*0.1;
	if(log_min == 0) log_min = frame_up.GetMinimum() + 1e-05*frame_up.GetMaximum();
	frame_up.GetYaxis()->SetRangeUser(log_min, frame_up.GetMaximum()*5);
	canvas.Modified();
	canvas.Update();
	canvas.Print((output_name+"_log.pdf").c_str());

	return chi2ndf;
}

const std::vector<std::string> FTest::availablePDFs_ = {
		"superdijet",
		"supernovosibirsk",
		"supernovoeffprod",
		"superdijeteffprod",
		"superdijetlinearprod",
		"berneffprod"
};

std::string FTest::DefineSubrange(){
	/*
	 * Function to deffine which subrange was used
	 */
	auto &x = *workspace_.var(obs_name_.c_str());
	std::string sub_range = "";
	if(int(x.getMin()) == 500 && int(x.getMax()) == 1700) sub_range = "sr3";
	else if(int(x.getMin()) == 350 && int(x.getMax()) == 1190) sub_range = "sr2";
	else if(int(x.getMin()) == 200 && int(x.getMax()) == 650) sub_range = "sr1";

	return sub_range;
}

void FTest::AdjustPDFVars(const int& relative_index){
	/*
	 * Method to hard code initial parameters
	 */
	if( DefineSubrange() == "sr2"){
		if(pdf_family_ == "superdijetlinearprod"){
			if(relative_index == 2){
				std::cout<<"WTF"<<std::endl;
//				workspace_.var("parc")->setVal(55.75);
//				std::cout<<"WTF"<<std::endl;
//				workspace_.var("para_dijet")->setVal(12.42);
//				std::cout<<"WTF"<<std::endl;
//				workspace_.var("parb_dijet")->setVal(0.125);
//				std::cout<<"WTF"<<std::endl;
//				workspace_.var("par0_dijet")->setVal(0.011);
				std::cout<<"WTF"<<std::endl;
				workspace_.var("par1_dijet")->setVal(0.); workspace_.var("par1_dijet")->setMin(-0.001); workspace_.var("par1_dijet")->setMax(0.001);
				std::cout<<"WTF"<<std::endl;
			}
		}
		else if(pdf_family_ == "supernovosibirsk"){
			if(relative_index == 1){
				workspace_.var("peak")->setVal(299);
				workspace_.var("width")->setVal(49.5);
				workspace_.var("tail")->setVal(-0.98);
				workspace_.var("par0")->setVal(0.0002);
			}
			if(relative_index == 2){
				workspace_.var("par1")->setVal(0);
			}
		}
	}
	else if(DefineSubrange() == "sr1"){
		if(pdf_family_.find("superdijeteffprod") != std::string::npos){
			if(relative_index == 0 || relative_index == 1 || relative_index == 2 || relative_index == 3){
				workspace_.var("para_dijet")->setMin(1);
				workspace_.var("mean_dijet")->setVal(167.0);
				workspace_.var("para_dijet")->setVal(11.0);
				workspace_.var("parb_dijet")->setVal(0.1);
				if(relative_index == 1) workspace_.var("par0_dijet")->setVal(0.);
				if(relative_index == 2){
					workspace_.var("par0_dijet")->setVal(0.15);
					workspace_.var("par1_dijet")->setVal(0.028);

				}
				if(relative_index == 3) {
					workspace_.var("mean_dijet")->setVal(127.7);
					workspace_.var("para_dijet")->setVal(2.21);
					workspace_.var("parb_dijet")->setVal(0.07);
					workspace_.var("par0_dijet")->setVal(0.156);
					workspace_.var("par1_dijet")->setVal(0.027);
					workspace_.var("slope_novoeff")->setVal(0.015);
					workspace_.var("turnon_novoeff")->setVal(229);
					workspace_.var("par2_dijet")->setVal(0); workspace_.var("par2_dijet")->setMin(-0.001); workspace_.var("par2_dijet")->setMax(0.001);

				}
			}
		}
		else if(pdf_family_ == "supernovoeffprod"){
			if(relative_index == 0){
				workspace_.var("peak")->setVal(268);
				workspace_.var("width")->setVal(63);
				workspace_.var("tail")->setVal(-0.45);
				workspace_.var("slope_novoeff")->setVal(0.016);
				workspace_.var("turnon_novoeff")->setVal(223);
			}
			if (relative_index == 1) workspace_.var("par0")->setVal(-0.0006);
			if(relative_index == 2){
				workspace_.var("peak")->setVal(268);
				workspace_.var("width")->setVal(63);
				workspace_.var("tail")->setVal(-0.45);
				workspace_.var("par0")->setVal(-0.0006);
				workspace_.var("slope_novoeff")->setVal(0.016);
				workspace_.var("turnon_novoeff")->setVal(223);
				workspace_.var("par1")->setVal(0.0); workspace_.var("par1")->setMin(-0.001); workspace_.var("par1")->setMax(0.001);
			}
			if(relative_index == 3){
				workspace_.var("par1")->setVal(5.15274e-06);
				workspace_.var("par2")->setVal(-4.37699e-09); workspace_.var("par2")->setMin(-0.001); workspace_.var("par2")->setMax(0.001);
				workspace_.var("peak")->setVal(257.217); workspace_.var("peak")->setError(0.646384);
				workspace_.var("width")->setVal(42.5049); workspace_.var("width")->setError(0.312648);
				workspace_.var("tail")->setVal(-0.108706); workspace_.var("tail")->setError(0.00430771);
				workspace_.var("par0")->setVal(-0.00276266); workspace_.var("par0")->setError(1.4847e-05);
				workspace_.var("slope_novoeff")->setVal(0.00399448);
				workspace_.var("turnon_novoeff")->setVal(556.661);
			}
		}
	}
	if(pdf_family_.find("supernovosibirsk") != std::string::npos && DefineSubrange() == "sr3" &&
			relative_index == 0){
		workspace_.var("peak")->setVal(425);
		workspace_.var("width")->setVal(60);
		workspace_.var("tail")->setVal(-0.9);
	}

}
