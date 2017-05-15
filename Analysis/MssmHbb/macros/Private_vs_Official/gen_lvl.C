void SetStyle(TH1 * h){
	h->SetStats(kFALSE);
	h->SetMarkerStyle(20);
	h->SetMarkerSize(1.5);
	h->SetLineWidth(2.5);
	h->Scale(1./h->Integral());
}

int gen_lvl(){

	TH1::SetDefaultSumw2();
//	gStyle->SetOptStat(100002200);

	//Script to compare generated mass distribution at gen level
	//For General production / After mHat cut / Private production

	std::map<int,TFile*> fNtuples;
	fNtuples[1100] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172734/0000/output_2.root");

	TChain *fDAS = new TChain("Events");
	fDAS->AddFile("root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/SUSYGluGluToBBHToBB_M-1100_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/08569244-E4B8-E511-8D20-141877410E71.root");//new TFile("/nfs/dust/cms/user/shevchen/diff_files/gen_1100_das.root","READ");

	TChain *fPrivate = new TChain("Events");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_365.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_366.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_367.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_368.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_369.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_370.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_371.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_372.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_373.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_374.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_375.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_376.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_377.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_378.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_379.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_380.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_381.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_382.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_383.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_384.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_385.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_386.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_387.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_388.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_389.root");
	fPrivate->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-1100_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111744/0000/HIG-RunIIFall15MiniAODv2-00068_390.root");

	TTree *tNtuples;
	fNtuples[1100]->GetObject("MssmHbb/Events/prunedGenParticles",tNtuples);
	fDAS->SetAlias("genParticles","recoGenParticles_prunedGenParticles__PAT.obj");
	fPrivate->SetAlias("genParticles","recoGenParticles_prunedGenParticles__PAT.obj");

	TH1D *hDAS = new TH1D("hDAS","Generated Higgs mass (Central production 2015), M = 1100 GeV; M_{H}, [GeV]; a. u.",100,0,1300);
	TH1D *hNtuples = new TH1D("hNtuples","Generated Higgs mass (After Mhat cut), M = 1100 GeV; M_{H}, [GeV]; a. u.",100,0,1300);
	TH1D *hPrivate = new TH1D("hPrivate","Generated Higgs mass (Private production), M = 1100 GeV; M_{H}, [GeV]; a. u.",100,0,1300);

	TCanvas *cNtuples = new TCanvas("cNtuples","",1000,800);
	tNtuples->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz) >> hNtuples","pdg == 36","hist");
	SetStyle(hNtuples);
	cNtuples->Print("hNtuples_1100.pdf");
	TCanvas *cDAS = new TCanvas("cDAS","",1000,800);
	fDAS->Draw("genParticles.mass()>>hDAS","genParticles.pdgId() == 36", "hist");
	SetStyle(hDAS);
	cDAS->Print("hDAS_1100.pdf");
	TCanvas *cPrivate = new TCanvas("cPrivate","",1000,800);
	fPrivate->Draw("genParticles.mass()>>hPrivate","genParticles.pdgId() == 36", "hist");
	SetStyle(hPrivate);
	cPrivate->Print("cPrivate_1100.pdf");

	TCanvas *cAll = new TCanvas("cAll","",1000,800);
//	gPad->SetLogy();
	hPrivate->SetLineColor(2);
	hPrivate->Draw("hist");
	hNtuples->Draw("hist same");
	hDAS->SetLineStyle(kDashed);
//	hDAS->Draw("hist same");

	TLegend *l = new TLegend(0.2,0.6,0.5,0.85);
//	l->AddEntry(hDAS,"Central Production 2015","l");
	l->AddEntry(hNtuples,"Central Production 2015","l");
	l->AddEntry(hPrivate,"Private Production","l");
	l->Draw();
	cAll->Print("cAll_1100.pdf");



	
	return 0;
}

