#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"


void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
double finaleFunction(double *x, double *par);

int highMTriggerEfficiency(){

	TH1::SetDefaultSumw2();

	TFile *fData = new TFile("/nfs/dust/cms/user/shevchen/output/EfficiencyStudy_Run2015_16_02_2016.root");

	TFile *output = new TFile("~/cms/cmssw-analysis/CMSSW_7_5_2/src/Analysis/MssmHbb/bin/input_corrections/TwoDPtLowMassEfficiency_eta2p5.root","recreate");

	TTree *data;
	fData->GetObject("MssmHbb",data);

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

	TCanvas *oneDFit = new TCanvas();
	TH1F *pf100Eff = new TH1F("pf100Eff","",100,120.,500.);
	TH1F *pf100EffDenum = new TH1F("pf100EffDenum","",100,120.,500.);
	data->Draw("LeadPt[0]>>pf100Eff","LeadMatch160[0] == 1");
	data->Draw("LeadPt[0]>>pf100EffDenum","PFJet80 == 1");
	pf100Eff->Divide(pf100Eff,pf100EffDenum,1,1,"b");
	pf100Eff->Draw("E");

	TF1 *fit = new TF1("fit",finaleFunction,120,500,4);
	pf100Eff->Fit(fit);
	pf100Eff->Draw("E");
	drawRatio(pf100Eff,fit,oneDFit,"bla");



	TCanvas *twoDplot = new TCanvas("twoDplot","Two dimensional Efficiency");
	float Bins[] = {0.,90.,96.,112.,118.,124,130.,135.,140.,145,150,170,200,250,360,500};
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
    data ->Draw("LeadPt[1]:LeadPt[0]>>TwoDEff_NumSym","LeadMatch100[0] == 1 && LeadMatch100[1] == 1 && doubleJetTopology == 1 && dPhiFS>2.7");
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
	TwoDEff_Num->Draw("E");

	TCanvas *can4;
	TwoDEff_Num->Draw("COLZ");
//	TwoDEff_Num->Write();

//	output->Close();


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

void drawRatio(TH1F *histo,TF1 *fit, TCanvas *canva, std::string fitName)
{

   //TCanvas *canva = new TCanvas("canva","",1000,800);
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();

//   histo -> SetTitle("HLT_PFJet80 100 GeV Pt treshold;; #epsilon");
//   histo -> SetMarkerColor(1);
//   histo -> SetMarkerStyle(20);
//   histo -> Draw("ep");

   TH1F * hRatio = (TH1F*)histo->Clone("hRatio");
   histo -> Fit(fit,"R+");
   char name[200];
   sprintf(name,"}{#chi^{2}/ndf = %.2f}",fit->GetChisquare()/fit->GetNDF());
   fitName = "#splitline{ " + fitName + name;
   TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend -> SetTextSize(0.04);
   fitLegend -> AddEntry(fit,fitName.c_str(),"l");
   histo -> Draw();
   fitLegend->Draw();
   canva -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   hRatio -> Sumw2();

   hRatio -> Divide(fit);
   hRatio -> Draw();

   TLine *horizLine = new TLine(120,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();


//   cc2->cd();
//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.6,1.4);
   hRatio -> GetYaxis() -> SetTitle("Ratio histo/fit ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);

   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(histo->GetXaxis() -> GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);

//   std::cout<<" Chi2/ndf: = "<<fit1D -> GetChisquare()/fit1D -> GetNDF()<<" sigmoid: "<<fit2D -> GetChisquare() / fit2D -> GetNDF()<<" Finale: "<<fit3D -> GetChisquare() / fit3D -> GetNDF()<<std::endl;

}
