#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"
#include "Analysis/MssmHbb/macros/Drawer/SystematicErrorsCalculator.cpp"
#include "TCut.h"
#include "TGraphAsymmErrors.h"
#include "THStack.h"

double M12FitFunction(double *x, double *par);

void ComparisonLeaves()
{
	gROOT -> Reset();
   //gStyle->SetOptFit(1111);
   //gROOT->ForceStyle();

   TFile * fData = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_lowMTrigger_14_03_2016.root");
   TFile * fMonteCarlo = new TFile("/nfs/dust/cms/user/shevchen/output/DoubleBTagSelection_76X_lowMTrigger_Pythia8_16_03_2016.root");

   std::string mcName = "Pythia 8 MC";

   TTree *dataTree, *mcTree;
   fMonteCarlo -> GetObject("MssmHbb",mcTree);
   fData -> GetObject("MssmHbb",dataTree);

   //Setup style
   HbbStyle style;
   style.set(PRIVATE);
   TH1::SetDefaultSumw2();

   //Setup ratio plots
   RatioPlots *ratio = new RatioPlots(PRIVATE);
   ratio->SetRatioTitle("Data / MC"); // Default Title for Ratio plot Y-axis
   ratio->SetRatioRange(0.2,1.8);	// Default Y-axis range for Ratio plot

   //Declare SystematicCalc object
   SystematicErrorsCalculator *systCalc = new SystematicErrorsCalculator();

   //...............................WEIGHTS.................................//
   //Central
   TCut centralWeight = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[0]";
   //SFb
   TCut SFbWeightUp = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFup[0] * BTagSFup[1]  * BTagWeight * WeightPileUp[0]";
   TCut SFbWeightDown = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFdown[0] * BTagSFdown[1]  * BTagWeight * WeightPileUp[0]";
   //PileUp
   TCut PileUpWeightUp = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[1]";
   TCut PileUpWeightDown = "LumiWeight*TwoDPtWeight*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[2]";
   //Trigger Pt Efficiency
   TCut PtTriggerWeight = "LumiWeight*abs(FactorizationPtWeight - TwoDPtWeight)*dEtaWeight*BTagSFcentral[0] * BTagSFcentral[1]  * BTagWeight * WeightPileUp[0]";

   TCut weightLumi = "LumiWeight";
   TCut weightPt  = "TwoDPtWeight";
   TCut weightdEta = "dEtaWeight";
   TCut weightBTag = "BTagWeight";
   TCut weightPtFactorization = "FactorizationPtWeight";
   TCut ptSystWeight = "abs(FactorizationPtWeight - TwoDPtWeight)";
   TCut btagSFcentral = "BTagSFcentral[0] * BTagSFcentral[1]";
   TCut btagSFup = "BTagSFup[0] * BTagSFup[1]";
   TCut btagSFdown = "BTagSFdown[0] * BTagSFdown[1]";
   TCut pileupWeight = "WeightPileUp[0]";

   //Start drawing
   //......................Primary Vertex................
   TCanvas *canvaPV = new TCanvas("canvaPV","Primary Vertex",1000,800);

   const int NumberOfNpvBins = 33;


   //Main Histograms
   TH1F *NPV_MC = new TH1F("NPV_MC","",NumberOfNpvBins,0,33);
   TH1F *NPV_MCSyst = new TH1F("NPV_MCSyst","",NumberOfNpvBins,0,33);
   TH1F *NPV_Data = new TH1F("NPV_Data",";N_{PV} ; dN / d(N_{PV}) ",NumberOfNpvBins,0,33);
   NPV_Data->GetXaxis()->SetRangeUser(0.,50.);

   mcTree ->Draw("NPrimaryVTX>>NPV_MCSyst",centralWeight,"E");
   mcTree ->Draw("NPrimaryVTX>>NPV_MC",centralWeight,"E");
   dataTree ->Draw("NPrimaryVTX>>NPV_Data","","E");


   //Systematic errors
   // Syst. for SFb

   auto SFbUp = (TH1F*) NPV_MC->Clone("SFbUp");
   auto SFbDown = (TH1F*) NPV_MC->Clone("SFbDown");
   //Syst. for 2D Pt trigger efficiency
   auto Pt2D = (TH1F*) NPV_MC->Clone("Pt2D");
   //Syst. for PileUp
   auto PUup = (TH1F*) NPV_MC->Clone("PUup");
   auto PUdown = (TH1F*) NPV_MC->Clone("PUdown");

   mcTree ->Draw("NPrimaryVTX>>SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("NPrimaryVTX>>SFbDown",SFbWeightDown,"E");

   mcTree ->Draw("NPrimaryVTX>>Pt2D",PtTriggerWeight,"E");

   mcTree ->Draw("NPrimaryVTX>>PUup",PileUpWeightUp,"E");
   mcTree ->Draw("NPrimaryVTX>>PUdown",PileUpWeightDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfNpvBins;++i){
	   NPV_Data->SetBinContent(i,NPV_Data->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
	   NPV_Data->SetBinError(i,NPV_Data->GetBinError(i)/(NPV_Data->GetBinWidth(10)));

	   NPV_MC->SetBinContent(i,NPV_MC->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
	   NPV_MC->SetBinError(i,NPV_MC->GetBinError(i)/(NPV_Data->GetBinWidth(10)));

	   NPV_MCSyst->SetBinContent(i,NPV_MCSyst->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
	   NPV_MCSyst->SetBinError(i,NPV_MCSyst->GetBinError(i)/(NPV_Data->GetBinWidth(10)));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(NPV_Data->GetBinWidth(10)));
   }

   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->AddUncorelatedSystErrors(NPV_MCSyst);

   TLegend *leg_pv;
   leg_pv = (TLegend*) style.legend("top,right",3,0.3);
   leg_pv->AddEntry(NPV_Data,"Data","p");
   leg_pv->AddEntry(NPV_MC,mcName.c_str(),"p");

   TH1F *null = 0;

   TH1F *ratioPV = (TH1F*) ratio->DrawRatio(NPV_Data,NPV_MC,canvaPV,leg_pv,NPV_MCSyst);
   ratio->GetTopPad()->SetLogy();
   ratioPV->GetXaxis()->SetRangeUser(0.,50.);
   canvaPV->SaveAs("pictures/NPrimaryVertices1.pdf");

//   ratio->DrawPhaseSpaceDescription(100.,0.1,500.,3.);


   //.....................Ht bins.........................
   TCanvas *canva001 = new TCanvas("canva001","Jet multiplicity",1000,800);


   double HtBins[]={200,205,210,215,220,
		   	   	   	   	   	   	    240,260,280,300,330,
									360,390,420,450,480,
									510,550,600,650,700,
									750,800,850,900,950,
									1000,1100,1200,1300,1400,
									1500,1600,1700,1800,1900,
   	   	   	   	   	   	   	   	   	2000,2200,2600};
   const int NumberofHtBins = sizeof(HtBins)/sizeof(double) - 1;

   //Main Histograms
   TH1F *fHtMC = new TH1F("fHtMC","",NumberofHtBins,HtBins);
   TH1F *fHtMCSyst = new TH1F("fHtMCSyst","",NumberofHtBins,HtBins);
   TH1F *fHtData = new TH1F("fHtData",";H_{T}, (GeV) ; dN / d H_{T}",NumberofHtBins,HtBins);
   fHtData->GetXaxis()->SetRangeUser(0.,2600.);

   mcTree ->Draw("Ht>>fHtMCSyst",centralWeight,"E");
   mcTree ->Draw("Ht>>fHtMC",centralWeight,"E");
   dataTree ->Draw("Ht>>fHtData","");

   SFbUp = (TH1F*) fHtMC->Clone("HtSFbUp");
   SFbDown = (TH1F*) fHtMC->Clone("HtSFbDown");
   Pt2D = (TH1F*) fHtMC->Clone("HtPt2D");
   PUup = (TH1F*) fHtMC->Clone("HtPUup");
   PUdown = (TH1F*) fHtMC->Clone("HtPUdown");

   mcTree ->Draw("Ht>>HtSFbUp",SFbWeightUp,"E");
   mcTree ->Draw("Ht>>HtSFbDown",SFbWeightDown,"E");
   mcTree ->Draw("Ht>>HtPt2D",PtTriggerWeight,"E");
   mcTree ->Draw("Ht>>HtPUup",PileUpWeightUp,"E");
   mcTree ->Draw("Ht>>HtPUdown",PileUpWeightDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberofHtBins;++i){
	   fHtData->SetBinContent(i,fHtData->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   fHtData->SetBinError(i,fHtData->GetBinError(i)/(HtBins[i]-HtBins[i-1]));

	   fHtMC->SetBinContent(i,fHtMC->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   fHtMC->SetBinError(i,fHtMC->GetBinError(i)/(HtBins[i]-HtBins[i-1]));

	   fHtMCSyst->SetBinContent(i,fHtMCSyst->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   fHtMCSyst->SetBinError(i,fHtMCSyst->GetBinError(i)/(HtBins[i]-HtBins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(HtBins[i]-HtBins[i-1]));

   }

   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->AddUncorelatedSystErrors(fHtMCSyst);

//   std::cout<<"Table of values:"<<std::endl;
//   for (int bin = 1; bin<=NumberofHtBins;++bin){
//	   std::cout<<"Data: "<<fHtData->GetBinContent(bin)<<" +/- "<<fHtData->GetBinError(bin)<<std::endl;
//	   std::cout<<"MC  : "<<fHtMC->GetBinContent(bin)<<" +/- "<<fHtMC->GetBinError(bin)<<" (stat.) "<<" +/- "<<std::sqrt(fHtMCSyst->GetBinError(bin)*fHtMCSyst->GetBinError(bin) - fHtMC->GetBinError(bin)*fHtMC->GetBinError(bin))<<" (syst.) "<<" +/- "<<fHtMCSyst->GetBinError(bin)<<" (tot.)"<<std::endl;
//   }

   TLegend *leg_pt1;
   leg_pt1 = (TLegend*) style.legend("top,right",3,0.3);
   leg_pt1->AddEntry(fHtData,"Data","p");
   leg_pt1->AddEntry(fHtMC,mcName.c_str(),"p");


   TH1F *ratioHt = (TH1F*) ratio->DrawRatio(fHtData,fHtMC,canva001,leg_pt1,fHtMCSyst);
   ratio->GetTopPad()->SetLogy();
   ratioHt->GetXaxis()->SetRangeUser(0.,2600.);
//   ratio->DrawPhaseSpaceDescription(300.,0.008,1200.,0.3);

   canva001->SaveAs("pictures/ht.pdf");

   /*
   TFile *Htratio = new TFile("HtRatio.root","recreate");
   ratioHt->Write();
   Htratio->Write();
   Htratio->Close();
   */



   //.....................Jet Multiplicity.................
   TCanvas *canva000 = new TCanvas("canva000","Jet multiplicity",1000,800);

   const int NumberOfNBins = 8;
   double NBins[NumberOfNBins+1]={2,3,4,5,6,7,8,9,14};

   //Main Histograms
   TH1F *fNjetsMC = new TH1F("fNjetsMC","",NumberOfNBins,NBins);
   TH1F *fNjetsMCSyst = new TH1F("fNjetsMCSyst","",NumberOfNBins,NBins);
   TH1F *fNjetsData = new TH1F("fNjetsData",";Jet multiplicity ; dN / d Jet Multiplicity",NumberOfNBins,NBins);
   fNjetsData->GetXaxis()->SetRangeUser(0.,14.);

   mcTree ->Draw("Njets>>fNjetsMCSyst",centralWeight,"E");
   mcTree ->Draw("Njets>>fNjetsMC",centralWeight,"E");
   dataTree ->Draw("Njets>>fNjetsData","");

   SFbUp = (TH1F*) fNjetsMC->Clone("NjSFbUp");
   SFbDown = (TH1F*) fNjetsMC->Clone("NjSFbDown");
   Pt2D = (TH1F*) fNjetsMC->Clone("NjPt2D");
   PUup = (TH1F*) fNjetsMC->Clone("NjPUup");
   PUdown = (TH1F*) fNjetsMC->Clone("NjPUdown");

   mcTree ->Draw("Njets>>NjSFbUp",SFbWeightUp,"E");
   mcTree ->Draw("Njets>>NjSFbDown",SFbWeightDown,"E");
   mcTree ->Draw("Njets>>NjPt2D",PtTriggerWeight,"E");
   mcTree ->Draw("Njets>>NjPUup",PileUpWeightUp,"E");
   mcTree ->Draw("Njets>>NjPUdown",PileUpWeightDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfNBins;++i){
	   fNjetsData->SetBinContent(i,fNjetsData->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   fNjetsData->SetBinError(i,fNjetsData->GetBinError(i)/(NBins[i]-NBins[i-1]));

	   fNjetsMC->SetBinContent(i,fNjetsMC->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   fNjetsMC->SetBinError(i,fNjetsMC->GetBinError(i)/(NBins[i]-NBins[i-1]));

	   fNjetsMCSyst->SetBinContent(i,fNjetsMCSyst->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   fNjetsMCSyst->SetBinError(i,fNjetsMCSyst->GetBinError(i)/(NBins[i]-NBins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(NBins[i]-NBins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(NBins[i]-NBins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(NBins[i]-NBins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(NBins[i]-NBins[i-1]));
   }


   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->AddUncorelatedSystErrors(fNjetsMCSyst);

//   TH1F *ratioPt = (TH1F*) fNjetsData->Clone("ratioPt");

   TH1F *ratioN = (TH1F*) ratio->DrawRatio(fNjetsData,fNjetsMC,canva000,leg_pt1,fNjetsMCSyst);
   ratio->GetTopPad()->SetLogy();
   ratioN->GetXaxis()->SetRangeUser(0.,14.);
   canva000->SaveAs("pictures/Multiplicity.pdf");
//   ratio->DrawPhaseSpaceDescription(1.5,120.,6.,3000.);
/*
   //..............................Pt1 ....................
   TCanvas *canva00 = new TCanvas("canva00","Pt1",1000,800);

   const int NumberOfPtBins = 24;
   double PtBins[NumberOfPtBins+1]={100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,375,400,450,500,575,650,750,1000};

   //Main Histograms
   TH1F *fLeadPtMC = new TH1F("fLeadPtMC","",NumberOfPtBins,PtBins);
   TH1F *fLeadPtMCSyst = new TH1F("fLeadPtMCSyst","",NumberOfPtBins,PtBins);
   TH1F *fLeadPtData = new TH1F("fLeadPtData",";leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   fLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[0]>>fLeadPtMCSyst",centralWeight,"E");
   mcTree ->Draw("LeadPt[0]>>fLeadPtMC",centralWeight,"E");
   dataTree ->Draw("LeadPt[0]>>fLeadPtData","");

   SFbUp = (TH1F*) fLeadPtMC->Clone("Pt1SFbUp");
   SFbDown = (TH1F*) fLeadPtMC->Clone("Pt1SFbDown");
   Pt2D = (TH1F*) fLeadPtMC->Clone("Pt1Pt2D");
   PUup = (TH1F*) fLeadPtMC->Clone("Pt1PUup");
   PUdown = (TH1F*) fLeadPtMC->Clone("Pt1PUdown");

   mcTree ->Draw("LeadPt[0]>>Pt1SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("LeadPt[0]>>Pt1SFbDown",SFbWeightDown,"E");
   mcTree ->Draw("LeadPt[0]>>Pt1Pt2D",PtTriggerWeight,"E");
   mcTree ->Draw("LeadPt[0]>>Pt1PUup",PileUpWeightUp,"E");
   mcTree ->Draw("LeadPt[0]>>Pt1PUdown",PileUpWeightDown,"E");

   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   fLeadPtData->SetBinContent(i,fLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtData->SetBinError(i,fLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMC->SetBinContent(i,fLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMC->SetBinError(i,fLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   fLeadPtMCSyst->SetBinContent(i,fLeadPtMCSyst->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   fLeadPtMCSyst->SetBinError(i,fLeadPtMCSyst->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(fLeadPtMCSyst);

   std::cout<<"Table of values:"<<std::endl;
   for (int bin = 1; bin<=NumberOfPtBins;++bin){
	   std::cout<<"Data: "<<fLeadPtData->GetBinContent(bin)<<" +/- "<<fLeadPtData->GetBinError(bin)<<std::endl;
	   std::cout<<"MC  : "<<fLeadPtMC->GetBinContent(bin)<<" +/- "<<fLeadPtMC->GetBinError(bin)<<" (stat.) "<<" +/- "<<std::sqrt(fLeadPtMCSyst->GetBinError(bin)*fLeadPtMCSyst->GetBinError(bin) - fLeadPtMC->GetBinError(bin)*fLeadPtMC->GetBinError(bin))<<" (syst.) "<<" +/- "<<fLeadPtMCSyst->GetBinError(bin)<<" (tot.)"<<std::endl;
   }

   TH1F *ratioPt = ratio->DrawRatio(fLeadPtData,fLeadPtMC,fLeadPtMCSyst,leg_pt1,canva00);
   ratio->GetTopPad()->SetLogy();
   ratioPt->GetXaxis()->SetRangeUser(0.,1000.);
   canva00->SaveAs("pictures/pt1.pdf");
   ratio->DrawPhaseSpaceDescription(100.,0.1,500.,3.);


   //..............................Pt2 ....................
   TCanvas *canva01 = new TCanvas("canva01","Pt2",1000,800);

   //Main Histograms
   TH1F *sLeadPtMC = new TH1F("sLeadPtMC","",NumberOfPtBins,PtBins);
   TH1F *sLeadPtMCSyst = new TH1F("sLeadPtMCSyst","",NumberOfPtBins,PtBins);
   TH1F *sLeadPtData = new TH1F("sLeadPtData",";sub-leading b jet p_{T} (GeV) ; dN / dp_{T} (1/GeV)",NumberOfPtBins,PtBins);
   sLeadPtData->GetXaxis()->SetRangeUser(0.,1000.);

   mcTree ->Draw("LeadPt[1]>>sLeadPtMCSyst",centralWeight,"E");
   mcTree ->Draw("LeadPt[1]>>sLeadPtMC",centralWeight,"E");
   dataTree ->Draw("LeadPt[1]>>sLeadPtData");

   SFbUp = (TH1F*) sLeadPtMC->Clone("Pt2SFbUp");
   SFbDown = (TH1F*) sLeadPtMC->Clone("Pt2SFbDown");
   Pt2D = (TH1F*) sLeadPtMC->Clone("Pt2Pt2D");
   PUup = (TH1F*) sLeadPtMC->Clone("Pt2PUup");
   PUdown = (TH1F*) sLeadPtMC->Clone("Pt2PUdown");

   mcTree ->Draw("LeadPt[1]>>Pt2SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("LeadPt[1]>>Pt2SFbDown",SFbWeightDown,"E");
   mcTree ->Draw("LeadPt[1]>>Pt2Pt2D",PtTriggerWeight,"E");
   mcTree ->Draw("LeadPt[1]>>Pt2PUup",PileUpWeightUp,"E");
   mcTree ->Draw("LeadPt[1]>>Pt2PUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=NumberOfPtBins;++i){
	   sLeadPtData->SetBinContent(i,sLeadPtData->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtData->SetBinError(i,sLeadPtData->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMC->SetBinContent(i,sLeadPtMC->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMC->SetBinError(i,sLeadPtMC->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   sLeadPtMCSyst->SetBinContent(i,sLeadPtMCSyst->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   sLeadPtMCSyst->SetBinError(i,sLeadPtMCSyst->GetBinError(i)/(PtBins[i]-PtBins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(PtBins[i]-PtBins[i-1]));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(sLeadPtMCSyst);

//   TH1F *ratioPt = (TH1F*) sLeadPtData->Clone("ratioPt");

   TH1F *ratioPt2 = ratio->DrawRatio(sLeadPtData,sLeadPtMC,sLeadPtMCSyst,leg_pt1,canva01);
   ratio->GetTopPad()->SetLogy();
   ratioPt2->GetXaxis()->SetRangeUser(0.,1000.);
//   ratio->DrawPhaseSpaceDescription(100.,0.01,500.,.3);
   canva01->SaveAs("pictures/pt2.pdf");

   //.........................(Pt1-Pt2)/(Pt1+Pt2)..................
   TCanvas *canva02 = new TCanvas("canva02","PtAsym",1000,800);

   double PtAssymBins[] = {0.,0.02,0.04,0.06,0.08,0.1,0.12,0.14,0.16,0.18,0.2,0.24,0.28,0.32,0.36,0.4,0.45,0.5,0.6};
   const int NumberOfPtAsymBins = sizeof(PtAssymBins)/sizeof(double) - 1;
   //Central values
   //Main Histograms
   TH1F *AssymPtMC = new TH1F("AssymPtMC","",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtMCSyst = new TH1F("AssymPtMCSyst","",NumberOfPtAsymBins,PtAssymBins);
   TH1F *AssymPtData = new TH1F("AssymPtData",";(p^{1}_{T}-p^{2}_{T})/(p^{1}_{T}+p^{2}_{T});dN / d#frac{p^{1}_{T}-p^{2}_{T}}{p^{1}_{T}+p^{2}_{T}}",NumberOfPtAsymBins,PtAssymBins);
   AssymPtData->GetXaxis()->SetRangeUser(0.,0.6);

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMCSyst",centralWeight,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtMC",centralWeight,"E");
   dataTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>AssymPtData");

   SFbUp = (TH1F*) AssymPtMC->Clone("PtASFbUp");
   SFbDown = (TH1F*) AssymPtMC->Clone("PtASFbDown");
   Pt2D = (TH1F*) AssymPtMC->Clone("PtAPt2D");
   PUup = (TH1F*) AssymPtMC->Clone("PtAPUup");
   PUdown = (TH1F*) AssymPtMC->Clone("PtAPUdown");

   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>PtASFbUp",SFbWeightUp,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>PtASFbDown",SFbWeightDown,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>PtAPt2D",PtTriggerWeight,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>PtAPUup",PileUpWeightUp,"E");
   mcTree ->Draw("(LeadPt[0]-LeadPt[1])/(LeadPt[0]+LeadPt[1])>>PtAPUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=NumberOfPtAsymBins;++i){
	   AssymPtData->SetBinContent(i,AssymPtData->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtData->SetBinError(i,AssymPtData->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMC->SetBinContent(i,AssymPtMC->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMC->SetBinError(i,AssymPtMC->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   AssymPtMCSyst->SetBinContent(i,AssymPtMCSyst->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   AssymPtMCSyst->SetBinError(i,AssymPtMCSyst->GetBinError(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(PtAssymBins[i]-PtAssymBins[i-1]));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(AssymPtMCSyst);

   TH1F *ratioPtAssym;
   ratioPtAssym = ratio->DrawRatio(AssymPtData,AssymPtMC,AssymPtMCSyst,leg_pt1,canva02);
   ratio->GetTopPad()->SetLogy();
   ratioPtAssym->GetXaxis()->SetRangeUser(0.,0.6);
//   ratio->DrawPhaseSpaceDescription(0.03,500,0.3,9000);
   canva02->SaveAs("pictures/pt_asym.pdf");

   //........................dEta plot.............................
   TCanvas *canva03 = new TCanvas("canva03","dEta",1000,800);

   TH1F *dEtaMC = new TH1F("dEtaMC","",20,-1.6,1.6);
   TH1F *dEtaMCSyst = new TH1F("dEtaMCSyst","",20,-1.6,1.6);
   TH1F *dEtaData = new TH1F("dEtaData",";#Delta#eta; dN / d #Delta#eta",20,-1.6,1.6);
   dEtaData->GetXaxis()->SetRangeUser(-2.,2.);
   dEtaData->SetMinimum(10.);

   mcTree ->Draw("dEtaFS>>dEtaMCSyst",centralWeight,"E");
   mcTree ->Draw("dEtaFS>>dEtaMC",centralWeight,"E");
   dataTree ->Draw("dEtaFS>>dEtaData");

   SFbUp = (TH1F*) dEtaMC->Clone("dEtaSFbUp");
   SFbDown = (TH1F*) dEtaMC->Clone("dEtaSFbDown");
   Pt2D = (TH1F*) dEtaMC->Clone("dEtaPt2D");
   PUup = (TH1F*) dEtaMC->Clone("dEtaPUup");
   PUdown = (TH1F*) dEtaMC->Clone("dEtaPUdown");

   mcTree ->Draw("dEtaFS>>dEtaSFbUp",SFbWeightUp,"E");
   mcTree ->Draw("dEtaFS>>dEtaSFbDown",SFbWeightDown,"E");
   mcTree ->Draw("dEtaFS>>dEtaPt2D",PtTriggerWeight,"E");
   mcTree ->Draw("dEtaFS>>dEtaPUup",PileUpWeightUp,"E");
   mcTree ->Draw("dEtaFS>>dEtaPUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=20;++i){
	   dEtaData->SetBinContent(i,dEtaData->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaData->SetBinError(i,dEtaData->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMC->SetBinContent(i,dEtaMC->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMC->SetBinError(i,dEtaMC->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   dEtaMCSyst->SetBinContent(i,dEtaMCSyst->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   dEtaMCSyst->SetBinError(i,dEtaMCSyst->GetBinError(i)/(dEtaData->GetBinWidth(20)));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(dEtaData->GetBinWidth(20)));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(dEtaData->GetBinWidth(20)));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(dEtaData->GetBinWidth(20)));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(dEtaMCSyst);

   TLegend *leg_pt2;
   leg_pt2 = (TLegend*) style.legend("right,bottom",3);
   leg_pt2->AddEntry(fLeadPtData,"Data","p");
   leg_pt2->AddEntry(fLeadPtMC,mcName.c_str(),"p");

   TH1F *ratioDEta;
   ratioDEta = ratio->DrawRatio(dEtaData,dEtaMC,dEtaMCSyst,leg_pt1,canva03);
   ratioDEta->GetXaxis()->SetRangeUser(-2.,2.);
//   ratio->DrawPhaseSpaceDescription(-1.6,10000,0.,75000);
   canva03->SaveAs("pictures/dEta.pdf");

   //..............................M12 ....................
   TCanvas *canva04 = new TCanvas("canva04","M12",1000,800);
   double M12Bins[] = {200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,520,540,560,580,600,620,640,660,700,750,800,850,940,1100};
   const int NumberM12Bins = sizeof(M12Bins)/sizeof(double) -1;
   //Central values
   //Main Histograms
   TH1F *M12MC = new TH1F("M12MC","",NumberM12Bins,M12Bins);
   TH1F *M12MCSyst = new TH1F("M12MCSyst","",NumberM12Bins,M12Bins);
   TH1F *M12Data = new TH1F("M12Data",";di-jet Mass (GeV); dN / d(di-jet Mass) (1/GeV)",NumberM12Bins,M12Bins);
   M12Data->GetXaxis()->SetRangeUser(200.,1000.);

   mcTree ->Draw("ObjM12>>M12MCSyst",centralWeight,"E");
   mcTree ->Draw("ObjM12>>M12MC",centralWeight,"E");
   dataTree ->Draw("ObjM12>>M12Data");

   SFbUp = (TH1F*) M12MC->Clone("M12SFbUp");
   SFbDown = (TH1F*) M12MC->Clone("M12SFbDown");
   Pt2D = (TH1F*) M12MC->Clone("M12Pt2D");
   PUup = (TH1F*) M12MC->Clone("M12PUup");
   PUdown = (TH1F*) M12MC->Clone("M12PUdown");

   mcTree ->Draw("ObjM12>>M12SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("ObjM12>>M12SFbDown",SFbWeightDown,"E");
   mcTree ->Draw("ObjM12>>M12Pt2D",PtTriggerWeight,"E");
   mcTree ->Draw("ObjM12>>M12PUup",PileUpWeightUp,"E");
   mcTree ->Draw("ObjM12>>M12PUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=NumberM12Bins;++i){
	   M12Data->SetBinContent(i,M12Data->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12Data->SetBinError(i,M12Data->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

//	   if( M12Data->GetBinLowEdge(i) > 540 && (M12Data->GetBinLowEdge(i) + M12Data->GetBinWidth(i)) <= 940 ){
//		   M12Data->SetBinContent(i,0);
//		   M12Data->SetBinError(i,0);
//	   }

	   M12MC->SetBinContent(i,M12MC->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MC->SetBinError(i,M12MC->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   M12MCSyst->SetBinContent(i,M12MCSyst->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   M12MCSyst->SetBinError(i,M12MCSyst->GetBinError(i)/(M12Bins[i]-M12Bins[i-1]));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(M12Bins[i]-M12Bins[i-1]));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(M12MCSyst);

   TH1F *ratioM12;
   ratioM12 = ratio->DrawRatio(M12Data,M12MC,M12MCSyst,leg_pt1,canva04);
   ratio->GetTopPad()->SetLogy();
   ratioM12->GetXaxis()->SetRangeUser(200.,1000.);
   ratioM12->SetTitle(";di-jet Mass (GeV);Data / MC");
//   ratio->DrawPhaseSpaceDescription(240.,3,600.,30);
   canva04->SaveAs("pictures/M12.pdf");


   //........................Eta1 plot.............................
   TCanvas *canva10 = new TCanvas("canva10","Eta1",1000,800);

   TH1F *fEtaMC = new TH1F("fEtaMC","",30,-2.2,2.2);
   TH1F *fEtaMCSyst = new TH1F("fEtaMCSyst","",30,-2.2,2.2);
   TH1F *fEtaData = new TH1F("fEtaData",";leading b jet #eta; dN / d #eta",30,-2.2,2.2);
   fEtaData->GetXaxis()->SetRangeUser(-2.4,2.4);
   fEtaData->SetMinimum(10.);

   mcTree ->Draw("LeadEta[0]>>fEtaMCSyst",centralWeight,"E");
   mcTree ->Draw("LeadEta[0]>>fEtaMC",centralWeight,"E");
   dataTree ->Draw("LeadEta[0]>>fEtaData");

   SFbUp = (TH1F*) fEtaMC->Clone("Eta1SFbUp");
   SFbDown = (TH1F*) fEtaMC->Clone("Eta1SFbDown");
   Pt2D = (TH1F*) fEtaMC->Clone("Eta1Pt2D");
   PUup = (TH1F*) fEtaMC->Clone("Eta1PUup");
   PUdown = (TH1F*) fEtaMC->Clone("Eta1PUdown");

   mcTree ->Draw("LeadEta[0]>>Eta1SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("LeadEta[0]>>Eta1SFbDown",SFbWeightDown,"E");
   mcTree ->Draw("LeadEta[0]>>Eta1Pt2D",PtTriggerWeight,"E");
   mcTree ->Draw("LeadEta[0]>>Eta1PUup",PileUpWeightUp,"E");
   mcTree ->Draw("LeadEta[0]>>Eta1PUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=30;++i){
	   fEtaData->SetBinContent(i,fEtaData->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaData->SetBinError(i,fEtaData->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMC->SetBinContent(i,fEtaMC->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMC->SetBinError(i,fEtaMC->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   fEtaMCSyst->SetBinContent(i,fEtaMCSyst->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   fEtaMCSyst->SetBinError(i,fEtaMCSyst->GetBinError(i)/(fEtaData->GetBinWidth(20)));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(fEtaData->GetBinWidth(20)));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(fEtaData->GetBinWidth(20)));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(fEtaData->GetBinWidth(20)));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(fEtaMCSyst);

   TLegend *leg_pt3 = new TLegend(0.57,0.03,0.8,0.2);
   leg_pt3->AddEntry(fLeadPtData,"Data","p");
   leg_pt3->AddEntry(fLeadPtMC,mcName.c_str(),"p");

   TH1F *ratiofEta;
   ratiofEta = ratio->DrawRatio(fEtaData,fEtaMC,fEtaMCSyst,leg_pt1,canva10);
   ratiofEta->GetXaxis()->SetRangeUser(-2.4,2.4);
   ratio->DrawPhaseSpaceDescription(-1.45,10000,0.5,70000);
   canva10->SaveAs("pictures/eta1.pdf");

   //..............BTag Discriminant................

   TCanvas *canva05 = new TCanvas("canva05","BTagDiscr",1000,800);
   //Central values
   //Main Histograms
   TH1F *BTagDiscrMC = new TH1F("BTagDiscrMC","",30,0.935,1.);
   TH1F *BTagDiscrMCSyst = new TH1F("BTagDiscrMCSyst","",30,0.935,1.);
   TH1F *BTagDiscrData = new TH1F("BTagDiscrData",";BTagCSV discr. ; dN / dD_{BTagCSV} ",30,0.935,1.);
   BTagDiscrData->GetXaxis()->SetRangeUser(0.93,1.);

   mcTree ->Draw("LeadBTag[0]>>BTagDiscrMCSyst",centralWeight,"E");
   mcTree ->Draw("LeadBTag[0]>>BTagDiscrMC",centralWeight,"E");
   dataTree ->Draw("LeadBTag[0]>>BTagDiscrData");

   SFbUp = (TH1F*) BTagDiscrMC->Clone("BTag1SFbUp");
   SFbDown = (TH1F*) BTagDiscrMC->Clone("BTag1SFbDown");
   Pt2D = (TH1F*) BTagDiscrMC->Clone("BTag1Pt2D");
   PUup = (TH1F*) BTagDiscrMC->Clone("BTag1PUup");
   PUdown = (TH1F*) BTagDiscrMC->Clone("BTag1PUdown");

   mcTree ->Draw("LeadBTag[0]>>BTag1SFbUp",SFbWeightUp,"E");
   mcTree ->Draw("LeadBTag[0]>>BTag1SFbDown",SFbWeightDown,"E");
   mcTree ->Draw("LeadBTag[0]>>BTag1Pt2D",PtTriggerWeight,"E");
   mcTree ->Draw("LeadBTag[0]>>BTag1PUup",PileUpWeightUp,"E");
   mcTree ->Draw("LeadBTag[0]>>BTag1PUdown",PileUpWeightDown,"E");
   //Normalization:

   for(int i = 1;i<=30;++i){
	   BTagDiscrData->SetBinContent(i,BTagDiscrData->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrData->SetBinError(i,BTagDiscrData->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

	   BTagDiscrMC->SetBinContent(i,BTagDiscrMC->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrMC->SetBinError(i,BTagDiscrMC->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

	   BTagDiscrMCSyst->SetBinContent(i,BTagDiscrMCSyst->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   BTagDiscrMCSyst->SetBinError(i,BTagDiscrMCSyst->GetBinError(i)/(BTagDiscrData->GetBinWidth(10)));

	   SFbUp->SetBinContent(i,SFbUp->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   SFbDown->SetBinContent(i,SFbDown->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));

	   Pt2D->SetBinContent(i,Pt2D->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));

	   PUup->SetBinContent(i,PUup->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
	   PUdown->SetBinContent(i,PUdown->GetBinContent(i)/(BTagDiscrData->GetBinWidth(10)));
   }

   systCalc->CalculateSingleSystError(Pt2D);
   systCalc->CalculateSingleSystError(SFbUp,SFbDown);
   systCalc->CalculateSingleSystError(PUup,PUdown);
   systCalc->AddUncorelatedSystErrors(BTagDiscrMCSyst);

   TH1F *ratioBTagDiscr;
   leg_pt1->SetHeader("76X");
   ratioBTagDiscr = ratio->DrawRatio(BTagDiscrData,BTagDiscrMC,BTagDiscrMCSyst,leg_pt1,canva05);
   ratioBTagDiscr->GetXaxis()->SetRangeUser(0.93,1.);
   ratioBTagDiscr->SetTitle(";BTagCSV discr. ;Data / MC");
   ratio->DrawPhaseSpaceDescription(0.94,10000000,0.965,20000000);
   canva05->SaveAs("pictures/Btag1.pdf");

   /*

   // ..........Flavour Composition..........
   TCanvas *canva11 = new TCanvas("canva11","Flavour Comp",1000,800);
   TH1F *DataFlav = (TH1F*)M12Data->Clone("DataFlav");

   TH1F *cc = new TH1F("cc","",NumberM12Bins,M12Bins);
   TH1F *bb = new TH1F("bb","",NumberM12Bins,M12Bins);
   TH1F *qq = new TH1F("qq","",NumberM12Bins,M12Bins);
   TH1F *bc = new TH1F("bc","",NumberM12Bins,M12Bins);
   TH1F *bq = new TH1F("bq","",NumberM12Bins,M12Bins);
   TH1F *qc = new TH1F("qc","",NumberM12Bins,M12Bins);

   mcTree ->Draw("ObjM12>>cc",centralWeight*"cc==1","E");
   mcTree ->Draw("ObjM12>>bb",centralWeight*"bb==1","E");
   mcTree ->Draw("ObjM12>>qq",centralWeight*"qq==1","E");
   mcTree ->Draw("ObjM12>>bc",centralWeight*"bc==1","E");
   mcTree ->Draw("ObjM12>>bq",centralWeight*"bq==1","E");
   mcTree ->Draw("ObjM12>>qc",centralWeight*"qc==1","E");

   std::vector<TH1F*> flavours;
   flavours.push_back(bb);
   flavours.push_back(cc);
   flavours.push_back(qq);
   flavours.push_back(bc);
   flavours.push_back(bq);
   flavours.push_back(qc);

   for(int i = 1; i<=NumberM12Bins;++i){
	   for(const auto &ih : flavours ){
		   ih->SetBinContent(i,ih->GetBinContent(i) / (M12Bins[i]-M12Bins[i-1]));
	   }
   }

   THStack *flavourComposition = new THStack("flavourComposition","");
   flavourComposition->SetTitle(";di-jet Mass (GeV); dN / d(di-jet Mass) (1/GeV)");
   //flavourComposition->GetXaxis()->SetRangeUser(0.,1000.);

   TLegend *flavourCompLegend = new TLegend(0.6,0.65,0.9,0.85);

   int colors[]= {kCyan+2,kYellow-6,kMagenta+2,kRed+2,kYellow+2,kGreen+2,kBlue-6};
   int counter = 0;
   double Flavourfraction[10];
   char name[200];
   for(const auto &ih : flavours ){
	   ih->SetFillColor(colors[counter]);
	   ih->SetLineColor(colors[counter]);
	   flavourComposition->Add(ih);

	   counter ++;
   }

   counter = 0;
   flavourCompLegend->AddEntry(DataFlav,"Data","p");
   for(const auto &ih : flavours ){
	   Flavourfraction[counter] = ih->Integral()/ ((TH1*)flavourComposition -> GetStack() -> Last()) -> Integral();
	   sprintf(name,"%s, %.2f",(ih->GetName()),Flavourfraction[counter]);
	   flavourCompLegend->AddEntry(ih,name,"la");
	   counter ++;
   }

   flavourComposition->Draw("hist");
   DataFlav->Draw("same");
   flavourCompLegend->Draw();

   //..............bb/ALL..............
   TCanvas *canva12 = new TCanvas("canva12","",1000,800);
   TH1F *bb_all = (TH1F*) bb -> Clone("bb_all");
   bb_all->Divide((TH1*)flavourComposition -> GetStack() -> Last());
   bb_all->SetMarkerStyle(20);
   bb_all->Draw();

/**/
}

double M12FitFunction(double *x, double *par){
	return par[1]*pow((1-pow(x[0],par[0])),par[2]);
	//return exp(x[0]);
}


