#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

int DataVSData(){

	gROOT -> Reset();

   TFile * fLowMass = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_NoTriggerMatching_lowMTrigger_samples_02_03_2016.root");
   TFile * fHighMass = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_NoTriggerMatching_highMTrigger_samples_02_03_2016.root");
   std::string mcName = "fMC1";

   TTree *lowMTree, *highMTree;
   fLowMass -> GetObject("MssmHbb",lowMTree);
   fHighMass -> GetObject("MssmHbb",highMTree);

   //Setup style
   HbbStyle style;
   style.set(PUBLIC);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PUBLIC);
   ratio->SetRatioTitle("Data / MC"); // Default Title for Ratio plot Y-axis
   ratio->SetRatioRange(0.2,1.8);	// Default Y-axis range for Ratio plot


   //TCut section
   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   //..............................Pt1 ....................
    TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

    const int NumberOfPtBins = 24;
    double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

    //Main Histograms
    TH1F *fLeadPtLow = new TH1F("fLeadPtLow","First Leading Jet Pt",NumberOfPtBins,PtBins);
    TH1F *fLeadPtHigh = new TH1F("fLeadPtHigh",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
    fLeadPtHigh->GetXaxis()->SetRangeUser(0.,1000.);

    lowMTree ->Draw("LeadPt[0]>>fLeadPtLow"  ,"LeadPt[0] > 250 && LeadPt[1] > 250 && abs(LeadEta[0] - LeadEta[1]) < 1.4 && LeadBTag[0] > 0.97 && LeadBTag[1] > 0.97","E");
    highMTree ->Draw("LeadPt[0]>>fLeadPtHigh","LeadPt[0] > 250 && LeadPt[1] > 250 && abs(LeadEta[0] - LeadEta[1]) < 1.4 && LeadBTag[0] > 0.97 && LeadBTag[1] > 0.97 ","E");

    //Normalization:

    for(int i = 1;i<=NumberOfPtBins;++i){
 	   fLeadPtHigh->SetBinContent(i,fLeadPtHigh->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
 	   fLeadPtHigh->SetBinError(i,fLeadPtHigh->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

 	   fLeadPtLow->SetBinContent(i,fLeadPtLow->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
 	   fLeadPtLow->SetBinError(i,fLeadPtLow->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
    }


    TLegend *leg_pt1 = new TLegend(0.55,0.65,0.8,0.9);
    leg_pt1->AddEntry(fLeadPtHigh,"High","p");
    leg_pt1->AddEntry(fLeadPtLow,"Low","p");

    TH1F *null_ptr = NULL;


    TH1F *ratioPt = ratio->DrawRatio(fLeadPtHigh,fLeadPtLow,null_ptr,leg_pt1,canva00);
    ratio->GetTopPad()->SetLogy();
    ratioPt->GetXaxis()->SetRangeUser(0.,1000.);

	return 0;
}
