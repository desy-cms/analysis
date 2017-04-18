{	
	TFile *f = new TFile("BgStudy_everything_for_you_lowM_QCD.root") ;
	TDirectory *dir = (TDirectory*)f->Get("distributions");
	TH1D *h = (TH1D*)dir->Get("data_Mbb_50");
	h->SetName("data_Mbb");

	TFile* outputFile = new TFile("QCD_MC_lowM_noBtag_bins50.root","RECREATE");
        outputFile->cd();
        h->Write();
        outputFile->Close();
}
