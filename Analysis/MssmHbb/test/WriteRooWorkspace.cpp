/*
 * WriteRooWorkspace.cpp
 *
 *  Created on: 10 Apr 2017
 *      Author: shevchen
 */

int WriteRooWorkspace(){
	RooWorkspace w("workspace");

	RooRealVar mbb("mbb","mbb",650,200,650,"GeV");
	mbb.setBins(45);
	w.import(mbb);

	RooRealVar par_a("par_a","par_a",11.1034,0,50);
	par_a.setError(0.345282);
	w.import(par_a);

	RooRealVar par_b("par_b","par_b",0.0966609,0.001,1);
	par_b.setError(0.000963466);
	w.import(par_b);

	RooRealVar slope("slope_bg","slope",0.0364019,0,0.5);
	slope.setError(0.000890197);
	w.import(slope);

	RooRealVar mean("mean_bg","mean",170.728,0,199);
	mean.setError(2.3982);
	w.import(mean);

	RooRealVar turnon("turnon_bg","turnon",251.386,150,650);
	turnon.setError(2.38306);
	w.import(turnon);

	RooRealVar weight("weight","weight",500,0,1000);
	w.import(weight);

	RooFormulaVar background_eff("background_eff","background_eff","1/(1+TMath::Exp(-slope_bg*(mbb-turnon_bg)))",RooArgList(mbb,slope,turnon));
	RooGenericPdf background_dijet("background_dijet","background_dijet","TMath::Exp(-par_a * TMath::Log((mbb- mean_bg)/13000.) - par_b*par_a*(pow(TMath::Log((mbb- mean_bg)/13000.),2)))",RooArgList(mbb,par_a,par_b,mean));
	RooEffProd background("background","background",background_dijet,background_eff);
	w.import(background);

	w.writeToFile("../../BackgroundModel/test/dijetv3logprod_200to650_10GeV_G4/workspace/FitContainer_workspace_corr.root");

	return 0;
}


