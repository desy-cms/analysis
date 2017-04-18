///*
// * FTest.h
// *
// *  Created on: 12 Apr 2017
// *      Author: shevchen
// */

#ifndef ANALYSIS_BACKGROUNDMODEL_INTERFACE_FTEST_H_
#define ANALYSIS_BACKGROUNDMODEL_INTERFACE_FTEST_H_

/*
 * @author shevchen
 *
 * 12 Apr 2017
 */

#include <string>
#include <exception>
//#include <stringstream>
#include <boost/algorithm/string/join.hpp>

#include "Math/ProbFuncMathCore.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooWorkspace.h"
#include "RooPlot.h"

#include "RooSuperNovosibirsk.h"

namespace analysis{
namespace backgroundmodel{
class FTest {
public:
//	FTest() : pdf_family_(""), max_degree_(0), workspace_("workspace") {}  = default;
	FTest()  = default;
	virtual ~FTest() = default;
	FTest(const std::string& pdf_family, const int& max_degree = 7) : pdf_family_(pdf_family), max_degree_(max_degree) {};
	FTest(TH1 & data_obs, RooRealVar & observable, const std::string& pdf_family, const int& max_degree = 7);
	FTest(RooDataSet & data_obs, RooRealVar & observable, const std::string& pdf_family, const int& max_degree = 7);
	FTest(TH1 & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family, const int& max_degree = 7);
	FTest(RooDataSet & data_obs, const double& xmin, const double& xmax, const std::string& pdf_family, const int& max_degree = 7);

	void PerformFTest(const int& max_degree);

//	void Initialise();

	void CheckPdfFamily();
	int GetPdf(const int& degree, const std::string& name);

//	virtual ~FTest();

private:
	std::string pdf_family_;
	RooWorkspace workspace_;
	int max_degree_;
	static const std::vector<std::string> availablePDFs_;
};
}
}
#endif /* ANALYSIS_BACKGROUNDMODEL_INTERFACE_FTEST_H_ */
