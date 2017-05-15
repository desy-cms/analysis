using namespace std;

int fitPenalty(){

	vector<double> penalty8TeV = {2.66,2.14,2.01,1.98,1.93,1.94};
//	vector<double> penalty8TeV = { 2.2951 / sqrt(21661) / 1.96 * 0.0064 * 19800,
//								   1.2644 / sqrt(5970) / 1.96 * 0.0061 * 19800,
//								   0.8438 / sqrt(1849) / 1.96 * 0.0051 * 19800,
//								   0.6553 / sqrt(643)  / 1.96 * 0.0039 * 19800,
//								   0.5689 / sqrt(275)  / 1.96 * 0.0029 * 19800,
//								   0.5162 / sqrt(52)   / 1.96 * 0.0014 * 19800};
	vector<double> mass8TeV = {300,400,500,600,700,900};
	// 13 TeV numbers
	vector<double> signal_eff 	= {0.00573913,0.00845437,0.0102627,0.0114808,0.0110365,0.0104916,0.00812815,0.0057764,0.0040667};
	vector<double> UL95			= {11.4062,10.9688,5.20312,4.26562,2.60156,1.57031,0.957031,0.785156,0.800781};
	vector<double> sqrtBg		= {403.709,384.128,330.79,228.332,171.068,123.283,69.6006,42.226,29.4268};
	vector<double> mass13TeV = {300,350,400,500,600,700,900,1100,1300};
	vector<double> penalty13TeV;
	const double ful = 1.96;
	const double Lumi13TeV = 36620;
	for (int i = 0; i != signal_eff.size();++i){
		penalty13TeV.push_back( UL95.at(i) / ful / sqrtBg.at(i) * signal_eff.at(i) * Lumi13TeV );
		cout<<"M = "<<mass13TeV.at(i)<<" UL95 = "<<UL95.at(i)<<" sqrt(BG) = "<<sqrtBg.at(i)<<" eff = "<<signal_eff.at(i)<<" const = "<<Lumi13TeV/ful<<" penalty = "<<penalty13TeV.at(i)<<endl;
	}

	TGraph *gr_8TeV = new TGraph(penalty8TeV.size(),mass8TeV.data(),penalty8TeV.data());
	gr_8TeV->SetLineColor(2);
	gr_8TeV->SetLineWidth(1.5);
	TGraph *gr_13TeV = new TGraph(penalty13TeV.size(),mass13TeV.data(),penalty13TeV.data());
	gr_13TeV->SetMarkerStyle(20);
	gr_13TeV->Draw("AP");
	gr_8TeV->Draw("l");

	return 0;
}
