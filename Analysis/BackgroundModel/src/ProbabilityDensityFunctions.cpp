/*
 * ProbabilityDensityFunctions.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: shevchen
 */

#include "Analysis/BackgroundModel/interface/ProbabilityDensityFunctions.h"

using namespace analysis::backgroundmodel;

ProbabilityDensityFunctions::ProbabilityDensityFunctions(RooWorkspace& workspace, const std::string& var_name) : peak_(-100), workspace_(&workspace), var_(var_name) {
	// TODO Auto-generated constructor stub
	//	workspace_.reset(&workspace);// = &workspace;
	//Check whether "X" variable exist in the workspace:
	if(!workspace_->var(var_name.c_str())){
		std::cerr<<"Variable "<<var_name<<" wasn't found in the provided workspace. Interupt"<<std::endl;
		exit(-1);
	}
}

ProbabilityDensityFunctions::~ProbabilityDensityFunctions() {
	// TODO Auto-generated destructor stub
//	delete workspace_;
}

void ProbabilityDensityFunctions::setPdf(const std::string& function, const std::string& name){

	if (workspace_->allPdfs().find(name.c_str())) {
		std::stringstream msg;
	    msg << "Model for " << name << " has already been set!";
	    throw std::runtime_error(msg.str());
	}

	int numCoeffs = defaultNumberOfCoefficients_;
	std::vector<std::string> nameSplitted;
	boost::split(nameSplitted, function, boost::is_any_of(","));
	switch (nameSplitted.size()) {
	case 1: break;
	case 2:
		try {
	      numCoeffs = std::stoi(nameSplitted[1]);
		} catch (const std::invalid_argument& ex) {
	      if (std::string(ex.what()) == "stoi") {
	        std::cerr << "Conversion of '" << nameSplitted[1]
	                  << "' to an integral value for the number of coefficients "
	                  << "failed. Using the default value of " << numCoeffs << "."
	                  << std::endl;
	      } else {
	        throw;
	      }
	    }
	    break;
	  default:
	    std::stringstream msg;
	    msg << "Unsupported number of arguments for fit model: "
	        << nameSplitted.size();
	    throw std::runtime_error(msg.str());
	  }

	if 		(function == "novosibirsk") getNovosibirsk(name);			//Novosibirsk (3)
	else if (function == "novopsprod") getNovoPSProd(name);		//PS x Novosibirsk (3)
	else if (function == "novoeffprod") getNovoEffProd(name);             //Eff x Novosibirsk (5)
	else if (function == "novopshighMpol4") getNovoPSHighMPol4(name);	//PS x Novosibirsk (3) HighM Poly[4]
	else if (function == "crystalball") getCrystalBall(name);
	else if (function == "crystalpsprod") getCrystalPSProd(name);		//PS x CrystalBall (4)
	else if (function == "crystaleffprod") getCrystalEffProd(name);       //Eff x CrystalBall (6)
	else if (function == "expeffprod") getExpEffProd(name);
	else if (function == "doublecb") getDoubleCB(name);
	else if (function == "dijetv1") getDijetv1(name);                     //Dijet (EXO-16-020) (3)
	else if (function == "dijetv1psprod") getDijetv1PSProd(name);         //PS x Dijet (3)
	else if (function == "dijetv2") getDijetv2(name); 			//Dijet X(750) (3)
	else if (function == "dijetv2psprod") getDijetv2PSProd(name);		//PS x Dijet (3)
	else if (function == "expgausexp") getExpGausExp(name);
	else if (function == "gausexp") getGausExp(name);
	else if (function == "doublegausexp") getDoubleGausExp(name);
	else if (function == "triplegausexp") getTripleGausExp(name);
	else if (function == "gausexppsprod") getGausExpPSProd(name);		//PS x GausExp (3)
	else if (function == "expbwexp") getExpBWExp(name);
	else if (function == "bukin") getBukin(name);
	else if (function == "bukinpsprod") getBukinPSProd(name);		//PS x Bukin (5)
	else if (function == "bernstein") getBernstein(name, numCoeffs);
	else if (function == "chebychev") getChebychev(name, numCoeffs);
	else if (function == "berneffprod") getBernEffProd(name, numCoeffs);
	else if (function == "bernpsprod") getBernPSProd(name, numCoeffs);
	else if (function == "chebeffprod") getChebEffProd(name, numCoeffs);
	else if (function == "breitwigner") getBreitWigner(name);
	else if (function == "relbreitwigner") getRelBreitWigner(name);
	else if (function == "quadgausexp") getRooQuadGausExp(name);
	else if (function == "mynovosibirsk") getMyNovosibirsk(name);
	else if (function == "mynovopsprod") getMyNovoPSProd(name);
	else if (function == "extnovosibirsk") getExtNovosibirsk(name);
	else if (function == "extnovopsprod") getExtNovoPSProd(name);
	else if (function == "extnovoeffprod") getExtNovoEffProd(name);
	else {
		std::stringstream msg;
		msg << "Model '" << function
				<< "' not implemented! Choose one of the following: "
		<< boost::algorithm::join(availableModels_, ", ");
	    throw std::runtime_error(msg.str());
	}
}

