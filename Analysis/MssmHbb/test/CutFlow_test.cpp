/*
 * CutFlow_test.cpp
 *
 *  Created on: 25 июля 2016 г.
 *      Author: rostyslav
 */

#include <iostream>

#include "Analysis/MssmHbb/interface/CutFlow.h"
#include "Analysis/MssmHbb/interface/Cut.h"
#include "TRandom2.h"

int main(){

	CutFlow * cutflow1 = new CutFlow("test_selection","low Mass");
	double a = 6;
	double d = 2;
	double c = 4;
	double x = 0;

	std::cout<<"a = "<<a<<" d = "<<d<<"c = "<<c<<std::endl;

	//Create single Cut object
	Cut cut("x>=a");

	//Test of event-by-event case
	const int nentries = 10;
	for(int i = 0; i < nentries; ++i){
		//assign random number between [0:10]
		x = gRandom->Uniform(10);
		//Starting Cut-Flow:

		std::cout<<"X = "<<x<<std::endl;

		//Total number of events:
		cutflow1->check("Total Number");

		//test template void, for additional counters:
		cutflow1->check<void>("x<c",x<c);

		//Add cut-by-cut:
		if(!cutflow1->check("x>d",x>d)) continue;
		if(!cut.check(x>=a))continue;
	}

	//test Add method:
	cutflow1->Add(cut);
	cutflow1->Print();

	return 0;
}

