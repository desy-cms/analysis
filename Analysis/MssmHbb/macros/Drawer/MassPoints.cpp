/*
 * MassPoints.cpp
 *
 *  Created on: Mar 29, 2016
 *      Author: shevchen
 */

#include "TFile.h"

#include <iostream>
#include <fstream> //std::ofstream
#include <ctime> // for the time checking
#include "math.h"
#include <string>

using namespace std;

template <typename T>
class counter
{
public:

    counter()
    {
        objects_created++;
        objects_alive++;
    }

    virtual ~counter()
    {
        --objects_alive;
    }
    static int objects_created;
    static int objects_alive;
};
template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );



class MPoint : counter<MPoint> {
public:
//	MPoint(){};
	MPoint(const int &MassPoint,const std::string & weight, TFile *signal, TFile *bg) : MassPoint_(MassPoint), weight_(weight), fSignal_(signal), fBg_(bg), signal_(0), bg_(0), tot_(0), cut_(""), eff_(0), relEff_(0), xSection_(0) {

		signal->GetObject("MssmHbb",tree_);
		bg->GetObject("MssmHbb",bgTree_);

		TH1D *hM = new TH1D("hM","M",1,0,20);
		TH1D *hMTot = new TH1D("hMTot","",1,0,10e7);
		TH1D *hMLumi = new TH1D("hMLumi","",1,0,10e7);
		TH1D *hbg = new TH1D("hbg","M",1,0,20);

		char cut[100];
		sprintf(cut,"(ObjM12 > %f && ObjM12 < %f ) *",0.8 * MassPoint, 1.2 * MassPoint);
		std::string cuts(cut);
		cuts += weight_;

		tree_->Draw("Njets>>hM",cuts.c_str());
		tree_->Draw("TotalNumberOfEvents>>hMTot");
		tree_->Draw("LumiWeight>>hMLumi");
		bgTree_->Draw("Njets>>hbg",cuts.c_str());

		signal_ = hM->GetBinContent(1);
		signalErr_ = hM->GetBinError(1);

		bg_ = hbg->GetBinContent(1);
		bgErr_ = hbg->GetBinError(1);

		tot_ = hMTot->GetMean() * hMLumi->GetMean();
		totErr_ = std::sqrt(hMTot->GetMean()) * hMLumi->GetMean();

		eff_ =  signal_ / tot_;
		effErr_ = signal_ / tot_ * std::sqrt(signalErr_*signalErr_/(signal_*signal_) + totErr_*totErr_/(tot_*tot_));

		xSection_ = getXSection(MassPoint_);
		relEff_ = (xSection_ * Luminosity_ * eff_) / std::sqrt(bg_);
		relEffErr_ = xSection_ * Luminosity_* std::sqrt(1./bg_ * effErr_*effErr_ + eff_*eff_/(4. * bg_*bg_*bg_) * bgErr_* bgErr_);

		cut_ = cuts;

		getDescription();

		delete hM;
		delete hMTot;
		delete hMLumi;
		delete hbg;
	};


	~MPoint(){};

	MPoint(const MPoint & p){
		bgTree_ = p.bgTree_;
		tree_ = p.tree_;
		signal_ = p.signal_;
		bg_ = p.bg_;
		tot_ = p.tot_;
		cut_ = p.cut_;
		eff_ = p.eff_;
		xSection_ = p.xSection_;
		relEff_ = p.relEff_;
		MassPoint_ = p.MassPoint_;
		optimalCuts_ = p.optimalCuts_;
	}

	MPoint operator=(const MPoint &p){
		fSignal_ = p.fSignal_;
		fBg_ = p.fBg_;
		bgTree_ = p.bgTree_;
		tree_ = p.tree_;
		signal_ = p.signal_;
		signalErr_ = p.signalErr_;
		bg_ = p.bg_;
		bgErr_ = p.bgErr_;
		tot_ = p.tot_;
		totErr_ = p.totErr_;
		cut_ = p.cut_;
		eff_ = p.eff_;
		effErr_ = p.effErr_;
		xSection_ = p.xSection_;
		relEff_ = p.relEff_;
		relEffErr_ = p.relEffErr_;
		MassPoint_ = p.MassPoint_;
		optimalCuts_ = p.optimalCuts_;
		return *this;
	}

	void printDifference(const MPoint &initial);
	void getDescription();
	void calculateEfficiency();

	//Sets
	void setSignal(double const &signal);
	void setBg(double const &bg);
	void setTot(double const &tot);
	void setCuts(const std::string &cut);
	void setOptimalCuts(const std::string &cut);

	//Gets
	const double & getSignal();
	const double & getSignalErr();
	const double & getBg();
	const double & getBgErr();
	const double & getTot();

	const double & getRelEfficiency();
	const double & getRelEfficiencyErr();
	const double & getSignalEfficiency();
	const double & getSignalEfficiencyErr();
	const std::string & getCuts();
	const int & getMassPoint();
	const std::string & getOptimalCuts();
	size_t getSize() const;
	const TTree* getSignalTree() const;
	TFile* getSignalFile();
	TFile* getBgFile();

private:

	//For constructor
	TTree *tree_;
	TTree *bgTree_;
	double signal_;	// number of pure signal
	double signalErr_;
	double bg_;		// number of the background
	double bgErr_;
	double tot_; 	// total number of events in signal sample
	double totErr_;
	std::string cut_;
	int MassPoint_;
	TFile *fSignal_;
	TFile *fBg_;

