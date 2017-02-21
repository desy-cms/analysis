int TotalRatesMC(const std::string & trigger)
{
   double nBX = 2200;
   double mBX = 3564;
   double tBX = 24.95E-9;
   double fBX = (nBX/mBX)/tBX;
   double sigmaMB = 80.E9; // minimum bias cross section 80 mb
   double myPU = 45;
   
   
   std::vector<TFile *> f;
   std::vector<TGraphAsymmErrors *> g_eff;
   std::vector<TGraphAsymmErrors *> g_rate;
   
//    f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_15to30.root","old"));
    f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_30to50.root","old"));
    f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_50to80.root","old"));
   f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_80to120.root","old"));
   f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_120to170.root","old"));
   f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_170to300.root","old"));
   f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_300to470.root","old"));
   f.push_back(new TFile("../test/mssmhbb_triggers_QCD_Pt_470to600.root","old"));
    
   
   std::vector<double> totRate;
   std::vector<double> totRateErrH;
   std::vector<double> totRateErrL;
   std::vector<double> totPU;
   std::vector<double> totPUErr;
   
   for ( size_t i = 0 ; i < f.size() ; ++i )  // loop over files
   {
      TH1F * h_total    = (TH1F*) f[i]->Get("h_nZeroBias");
      TH1F * h_selected = (TH1F*) f[i]->Get(Form("h_n%s",trigger.c_str()));
      
      const char * sampleName = ((TNamed*) f[i] -> Get("SampleName"))->GetTitle();
      double xsection = (*((TVectorD*) f[i] -> Get("xsection")))[0];
   
      g_eff.push_back(new TGraphAsymmErrors(h_selected,h_total,"cl=0.683 b(1,1) mode"));
      g_eff.back()->SetName(Form("Efficiency_%s",sampleName));
      
      // get sizes
      int gsize        = g_eff.back() -> GetN();
      double * pu      = g_eff.back() -> GetX();
      std::vector<double> puErr(gsize);
      double * eff     = g_eff.back() -> GetY();
      std::vector<double> effErrH(gsize);
      std::vector<double> effErrL(gsize);
      std::vector<double> rate(gsize);
      std::vector<double> rateErrH(gsize);
      std::vector<double> rateErrL(gsize);
      
      if ( i == 0 ) // adjust size of total rates, i.e. rates sum of all pt-hat bins
      {
         totPU.assign(pu,pu+gsize);
         totPUErr = puErr;
         totRate.resize(gsize);
         totRateErrH.resize(gsize);
         totRateErrL.resize(gsize);
      }
      
      for ( int j = 0; j < gsize ; ++j )
      {
         effErrH[j] = g_eff.back()->GetErrorYhigh(j);
         effErrL[j] = g_eff.back()->GetErrorYlow(j);
         double mu_i = xsection/sigmaMB*pu[j];
         rate[j] = fBX*(1-exp(-mu_i*eff[j]));
         rateErrH[j] = fBX*mu_i*exp(-mu_i*eff[j])*effErrH[j];
         rateErrL[j] = fBX*mu_i*exp(-mu_i*eff[j])*effErrL[j];
         
         totRate[j] += rate[j];
         totRateErrH[j] += rateErrH[j]*rateErrH[j];
         totRateErrL[j] += rateErrL[j]*rateErrL[j];
         if ( pu[j] == 45 )
      std::cout << sampleName << "   rate = " << rate[j] << " + " << rateErrH[j] << " - " << rateErrL[j] << std::endl;
      }
      
      g_rate.push_back(new TGraphAsymmErrors(gsize,pu,&rate[0],&puErr[0],&puErr[0],&rateErrL[0],&rateErrL[0]));
      g_rate.back() -> SetName(Form("rate_%s",sampleName));
      
      
   }
   for ( int j = 0; j <  int(totRate.size()); ++j )
   {
      totRateErrH[j] = sqrt(totRateErrH[j]);
      totRateErrL[j] = sqrt(totRateErrL[j]);
      std::cout << "Pile up = " << totPU[j] << "   rate = " << totRate[j] << " + " << totRateErrH[j] << " - " << totRateErrL[j] << std::endl;
   }
   TGraphAsymmErrors * g_totRate = new TGraphAsymmErrors(int(totRate.size()),&totPU[0],&totRate[0],&totPUErr[0],&totPUErr[0],&totRateErrL[0],&totRateErrH[0]);
   
   g_totRate -> SetName("rate_total");
   
   double * y = g_totRate -> GetY();
   double ymax = *std::max_element(y,y+int(totRate.size()));
   
   g_totRate -> SetMarkerStyle(20);
   
   g_totRate -> GetXaxis() -> SetTitle("pile up");
   g_totRate -> GetXaxis() -> SetTitleSize(0.05);
   g_totRate -> GetXaxis() -> SetLabelSize(0.05);
   
   g_totRate -> GetYaxis() -> SetTitle("rate (Hz)");
   g_totRate -> GetYaxis() -> SetTitleSize(0.05);
   g_totRate -> GetYaxis() -> SetLabelSize(0.05);
   g_totRate -> GetYaxis() -> SetRangeUser(0,ymax*1.1);
   
   g_totRate -> SetTitle("");
   
   
   TCanvas * c1 = new TCanvas("c1","",1000,500);
   
   g_totRate -> Draw("AP");
   
   c1 -> SaveAs(Form("Rates_%s.png",trigger.c_str()));
   
   TFile * out = new TFile(Form("%s.root",trigger.c_str()),"recreate");
   g_totRate -> Write();
   for ( size_t i = 0 ; i < f.size() ; ++i )  // loop over files
   {
      g_rate[i] -> Write();
   }
   
   out -> Close();
   
   return 0;
   
   
   
}
