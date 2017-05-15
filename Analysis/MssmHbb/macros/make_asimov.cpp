/*
 * Macro to generate Asymov histogram from RooAbsPdf
 */

const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

using namespace std;

int make_asimov(){

	string path;
	int observed = 19251;
	TFile fIn(path.c_str(),"READ");
	TFile fOut( (cmsswBase + "/src/Analysis/MssmHbb/output/bg_template.root").c_str(),"RECREATE");

	RooWorkspace& w = (RooWorkspace&) * fIn.Get("workspace");
	RooRealVar&   x = (RooRealVar&) * w.var("mbb");
	RooAbsPdf& pdf  = (RooAbsPdf&) * w.pdf("name");
	w.Print("v");
	//Generate Central Template
	cout<<"Generate Central Template"<<endl;
	pdf.Print("v");
	TH1D * hC = pdf.createHistogram("data_obs",x,RooFit::Binning(73,200,1660));
	hC->Scale(observed / hC->Integral());

	return 0;
}
