#include <boost/lexical_cast.hpp>

using namespace std;


int test2(){

	gStyle->SetOptStat(0);
	TFile f("/pnfs/desy.de/cms/tier2/store/mc/RunIIFall15DR76/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/AODSIM/PU25nsData2015v1Raw_76X_mcRun2_asymptotic_v12-v1/00000/32A0AE1D-28C5-E511-BE07-782BCB783902.root");

	Events->SetAlias("genParticles","recoGenParticles_genParticles__HLT.obj");
	Events->SetAlias("genEventInfo","GenEventInfoProduct_generator__SIM.obj");

	Events->Draw("genParticles.mass()","genParticles.pdgId() == 36 && (6500.*TMath::Sqrt( (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) * (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) - (genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second)*(genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second) ) >= 0.7*300 )");

	return 0;
}
