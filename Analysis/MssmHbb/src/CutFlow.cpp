/*
 * CutFlow.cpp
 *
 *  Created on: 25 июля 2016 г.
 *      Author: rostyslav
 */

#include "Analysis/MssmHbb/interface/CutFlow.h"

bool comp(const std::pair< std::string, pCut > &a, const std::pair< std::string, pCut  > &b)
{
    return a.second->getCounter() > b.second->getCounter();
}

CutFlow::CutFlow(const std::string & name, const std::string & description,	const std::map<std::string,pCut> & cuts){
	name_ = name;
	description_ = description;
	cuts_ = cuts;
	size_ = cuts.size();
}

CutFlow::CutFlow(const std::string & name, const std::map<std::string,pCut> & cuts){
	name_ = name;
	description_ = "";
	cuts_ = cuts;
	size_ = cuts.size();
}

CutFlow::CutFlow(const std::string & name){
	name_ = name;
	description_ = "";
	cuts_ = {};
	size_ = 0;
}

CutFlow::CutFlow(const std::string & name, const std::string & description){
	name_ = name;
	description_ = description;
	cuts_ = {};
	size_ = 0;
}

void CutFlow::Print(){
	std::cout<<"Selection: "<<name_<<std::endl;
	std::cout<<"Trigger Type: "<<description_<<std::endl;
	std::cout<<"Number of Cuts: "<<size_<<std::endl;
	std::vector<std::pair<std::string,pCut>> vec(cuts_.begin(),cuts_.end());
	std::sort(vec.begin(),vec.end(),comp);
	if(size_ != 0){
		for(const auto & cut : vec){
			std::cout<<cut.first<<" : "<<cut.second->getCounter()<<" events"<<std::endl;
		}
	}
}

template<typename T> T CutFlow::check(const std::string & name, const bool & cut){
	return check(name,cut);
}

template<> void CutFlow::check(const std::string & name, const bool & cut){
	if(cuts_[name] == nullptr) {
		cuts_[name] = std::make_shared<Cut>(name);
		++size_;
	}
	if(cut) cuts_[name]->increment();
}

const bool & CutFlow::check(const std::string & name, const bool & cut){
	if(cuts_[name] == nullptr) {
		cuts_[name] = std::make_shared<Cut>(name);
		++size_;
	}
	return cuts_[name]->check(cut);
}

void CutFlow::check(const std::string & name){
	if(cuts_[name] == nullptr) {
		cuts_[name] = std::make_shared<Cut>(name);
		++size_;
	}
	cuts_[name]->increment();
}

void CutFlow::Add(const Cut & cut){
	std::string name = cut.getName();
	if(cuts_[name] != nullptr){
		std::cerr<<"CutFlow::Add - Cut element "<<name<<" already exist in the CutFlow map"<<std::endl;
		exit(-1);
	}
	else {
		cuts_[name] = std::make_shared<Cut>(cut);
		++size_;
	}
}

const std::string& CutFlow::getDescription() const {
	return description_;
}

void CutFlow::setDescription(const std::string& description) {
	description_ = description;
}

const std::string& CutFlow::getName() const {
	return name_;
}

void CutFlow::setName(const std::string& name) {
	name_ = name;
}

int CutFlow::getSize() const {
	return size_;
}
