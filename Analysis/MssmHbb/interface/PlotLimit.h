/*
 * PlotLimit.h
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_PLOTLIMIT_H_
#define MSSMHBB_INTERFACE_PLOTLIMIT_H_

// cpp includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// ROOT includes
#include "TMath.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TH2.h"

//Style files from Alexei
#include "Analysis/MssmHbb/macros/Drawer/HbbStylesNew.C"
#include "Analysis/MssmHbb/interface/Luminosity.h"

//my includes
#include "Analysis/MssmHbb/interface/Limit.h"

// xsection estimation
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.h"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"


class PlotLimit {
public:
	PlotLimit(const std::string & fileList = "Hbb.limits", const bool& blindData = true);
	virtual ~PlotLimit();

	// Sigma x BR limits
	void CalculateSigmaBRLimits();
	void CalculateTanBetaLimits(const std::string& name = "mhmodp_mu200", const std::string& benchmark = "mhmodp_mu200_13TeV.root");
	void CalculateTanBetaLimits(const std::vector<std::string> & names, const std::vector<std::string> & benchmarks);

	void PlotSigmaBRLimits(const std::string& output = "",
		       const float& yMin = 0.5,
		       const float& yMax = 1000,
			   const float& xMin = 200,
			   const float& xMax = 1300,
		       const std::string& Lumi = "2.69 fb^{-1}",
			   const std::string& xtitle = "m_{#Phi} [GeV]",
			   const std::string& ytitle = "95% C.L. limit on #sigma#times BR [pb]",
			   const bool& logY = true);
	void PlotSigmaBRLimits(const std::vector<Limit>& differ_limits,
			   const std::string& output = "13vs8TeV",
		       const float& yMin = 0.5,
		       const float& yMax = 1000,
			   const float& xMin = 200,
			   const float& xMax = 1300,
		       const std::string& Lumi = "19.7 fb^{-1} (8 TeV) vs 2.69 fb^{-1}",
			   const std::string& xtitle = "m_{#Phi} [GeV]",
			   const std::string& ytitle = "95% C.L. limit on #sigma#times BR [pb]",
			   const bool& logY = true);
	void PlotTanBetaLimit(const std::string& name = "mhmod+ mu=+200",
			  const std::string& output = "",
		      const float& yMin =   1,
		      const float& yMax =  60,
		      const float& xMin = 200,
		      const float& xMax = 700,
		      const std::string& Lumi = "2.69 fb^{-1}",
		      const std::string& xtitle = "m_{A} [GeV]",
		      const std::string& ytitle = "tan#beta",
		      const bool& logY = false
		      );
	void PlotTanBetaLimit(const std::vector<Limit>& differ_limits,
			  const std::string& name = "mhmod+ mu=+200",
			  const std::string& output = "13vs8TeV",
		      const float& yMin =   1,
		      const float& yMax =  60,
		      const float& xMin = 200,
		      const float& xMax = 700,
		      const std::string& Lumi = "24.6 fb^{-1} (7 + 8 TeV) vs 2.69 fb^{-1}",//"4.9 fb^{-1} (7 TeV) + 19.7 fb^{-1} (8 TeV) vs 2.69 fb^{-1}",
		      const std::string& xtitle = "m_{A} [GeV]",
		      const std::string& ytitle = "tan#beta",
		      const bool& logY = false
		      );

private:

	double XSecToTanBeta_(mssm_xs_tools my, double mA,
			     double xsection,
			     double minTanBeta,
			     double maxTanBeta);

	void LimitPlotter_(const std::vector<Limit>& limits,
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

	std::string fileList_;
	bool blindData_;
	std::vector<Limit> GxBr_limits_;
	std::vector<std::pair<std::string, std::vector<Limit> > > tanBeta_limits_;
};

#endif /* MSSMHBB_INTERFACE_PLOTLIMIT_H_ */
