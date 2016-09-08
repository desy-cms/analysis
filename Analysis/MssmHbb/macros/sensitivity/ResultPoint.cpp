
#ifndef RESULTPOINT_CPP
#define RESULTPOINT_CPP

#include "memory"
#include "Analysis/MssmHbb/macros/signal/mssm_xs_tools.C"
#include "Analysis/MssmHbb/macros/signal/8TeV/mssm_xs_tools_8TeV.C"
#include "Analysis/MssmHbb/macros/sensitivity/Measurement.cpp"
#include "string"
#include "TSystem.h"
#include "TStyle.h"

class ResultPoint{

public:
	ResultPoint();
	~ResultPoint();


	ResultPoint(const int & era, const int & mass, const double & Bg, const double & sig_selected, const double & sig_total, const double & ulPred, const double & ulData);
	ResultPoint(const int & era, const int & mass, const double & Bg, const double & sig_efficiency, const double & ulPred, const double & ulData);
	ResultPoint(const int & era, const int & mass, const Measurement & Bg, const Measurement & sig_selected, const Measurement & sig_total,
				const Measurement & ulPred, const Measurement & ulData);
	ResultPoint(const ResultPoint & p);

	const double 		XSec_Bg();					// return Bg XSection
	void				XSec_Bg( Measurement & );	// return Bg XSection Measurement

	const double &    	Sig_Eff();					// return Signal Efficiency
	void			 	Sig_Eff( Measurement & );	// return Signal Efficiency Measurement

	const double 		F_p();					// return f_p
	void				F_p( Measurement & );	// return f_p Measurement

	const double &		XSec_Signal();			// return signal XSection
	const double &		Sig_BR();				// return signal BR

	const double &		Sig_sel();				// return selected signal val;
	const double &		Sig_tot();				// return selected signal val;
	const double &		Bg();					// return selected Bg val;
	const double &		PunziSignificance();	// return Sig_eff / SQRT(BG)

	void Print();

protected:
	int mass_;
	Measurement bg_;
	Measurement sig_sel_;
	Measurement sig_eff_;
	Measurement sig_tot_;
	Measurement ulPred_;
	Measurement ulData_;
	double sig_xs_;
	double sig_br_;
	double lumi_    = -100;

	void calculate_sig_xs();
	void check_era(const int & era);

private:

	bool era_13TeV_ = false;
	bool era_8TeV_  = false;
	const double beta_ = 30;
};

ResultPoint::ResultPoint(const int & era, const int & mass, const double & Bg, const double & sig_selected, const double & sig_total, const double & ulPred, const double & ulData) :
		mass_(mass) {
	check_era(era);
//	bg_ = Measurement(Bg);
//	sig_sel_ = Measurement(sig_selected);
//	sig_tot_ = Measurement(sig_total);
//	ulPred_ = Measurement(ulPred);
//	ulData_ = Measurement(ulData);
//	sig_eff_ = Measurement(sig_sel_.val / sig_tot_.val);
	bg_.val = Bg;
	sig_sel_.val = sig_selected;
	sig_tot_.val = sig_total;
	ulPred_.val  = ulPred;
	ulData_.val  = ulData;
	sig_eff_.val = sig_sel_.val / sig_tot_.val;
	calculate_sig_xs();

}

ResultPoint::ResultPoint(const int & era, const int & mass, const double & Bg, const double & sig_efficiency, const double & ulPred, const double & ulData) :
		mass_(mass) {
	check_era(era);
//	bg_(Bg);
//	sig_eff_(sig_efficiency);
//	ulPred_(ulPred);
//	ulData_(ulData);
	bg_.val = Bg;
	sig_eff_.val = sig_efficiency;
	ulPred_.val  = ulPred;
	ulData_.val  = ulData;
	calculate_sig_xs();
}

ResultPoint::ResultPoint(const int & era, const int & mass, const Measurement & Bg, const Measurement & sig_selected, const Measurement & sig_total,
			const Measurement & ulPred, const Measurement & ulData) : mass_(mass){
	check_era(era);
	bg_ = Bg;
	sig_sel_ = sig_selected;
	sig_tot_ = sig_total;
	ulPred_ = ulPred;
	ulData_ = ulData;
	Sig_Eff(sig_eff_);
	calculate_sig_xs();
}

const double & ResultPoint::Sig_Eff(){
	return sig_eff_.val;
}

