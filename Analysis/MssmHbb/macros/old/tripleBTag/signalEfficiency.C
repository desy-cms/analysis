
#include <iostream>
#include <fstream> //std::ofstream
#include <ctime> // for the time checking
#include "math.h"
#include <string>

using namespace std;

const double Luminosity = 2182.680439; // pb-1;

//template <int MassPoint> struct Point;
void printfMinimisationStat(const char* cut, const double& maximum_eff, const double& initial_eff, const double &initial_sig, const double &sig, const double &initial_bg, const double& bg);
//Create structure that contain signal, backgorund and relative efficiency
struct Point{

public:
	Point() : MassPoint_(0), signal_(0), bg_(0), tot_(0), cut_(""), eff_(0), relEff_(0), xSection_(0) {};
	Point(const int &MassPoint, const double &signal, const double &bg, const double &tot, const std::string &cut = "") : MassPoint_(MassPoint), signal_(signal), bg_(bg), tot_(tot), cut_(cut)  {
		eff_ =  signal_ / tot_;
		if(MassPoint_ == 300){
			xSection_ = Sigma300__;
		}
		else if (MassPoint_ == 600){
			xSection_ = Sigma600__;
		}
		else if (MassPoint_ == 1100){
			xSection_ = Sigma1100__;
		}
		else {
			std::cerr<<"Error: wrong Mass point were introduced! Interrupt!"<<std::endl;
			exit(0);
		}
		relEff_ = (xSection_ * Luminosity * eff_) / sqrt(bg_);
	};

