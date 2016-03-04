#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

int MCvsMC(){

	gROOT -> Reset();

   TFile * fMC1 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_LowMass_QCD_26_02_2016.root");
   TFile * fMC2 = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_QCD_Pt_12_02_2016.root");
   std::string mcName = "fMC1";

   TTree *mc1Tree, *mc2Tree;
   fMC1 -> GetObject("MssmHbb",mc1Tree);
   fMC2 -> GetObject("MssmHbb",mc2Tree);

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
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   //..............................Pt1 ....................
    TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

    const int NumberOfPtBins = 24;
    double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

    //Main Histograms
    TH1F *fLeadPtMC = new TH1F("fLeadPtMC","First Leading Jet Pt",30,-2.,2);
    TH1F *fLeadPtMCnew = new TH1F("fLeadPtMCnew",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",30,-2,2);
    fLeadPtMCnew->GetXaxis()->SetRangeUser(-2.,2.);

    mc1Tree ->Draw("dEtaFS>>fLeadPtMC",weightdEta,"E");
    mc2Tree ->Draw("dEtaFS>>fLeadPtMCnew",weightdEta,"E");

    //Normalization:

//    for(int i = 1;i<=30;++i){
// 	   fLeadPtMCnew->SetBinContent(i,fLeadPtMCnew->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
// 	   fLeadPtMCnew->SetBinError(i,fLeadPtMCnew->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
//
// 	   fLeadPtMC->SetBinContent(i,fLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
// 	   fLeadPtMC->SetBinError(i,fLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));
//    }


    TLegend *leg_pt1 = new TLegend(0.55,0.65,0.8,0.9);
    leg_pt1->AddEntry(fLeadPtMCnew,"fMC2","p");
    leg_pt1->AddEntry(fLeadPtMC,mcName.c_str(),"p");

    TH1F *null_ptr = NULL;


    TH1F *ratioPt = ratio->DrawRatio(fLeadPtMCnew,fLeadPtMC,null_ptr,leg_pt1,canva00);
    ratio->GetTopPad()->SetLogy();
    ratioPt->GetXaxis()->SetRangeUser(-2.,2.);

    TCanvas *can2 = new TCanvas();
    auto weightDEtaOld = new TH1F("weightDEtaOld","",20,0.9,1);
    auto weightDEtaNew = new TH1F("weightDEtaNew","",20,0.9,1);

    mc1Tree->Draw("dEtaWeight>>weightDEtaNew");
    mc2Tree->Draw("dEtaWeight>>weightDEtaOld");

    TH1F *ratiodEta = ratio->DrawRatio(weightDEtaOld,weightDEtaNew,null_ptr,leg_pt1,can2);
    ratio->GetTopPad()->SetLogy();
    ratiodEta->GetXaxis()->SetRangeUser(0.4,1.1);

	return 0;
}