	//Calculated variables
	std::string weight_;
	double eff_;
	double effErr_;
	double relEff_;
	double relEffErr_;
	double xSection_;
	std::string optimalCuts_;

protected:

	const double getXSection(const int & MassPoint);

	//Declare constants, pb
	// from Santander method
	const double Sigma100__ = 5.23;		//5.22917222713
	const double Sigma120__ = 41.79;	//41.7887314999
	const double Sigma160__ = 75.32;	//75.3185132566
	const double Sigma200__ = 35.42;	//35.4180244078
	const double Sigma250__ = 15.55;	//15.5527782822
	const double Sigma300__ = 7.64;		//7.63976194847
	const double Sigma400__ = 2.34;		//2.34044663823
	const double Sigma500__ = 0.88;		//0.877385402091
	const double Sigma600__ = 0.377;	//0.376766971183
	const double Sigma700__ = 0.18;		//0.17940535717
	const double Sigma900__ = 0.051;	//0.0507664661841
	const double Sigma1100__ = 0.018;	//0.0180548176636
	const double Sigma1300__ = 0.008;	//0.00802737010696
	const double Luminosity_ = 2318.278306;

};


const double MPoint::getXSection(const int & MassPoint){
	if(MassPoint == 100){ return Sigma100__; }
	else if (MassPoint == 120) {return Sigma120__;}
	else if (MassPoint == 160) {return Sigma160__;}
	else if (MassPoint == 200) {return Sigma200__;}
	else if (MassPoint == 250) {return Sigma250__;}
	else if (MassPoint == 300) {return Sigma300__;}
	else if (MassPoint == 400) {return Sigma400__;}
	else if (MassPoint == 500) {return Sigma500__;}
	else if (MassPoint == 600) {return Sigma600__;}
	else if (MassPoint == 700) {return Sigma700__;}
	else if (MassPoint == 900) {return Sigma900__;}
	else if (MassPoint == 1100){return Sigma1100__;}
	else if (MassPoint == 1300){return Sigma1300__;}
	else {
		std::cerr<<"Error: wrong Mass point were introduced! Interrupt!"<<std::endl;
		exit(0);
	}

}

void MPoint::printDifference(const MPoint &initial){
	cout<<"--------Optimisation for Mass point: "<<MassPoint_<<"--------"<<endl;
	std::cout<<"SIGNAL: "<<"initial = "<<initial.signal_<<" optim. = "<<signal_<<" delta = "<<(signal_ - initial.signal_)* 100 / initial.signal_<<" %"<<std::endl;
	std::cout<<"BG    : "<<"initial = "<<initial.bg_<<" optim. = "<<bg_<<" delta = "<<(bg_ - initial.bg_)*100/initial.bg_<<" %"<<std::endl;
	std::cout<<"Eff.  : "<<"initial = "<<initial.relEff_<<" optim. = "<<relEff_<<" delta = "<<(relEff_ - initial.relEff_)*100/initial.relEff_<<" %"<<std::endl;
	cout<<"Optimal cuts:"<<endl;
	cout<<optimalCuts_<<endl;
}

void MPoint::getDescription(){
	cout<<"--------Mass point: "<<MassPoint_<<"--------"<<endl;
	cout<<"Nsig     = "<<signal_<<endl;
	cout<<"Nbg      = "<<bg_<<endl;
	cout<<"Ntot     = "<<tot_<<endl;
	cout<<"Eff      = "<<eff_<<endl;
	cout<<"Rel. Eff = "<<relEff_<<endl;
	cout<<"Optimal cuts:"<<endl;
	cout<<optimalCuts_<<endl;
}

void MPoint::calculateEfficiency(){
	eff_ =  signal_ / tot_;
	relEff_ = (xSection_ * Luminosity_ * eff_) / sqrt(bg_);
}

inline const double & MPoint::getSignal(){
	return signal_;
}

inline const double & MPoint::getSignalErr(){
	return signalErr_;
}

inline const double & MPoint::getBg(){
	return bg_;
}

inline const double & MPoint::getBgErr(){
	return bgErr_;
}

inline const double & MPoint::getTot(){
	return tot_;
}
inline const double & MPoint::getRelEfficiency(){
	return relEff_;
}

inline const double & MPoint::getRelEfficiencyErr(){
	return relEffErr_;
}

inline const double & MPoint::getSignalEfficiency(){
	return eff_;
}

inline const double & MPoint::getSignalEfficiencyErr(){
	return effErr_;
}

inline const std::string & MPoint::getCuts(){
	return cut_;
}
inline const int & MPoint::getMassPoint(){
	return MassPoint_;
}

inline const std::string & MPoint::getOptimalCuts(){
	return optimalCuts_;
}

inline size_t MPoint::getSize() const {
	return counter<MPoint>::objects_alive;
}

inline const TTree* MPoint::getSignalTree() const {
	return tree_;
}

inline TFile* MPoint::getSignalFile() {
	return fSignal_;
}

inline TFile* MPoint::getBgFile() {
	return fBg_;
}

// Sets

inline void MPoint::setSignal(double const &signal){
	signal_ = signal;
}
inline void MPoint::setBg(double const &bg){
	bg_ = bg;
}
inline void MPoint::setTot(double const &tot){
	tot_ = tot;
}
inline void MPoint::setCuts(const std::string &cut){
	cut_ = cut;
}
inline void MPoint::setOptimalCuts(const std::string &cut){
	optimalCuts_ = cut;
}
