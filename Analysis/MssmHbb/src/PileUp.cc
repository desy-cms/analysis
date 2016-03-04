#include "Analysis/MssmHbb/interface/PileUp.h"
#include <cmath>

void PileUp::setDataHisto(TH1F * data) {
 
	h_data = new TH1F (*data);
	h_data->SetDirectory(0);

	if (h_data ==0 ) {
		std::cout << "Error in  PileUp::setDataHisto : failed reading the data histogram." << std::endl;
		exit(0);
	}

	h_data->Scale( (1/(h_data->Integral()) ) );

	if (h_data != 0 and h_MC != 0) 
		this->make_ratio();
	
}

void PileUp::setMCHisto(TH1F * mc) {

	h_MC = new TH1F (*mc);
	h_MC->SetDirectory(0);
	if (h_MC ==0 ) {
		std::cout << "Error in  PileUp::setMCHisto : failed reading the Monte Carlo histogram" << std::endl;
		exit(1);
	}

	h_MC->Scale( (1/(h_MC->Integral()) ) );

	if (h_data != 0 and h_MC != 0) 
		this->make_ratio();
}

void PileUp::make_ratio () {

	if ( fabs( (h_data->Integral()) -1) > 0.01 or fabs( (h_MC->Integral()) -1 ) > 0.01) {
		std::cout << "Error in PileUp::make_ratio. Distributions not normalized to 1 " << std::endl;
		std::cout << " Integral of h_data is " << h_data->Integral() << std::endl;
		std::cout << " Integral of h_MC is " << h_MC->Integral() << std::endl;		
		exit(2);
	}
	
	h_ratio = new TH1F(*h_data);
	h_ratio->SetDirectory(0);
	h_ratio->Divide(h_MC);

}



double PileUp::getPUweight (const double &Nvertex_MC) {

	if (h_MC == 0 or h_data == 0) {
	std::cout << "Error in PileUp::get_PUweight : missing MC and/or data histogram! " << std::endl;
	exit(3);
	}

	if (h_ratio == 0) {
		std::cout << "Error : missing h_ratio " << std::endl;
		exit(2);
	}

	int binNumber = h_ratio->FindBin(Nvertex_MC);
	double PUweight = h_ratio->GetBinContent(binNumber); 
	
	return PUweight;

}