	Point(const Point & p){
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

	Point operator=(const Point &p){
		signal_ = p.signal_;
		bg_ = p.bg_;
		tot_ = p.tot_;
		cut_ = p.cut_;
		eff_ = p.eff_;
		xSection_ = p.xSection_;
		relEff_ = p.relEff_;
		MassPoint_ = p.MassPoint_;
		optimalCuts_ = p.optimalCuts_;
		return *this;
	}
	//Sets
	void setSignal(double const &signal){
		signal_ = signal;
	}
	void setBg(double const &bg){
		bg_ = bg;
	}
	void setTot(double const &tot){
		tot_ = tot;
	}
	void setCuts(const std::string &cut){
		cut_ = cut;
	}
	void setOptimalCuts(const std::string &cut){
		optimalCuts_ = cut;
	}

	//Gets
	const double & getSignal(){
		return signal_;
	}
	const double & getBg(){
		return bg_;
	}
	const double & getTot(){
		return tot_;
	}
	const double & getRelEfficiency(){
		return relEff_;
	}
	const double & getSignalEfficiency(){
		return eff_;
	}
	const std::string & getCuts(){
		return cut_;
	}
	const int & getMassPoint(){
		return MassPoint_;
	}

	const std::string & getOptimalCuts(){
		return optimalCuts_;
	}

	void printDifference(const Point &initial){
		cout<<"--------Optimisation for Mass point: "<<MassPoint_<<"--------"<<endl;
		std::cout<<"SIGNAL: "<<"initial = "<<initial.signal_<<" optim. = "<<signal_<<" delta = "<<(signal_ - initial.signal_)* 100 / initial.signal_<<" %"<<std::endl;
		std::cout<<"BG    : "<<"initial = "<<initial.bg_<<" optim. = "<<bg_<<" delta = "<<(bg_ - initial.bg_)*100/initial.bg_<<" %"<<std::endl;
		std::cout<<"Eff.  : "<<"initial = "<<initial.relEff_<<" optim. = "<<relEff_<<" delta = "<<(relEff_ - initial.relEff_)*100/initial.relEff_<<" %"<<std::endl;
		cout<<"Optimal cuts:"<<endl;
		cout<<optimalCuts_<<endl;
	}

	void getDescription(){
		cout<<"--------Mass point: "<<MassPoint_<<"--------"<<endl;
		cout<<"Nsig     = "<<signal_<<endl;
		cout<<"Nbg      = "<<bg_<<endl;
		cout<<"Ntot     = "<<tot_<<endl;
		cout<<"Eff      = "<<eff_<<endl;
		cout<<"Rel. Eff = "<<relEff_<<endl;
		cout<<"Optimal cuts:"<<endl;
		cout<<optimalCuts_<<endl;
	}

	void calculateEfficiency(){
		eff_ =  signal_ / tot_;
		relEff_ = (xSection_ * Luminosity * eff_) / sqrt(bg_);
	}

private:

	//For constructor
	double signal_;	// number of pure signal
	double bg_;		// number of the background
	double tot_; 	// total number of events in signal sample
	std::string cut_;
	int MassPoint_;

	//Calculated variables
	double eff_;
	double relEff_;
	double xSection_;
	std::string optimalCuts_;

protected:

	//Declare constants, pb
	// from Santander method
	const double Sigma300__ = 7.64;		//7.63976194847
	const double Sigma600__ = 0.377;	//0.376766971183
	const double Sigma1100__ = 0.018;	//0.0180548176636

};

Point* evaluateOptimisedCuts(Point *p,TTree *signalTree, TTree *bgTree, TH1F *hSignal, TH1F *hBg);
Point* evaluateOptimisedCuts(Point *p,TTree *signalTree, TTree *bgTree, TH1F *hSignal, TH1F *hBg, double *, double *, double *);

int signalEfficiency(){
	gROOT->Reset();
	TH1::SetDefaultSumw2();

	TFile * fSignal_300 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M300_output_104_02_2016.root");
	TFile * fSignal_600 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M600_output_104_02_2016.root");
	TFile * fSignal_1100 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M1100_output_104_02_2016.root");
	TFile * fBg			= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_QCD_08_02_2016.root");

	TTree *BgTree, *S300Tree, *S600Tree, *S1100Tree;
	fSignal_300->GetObject("MssmHbb",S300Tree);
	fSignal_600->GetObject("MssmHbb",S600Tree);
	fSignal_1100->GetObject("MssmHbb",S1100Tree);
	fBg -> GetObject("MssmHbb",BgTree);

	//...................M-300.....................

	TH1F *h300 = new TH1F("h300","M-300",1,0,20);
	TH1F *h300Tot = new TH1F("h300Tot","",1,0,10e7);
	TH1F *h300Lumi = new TH1F("h300Lumi","",1,0,10e7);
	TH1F *hBg300 = new TH1F("hBg300","",1,0,20);
	S300Tree->Draw("Njets>>h300","(ObjM12> 240 &&ObjM12 < 360)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S300Tree->Draw("TotalNumberOfEvents>>h300Tot");
	S300Tree->Draw("LumiWeight>>h300Lumi");
	BgTree->Draw("Njets>>hBg300","(ObjM12> 240 &&ObjM12 < 360)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");

	//Total number of the events for 300 GeV mass point
	const double NormalizedTot300 = h300Tot->GetMean() * h300Lumi->GetMean();
	Point *start300 = new Point(300,h300->GetMaximum(),hBg300->GetMaximum(),NormalizedTot300);
	start300->getDescription();

	//.................M-600......................

	TH1F *h600 = new TH1F("h600","M-300",1,0,20);
	TH1F *h600Tot = new TH1F("h600Tot","",1,0,10e7);
	TH1F *h600Lumi = new TH1F("h600Lumi","",1,0,10e7);
	TH1F *hBg600 = new TH1F("hBg600","",1,0,20);
	S600Tree->Draw("Njets>>h600","(ObjM12> 480 &&ObjM12 < 720)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	BgTree->Draw("Njets>>hBg600","(ObjM12> 480 &&ObjM12 < 720)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S600Tree->Draw("TotalNumberOfEvents>>h600Tot");
	S600Tree->Draw("LumiWeight>>h600Lumi");

	const double NormalizedTot600 = h600Tot->GetMean() * h600Lumi->GetMean();
	Point *start600 = new Point(600,h600->GetMaximum(),hBg600->GetMaximum(),NormalizedTot600);
	start600->getDescription();

	//................M-1100..................

	TH1F *h1100 = new TH1F("h1100","M-300",1,0,20);
	TH1F *h1100Tot = new TH1F("h1100Tot","",1,0,10e7);
	TH1F *h1100Lumi = new TH1F("h1100Lumi","",1,0,10e7);
	TH1F *hBg1100 = new TH1F("hBg1100","",1,0,20);
	S1100Tree->Draw("Njets>>h1100","(ObjM12> 880 &&ObjM12 < 1320)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	BgTree->Draw("Njets>>hBg1100","(ObjM12> 880 &&ObjM12 < 1320)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight");
	S1100Tree->Draw("TotalNumberOfEvents>>h1100Tot");
	S1100Tree->Draw("LumiWeight>>h1100Lumi");

	const double NormalizedTot1100 = h1100Tot->GetMean() * h1100Lumi->GetMean();
	Point *start1100 = new Point(1100,h1100->GetMaximum(),hBg1100->GetMaximum(),NormalizedTot1100);
	start1100->getDescription();

	// Cut optimisation
	//Define cuts arrays:
	const double pt1[] = {100,105,110,115,120,125,130,135,140,145,150,155,160,165,170};
	const int Npt1 = sizeof(pt1)/sizeof(const double);	//Pt1-2
	const double pt3[] = {30,35,40,45,50,55,60};
	const int Npt3 = sizeof(pt3)/sizeof(const double);	//Pt3
	const double eta1[] = {2.,2.04,2.08,2.12,2.16,2.2,2.24,2.28,2.32,2.36,2.4};
	const int Neta1 = sizeof(eta1)/sizeof(const double);	//eta1-2-3
	const double btag1[] = {0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag1 = sizeof(btag1)/sizeof(const double);	//Btag3
//	const double btag3[Nbtag3] = {0.605,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76};
	const double btag3[] = {0.8,0.82,0.84,0.86,0.88,0.9,0.91,0.92,0.93,0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag3 = sizeof(btag3)/sizeof(const double);	//Btag3
	const double dEta[] = {1.6,1.58,1.56,1.54,1.52,1.5,1.48,1.46,1.44,1.42,1.4};
	const int NdEta = sizeof(dEta)/sizeof(const double);	//dEta12
	const double dR[] = {0.9,0.92,0.94,0.96,0.98,1,1.02,1.04,1.06,1.08,1.1};
	const int NdR = sizeof(dR)/sizeof(const double);		//dR123

//	std::string cut;
	char char_cut[1000];
	char maxCuts[1000];
	double signal, max_signal, bg;

	//.................M-300.....................
	//................pt1,2,3....................
	Point *pt123Variation = new Point(*start300);
	Point *pt123VariationM600 = new Point(*start600);

	for(int npt1 = 0; npt1< Npt1;++npt1){			// Loop over the first Pt bins
		for(int npt2 = 0; npt2 < Npt1; ++npt2){		//Loop over the second Pt bins
			for(int npt3 = 0; npt3 < Npt3; ++npt3){	// Loop over the third Pt bins
				sprintf(char_cut,"(ObjM12> 240 &&ObjM12 < 360 && LeadPt[0] > %f && LeadPt[1] > %f && LeadPt[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
						pt1[npt1],pt1[npt2],pt3[npt3]); // Make cut char*
				std::string cut(char_cut);
				pt123Variation->setCuts(cut);
				pt123Variation = evaluateOptimisedCuts(pt123Variation,S300Tree,BgTree,h300,hBg300);

				//M600
				sprintf(char_cut,"(ObjM12> 480 &&ObjM12 < 720 && LeadPt[0] > 125 && LeadPt[1] > %f && LeadPt[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
						pt1[npt2],pt3[npt3]); // Make cut char*
				pt123VariationM600->setCuts(std::string(char_cut));
				pt123VariationM600 = evaluateOptimisedCuts(pt123VariationM600,S600Tree,BgTree,h600,hBg600);

			}
		}
	}
	pt123Variation->getDescription();
	pt123Variation->printDifference(*start300);
	pt123VariationM600->printDifference(*start600);

	//btag12
	Point *btag13Variation = new Point(*start300);
	Point *btag13VariationM600 = new Point(*start600);

	for(int nbtag1 = 0; nbtag1< Nbtag1;++nbtag1){			// Loop over the first btag bins
		for(int nbtag3 = 0; nbtag3 < Nbtag3; ++nbtag3){	// Loop over the third Pt bins
			sprintf(char_cut,"(ObjM12> 240 &&ObjM12 < 360 && LeadBTag[0] > %f && LeadBTag[1] > %f && LeadBTag[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
					btag1[nbtag1],btag1[nbtag1],btag3[nbtag3]);
			std::string cut(char_cut);
			btag13Variation->setCuts(cut);
			btag13Variation = evaluateOptimisedCuts(btag13Variation,S300Tree,BgTree,h300,hBg300);

			//M600
			sprintf(char_cut,"(ObjM12> 480 &&ObjM12 < 720 && LeadBTag[0] > %f && LeadBTag[1] > %f && LeadBTag[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
					btag1[nbtag1],btag1[nbtag1],btag3[nbtag3]);
			btag13VariationM600->setCuts(std::string(char_cut));
			btag13VariationM600 = evaluateOptimisedCuts(btag13VariationM600,S600Tree,BgTree,h600,hBg600);
		}
	}

	btag13Variation->printDifference(*start300);
	btag13VariationM600->printDifference(*start600);

	//--------------------SCAN-----------------
	//pt3
	Point *pt3Variation = new Point(*start300);

	double m300Pt3Signal[Npt3], m300Pt3Bg[Npt3], m300Pt3Eff[Npt3];
	for(int npt3 = 0; npt3 < Npt3; ++npt3){	// Loop over the third Pt bins
		sprintf(char_cut,"(ObjM12> 240 &&ObjM12 < 360 && LeadPt[0] > 115 && LeadPt[1] > 100 && LeadPt[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
				pt3[npt3]); // Make cut char*
		std::string cut(char_cut);
		pt3Variation->setCuts(cut);
		pt3Variation = evaluateOptimisedCuts(pt3Variation,S300Tree,BgTree,h300,hBg300,m300Pt3Signal+npt3,m300Pt3Bg+npt3,m300Pt3Eff+npt3);
	}
	TCanvas *pt3scanS = new TCanvas();
	TGraph *gr300Pt3Signal = new TGraph(Npt3,pt3,m300Pt3Signal);
	gr300Pt3Signal->SetTitle("Signal vs pt3;pt3;Signal");
	gr300Pt3Signal->SetMarkerStyle(20);
	gr300Pt3Signal->SetMarkerSize(1.1);
	gr300Pt3Signal->Draw("AP");

	TCanvas *pt3scanBg = new TCanvas();
	TGraph *gr300Pt3Bg = new TGraph(Npt3,pt3,m300Pt3Bg);
	gr300Pt3Bg->SetTitle("Bg vs pt3;pt3;Bg");
	gr300Pt3Bg->SetMarkerStyle(20);
	gr300Pt3Bg->SetMarkerSize(1.1);
	gr300Pt3Bg->Draw("AP");

	TCanvas *pt3scanE = new TCanvas();
	TGraph *gr300Pt3Eff = new TGraph(Npt3,pt3,m300Pt3Eff);
	gr300Pt3Eff->SetTitle("Relative Efficiency vs pt3;pt3;Relative Efficiency");
	gr300Pt3Eff->SetMarkerStyle(20);
	gr300Pt3Eff->SetMarkerSize(1.1);
	gr300Pt3Eff->Draw("AP");

	//btag3
	TCanvas *temp = new TCanvas();
	double m300Btag3Signal[Nbtag3], m300Btag3Bg[Nbtag3], m300Btag3Eff[Nbtag3];
	double m600Btag3Signal[Nbtag3], m600Btag3Bg[Nbtag3], m600Btag3Eff[Nbtag3];
	Point *btag3Variation = new Point(*start300);
	Point *btag3VariationM600 = new Point(*start600);

	for(int nbtag3 = 0; nbtag3 < Nbtag3; ++nbtag3){	// Loop over the third Pt bins
		sprintf(char_cut,"(ObjM12> 240 &&ObjM12 < 360 && LeadBTag[0] > 0.941 && LeadBTag[1] > 0.941 && LeadBTag[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
				btag3[nbtag3]);
		std::string cut(char_cut);
		btag3Variation->setCuts(cut);
		btag3Variation = evaluateOptimisedCuts(btag3Variation,S300Tree,BgTree,h300,hBg300,m300Btag3Signal+nbtag3,m300Btag3Bg+nbtag3,m300Btag3Eff+nbtag3);

		//M600
		sprintf(char_cut,"(ObjM12> 480 &&ObjM12 < 720 && LeadBTag[0] > 0.941 && LeadBTag[1] > 0.941 && LeadBTag[2] > %f)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
				btag3[nbtag3]);
		btag3VariationM600->setCuts(std::string(char_cut));
		btag3VariationM600 = evaluateOptimisedCuts(btag3VariationM600,S600Tree,BgTree,h600,hBg600,m600Btag3Signal+nbtag3,m600Btag3Bg+nbtag3,m600Btag3Eff+nbtag3);
	}

	TCanvas *btag3scanS = new TCanvas();
	btag3scanS->Divide(1,3);
	btag3scanS->cd(1);
	TGraph *gr300btag3Signal = new TGraph(Nbtag3,btag3,m300Btag3Signal);
	gr300btag3Signal->SetTitle("Signal vs btag3 (M300);btag3;Signal");
	gr300btag3Signal->SetMarkerStyle(20);
	gr300btag3Signal->SetMarkerSize(1.1);
	gr300btag3Signal->Draw("AP");
	btag3scanS->cd(2);
	TGraph *gr300btag3Bg = new TGraph(Nbtag3,btag3,m300Btag3Bg);
	gr300btag3Bg->SetTitle("Bg vs btag3 (M300);btag3;Bg");
	gr300btag3Bg->SetMarkerStyle(20);
	gr300btag3Bg->SetMarkerSize(1.1);
	gr300btag3Bg->Draw("AP");
	btag3scanS->cd(3);
	TGraph *gr300btag3Eff = new TGraph(Nbtag3,btag3,m300Btag3Eff);
	gr300btag3Eff->SetTitle("Relative Efficiency vs btag3 (M300);btag3;Relative Efficiency");
	gr300btag3Eff->SetMarkerStyle(20);
	gr300btag3Eff->SetMarkerSize(1.1);
	gr300btag3Eff->Draw("AP");

	//M600
	TCanvas *btag3scan600 = new TCanvas();
	btag3scan600->Divide(1,3);
	btag3scan600->cd(1);
	TGraph *gr600btag3Signal = new TGraph(Nbtag3,btag3,m600Btag3Signal);
	gr600btag3Signal->SetTitle("Signal vs btag3 (M600);btag3;Signal");
	gr600btag3Signal->SetMarkerStyle(20);
	gr600btag3Signal->SetMarkerSize(1.1);
	gr600btag3Signal->Draw("AP");
	btag3scan600->cd(2);
	TGraph *gr600btag3Bg = new TGraph(Nbtag3,btag3,m600Btag3Bg);
	gr600btag3Bg->SetTitle("Bg vs btag3 (M600);btag3;Bg");
	gr600btag3Bg->SetMarkerStyle(20);
	gr600btag3Bg->SetMarkerSize(1.1);
	gr600btag3Bg->Draw("AP");
	btag3scan600->cd(3);
	TGraph *gr600btag3Eff = new TGraph(Nbtag3,btag3,m600Btag3Eff);
	gr600btag3Eff->SetTitle("Relative Efficiency vs btag3 (M600);btag3;Relative Efficiency");
	gr600btag3Eff->SetMarkerStyle(20);
	gr600btag3Eff->SetMarkerSize(1.1);
	gr600btag3Eff->Draw("AP");

	//pt1
	const double pt1M600[] = {100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200};
	const int Npt1M600 = sizeof(pt1M600)/sizeof(double);
	Point *pt1Variation = new Point(*start300);
	Point *pt1VariationM600 = new Point(*start600);

	double m300Pt1Signal[Npt1M600], m300Pt1Bg[Npt1M600], m300Pt1Eff[Npt1M600];
	double m600Pt1Signal[Npt1M600], m600Pt1Bg[Npt1M600], m600Pt1Eff[Npt1M600];
	for(int npt1 = 0; npt1 < Npt1M600; ++npt1){	// Loop over the third Pt bins
		sprintf(char_cut,"(ObjM12> 240 &&ObjM12 < 360 && LeadPt[0] > %f && LeadPt[1] > 100 && LeadPt[2] > 30)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
				pt1M600[npt1]); // Make cut char*
		pt1Variation->setCuts(std::string(char_cut));
		pt1Variation = evaluateOptimisedCuts(pt1Variation,S300Tree,BgTree,h300,hBg300,m300Pt1Signal+npt1,m300Pt1Bg+npt1,m300Pt1Eff+npt1);

		//M600
		sprintf(char_cut,"(ObjM12> 480 &&ObjM12 < 720 && LeadPt[0] > %f && LeadPt[1] > 100 && LeadPt[2] > 30)*LumiWeight*TwoDPtWeight*dEtaWeight*BTagWeight",
				pt1M600[npt1]); // Make cut char*
		pt1VariationM600->setCuts(std::string(char_cut));
		pt1VariationM600 = evaluateOptimisedCuts(pt1VariationM600,S600Tree,BgTree,h600,hBg600,m600Pt1Signal+npt1,m600Pt1Bg+npt1,m600Pt1Eff+npt1);
		cout<<pt1[npt1]<<" "<<m600Pt1Eff[npt1]<<endl;
	}
	TCanvas *pt1scanS = new TCanvas();
	pt1scanS->Divide(1,3);
	pt1scanS->cd(1);
	TGraph *gr300pt1Signal = new TGraph(Npt1M600,pt1M600,m300Pt1Signal);
	gr300pt1Signal->SetTitle("Signal vs pt1;pt1;Signal");
	gr300pt1Signal->SetMarkerStyle(20);
	gr300pt1Signal->SetMarkerSize(1.1);
	gr300pt1Signal->Draw("AP");
	pt1scanS->cd(2);
	TGraph *gr300pt1Bg = new TGraph(Npt1M600,pt1M600,m300Pt1Bg);
	gr300pt1Bg->SetTitle("Bg vs pt1;pt1;Bg");
	gr300pt1Bg->SetMarkerStyle(20);
	gr300pt1Bg->SetMarkerSize(1.1);
	gr300pt1Bg->Draw("AP");
	pt1scanS->cd(3);
	TGraph *gr300pt1Eff = new TGraph(Npt1M600,pt1M600,m300Pt1Eff);
	gr300pt1Eff->SetTitle("Relative Efficiency vs pt1;pt1;Relative Efficiency");
	gr300pt1Eff->SetMarkerStyle(20);
	gr300pt1Eff->SetMarkerSize(1.1);
	gr300pt1Eff->Draw("AP");

	TCanvas *pt1scanSM600 = new TCanvas();
	pt1scanSM600->Divide(1,3);
	pt1scanSM600->cd(1);
	TGraph *gr600pt1Signal = new TGraph(Npt1M600,pt1M600,m600Pt1Signal);
	gr600pt1Signal->SetTitle("Signal vs pt1 (M600);pt1;Signal");
	gr600pt1Signal->SetMarkerStyle(20);
	gr600pt1Signal->SetMarkerSize(1.1);
	gr600pt1Signal->Draw("AP");
	pt1scanSM600->cd(2);
	TGraph *gr600pt1Bg = new TGraph(Npt1M600,pt1M600,m600Pt1Bg);
	gr600pt1Bg->SetTitle("Bg vs pt1 (M600);pt1;Bg");
	gr600pt1Bg->SetMarkerStyle(20);
	gr600pt1Bg->SetMarkerSize(1.1);
	gr600pt1Bg->Draw("AP");
	pt1scanSM600->cd(3);
	TGraph *gr600pt1Eff = new TGraph(Npt1M600,pt1M600,m600Pt1Eff);
	gr600pt1Eff->SetTitle("Relative Efficiency vs pt1 (M600);pt1;Relative Efficiency");
	gr600pt1Eff->SetMarkerStyle(20);
	gr600pt1Eff->SetMarkerSize(1.1);
	gr600pt1Eff->Draw("AP");


	return 0;
}

void printfMinimisationStat(const char* cut, const double& maximum_eff, const double& initial_eff, const double &initial_sig, const double &sig, const double & initial_bg, const double& bg){
	std::cout<<"\n Optimization done"<<std::endl;
	std::cout<<"Optimized cuts: \n"<<cut<<std::endl;
	std::cout<<"Opt. Eff = "<<maximum_eff<<" ,init. Eff = "<<initial_eff<<" ,100%*(Opt - Init)/Init = "<<(maximum_eff-initial_eff)*100/initial_eff<<std::endl;
	std::cout<<"Opt. sig. = "<<sig<<" ,init. sig. = "<<initial_sig<<" ,100%*(Opt - Init)/Init = "<<(sig - initial_sig)*100/initial_sig<<std::endl;
	std::cout<<"Opt. bg. = "<<bg<<" ,init. bg. = "<<initial_bg<<" ,100%*(Opt - Init)/Init = "<<(bg - initial_bg)*100/initial_bg<<std::endl;
	if(((maximum_eff-initial_eff)*100/initial_eff + (sig - initial_sig)*100/initial_sig) > 0) std::cout<<"Could be applied"<<std::endl;
	else
	{
		std::cout<<"Big loose of signal in comparison of won efficiency"<<std::endl;
	}
	std::cout<<"\n "<<std::endl;
}

Point* evaluateOptimisedCuts(Point *p,TTree *signalTree, TTree *bgTree, TH1F *hSignal, TH1F *hBg){

	std::string drawer(hSignal->GetName());
	drawer = "Njets>>" + drawer;
	signalTree->Draw(drawer.c_str(),p->getCuts().c_str());
	drawer = "Njets>>" + (string)(hBg->GetName());
	bgTree->Draw(drawer.c_str(),p->getCuts().c_str());
	Point *result = new Point(p->getMassPoint(),hSignal->GetMaximum(),hBg->GetMaximum(),p->getTot(),p->getCuts());

	double signal = hSignal->GetMaximum();
	if(result->getRelEfficiency() > p->getRelEfficiency()) {
		p = result;
		p->setOptimalCuts(result->getCuts());
	}
	return p;
}

Point* evaluateOptimisedCuts(Point *p,TTree *signalTree, TTree *bgTree, TH1F *hSignal, TH1F *hBg, double *Nsignal, double *Nbg, double *NEff){

	std::string drawer(hSignal->GetName());
	drawer = "Njets>>" + drawer;
	signalTree->Draw(drawer.c_str(),p->getCuts().c_str());
	drawer = "Njets>>" + (string)(hBg->GetName());
	bgTree->Draw(drawer.c_str(),p->getCuts().c_str());
	Point *result = new Point(p->getMassPoint(),hSignal->GetMaximum(),hBg->GetMaximum(),p->getTot(),p->getCuts());
	*Nsignal = result->getSignal();
	*Nbg = result->getBg();
	*NEff = result->getRelEfficiency();

	double signal = hSignal->GetMaximum();
	if(result->getRelEfficiency() > p->getRelEfficiency()) {
		p = result;
		p->setOptimalCuts(result->getCuts());
	}
	return p;
}

//void calculate


