void TotalRates()
{
   double nBX = 2200;
   double mBX = 3564;
   double tBX = 24.95E-9;
   double fBX = (nBX/mBX)/tBX;
   double sigmaMB = 80.E9; // minimum bias cross section 80 mb
   double myPU = 45;
   
   
   std::vector<TFile *> f;
   std::vector<TGraphAsymmErrors *> g_eff;
   std::vector<TGraphErrors *> g_rate;
   
   
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_15to30.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_30to50.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_50to80.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_80to120.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_120to170.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_170to300.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_300to470.root","old"));
    f.push_back(new TFile("../test/l1rates_mod_QCD_Pt_470to600.root","old"));
   
   std::vector<double> totRate;
   std::vector<double> totRateErr;
   std::vector<double> totPU;
   std::vector<double> totPUErr;
   
   for ( size_t i = 0 ; i < f.size() ; ++i )
   {
      TH1F * h_total    = (TH1F*) f[i]->Get("h_nTotal");
      TH1F * h_selected = (TH1F*) f[i]->Get("h_nSelected");
      const char * sampleName = ((TNamed*) f[i] -> Get("SampleName"))->GetTitle();
      double xsection = (*((TVectorD*) f[i] -> Get("xsection")))[0];
      
      g_eff.push_back(new TGraphAsymmErrors(h_selected,h_total,"cl=0.683 b(1,1) mode"));
      g_eff.back()->SetName(Form("Efficiency_%s",sampleName));
      int gsize = g_eff.back() -> GetN();
      double * pu      = g_eff.back()->GetX();
      std::vector<double> puErr(gsize);
      double * eff     = g_eff.back()->GetY();
      std::vector<double> effErr(gsize);
      std::vector<double> rate(gsize);
      std::vector<double> rateErr(gsize);
      
//      std::cout << sampleName << "   " << xsection << std::endl;
      
      if ( i == 0 )
      {
         totPU.assign(pu,pu+gsize);
         totPUErr = puErr;
         totRate.resize(gsize);
         totRateErr.resize(gsize);
      }
      
      for ( int j = 0; j < gsize ; ++j )
      {
         effErr[j] = g_eff.back()->GetErrorYhigh(j);
         double mu_i = xsection/sigmaMB*pu[j];
         rate[j] = fBX*(1-exp(-mu_i*eff[j]));
         rateErr[j] = fBX*mu_i*exp(-mu_i*eff[j])*effErr[j];
//         std::cout << "Pile up = " << pu[j] << "   rate = " << rate[j] << " +- " << rateErr[j] << std::endl;
         
         if ( pu[j] == 45 ) 
            printf ("%40s \t %7.2f \t %7.2f  \n", sampleName, rate[j], rateErr[j] );
//            std::cout << sampleName << "\t" << rate[j] << "\t" << rateErr[j] << std::endl;
         
         totRate[j] += rate[j];
         totRateErr[j] += rateErr[j]*rateErr[j];
      }
      
      g_rate.push_back(new TGraphErrors(gsize,pu,&rate[0],&puErr[0],&rateErr[0]));
      
   }
   for ( int j = 0; j <  int(totRate.size()); ++j )
   {
      totRateErr[j] = sqrt(totRateErr[j]);
      std::cout << "Pile up = " << totPU[j] << "   rate = " << totRate[j] << " +- " << totRateErr[j] << std::endl;
   }
   TGraphErrors * g_totRate = new TGraphErrors(int(totRate.size()),&totPU[0],&totRate[0],&totPUErr[0],&totRateErr[0]);
   
   g_totRate -> SetMarkerStyle(20);
   
   g_totRate -> GetXaxis() -> SetTitle("pile up");
   g_totRate -> GetXaxis() -> SetTitleSize(0.05);
   g_totRate -> GetXaxis() -> SetLabelSize(0.05);
   
   g_totRate -> GetYaxis() -> SetTitle("rate (Hz)");
   g_totRate -> GetYaxis() -> SetTitleSize(0.05);
   g_totRate -> GetYaxis() -> SetLabelSize(0.05);
   g_totRate -> GetYaxis() -> SetRangeUser(0,15000);
//   g_totRate -> GetYaxis() -> SetRangeUser(0,130);
   
   
   g_totRate -> SetTitle("");
   
// L1 functions   
   TF1 * f1 = new TF1("f1","217 + 98.7*x + 0.381*x*x",20,65);
   f1 -> SetLineColor(4);
   TF1 * f2 = new TF1("f2","-1.23e+3 + 255*x -5.01*x*x + 0.0594*x*x*x",20,65);
   f2 -> SetLineColor(6);
   
// HLT functions  
//    TF1 * f11 = new TF1("f11","[0]*x +[1]*x*x",20,65);
//    double factor = fBX/(80000);
//    f11 -> SetParameter(0,0.00107*factor);
//    f11 -> SetParameter(1,1.02e-8*factor*factor);
//    f11 -> SetLineColor(4);
//    
//    TF1 * f21 = new TF1("f21","[0]*x +[1]*x*x + [2]*x*x*x",20,65);
//    f21 -> SetParameter(0,0.00104*factor);
//    f21 -> SetParameter(1,1.71e-8*factor*factor);
//    f21 -> SetParameter(2,-3.74e-13*factor*factor*factor);
//    f21 -> SetLineColor(6);
   
   TCanvas * c1 = new TCanvas("c1","",600,500);
   
   g_totRate -> Draw("AP");
   
   g_totRate -> Fit("pol2");
   
  f1 -> Draw("same");
  f2 -> Draw("same");
   
//    f21 -> Draw("same");
//    f11 -> Draw("same");
   
   c1 -> SaveAs("l1rates.png");
   
   
   
   
   
   
}
