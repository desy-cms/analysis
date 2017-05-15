/*
 * signalStudy.C
 *
 *  Created on: Mar 29, 2016
 *      Author: shevchen
 */


#include <iostream>
#include <fstream> //std::ofstream
#include <ctime> // for the time checking
#include "math.h"
#include <string>
#include "TGraphErrors.h"
#include "map"

#include "Analysis/MssmHbb/macros/Drawer/MassPoints.cpp"
#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"

using namespace std;

void graphStyle(TGraph* gr);
MPoint* evaluateOptimisedCuts(MPoint *max,MPoint *comp );
TGraphErrors* cutOptimisation(TGraphErrors * gr, MPoint *max, MPoint *temp, const double *variable, const double & nbins, char * cut, const std::string &parameter);

std::string general_weights = "LumiWeight * TwoDPtWeight * dEtaWeight * BTagWeight ";//* BTagSFcentral[0] * BTagSFcentral[1] * BTagSFcentral[2] * WeightPileUp[0]";

int signalStudy(){

	//Setup style
	HbbStyle style;
	style.set(PRIVATE);
	TH1::SetDefaultSumw2();

	auto fLow500New 		= new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_New3dJetSelection_lowMTrigger_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8_14_04_2016.root");
	auto fLow500NewLead 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fBgLow     	= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_Pythia8_29_03_2016.root");
	auto fBgNew     	= new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_New3dJetSelection_lowMTrigger_Pythia8_14_04_2016.root");

	std::string cut  	= "TMath::Sqrt((LeadEta[2]-LeadEta[0])*(LeadEta[2]-LeadEta[0]) + fabs(fabs(fabs(LeadPhi[2]-LeadPhi[0])-TMath::Pi())-TMath::Pi())*fabs(fabs(fabs(LeadPhi[2]-LeadPhi[0])-TMath::Pi())-TMath::Pi())) > 1.1 && TMath::Sqrt((LeadEta[2]-LeadEta[1])*(LeadEta[2]-LeadEta[1]) + fabs(fabs(fabs(LeadPhi[2]-LeadPhi[1])-TMath::Pi())-TMath::Pi())*fabs(fabs(fabs(LeadPhi[2]-LeadPhi[1])-TMath::Pi())-TMath::Pi())) > 1.1";

	std::map<const int,MPoint *> pointNew;
	pointNew[501]  = new MPoint(500,"New3dJet_pt>40",fLow500New,fBgNew);
	pointNew[500]  = new MPoint(500,cut,fLow500NewLead,fBgLow);
	/*

	//Low M files
	auto fLow100 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow120 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow160 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow200 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow250 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow300 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow400 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow500 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow600 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow700 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow900 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow1100 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fLow1300 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");

	//High M files
	auto fhigh100 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-100_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh120 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-120_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh160 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-160_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh200 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh250 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-250_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh300 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh400 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh500 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh600 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh700 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh900 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh1100		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");
	auto fhigh1300 		= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8_29_03_2016.root");

	auto fBgLow     	= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_lowMTrigger_Pythia8_29_03_2016.root");
	auto fBgHigh     	= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_76X_highMTrigger_Pythia8_29_03_2016.root");


	///***************************74X*************************
	TFile * fSignal_300 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M300_output_104_02_2016.root");
	TFile * fSignal_600 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M600_output_104_02_2016.root");
	TFile * fSignal_1100 = new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_M1100_output_104_02_2016.root");
	TFile * fBg			= new TFile("/nfs/dust/cms/user/shevchen/output/TripleBTagSelection_QCD_08_02_2016.root");
	///*******************************************************

	std::map<const int,MPoint *> pointLowM;
	pointLowM[100]  = new MPoint(100,general_weights,fLow100,fBgLow);
	pointLowM[120]  = new MPoint(120,general_weights,fLow120,fBgLow);
	pointLowM[160]  = new MPoint(160,general_weights,fLow160,fBgLow);
	pointLowM[200]  = new MPoint(200,general_weights,fLow200,fBgLow);
	pointLowM[250]  = new MPoint(250,general_weights,fLow250,fBgLow);
	pointLowM[300]  = new MPoint(300,general_weights,fLow300,fBgLow);
	pointLowM[400]  = new MPoint(400,general_weights,fLow400,fBgLow);
	pointLowM[500]  = new MPoint(500,general_weights,fLow500,fBgLow);
	pointLowM[600]  = new MPoint(600,general_weights,fLow600,fBgLow);
	pointLowM[700]  = new MPoint(700,general_weights,fLow700,fBgLow);
	pointLowM[900]  = new MPoint(900,general_weights,fLow900,fBgLow);
	pointLowM[1100] = new MPoint(1100,general_weights,fLow1100,fBgLow);
	pointLowM[1300] = new MPoint(1300,general_weights,fLow1300,fBgLow);

	std::map<const int,MPoint *> pointHighM;
	pointHighM[100]  = new MPoint(100,general_weights,fhigh100,fBgLow);
	pointHighM[120]  = new MPoint(120,general_weights,fhigh120,fBgLow);
	pointHighM[160]  = new MPoint(160,general_weights,fhigh160,fBgHigh);
	pointHighM[200]  = new MPoint(200,general_weights,fhigh200,fBgHigh);
	pointHighM[250]  = new MPoint(250,general_weights,fhigh250,fBgHigh);
	pointHighM[300]  = new MPoint(300,general_weights,fhigh300,fBgHigh);
	pointHighM[400]  = new MPoint(400,general_weights,fhigh400,fBgHigh);
	pointHighM[500]  = new MPoint(500,general_weights,fhigh500,fBgHigh);
	pointHighM[600]  = new MPoint(600,general_weights,fhigh600,fBgHigh);
	pointHighM[700]  = new MPoint(700,general_weights,fhigh700,fBgHigh);
	pointHighM[900]  = new MPoint(900,general_weights,fhigh900,fBgHigh);
	pointHighM[1100] = new MPoint(1100,general_weights,fhigh1100,fBgHigh);
	pointHighM[1300] = new MPoint(1300,general_weights,fhigh1300,fBgHigh);


	//Efficiency as a function of mass
	auto SignalEff = new TCanvas("SignalEff","Signal Efficiency Low M vs High M",1000,800);
	TGraphErrors *EffHighM = new TGraphErrors(pointHighM.size());
	EffHighM->SetTitle("Signal Efficiency Low M vs High M scenario, 40% M. window; M_{12} ,[GeV]; N_{sel. signal} / N_{tot. signal}");
	TGraphErrors *RelEffHighM = new TGraphErrors(pointHighM.size());
	RelEffHighM->SetTitle("Rel. Signal Efficiency (PANZI) Low M vs High M scenario, 40% M. window; M_{12} ,[GeV]; #epsilon");

	//Efficiency as a function of mass

	TGraphErrors *EffM = new TGraphErrors(pointLowM.size());
	EffM->SetTitle("Signal Efficiency Low M vs High M scenario, 40% M. window; M_{12} ,[GeV]; N_{sel. signal} / N_{tot. signal}");
	TGraphErrors *RelEffM = new TGraphErrors(pointLowM.size());
	RelEffM->SetTitle("Rel. Signal Efficiency (PANZI) Low M vs High M scenario, 40% M. window; M_{12} ,[GeV]; #epsilon");

	int i = 0;
	for(const auto & p : pointLowM){
		++i;
		EffM->SetPoint(i,p.second->getMassPoint(),p.second->getSignalEfficiency());
		EffM->SetPointError(i,0.,p.second->getSignalEfficiencyErr());
		RelEffM->SetPoint(i,p.second->getMassPoint(),p.second->getRelEfficiency());
		RelEffM->SetPointError(i,0.,p.second->getRelEfficiencyErr());

		EffHighM->SetPoint(i,p.second->getMassPoint(),p.second->getSignalEfficiency());
		EffHighM->SetPointError(i,0.,p.second->getSignalEfficiencyErr());
		RelEffHighM->SetPoint(i,p.second->getMassPoint(),p.second->getRelEfficiency());
		RelEffHighM->SetPointError(i,0.,p.second->getRelEfficiencyErr());
	}

	i = 0;

	for(const auto & p : pointHighM){
		++i;

		EffHighM->SetPoint(i,p.second->getMassPoint(),p.second->getSignalEfficiency());
		EffHighM->SetPointError(i,0.,p.second->getSignalEfficiencyErr());
		RelEffHighM->SetPoint(i,p.second->getMassPoint(),p.second->getRelEfficiency());
		RelEffHighM->SetPointError(i,0.,p.second->getRelEfficiencyErr());
	}

	graphStyle(EffM);
	graphStyle(RelEffM);


	graphStyle(EffHighM);
	graphStyle(RelEffHighM);

	EffM->SetMinimum(0);
	EffM->SetMaximum(EffHighM->GetMaximum()	);
	EffHighM->SetMarkerColor(2);
	RelEffHighM->SetMarkerColor(2);

	TLegend* legend;
	legend = (TLegend*) style.legend("top,right",3,0.3);
	legend->AddEntry(EffM,"Low M","p");
	legend->AddEntry(EffHighM,"High M","p");

	EffHighM->Draw("AP");
	EffM->Draw("P");
	legend->Draw();

	auto RelSignalEff = new TCanvas("RelSignalEff","Rel Efficiency Low vs High",1000,800);
	RelEffHighM->SetMaximum(1);
	RelEffHighM->Draw("AP");
	RelEffM->Draw("P");

	legend->Draw();

	auto temp_can = new TCanvas("temp_can","temp_can",1000,800);

	// Cut optimisation
	//Define cuts arrays:
	const double pt1[] = {100,105,110,115,120,125,130,135,140,145,150,155,160,165,170};
	const int Npt1 = sizeof(pt1)/sizeof(const double);	//Pt1-2
	const double pt3[] = {40,45,50,55,60,65,70,75};
	const int Npt3 = sizeof(pt3)/sizeof(const double);	//Pt3
	const double eta1[] = {2.,2.04,2.08,2.12,2.16,2.2};
	const int Neta1 = sizeof(eta1)/sizeof(const double);	//eta1-2-3
	const double btag1[] = {0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag1 = sizeof(btag1)/sizeof(const double);	//Btag3
//	const double btag3[Nbtag3] = {0.605,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76};
	const double btag3[] = {0.8,0.82,0.84,0.86,0.88,0.9,0.91,0.92,0.93,0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag3 = sizeof(btag3)/sizeof(const double);	//Btag3
	const double dEta[] = {1.6,1.58,1.56,1.54,1.52,1.5,1.48,1.46,1.44,1.42,1.4};
	const int NdEta = sizeof(dEta)/sizeof(const double);	//dEta12
	const double dR[] = {1.,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.,2.1,2.2,2.3,2.4,2.5,2.6,2.7,3,3.3,3.6,3.9,4.};
	const int NdR = sizeof(dR)/sizeof(const double);		//dR123

	//Define cuts arrays for high M:
	const double pt1h[] = {160,165,170,175,180,185,190,195,200,205,210,215,220,225,230};
	const int Npt1h = sizeof(pt1h)/sizeof(const double);	//Pt1-2
	const double pt3h[] = {40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160};
	const int Npt3h = sizeof(pt3h)/sizeof(const double);	//Pt3
	const double btag1h[] = {0.8,0.82,0.84,0.86,0.88,0.9,0.91,0.92,0.93,0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag1h = sizeof(btag1h)/sizeof(const double);	//Btag3
//	const double btag3[Nbtag3] = {0.605,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76};
	const double btag3h[] = {0.46,0.48,0.5,0.52,0.54,0.56,0.58,0.6,0.62,0.64,0.66,0.68,0.7,0.72,0.74,0.76,0.78,0.8,0.82,0.84,0.86,0.88,0.9,0.91,0.92,0.93,0.935,0.94,0.945,0.95,0.955,0.96,0.965};
	const int Nbtag3h = sizeof(btag3h)/sizeof(const double);	//Btag3

//	std::string cut;
	char char_cut[50];

	int massPoint = 300;
	const std::string parameter = "releff";
	MPoint *maxPoint  = new MPoint(massPoint,general_weights,fLow500,fBgLow);
	MPoint *maxPointHigh  = new MPoint(massPoint,general_weights,fhigh500,fBgHigh);
	MPoint *max74 = new MPoint(massPoint,general_weights,fSignal_600,fBg);
	MPoint *temp_point;

	//M-600
	//Pt
	auto CanlowMRelEffPtScan = new TCanvas("CanlowMRelEffPtScan","Signal Efficiency Low M",1200,800);
	CanlowMRelEffPtScan->Divide(3,2);
	CanlowMRelEffPtScan->cd(1);
	TGraphErrors *RelEffPt1ScanLowM = new TGraphErrors(Npt1);
	RelEffPt1ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; p^{1}_{T} ,[GeV]; #epsilon");
	RelEffPt1ScanLowM = cutOptimisation(RelEffPt1ScanLowM,maxPoint,temp_point,pt1,Npt1,"(LeadPt[0] > %f )*",parameter);
	CanlowMRelEffPtScan->cd(2);
	TGraphErrors *RelEffPt2ScanLowM = new TGraphErrors(Npt1);
	RelEffPt2ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; p^{2}_{T} ,[GeV]; #epsilon");
	RelEffPt2ScanLowM = cutOptimisation(RelEffPt2ScanLowM,maxPoint,temp_point,pt1,Npt1,"(LeadPt[1] > %f )*",parameter);
	CanlowMRelEffPtScan->cd(3);
	TGraphErrors *RelEffPt3ScanLowM = new TGraphErrors(Npt3);
	RelEffPt3ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; p^{3}_{T} ,[GeV]; #epsilon");
	RelEffPt3ScanLowM = cutOptimisation(RelEffPt3ScanLowM,maxPoint,temp_point,pt3,Npt3,"(LeadPt[2] > %f )*",parameter);

	//Eta
	CanlowMRelEffPtScan->cd(4);
	TGraphErrors *RelEffEta1ScanLowM = new TGraphErrors(Neta1);
	RelEffEta1ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; |#eta|_{1} ,[GeV]; #epsilon");
	RelEffEta1ScanLowM = cutOptimisation(RelEffEta1ScanLowM,maxPoint,temp_point,eta1,Neta1,"(abs(LeadEta[0]) < %f )*",parameter);
	CanlowMRelEffPtScan->cd(5);
	TGraphErrors *RelEffEta2ScanLowM = new TGraphErrors(Neta1);
	RelEffEta2ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; |#eta|_{2} ,[GeV]; #epsilon");
	RelEffEta2ScanLowM = cutOptimisation(RelEffEta2ScanLowM,maxPoint,temp_point,eta1,Neta1,"(abs(LeadEta[1]) < %f )*",parameter);
	CanlowMRelEffPtScan->cd(6);
	TGraphErrors *RelEffEta3ScanLowM = new TGraphErrors(Neta1);
	RelEffEta3ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; |#eta|_{3} ,[GeV]; #epsilon");
	RelEffEta3ScanLowM = cutOptimisation(RelEffEta3ScanLowM,maxPoint,temp_point,eta1,Neta1,"(abs(LeadEta[2]) < %f )*",parameter);

	//btag
	auto CanlowMRelEffBTagScan = new TCanvas("CanlowMRelEffBTagScan","Signal Efficiency Low M",1200,800);
	CanlowMRelEffBTagScan->Divide(3,1);
	CanlowMRelEffBTagScan->cd(1);
	TGraphErrors *RelEffBTag1ScanLowM = new TGraphErrors(Nbtag1);
	RelEffBTag1ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; BTagDiscr^{1}_{CSVv2T}; #epsilon");
	RelEffBTag1ScanLowM = cutOptimisation(RelEffBTag1ScanLowM,maxPoint,temp_point,btag1,Nbtag1,"(LeadBTag[0] > %f )*",parameter);
	CanlowMRelEffBTagScan->cd(2);
	TGraphErrors *RelEffBTag2ScanLowM = new TGraphErrors(Nbtag1);
	RelEffBTag2ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; BTagDiscr^{2}_{CSVv2T}; #epsilon");
	RelEffBTag2ScanLowM = cutOptimisation(RelEffBTag2ScanLowM,maxPoint,temp_point,btag1,Nbtag1,"(LeadBTag[1] > %f )*",parameter);
	CanlowMRelEffBTagScan->cd(3);
	TGraphErrors *RelEffBTag3ScanLowM = new TGraphErrors(Nbtag3);
	RelEffBTag3ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; BTagDiscr^{3}_{CSVv2T}; #epsilon");
	RelEffBTag3ScanLowM = cutOptimisation(RelEffBTag3ScanLowM,maxPoint,temp_point,btag3,Nbtag3,"(LeadBTag[2] > %f )*",parameter);

	//dR
	auto CanlowMRelEffdRScan = new TCanvas("CanlowMRelEffdRScan","Signal Efficiency Low M dR",1200,800);
	TGraphErrors *RelEffdRScanLowM = new TGraphErrors(NdR);
	RelEffdRScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; #Delta R; #epsilon");
	RelEffdRScanLowM = cutOptimisation(RelEffdRScanLowM,maxPoint,temp_point,dR,NdR,"( (sqrt( (LeadEta[0]-LeadEta[1])*(LeadEta[0]-LeadEta[1]) + (LeadPhi[0]-LeadPhi[1])*(LeadPhi[0]-LeadPhi[1]) )) > %f )*",parameter);

	/*
	//74X vs 76X btag3
	auto can74Xvs76X = new TCanvas("can74Xvs76X","Signal Efficiency Low M",1200,800);
	can74Xvs76X->Divide(3,3);
	can74Xvs76X->cd(1);
	TGraphErrors *RelEffBTag1ScanLowMSignal = new TGraphErrors(Nbtag3);
	RelEffBTag1ScanLowMSignal->SetTitle("SIGNAL 76X; BTagDiscr^{3}_{CSVv2T}; N_{signal}");
	RelEffBTag1ScanLowMSignal = cutOptimisation(RelEffBTag1ScanLowMSignal,maxPoint,temp_point,btag3,Nbtag3,"(LeadBTag[2] > %f )*","signal");
	can74Xvs76X->cd(2);
	TGraphErrors *RelEffBTag2ScanLowMBg = new TGraphErrors(Nbtag3);
	RelEffBTag2ScanLowMBg->SetTitle("BG 76X; BTagDiscr^{3}_{CSVv2T}; N_{BG}");
	RelEffBTag2ScanLowMBg = cutOptimisation(RelEffBTag2ScanLowMBg,maxPoint,temp_point,btag3,Nbtag3,"(LeadBTag[2] > %f )*","bg");
	can74Xvs76X->cd(3);
	TGraphErrors *RelEffBTag3ScanLowMRel = new TGraphErrors(Nbtag3);
	RelEffBTag3ScanLowMRel->SetTitle("REL EFF 76X; BTagDiscr^{3}_{CSVv2T}; #epsilon");
	RelEffBTag3ScanLowMRel = cutOptimisation(RelEffBTag3ScanLowMRel,maxPoint,temp_point,btag3,Nbtag3,"(LeadBTag[2] > %f )*",parameter);

	can74Xvs76X->cd(4);
	TGraphErrors *RelEffBTag1ScanLowMSignal74X = new TGraphErrors(Nbtag3);
	RelEffBTag1ScanLowMSignal74X->SetTitle("SIGNAL 74X; BTagDiscr^{3}_{CSVv2T}; N_{signal}");
	RelEffBTag1ScanLowMSignal74X = cutOptimisation(RelEffBTag1ScanLowMSignal74X,max74,temp_point,btag3,Nbtag3,"(LeadBTag[0] > 0.97 && LeadBTag[1] > 0.97 && LeadBTag[2] > %f )*","signal");
	can74Xvs76X->cd(5);
	TGraphErrors *RelEffBTag2ScanLowM74XBg = new TGraphErrors(Nbtag3);
	RelEffBTag2ScanLowM74XBg->SetTitle("BG 74X; BTagDiscr^{3}_{CSVv2T}; N_{BG}");
	RelEffBTag2ScanLowM74XBg = cutOptimisation(RelEffBTag2ScanLowM74XBg,max74,temp_point,btag3,Nbtag3,"(LeadBTag[0] > 0.97 && LeadBTag[1] > 0.97 && LeadBTag[2] > %f )*","bg");
	can74Xvs76X->cd(6);
	TGraphErrors *RelEffBTag3ScanLowM74XRel = new TGraphErrors(Nbtag3);
	RelEffBTag3ScanLowM74XRel->SetTitle("REL EFF 74X; BTagDiscr^{3}_{CSVv2T}; #epsilon");
	RelEffBTag3ScanLowM74XRel = cutOptimisation(RelEffBTag3ScanLowM74XRel,max74,temp_point,btag3,Nbtag3,"(LeadBTag[0] > 0.97 && LeadBTag[1] > 0.97 && LeadBTag[2] > %f )*",parameter);

	can74Xvs76X->cd(7);
	double NomX[Nbtag3+1], NomY[Nbtag3+1], DenX[Nbtag3+1], DenY[Nbtag3+1];
	TGraphErrors *ratioSignal = new TGraphErrors(Nbtag3);
	ratioSignal->SetTitle("SIGNAL 76X/74X; BTagDiscr^{3}_{CSVv2T}; N_{signal}");
	TGraphErrors *ratioBg = new TGraphErrors(Nbtag3);
	ratioBg->SetTitle("BG 76X/74X; BTagDiscr^{3}_{CSVv2T}; N_{BG}");
	TGraphErrors *ratioRel = new TGraphErrors(Nbtag3);
	ratioRel->SetTitle("REL EFF 76X/74X; BTagDiscr^{3}_{CSVv2T}; #epsilon");
	for(int i = 0; i < Nbtag3; ++i){
		RelEffBTag1ScanLowMSignal->GetPoint(i,NomX[i],NomY[i]);
		RelEffBTag1ScanLowMSignal74X->GetPoint(i,DenX[i],DenY[i]);
		ratioSignal->SetPoint(i,NomX[i],NomY[i]/DenY[i]);

		RelEffBTag2ScanLowMBg->GetPoint(i,NomX[i],NomY[i]);
		RelEffBTag2ScanLowM74XBg->GetPoint(i,DenX[i],DenY[i]);
		ratioBg->SetPoint(i,NomX[i],NomY[i]/DenY[i]);

		RelEffBTag3ScanLowMRel->GetPoint(i,NomX[i],NomY[i]);
		RelEffBTag3ScanLowM74XRel->GetPoint(i,DenX[i],DenY[i]);
		ratioRel->SetPoint(i,NomX[i],NomY[i]/DenY[i]);
	}
	graphStyle(ratioSignal);
	ratioSignal->Draw("AP");

	can74Xvs76X->cd(8);
	graphStyle(ratioBg);
	ratioBg->Draw("AP");
	can74Xvs76X->cd(9);
	graphStyle(ratioRel);
	ratioRel->Draw("AP");

	//HIGH MASS
	//M-600
	//Pt
	auto CanHighMRelEffPtScan = new TCanvas("CanHighMRelEffPtScan","Signal Efficiency High M",1200,800);
	CanHighMRelEffPtScan->Divide(3,2);
	CanHighMRelEffPtScan->cd(1);
	TGraphErrors *RelEffPt1ScanHighM = new TGraphErrors(Npt1h);
	RelEffPt1ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; p^{1}_{T} ,[GeV]; #epsilon");
	RelEffPt1ScanHighM = cutOptimisation(RelEffPt1ScanHighM,maxPointHigh,temp_point,pt1h,Npt1h,"(LeadPt[0] > %f )*",parameter);
	CanHighMRelEffPtScan->cd(2);
	TGraphErrors *RelEffPt2ScanHighM = new TGraphErrors(Npt1h);
	RelEffPt2ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; p^{2}_{T} ,[GeV]; #epsilon");
	RelEffPt2ScanHighM = cutOptimisation(RelEffPt2ScanHighM,maxPointHigh,temp_point,pt1h,Npt1h,"(LeadPt[1] > %f )*",parameter);
	CanHighMRelEffPtScan->cd(3);
	TGraphErrors *RelEffPt3ScanHighM = new TGraphErrors(Npt3h);
	RelEffPt3ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; p^{3}_{T} ,[GeV]; #epsilon");
	RelEffPt3ScanHighM = cutOptimisation(RelEffPt3ScanHighM,maxPointHigh,temp_point,pt3h,Npt3h,"(LeadPt[2] > %f )*",parameter);

	//Eta
	CanHighMRelEffPtScan->cd(4);
	TGraphErrors *RelEffEta1ScanHighM = new TGraphErrors(Neta1);
	RelEffEta1ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; |#eta|_{1} ,[GeV]; #epsilon");
	RelEffEta1ScanHighM = cutOptimisation(RelEffEta1ScanHighM,maxPointHigh,temp_point,eta1,Neta1,"(abs(LeadEta[0]) < %f )*",parameter);
	CanHighMRelEffPtScan->cd(5);
	TGraphErrors *RelEffEta2ScanHighM = new TGraphErrors(Neta1);
	RelEffEta2ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; |#eta|_{2} ,[GeV]; #epsilon");
	RelEffEta2ScanHighM = cutOptimisation(RelEffEta2ScanHighM,maxPointHigh,temp_point,eta1,Neta1,"(abs(LeadEta[1]) < %f )*",parameter);
	CanHighMRelEffPtScan->cd(6);
	TGraphErrors *RelEffEta3ScanHighM = new TGraphErrors(Neta1);
	RelEffEta3ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; |#eta|_{3} ,[GeV]; #epsilon");
	RelEffEta3ScanHighM = cutOptimisation(RelEffEta3ScanHighM,maxPointHigh,temp_point,eta1,Neta1,"(abs(LeadEta[2]) < %f )*",parameter);

	//btag
	auto CanHighMRelEffBTagScan = new TCanvas("CanHighMRelEffBTagScan","Signal Efficiency High M",1200,800);
	CanHighMRelEffBTagScan->Divide(3,1);
	CanHighMRelEffBTagScan->cd(1);
	TGraphErrors *RelEffBTag1ScanHighM = new TGraphErrors(Nbtag1h);
	RelEffBTag1ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; BTagDiscr^{1}_{CSVv2T}; #epsilon");
	RelEffBTag1ScanHighM = cutOptimisation(RelEffBTag1ScanHighM,maxPointHigh,temp_point,btag1h,Nbtag1h,"(LeadBTag[0] > %f )*",parameter);
	CanHighMRelEffBTagScan->cd(2);
	TGraphErrors *RelEffBTag2ScanHighM = new TGraphErrors(Nbtag1h);
	RelEffBTag2ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; BTagDiscr^{2}_{CSVv2T}; #epsilon");
	RelEffBTag2ScanHighM = cutOptimisation(RelEffBTag2ScanHighM,maxPointHigh,temp_point,btag1h,Nbtag1h,"(LeadBTag[1] > %f )*",parameter);
	CanHighMRelEffBTagScan->cd(3);
	TGraphErrors *RelEffBTag3ScanHighM = new TGraphErrors(Nbtag3h);
	RelEffBTag3ScanHighM->SetTitle("Rel. Signal Efficiency High M scenario, 40% M. window; BTagDiscr^{3}_{CSVv2T}; #epsilon");
	RelEffBTag3ScanHighM = cutOptimisation(RelEffBTag3ScanHighM,maxPointHigh,temp_point,btag3h,Nbtag3h,"(LeadBTag[2] > %f )*",parameter);




	RelSignalEff->SaveAs("pictures/RelSignalEffvsM.pdf");
	SignalEff->SaveAs("pictures/SignalEffvsM.pdf");
	CanlowMRelEffPtScan->SaveAs("pictures/PtEtaLowMScan.pdf");
	CanHighMRelEffPtScan->SaveAs("pictures/PtEtaHighMScan.pdf");
	CanlowMRelEffBTagScan->SaveAs("pictures/BTagLowMScan.pdf");
	CanHighMRelEffBTagScan->SaveAs("pictures/BTagHighMScan.pdf");
	can74Xvs76X->SaveAs("pictures/74Xvs76X_BTag3.pdf");

	/*
//	MPoint *maxPt1Point300  = new MPoint(300,general_weights,fLow300,fBgLow);
//	MPoint *temp_point;
	for(int npt1 = 0; npt1< Npt1;++npt1){			// Loop over the first Pt bins

		sprintf(char_cut,"(LeadPt[1] > %f )*",pt1[npt1]);
		std::string opt_cuts(char_cut);
		opt_cuts += general_weights;
		temp_point = new MPoint(300,opt_cuts,fLow300,fBgLow);
		RelEffPt2ScanLowM->SetPoint(npt1,pt1[npt1],temp_point->getRelEfficiency());
		RelEffPt2ScanLowM->SetPointError(npt1,0,temp_point->getRelEfficiencyErr());

		maxPt1Point300 = evaluateOptimisedCuts(maxPt1Point300,temp_point);
	}
	maxPt1Point300->printDifference(*pointLowM[300]);
	graphStyle(RelEffPt2ScanLowM);
	RelEffPt2ScanLowM->Draw("AP");

	CanlowMRelEffPtScan->cd(3);
	TGraphErrors *RelEffPt3ScanLowM = new TGraphErrors(Npt3);
	RelEffPt3ScanLowM->SetTitle("Rel. Signal Efficiency Low M scenario, 40% M. window; p^{3}_{T} ,[GeV]; #epsilon");

	for(int npt1 = 0; npt1< Npt3;++npt1){			// Loop over the first Pt bins

		sprintf(char_cut,"(LeadPt[2] > %f )*",pt3[npt1]);
		std::string opt_cuts(char_cut);
		opt_cuts += general_weights;
		temp_point = new MPoint(300,opt_cuts,fLow300,fBgLow);
		RelEffPt3ScanLowM->SetPoint(npt1,pt3[npt1],temp_point->getRelEfficiency());
		RelEffPt3ScanLowM->SetPointError(npt1,0,temp_point->getRelEfficiencyErr());

		maxPt1Point300 = evaluateOptimisedCuts(maxPt1Point300,temp_point);
	}
	maxPt1Point300->printDifference(*pointLowM[300]);
	graphStyle(RelEffPt3ScanLowM);
	RelEffPt3ScanLowM->Draw("AP");

/*
  	for(int npt1 = 0; npt1< Npt1;++npt1){			// Loop over the first Pt bins

		sprintf(char_cut,"(LeadPt[0] > %f )*",pt1[npt1]);
		std::string opt_cuts(char_cut);
		opt_cuts += general_weights;
		temp_point = new MPoint(300,opt_cuts,fLow300,fBgLow);
		RelEffPt1ScanLowM->SetPoint(npt1,pt1[npt1],temp_point->getRelEfficiency());
		RelEffPt1ScanLowM->SetPointError(npt1,0,temp_point->getRelEfficiencyErr());

		maxPt1Point300 = evaluateOptimisedCuts(maxPt1Point300,temp_point);
	}
	maxPt1Point300->printDifference(*pointLowM[300]);
	graphStyle(RelEffPt1ScanLowM);
	RelEffPt1ScanLowM->Draw("AP");
 * */
	return 0;
}