void ProbabilityDensityFunctions::getNovosibirsk(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar peak = RooRealVar("peak1", "peak", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar width = RooRealVar("width1", "width", 50.0, 5.0, var.getMax()/2.0, "GeV");
	RooRealVar tail = RooRealVar("tail1", "tail", -0.1, -1.0, 1.0);
	RooNovosibirsk novo(name.c_str(),(name + "_novosibirsk").c_str(),var,peak,width,tail);

	workspace_->import(novo);
}
void ProbabilityDensityFunctions::getNovosibirsk(const std::string& name, const std::string& title, RooRealVar& peak
																							, RooRealVar& width
																							, RooRealVar& tail){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooNovosibirsk novo2(name.c_str(),title.c_str(),var,peak,width,tail);
	workspace_->import(novo2);
}

void ProbabilityDensityFunctions::getNovoPSProd(const std::string& name,const std::string& title, RooRealVar& peak
																							, RooRealVar& width
																							, RooRealVar& tail
																							, RooFormulaVar& PS){
	getNovosibirsk((name + "_novo").c_str(),(name + "_novo").c_str(),peak,width,tail);
	RooNovosibirsk& novo = (RooNovosibirsk&) *workspace_->pdf((name + "_novo").c_str());
	RooEffProd novopsprod(name.c_str(), title.c_str(), novo, PS);
	workspace_->import(novopsprod);
}

void ProbabilityDensityFunctions::getNovoPSProd(const std::string& name){
	getPhaseSpace(name+"_phasespace");
	getNovosibirsk((name+"_novo"));

	RooNovosibirsk& novo 		= (RooNovosibirsk&) *workspace_->pdf((name + "_novo").c_str());
	RooFormulaVar&  phasespace  = (RooFormulaVar&)  *workspace_->function((name + "_phasespace").c_str());
	RooEffProd novoEffProd(name.c_str(),(name + "_novopsprod").c_str(),novo,phasespace);

	workspace_->import(novoEffProd);
}

void ProbabilityDensityFunctions::getNovoEffProd(const std::string& name){
	std::string eff_name = name + "_novosibirskeff";
	getEfficiency(eff_name);
	std::string novo_name = name + "_novosibirsk";
	getNovosibirsk(novo_name);

	RooNovosibirsk& novo 		= (RooNovosibirsk&) *workspace_->pdf((novo_name).c_str());
	RooFormulaVar&  eff  = (RooFormulaVar&)  *workspace_->function((eff_name).c_str());
	RooEffProd novoEffProd(name.c_str(),(name + "_novoeffprod").c_str(),novo,eff);
	workspace_->import(novoEffProd);
}

void ProbabilityDensityFunctions::getNovoPSHighMPol4(const std::string& name){
	std::string novo_name = name + "_novosibirsk";
	getNovosibirsk(novo_name);
	RooNovosibirsk& novo 		= (RooNovosibirsk&) *workspace_->pdf((novo_name).c_str());

	RooRealVar& var = *workspace_->var(var_.c_str());
	RooPhaseSpacePol4 phasespace((name+"_pspol4").c_str(),(name+"_phasespace").c_str(),var);

	RooEffProd novopsprod(name.c_str(),
	                        (name + "_novopsprod").c_str(), novo, phasespace);
	workspace_->import(novopsprod);
}

void ProbabilityDensityFunctions::getCrystalBall(const std::string& name){
	  RooRealVar& var = *workspace_->var(var_.c_str());

	  RooRealVar m0("m0", "m0",getPeakStart(), 50.0, 500.0, "GeV");
	  RooRealVar sigma("sigma", "sigma", 35.0, 10.0, 100.0, "GeV");
	  RooRealVar alpha("alpha", "alpha", -1.0, -0.1);
	  RooRealVar n("n", "n", 20.0, 3.0, 100.0);
	  RooCBShape cb(name.c_str(), (name + "_crystalball").c_str(),
	                var, m0, sigma, alpha, n);
	  workspace_->import(cb);
}

void ProbabilityDensityFunctions::getCrystalPSProd(const std::string& name){
	std::string cb_name = name + "_cb";
	getCrystalBall(cb_name);
	RooCBShape& cb 		= (RooCBShape&) *workspace_->pdf((cb_name).c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd crystalpsprod(name.c_str(),
	                          (name + "_crystalpsprod").c_str(), cb, ps);
	workspace_->import(crystalpsprod);
}

void ProbabilityDensityFunctions::getCrystalEffProd(const std::string& name){
	std::string cb_name = name + "_cb";
	getCrystalBall(cb_name);
	RooCBShape& cb 		= (RooCBShape&) *workspace_->pdf((cb_name).c_str());

	std::string eff_name = name + "_eff";
	getEfficiency(eff_name);
	RooFormulaVar& eff = (RooFormulaVar&) *workspace_->function(eff_name.c_str());

	RooEffProd crystalEffprod(name.c_str(),
	                            (name + "_crystaleffprod").c_str(), cb, eff);
	workspace_->import(crystalEffprod);
}

void ProbabilityDensityFunctions::getExpEffProd(const std::string& name){
	std::string exp_name = name + "_exp";
	getExp(exp_name);
	RooExponential& exp = (RooExponential&) *workspace_->pdf(exp_name.c_str());

	std::string eff_name = name + "_eff";
	getEfficiency(eff_name);
	RooFormulaVar& eff = (RooFormulaVar&) *workspace_->function(eff_name.c_str());

	RooEffProd expEffProd(name.c_str(),(name + "_expeffprod").c_str(), exp, eff);
	workspace_->import(expEffProd);
}

void ProbabilityDensityFunctions::getDoubleCB(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean("mean", "mean", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar width("width", "width", 35.0, 5.0, 100.0, "GeV");
	RooRealVar alpha1("alpha1", "alpha1", -1.0, -0.1);
	RooRealVar n1("n1", "n1", 20.0, 3.0, 100.0);
	RooRealVar alpha2("alpha2", "alpha2", 0.1, 1.0);
	RooRealVar n2("n2", "n2", 20.0, 3.0, 100.0);
	RooDoubleCB doubleCB(name.c_str(),
	                       (name + "_doublecb").c_str(),
	                       var, mean, width, alpha1, n1, alpha2, n2);
	workspace_->import(doubleCB);
}

void ProbabilityDensityFunctions::getDijetv1(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar par_b("par_b", "par_b", -13, -1000, 1000.);
	RooRealVar par_c("par_c", "par_c", -1.4, -1000, 1000.);
	std::string formula = "pow(" + std::string(var.GetName()) + "/13000. , par_b + par_c * TMath::Log(" + std::string(var.GetName()) + "/13000.))";
	RooGenericPdf dijet(name.c_str(),
	                     (name + "_dijet").c_str(),
	                     formula.c_str(),	//(x)^(P1+P2log(x)) where x = mbb/sqrt(s)
	                     RooArgList(var,par_b,par_c));
	workspace_->import(dijet);
}

void ProbabilityDensityFunctions::getDijetv1PSProd(const std::string& name){
	std::string di_name = name + "_dijet";
	getDijetv1(di_name);
	RooGenericPdf& dijet = (RooGenericPdf&) *workspace_->pdf(di_name.c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd dijetpsprod(name.c_str(),
	                        (name + "_dijetpsprod").c_str(), dijet, ps);
	workspace_->import(dijetpsprod);
}

void ProbabilityDensityFunctions::getDijetv2(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	std::string var_name = var.GetName();
	RooRealVar par_a("par_a", "par_a", 8.0, 0.0, 10.0);
	RooRealVar par_b("par_b", "par_b", 0.06, 0.02, 0.08);
	RooRealVar par_c("par_c", "par_c", 70., 65., 100.);			//control non-zero values when start mbb > 200 GeV
	RooGenericPdf dijet(name.c_str(),				//(-1+cx)e^-alnx(1+blnx)
	                     (name + "_dijet").c_str(),
	                     ("TMath::Exp(-par_a * TMath::Log(" + var_name + "/13000.) - par_b*par_a*(pow(TMath::Log(" + var_name + "/13000.),2)))*(-1+par_c*(" + var_name + "/13000.))").c_str(),
	                     RooArgList(var,par_a,par_b,par_c));
	workspace_->import(dijet);
}

void ProbabilityDensityFunctions::getDijetv2PSProd(const std::string& name){
	std::string di_name = name + "_dijet";
	getDijetv2(di_name);
	RooGenericPdf& dijet = (RooGenericPdf&) *workspace_->pdf(di_name.c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd dijetpsprod(name.c_str(),
	                        (name + "_dijetpsprod").c_str(), dijet, ps);
	workspace_->import(dijetpsprod);
}

void ProbabilityDensityFunctions::getExpGausExp(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean("mean", "mean", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
	RooRealVar left("left", "left", 0.1, 15.0);
	RooRealVar right("right", "right", 0.1, 15.0);
	RooExpGausExp expGausExp(name.c_str(),
	                           (name + "_expgausexp").c_str(),
	                           var, mean, sigma, left, right);
	workspace_->import(expGausExp);
}

void ProbabilityDensityFunctions::getGausExp(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean("mean", "mean", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar sigma("sigma", "sigma", 35.0, 5.0, 150.0, "GeV");
	RooRealVar right("right", "right", 0.4, 0.1, 15.0);
	RooGausExp gausExp(name.c_str(),
	                           (name + "_gausexp").c_str(),
	                           var, mean, sigma, right);
	workspace_->import(gausExp);
}

void ProbabilityDensityFunctions::getDoubleGausExp(const std::string& name){
	std::string ge_name = name + "_gausexp";
	getGausExp(ge_name);
	RooGausExp& gausExp = (RooGausExp&) *workspace_->pdf(ge_name.c_str());

	std::string gaus_name = name + "_gaus2";
	getGaus(gaus_name,"mean2","sigma2");
	RooGaussian& gaus2 = (RooGaussian&) *workspace_->pdf(gaus_name.c_str());

	RooRealVar g1frac("g1frac","fraction of gauss1",0.5);
	RooRealVar g2frac("g2frac","fraction of gauss2",0.5);
	RooAddPdf sum(name.c_str(),(name + "_doublegausexp").c_str(),RooArgList(gausExp,gaus2),RooArgList(g1frac,g2frac));
	workspace_->import(sum);
}

void ProbabilityDensityFunctions::getTripleGausExp(const std::string& name){
	std::string ge_name = name + "_gausexp";
	getGausExp(ge_name);
	RooGausExp& gausExp = (RooGausExp&) *workspace_->pdf(ge_name.c_str());

	std::string gaus2_name = name + "_gaus2";
	getGaus(gaus2_name,"mean2","sigma2");
	RooGaussian& gaus2 = (RooGaussian&) *workspace_->pdf(gaus2_name.c_str());

	std::string gaus3_name = name + "_gaus3";
	getGaus(gaus3_name,"mean3","sigma3");
	RooGaussian& gaus3 = (RooGaussian&) *workspace_->pdf(gaus3_name.c_str());

	RooRealVar g1frac("g1frac","fraction of gauss1",0.3);
	RooRealVar g2frac("g2frac","fraction of gauss2",0.3);
	RooRealVar g3frac("g3frac","fraction of gauss3",0.3);
	RooAddPdf sum(name.c_str(),(name + "_triplegausexp").c_str(),RooArgList(gausExp,gaus2,gaus3),RooArgList(g1frac,g2frac,g3frac));
	workspace_->import(sum);

}

void ProbabilityDensityFunctions::getGausExpPSProd(const std::string& name){
	std::string ge_name = name + "_gausexp";
	getGausExp(ge_name);
	RooGausExp& gausExp = (RooGausExp&) *workspace_->pdf(ge_name.c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd gausExppsprod(name.c_str(),
	                          (name + "_gausExppsprod").c_str(), gausExp, ps);
	workspace_->import(gausExppsprod);
}

void ProbabilityDensityFunctions::getExpBWExp(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar peak("peak", "peak", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar width("width", "width", 35.0, 5.0, 150.0, "GeV");
	RooRealVar left("left", "left", 0.1, 15.0);
	RooRealVar right("right", "right", 0.1, 15.0);
	RooExpBWExp expBWExp(name.c_str(),
	                       (name + "_expbwexp").c_str(),
	                       var, peak, width, left, right);
	workspace_->import(expBWExp);
}

void ProbabilityDensityFunctions::getBukin(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar Xp("Xp", "Xp", getPeakStart(), 50.0, 350.0, "GeV");
	RooRealVar sigp("sigp", "sigp", 20.0, 85.0, "GeV");
	RooRealVar xi("xi", "xi", 0.0, 0.55);
	RooRealVar rho1("rho1", "rho1", 0.05, -0.1, 0.1);
	RooRealVar rho2("rho2", "rho2", -0.05, -0.07, 0.045);
	RooBukinPdf bukin(name.c_str(),
	                    (name + "_bukin").c_str(),
	                    var, Xp, sigp, xi, rho1, rho2);
	workspace_->import(bukin);
}

void ProbabilityDensityFunctions::getBukinPSProd(const std::string& name){
	std::string bukin_name = name + "_bukin";
	getBukin(bukin_name);
	RooBukinPdf& bukin = (RooBukinPdf&) *workspace_->pdf(bukin_name.c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd bukinpsprod(name.c_str(),
			(name + "_bukinpsprod").c_str(), bukin, ps);
	workspace_->import(bukinpsprod);
}

void ProbabilityDensityFunctions::getBernstein(const std::string& name,const int& numCoeffs){
	RooRealVar& var = *workspace_->var(var_.c_str());
	std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "bernstein"));
	RooBernstein bern(name.c_str(),
			(name+"_bernstein").c_str(), var, *coeffs);
	workspace_->import(bern);
}

void ProbabilityDensityFunctions::getChebychev(const std::string& name, const int& numCoeffs){
	if (numCoeffs > 7) {
		throw std::invalid_argument
		("Chebychev polynomials support not more than 7 coefficients.");
	}
	RooRealVar& var = *workspace_->var(var_.c_str());
	std::unique_ptr<RooArgList> coeffs(getCoefficients_(numCoeffs, "chebychev"));
	RooChebychev cheb(name.c_str(),
			(name + "_chebychev").c_str(), var, *coeffs);
	std::cout<<"WTF"<<std::endl;
	workspace_->import(cheb);
}

void ProbabilityDensityFunctions::getChebEffProd(const std::string& name, const int& numCoeffs){
	std::string cheb_name = name + "_cheb";
	getChebychev(cheb_name,numCoeffs);
	RooChebychev& cheb = (RooChebychev&) *workspace_->pdf(cheb_name.c_str());

	std::string eff_name = name + "_eff";
	getEfficiency(eff_name);
	RooFormulaVar& eff = (RooFormulaVar&) *workspace_->function(eff_name.c_str());

	RooEffProd chebEffProd(name.c_str(),
			(name+"_chebeffprod").c_str(), cheb, eff);
	workspace_->import(chebEffProd);
}

void ProbabilityDensityFunctions::getBernEffProd(const std::string& name, const int& numCoeffs){
	std::string bern_name = name + "_bern";
	getBernstein(bern_name,numCoeffs);
	RooBernstein& bern = (RooBernstein&) *workspace_->pdf(bern_name.c_str());

	std::string eff_name = name + "_eff";
	getEfficiency(eff_name);
	RooFormulaVar& eff = (RooFormulaVar&) *workspace_->function(eff_name.c_str());

	RooEffProd bernEffProd(name.c_str(),
			(name + "_berneffprod").c_str(), bern, eff);
	workspace_->import(bernEffProd);
}

void ProbabilityDensityFunctions::getBernPSProd(const std::string& name,const int& numCoeffs){
	std::string bern_name = name + "_bern";
	getBernstein(bern_name,numCoeffs);
	RooBernstein& bern = (RooBernstein&) *workspace_->pdf(bern_name.c_str());

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd bernPSProd(name.c_str(),
			(name+"_bernPSProd").c_str(), bern, ps);
	workspace_->import(bernPSProd);
}

void ProbabilityDensityFunctions::getBreitWigner(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean("mean","mean",getPeakStart(),200.,1500.,"GeV");
	RooRealVar width("width","width",35.,5.0,300.,"GeV");
	RooBreitWigner bw(name.c_str(),(name + "_breitwigner").c_str(),var,mean,width);
	workspace_->import(bw);
}

void ProbabilityDensityFunctions::getMyNovosibirsk(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar peak("peak", "peak", getPeakStart(), 50.0, 500.0, "GeV");
	RooRealVar width("width", "width", 50.0, 5.0, var.getMax()/2.0, "GeV");
	RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
	RooMyNovosibirsk novo(name.c_str(),(name + "_mynovosibirsk").c_str(),var, peak, width, tail);
	workspace_->import(novo);
}

void ProbabilityDensityFunctions::getMyNovoPSProd(const std::string& name){
	std::string novo_name = name + "_novosibirsk";
	getMyNovosibirsk(novo_name);
	RooMyNovosibirsk& novo = (RooMyNovosibirsk&) *workspace_->pdf(novo_name.c_str());	

	std::string ps_name = name + "_ps";
	getPhaseSpace(ps_name);
	RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

	RooEffProd novopsprod(name.c_str(),(name + "_mynovopsprod").c_str(),novo,ps);
	workspace_->import(novopsprod);
}

void ProbabilityDensityFunctions::getExtNovosibirsk(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar peak("peak", "peak", getPeakStart(), 50.0, 500.0, "GeV");
        RooRealVar width("width", "width", 50.0, 5.0, var.getMax()/2.0, "GeV");
        RooRealVar tail("tail", "tail", -0.1, -1.0, 1.0);
	RooRealVar par4("par4", "par4", -0.0001, -1.0, 1.0);
	RooExtendNovosibirsk novo(name.c_str(),(name + "_extnovosibirsk").c_str(),var,peak, width, tail, par4);
	workspace_->import(novo);
}

void ProbabilityDensityFunctions::getExtNovoPSProd(const std::string& name){
        std::string novo_name = name + "_extnovosibirsk";
        getExtNovosibirsk(novo_name);
        RooExtendNovosibirsk& novo = (RooExtendNovosibirsk&) *workspace_->pdf(novo_name.c_str());

        std::string ps_name = name + "_ps";
        getPhaseSpace(ps_name);
        RooFormulaVar& ps = (RooFormulaVar&) *workspace_->function(ps_name.c_str());

        RooEffProd novopsprod(name.c_str(),(name + "_extnovopsprod").c_str(),novo,ps);
        workspace_->import(novopsprod);
}

void ProbabilityDensityFunctions::getExtNovoEffProd(const std::string& name){
        std::string novo_name = name + "_extnovosibirsk";
        getExtNovosibirsk(novo_name);
        RooExtendNovosibirsk& novo = (RooExtendNovosibirsk&) *workspace_->pdf(novo_name.c_str());

        std::string eff_name = name + "_eff";
        getEfficiency(eff_name);
        RooFormulaVar& eff = (RooFormulaVar&) *workspace_->function(eff_name.c_str());	

	RooEffProd novoEffProd(name.c_str(),(name + "_extnovoeffprod").c_str(),novo,eff);
	workspace_->import(novoEffProd);
}

void ProbabilityDensityFunctions::getRelBreitWigner(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean("mean","mean",getPeakStart(),200.,1500.,"GeV");
	RooRealVar width("width","width",120,5.0,300.,"GeV");
//	RooRealVar sigma("sigma","sigma",0.1,5.0,300.,"GeV");
	RooRelBreitWigner bw(name.c_str(),(name + "_relbreitwigner").c_str(),var,mean,width);
	workspace_->import(bw);
}

void ProbabilityDensityFunctions::getRooQuadGausExp(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
//	RooRealVar mean("mean","mean",getPeakStart(),200,1500,"GeV");
//	RooRealVar sigmaL1("sigmaL1", "sigmaL1", 90.0, 20., 800.0, "GeV");
//	RooRealVar sigmaL2("sigmaL2", "sigmaL2", 225.0, 20., 800.0, "GeV");
//	RooRealVar sigmaR1("sigmaR1", "sigmaR1", 409.0, 10., 800.0, "GeV");
//	RooRealVar sigmaR2("sigmaR2", "sigmaR2", 60.0, 20., 800.0, "GeV");
//	RooRealVar tail_shift("tail_shift", "tail_shift", 1.2*getPeakStart(), 200.0, 1500.0, "GeV");
//	RooRealVar tail_sigma("tail_sigma", "tail_sigma", 157.0, 0.5, 900.0, "GeV");
//	RooRealVar norm_g1("norm_g1", "norm_g1", 0.5, 0, 1);
//	RooRealVar norm_g2("norm_g2", "norm_g2", 0.01, 0, 1);
	RooRealVar mean("mean","mean",getPeakStart(),200,1500,"GeV");
	RooRealVar sigmaL1("sigmaL1", "sigmaL1", 0.1*getPeakStart(), 20., 800.0, "GeV");
	RooRealVar sigmaL2("sigmaL2", "sigmaL2", 0.2*getPeakStart(), 20., 800.0, "GeV");
	RooRealVar sigmaR1("sigmaR1", "sigmaR1", 0.1*getPeakStart(), 10., 800.0, "GeV");
	RooRealVar sigmaR2("sigmaR2", "sigmaR2", 0.1*getPeakStart(), 20., 800.0, "GeV");
	RooRealVar tail_shift("tail_shift", "tail_shift", 1.2*getPeakStart(), 200.0, 1500.0, "GeV");
	RooRealVar tail_sigma("tail_sigma", "tail_sigma", 0.5*getPeakStart(), 0.5, 900.0, "GeV");
	RooRealVar norm_g1("norm_g1", "norm_g1", 0.5, 0, 1);
	RooRealVar norm_g2("norm_g2", "norm_g2", 0.5, 0, 1);
	RooQuadGausExp quadgexp(name.c_str(),(name + "_quadgexp").c_str(),var,mean,sigmaL1,sigmaL2,sigmaR1,sigmaR2,tail_shift,tail_sigma,norm_g1,norm_g2);
	workspace_->import(quadgexp);
}

void ProbabilityDensityFunctions::getPhaseSpace(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	std::string var_name = var.GetName();
	RooFormulaVar phasespace((name).c_str(),
				  ("(1.326 / (1 + (2.287e+03 * TMath::Exp(-3.331e-02 * " + var_name + ")))) - (1.326-1.)").c_str(),RooArgList(var));
	workspace_->import(phasespace);
}

void ProbabilityDensityFunctions::getEfficiency(const std::string& name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	std::string var_name = var.GetName();
	RooRealVar slope_novoeff("slope_novoeff", "slope_novoeff", 0.01, 0.0, 0.1);
	RooRealVar turnon_novoeff("turnon_novoeff", "turnon_novoeff",var.getMin()+ 5.0, var.getMin(), var.getMax());
	RooFormulaVar eff((name).c_str(),("0.5*(TMath::Erf(slope_novoeff*(" + var_name + "-turnon_novoeff)) + 1)").c_str(),
	                    RooArgSet(var, slope_novoeff, turnon_novoeff));
	workspace_->import(eff);
}

void ProbabilityDensityFunctions::getExp(const std::string& name,const std::string& name1){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar tau(paramName_(name1,"tau"), "tau", -0.1);
	tau.setConstant(false);
	RooExponential exp(name.c_str(),
	                     (name+"_exp").c_str(), var, tau);
	workspace_->import(exp);
}

void ProbabilityDensityFunctions::getGaus(const std::string& name,const std::string& mean_name, const std::string& sigma_name){
	RooRealVar& var = *workspace_->var(var_.c_str());
	RooRealVar mean(paramName_(mean_name,"mean"), "mean2", getPeakStart(), 100.0, 1500.0, "GeV");
	RooRealVar sigma(paramName_(sigma_name,"sigma"), "sigma2", 35.0, 5.0, 300.0, "GeV");
	RooGaussian gaus(name.c_str(),"gaussian PDF",var,mean,sigma);
	workspace_->import(gaus);
}

std::unique_ptr<RooArgList> ProbabilityDensityFunctions::getCoefficients_(const int numCoeffs, const std::string& name) {
  std::unique_ptr<RooArgList> coefficients
    (new RooArgList((name+"_coefficients").c_str()));
  for (int c = 0; c < numCoeffs; ++c) {
    std::string id(Form((name+"_coefficient_%02d").c_str(), c));
    std::unique_ptr<RooRealVar> coefficient
      (new RooRealVar(id.c_str(), id.c_str(), 0.0, 10.0));
    coefficients->addClone(*coefficient);
  }
  return coefficients;
}

const std::vector<std::string> ProbabilityDensityFunctions::availableModels_ =
  {"novosibirsk",
   "novopsprod",
   "novoeffprod",
   "novopshighMpol4",
   "crystalball",
   "crystalpsprod",
   "crystaleffprod",
   "cbeffprod",
   "expeffprod",
   "doublecb",
   "dijetv1",
   "dijetv1psprod",
   "dijetv2",
   "dijetv2psprod",
   "expgausexp",
   "gausexp",
   "gausexppsprod",
   "expbwexp",
   "bukin",
   "bukinpsprod",
   "bernstein",
   "chebychev",
   "berneffprod",
   "bernpsprod",
   "chebeffprod",
   "breitwigner",
   "mynovosibirsk",
   "mynovopsprod",
   "extnovosibirsk",
   "extnovopsprod",
   "extnovoeffprod",
   "relbreitwigner",
   "quadgausexp"};
