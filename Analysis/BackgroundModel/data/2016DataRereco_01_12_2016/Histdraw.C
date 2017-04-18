{
	//gstyle->SetOptStat(0);

	TFile *f1 = new TFile("TripleBTagReverseSelectionBtoH2016_13TeV.root");
	TFile *f2 = new TFile("TripleBTagReverseSelectionBtoH2016_prescale_13TeV.root");
	TFile *f3 = new TFile("../Optimization_18_08_2016/TripleBTagReverseSelection_13TeV.root");
	//TFile *f4 = new TFile("TripleBTagMMLSelection_13TeV.root");

	TTree *tree1 = (TTree*)f1->Get("MssmHbb_13TeV");
	TTree *tree2 = (TTree*)f2->Get("MssmHbb_13TeV");
	TTree *tree3 = (TTree*)f3->Get("MssmHbb_13TeV");
	//TTree *tree4 = (TTree*)f4->Get("MssmHbb_13TeV");

	double mbb1,mbb2,mbb3,mbb4;
 
	TH1D* h1 = new TH1D("h1",";mbb [GeV];Arbitrary Units",100,0.,2000.);
	TH1D* h2 = new TH1D("h2",";mbb [GeV];Arbitrary Units",100,0.,2000.);
	TH1D* h3 = new TH1D("h3",";mbb [GeV];Arbitrary Units",100,0.,2000.);
	//TH1D* h4 = new TH1D("h4",";mbb [GeV];Arbitrary Units",100,0.,2000.);
/*
	TH1D* h1 = new TH1D("h1",";mbb [GeV];Arbitrary Units",120,0.,3000.);
        TH1D* h2 = new TH1D("h2",";mbb [GeV];Arbitrary Units",120,0.,3000.);
        TH1D* h3 = new TH1D("h3",";mbb [GeV];Arbitrary Units",120,0.,3000.);
        TH1D* h4 = new TH1D("h4",";mbb [GeV];Arbitrary Units",120,0.,3000.);
*/
	tree1->SetBranchAddress("mbb",	&mbb1);
	tree2->SetBranchAddress("mbb",  &mbb2);
	tree3->SetBranchAddress("mbb",  &mbb3);
	//tree4->SetBranchAddress("mbb",  &mbb4);

	for (int i = 0; i < tree1->GetEntriesFast(); ++i)
	{	tree1->GetEntry(i);
		h1->Fill(mbb1);
	}

        for (int i = 0; i < tree2->GetEntriesFast(); ++i)
        {       tree2->GetEntry(i);
                h2->Fill(mbb2);
        }

        for (int i = 0; i < tree3->GetEntriesFast(); ++i)
        {       tree3->GetEntry(i);
                h3->Fill(mbb3);
        }
/*
        for (int i = 0; i < tree4->GetEntriesFast(); ++i)
        {       tree4->GetEntry(i);
                h4->Fill(mbb4);
        }
*/
	h1->Scale(1/h1->Integral());
	h1->SetLineColor(kBlue);
	h1->SetLineWidth(2);
	h2->Scale(1/h2->Integral());
        h2->SetLineColor(kRed);
        h2->SetLineWidth(2);
	h3->Scale(1/h3->Integral());
        h3->SetLineColor(kGreen+2);
        h3->SetLineWidth(2);
	//h4->Scale(1/h4->Integral());
        //h4->SetLineColor(kViolet);
        //h4->SetLineWidth(2);

	h1->Draw("hist");
	h2->Draw("same hist");
	h3->Draw("same hist");
	//h4->Draw("same hist");

}
