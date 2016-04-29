#include <boost/lexical_cast.hpp>

using namespace std;


int test2(){

	std::map<int,TFile*> f;
	f[300] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085505/0000/output_2.root");
	f[400] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-400_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085528/0000/output_2.root");
	f[500] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085537/0000/output_2.root");
	f[600] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-600_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085546/0000/output_2.root");
	f[700] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-700_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085556/0000/output_2.root");
	f[900] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-900_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085608/0000/output_2.root");
	f[1100] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085617/0000/output_2.root");
	f[1300] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160421/SUSYGluGluToBBHToBB_M-1300_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160422_085628/0000/output_2.root");

	std::string mass = "pdg == 36 && (6500. * TMath::Sqrt( (pdfx1 +pdfx2) * (pdfx1 +pdfx2) - (pdfx1-pdfx2)*(pdfx1-pdfx2)) )";

	TCanvas *c[2];
	c[0] = new TCanvas("c[0]","bl",1000,800);
	c[0]->Divide(2,2);
	c[1] = new TCanvas("c[1]","bl2",1000,800);
	c[1]->Divide(2,2);
	int count = -1;

	TText *text[8];

	for(const auto fi : f){
		++count;
		text[count] = new TText(0.5,0.5,"lol");
		text[count]->SetNDC(1);
		TTree *gen, *ev;
		fi.second->GetObject("MssmHbb/Events/prunedGenParticles",gen);
		fi.second->GetObject("MssmHbb/Events/EventInfo",ev);
		gen->AddFriend(ev);
		if(count<4){
			c[0]->cd(count+1);
			gPad->SetLogy();
			gPad->SetLogx();
			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)",(mass + "> 0.7 *" + boost::lexical_cast<std::string>(fi.first)).c_str(),"E");
			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)","","same");
			text[count]->DrawText(0.7,0.7,(boost::lexical_cast<std::string>(fi.first)).c_str());
		}
		else{
			c[1]->cd(count+1 - 4);
			gPad->SetLogy();
			gPad->SetLogx();
			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)",(mass + "> 0.7 *" + boost::lexical_cast<std::string>(fi.first)).c_str(),"E");
			gen->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz)","","same");
			text[count]->DrawText(0.7,0.7,(boost::lexical_cast<std::string>(fi.first)).c_str());
		}
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
