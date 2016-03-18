#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

int MCvsMC(){

	gROOT -> Reset();

   TFile * fOldVersion = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_74X_test_lowMTrigger_QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_16_03_2016.root");
   TFile * fNewVersion = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_test_lowMTrigger_QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8_15_03_2016.root");
   std::string mcName = "fOldVersion";

   TTree *tOldVersion, *tNewVersion;
   fOldVersion -> GetObject("MssmHbb",tOldVersion);
   fNewVersion -> GetObject("MssmHbb",tNewVersion);

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PUBLIC);
   ratio->SetRatioTitle("Data / MC"); // Default Title for Ratio plot Y-axis
   ratio->SetRatioRange(0.2,1.8);	// Default Y-axis range for Ratio plot


   //TCut section
   TCut withPUWeight = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[0]";
   TCut noPUWeight = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight";


   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   //..............................Pt1 ....................


    const int NumberOfPtBins = 24;
    double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

    TCanvas *canva_no_PU = new TCanvas("canva_no_PU","Pt1",1000,800);

    //Main Histograms
    TH1F *hLeadPt_OldVersion = new TH1F("hLeadPt_OldVersion","First Leading Jet Pt",NumberOfPtBins,PtBins);
    TH1F *hLeadPt_NewVersion = new TH1F("hLeadPt_NewVersion",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);


    tOldVersion ->Draw("LeadPt[0]>>hLeadPt_OldVersion",weightLumi,"E");
    tNewVersion ->Draw("LeadPt[0]>>hLeadPt_NewVersion",weightLumi,"E");

    //Normalization:

    for(int i = 1;i<=NumberOfPtBins;++i){
    	hLeadPt_NewVersion->SetBinContent(i,hLeadPt_NewVersion->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
    	hLeadPt_NewVersion->SetBinError(i,hLeadPt_NewVersion->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

 	   hLeadPt_OldVersion->SetBinContent(i,hLeadPt_OldVersion->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
 	   hLeadPt_OldVersion->SetBinError(i,hLeadPt_OldVersion->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
    }


    TLegend *leg_pt1 = new TLegend(0.55,0.65,0.8,0.9);
    leg_pt1->AddEntry(hLeadPt_NewVersion,"76X","p");
    leg_pt1->AddEntry(hLeadPt_OldVersion,"74X","p");

    TH1F *null_ptr = NULL;
    hLeadPt_NewVersion->SetMarkerStyle(20);
    hLeadPt_NewVersion->SetMarkerColor(1);
    hLeadPt_NewVersion->GetXaxis()->SetRangeUser(0.,1000.);
    hLeadPt_OldVersion->SetMarkerStyle(21);
    hLeadPt_OldVersion->SetMarkerColor(2);


    TH1F *ratioPt = ratio->DrawRatio(hLeadPt_NewVersion,hLeadPt_OldVersion,null_ptr,leg_pt1,canva_no_PU);
    ratio->GetTopPad()->SetLogy();
    ratioPt->GetXaxis()->SetRangeUser(0.,1000.);
    canva_no_PU->SaveAs("MCvsMC_Pt_AllWeights.pdf");


	return 0;
}
