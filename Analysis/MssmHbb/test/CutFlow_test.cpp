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

	CutFlow * cutflow1 = new CutFlow("test_selection");
	double a = 6;
	double c = 4;
	double d = 2;
	double x = 0;

	//Test of event-by-event case
	const int nentries = 10;
	for(int i = 0; i < nentries; ++i){
		x = gRandom->Uniform(10);
		std::cout<<"x = "<<x<<" d = "<<d<<" x<d? "<<cutflow1->check("x<d",x<d)<<std::endl;
		std::cout<<"x = "<<x<<" c = "<<c<<" x<c? "<<cutflow1->check("x<c",x<c)<<std::endl;
	}
	cutflow1->Print();

	//test Add method:
	Cut cut("x<a");
	x = gRandom->Uniform(10);
	std::cout<<"x = "<<x<<" a = "<<a<<" x<a? "<<cut.check(x<a)<<std::endl;
	cutflow1->Add(cut);
	cutflow1->Print();

	return 0;
}

