#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"


std::pair<double,double> linearInterpolation(const double &x, TH1* h);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);

int kinematicTrigEff_Data(){

	TH1::SetDefaultSumw2();
	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

	TFile *fDataLowM = new TFile( ( cmsswBase + "/src/Analysis/MssmHbb/output/TriggerEff_Data2016_ReReco.root").c_str());
//	TFile *fDataHighM = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3/src/Analysis/MssmHbb/output/TriggerEff_highM_Run2015D-16Dec2015-v1.root");

//	TFile *outputLow 	= new TFile("TwoDPtLowMassEfficiency.root","recreate");
//	TFile *outputHigh 	= new TFile("TwoDPtHighMassEfficiency.root","recreate");

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup Ratio
   RatioPlots ratio(PRIVATE);
   ratio.SetRatioRange(0.5,1.5);
   ratio.SetRatioTitle("Data/Fit");

   //........................LOW M..................................//
	auto dEtaCan = new TCanvas("dEtaCan","dEta Trig Eff",1000,800);
	style.standardTitle(PRIVATE);

	TH1D *dEtaEff = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_1D_PF60_PF100_dEta");
	dEtaEff->SetMarkerStyle(20);
	dEtaEff->SetTitle(";#Delta #eta;#epsilon");

	TF1 *fitEta = new TF1("fitEta",dEtaFitFunction,0,1.65,2);
	fitEta-> SetParameters(-1.15698e+02,1.59862e+00);//,9.93479e-02,-4.49790e+01);
	auto ratiodEta = (TH1D*) ratio.DrawRatio(dEtaEff,fitEta,"Fit:",dEtaCan);
	ratiodEta->Draw();
	dEtaCan->SaveAs("../pictures/TriggerPerformance/dEtaEff_lowM.pdf");

	/*

	TCanvas *oneDFitLowM = new TCanvas("oneDFitLowM","Factor Low M",1000,800);
//	TH1D *pf100Effpt2 = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_1D_PF80_PF100_pt2");
	TH1D *pf100Eff = (TH1D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_1D_PF80_PF100_pt1");
	style.standardTitle(PRIVATE);
	pf100Eff->SetTitle(";leading jet p_{T}, [GeV]; #epsilon");
	pf100Eff->SetMarkerStyle(20);
	pf100Eff->Draw("E1");
	TF1 *fit1dLowM = new TF1("fit1dLowM",finaleFunction,100,500,4);
	fit1dLowM->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
	auto hLowMRatio = (TH1D*) ratio.DrawRatio(pf100Eff,fit1dLowM,"Sigmoid*Sigmoid",oneDFitLowM);

	oneDFitLowM->Print("../pictures/TriggerPerformance/1DEff_PF80_LowM.pdf");

	TCanvas *twoDplotLowM = new TCanvas("twoDplotLowM","Two dimensional Efficiency");
	TH2D *TwoDEff_Num = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_PF80_PF100_pt1vspt2");
    TH2D *TwoDEff_NumSym = (TH2D*) fDataLowM->Get("TriggerEfficiencies/KinTrigEff_2D_PF80_PF100_pt2vspt1");
	TwoDEff_Num->SetTitle(";sub-leading jet p_{T}, [GeV]; leading jet p_{T}, [GeV]; #epsilon");
	style.standardTitle(PRIVATE);
    for(int binx = 1; binx <= TwoDEff_NumSym->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEff_NumSym->GetNbinsY(); biny++)
    	{
    		if(binx == biny){
    			TwoDEff_NumSym->SetBinContent(binx,biny,0);
    			TwoDEff_NumSym->SetBinError(binx,biny,0);
    		}
    	}
    }
    TwoDEff_Num->Add(TwoDEff_NumSym);

	TwoDEff_Num ->SetMarkerStyle(20);
    gPad->SetRightMargin(0.15);
    TwoDEff_Num->GetZaxis()->SetTitleOffset(0.8);
//    gStyle->SetPadLeftMargin(0.85);
    TwoDEff_Num->GetXaxis()->SetTitleOffset(1.1);
	TwoDEff_Num->Draw("E");
//	outputLow->cd();
//	TwoDEff_Num->Write();

	twoDplotLowM->SetLeftMargin(0.08);
	twoDplotLowM->SetRightMargin(0.1);
	TwoDEff_Num->Draw("COLZ");
	twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_PF80_PF100_LowM_COLZ.pdf");
	TwoDEff_Num->Draw("BOX");
	twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_PF80_PF100_LowM_BOX.pdf");
	TwoDEff_Num->Draw("CONT1");
	twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_PF80_PF100_LowM_CONT1.pdf");
	TwoDEff_Num->Draw("CONT4");
	twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_PF80_PF100_LowM_CONT4.pdf");
	TwoDEff_Num->Draw("E");
	twoDplotLowM->Print("../pictures/TriggerPerformance/2DEff_PF80_PF100_LowM_E.pdf");
//	TwoDEff_Num->Write();


	//........Corelation.......//
	TH2D *Correlation_PF80_LowM = new TH2D(*TwoDEff_Num);
    for(int binx = 1; binx <= TwoDEff_Num->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEff_Num->GetNbinsY(); biny++)
    	{
    		double xx = TwoDEff_Num->GetBinContent(binx,binx);
    		double yy = TwoDEff_Num->GetBinContent(biny,biny);
    		double xy = TwoDEff_Num->GetBinContent(binx,biny);
    		double cxy = 0;
    		double ecxy = 0;
    		if(xx > 0 && yy>0){
    			cxy = xy/sqrt(xx*yy);
    			if(xx!=yy && xy != 0){
    		          double exy = TwoDEff_Num->GetBinError(binx,biny) / xy;
    		          double exx = TwoDEff_Num->GetBinError(binx,binx) / xx;
    		          double eyy = TwoDEff_Num->GetBinError(biny,biny) / yy;
    		          ecxy = sqrt( exy*exy + 0.25*exx*exx + 0.25*eyy*eyy );
    			}
    		}
    		Correlation_PF80_LowM->SetBinContent(binx,biny,cxy);
    		Correlation_PF80_LowM->SetBinError(binx,biny,ecxy);
    	}
    }
    Correlation_PF80_LowM->SetTitle("Correlation coefficient");
    TCanvas *canCorelLowM = new TCanvas("canCorelLowM","Correlation for LowM");
    canCorelLowM->SetLeftMargin(0.08);
    canCorelLowM->SetRightMargin(0.1);
    Correlation_PF80_LowM->Draw("COLZ");
    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_Correlation_PF80_PF100_LowM_COLZ.pdf");
    Correlation_PF80_LowM->Draw("BOX");
    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_Correlation_PF80_PF100_LowM_BOX.pdf");
    Correlation_PF80_LowM->Draw("CONT1");
    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_Correlation_PF80_PF100_LowM_CONT1.pdf");
    Correlation_PF80_LowM->Draw("CONT4");
    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_Correlation_PF80_PF100_LowM_CONT4.pdf");
    Correlation_PF80_LowM->Draw("E");
    canCorelLowM->Print("../pictures/TriggerPerformance/2DEff_Correlation_PF80_PF100_LowM_E.pdf");

    //..........2D / 1D*1D..........//
    TH2D *Ratio2DOver1D = new TH2D(*TwoDEff_Num);
    for(int binx = 1; binx <= TwoDEff_Num->GetNbinsX(); binx++)
    {
    	for(int biny = 1; biny <= TwoDEff_Num->GetNbinsY(); biny++)
    	{
    		double xVal = TwoDEff_Num->GetXaxis()->GetBinCenter(binx);
    		double yVal = TwoDEff_Num->GetYaxis()->GetBinCenter(biny);
    		std::pair <double,double> interpX(linearInterpolation(xVal,pf100Eff));
    		std::pair <double,double> interpY(linearInterpolation(yVal,pf100Eff));
    		double val1D = interpX.first * interpY.first;
    		double eVal1D = val1D* sqrt( (interpX.second/interpX.first)*(interpX.second/interpX.first) + (interpY.second/interpY.first)*(interpY.second/interpY.first)  );
    		double ratio;
    		if(val1D == 0 || TwoDEff_Num->GetBinContent(binx,biny) == 0) ratio = 0;
    		else ratio = TwoDEff_Num->GetBinContent(binx,biny) / val1D;
    		double eratio;
    		if(eVal1D == 0 || TwoDEff_Num->GetBinError(binx,biny) == 0) eratio = 0;
    		else eratio = ratio * sqrt( (eVal1D/val1D)*(eVal1D/val1D) + (TwoDEff_Num->GetBinError(binx,biny)/TwoDEff_Num->GetBinContent(binx,biny))*(TwoDEff_Num->GetBinError(binx,biny)/TwoDEff_Num->GetBinContent(binx,biny)) );

//    		std::cout<<"X: "<<interpX.first<<" +/- "<<interpX.second<<std::endl;
    		std::cout<<"X = "<<xVal<<" Y = "<<yVal<<std::endl;
    		std::cout<<"Ratio: "<<ratio<<" +/- "<<eratio<<std::endl;
    		std::cout<<"Corel: "<<Correlation_PF80_LowM->GetBinContent(binx,biny)<<" +/- "<<Correlation_PF80_LowM->GetBinError(binx,biny)<<""<<std::endl;

    		Ratio2DOver1D->SetBinContent(binx,biny,ratio);
    		Ratio2DOver1D->SetBinError(binx,biny,eratio);
    	}
    }
    Ratio2DOver1D->SetTitle("Ratio 2D vs 1D*1D");
    TCanvas *ratio1D2D = new TCanvas("ratio1D2D","RATIO 1D vs 2D");
    ratio1D2D->SetLeftMargin(0.08); // ->SetPadLeftMargin(0.1);
    ratio1D2D->SetRightMargin(0.1);
    Ratio2DOver1D->Draw("COLZ");
    ratio1D2D->Print("../pictures/TriggerPerformance/2DEff_Ratio2D1D_PF80_PF100_LowM_COLZ.pdf");
    Ratio2DOver1D->Draw("BOX");
    ratio1D2D->Print("../pictures/TriggerPerformance/2DEff_Ratio2D1D_PF80_PF100_LowM_BOX.pdf");
    Ratio2DOver1D->Draw("CONT1");
    ratio1D2D->Print("../pictures/TriggerPerformance/2DEff_Ratio2D1D_PF80_PF100_LowM_CONT1.pdf");
    Ratio2DOver1D->Draw("CONT4");
    ratio1D2D->Print("../pictures/TriggerPerformance/2DEff_Ratio2D1D_PF80_PF100_LowM_CONT4.pdf");
    Ratio2DOver1D->Draw("E");
    ratio1D2D->Print("../pictures/TriggerPerformance/2DEff_Ratio2D1D_PF80_PF100_LowM_E.pdf");

    //Slices:
    TCanvas *canl;
    TH1D ** slix_eff_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
    TH1D ** slix_corel_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
    TH1D ** slix_ratio_lowM = new TH1D*[TwoDEff_Num->GetNbinsY()];
    std::string name;

    for(int biny = 0; biny < TwoDEff_Num->GetNbinsY(); biny++){
    	canl = new TCanvas("canl");
    	slix_eff_lowM[biny] 	= TwoDEff_Num->ProjectionX(("Slice_Eff_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
    	slix_eff_lowM[biny]->Draw("E");
    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_eff_lowM[biny]->GetName()) + ".pdf";
    	canl->Print(name.c_str());
    	slix_ratio_lowM[biny]  	= Ratio2DOver1D->ProjectionX(("Slice_Ratio_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
    	slix_ratio_lowM[biny]->Draw("E");
    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_ratio_lowM[biny]->GetName()) + ".pdf";
    	canl->Print(name.c_str());
    	slix_corel_lowM[biny]	= Correlation_PF80_LowM->ProjectionX(("Slice_Corel_LowM_ybin_" + std::to_string(biny) ).c_str(),biny+1,biny+1,"e");
    	slix_corel_lowM[biny]->Draw("E");
    	name = "../pictures/TriggerPerformance/slices/" + std::string(slix_corel_lowM[biny]->GetName()) + ".pdf";
    	canl->Print(name.c_str());
    }


	//............................	HIGH M ....................................

//	TCanvas *oneDFitHighM = new TCanvas("oneDFitHighM","Factor Low M",1000,800);
//	TH1D *pf160Eff = (TH1D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_1D_PF80_PF160_pt1");
//	pf160Eff->SetMarkerStyle(20);
//	pf160Eff->Draw("E1");
//	TF1 *fit1DHighM = new TF1("fit1DHighM",finaleFunction,160,500,4);
//	fit1DHighM->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
//	auto hHighMRatio = (TH1D*) ratio.DrawRatio(pf160Eff,fit1DHighM,"Sigmoid*Sigmoid",oneDFitHighM);
//
//	oneDFitHighM->Print("../pictures/TriggerPerformance/1DEff_PF80_HighM.pdf");
//
//	TCanvas *twoDplotHighM = new TCanvas("twoDplotHighM","Two dimensional Efficiency");
//
//	TH2D *TwoDEffPF160_Num = (TH2D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_2D_PF80_PF160_pt1vspt2");
//    TH2D *TwoDEffPF160_NumSym = (TH2D*) fDataHighM->Get("TriggerEfficiencies/KinTrigEff_2D_PF80_PF160_pt2vspt1");
//    for(int binx = 1; binx <= TwoDEffPF160_NumSym->GetNbinsX(); binx++)
//    {
//    	for(int biny = 1; biny <= TwoDEffPF160_NumSym->GetNbinsX(); biny++)
//    	{
//    		if(binx == biny){
//    			TwoDEffPF160_NumSym->SetBinContent(binx,biny,0);
//    			TwoDEffPF160_NumSym->SetBinError(binx,biny,0);
//    		}
//    	}
//    }
//    TwoDEffPF160_Num->Add(TwoDEffPF160_NumSym);
//
//	TwoDEffPF160_Num ->SetMarkerStyle(20);
//	TwoDEffPF160_Num ->SetTitle("2D Efficiency;sub-leading jet p_{T};leading jet p_{T}; #epsilon");
//    gPad->SetRightMargin(0.15);
//    TwoDEffPF160_Num->GetZaxis()->SetTitleOffset(0.8);
//    TwoDEffPF160_Num->GetXaxis()->SetTitleOffset(1.1);
//	TwoDEffPF160_Num->Draw("E");
//
//	TCanvas *canSave_high;
//	TwoDEffPF160_Num->Draw("COLZ");
//	canSave_high->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_HighM.pdf");

	//.................................f(pt,eta).....................................//
	//.................................Hope- Finale version..........................//
	TCanvas *can_twoDLowM_pt1eta1 = new TCanvas("can_twoDLowM_pt1eta1","2D Trigger eff = f(pt1,eta1)",1000,800);
	TH2D *pf80for100_pt1eta1 = (TH2D*) fDataLowM ->Get("TriggerEfficiencies/KinTrigEff_2D_PF80_PF100_pt1eta1");
	can_twoDLowM_pt1eta1->SetLeftMargin(0.08); // ->SetPadLeftMargin(0.1);
	can_twoDLowM_pt1eta1->SetRightMargin(0.1);
	pf80for100_pt1eta1->Draw("COLZ");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_colz.pdf");
	pf80for100_pt1eta1->Draw("E");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_e.pdf");
	pf80for100_pt1eta1->Draw("CONT1");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_cont1.pdf");
	pf80for100_pt1eta1->Draw("CONT4");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_cont4.pdf");
	pf80for100_pt1eta1->Draw("SURF2");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_surf2.pdf");
	pf80for100_pt1eta1->Draw("SURF4");
	can_twoDLowM_pt1eta1->Print("../pictures/TriggerPerformance/2DEff_PF80_PF160_LowM_pt1eta1_surf4.pdf");

	/*
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
//	TwoDEff_H_mass_Num->Write("TwoDEff_Num");


	TCanvas *oneDFitHighM = new TCanvas("oneDFitHighM","Factor High M",1000,800);
	TH1D *pf160Eff = new TH1D("pf160Eff",";p_{T}, [GeV]; #epsilon",100,100.,500.);
	pf160Eff->SetMarkerStyle(20);
	TH1D *pf160EffDenum = new TH1D("pf160EffDenum","",100,100.,500.);
	data->Draw("LeadPt[0]>>pf160Eff","LeadMatch160[0] == 1");
	data->Draw("LeadPt[0]>>pf160EffDenum","PFJet80 == 1");
	pf160Eff->Divide(pf160Eff,pf160EffDenum,1,1,"b");
	pf160Eff->Draw("E");

	fit->SetRange(160,500);
	fit->SetParameters(1.16893e-01,1.05584e+02,1.16893e-01,1.05584e+02);
//	pf160Eff->Fit(fit);
//	pf160Eff->Draw("E");
//	drawRatio(pf160Eff,fit,oneDFitLowM,"bla");
	auto hHighMRatio = (TH1D*) ratio.DrawRatio(pf160Eff,fit,"Sigmoid*Sigmoid",oneDFitHighM);
	oneDFitLowM->SaveAs("pictures/OneDEfficiencyHighM.pdf");

//	outputHM->Close();
//	output->Close();


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

std::pair<double,double> linearInterpolation(const double &x, TH1* h){
	std::pair<double,double> result;
	//For error propagation
	double y,x0,y0,x1,y1,ery, ery1, ery0;
	int binx = h->FindBin(x);

	if(x<=h->GetBinCenter(1)) {
		result.first = h->GetBinContent(1);
		result.second = h->GetBinError(1);
		return result;
	} else if(x>=h->GetBinCenter(h->GetNbinsX())) {
		result.first = h->GetBinContent(h->GetNbinsX());
		result.second = h->GetBinContent(h->GetNbinsX());
		return result;
	}
	else if(x<=h->GetBinCenter(binx)){
		y0 = h->GetBinContent(binx-1);
		ery0 = h->GetBinError(binx-1);
		x0 = h->GetBinCenter(binx-1);
		y1 = h->GetBinContent(binx);
		ery1 = h->GetBinError(binx);
		x1 = h->GetBinCenter(binx);
	} else {
		y0 = h->GetBinContent(binx);
		ery0 = h->GetBinError(binx);
		x0 = h->GetBinCenter(binx);
		y1 = h->GetBinContent(binx+1);
		ery1 = h->GetBinError(binx+1);
		x1 = h->GetBinCenter(binx+1);
	}
	ery = sqrt( ((x1-x)/(x1-x0))*((x1-x)/(x1-x0)) * ery0*ery0 + ((x-x0)/(x1-x0))*((x-x0)/(x1-x0))  * ery1*ery1 );
	y   = y0 + (x-x0)*((y1-y0)/(x1-x0));
	result.first = y;
	result.second = ery;

	return result;
}
