/*
 * CheckCombLimits7p8.cpp
 *
 *  Created on: 8 Mar 2017
 *      Author: shevchen
 */


#include "TROOT.h"
#include "TSystem.h"
#include <fstream>
#include <iostream>
#include "TMath.h"
#include "TTree.h"
#include <string>
#include "mssm_xs_tools.C"

#include "Analysis/MssmHbb/interface/Limit.h"

using namespace std;

double MSSMTanBeta(const std::string& benchmark_path, const std::string& benchmark_ref_path, double mA, double xsection, const double& tanBref);
std::vector<Limit> ReadCombineLimits(const std::string& file_name);
const Limit ReadCombineLimit(const std::string& tfile_name, const bool& blindData);

int main(){

	string path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201703/07/asymptotic/7p8TeV/Hbb.limits";
	string benchmark_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/signal/8TeV/mhmodp-8TeV-tanbHigh-nnlo.root";
	string benchmark_ref_path = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/signal/8TeV/mhmax-mu200-8TeV-tanbHigh-nnlo.root";
	const double tanBref = 30;

	cout<<"Norm"<<endl;
	vector<Limit> GxBR_limits = ReadCombineLimits(path);
	std::vector<Limit> TanB_limits;
	for(unsigned int i = 0; i != GxBR_limits.size(); ++i){
		Limit gxbr_limit = GxBR_limits.at(i);
		Limit tan_b_limit;
//		Set Mass point
		tan_b_limit.setX(gxbr_limit.getX());
//		Set limits
		tan_b_limit.setMinus2G(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getMinus2G(),tanBref)));
		tan_b_limit.setMinus1G(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getMinus1G(),tanBref)));
		tan_b_limit.setMedian(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getMedian(),tanBref)));
		tan_b_limit.setPlus1G(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getPlus1G(),tanBref)));
		tan_b_limit.setPlus2G(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getPlus2G(),tanBref)));
		tan_b_limit.setObserved(double(MSSMTanBeta(benchmark_path,benchmark_ref_path,tan_b_limit.getX(),gxbr_limit.getObserved(),tanBref)));

		TanB_limits.push_back(tan_b_limit);
	}

	for(const auto& l : TanB_limits){
		cout<<"M = "<<l.getX()<<" -2G = "<<l.getMinus2G()<<" -1G = "<<l.getMinus1G()<<" Med = "<<l.getExpected()<<" +1G = "<<l.getPlus1G()<<" +2G = "<<l.getPlus2G()<<endl;
	}

	return 0;
}

double MSSMTanBeta(const std::string& benchmark_path, const std::string& benchmark_ref_path, double mA, double xsection, const double& tanBref){
	/*
	 * Method to translate GxBR value to tanBeta for MSSM
	 */
	mssm_xs_tools ref_my;
	ref_my.SetInput((char*) benchmark_ref_path.c_str());
	mssm_xs_tools my;
	my.SetInput((char*)benchmark_path.c_str());
	double minimalDifference = 1e+10;
	bool rangeExceeded = true;
	double tanBetaTarget = -1;
	double xsecTarget    = -1;
	double maxTanBeta = 60;
	double minTanBeta = 1;

	double sigmaBBA = -100,sigmaBBH = -100, sigmaBBh = -100;
	double tanBeta, BrAbb, BrHbb, Brhbb, totXSec, difference;
	int ibmax = 100*int(maxTanBeta-minTanBeta);
	for (int ib=0; ib<ibmax; ++ib) {

		tanBeta = minTanBeta + 0.1*double(ib);

		sigmaBBA = my.Give_Xsec_bbA5f(mA,tanBeta);
		sigmaBBH = my.Give_Xsec_bbH5f(mA,tanBeta);
		sigmaBBh = my.Give_Xsec_bbh5f(mA,tanBeta);

	    BrAbb = my.Give_BR_A_bb(mA,tanBeta);
	    BrHbb = my.Give_BR_H_bb(mA,tanBeta);
	    Brhbb = my.Give_BR_h_bb(mA,tanBeta);

	    totXSec = sigmaBBA*BrAbb;
		  if(        mA < 125 ) {
			  totXSec += sigmaBBh*Brhbb;
		  } else if( mA < 135 ) {
			  totXSec += sigmaBBh*Brhbb;
			  totXSec += sigmaBBH*BrHbb;
		  } else {
			  totXSec += sigmaBBH*BrHbb;
		  }

//	    totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;

		  //reference mhmax+
		const double XSbbA = ref_my.Give_Xsec_bbA5f(mA,tanBref);
		const double XSbbH = ref_my.Give_Xsec_bbH5f(mA,tanBref);
		const double XSbbh = ref_my.Give_Xsec_bbh5f(mA,tanBref);
		const double BRAbb = ref_my.Give_BR_A_bb(mA,tanBref);
		const double BRHbb = ref_my.Give_BR_H_bb(mA,tanBref);
		const double BRhbb = ref_my.Give_BR_h_bb(mA,tanBref);

		  double tot = XSbbA*BRAbb;
		  if(        mA < 125 ) {
		    tot += XSbbh*BRhbb;
		  } else if( mA < 135 ) {
		    tot += XSbbh*BRhbb;
		    tot += XSbbH*BRHbb;
		  } else {
		    tot += XSbbH*BRHbb;
		  }
	    // for the combination of 7 + 8 + 13 TeV crosssection should be divided by the xsection at reference tanB
//	    totXSec /= my.GiveXsec_Santander_A(mA,tanBref);
//	    totXSec /= (my.GiveXsec_Santander_A(mA,tanBref)*my.Give_BR_A_bb(mA,tanBref) + my.GiveXsec_Santander_H(mA,tanBref)*my.Give_BR_H_bb(mA,tanBref));
//	    totXSec /= (my.Give_Xsec_bbA5f(mA,tanBref)*my.Give_BR_A_bb(mA,tanBref) + my.Give_Xsec_bbH5f(mA,tanBref)*my.Give_BR_H_bb(mA,tanBref));
		  totXSec /= tot;

	    difference = TMath::Abs(totXSec-xsection);

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

	if (rangeExceeded){
		tanBetaTarget = tanBetaTarget*TMath::Sqrt(xsection/xsecTarget);
		cout<<"WTFFF"<<endl;
	}

	return tanBetaTarget;
}

std::vector<Limit> ReadCombineLimits(const std::string& file_name){
	/*
	 * Method to read Limits calculated with Asymptotic method in the combine tool
	 * and written to the file.
	 */
//	input stream:
	cout<<"Norm"<<endl;
	std::ifstream inputList(file_name);
//	TFile name
	std::string tfileName;
	std::vector<Limit> GxBr_limits{};
	cout<<"Norm"<<endl;
//	Loop over the lines in the input file:
	while (inputList >> tfileName) {
		Limit limit = ReadCombineLimit(tfileName,1);
		GxBr_limits.push_back(limit);
	}
	return GxBr_limits;
}

const Limit ReadCombineLimit(const std::string& tfile_name, const bool& blindData){
	/*
	 * Method to get information about limit from root file
	 */
	TFile file(tfile_name.c_str(),"read");
//	Get TTree from this file
	TTree& tree = *((TTree*)file.Get("limit"));
	double LIMIT;
	tree.SetBranchAddress("limit",&LIMIT);
	double MH;
	tree.SetBranchAddress("mh",&MH);
//	Get iinformation from TTree:
    Limit limit;

    tree.GetEntry(0);
    limit.setX(double(MH));
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
//    if(blindData) limit.setObserved(limit.getExpected());

    return limit;
}
