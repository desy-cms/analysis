#include "map"
#include "TFile.h"
#include "iomanip"
#include "stdlib.h"
#include "TText.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TPad.h"
#include <boost/lexical_cast.hpp>

using namespace std;


int test2(){

	std::map<int,TFile*> f;
	f[300] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_171815/0000/output_2.root");
	f[400] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172015/0000/output_2.root");
	f[500] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172041/0000/output_2.root");
//	f[600] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172112/0000/output_2.root");
//	f[700] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172143/0000/output_2.root");
//	f[900] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616/SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160615_233835/0000/output_2.root");
//	f[1100] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160615_234113/0000/output_2.root");
//	f[1300] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616/SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160615_234219/0000/output_2.root");

	//Reconstructed
	map<int,TFile *> reco;
	reco[300] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8.root");
	reco[400] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8.root");
	reco[500] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8.root");
//	reco[600] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8.root");
//	reco[700] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8.root");
//	reco[900] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8.root");
//	reco[1100] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8.root");
//	reco[1300] = new TFile("../output/MssmHbbSignal_highM_SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8.root");


	std::string mass = "pdg == 36 && (6500. * TMath::Sqrt( (pdfx1 +pdfx2) * (pdfx1 +pdfx2) - (pdfx1-pdfx2)*(pdfx1-pdfx2)) )";

	map<int, TH1D*> h_reco;
	map<int, TH1D*> h_gen;
	map<int, TH1D*> h_gen_j;

	TCanvas *c[2];
	c[0] = new TCanvas("c[0]","bl",1000,800);
	c[0]->Divide(2,2);
	c[1] = new TCanvas("c[1]","bl2",1000,800);
	c[1]->Divide(2,2);
	int count = -1;

	TText *text[8];
	TLegend *leg[8];

	for(const auto fi : f){
		++count;
		leg[count-1] = new TLegend(0.65,0.5,0.9,0.8);
		leg[count-1]->SetBorderSize(0);
		text[count] = new TText(0.5,0.5,"lol");
		text[count]->SetNDC(1);
		TTree *gen, *ev, *gen_j, *reco_j;
		fi.second->GetObject("MssmHbb/Events/prunedGenParticles",gen);
		fi.second->GetObject("MssmHbb/Events/EventInfo",ev);
		fi.second->GetObject("MssmHbb/Events/slimmedGenJets",gen_j);
		fi.second->GetObject("MssmHbb/Events/slimmedJetsReapplyJEC",reco_j);
		gen->AddFriend(ev);
		gen->AddFriend(gen_j);

		h_reco[fi.first] = (TH1D*) reco[fi.first]->Get("bbH_Mbb");
		h_reco[fi.first]->SetMarkerStyle(20);
		h_gen[fi.first] = (TH1D*) h_reco[fi.first]->Clone(("h_gen_" + to_string(fi.first)).c_str());
		h_gen[fi.first]->SetLineColor(2);
		h_gen_j[fi.first] = (TH1D*) h_reco[fi.first]->Clone(("h_gen_j_" + to_string(fi.first)).c_str());
		h_gen_j[fi.first]->SetLineColor(65);
		cout<<fi.first<<std::endl;
		if(count<4){
			c[0]->cd(count+1);
//			gPad->SetLogy();
//			gPad->SetLogx();
//			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)",(mass + "> 0.7 *" + boost::lexical_cast<std::string>(fi.first)).c_str(),"E");
//			h_gen[fi.first] = new TH1D(("h_gen_" + to_string(fi.first)).c_str(),"",50,0,fi.first*1.5);
			gen->Draw(("TMath::Sqrt(e*e-px*px-py*py-pz*pz) >> h_gen_" + to_string(fi.first)).c_str(),"TMath::Sqrt(e*e-px*px-py*py-pz*pz) > 100  && pdg == 36","histo");
//			gen->Draw( ("TMath::Sqrt( e[0]*e[0] - px[0]*px[0] - py[0]*py[0] - pz[0]*pz[0] + e[1]*e[1] - px[1]*px[1] - py[1]*py[1] - pz[1]*pz[1] + 2*(e[0]*e[1] - px[0]*px[1] - py[0]*py[1] - pz[0]*pz[1])) >> h_gen_j_" + to_string(fi.first)).c_str(),"","histo same" );
			h_reco[fi.first]->Draw("E same");
		}
		else{
			c[1]->cd(count+1 - 4);
//			gPad->SetLogy();
//			gPad->SetLogx();
//			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)",(mass + "> 0.7 *" + boost::lexical_cast<std::string>(fi.first)).c_str(),"E");
			gen->Draw(("TMath::Sqrt(e*e-px*px-py*py-pz*pz) >> h_gen_" + to_string(fi.first)).c_str()," TMath::Sqrt(e*e-px*px-py*py-pz*pz) > 100 && pdg == 36","histo");
//			gen->Draw( ("TMath::Sqrt(2*(e[0]*e[1] - px[0]*px[1] - py[0]*py[1] - pz[0]*pz[1])) >> h_gen_j_" + to_string(fi.first)).c_str(),"","histo same" );
			h_reco[fi.first]->Draw("E same");

		}
		h_reco[fi.first]->Scale(1./h_reco[fi.first]->Integral());
		h_gen[fi.first] ->Scale(1./h_gen[fi.first]->Integral());
//		h_gen_j[fi.first] ->Scale(1./h_gen_j[fi.first]->Integral());
		leg[count-1]->AddEntry(h_reco[fi.first],("Reco Jets, M = " + to_string(fi.first)).c_str(),"p");
		leg[count-1]->AddEntry(h_gen[fi.first],("Gen Part. , M = " + to_string(fi.first)).c_str(),"l");
//		leg[count-1]->AddEntry(h_gen_j[fi.first],("Gen Jets, M = " + to_string(fi.first)).c_str(),"l");
		leg[count-1]->Draw();
	}
/*

	TTree *Gen300, *Ev300;
	f300.GetObject("MssmHbb/Events/prunedGenParticles",Gen300);
	f300.GetObject("MssmHbb/Events/EventInfo",Ev300);
	Gen300->AddFriend(Ev300);



//	tree300->SetAlias("gen","prunedGenParticles");
//	tree300->SetAlias("evInfo","EventInfo");

//	tree300->Draw("TMath::Sqrt(prunedGenParticles.e()*e-px*px-py*py-pz*pz)");
	std::string mass = "pdg == 36 && (6500. * TMath::Sqrt( (pdfx1 +pdfx2) * (pdfx1 +pdfx2) - (pdfx1-pdfx2)*(pdfx1-pdfx2)) )";
	Gen300->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)",(mass + "> 0.7 * 300").c_str());


	/*
	gStyle->SetOptStat(0);
	TFile f("/pnfs/desy.de/cms/tier2/store/mc/RunIIFall15DR76/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/AODSIM/PU25nsData2015v1Raw_76X_mcRun2_asymptotic_v12-v1/00000/32A0AE1D-28C5-E511-BE07-782BCB783902.root");

	TTree *Events;
	f.GetObject("Events",Events);

	Events->SetAlias("genParticles","recoGenParticles_genParticles__HLT.obj");
	Events->SetAlias("genEventInfo","GenEventInfoProduct_generator__SIM.obj");

	Events->Draw("genParticles.mass()","genParticles.pdgId() == 36 && (6500.*TMath::Sqrt( (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) * (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) - (genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second)*(genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second) ) >= 0.7*300 )");
	*/


	return 0;
}
