/*
 * HbbLimits.h
 *
 *  Created on: Dec 13, 2016
 *      Author: shevchen
 */

#include <string>
#include <iostream>
#include <map>

#include "Analysis/MssmHbb/interface/Luminosity.h"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

#include "TH3.h"
#include <TH2.h>
#include <TH1.h>
#include "TLegend.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphAsymmErrors.h"


#ifndef MSSMHBB_INTERFACE_HBBLIMITS_H_
#define MSSMHBB_INTERFACE_HBBLIMITS_H_
// Root includes
#include <TTree.h>
#include <TMath.h>
#include <TH1.h>
#include <TH3F.h>

// cpp includes
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//my includes
#include "Analysis/MssmHbb/interface/Limit.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

// MSSM tools
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"

namespace analysis {
namespace mssmhbb {

class HbbLimits {
public:
	HbbLimits();
	HbbLimits(const bool& blindData, const bool& test = false);
	HbbLimits(const bool& blindData, const std::string& boson, const bool& test = false);
	virtual ~HbbLimits();

	struct THDMScan{
		TH2D expected;
		TH2D observed;
	};

//	Method to read Limits from the combine output
	std::vector<Limit> ReadCombineLimits(const std::string& file_name);
//	Method to read one limit
	const Limit ReadCombineLimit(const std::string& tfile_name, const bool& blindData);

//	Method to set higgs boson to be used: A/H/Degenarated
	void SetHiggsBoson(const std::string& boson);

//	Method to get tanBeta limits according to theoretical Br and Sigma
	const std::vector<Limit> GetMSSMLimits(const std::vector<Limit>& GxBR_limits, const std::string& benchmark_path, const std::string& uncert = "", const bool& UP = false, const std::string& benchmark_ref_path = "", const double& tanBref = -1);

//	Method to get 2HDM 3D GxBR
	TH3D Get2HDM_GxBR_3D(const std::string& benchmark_path);
//	Method to get 2HDM 2D GxBR for particular value of VAR
	TH2D Get2HDM_GxBR_2D(const TH3& GxBR, const double& var, const std::string& axis = "X");

//	Method to get 2D limits of G_95%CL / G_pred for particular mass point
	THDMScan Get2HDMmuScan(const TH2& GxBR_2hdm, const Limit& GxBR_95CL);
//	Method to calculate 2HDM limits
	std::vector<Limit> Get2HDM_Limits(const TH2& GxBR_2hdm, const Limit& GxBR_95CL, const double& xmin = -1, const double& xmax = 1);
//	Method to plot Brazil 2HDM limits
	const std::vector<Limit> Get2HDM_1D_Limits(const TH2& GxBR_2hdm,const std::vector<Limit>& GxBR_limits);

	//	Make output .txt with limits written in a human readable way
	void Write(const std::vector<Limit>& limits, const std::string& name);

	//	Method to receive tanBeta value from Sigma x BR in MSSM interpretation
	double MSSMTanBeta(const std::string& benchmark_path, double mA, double xsection, const std::string& uncert = "", const bool& UP = false, const std::string& benchmark_ref_path = "", const double& tanBref = -1);
	// Method to receive tanBeta value for 1D Siga x BR in 2HDMinterpretation
	double THDMTanBeta(const TH2& GxBR_2hdm, double mA, double xsection);

	// Method to get contour of atlas plot
	TGraph GetAtlasZhll_flipped();

//	//	Method to get 2HDM tanBeta vs MA limits
//		const std::vector<Limit> Get2HDM_tanB_mA_Limits(const std::vector<Limit>& GxBR_limits, const std::string& benchmark_path, const double& sinB_A);
//	//	Method to get 2HDM sin(beta-alpha) vs tanBeta limits for particular value of mA
//		const std::vector<Limit> Get2HDM_cosB_A_tanB_Limits(const std::vector<Limit>& GxBR_limits, const std::string& benchmark_path, const double& mA);
//		const std::vector<Limit> Get2HDM_cosB_A_tanB_Limits(const Limit& GxBR_limit, const std::string& benchmark_path, const double& mA); // TODO: Implement!

	void LimitPlotter(const std::vector<Limit>& limits,
			const std::vector<Limit>& differ_limits,
			TLegend& leg,
			const std::string& output = "",
			const float& yMin =   1,
			const float& yMax =  60,
			const float& xMin = 200,
			const float& xMax = 700,
			const std::string& Lumi = "2.69 fb^{-1}",
			const std::string& xtitle = "m_{A} [GeV]",
			const std::string& ytitle = "tan#beta",
			const bool& logY = true);

private:

	bool blindData_;
	std::string boson_;
	bool TEST_;
	HbbStyle style_;

protected:
	void CheckHiggsBoson();
	void SetTHDMHistos(TFile& file,std::map<std::string,TH3D*>&);

};

inline void HbbLimits::SetHiggsBoson(const std::string& boson) {

	boson_ = boson;
}

} /* namespace MssmHbb */
} /* namespace Analysis */

#endif /* MSSMHBB_INTERFACE_HBBLIMITS_H_ */
