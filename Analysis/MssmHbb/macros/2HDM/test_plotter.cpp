#include <string>
#include <map>
#include <TGraph.h>
#include <TVectorD.h>

#include <TFile.h>
#include <TROOT.h>
#include <TObjArray.h>
#include <TList.h>


using namespace std;

void print_variation(TGraph *);
void remove_stranges(double *, const int&);

int test_plotter(){
	string thdm_scans = "/nfs/dust/cms/user/shevchen/SusHiScaner/output/production_corseBins_cosB_A_-1_1_tanB_1-100/rootFiles/Histograms3D_type3_mA.root";
	double GxBR_95CL = 8.84;
	double GxBR_95CL_up = 12.55;
	double GxBR_95CL_down = 6.37;
	double GxBR_95CL_2up = 17.08;
	double GxBR_95CL_2down = 4.75;
	vector<double> cos, cos_up;
	vector<double> tb, tb_up;

	TFile *f = new TFile(thdm_scans.c_str());
	std::map<std::string,TH3F*> histos;
	std::map<std::string,TH2*> proj;
	histos["xs_bbA"] = (TH3F*) f->Get("xs_bbA");
	histos["br_Abb"] = (TH3F*) f->Get("br_Abb");

	//Select one mass point
	histos["xs_bbA"]->GetXaxis()->SetRange(1,1);
	histos["br_Abb"]->GetXaxis()->SetRange(1,1);

	//Contours:
	double GxBR[5];
	GxBR[0] = GxBR_95CL_2down; GxBR[1] = GxBR_95CL_down; GxBR[2] = GxBR_95CL; GxBR[3] = GxBR_95CL_up; GxBR[4] = GxBR_95CL_2up;

	proj["xs_bbA"] = (TH2F*)  histos["xs_bbA"]->Project3D("yz");
	proj["br_Abb"] = (TH2F*)  histos["br_Abb"]->Project3D("yz");
	proj["xs_bbA"]->Multiply(proj["br_Abb"]);
	proj["xs_bbA"]->GetXaxis()->SetRangeUser(-0.99,0.99);

	proj["xs_bbA"]->SetStats(kFALSE);
	proj["xs_bbA"]->DrawCopy("colz");
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL);
	proj["xs_bbA"]->SetLineColor(1);
	proj["xs_bbA"]->SetLineWidth(1.5);
	proj["xs_bbA"]->DrawCopy("cont3 same");
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL_up);
	proj["xs_bbA"]->SetLineColor(2);
	proj["xs_bbA"]->SetLineWidth(1.5);
	proj["xs_bbA"]->DrawCopy("cont3 same");
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL_down);
	proj["xs_bbA"]->SetLineColor(2);
	proj["xs_bbA"]->SetLineWidth(1.5);
	proj["xs_bbA"]->DrawCopy("cont3 same");

	TCanvas *can2 = new TCanvas("can2","",800,600);
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL);
	proj["xs_bbA"]->SetLineColor(2);
	proj["xs_bbA"]->SetLineWidth(3);
	proj["xs_bbA"]->SetLineStyle(2);
	proj["xs_bbA"]->DrawCopy("cont3");
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL_up);
	proj["xs_bbA"]->SetLineStyle(1);
	proj["xs_bbA"]->SetFillColor(kGreen);
	proj["xs_bbA"]->SetLineColor(kGreen);
	proj["xs_bbA"]->DrawCopy("cont3 same");
	proj["xs_bbA"]->SetContour(1,&GxBR_95CL_down);
	proj["xs_bbA"]->SetLineStyle(1);
	proj["xs_bbA"]->SetFillColor(kGreen);
	proj["xs_bbA"]->SetLineColor(kGreen);
	proj["xs_bbA"]->DrawCopy("cont3 same");

	TCanvas *can3 = new TCanvas("can3","",800,600);
	proj["xs_bbA"]->SetContour(5,GxBR);
//	proj["xs_bbA"]->SetLineStyle(1);
	proj["xs_bbA"]->Draw("cont Z LIST");
	can3->Update();

	// Get Contours
	TObjArray *conts = (TObjArray*)gROOT->GetListOfSpecials()->FindObject("contours");
	gROOT->GetListOfSpecials()->Print();
	TList* contLevel = nullptr;

	TGraph * expG = nullptr;

	TGraph *innerBand_up 	= nullptr;
	TGraph *innerBand_down = nullptr;
	TGraph *outerBand_up = nullptr;
	TGraph *outerBand_down = nullptr;

	if (conts == nullptr){
		cout<<"*** No Contours Were Extracted!\n";
		return 0;
	}

	outerBand_down	= (TGraph*) ((TList*)conts->At(0))->First();
	innerBand_down	= (TGraph*) ((TList*)conts->At(1))->First();
	expG 			= (TGraph*) ((TList*)conts->At(2))->First();
	innerBand_up 	= (TGraph*) ((TList*)conts->At(3))->First();
	outerBand_up 	= (TGraph*) ((TList*)conts->At(4))->First();

	int np = expG->GetN();

	if(innerBand_down == nullptr ||innerBand_up == nullptr){
		cout<<"*** No Up or Down contours!\n ";
		return 0;
	}

	double in_up[200], in_down[200], in[200], x[200], zero[200], out_up[200], out_down[200];
	np = 199;
	double max = 0.99, min = -0.99;
	double step = 0.01;
//	x = expG->GetX();
//	in = expG->GetY();
//	in_up = innerBand_up->GetY();
//	in_down = innerBand_down->GetY();
	cout<<"nrom"<<endl;
	for(int i = 0;i < np;++i) {
		x[i] = min + i*step;
		in[i] = expG->Eval(x[i]);
		in_up[i] = innerBand_up->Eval(x[i]) - in[i];
		in_down[i] = in[i] - innerBand_down->Eval(x[i]);
		out_up[i] = outerBand_up->Eval(x[i]) - in[i];
		out_down[i] = in[i] - outerBand_down->Eval(x[i]);
		zero[i] = 0;
	}



	cout<<"Number of points: "<<np<<endl;

