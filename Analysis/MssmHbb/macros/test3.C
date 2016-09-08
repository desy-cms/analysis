int test3(){

	TH1::SetDefaultSumw2();

//	TFile *fOld_1p1 = new TFile("/afs/desy.de/user/c/chayanit/cms/CMSSW_7_1_22/src/HIG-RunIISummer15GS-00441_1.root","READ");

	std::map<int,TFile*> f;
	f[500] = new TFile("/pnfs/desy.de/cms/tier2/store/user/rshevche/Analysis/Ntuples/MSSMHbb/MINIAODSIM/Fall15.76X.20160616_noFilters/SUSYGluGluToBBHToBB_M-500_TuneCUETP8M1_13TeV-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/160623_172041/0000/output_2.root");

	TTree *old;
	f[500]->GetObject("MssmHbb/Events/prunedGenParticles",old);

	TChain *events = new TChain("Events");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_365.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_366.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_367.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_368.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_369.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_370.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_371.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_372.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_373.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_374.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_375.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_376.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_377.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_378.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_379.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_380.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_381.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_382.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_383.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_384.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_385.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_386.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_387.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_388.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_389.root");
	events->AddFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_390.root");

	events->SetAlias("genParticles","recoGenParticles_prunedGenParticles__PAT.obj");

	TCanvas *c = new TCanvas("c","",1000,800);
	gPad->SetLogy();
	TH1D *hOld = new TH1D("hOld","Generated Higgs mass",100,0,1000);
	TH1D *hNew = new TH1D("hNew","Generated Higgs mass",100,0,1000);
	TLegend *l = new TLegend(0.6,0.5,0.8,0.65);
	l->AddEntry(hOld,"Old MC","l");
	l->AddEntry(hNew,"New MC","lp");
	hNew->SetMarkerStyle(20);

	old->Draw("TMath::Sqrt(e*e-px*px-py*py-pz*pz) >> hOld","pdg == 36","E");
	events->Draw("genParticles.mass()>>hNew","genParticles.pdgId() == 36", "E");

	hOld->Scale(1./hOld->Integral());
	hOld->SetLineColor(2);
	hNew->Scale(1./hNew->Integral());
	hNew->SetMarkerStyle(20);

	hNew->Draw("E");
	hOld->Draw("hist same");
	l->Draw();

	/*
	TFile *fNew_1p1 = new TFile("/pnfs/desy.de/cms/tier2/store/user/chayanit/MSSMHbb/SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X/160706_111819/0000/HIG-RunIIFall15MiniAODv2-00068_365.root");

	if(fNew_1p1 == 0){
		std::cerr<<"Wrong file name / Couldn't open the file"<<std::endl;
		exit(-1);
	}
	
	TTree *tOld, *tNew;
//	fOld_1p1->GetObject("Events",tOld);
	fNew_1p1->GetObject("Events",tNew);

//	tOld->SetAlias("genParticles","recoGenParticles_genParticles__GEN.obj");
	//tNew->SetAlias("genParticles","recoGenParticles_genParticles__GEN.obj");
	tNew->SetAlias("genParticles","recoGenParticles_prunedGenParticles__PAT.obj");

	TH1D *hNew = new TH1D("hNew","",200,00,1300);
//	TH1D *hOld = new TH1D("hOld","",200,00,1300);
	hNew->SetMarkerStyle(20);
//	hOld->SetMarkerStyle(21);
//	tree->SetAlias("genEventInfo","GenEventInfoProduct_generator__GEN.obj");

	tNew->Draw("genParticles.mass()>>hNew","genParticles.pdgId() == 36", "E"); //&& (6500.*TMath::Sqrt( (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) * (genEventInfo->pdf()->x.first + genEventInfo->pdf()->x.second) - (genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second)*(genEventInfo->pdf()->x.first - genEventInfo->pdf()->x.second) ) >= 0.7*300 )");

//	tOld->Draw("genParticles.mass()>>hOld","genParticles.pdgId() == 36","E same");
	
/*	TFile *output = new TFile("output_test_m500.root","RECREATE");
	output->cd();
	hNew->Write();
	hOld->Write();
	
	*/
	
	return 0;
}

