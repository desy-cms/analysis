#include "Analysis/Drawer/src/HbbStyle.cc"
#include "Analysis/Drawer/src/RatioPlots.cpp"
#include "TCut.h"

double erfFunction(double *x, double *par);
double sigmoid(double *x, double *par);
double finaleFunction(double *x, double *par);
double dEtaFitFunction(double *x, double *par);
void drawRatio(TH1F *, TF1 *,TCanvas *,std::string );
void drawRatio(TH1F*, TH1F*, TCanvas*);
TCanvas *drawRatio(TH1F *, TF1 *,TF1 *,TH1F *, TCanvas *,std::string );
TH1F *constructCombined(TH1F *, TFile * file,const int &,const int &);
TH1F *constructCombined(TH1F *,const TCut &, TTree * tree,const int &,const int &);

TH1F *calculatePtEffSyst(TH1F*,TH1F*);

void MCbranches()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelectionQCD_180116.root");

   TTree *dataTree, *mcTree;
   fMonteCarlo -> GetObject("MssmHbb",mcTree);
   fData -> GetObject("MssmHbb",dataTree);

   //Setup style
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PUBLIC);

   int bin40to60 = 90;
   int bin60to80 = 115;
   TCut cut = "dPhiFS>2.7";// && Njets == 2 && NL1Object >= 2";//"ptVeto < 10 && ((fLeadPt - sLeadPt)/(fLeadPt + sLeadPt) <= 0.2) && abs(fLeadEta) <= 2. && abs(sLeadEta)<=2";
   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   TCut btagSFup = "BTagSFup";
   TCut btagSFdown = "BTagSFdown";
   double SFb = 0.97*0.97;

   //..............................Pt1 ....................
   TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);
   TH1F *fLeadPtMC = new TH1F("fLeadPtMC","First Leading Jet Pt",100,0.,1000.);
   TH1F *fLeadPtMCSyst = new TH1F("fLeadPtMCSyst","Systematic errors",100,0.,1000.);
   TH1F *fLeadPtData = new TH1F("fLeadPtData","First Leading Jet Pt",100,0.,1000.);
   fLeadPtData->SetTitle(" ;p_{T1} , [GeV]; Events");

   mcTree ->Draw("LeadPt[0]>>fLeadPtMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtMCSyst",weightLumi*weightBTag*ptSystWeight*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[0]>>fLeadPtData");

   fLeadPtMCSyst = calculatePtEffSyst(fLeadPtMC,fLeadPtMCSyst);
   fLeadPtMC->Draw("E");

   TLegend *leg_pt1 = new TLegend(0.55,0.5,0.8,0.8);
   leg_pt1->AddEntry(fLeadPtData,"Run2015D-Prompt-Reco-v4","p");
   leg_pt1->AddEntry(fLeadPtMC,"QCD 13 TeV pythia 8","p");

   fLeadPtMCSyst = NULL;
   TH1F *ratioPt = ratio->DrawRatio(fLeadPtData,fLeadPtMC,fLeadPtMCSyst,leg_pt1,canva00);
   ratio->GetTopPad()->SetLogy();
   ratioPt->SetAxisRange(0.,2.,"Y");
   ratioPt->SetTitle(";leading b jet p_{T} (GeV);Data/MC");

   //..............................Pt2 ....................
   TCanvas *canva01 = new TCanvas("canva01","Pt2",1000,800);
   TH1F *fLeadPt2MC = new TH1F("fLeadPt2MC","First Leading Jet Pt",100,0.,1000.);
   TH1F *fLeadPt2MCSyst = new TH1F("fLeadPt2MCSyst","Systematic errors",100,0.,1000.);
   TH1F *fLeadPt2Data = new TH1F("fLeadPt2Data","First Leading Jet Pt",100,0.,1000.);
   fLeadPt2Data->SetTitle(" ;p_{T2} , [GeV]; Events");

   mcTree ->Draw("LeadPt[1]>>fLeadPt2MC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("LeadPt[1]>>fLeadPt2MCSyst",weightLumi*weightBTag*ptSystWeight*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("LeadPt[1]>>fLeadPt2Data");

   fLeadPt2MCSyst->Draw("E");
   fLeadPt2MCSyst = calculatePtEffSyst(fLeadPt2MC,fLeadPt2MCSyst);
//   fLeadPt2MC->Draw("E");

   TH1F *ratioPt2 = ratio->DrawRatio(fLeadPt2Data,fLeadPt2MC,fLeadPt2MCSyst,NULL,canva01);
   ratio->GetTopPad()->SetLogy();
   ratioPt2->SetAxisRange(0.,2.,"Y");
   ratioPt2->SetTitle(";p_{T2};Data/MC");

   //..............................dEta ....................
   TCanvas *canva02 = new TCanvas("canva02","dEta",1000,800);
   TH1F *fLeaddEtaMC = new TH1F("fLeaddEtaMC","First Leading Jet Pt",40,-2.,2.);
   TH1F *fLeaddEtaMCSyst = new TH1F("fLeaddEtaMCSyst","Systematic errors",40,-2.,2.);
   TH1F *fLeaddEtaData = new TH1F("fLeaddEtaData","First Leading Jet Pt",40,-2.,2.);
   fLeaddEtaData->SetTitle(" ;#Delta #Eta; Events");

   mcTree ->Draw("dEtaFS>>fLeaddEtaMC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("dEtaFS>>fLeaddEtaMCSyst",weightLumi*weightBTag*ptSystWeight*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("dEtaFS>>fLeaddEtaData");

   fLeaddEtaMCSyst->Draw("E");
   fLeaddEtaMCSyst = calculatePtEffSyst(fLeaddEtaMC,fLeaddEtaMCSyst);
//   fLeaddEtaMC->Draw("E");

   TH1F *ratiodEta = ratio->DrawRatio(fLeaddEtaData,fLeaddEtaMC,fLeaddEtaMCSyst,NULL,canva02);
   ratiodEta->SetAxisRange(0.,2.,"Y");
   ratiodEta->SetTitle(";#Delta #Eta;Data/MC");

   //..............................M12 ....................
   TCanvas *canva03 = new TCanvas("canva03","M12",1000,800);
   TH1F *fLeadObjM12MC = new TH1F("fLeadObjM12MC","",80,0.,1000.);
   TH1F *fLeadObjM12MCSyst = new TH1F("fLeadObjM12MCSyst","",80,0.,1000.);
   TH1F *fLeadObjM12Data = new TH1F("fLeadObjM12Data","",80,0.,1000.);
   fLeadObjM12Data->GetYaxis()->SetTitle("Events");

   mcTree ->Draw("ObjM12>>fLeadObjM12MC",weightLumi*weightBTag*weightPt*weightdEta*btagSFcentral,"E");
   mcTree ->Draw("ObjM12>>fLeadObjM12MCSyst",weightLumi*weightBTag*ptSystWeight*weightdEta*btagSFcentral,"E");
   dataTree ->Draw("ObjM12>>fLeadObjM12Data");

   fLeadObjM12MCSyst->Draw("E");
   fLeadObjM12MCSyst = calculatePtEffSyst(fLeadObjM12MC,fLeadObjM12MCSyst);
//   fLeadObjM12MC->Draw("E");

   TH1F *ratioObjM12 = ratio->DrawRatio(fLeadObjM12Data,fLeadObjM12MC,fLeadObjM12MCSyst,NULL,canva03);
   ratioObjM12->SetAxisRange(0.,2.,"Y");
   ratioObjM12->SetTitle(";M_{12};Data/MC");

   //..............................BTag discr ....................
   TCanvas *canva04 = new TCanvas("canva04","BTagCSV",1000,800);
   TH1F *fLeadBTagMC = new TH1F("fLeadBTagMC","",80,0.94,1.);
   TH1F *fLeadBTagMCSyst = new TH1F("fLeadBTagMCSyst","",80,0.94,1.);
   TH1F *fLeadBTagData = new TH1F("fLeadBTagData","",80,0.94,1.);
   fLeadBTagData->GetYaxis()->SetTitle("Events");

   mcTree ->Draw("LeadBTag[0]>>fLeadBTagMC",weightLumi*weightBTag*weightPt*weightdEta,"E");
   mcTree ->Draw("LeadBTag[0]>>fLeadBTagMCSyst",weightLumi*weightBTag*ptSystWeight*weightdEta,"E");
   dataTree ->Draw("LeadBTag[0]>>fLeadBTagData");

   fLeadBTagMCSyst->Draw("E");
   fLeadBTagMCSyst = calculatePtEffSyst(fLeadBTagMC,fLeadBTagMCSyst);
   fLeadBTagMC->Scale(SFb);
   fLeadBTagMCSyst->Scale(SFb);
//   fLeadBTagMC->Draw("E");

   TH1F *ratioBTag = ratio->DrawRatio(fLeadBTagData,fLeadBTagMC,fLeadBTagMCSyst,NULL,canva04);
   ratioBTag->SetAxisRange(0.,2.,"Y");
   ratioBTag->SetTitle(";BTag CSV discr J_{1};Data/MC");
/**/
}

TH1F *calculatePtEffSyst(TH1F *D2Plot, TH1F *SystPlot){
	TH1F *result = (TH1F*) D2Plot->Clone("result");
	for(int bin = 1; bin < result->GetNbinsX();bin++){
		result->SetBinContent(bin,D2Plot->GetBinContent(bin));
		result->SetBinError(bin,D2Plot->GetBinError(bin)+abs(SystPlot->GetBinContent(bin)));
	}

	return result;
}


TH1F *constructCombined(TH1F *CombinedH,const TCut &cut, TTree *tree,const int & bin40to60,const int &bin60to80){

	CombinedH -> Reset();
	TH1::SetDefaultSumw2();

	TH1F *h_Num = new TH1F("h_Num","",200,0.,500);
	TH1F *h_Denum = new TH1F("h_Denum","",200,0.,500);

	tree->Draw("hpf60for100_Num>>h_Num",cut);
	tree->Draw("hpf60for100_Denum>>h_Denum",cut);
	h_Num ->Divide(h_Num,h_Denum,1,1,"b");
	TH1F *HLT60 = (TH1F*) h_Num -> Clone("HLT60");

	tree->Draw("hpf100_Num>>h_Num",cut);
	tree->Draw("hpf100_Denum>>h_Denum",cut);
	h_Num ->Divide(h_Num,h_Denum,1,1,"b");
	TH1F *HLT80 =(TH1F*) h_Num -> Clone("HLT80");

    int xlow90 = HLT60 -> FindBin(bin40to60 + 2);
   	int xlow115 = HLT80  -> FindBin(bin60to80 + 2);
   	int xupp115 = HLT60  -> FindBin(bin60to80 - 2);
   	for(int bin = 1;bin< HLT60 -> GetNbinsX (); bin++)
   {
   		if(bin > xupp115) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   		if(bin < xlow90) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   	}

   	for(int bin = 1;bin< HLT80 -> GetNbinsX (); bin++)
   {
   		if(bin < xlow115) {
   			HLT80 -> SetBinContent(bin,0.);
   			HLT80 -> SetBinError(bin,0.);
   		}
   	}

//   	CombinedH->Add(HLT40);
   	CombinedH->Add(HLT60);
   	CombinedH->Add(HLT80);
   	CombinedH -> SetMaximum(1.02);

   	return CombinedH;
}

void drawRatio(TH1F *hNum, TH1F *hDenum, TCanvas *can){

	HbbStyle style;
	style.set(PUBLIC);
   TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();
   gPad->SetLogy();

   TH1F * hRatio = (TH1F*)hNum->Clone("hRatio");
   TLegend *Legend = new TLegend(0.4,0.45,0.9,0.65);
   Legend -> SetTextSize(0.04);
   Legend -> AddEntry(hNum,hNum->GetTitle(),"p");
   Legend -> AddEntry(hDenum,hDenum->GetTitle(),"p");
   hNum -> Draw();
   hDenum -> Draw("same");
   Legend->Draw();
   can -> cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();

   hRatio -> Sumw2();
   hRatio -> Divide(hDenum);
   hRatio -> Draw();

   TLine *horizLine = new TLine(0,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();

	//   cc2->cd();
	//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.,2.);
   hRatio -> GetYaxis() -> SetTitle("Ratio Old/New ");
   hRatio -> GetYaxis() -> SetNdivisions(505);
   hRatio -> GetYaxis() -> SetTitleSize(20);
   hRatio -> GetYaxis() -> SetTitleFont(43);
   hRatio -> GetYaxis() -> SetTitleOffset(1.55);
   hRatio -> GetYaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetYaxis() -> SetLabelSize(20);

	   // X axis ratio plot settings
   hRatio -> GetXaxis() -> SetTitle(hNum->GetXaxis() -> GetTitle());
   hRatio -> GetXaxis() -> SetTitleSize(20);
   hRatio -> GetXaxis() -> SetTitleFont(43);
   hRatio -> GetXaxis() -> SetTitleOffset(3.);
   hRatio -> GetXaxis() -> SetLabelFont(43); // Absolute font size in pixel (precision 3)
   hRatio -> GetXaxis() -> SetLabelSize(20);
}

TH1F *constructCombined(TH1F *CombinedH, TFile *file,const int & bin40to60,const int &bin60to80){

	CombinedH -> Reset();
	TH1F *h = (TH1F*) file -> Get("hpf40_eff");
	TH1F *HLT40 = (TH1F*) h -> Clone("HLT40");
	h = (TH1F*) file -> Get("HLT_PFJet60/hpf60_eff");
	TH1F *HLT60 = (TH1F*) h -> Clone("HLT60");
	h = (TH1F*) file ->Get("HLT_PFJet80/hpf80_eff");
	TH1F *HLT80 =(TH1F*) h -> Clone("HLT80");

    int xlow90 = HLT60 -> FindBin(bin40to60 + 2);
    int xupp90 = HLT40 -> FindBin(bin40to60 - 2);
    for(int bin = 1;bin< HLT40 -> GetNbinsX (); bin++)
    {
    		if(bin > xupp90) {
   			HLT40 -> SetBinContent(bin,0.);
   			HLT40 -> SetBinError(bin,0.);
   		}
   	}

   	int xlow115 = HLT80  -> FindBin(bin60to80 + 2);
   	int xupp115 = HLT60  -> FindBin(bin60to80 - 2);
   	for(int bin = 1;bin< HLT60 -> GetNbinsX (); bin++)
   {
   		if(bin > xupp115) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   		if(bin < xlow90) {
   			HLT60 -> SetBinContent(bin,0.);
   			HLT60 -> SetBinError(bin,0.);
   		}

   	}

   	for(int bin = 1;bin< HLT80 -> GetNbinsX (); bin++)
   {
   		if(bin < xlow115) {
   			HLT80 -> SetBinContent(bin,0.);
   			HLT80 -> SetBinError(bin,0.);
   		}
   	}

//   	CombinedH->Add(HLT40);
   	CombinedH->Add(HLT60);
   	CombinedH->Add(HLT80);
   	CombinedH -> SetMaximum(1.02);

   	return CombinedH;
}

double dEtaFitFunction(double *x, double *par)
{
	double sigmoid = 1./(1.+exp( - par[0] * (x[0] - par[1]) ));
	return sigmoid;
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

double erfFunction(double *x, double *par)
{
	//return 0.5 * (1. + TMath::Erf((x[0]-par[0])/(sqrt(2)*par[1])));
	return 0.5 * (1. + TMath::Erf((x[0]-par[0])/(sqrt(2)*par[1]))) * 1./(1.+exp( -par[2] * (x[0] - par[3]) ));
	//return (par[0]/2)*(par[0]/2) * (1 + TMath::Erf((x[0]-par[2])/(sqrt(2)*par[1]))) * (1 + TMath::Erf((x[0]-par[3])/(sqrt(2)*par[4])));
}

double sigmoid(double *x, double *par)
{
	return 1./(1.+exp( -par[0] * (x[0] - par[1]) ));
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

   TLine *horizLine = new TLine(0,1,500,1);
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

TCanvas *drawRatio(TH1F *histo,TF1 *fitMC, TF1 *fitData, TH1F *hData,  TCanvas *canva, std::string fitName)
{

TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1);
   pad1 -> SetBottomMargin(0.05);
   pad1 -> Draw();
   pad1 -> cd();

//   histo -> SetTitle("HLT_PFJet80 100 GeV Pt treshold;; #epsilon");
//   histo -> SetMarkerColor(1);
//   histo -> SetMarkerStyle(20);
//   histo -> Draw("ep");

   histo -> Fit(fitMC,"R+");
   fitData -> SetLineColor(2);
   char name[200];
   sprintf(name,"}{#chi^{2}/ndf = %.2f}",fitMC->GetChisquare()/fitMC->GetNDF());
   fitName = "#splitline{ " + fitName + name;
   TLegend *fitLegend = new TLegend(0.4,0.45,0.9,0.65);
   fitLegend -> SetTextSize(0.04);
   fitLegend -> AddEntry(fitMC,fitName.c_str(),"l");
   fitLegend -> AddEntry(fitData,"Fit to Data","l");
   histo -> Draw();
   fitLegend->Draw();
   fitData -> Draw("same");
   canva->cd();

   TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
   pad2 -> SetTopMargin(0);
   pad2 -> SetBottomMargin(0.2);
   pad2 -> Draw();
   pad2 -> cd();



   TH1F* hRatio = (TH1F*) hData->Clone("hRatio");
   hRatio -> Divide(hData,histo,1,1,"b");
   hRatio -> Draw();

   TLine *horizLine = new TLine(0,1,500,1);
   horizLine -> SetLineStyle(2);
   horizLine -> Draw();


//   cc2->cd();
//   cc2->Update();
   hRatio -> SetTitle("");
   hRatio -> GetYaxis() -> SetRangeUser(0.6,1.4);
   hRatio -> GetYaxis() -> SetTitle("Ratio fitData/fitMC ");
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
	return canva;
}
