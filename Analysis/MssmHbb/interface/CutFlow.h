/*
 * CutFlow.h
 *
 *  Created on: 25 июля 2016 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_MSSMHBB_SRC_CUTFLOW_H_
#define ANALYSIS_MSSMHBB_SRC_CUTFLOW_H_

#include <string>								// strings
#include <memory> 								// unique_ptr
#include <map>									// map
#include <iostream>								//standard output

#include "Analysis/MssmHbb/interface/Cut.h"
#include "Analysis/MssmHbb/interface/utilLib.h"

typedef std::shared_ptr<Cut> pCut;

class CutFlow {
public:

	CutFlow() : name_(), description_(), cuts_(), size_(0) {};
	CutFlow(const std::string & name);
	CutFlow(const std::string & name,
			const std::string & description);
	CutFlow(const std::string & name,
			const std::map<std::string,pCut> & cuts);
	CutFlow(const std::string & name,
			const std::string & description,
			const std::map<std::string,pCut> & cuts);

	virtual ~CutFlow() {};

	//copy assignment
	CutFlow & operator=(const CutFlow & cutFlow){
		name_ = cutFlow.name_;
		description_ = cutFlow.description_;
		cuts_ = cutFlow.cuts_;
		size_ = cutFlow.size_;

		return *this;
	}

	void Add(const Cut & cut);												//Add cut to the map
	template<typename T> T check(const std::string & name, const bool & cut);	//Check condition
	const bool & check(const std::string & name, const bool & cut);			//Check condition
	void check(const std::string & name);									//Check condition
	void Print();															//Print information


	//Getters
	const std::string& getDescription() const;
	const std::string& getName() const;
	int getSize() const;

	//Setters
	void setDescription(const std::string& description);
	void setName(const std::string& name);

protected:
	std::string name_;
	std::string description_;
	std::map<std::string,pCut> cuts_;
	int size_;
};

#endif /* ANALYSIS_MSSMHBB_SRC_CUTFLOW_H_ */
