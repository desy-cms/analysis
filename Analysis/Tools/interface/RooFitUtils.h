/*
 * RooFitUtils.h
 *
 *  @created on: 19 Apr 2017
 *  @author: shevchen
 *  @description:
 *  Lib for working with RooFit classes.
 */

#ifndef ANALYSIS_TOOLS_INTERFACE_ROOFITUTILS_H_
#define ANALYSIS_TOOLS_INTERFACE_ROOFITUTILS_H_

#include <string>
#include <exception>
#include <iostream>

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooHist.h"

#include "Analysis/MssmHbb/interface/utilLib.h"

namespace analysis {

template<typename T, typename std::enable_if< std::is_base_of<RooCurve, T>::value ||
											std::is_base_of<RooHist, T>::value, T >::type* = nullptr>
								T* GetFromRooPlot(RooPlot& frame, const std::string& object_name){
	/*
	 * Function to get Object of type T from RooPlot
	 */
	T* foo = static_cast<T*>(frame.findObject(object_name.c_str(),T::Class()));
	if(!foo) throw std::invalid_argument("Invalid Object name: " + object_name);
	return foo;
}

template<typename T> T* GetFromRooWorkspace(RooWorkspace& w, const std::string& name){
	/*
	 * Function to get a RooFit object from a RooWorkspace
	 * Generalisation of a template function
	 */
	T *var = static_cast<T*>(w.obj(name.c_str()));
	if(!var) throw std::invalid_argument("Invalid TObject name: " + name);
	return var;
}

//template<typename T> T* GetRooObjectFromTFile(const std::string& file_name)
template<typename T> T* GetRooObjectFromTFile( TFile& file, const std::string& obj_name, const std::string& workspace_name = "workspace"){
	/*
	 * Function to get RooObject from TFile through the interaction with RooWorkspace
	 */
	auto &workspace = *GetFromTFile<RooWorkspace>(file, workspace_name); 	//Get RooWorkspace
	auto *obj = GetFromRooWorkspace<T>(workspace,obj_name); 				//Get an object
	return obj;
}

template<typename T> T* GetRooObjectFromTFile( const std::string& file_name, const std::string& obj_name, const std::string& workspace_name = "workspace"){
	/*
	 * Function to get RooObject from TFile through the interaction with RooWorkspace
	 */
	auto &workspace = *GetFromTFile<RooWorkspace>(file_name, workspace_name); 	//Get RooWorkspace
	auto *obj = GetFromRooWorkspace<T>(workspace,obj_name); 					//Get an object
	return obj;
}

} /* namespace analysis */



#endif /* ANALYSIS_TOOLS_INTERFACE_ROOFITUTILS_H_ */
