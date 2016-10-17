/*
 * Limit.h
 *
 *  Created on: Sep 25, 2016
 *      Author: shevchen
 */

#ifndef MSSMHBB_INTERFACE_LIMIT_H_
#define MSSMHBB_INTERFACE_LIMIT_H_

#include "Measurement.h"

class Limit: public Measurement {
public:
	Limit();
	Limit(const int& mass, const double& median, const double& observed, const double& plus1G, const double& minus1G, const double& plus2G, const double& minus2G);
	Limit(const int& mass, const Measurement& mes, const double& observed);
	Limit(const int& mass, const double& expected);
	virtual ~Limit();

	int getMass() const;
	void setMass(const int& mass);
	double getObserved() const;
	void setObserved(const double& observed);

	//Additional getters for Expected limits
	double getExpected() const;

private:
	int mass_;
	double observed_;
};

#endif /* MSSMHBB_INTERFACE_LIMIT_H_ */
