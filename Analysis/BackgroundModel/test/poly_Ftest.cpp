/*
 * poly_Ftest.cpp
 *
 *  Created on: 12 Apr 2017
 *      Author: shevchen
 */

/*
 * Macro to run F-test on a polynomial function
 */

#include <memory>
#include "RooPolynomial.h"

TH1 * GenerateToyPolyData(RooRealVar &x ,const int& degree);
void  GetPolynom(RooWorkspace &w , const int& degree, const std::string& name);
void FTest(RooDataHist& data, RooRealVar &x, const int& max_degree = 10);

using namespace std;

int poly_Ftest(){

	int gen_degree = 5;
	RooRealVar x("x","x",0,100);
	TH1D * gen_toy = static_cast<TH1D*>(GenerateToyPolyData(x,gen_degree));
	RooDataHist data_toy_sb("data_toy","data_toy", x, RooFit::Import(*gen_toy));
	gen_toy->Draw();
	FTest(data_toy_sb, x, 8);

	cout<<"Done: main"<<endl;
	return 0;
}

TH1 * GenerateToyPolyData(RooRealVar &x ,const int& degree){

	RooWorkspace w("w");
	w.import(x);
	GetPolynom(w,degree,"toy");
	RooAbsPdf &poly = *w.pdf("poly_toy");
	TH1D *toy = static_cast<TH1D*>(poly.createHistogram(("toy_degree" + to_string(degree)).c_str(),x));
	toy->Scale(100);

	cout<<"Done: GenerateToyPolyData"<<endl;
	return std::move(toy);
}

void GetPolynom(RooWorkspace &w ,const int& degree, const std::string& name){
	RooArgList arg_list;
	for(int i =0;i <= degree; ++i) {
		string var_name = "par" + to_string(i) + "_" + name;
		RooRealVar v(var_name.c_str(),var_name.c_str(),1,-10,10); v.setConstant(false);
		w.import(v);
		arg_list.add(*w.var(var_name.c_str()));
	}

	string poly_name = "poly_" + name;
	RooPolynomial poly(poly_name.c_str(),poly_name.c_str(),*w.var("x"),arg_list,0);
	w.import(poly);

	cout<<"Done: GetPolynom"<<endl;
}

void FTest(RooDataHist& data, RooRealVar &x, const int& max_degree){
	int null_degree = 2;

	RooPlot *frame = x.frame();
	data.plotOn(frame,RooFit::Name("data"),RooFit::MarkerSize(0.1));

	RooWorkspace w_null("w_null");
	w_null.import(x);
	GetPolynom(w_null,null_degree,"null");
	RooAbsPdf &poly_null = *w_null.pdf("poly_null");
	poly_null.fitTo(data);
	poly_null.plotOn(frame,RooFit::Name("pdf_null"));
	frame->Draw();
	double chi2_null = frame->chiSquare("pdf_null", "data", null_degree + 1);
	cout<<"chi2_null = "<<chi2_null<<endl;
	for(int i = null_degree + 1; i < max_degree; ++i){
		RooWorkspace w("w_pdf");
		w.import(x);
		GetPolynom(w,i,"pdf");
		RooAbsPdf &poly = *w.pdf("poly_pdf");
		poly.fitTo(data,RooFit::PrintLevel(0),RooFit::Verbose(0));
		poly.plotOn(frame,RooFit::Name("pdf_ftest"),RooFit::LineColor(2));
		double chi2 = frame->chiSquare("pdf_ftest", "data", i + 1);
		cout<<"chi2: Iter "<<i<<" = "<<chi2<<endl;
		double F_stat = (chi2_null - chi2) / (i - null_degree) / (chi2 / (100- i));
		double p_value = ROOT::Math::fdistribution_cdf(F_stat,i - null_degree,(100 - i));
		cout<<"dChi = "<<chi2_null - chi2<<endl;
		cout<<"Degrees: "<<null_degree - i<<" "<<i<<endl;
		cout<<"F-stat = "<<F_stat<<endl;
		cout<<"p value: "<<p_value<<" (1-p) = "<<1-p_value<<endl;
		chi2_null = chi2;
		null_degree = i;

	}
}
