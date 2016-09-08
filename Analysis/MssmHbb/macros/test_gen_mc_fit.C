#include "DataFormats/FWLite/interface/Handle.h"
#include "TH1.h"

double bw(double *x, double *par){
	double M  = x[0];
	double M2 = M*M;
	double G = par[1];
	double G2 = G*G;
	double E = par[2];
	double E2= E*E;

	double result = par[0] / TMath::Pi() * G / 2 / ( pow(M2-E2,2.)+ G2/4 );
	return result;
}

double rlbw(double *x, double *par){
	double M  = x[0];
	double M2 = M*M;
	double G = par[1];
	double G2 = G*G;
	double E = par[2];
	double E2= E*E;

	double gamma = std::sqrt(M2 * (M2 + G2));
	double k = (2 * std::sqrt(2) * M * G * gamma) / (TMath::Pi() * std::sqrt(M2 + gamma));
//	double result = par[0]* k / ( std::pow(E2-M2,2.)+ M2*G2 );
	double result = par[0] * 2 / TMath::Pi() * G2*E2 / ( (M2-E2)*(M2-E2)+ M2*M2*(G2/E2) );
	return result;
}



int test_gen_mc_fit(){
	TH1::SetDefaultSumw2(kTRUE);

	TFile *input = new TFile("output_test.root","READ");
	if(input == nullptr){
		std::cerr<<"Wrong file name / Couldn't open the file"<<std::endl;
		exit(-1);
	}
	
	TFile *fOld_1p1 = new TFile("/afs/desy.de/user/c/chayanit/cms/CMSSW_7_1_22/src/HIG-RunIISummer15GS-00441_1.root","READ");
	if(fOld_1p1 == nullptr){
		std::cerr<<"Wrong Gen Input File"<<std::endl;
		exit(-1);
	}
	
	TH1D *hNew = (TH1D*) input->Get("hNew");
	hNew->Sumw2(true);
	hNew->Scale(1./hNew->Integral(hNew->FindBin(1000.),hNew->FindBin(1200.)));
	TH1D *hOld = (TH1D*) input->Get("hOld");
	hOld->Sumw2(true);
	hOld->Scale(1./hOld->Integral(hOld->FindBin(1000.),hOld->FindBin(1200.)));
	TH1D *hOld_rew = (TH1D*) hOld->Clone("hOld_rew");
	hOld_rew->Reset();
	hOld_rew->Sumw2(true);
	TH1D *hOld_not_rew = (TH1D*) hOld->Clone("hOld_not_rew");
	hOld_not_rew->Reset();
	hOld_not_rew->Sumw2(true);
//	TH1D *hOld_rew = new TH1D("hOld_rew","",120,200,1400);
//	TH1D *hOld_not_rew = new TH1D("hOld_not_rew","",120,200,1400);
//	hOld_rew->Sumw2(true);
	
	hOld->Draw("E");

	TF1 *fit_old = new TF1("fit_old","gaus",1000,1200);
	fit_old->SetParameters(4.85992e+02,8.82038e+02,2.05295e+02);
	hOld->Fit(fit_old,"R");

	TF1 *fit_new = new TF1("fit_new",rlbw,1000,1200,3);
	fit_new->SetParNames("Const","#Gamma","Mean");
	fit_new->SetParameters(4.85992e+02,30,1100);
	hNew->Fit(fit_new,"+R");
	std::cout<<"Chi2/NDF = "<<fit_new->GetChisquare()/fit_new->GetNDF()<<std::endl;
	
/*	TF1 *rew_func = new TF1("rew_func",function,1000,1200,6);
	rew_func->SetParameters(fit_old->GetParameter(0),
							fit_old->GetParameter(1),
							fit_old->GetParameter(2),
							fit_new->GetParameter(3),
*/							

	hNew->Draw("E");
	hOld->Draw("E same");
	std::cout<<"LOL "<<fit_old->Integral(1000,1200)<<" "<<fit_new->Integral(1000,1200)<<std::endl;

	TCanvas *can2 = new TCanvas("can2","can fit");
	fwlite::Event ev(fOld_1p1);
	
	for(ev.toBegin(); !ev.atEnd(); ++ev){
		edm::EventBase const & event = ev;
		edm::Handle<std::vector<reco::GenParticle> > gen_particles;
		event.getByLabel(std::string("genParticles"),gen_particles);
		for(const auto & part : *gen_particles){
			if(part.pdgId() == 36){
				double weight = 0;
				if(part.mass() > 1000 && part.mass() < 1200) {
					weight = fit_new->Eval(part.mass())/fit_old->Eval(part.mass());// * fit_old->Integral(1000,1200) / fit_new->Integral(1000,1200);
				}
				else weight = 0;
				hOld_rew->Fill(part.mass(),weight);
				hOld_not_rew->Fill(part.mass());
			}
		}
	}
	hOld_not_rew->SetMarkerColor(2);
	hOld_not_rew->SetMinimum(0.);
	hOld_not_rew->SetMarkerStyle(23);
	hOld_rew->SetMarkerStyle(20);
	hOld_not_rew->Draw("E");
	hOld_rew->Draw("E same");
	
	std::cout<<"Area Integral: "<<hNew->Integral(hNew->FindBin(1000.),hNew->FindBin(1200.))<<" "<<hOld->Integral(hOld->FindBin(1000.),hOld->FindBin(1200.))<<std::endl;
	std::cout<<"Integral: old = "<<hOld_not_rew->Integral(hOld_not_rew->FindBin(1000.),hOld_not_rew->FindBin(1200.))<<" new = "<<hOld_rew->Integral(hOld_rew->FindBin(1000.),hOld_rew->FindBin(1200.))<<std::endl;
	std::cout<<"function : BW / 4.48655e-02 * exp(-0.5 * ((x-8.82140e+02)/2.12431e+02)^2) "<<std::endl;
	/**/

	return 0;
}
	
