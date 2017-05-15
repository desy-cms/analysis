#include "Analysis/MssmHbb/macros/Drawer/HbbStyle.cc"
#include "Analysis/MssmHbb/macros/Drawer/RatioPlots.cpp"

int MCvsMC(){

	gROOT -> Reset();

   TFile * fOldVersion = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_lowM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
	if(fOldVersion->IsZombie()){
		std::cerr<<"Error opening file with old version: "<<fOldVersion->GetName()<<std::endl;
		exit(-1);
	}

   TFile * fNewVersion = new TFile("/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/output/MssmHbbSignal_oldGT_lowM_chayanit-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root");
	if(fNewVersion->IsZombie()){
		std::cerr<<"Error opening file with new version: "<<fNewVersion->GetName()<<std::endl;
		exit(-1);
	}

	//Read all objects from Bg templates file
	std::vector<std::string> vars = {"diJet_eta","diJet_m","diJet_pt","jet_dR12","jet_eta1","jet_pt1","jet_pt2"};
	std::vector<std::string> folders = {"distributions/"};
	std::map<std::string,TH1D*> hOld;
	std::map<std::string,TH1D*> hNew;
	std::map<std::string, TCanvas*> c;
	std::map<std::string, TLegend*> l;

	for(const auto & v : vars){
		std::string name = folders.at(0) + v;

		hOld[v] = (TH1D*) fOldVersion->Get(name.c_str())->Clone();
		hNew[v] = (TH1D*) fNewVersion->Get(name.c_str())->Clone();
		hOld[v]->Scale(1./hOld[v]->Integral());
		hNew[v]->Scale(1./hNew[v]->Integral());
		c[v] = new TCanvas( v.c_str(),v.c_str(),1000,800);
//		if(v.find("pt") != std::string::npos || v.find("m") != std::string::npos) gPad->SetLogy();
		l[v] = new TLegend(0.6,0.5,0.8,0.65);
		l[v]->AddEntry(hOld[v],"Old MC","l");
		l[v]->AddEntry(hNew[v],"New MC","lp");

		hOld[v]->SetLineColor(2);
		hOld[v]->Draw("E");

		c[v]->Update();
		TPaveStats *ps2 = (TPaveStats*)hOld[v]->GetListOfFunctions()->FindObject("stats");
		ps2->SetX1NDC(0.8); ps2->SetX2NDC(0.99);
		ps2->SetLineColor(2);

		hNew[v]->SetMarkerStyle(20);
		hNew[v]->SetStats(kTRUE);
		hNew[v]->Draw("E");
		c[v]->Update();
		TPaveStats *ps1 = (TPaveStats*)hNew[v]->GetListOfFunctions()->FindObject("stats");
		ps1->SetX1NDC(0.6); ps1->SetX2NDC(0.8);
		c[v]->Modified();

		hOld[v]->Draw("hist");
		hNew[v]->Draw("E same");
		l[v]->Draw();

		c[v]->Print((v + ".pdf").c_str());

	}

	return 0;
}