void ResultPoint::Sig_Eff(Measurement & mes){
	//if uncor.
	sig_eff_.val = sig_sel_.val / sig_tot_.val;
	sig_eff_.val_up = sig_eff_.val * std::sqrt(  sig_sel_.val_up *  sig_sel_.val_up /  (sig_sel_.val *  sig_sel_.val) + sig_tot_.val_up *  sig_tot_.val_up /  (sig_tot_.val *  sig_tot_.val));
	sig_eff_.val_down = sig_eff_.val * std::sqrt(  sig_sel_.val_down *  sig_sel_.val_down /  (sig_sel_.val *  sig_sel_.val) + sig_tot_.val_down *  sig_tot_.val_down /  (sig_tot_.val *  sig_tot_.val));
}

const double ResultPoint::F_p(){
	double f_p = ulData_.val / ulPred_.val;
	return f_p;
}

const double ResultPoint::XSec_Bg(){
	double xsec = bg_.val / (lumi_ * sig_eff_.val );
	return xsec;

}

const double & ResultPoint::XSec_Signal(){
	return sig_xs_;
}

const double & ResultPoint::Sig_BR(){
	return sig_br_;
}

const double & ResultPoint::Sig_sel(){
	return sig_sel_.val;
}

const double & ResultPoint::Sig_tot(){
	return sig_tot_.val;
}

const double & ResultPoint::Bg(){
	return bg_.val;
}

const double & ResultPoint::PunziSignificance(){
	return sig_eff_.val/std::sqrt(bg_.val);
}

void ResultPoint::calculate_sig_xs(){
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));
	std::string fullpath = cmsswBase + "/src/Analysis/MssmHbb/macros/signal/";

    double sigmaBBA;
    double sigmaBBH;

    double BrAbb;
    double BrHbb;

    double totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;

	if(era_13TeV_){
		fullpath += "mhmodp_mu200_13TeV.root";
		mssm_xs_tools tool(fullpath.c_str());
		sig_br_ = tool.br_Abb(mass_,beta_);

	    sigmaBBA = tool.bbHSantander_A(mass_,beta_);
	    sigmaBBH = tool.bbHSantander_H(mass_,beta_);

	    BrAbb = tool.br_Abb(mass_,beta_);
	    BrHbb = tool.br_Hbb(mass_,beta_);

	    totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
	    sig_xs_ = totXSec;
	}
	else if (era_8TeV_){
		//fullpath += "8TeV/mhmodp_mu-200_8TeV.root";
		fullpath += "8TeV/mhmodp_8TeV_tanbHigh.root";
		mssm_xs_tools_8tev tool;
		tool.SetInput(fullpath.c_str());
		sig_br_ = tool.Give_BR_A_bb(mass_,beta_);

	    sigmaBBA = tool.GiveXsec_Santander_A(mass_,beta_);
	    sigmaBBH = tool.GiveXsec_Santander_H(mass_,beta_);

	    BrAbb = tool.Give_BR_A_bb(mass_,beta_);
	    BrHbb = tool.Give_BR_H_bb(mass_,beta_);

	    totXSec = sigmaBBA*BrAbb + sigmaBBH*BrHbb;
	    sig_xs_ = totXSec;
	}
}

void ResultPoint::check_era(const int & era){
	if(era == 13) {
		era_13TeV_ = true;
		lumi_ = 2621;
	}
	else if (era == 8) {
		era_8TeV_ = true;
		lumi_ = 19700;
	}
	else exit(-1);
}

void ResultPoint::Print(){
	if(era_13TeV_) {
		std::cout<<" 13 TeV "<<mass_<<"    "<<bg_.val<<"        "<<sig_sel_.val<<"         "<<sig_tot_.val<<"          "<<sig_eff_.val<<"               "<<XSec_Bg()<<"            "<<sig_xs_<<"     "<<sig_br_<<"   "<<ulPred_.val<<"  "<<ulData_.val<<"   "<<F_p()<<std::endl;
	}
	else if (era_8TeV_) {
		std::cout<<" 8 TeV "<<mass_<<"    "<<bg_.val<<"        not defined="<<sig_sel_.val<<"   not defined="<<sig_tot_.val<<"    "<<sig_eff_.val<<"         "<<XSec_Bg()<<"            "<<sig_xs_<<"     "<<sig_br_<<"   "<<ulPred_.val<<"  "<<ulData_.val<<"   "<<F_p()<<std::endl;
	}

}

#endif
