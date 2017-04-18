#include "Analysis/MssmHbb/macros/Drawer/HbbStylesNew.C"

using namespace std;

void plot(const string & input_folder_template, const string& output_path);
void plotCentral(const int& mass, const string& input_path, const string& output_path);
void drawSystFits(const int & mass, const string& input_path, const std::string &syst, const std::string &output);
void compare(const vector<string>& input_folder_templates, const string& output_path);

vector<int> mass = {300,350,400,500,600,700,900,1100,1300};
vector<string> syst = {"JES","JER","PtEff","SFb"};

int signal_fits(){

	gStyle->SetOptFit(0000);
	gStyle->SetErrorX(0.5);

	const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));
	HbbStylesNew* style = new HbbStylesNew();
	style->SetStyle();
	string fits_initial = cmsswBase + "/src/Analysis/MssmHbb/output/SignalFits_study/initial/ReReco_signal_M-";
	string output = cmsswBase + "/src/Analysis/MssmHbb/macros/pictures/SignalFits/";
	plot(fits_initial,output);

	return 0;
}

void plot(const string & input_folder_template, const string& output_path){
	/*
	 * Method to draw a sexy fits
	 */
	const string fit_container = "/workspace/FitContainer_workspace.root";
	for(const auto& m : mass){
		plotCentral(m,input_folder_template + to_string(m) + fit_container,output_path);
		for(const auto& s : syst){
			drawSystFits(m,input_folder_template + to_string(m),s,output_path);
		}
	}
}

void plotCentral(const int& mass, const string& input_path, const string& output_path){
	gStyle->SetTitleOffset(1.300,"Y");
	gStyle->SetTitleOffset(1.100,"X");
	TFile f(input_path.c_str(),"READ");
	TCanvas c("c","c",1000,800);
	RooWorkspace& wCentral = *( (RooWorkspace*) f.Get("workspace"));

	RooRealVar& x = *( wCentral.var("mbb"));
	RooPlot& frame = *(x.frame(RooFit::Title( ("signal fit, m_{A} = " + to_string(mass) + " GeV").c_str())));
	RooAbsPdf& PdfCentral = *(wCentral.pdf("signal"));
	RooAbsData& dataC = *wCentral.data("signal_container");
	dataC.plotOn(&frame,RooFit::MarkerSize(1.0),RooFit::MarkerColor(kBlack),RooFit::LineColor(kRed),RooFit::Name("Central"));
	PdfCentral.plotOn(&frame,RooFit::Name("central_curve"),RooFit::LineColor(kBlack),RooFit::Normalization(dataC.sumEntries("1","fit_range"),RooAbsReal::NumEvent));
	frame.Draw();

	c.Print( (output_path + "signal_fit_mA_" + to_string(mass) + ".pdf").c_str() );
}

void drawSystFits(const int & mass, const string& input_path, const std::string &syst, const std::string &output){

	TFile fCentral( (input_path + "/workspace/FitContainer_workspace.root").c_str(),"read");
	TCanvas c("c","c",1000,800);
	RooWorkspace& wCentral = *( (RooWorkspace*) fCentral.Get("workspace"));

	//Extract RooRealVar for X-axis
	RooRealVar& x = *( wCentral.var("mbb"));
	RooPlot& frame = *(x.frame());
	RooAbsPdf& PdfCentral = *(wCentral.pdf("signal"));
	RooAbsData& dataC = *wCentral.data("signal_container");
	dataC.plotOn(&frame,RooFit::MarkerSize(0.8),RooFit::MarkerColor(kBlack),RooFit::LineColor(kBlack),RooFit::Name("Central"));
	PdfCentral.plotOn(&frame,RooFit::Name("central_curve"),RooFit::LineColor(kBlack),RooFit::Normalization(dataC.sumEntries("1","fit_range"),RooAbsReal::NumEvent));

	//down:
	TFile fDown( (input_path + "/" + syst + "_" + "Down/workspace/FitContainer_workspace.root").c_str());
	RooWorkspace& wDown = *( (RooWorkspace*) fDown.Get("workspace"));
	RooAbsPdf& PdfDown = *(wDown.pdf("signal"));
	RooAbsData& dataDown = *wDown.data("signal_container");
	dataDown.plotOn(&frame,RooFit::MarkerSize(0.8),RooFit::MarkerColor(kRed),RooFit::LineColor(kRed),RooFit::Name("Down"));
	PdfDown.plotOn(&frame,RooFit::LineColor(kRed),RooFit::Name("Down_curve"),RooFit::Normalization(dataDown.sumEntries("1","fit_range"),RooAbsReal::NumEvent));

	//up
	TFile fUp( (input_path + "/" + syst + "_" + "Up/workspace/FitContainer_workspace.root").c_str());
	RooWorkspace& wUp = *( (RooWorkspace*) fUp.Get("workspace"));
	RooAbsPdf& PdfUp = *(wUp.pdf("signal"));
	RooAbsData& dataUp = *wUp.data("signal_container");
	dataUp.plotOn(&frame,RooFit::MarkerSize(0.8),RooFit::MarkerColor(kBlue),RooFit::LineColor(kBlue),RooFit::Name("Up"));
	PdfUp.plotOn(&frame,RooFit::LineColor(kBlue),RooFit::Name("Up_curve"),RooFit::Normalization(dataUp.sumEntries("1","fit_range"),RooAbsReal::NumEvent));

	double xmin = 0.6,ymin = 0.6,xmax = 0.9,ymax = 0.85;
	if(mass > 900) {
		xmin = 0.25; ymin = 0.6 ; xmax = 0.45 ; ymax = 0.85;
	}
	TLegend leg(xmin,ymin,xmax,ymax);
	leg.SetBorderSize(0);
	leg.SetLineColor(0);
	frame.Draw();
	leg.AddEntry(frame.findObject("Central"),"Central","pl");
	leg.AddEntry(frame.findObject("Down"),("-2#sigma variation " + syst).c_str(),"pl");
	leg.AddEntry(frame.findObject("Up"),("+2#sigma variation " + syst).c_str(),"pl");
	leg.Draw();

	c.Print( (output + "signal_fit_mA_" + to_string(mass) + "_" + syst + ".pdf").c_str());
}