MPoint* evaluateOptimisedCuts(MPoint *max,MPoint *comp ){

	if(comp->getRelEfficiency() > max->getRelEfficiency()) {
		comp->setOptimalCuts(comp->getCuts());
		return comp;
	}
	else {
		max->setOptimalCuts(max->getCuts());
		return max;
	}
}

TGraphErrors* cutOptimisation(TGraphErrors * gr, MPoint *max, MPoint *temp, const double *variable, const double & nbins, char * cut,const std::string & param){
	char char_cut[200];
	for(int n = 0; n< nbins;++n){			// Loop over the first Pt bins

		sprintf(char_cut,cut,variable[n]);
		std::string opt_cuts(char_cut);
		opt_cuts += general_weights;
		temp = new MPoint(max->getMassPoint(),opt_cuts,max->getSignalFile(),max->getBgFile());

		std::string params = param;
		std::transform(params.begin(),params.end(),params.begin(), ::tolower);
		if(params.find("releff") != std::string::npos){
			gr->SetPoint(n,variable[n],temp->getRelEfficiency());
			gr->SetPointError(n,0,temp->getRelEfficiencyErr());
		}
		else if(params.find("sigeff") != std::string::npos){
			gr->SetPoint(n,variable[n],temp->getSignalEfficiency());
			gr->SetPointError(n,0,temp->getSignalEfficiencyErr());
		}
		else if (params.find("signal") != std::string::npos){
			gr->SetPoint(n,variable[n],temp->getSignal());
			gr->SetPointError(n,0,temp->getSignalErr());
		}
		else if (params.find("bg") != std::string::npos){
			gr->SetPoint(n,variable[n],temp->getBg());
			gr->SetPointError(n,0,temp->getBgErr());
		}
		else {
			std::cerr<<" Error: wrong parameter for TGraph in cutOptimisation function were specified. Check the input string"<<std::endl;
			exit(1);
		}

		max = evaluateOptimisedCuts(max,temp);
	}
	graphStyle(gr);
	gr->Draw("AP");

	return gr;
}

void graphStyle(TGraph* gr){
	gr->GetXaxis()->SetRangeUser(gr->GetXaxis()->GetXmin()*0.9,gr->GetXaxis()->GetXmax()*1.1);
	gr->GetXaxis()->SetTitleOffset(1.1);
	gr->SetMarkerStyle(20);
	gr->SetMarkerColor(1);
	gr->SetMarkerSize(1.2);
}
