/*
 * Cut.h
 *
 *  Created on: 25 июля 2016 г.
 *      Author: rostyslav
 */

#ifndef ANALYSIS_MSSMHBB_SRC_CUT_H_
#define ANALYSIS_MSSMHBB_SRC_CUT_H_

#include <string>

class Cut {
public:

	Cut() : name_(), counter_(0) {};
	Cut(const std::string & name) : name_(name), counter_(0) {};
	Cut(const Cut & cut) : name_(cut.name_), counter_(cut.counter_) {};
	const bool & check(const bool & expression);

	virtual ~Cut() {}

	void increment();

	//operators
	//copy assignment
	Cut & operator=(const Cut & cut){
		name_ = cut.name_;
		counter_ = cut.counter_;
		return *this;
	}
	//Add
	Cut & operator+(const Cut & cut){
		 name_ = name_ + " + " + cut.name_;
		 counter_ = counter_ + cut.counter_;
		 return *this;
	}
	//Subtraction
	Cut & operator-(const Cut & cut){
		 name_ = name_ + " - " + cut.name_;
		 counter_ = counter_ - cut.counter_;
		 return *this;
	}

	//Getters
	int getCounter() const;
	const std::string& getName() const;

	//Setters
	void setCounter(int counter);
	void setName(const std::string& name);

protected:
	std::string name_;
	int counter_;

};

inline int Cut::getCounter() const { return counter_;}
inline void Cut::setCounter(int counter) { counter_ = counter;}
inline const std::string& Cut::getName() const { return name_; }
inline void Cut::setName(const std::string& name) {	name_ = name;}
inline void Cut::increment(){ ++counter_; }

#endif /* ANALYSIS_MSSMHBB_SRC_CUT_H_ */