//	TGraph *innerBand = new TGraph(2*np + 1);
	TGraphAsymmErrors * innerBand = new TGraphAsymmErrors(np, x, in, zero, zero, in_down, in_up);
	TGraphAsymmErrors * outerBand = new TGraphAsymmErrors(np, x, in, zero, zero, out_down, out_up);

//	for(int i = 0; i <= np; ++i){
//		innerBand->SetPoint(i,x[i],in_up[i]);
//		innerBand->SetPoint(np + i,x[np-i],in_down[np-i]);
//		cout<<i<<" x = "<<x[i]<<" -1G = "<<in_down[np-i]<<" Mid = "<<in[i]<<" +1G = "<<in_up[i]<<endl;
//	}

	expG->SetTitle("M_{A} = 300 GeV;cos(#beta-#alpha);tan(#beta)");
	expG->GetYaxis()->SetRangeUser(0.,100.);
	expG->GetXaxis()->SetRangeUser(-1.,1.);
	expG->SetLineWidth(3);
	expG->SetLineColor(2);
	expG->SetLineStyle(2);
	expG->Draw("Al");
	outerBand->Draw("3same");
	innerBand->Draw("3same");
	expG->Draw("l");
	innerBand->SetFillColor(kGreen);
	innerBand->SetLineColor(kGreen);
	outerBand->SetFillColor(kYellow);
	outerBand->SetLineColor(kYellow);

	TLegend *leg = new TLegend(0.56,0.53,0.83,0.87);
	leg->SetHeader("2HDM, Flipped model");
	leg->SetFillColor(0);
	leg->SetTextSize(0.035);
	leg->SetBorderSize(0);
	leg->AddEntry(expG,"Expected","l");
	leg->AddEntry(innerBand,"#pm1#sigma Expected","f");
	leg->AddEntry(outerBand,"#pm2#sigma Expected","f");
	leg->Draw();


	/*
	for(int i = 0; i < TotalConts; i++){
		contLevel = (TList*)conts->At(i);
		cout<<"Contour "<<i<< " has "<<contLevel->GetSize()<<" Graphs\n";
//		curv = (TGraph*)contLevel->First();
	}




	//Try to construct green area



	/*
	 * Example to fill the area
	 */
/*
	   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);

	   c1->SetGrid();
	   c1->DrawFrame(0,0,2.2,12);

	   const Int_t n = 20;
	   Double_t x[n], y[n],ymin[n], ymax[n];
	   Int_t i;
	   for (i=0;i<n;i++) {
	     x[i] = 0.1+i*0.1;
	     ymax[i] = 10*sin(x[i]+0.2);
	     ymin[i] = 8*sin(x[i]+0.1);
	     y[i] = 9*sin(x[i]+0.15);
	   }
	   TGraph *grmin = new TGraph(n,x,ymin);
	   TGraph *grmax = new TGraph(n,x,ymax);
	   TGraph *gr    = new TGraph(n,x,y);
	   TGraph *grshade = new TGraph(2*n);
	   for (i=0;i<n;i++) {
	      grshade->SetPoint(i,x[i],ymax[i]);
	      grshade->SetPoint(n+i,x[n-i-1],ymin[n-i-1]);
	   }
	   grshade->SetFillStyle(3013);
	   grshade->SetFillColor(16);
	   grshade->Draw("f");
	   grmin->Draw("l");
	   grmax->Draw("l");
	   gr->SetLineWidth(4);
	   gr->SetMarkerColor(4);
	   gr->SetMarkerStyle(21);
	   gr->Draw("CP");

/*
	TCanvas *can2 = new TCanvas("can2","",800,600);
	for(int binx = 1; binx <= proj["xs_bbA"]->GetNbinsX(); ++binx){	//cos(beta-alpha)
		for(int biny = 1; biny <= proj["xs_bbA"]->GetNbinsY(); ++biny){	//tanBeta
			double cs   = proj["xs_bbA"]->GetXaxis()->GetBinCenter(binx);
			double tanB = proj["xs_bbA"]->GetYaxis()->GetBinCenter(biny);
			double GxBR_pred    = proj["xs_bbA"] -> GetBinContent(binx,biny);
			if( abs(GxBR_pred - GxBR_95CL) / GxBR_95CL * 100 < 1){	//Central
				cos.push_back(cs);
				tb.push_back(tanB);
			}

			if( abs(GxBR_pred - GxBR_95CL_up) / GxBR_95CL_up * 100 < 1){	//Up 1G
				cos_up.push_back(cs);
				tb_up.push_back(tanB);
			}
		}
	}

	TGraph *gr    = new TGraph(cos.size(),&cos[0],&tb[0]);
	TGraph *gr_up = new TGraph(cos.size(),&cos_up[0],&tb_up[0]);
	gr_up->SetMarkerColor(2);
	gr_up->SetMarkerStyle(21);
	gr->SetMarkerStyle(20);
	gr->Draw("AP");
	gr_up->Draw("P");

	/**/
	return 0;
}

void print_variation(TGraph *gr){
	for(int i = 0; i < gr->GetN(); ++i){
		cout<<i<<" x = "<<gr->GetX()[i]<<" y = "<<gr->GetY()[i]<<endl;
	}
}

void remove_stranges(double *arr, const int& size){
	for(int i = 0; i < size; ++i){
//		if(arr[i] - (int)arr[i] == 0 )
	}
}
