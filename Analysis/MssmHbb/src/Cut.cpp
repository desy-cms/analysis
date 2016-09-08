/*
 * Cut.cpp
 *
 *  Created on: 25 июля 2016 г.
 *      Author: rostyslav
 */

#include "Analysis/MssmHbb/interface/Cut.h"

const bool & Cut::check(const bool & expression){
	if(expression) ++counter_;
	return expression;
}
