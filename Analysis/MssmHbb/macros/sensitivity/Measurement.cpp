/*
 * Measurement.cpp
 *
 *  Created on: Sep 5, 2016
 *      Author: shevchen
 */

#ifndef MEASUREMENT_CPP
#define MEASUREMENT_CPP

#include "memory"

class Measurement{

public:
	Measurement(const double & val) : val(val), val_up(0), val_down(0) {};
	Measurement() : val(0), val_up(0), val_down(0){};
	Measurement(const double & central, const double & up, const double & down) : val(central), val_up(up), val_down(down) {};
	virtual ~Measurement(){};
	//Copy assignment
	Measurement & operator=(const Measurement & other){
		val = other.val;
		val_up = other.val_up;
		val_down = other.val_down;
		return *this;
	}

	double val;
	double val_up;
	double val_down;
};

#endif
