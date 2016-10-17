/*
 * Limit.cpp
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/Limit.h"

Limit::Limit() : Measurement(), mass_(0), observed_(0) {};

Limit::Limit(const int& mass, const double& median, const double& observed, const double& plus1G, const double& minus1G, const double& plus2G, const double& minus2G) :
		Measurement(median,plus1G,minus1G,plus2G,minus2G) , mass_(mass), observed_(observed) {};

Limit::Limit(const int& mass,const double&expected): Measurement(expected), mass_(mass), observed_(0) {};

int Limit::getMass() const {
	return mass_;
}

void Limit::setMass(const int& mass) {
	mass_ = mass;
}

double Limit::getObserved() const {
	return observed_;
}

void Limit::setObserved(const double& observed) {
	observed_ = observed;
}

double Limit::getExpected() const {
	return median_;
}

Limit::~Limit() {
	// TODO Auto-generated destructor stub
}

