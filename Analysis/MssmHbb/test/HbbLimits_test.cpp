#include <iostream>
#include <string>
#include <vector>

#include <TCanvas.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TPad.h>
#include <TFile.h>

#include "Analysis/MssmHbb/interface/HbbLimits.h"
#include "Analysis/MssmHbb/interface/Limit.h"

using namespace std;
using namespace analysis::mssmhbb;

TGraph getLine( TH2& );

int main(){

	HbbLimits limits(true,true);
//	string path2015 = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/datacards/201612/20/2015_shape/Novosibirsk/Hbb.limits";
//	string path2015 = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201701/26/finale/Hbb.limits";
	string path2015 = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201702/13/4xBins/Hbb.limits";
	string path2016 = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201702/13/1sxBins/Hbb.limits";
//	string path2016 = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/datacards/201702/03/5000bins/finale_SFb/Hbb.limits";
	string thdm_production = "production_cosB_A_-1_1_tanB_0p5-100_COMBINATION"; //production_corseBins_cosB_A_-1_1_tanB_1-100 //
	string thdm_type = "type3";
	string thdm_scans = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/" + thdm_production + "/rootFiles/Histograms3D_" + thdm_type + "_mA_mH.root";
	string output = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/pictures/ParametricLimits/";

	vector<Limit> GBR2015 = limits.ReadCombineLimits(path2015);
	vector<Limit> GBR2016 = limits.ReadCombineLimits(path2016);
	vector<Limit> TanB_2HDM_Limits;
	TGraph obs_line;
	HbbLimits::THDMScan scan, scan_old;
	TH3D GxBR_2hdm_3D;
	TH2D GxBR_2hdm_mA;
	TH2D GxBR_2hdm_cB_A;
	vector<Limit> null_vec;
	double mass =300;
	double cB_A = 0.1;
	string boson = "both";
	limits.SetHiggsBoson(boson);
	vector<Limit> THDM_limits;
	for(const auto& l : GBR2016){
		cout<<"M: "<<l.getX()<<" exp = "<<l.getMedian()<<" 1G = "<<l.getPlus1G()<<endl;
		if(l.getX() == mass){
			GxBR_2hdm_3D = limits.Get2HDM_GxBR_3D(thdm_scans);
			GxBR_2hdm_mA = limits.Get2HDM_GxBR_2D(GxBR_2hdm_3D,mass,"x");
			scan = limits.Get2HDMmuScan(GxBR_2hdm_mA,l);
			GxBR_2hdm_mA.GetXaxis()->SetRangeUser(-0.99,0.99);
			if(thdm_type!="type1" && thdm_type!="type4") THDM_limits = limits.Get2HDM_Limits(GxBR_2hdm_mA,l);
		}
	}
	GxBR_2hdm_cB_A = limits.Get2HDM_GxBR_2D(GxBR_2hdm_3D,cB_A,"z");
	TanB_2HDM_Limits = limits.Get2HDM_1D_Limits(GxBR_2hdm_cB_A,GBR2016);
	limits.Write(GBR2016,path2016);

	// 2015 vs 2016 Limits
	TLegend leg_GxBR(0.62,0.55,0.85,0.75);
	leg_GxBR.SetFillColor(0);
	leg_GxBR.SetTextSize(0.035);
	leg_GxBR.SetBorderSize(0);
	string output_independet_limits = output + "Hbb_Limits_2016_vs_2015";
	limits.LimitPlotter(GBR2016,GBR2015,leg_GxBR,output_independet_limits,0.1,30,200,1300,"36.62(2016) vs 2.62(2015)","M_{#Phi} [GeV]","95%C.L. limit on #sigma x BR [pb]",true);

	/*
	 *
	 */
/*
	obs_line = getLine(scan.expected);
	TCanvas can("can","",800,600);
	gPad->SetLogz();
	gPad->SetLogy();
	double mu = 1;
//	scan.expected.GetZaxis()->SetRangeUser(1e-02,1e03);
	scan.expected.GetXaxis()->SetRangeUser(-0.99,0.99);
	scan.expected.SetStats(kFALSE);
	scan.expected.DrawCopy("COLZ");
	scan.expected.SetContour(1,&mu);
	scan.expected.SetLineColor(2);
	scan.expected.SetLineStyle(2);
	scan.expected.DrawCopy("cont3 same");
//	obs_line.Draw("P same");
	can.Print( (output + boson + "_2HDM_" + thdm_type + "_Limits_mA" + to_string((int)mass) + "_2D.pdf").c_str());

	/*
	// 2015 only
	output_independet_limits = output + "Hbb_Limits_2016";
	limits.LimitPlotter(GBR2016,null_vec,leg_GxBR,output_independet_limits,0.1,30,200,1300,"36.62","M_{#Phi} [GeV]","95%C.L. limit on #sigma x BR [pb]",true);

	// Brzil plot:

//	TLegend leg(0.56,0.53,0.83,0.87);
	TLegend leg(0.65,0.15,0.87,0.4); // In case of
	leg.SetFillColor(0);
	leg.SetTextSize(0.035);
	leg.SetBorderSize(0);

	string output_brazil = output + boson + "_2HDM_" + thdm_type + "_Limits_mA" + to_string((int)mass) + "_brazil";
	limits.LimitPlotter(THDM_limits,null_vec,leg,output_brazil,0.5,100,-0.99,0.99,"36.62","cos(#beta-#alpha)","tan#beta",true);

	// 2HDM tanBeta vs mA limits for cos(beta-alpha) = cB_A
	TLegend leg_2HDM_tB(0.62,0.25,0.85,0.45);
	leg_2HDM_tB.SetFillColor(0);
	leg_2HDM_tB.SetTextSize(0.035);
	leg_2HDM_tB.SetBorderSize(0);
	string output_2HDM_tanB_limits = output + boson + "_2HDM_" + thdm_type + "_Limits_tanB_cB_A" + to_string((double)cB_A) + "_brazil";
	limits.LimitPlotter(TanB_2HDM_Limits,null_vec,leg_2HDM_tB,output_2HDM_tanB_limits,0.5,60,200,900,"36.62(2016)","M_{#Phi} [GeV]","tan(#beta)",true);

	//MSSM limits
	vector<Limit> mssm_limits;
	mssm_limits = limits.GetMSSMLimits(GBR2016,"/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_8_0_20_patch1/src/Analysis/MssmHbb/macros/signal/mhmodp_mu200_13TeV.root");

	TLegend leg_mssm(0.56,0.17,0.83,0.44);
	leg_mssm.SetHeader("m_{h}^{mod+}, #mu = +200");
	leg_mssm.SetFillColor(0);
	leg_mssm.SetTextSize(0.035);
	leg_mssm.SetBorderSize(0);
	string output_mssm_tanB_limits = output + boson + "_MSSM_tanB_brazil";
	limits.LimitPlotter(mssm_limits,null_vec,leg_mssm,output_mssm_tanB_limits,0,60,200,900,"36.62(2016)","M_{#Phi} [GeV]","tan(#beta)",false);


/**/
	return 0;
}

TGraph getLine( TH2& hIn){
	TGraph out;

	double tanB_Target = -100;
	double cosB_A_Target = -100;
	int npoints = 0;
	for(int binx = 1; binx <= hIn.GetNbinsX(); ++binx){
		double cosB_A = hIn.GetXaxis()->GetBinCenter(binx);
		for(int biny = 1; biny <= hIn.GetNbinsY(); ++biny){
			double tanB = hIn.GetYaxis()->GetBinCenter(biny);
			double GxBR_obs_vs_GxBR_pred = hIn.GetBinContent(binx,biny);
			double difference = std::abs(GxBR_obs_vs_GxBR_pred - 1.);

			if(difference < 0.1){
				++npoints;
				tanB_Target = tanB;
				cosB_A_Target = cosB_A;
				out.SetPoint(npoints,cosB_A_Target,tanB_Target);
			}
		}
	}

	return out;
}
