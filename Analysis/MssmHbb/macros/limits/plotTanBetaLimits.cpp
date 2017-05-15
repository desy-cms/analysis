/*
 * plotTanBetaLimits.cpp
 *
 *  Created on: 9 Mar 2017
 *      Author: shevchen
 */

#include <iostream>
#include <string>
#include <vector>

//ROOT includes
#include <TFile.h>

#include "Analysis/MssmHbb/interface/HbbLimits.h"
#include "Analysis/MssmHbb/interface/Limit.h"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

HbbStyle style;

using namespace std;
using namespace analysis::mssmhbb;

int main(){

	HbbLimits limits(true,true);
	style.set(PRIVATE);

	//pathes with results
	string path2016_combined = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201703/08/asymptotic/combination/mssm/Hbb.limits";
	string path2016_solo = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201703/08/asymptotic/mssm/Hbb.limits";
	vector<Limit> null_vec;
	string output = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/ParametricLimits/20170309/";

	string boson = "both";
	limits.SetHiggsBoson(boson);

	TLegend legenda(0.65,0.17,0.92,0.44);
	legenda.SetFillColor(0);
	legenda.SetTextSize(0.035);
	legenda.SetBorderSize(0);

	// combined results
	vector<Limit> GxBR_combined = limits.ReadCombineLimits(path2016_combined);
	string benchmark = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root";
	string benchmark_ref = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/signal/newmhmax_mu200_13TeV.root";
	vector<Limit> mssm_limits_combined = limits.GetMSSMLimits(GxBR_combined,benchmark,"",false,benchmark_ref,30);
	limits.Write(mssm_limits_combined, "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/ParametricLimits/20170309/combined_limits.txt");

	//solo 2016 13 TeV
	vector<Limit> GxBR_13TeV = limits.ReadCombineLimits(path2016_solo);
	vector<Limit> mssm_limits_13TeV = limits.GetMSSMLimits(GxBR_13TeV,benchmark);
	limits.Write(mssm_limits_13TeV, "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/ParametricLimits/20170309/solo_13TeV_limits.txt");

	//solo 7+8 TeV
	vector<Limit> tanBeta2012 = {
			Limit(300,24.8,34.1,31.1,20.5,39.2,17.6),
			Limit(350,30.0,37.1,37.5,24.8,47.1,21.4),
			Limit(400,36.5,34.4,46.9,29.8,00.0,25.5),
			Limit(500,53.5,50.0,00.0,43.2,00.0,36.8)
	};

	string output_mssm_tanB_limits = output + boson + "_Combvs13TeV_MSSM_tanB_brazil";
	limits.LimitPlotter(mssm_limits_13TeV,tanBeta2012,legenda,output_mssm_tanB_limits,0,60,200,900,"24.6 vs 35.7(2016)","M_{#Phi} [GeV]","tan(#beta)",false);
}
