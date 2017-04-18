#include "TGraph.h"

using namespace std;

class point {
public:
	point() : x(0), y(0) {};
	point(const double& v1, const double& v2) : x(v1), y(v2) {};

	double x;
	double y;

private:
};

int ATLAS_A_Zh_ll_plot(){

	gStyle->SetLineWidth(3);
	gStyle->SetMarkerSize (1.4);

	string xtitle = "tan#beta";
	string ytitle = "cos(#beta-#alpha)";


	TCanvas *can = new TCanvas("can","can",800,600);
	gPad->SetLogy();

	//Frame
	TH2F *frame = new TH2F("frame","",2,-0.95,0.95,2,0.5,100);
	frame->GetXaxis()->SetTitle(xtitle.c_str());
	frame->GetYaxis()->SetTitle(ytitle.c_str());
	frame->GetXaxis()->SetNdivisions(505);
	frame->GetYaxis()->SetNdivisions(206);
	frame->GetYaxis()->SetTitleOffset(1.3);
	frame->GetXaxis()->SetTitleOffset(1.05);
	frame->GetYaxis()->SetTitleSize(0.048);
	frame->SetStats(0);
	frame->Draw();

	TLine *line = new TLine(-0.95,50,0.95,50);
	vector<point> lower_left= {
			point(-0.02,0.5),
			point(-0.021,0.6),
			point(-0.022,0.7),
			point(-0.023,0.8),
			point(-0.024,0.9),
			point(-0.025,1.),
			point(-0.026,1.3),
			point(-0.05,2.),
			point(-0.054,2.5),
			point(-0.1,3.),
			point(-0.15,3.5),
			point(-0.21,4),
			point(-0.3,4.5),
			point(-0.45,5),
			point(-0.64,5.5),
			point(-0.95,6)
	};

	vector<double> cos_lower_left, tan_lower_left;
	for(const auto& val : lower_left) {
		cos_lower_left.push_back(val.x);
		tan_lower_left.push_back(val.y);
	}

	vector<point> upper_left = {
			point(-0.95,8.5),
			point(-0.75,9.),
			point(-0.7,9.5),
			point(-0.675,10.),
			point(-0.65,11),
			point(-0.6,12),
			point(-0.55,12.5),
			point(-0.5,13.5),
			point(-0.45,14),
			point(-0.4,18),
			point(-0.375,25.),
			point(-0.36,40.),
			point(-0.35,50.)
	};

	vector<double> cos_upper_left, tan_upper_left;
	for(const auto& val : upper_left) {
		cos_upper_left.push_back(val.x);
		tan_upper_left.push_back(val.y);
	}

	vector<point> upper_right = {
			point(0.35,50),
			point(0.36,40.),
			point(0.375,25.),
			point(0.4,20),
			point(0.45,14),
			point(0.5,13.5),
			point(0.55,12.5),
			point(0.6,12),
			point(0.65,11),
			point(0.675,10.),
			point(0.7,9.7),
			point(0.75,9.),
			point(0.95,8.5),
	};

	vector<double> cos_upper_right, tan_upper_right;
	for(const auto& val : upper_right) {
		cos_upper_right.push_back(val.x);
		tan_upper_right.push_back(val.y);
	}

	vector<point> lower_right= {
			point(0.95,6),
			point(0.725,5.5),
			point(0.575,5),
			point(0.5,4.5),
			point(0.425,4),
			point(0.4,3.7),
			point(0.35,3.5),
			point(0.3,3.5),
			point(0.275,3.5),
			point(0.25,3.4),
			point(0.15,3.2),
			point(0.12,3.),
			point(0.1,2.7),
			point(0.07,2.5),
			point(0.05,2.),
			point(0.026,1.3),
			point(0.025,1.),
			point(0.024,0.9),
			point(0.023,0.8),
			point(0.022,0.7),
			point(0.021,0.6),
			point(0.02,0.5),
	};

	vector<double> cos_lower_right, tan_lower_right;
	for(const auto& val : lower_right) {
		cos_lower_right.push_back(val.x);
		tan_lower_right.push_back(val.y);
	}

	TGraph *fl_lower_left = new TGraph(cos_lower_left.size(),cos_lower_left.data(),tan_lower_left.data());
	TGraph *fl_upper_left = new TGraph(cos_upper_left.size(),cos_upper_left.data(),tan_upper_left.data());
	TGraph *fl_upper_right = new TGraph(cos_upper_right.size(),cos_upper_right.data(),tan_upper_right.data());
	TGraph *fl_lower_right = new TGraph(cos_lower_right.size(),cos_lower_right.data(),tan_lower_right.data());
	fl_upper_left->SetMarkerStyle(20);
	fl_upper_right->SetMarkerStyle(20);
	fl_lower_right->SetMarkerStyle(20);
	fl_lower_left->SetMarkerStyle(20);
//	fl_lower_left->Draw("PL");
//	fl_upper_left->Draw("PL same");
//	fl_lower_right->Draw("PL same");
//	fl_upper_right->Draw("PL same");
	line->Draw("same");

	//One graph for all of them:
	int total_size = cos_upper_left.size() + cos_lower_left.size() + cos_upper_right.size() + cos_lower_right.size();
	vector<double> cos_all, tan_all;
	cos_all.reserve(total_size);
	cos_all.insert(cos_all.end(),cos_lower_left.begin(),cos_lower_left.end());
	cos_all.insert(cos_all.end(),cos_upper_left.begin(),cos_upper_left.end());
	cos_all.insert(cos_all.end(),cos_upper_right.begin(),cos_upper_right.end());
	cos_all.insert(cos_all.end(),cos_lower_right.begin(),cos_lower_right.end());

	tan_all.reserve(total_size);
	tan_all.insert(tan_all.end(),tan_lower_left.begin(),tan_lower_left.end());
	tan_all.insert(tan_all.end(),tan_upper_left.begin(),tan_upper_left.end());
	tan_all.insert(tan_all.end(),tan_upper_right.begin(),tan_upper_right.end());
	tan_all.insert(tan_all.end(),tan_lower_right.begin(),tan_lower_right.end());

	TGraph *all = new TGraph(total_size,cos_all.data(),tan_all.data());
	all->SetMarkerStyle(20);
	all->Draw("PL");


	return 0;
}
