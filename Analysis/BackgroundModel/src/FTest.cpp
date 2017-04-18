///*
// * FTest.cpp
// *
// *  Created on: 12 Apr 2017
// *      Author: shevchen
// */

#include "Analysis/BackgroundModel/interface/FTest.h"

using namespace analysis::backgroundmodel;

FTest::FTest(TH1 & data_obs, RooRealVar & observable, const std::string& pdf_family, const int& max_degree) :
		pdf_family_(pdf_family), max_degree_(max_degree) {
	/*
	 * Constructor from TH1 and RooRealVar observable
	 */
	RooDataSet data("data","data",observable,RooFit::Import(data_obs));
	workspace_.import(data);
	workspace_.import(observable);
}

FTest::FTest(RooDataSet & data_obs, RooRealVar & observable, const std::string& pdf_family, const int& max_degree) :
		pdf_family_(pdf_family), max_degree_(max_degree) {
	/*
	 * Constructor from RooDataSet and RooRealVar observable
	 */
	workspace_.import(data_obs);
	workspace_.import(observable);
}

FTest::FTest(TH1 & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family, const int& max_degree) :
	pdf_family_(pdf_family), max_degree_(max_degree) {
	/*
	 * Constructor from TH1 and xmin and xmax of observable
	 */
	RooRealVar observable("observable","observable",xmin,xmax);
	RooDataSet data("data","data",observable,RooFit::Import(data_obs));
	workspace_.import(data);
	workspace_.import(observable);
}

FTest::FTest(RooDataSet & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family, const int& max_degree) :
	pdf_family_(pdf_family), max_degree_(max_degree) {
	/*
	 * Constructor from RooDataSet and xmin and xmax of observable
	 */
	RooRealVar observable("observable","observable",xmin,xmax);
	workspace_.import(data_obs);
	workspace_.import(observable);
}

//FTest::Initialise(){
//	/*
//	 * Method to Initialise the pdfs to be used in a F-test
//	 */
//
//}

void FTest::PerformFTest(const int& max_degree){
	/*
	 * F-test
	 */
	CheckPdfFamily();

	RooDataSet &data = *static_cast<RooDataSet*>(workspace_.data("data"));
	RooRealVar &x = *workspace_.var("observable");
	RooPlot &frame = *x.frame();

	int null_degree = GetPdf(0,"null");
	RooAbsPdf &null_pdf = *workspace_.pdf("pdf_null");
	null_pdf.fitTo(data);
	data.plotOn(&frame,RooFit::Name("data"),RooFit::MarkerSize(0.1));
	null_pdf.plotOn(&frame,RooFit::Name("pdf_null"));
	frame.Draw();
	double chi2_null = frame.chiSquare("pdf_null", "data", null_degree + 1);
	std::cout<<"chi2_null = "<<chi2_null<<std::endl;
	//FTest loop - each iteration number of parameters is increasing by 1
	for(int i = null_degree + 1; i < max_degree; ++i){
		std::string pdf_name = std::to_string(i);
//		int current_degree = GetPdf(i,pdf_name);
		GetPdf(i,pdf_name);
		RooAbsPdf &pdf = *workspace_.pdf(("pdf_" + pdf_name).c_str());
		pdf.fitTo(data,RooFit::PrintLevel(0),RooFit::Verbose(0));
		pdf.plotOn(&frame,RooFit::Name(pdf_name.c_str()),RooFit::LineColor(i));
		double chi2 = frame.chiSquare(pdf_name.c_str(), "data", i + 1);
		std::cout<<"chi2: Iter "<<i<<" = "<<chi2<<std::endl;
		double F_stat = (chi2_null - chi2) / (i - null_degree) / (chi2 / (x.getBins() - i));
		double p_value = ROOT::Math::fdistribution_cdf(F_stat,i - null_degree,(x.getBins() - i));
		std::cout<<"dChi = "<<chi2_null - chi2<<std::endl;
		std::cout<<"Degrees: "<<null_degree - i<<" "<<i<<std::endl;
		std::cout<<"F-stat = "<<F_stat<<std::endl;
		std::cout<<"p value: "<<p_value<<" (1-p) = "<<1-p_value<<std::endl;
		chi2_null = chi2;
		null_degree = i;
	}
}

int FTest::GetPdf(const int& degree, const std::string& name){
	/*
	 * Method to get PDF according to
	 * the choosen pdf_family
	 */
	RooArgList arg_list;
	RooRealVar &x = *workspace_.var("observable");
	int npars = 0;
	for(int i =0;i < degree; ++i) {
		std::string var_name = "par" + std::to_string(i) + "_" + name;
		RooRealVar v(var_name.c_str(),var_name.c_str(),1,-100,100); v.setConstant(false);
		workspace_.import(v);
		arg_list.add(*workspace_.var(var_name.c_str()));
		++npars;
	}
	std::string pdf_name = pdf_family_ + "_" + name;
	if(pdf_family_ == "novosibirsk") {
		RooRealVar peak = RooRealVar("peak", "peak", 300, 50.0, 500.0, "GeV");
		RooRealVar width = RooRealVar("width", "width", 50.0, 5.0, 20, "GeV");
		RooRealVar tail = RooRealVar("tail", "tail", -0.1, -1.0, 1.0);
		RooSuperNovosibirsk pdf(pdf_name.c_str(),pdf_name.c_str(),x,peak,width,tail,arg_list);
		npars += 3;
		workspace_.import(pdf);
	}
	else if (pdf_family_ == "dijet"){

	}

	return npars;
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

const std::vector<std::string> FTest::availablePDFs_ = {
		"novosibirsk",
		"dijet",
		"bernstein"
};
