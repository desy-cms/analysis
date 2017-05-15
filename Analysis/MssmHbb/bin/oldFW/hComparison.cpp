//============================================================================
// Name        : hComparison.cpp
// Author      : Rostyslav Shevchenko
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Analysis/MssmHbb/interface/basicHistoComparison.h"

using namespace std;

int main(int argc, char *argv[]) {

	basicHistoComparison s = basicHistoComparison();
	s.addHistogram("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/outputFiles/Resultsoutput_9.root","HLT_PFJet80/hpf80_eff");
	TCanvas *can = new TCanvas("can","",1000,800);
	s.draw(can);
	can->SaveAs("lol.png");

	return 0;
}
