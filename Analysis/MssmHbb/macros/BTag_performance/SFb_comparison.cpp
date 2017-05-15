#include "TSystem.h"

#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TF1.h"

#include "Analysis/Tools/interface/Jet.h"
#include "Analysis/MssmHbb/interface/BTagScaleFactor.h"

using namespace std;
using namespace analysis::tools;

BTagScaleFactor::ScaleFactor combineBTagSFs(const std::vector<BTagScaleFactor::ScaleFactor> & sfs, const std::vector<double>& lumis, const double& lumi_tot);
int main(){

	const string cmsswBase = getenv("CMSSW_BASE");

	string sICHEP17		 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/SFbLib.csv";
	string sMoriond17_G_H 	 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/CSVv2_Moriond17_G_H.csv";
	string sMoriond17_B_F 	 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/CSVv2_Moriond17_B_F.csv";
	vector<string> csvs = {sMoriond17_B_F,sMoriond17_G_H};
	std::vector<double> lumis = {20233, //  B-F
								 16382}; // G-H
	const double eta = 1.0;
	const int flavour = 5;
	const int npoints = 98;
	const int operatio_point = 1; //for medium;

	vector<double> pts; for(auto i = 3; i <= npoints + 2; ++i) pts.push_back( i*10);
	vector<Jet> Jets;
	for(auto i = 0; i < npoints; ++i){
		Jet j(pts.at(i),eta,0,10);
		j.flavour(flavour);
		Jets.push_back(j);
	}

	TGraphAsymmErrors *grMoriond17 = new TGraphAsymmErrors(npoints);
	TGraphAsymmErrors *grICHEP17 = new TGraphAsymmErrors(npoints);
	TGraph *grICHEP17_mid = new TGraph(npoints);
	BTagScaleFactor sfMoriond17, sfICHEP17;
	sfMoriond17.Setup("csvv2",csvs);
	sfICHEP17.Setup("csvv2",sICHEP17);

	//Calculate SFs
	for(auto i = 0; i < npoints; ++i){
		Jet j = Jets.at(i);
		vector<BTagScaleFactor::ScaleFactor> vmor 	= sfMoriond17.calculateBTagSF(j,operatio_point);
		BTagScaleFactor::ScaleFactor mor = combineBTagSFs(vmor,lumis,16382. + 20233.);
		BTagScaleFactor::ScaleFactor ichep 	= sfICHEP17.calculateBTagSF(j,operatio_point).at(0);

		grMoriond17->SetPoint(i,j.pt(),mor.central);
		grMoriond17->SetPointError(i,0,0,mor.central - mor.down,mor.up - mor.central);

		grICHEP17->SetPoint(i,j.pt(),ichep.central);
		grICHEP17->SetPointError(i,0,0,ichep.central - ichep.down,ichep.up - ichep.central);
		grICHEP17_mid->SetPoint(i,j.pt(),ichep.central);

		cout<<"Moriond17: central = "<<mor.central<<" down = "<<mor.central - mor.down<<" up = "<<mor.up - mor.central<<endl;
		cout<<"ICHEP17: central = "<<ichep.central<<" down = "<<ichep.central - ichep.down<<" up = "<<ichep.up - ichep.central<<endl;
	}

	TCanvas *can = new TCanvas("can","can",800,600);

	double ymin = grMoriond17->GetYaxis()->GetXmin() - 0.2 * grMoriond17->GetYaxis()->GetXmin();//0.7;//
	double ymax = grMoriond17->GetYaxis()->GetXmax() + 0.2 * grMoriond17->GetYaxis()->GetXmax();//1.3;//

	TH2F *frame = new TH2F("frame","",2,0,1000,2,ymin,ymax);
	string title = "flavour ";
	if(flavour == 5) title += "b";
	else if(flavour == 4) title += "c";
	else title += "udsg";
	frame->SetTitle(title.c_str());
	frame->GetXaxis()->SetTitle("p_{T}, [GeV]");
	frame->GetYaxis()->SetTitle("(Data/Monte Carlo) scale factors");
	frame->GetXaxis()->SetNdivisions(505);
	frame->GetYaxis()->SetNdivisions(206);
	frame->GetYaxis()->SetTitleOffset(1.1);
	frame->GetXaxis()->SetTitleOffset(1.05);
	frame->GetYaxis()->SetTitleSize(0.048);
	frame->SetStats(0);
	frame->Draw();

	grICHEP17->SetFillStyle(3002);
	grICHEP17->SetFillColor(kRed-10);
	grMoriond17->SetMarkerStyle(20);
	grMoriond17->SetMarkerSize(1.2);
	grICHEP17_mid->SetMarkerStyle(21);
	grICHEP17_mid->SetMarkerColor(kRed-10);
	grICHEP17_mid->SetMarkerSize(1.2);
	grICHEP17->Draw("3same");
	grMoriond17->Draw("Psame");
	grICHEP17_mid->Draw("Psame");

	can->Print( (cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/SFb_flavour_" + to_string(flavour) + "_OP_"+ to_string(operatio_point) + ".pdf").c_str() );

	return 0;
}

BTagScaleFactor::ScaleFactor combineBTagSFs(const std::vector<BTagScaleFactor::ScaleFactor> & sfs, const std::vector<double>& lumis, const double& lumi_tot){
//	Method to combine SFs from different jets(virtual)

	BTagScaleFactor::ScaleFactor sf;
	double SF_central = 0, eSF_up = 0, eSF_down = 0;
	for(auto j = 0; j != int(sfs.size()); ++j){
		SF_central 	+= sfs.at(j).central * lumis.at(j) / lumi_tot;
		eSF_up 		+= (sfs.at(j).up) * lumis.at(j) / lumi_tot;
		eSF_down	+= (sfs.at(j).down) * lumis.at(j) / lumi_tot;
	}
	sf.central 	= SF_central;
	sf.up 		= eSF_up;
	sf.down		= eSF_down;

	return sf;
}
