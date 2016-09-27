/*
 * PlotLimit.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/PlotLimit.h"

PlotLimit::PlotLimit(const std::string & fileList, const bool& blindData) : fileList_(fileList), blindData_(blindData) {
	// TODO Auto-generated constructor stub
	HbbStylesNew* style = new HbbStylesNew();
	style->SetStyle();
	gStyle->SetOptFit(0000);
	gStyle->SetErrorX(0.5);
}

PlotLimit::~PlotLimit() {
	// TODO Auto-generated destructor stub
}

void PlotLimit::CalculateSigmaBRLimits(){

	// input stream
	std::ifstream inputList(fileList_);

	std::string fileName;

	double MH;
	double LIMIT;

	while (inputList >> fileName) {

		TFile file(fileName.c_str());
		if(gSystem->AccessPathName(fileName.c_str())){
	    	std::cout<<"No file: "<<fileName<<std::endl;
	    	exit(-1);
	    }

	    TTree& tree = *((TTree*)file.Get("limit"));

	    tree.SetBranchAddress("limit",&LIMIT);
	    tree.SetBranchAddress("mh",&MH);

	    Limit limit;

	    tree.GetEntry(0);
	    limit.setMass(double(MH));
	    limit.setMinus2G(double(LIMIT));

	    tree.GetEntry(1);
	    limit.setMinus1G(double(LIMIT));

	    tree.GetEntry(2);
	    limit.setMedian(double(LIMIT));

	    tree.GetEntry(3);
	    limit.setPlus1G(double(LIMIT));

	    tree.GetEntry(4);
	    limit.setPlus2G(double(LIMIT));

	    tree.GetEntry(5);
	    limit.setObserved(double(LIMIT));
	    if (blindData_) limit.setObserved(limit.getExpected());
	    GxBr_limits_.push_back(limit);
	  }


	  std::cout << std::endl;
	  std::cout << "m(H)     -2s     -1s  median     +1s     +2s     obs" << std::endl;
	  for(const auto& l : GxBr_limits_){
		  char strOut[400];
		  sprintf(strOut,"%4i  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f",
				  int(l.getMass()),l.getMinus2G(),l.getMinus1G(),l.getExpected(),l.getPlus1G(),l.getPlus2G(),l.getObserved());
		  std::cout << strOut << std::endl;
	  }

	  std::cout << std::endl;
}

void PlotLimit::CalculateTanBetaLimits(const std::vector<std::string> & names, const std::vector<std::string> & benchmarks){
	//Check containers size:
	if(names.size() != benchmarks.size()){
		std::cerr<<"Error in PlotLimit::CalculateTanBetaLimits: size of names should be equal to benchmarks";
		exit(-1);
	}

	for(size_t i = 0; i < names.size(); ++i){
		CalculateTanBetaLimits(names.at(i),benchmarks.at(i));
	}
}

void PlotLimit::CalculateTanBetaLimits(const std::string& name, const std::string& benchmark){

	std::pair<std::string, std::vector<Limit> > benchmark_limit;
	benchmark_limit.first = name;

	mssm_xs_tools my(benchmark.c_str(),true,0);

	double minTanBeta =  1;
	double maxTanBeta = 60;

	std::ifstream inputList(fileList_);

	std::string fileName;

	double MH;
	double LIMIT;

	while (inputList >> fileName) {

		TFile file(fileName.c_str());
		if(gSystem->AccessPathName(fileName.c_str())){
	    	std::cout<<"No file: "<<fileName<<std::endl;
	    	exit(-1);
	    }

	    TTree& tree = *((TTree*)file.Get("limit"));

	    tree.SetBranchAddress("limit",&LIMIT);
	    tree.SetBranchAddress("mh",&MH);

	    Limit limit;

	    tree.GetEntry(0);
	    limit.setMass(double(MH));
	    limit.setMinus2G(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));

	    tree.GetEntry(1);
	    limit.setMinus1G(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));

	    tree.GetEntry(2);
	    limit.setMedian(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));

	    tree.GetEntry(3);
	    limit.setPlus1G(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));

	    tree.GetEntry(4);
	    limit.setPlus2G(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));

	    tree.GetEntry(5);
	    limit.setObserved(double(XSecToTanBeta_(my,MH,LIMIT,minTanBeta,maxTanBeta)));
	    if (blindData_) limit.setObserved(limit.getExpected());
	    benchmark_limit.second.push_back(limit);
	  }

	  std::cout << std::endl;
	  std::cout << "m(H)     -2s     -1s  median     +1s     +2s     obs" << std::endl;
	  for(const auto& l : benchmark_limit.second){
		  char strOut[400];
		  sprintf(strOut,"%4i  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f",
				  int(l.getMass()),l.getMinus2G(),l.getMinus1G(),l.getExpected(),l.getPlus1G(),l.getPlus2G(),l.getObserved());
		  std::cout << strOut << std::endl;
	  }

	  std::cout << std::endl;
	  tanBeta_limits_.push_back(benchmark_limit);
}

void PlotLimit::PlotSigmaBRLimits(const std::string& output,
	       const float& yMin,
	       const float& yMax,
		   const float& xMin,
		   const float& xMax,
	       const std::string& Lumi,
		   const std::string& xtitle,
		   const std::string& ytitle,
		   const bool& logY){

	std::string output_new(fileList_);
	if(output != "" ) output_new += "_" + output;

	TLegend leg(0.62,0.54,0.85,0.76);
	leg.SetFillColor(0);
	leg.SetTextSize(0.035);
	leg.SetBorderSize(0);
	std::vector<Limit> nullvct;
	LimitPlotter_(GxBr_limits_,nullvct,leg,output_new,yMin,yMax,xMin,xMax,Lumi,xtitle,ytitle,logY);
}


void PlotLimit::PlotSigmaBRLimits(const std::vector<Limit>&differ_limits,
		   const std::string& output,
	       const float& yMin,
	       const float& yMax,
		   const float& xMin,
		   const float& xMax,
	       const std::string& Lumi,
		   const std::string& xtitle,
		   const std::string& ytitle,
		   const bool& logY){
	std::string output_new(fileList_);
	if(output != "" ) output_new += "_" + output;

	TLegend leg(0.62,0.54,0.85,0.76);
	leg.SetFillColor(0);
	leg.SetTextSize(0.035);
	leg.SetBorderSize(0);
	LimitPlotter_(GxBr_limits_,differ_limits,leg,output_new,yMin,yMax,xMin,xMax,Lumi,xtitle,ytitle,logY);

}

void PlotLimit::PlotTanBetaLimit(const std::string& name,
		const std::string& output,
		const float& yMin,
		const float& yMax,
		const float& xMin,
		const float& xMax,
		const std::string& Lumi,
		const std::string& xtitle,
		const std::string& ytitle,
		const bool& logY){
	//Find requested benchmark
	std::vector<Limit> myLimits;
	for(const auto& l: tanBeta_limits_){
		if(l.first == name){
			myLimits = l.second;
			break;
		}
	}

	std::string output_new(fileList_);
	output_new = output_new + "_" + name  + "_tanBeta";
	if(output != "") output_new += "_" + output;

	TLegend leg(0.65,0.17,0.92,0.44);
	leg.SetHeader(name.c_str());
	leg.SetFillColor(0);
	leg.SetTextSize(0.035);
	leg.SetBorderSize(0);
	std::vector<Limit> nullvct;
	LimitPlotter_(myLimits,nullvct,leg,output_new,yMin,yMax,xMin,xMax,Lumi,xtitle,ytitle,logY);
}

void PlotLimit::PlotTanBetaLimit(const std::vector<Limit>& differ_limits,
		const std::string& name,
		const std::string& output,
		const float& yMin,
		const float& yMax,
		const float& xMin,
		const float& xMax,
		const std::string& Lumi,
		const std::string& xtitle,
		const std::string& ytitle,
		const bool& logY){

	//Find requested benchmark
	std::vector<Limit> myLimits;
	for(const auto& l: tanBeta_limits_){
		if(l.first == name){
			myLimits = l.second;
			break;
		}
	}

	std::string output_new(fileList_);
	output_new = output_new + "_" + name  + "_tanBeta";
	if(output != "") output_new += "_" + output;

	TLegend leg(0.56,0.17,0.83,0.44);
	leg.SetHeader(name.c_str());
	leg.SetFillColor(0);
	leg.SetTextSize(0.035);
	leg.SetBorderSize(0);
	LimitPlotter_(myLimits,differ_limits,leg,output_new,yMin,yMax,xMin,xMax,Lumi,xtitle,ytitle,logY);
}

double PlotLimit::XSecToTanBeta_(mssm_xs_tools my,
		     double mA,
		     double xsection,
		     double minTanBeta,
		     double maxTanBeta) {

  double minimalDifference = 1e+10;
  bool rangeExceeded = true;
  double tanBetaTarget = -1;
  double xsecTarget    = -1;
  int ibmax = 10*int(maxTanBeta-minTanBeta);
  for (int ib=0; ib<ibmax; ++ib) {

    double tanBeta = minTanBeta + 0.1*double(ib);

    double sigmaBBA = my.bbHSantander_A(mA,tanBeta);
    double sigmaBBH = my.bbHSantander_H(mA,tanBeta);

    double BrAbb = my.br_Abb(mA,tanBeta);
    double BrHbb = my.br_Hbb(mA,tanBeta);

    double totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
    double difference = TMath::Abs(totXSec-xsection);

    if (difference<minimalDifference) {
      minimalDifference = difference;
      tanBetaTarget = tanBeta;
      xsecTarget = totXSec;
    }

    if (totXSec>xsection) {
      rangeExceeded = false;
      break;
    }

  }

  if (rangeExceeded)
    tanBetaTarget = tanBetaTarget*TMath::Sqrt(xsection/xsecTarget);

  return tanBetaTarget;

}

void PlotLimit::LimitPlotter_(const std::vector<Limit>& limits,
		const std::vector<Limit>& differ_limits,
		TLegend& leg,
		const std::string& output,
		const float& yMin,
		const float& yMax,
		const float& xMin,
		const float& xMax,
		const std::string& Lumi,
		const std::string& xtitle,
		const std::string& ytitle,
		const bool& logY){


	if(limits.size() == 0) {
		std::cerr<<"Error: No limits with this name. Please check spelling";
		exit(-1);
	}

	//Write value of limits in file
	std::fstream fs;
	fs.open( (output + ".txt").c_str(),std::fstream::out );

	fs<< "m(H)     -2s     -1s  median     +1s     +2s     obs \n";
	for(const auto& l : limits){
		char strOut[400];
		sprintf(strOut,"%4i  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f  %6.2f",
				int(l.getMass()),l.getMinus2G(),l.getMinus1G(),l.getExpected(),l.getPlus1G(),l.getPlus2G(),l.getObserved());
		fs<<strOut;
		fs<<"\n";

	  }

	int nPointsX = limits.size();
	std::vector<double> mA;
	std::vector<double> obs;
	std::vector<double> median;
	std::vector<double> minus1;
	std::vector<double> minus2;
	std::vector<double> plus1;
	std::vector<double> plus2;
	std::vector<double> zero;

	float massMax = 0;
	float massMin = 1000;

	for(const auto& l : limits){
		if(l.getMass() < massMin) massMin = l.getMass();
		if(l.getMass() > massMax) massMax = l.getMass();
		mA.push_back(l.getMass());
		obs.push_back(l.getObserved());
		median.push_back(l.getMedian());
		minus2.push_back(l.getMedian() - l.getMinus2G());
		minus1.push_back(l.getMedian() - l.getMinus1G());
		plus2.push_back(l.getPlus2G() - l.getMedian());
		plus1.push_back(l.getPlus1G() - l.getMedian());
		zero.push_back(0);

	}

	TGraph * differ_exp = nullptr;
	bool compare_limits = false;
	if(differ_limits.size() != 0) compare_limits = true;
	std::vector<double> mA_2;
	std::vector<double> median_2;
	if(compare_limits){
		for(const auto& l: differ_limits){
			mA_2.push_back(l.getMass());
			median_2.push_back(l.getMedian());
		}
		differ_exp = new TGraph(differ_limits.size(),mA_2.data(),median_2.data());
		differ_exp->SetLineWidth(3);
		differ_exp->SetLineColor(1);
		differ_exp->SetLineStyle(3);
	}

	TGraph * obsG = new TGraph(nPointsX, mA.data(), obs.data());
	obsG->SetLineWidth(3);
	obsG->SetLineColor(1);
	obsG->SetLineWidth(2);
	obsG->SetMarkerColor(1);
	obsG->SetMarkerStyle(20);
	obsG->SetMarkerSize(1.4);

	TGraph * expG = new TGraph(nPointsX, mA.data(),median.data());
	expG->SetLineWidth(3);
	expG->SetLineColor(2);
	expG->SetLineStyle(2);

	TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(nPointsX, mA.data(), median.data(), zero.data(), zero.data(), minus1.data(), plus1.data());
	innerBand->SetFillColor(kGreen);
	innerBand->SetLineColor(kGreen);

	TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(nPointsX, mA.data(), median.data(), zero.data(), zero.data(), minus2.data(), plus2.data());
	outerBand->SetFillColor(kYellow);
	outerBand->SetLineColor(kYellow);

	TH2F * frame = NULL;

	if (xMax>0) massMax = xMax;

	frame = new TH2F("frame","",2,xMin,xMax,2,yMin,yMax);
	frame->GetXaxis()->SetTitle(xtitle.c_str());
	frame->GetYaxis()->SetTitle(ytitle.c_str());
	frame->GetXaxis()->SetNdivisions(505);
	frame->GetYaxis()->SetNdivisions(206);
	frame->GetYaxis()->SetTitleOffset(1.25);
	frame->GetYaxis()->SetTitleSize(0.048);


	TCanvas *canv = new TCanvas("canv", "histograms", 600, 600);

	frame->Draw();

	outerBand->Draw("3same");
	innerBand->Draw("3same");
	expG->Draw("lsame");
	if(compare_limits) differ_exp->Draw("lsame");
	if (!blindData_)
		obsG->Draw("lpsame");

	//  float xLeg = 0.18;
	//  float yLeg = 0.83;
	//  float xLegend = 0.57;
	//  float yLegend = 0.41;
	//  float sizeLeg = 0.27;

	  //  TLine * line = new TLine(massMin,1,massMax,1);
	  //  line->SetLineWidth(2);
	  //  line->SetLineStyle(2);
	  //  line->Draw();
	std::string entry_comp;
	if(compare_limits) {
		if(Lumi.find("24.6") != std::string::npos) entry_comp = "Expected 7 + 8 TeV";
		else if (Lumi.find("19.7") != std::string::npos) entry_comp = "Expected 8 TeV";
		leg.AddEntry(differ_exp,entry_comp.c_str(),"l");
	}
	if (!blindData_)
		leg.AddEntry(obsG,"Observed","lp");
	leg.AddEntry(expG,"Expected","l");
	leg.AddEntry(innerBand,"#pm1#sigma Expected","f");
	leg.AddEntry(outerBand,"#pm2#sigma Expected","f");

	TPad * pad = (TPad*)canv->GetPad(0);
	Luminosity lum;
	lum.writeExtraText = true;
	lum.lumi_13TeV = Lumi;
	lum.extraText = "Private Work";
	int offs = 33;
	if(ytitle.find("tan") != std::string::npos) offs = 11;
	lum.CMS_lumi(pad,4,offs);

	pad->RedrawAxis();

	leg.Draw();

	if(logY) canv->SetLogy();
	canv->Update();
	canv->Print( (output+".pdf").c_str() ,"Portrait pdf");
	canv->Print( (output+".png").c_str()) ;
	fs.close();
}
