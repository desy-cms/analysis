int TotalRatesData(const std::string & trigger)
{
   double zbRate = 24750000;
   
   std::vector<TFile *> f;
   std::vector<TGraphAsymmErrors *> g_eff;
   std::vector<TGraphAsymmErrors *> g_rate;
   
   f.push_back(new TFile("../test/mssmhbb_triggers_data_parking_zerobias0.root","old"));
  
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
         effErrH[j]  = g_eff.back()->GetErrorYhigh(j);
         effErrL[j]  = g_eff.back()->GetErrorYlow(j);
         rate[j]     = eff[j] * zbRate;
         rateErrH[j] = effErrH[j] * zbRate;
         rateErrL[j] = effErrL[j] * zbRate;
         
         totRate[j] += rate[j];
         totRateErrH[j] += rateErrH[j]*rateErrH[j];
         totRateErrL[j] += rateErrL[j]*rateErrL[j];
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
   
// L1 functions   
   TF1 * f1 = new TF1("f1","217 + 98.7*x + 0.381*x*x",0,65);
   f1 -> SetLineColor(4);
   
   TCanvas * c1 = new TCanvas("c1","",1000,500);
   
   g_totRate -> Draw("AP");
   
   g_totRate -> Fit("pol2","","",5,47);
   
   f1 -> Draw("same");
   
   
   c1 -> SaveAs(Form("Rates_Data_%s.png",trigger.c_str()));
   
   TFile * out = new TFile(Form("data_%s.root",trigger.c_str()),"recreate");
   g_totRate -> Write();
   for ( size_t i = 0 ; i < f.size() ; ++i )  // loop over files
   {
      g_rate[i] -> Write();
   }
   
   out -> Close();
   
   return 0;
   
   
   
}
