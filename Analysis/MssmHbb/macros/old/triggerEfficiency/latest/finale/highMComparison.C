#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"


void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);

int highMComparison(){

	TH1::SetDefaultSumw2();

	TFile *fData = new TFile("/nfs/dust/cms/user/shevchen/output/EfficiencyStudy_76X_13_03_2016.root");
	TFile *f74XData = new TFile("/nfs/dust/cms/user/shevchen/output/EfficiencyStudy_29_02_2016.root");

	TTree *data, *data74X;
	fData->GetObject("MssmHbb",data);
	f74XData->GetObject("MssmHbb",data74X);

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup Ratio
   RatioPlots ratio(PRIVATE);
   ratio.SetRatioRange(0.5,1.5);
   ratio.SetRatioTitle("Data/Fit");

   //........................LOW M..................................//
	TCanvas *oneDFitLowM = new TCanvas("oneDFitLowM","Factor Low M",1000,800);
	TH1F *pf100Eff = new TH1F("pf100Eff",";p_{T}, [GeV]; #epsilon",100,70.,500.);
	pf100Eff->SetMarkerStyle(20);
	TH1F *pf100EffDenum = new TH1F("pf100EffDenum","",100,70.,500.);
	data->Draw("LeadPt[0]>>pf100Eff","LeadMatch100[0] == 1");
	data->Draw("LeadPt[0]>>pf100EffDenum","PFJet80 == 1");
	pf100Eff->Divide(pf100Eff,pf100EffDenum,1,1,"b");
	pf100Eff->Draw("E");

	//74X
	TH1F *pf100Eff74X = new TH1F("pf100Eff74X",";p_{T}, [GeV]; #epsilon",100,70.,500.);
	pf100Eff74X->SetMarkerStyle(21);
	pf100Eff74X->SetMarkerColor(2);
	TH1F *pf100Eff74XDenum = new TH1F("pf100Eff74XDenum","",100,70.,500.);
	data74X->Draw("LeadPt[0]>>pf100Eff74X","LeadMatch100[0] == 1");
	data74X->Draw("LeadPt[0]>>pf100Eff74XDenum","PFJet80 == 1");
	pf100Eff74X->Divide(pf100Eff74X,pf100Eff74XDenum,1,1,"b");
	pf100Eff->Draw("E");
	pf100Eff74X->Draw("E same");

	auto legenda = style.legend("bottom,right",3,0.4);
	legenda->AddEntry(pf100Eff,"76X","p");
	legenda->AddEntry(pf100Eff74X,"74X","p");

	TH1F * null = 0;
	auto RatioLowPt = (TH1F*) ratio.DrawRatio(pf100Eff,pf100Eff74X,null,legenda,oneDFitLowM);
	RatioLowPt->GetYaxis()->SetTitle("76X/74X");
	RatioLowPt->Draw();
	oneDFitLowM->SaveAs("pictures/LowMPtTrigger74vs76.pdf");

	//,,,,,,,,,,,,,dEta..........
	auto dEtaCan = new TCanvas("dEtaCan","dEta Trig Eff",1000,800);
	style.standardTitle(PRIVATE);

	TH1F *dEtaEff = new TH1F("dEtaEff","; #Delta #eta; #epsilon",200,0.,3.);
	TH1F *dEtaEffDenum = new TH1F("dEtaEffDenum","; #Delta #eta; #epsilon",200,0.,3.);
	data->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEff","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && LeadMatch100dEta1p6 == 1 && doubleJetTopology == 1");
	data->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEffDenum","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && PFJet80 == 1 && doubleJetTopology == 1");
	dEtaEff->Divide(dEtaEff,dEtaEffDenum,1,1,"b");
	dEtaEff->SetMarkerStyle(20);
	dEtaEff->Draw("E");

	TH1F *dEtaEff74X = new TH1F("dEtaEff74X","; #Delta #eta; #epsilon",200,0.,3.);
	TH1F *dEtaEff74XDenum = new TH1F("dEtaEff74XDenum","; #Delta #eta; #epsilon",200,0.,3.);
	data74X->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEff74X","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && LeadMatch100dEta1p6 == 1 && doubleJetTopology == 1");
	data74X->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEff74XDenum","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && PFJet80 == 1 && doubleJetTopology == 1");
	dEtaEff74X->Divide(dEtaEff74X,dEtaEff74XDenum,1,1,"b");
	dEtaEff74X->SetMarkerStyle(20);
	dEtaEff74X->Draw("E");

	auto RatioDEta = (TH1F*) ratio.DrawRatio(dEtaEff,dEtaEff74X,null,legenda,dEtaCan);
	RatioDEta->GetYaxis()->SetTitle("76X/74X");
	RatioDEta->Draw();
	dEtaCan->SaveAs("pictures/dEtaTrigger74vs76.pdf");



	/*
	oneDFitLowM->SaveAs("pictures/OneDEfficiencyLowM.pdf");

	TCanvas *twoDplot = new TCanvas("twoDplot","Two dimensional Efficiency");
	float Bins[] = {50.,90.,96.,112.,118.,124,130.,135.,140.,145,150,170,200,250,360,500};
//	float Bins[] = {120.,140.,150,160,170,180,190,200,220,240,260,300,350,400,500};
	int size = sizeof(Bins)/sizeof(float) - 1;

	TH2F *TwoDEff_Num = new TH2F("TwoDEff_Num","2D Efficiency",size,Bins,size,Bins);
	TwoDEff_Num ->SetMarkerStyle(20);
	TH2F *TwoDEff_Denum = new TH2F("TwoDEff_Denum","2D Efficiency",size,Bins,size,Bins);
	TwoDEff_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_Num","LeadMatch100[0] == 1 && LeadMatch100[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_Denum","PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	TwoDEff_Num ->Divide(TwoDEff_Num,TwoDEff_Denum,1,1,"b");
	TwoDEff_Num ->Draw("E");

	TCanvas *can2 = new TCanvas("can2","Symetry Canva",1000,800);
    TH2F *TwoDEff_NumSym = new TH2F("TwoDEff_NumSym","2D Efficiency with True method",size,Bins,size,Bins);
    TwoDEff_NumSym ->SetMarkerStyle(20);
    TH2F *TwoDEffTrueMC_DenumSym = new TH2F("TwoDEffTrueMC_DenumSym","2D Efficiency with True method",size,Bins,size,Bins);
    TwoDEff_NumSym ->SetTitle("2D Efficiency with True method;sub-leading jet p_{T};leading jet p_{T}");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_NumSym"," LeadMatch100[0] == 1 && LeadMatch100[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEffTrueMC_DenumSym","PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    TwoDEff_NumSym ->Divide(TwoDEff_NumSym,TwoDEffTrueMC_DenumSym,1,1,"b");

    for(int binx = 1; binx <= size; binx++)
    {
    	for(int biny = 1; biny <= size; biny++)
    	{
    		if(binx == biny){
    			std::cout<<"bin = "<<binx<<std::endl;
    			TwoDEff_NumSym->SetBinContent(binx,biny,0);
				TwoDEff_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }

    TwoDEff_Num->SetTitle("Work in progress, 2.2 fb^{-1}, #sqrt{s} = 13 TeV; p^{(1)}_{T}, (GeV); p^{(2)}_{T}, (GeV)");
    TwoDEff_Num->Add(TwoDEff_NumSym);
    gPad->SetRightMargin(0.15);
    TwoDEff_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEff_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEff_Num->Draw("E");

	TCanvas *can4;
	TwoDEff_Num->Draw("COLZ");
	TwoDEff_Num->SaveAs("pictures/LowMTwoDPtTriggerEff.pdf");
	TwoDEff_Num->Write();

	output->Close();

	auto dEtaCan = new TCanvas("dEtaCan","dEta Trig Eff",1000,800);
	style.standardTitle(PRIVATE);

	TH1F *dEtaEff = new TH1F("dEtaEff","; #Delta #eta; #epsilon",200,0.,3.);
	TH1F *dEtaEffDenum = new TH1F("dEtaEffDenum","; #Delta #eta; #epsilon",200,0.,3.);
	data->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEff","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && LeadMatch100dEta1p6 == 1 && doubleJetTopology == 1");
	data->Draw("abs(LeadEta[0]-LeadEta[1])>>dEtaEffDenum","abs(LeadEta[0]) < 2.2 && abs(LeadEta[1]) < 2.2 && LeadMatch100L3[0] == 1 && LeadMatch100L3[1] == 1 && PFJet80 == 1 && doubleJetTopology == 1");
	dEtaEff->Divide(dEtaEff,dEtaEffDenum,1,1,"b");
	dEtaEff->SetMarkerStyle(20);
	dEtaEff->Draw("E");


	TF1 *fitEta = new TF1("fitEta",finaleFunction,0,2.5,4);
	fitEta-> SetParameters(-1.15698e+02,1.59862e+00,9.93479e-02,-4.49790e+01);
	//dEtaEff->Fit(fitEta);
	dEtaEff->Draw("E");
	TH1F * ratiodEta = ratio.DrawRatio(dEtaEff,fitEta,"Fit:",dEtaCan);
	ratiodEta->Draw();
	dEtaCan->SaveAs("pictures/dEtaEfficiency.pdf");



	//............................	HIGH M ....................................

	TFile *outputHM = new TFile("TwoDPtHighMassEfficiency.root","recreate");

	TCanvas *can_high_M = new TCanvas("can_high_M","Two D Pt trig Eff",1000,800);

	const float Bins_H_mass[] = {120.,140.,150,160,170,180,190,200,220,240,260,300,350,400,500};
	size = sizeof(Bins_H_mass)/sizeof(float) - 1;

	TH2F *TwoDEff_H_mass_Num = new TH2F("TwoDEff_H_mass_Num","2D Efficiency",size,Bins_H_mass,size,Bins_H_mass);
	TwoDEff_H_mass_Num ->SetMarkerStyle(20);
	TH2F *TwoDEff_H_mass_Denum = new TH2F("TwoDEff_H_mass_Denum","2D Efficiency",size,Bins_H_mass,size,Bins_H_mass);
	TwoDEff_H_mass_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_H_mass_Num","LeadPt[0] > 140 && LeadPt[1] > 140 && LeadMatch160[0] == 1 && LeadMatch160[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	data ->Draw("LeadPt[0]:LeadPt[1]>>TwoDEff_H_mass_Denum","LeadPt[0] > 140 && LeadPt[1] > 140 && PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
	TwoDEff_H_mass_Num ->Divide(TwoDEff_H_mass_Num,TwoDEff_H_mass_Denum,1,1,"b");
	TwoDEff_H_mass_Num ->Draw("E");

    TH2F *TwoDEff_H_mass_NumSym = new TH2F("TwoDEff_H_mass_NumSym","2D Efficiency with True method",size,Bins_H_mass,size,Bins_H_mass);
    TwoDEff_H_mass_NumSym ->SetMarkerStyle(20);
    TH2F *TwoDEff_H_massTrueMC_DenumSym = new TH2F("TwoDEff_H_massTrueMC_DenumSym","2D Efficiency with True method",size,Bins_H_mass,size,Bins_H_mass);
    TwoDEff_H_mass_NumSym ->SetTitle("2D Efficiency with True method;sub-leading jet p_{T};leading jet p_{T}");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_H_mass_NumSym","LeadPt[0] > 140 && LeadPt[1] > 140 && LeadMatch160[0] == 1 && LeadMatch160[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_H_massTrueMC_DenumSym","LeadPt[0] > 140 && LeadPt[1] > 140 && PFJet80 == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
    TwoDEff_H_mass_NumSym ->Divide(TwoDEff_H_mass_NumSym,TwoDEff_H_massTrueMC_DenumSym,1,1,"b");

    for(int binx = 1; binx <= size; binx++)
    {
    	for(int biny = 1; biny <= size; biny++)
    	{
    		if(binx == biny){
    			std::cout<<"bin = "<<binx<<std::endl;
    			TwoDEff_H_mass_NumSym->SetBinContent(binx,biny,0);
				TwoDEff_H_mass_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }

    TwoDEff_H_mass_Num->SetTitle("Work in progress, 2.2 fb^{-1}, #sqrt{s} = 13 TeV; p^{(1)}_{T}, (GeV); p^{(2)}_{T}, (GeV)");
    TwoDEff_H_mass_Num->Add(TwoDEff_H_mass_NumSym);
    gPad->SetRightMargin(0.15);
    TwoDEff_H_mass_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEff_H_mass_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEff_H_mass_Num->Draw("E");

	TwoDEff_H_mass_Num->Draw("COLZ");
	TwoDEff_H_mass_Num->SaveAs("pictures/HighMTwoDPtTriggerEff.pdf");
	TwoDEff_H_mass_Num->Write("TwoDEff_Num");
	outputHM->Close();

	TCanvas *oneDFitHighM = new TCanvas("oneDFitHighM","Factor High M",1000,800);
	TH1F *pf160Eff = new TH1F("pf160Eff",";p_{T}, [GeV]; #epsilon",100,100.,500.);
	pf160Eff->SetMarkerStyle(20);
	TH1F *pf160EffDenum = new TH1F("pf160EffDenum","",100,100.,500.);
	data->Draw("LeadPt[0]>>pf160Eff","LeadMatch160[0] == 1");
	data->Draw("LeadPt[0]>>pf160EffDenum","PFJet80 == 1");
	pf160Eff->Divide(pf160Eff,pf160EffDenum,1,1,"b");
	pf160Eff->Draw("E");

	fit->SetRange(160,500);
	fit->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
//	pf160Eff->Fit(fit);
//	pf160Eff->Draw("E");
//	drawRatio(pf160Eff,fit,oneDFitLowM,"bla");
	auto hHighMRatio = (TH1F*) ratio.DrawRatio(pf160Eff,fit,"Sigmoid*Sigmoid",oneDFitHighM);
	oneDFitLowM->SaveAs("pictures/OneDEfficiencyHighM.pdf");


/**/
	return 0;


}

double finaleFunction(double *x, double *par)
{

	double sigmoid = 1./(1.+exp( -par[0] * (x[0] - par[1]) ));
	double erf = 1 / 2. * (1 + par[0]*TMath::Erf((x[0]-par[1])/(sqrt(2)*par[2])));
	double erfc = 1+ TMath::Erfc((par[5]-x[0])/(sqrt(2)*par[6]));
	double result =  0.5 * (1. + TMath::Erf((par[2]*x[0]-par[1])/(sqrt(2)*par[0])));
	double sigmoid2 = 1./ (1.+exp( -par[2] * (x[0] - par[3]) ));
	return sigmoid * sigmoid2;

}

double dEtaFitFunction(double *x, double *par)
{
	double sigmoid = 1./(1.+exp( - par[0] * (x[0] - par[1]) ));
	return sigmoid;
}

