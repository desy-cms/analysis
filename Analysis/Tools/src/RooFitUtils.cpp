/*
 * RooFitUtils.cpp
 *
 *  Created on: 19 Apr 2017
 *      Author: shevchen
 */

#include "Analysis/Tools/interface/RooFitUtils.h"

namespace analysis {

template<> RooRealVar* GetFromRooWorkspace<RooRealVar>(RooWorkspace& w, const std::string& name){
	/*
	 * Explicit specialisation of GetFromRooWorkspace for RooRealVar
	 */
	RooRealVar *var = w.var(name.c_str());
	if(!var) throw std::invalid_argument("Invalid RooRealVar name: " + name);
	return var;
}

template<> RooAbsPdf* GetFromRooWorkspace<RooAbsPdf>(RooWorkspace& w, const std::string& name){
	/*
	 * Explicit specialisation of GetFromRooWorkspace for RooAbsPdf
	 */
	RooAbsPdf *var = w.pdf(name.c_str());
	if(!var) throw std::invalid_argument("Invalid RooAbsPdf name: " + name);
	return var;
}

template<> RooAbsData* GetFromRooWorkspace<RooAbsData>(RooWorkspace& w, const std::string& name){
	/*
	 * Explicit specialisation of GetFromRooWorkspace for RooAbsData
	 */
	RooAbsData *var = static_cast<RooAbsData*>(w.data(name.c_str()));
	if(!var) throw std::invalid_argument("Invalid RooAbsData name: " + name);
	return var;
}

template<> RooDataSet* GetFromRooWorkspace<RooDataSet>(RooWorkspace& w, const std::string& name){
	/*
	 * Explicit specialisation of GetFromRooWorkspace for RooDataSet
	 */
	RooDataSet *var = static_cast<RooDataSet*>(w.data(name.c_str()));
	if(!var) throw std::invalid_argument("Invalid RooDataSet name: " + name);
	return var;
}

template<> RooFormulaVar* GetFromRooWorkspace<RooFormulaVar>(RooWorkspace& w, const std::string& name){
	/*
	 * Explicit specialisation of GetFromRooWorkspace for RooFormulaVar
	 */
	RooFormulaVar *var = static_cast<RooFormulaVar*>(w.function(name.c_str()));
	if(!var) throw std::invalid_argument("Invalid RooFormulaVar name: " + name);
	return var;
}

template RooRealVar* GetFromRooWorkspace<RooRealVar>(RooWorkspace& w, const std::string& name);
template RooAbsPdf* GetFromRooWorkspace<RooAbsPdf>(RooWorkspace& w, const std::string& name);
template RooDataSet* GetFromRooWorkspace<RooDataSet>(RooWorkspace& w, const std::string& name);
template RooAbsData* GetFromRooWorkspace<RooAbsData>(RooWorkspace& w, const std::string& name);
template RooFormulaVar* GetFromRooWorkspace<RooFormulaVar>(RooWorkspace& w, const std::string& name);
} /* namespace analysis */
