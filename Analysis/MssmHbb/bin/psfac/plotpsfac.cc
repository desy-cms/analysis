#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"

void plotpsfac() {
  // output file
  TFile* hout = new TFile("psfacDATA_dEta1p55.root","recreate");
  hout->cd();
  TH2::AddDirectory(true);

  TFile* f[2];
  f[0] = new TFile("MC15_NoPtCut_50to170.root");
  f[1] = new TFile("MC15_PtCut_50to170.root");
  std::map < std::string, TH1F *> h1[3];
  std::map < std::string, TH2F *> h2[3];
  // h1["h_jet_N"] = (TH1F*) f->Get("h_jet_N");
  for (int ips = 0; ips<2; ++ips) {
    // h1[ips]["h_jet_N"] = NULL;
    h1[ips]["h_jet1_Pt"]       = NULL;
    // h1[ips]["h_jet1_Eta"]      = NULL; 
    // h1[ips]["h_jet1_BTag"]     = NULL; 
	
    h1[ips]["h_jet2_Pt"]       = NULL;
    // h1[ips]["h_jet2_Eta"]      = NULL; 
    // h1[ips]["h_jet2_BTag"]     = NULL; 
  	
    h1[ips]["h_obj_M12"]         = NULL; 

//    h1[ips]["h_jet1_hM_Pt"]         = NULL;
//    h1[ips]["h_jet2_hM_Pt"]         = NULL;
    // h1[ips]["h_obj_Pt"]        = NULL; 
    // h1[ips]["h_obj_Eta"]       = NULL; 
	
    // h1[ips]["h_djet_Pt12"]     = NULL; 
    // h1[ips]["h_djet_Eta12"]    = NULL; 
    // h1[ips]["h_djet_Phi12"]    = NULL; 
    // h1[ips]["h_djet_PtTOT"]    = NULL;

    // loop over the histograms
    for (std::map < std::string, TH1F *>::iterator mit=h1[ips].begin();
	 mit != h1[ips].end(); ++mit) {
      (*mit).second = (TH1F*) f[ips]->Get((*mit).first.c_str());
      std::cout << "For " << (*mit).first.c_str() 
		<< " Pointer " << (*mit).second << " ips=" << ips << std::endl;
      if ((*mit).second == NULL) {
	std::cout << "Bad pointer for histogram " << (*mit).first << std::endl;
	continue;
      }
      (*mit).second->Rebin(4);
      (*mit).second->SetName( Form("%sips%d",(*mit).second->GetName(),ips) );
      std::cout << "Renamed to " << (*mit).second->GetName() << std::endl;
    }
  }

  // plot and print
  for (std::map < std::string, TH1F *>::iterator mit=h1[0].begin();
       mit != h1[0].end(); ++mit) {
    string hname = (*mit).first;
    TH1F* hDenom = (*mit).second;  // this is the denominator histogram

    // now make a ratio plot
    h1[2][hname] = (TH1F*) h1[1][hname]->Clone();
    h1[2][hname]->SetName( Form("%s_Ratio",h1[1][hname]->GetName()) );
    h1[2][hname]->Divide( h1[0][hname] );
    for (int ips=0;ips<3;++ips) {
      if (hname == "h_obj_M12") {
	h1[ips][hname]->GetXaxis()->SetRange(1,100);
	h1[ips][hname]->GetXaxis()->SetTitle("M_{12}");
      }
      else if (hname == "h_jet1_Pt" || hname == "h_jet2_Pt") {
	h1[ips][hname]->GetXaxis()->SetRange(6,60);
	h1[ips][hname]->GetXaxis()->SetTitle("p_{T}");
      }
    }
    // adjust the errors to account for correlations
    for (int ibin=1; ibin<=h1[2][hname]->GetXaxis()->GetNbins(); ++ibin){
      double A = h1[0][hname]->GetBinContent(ibin); // contents of "all"
      double EA = h1[0][hname]->GetBinError(ibin);  // errors of "all"
      double P = h1[1][hname]->GetBinContent(ibin); // contents of "passed"
      double EP = h1[1][hname]->GetBinError(ibin);  // errors of "passed"
      double radicant = (A-P)*(A-P) * EP*EP 
	+ P*P * (EA*EA - EP*EP);
      double E_Eff = 0;
      if (radicant>0 && A>0) E_Eff = sqrt( radicant) / (A*A);
      h1[2][hname]->SetBinError(ibin,E_Eff);
    }
    h1[2][hname]->SetMarkerStyle(20);

    // draw numerator and denominator in the right ranges
    hDenom->SetMarkerStyle(20);
    hDenom->Draw("");
    std::cout << "Lookup hname=" << hname << " numerator " << std::endl; 
    h1[1][hname]->Draw("SAME,HIST");  // this is the numerator histogram
    std::cout << h1[1][hname] << std::endl;
    // h1[1][hname]->Draw("HIST");
    gPad->Print(Form("%s.png",hname.c_str()));
    gPad->SetLogy(1);
    gPad->Print(Form("%s_Log.png",hname.c_str()));
    gPad->SetLogy(0);

    h1[2][hname]->Draw("");
    gPad->Print(Form("%s_Ratio.png",hname.c_str()));
    // store the ratio root file
    hout->cd();
    h1[2][hname]->Write();
  }
}
