/*
 * Histograms.cpp
 *
 *  Created on: 16 апр. 2016 г.
 *      Author: rostyslav
 */

#include "Analysis/MssmHbb/interface/Histograms.h"

Histograms::Histograms() {
	// TODO Auto-generated constructor stub

}

Histograms::~Histograms() {
	std::cout<<"I'm at ~Histograms"<<std::endl;
	// TODO Auto-generated destructor stub
}

void Histograms::Make(const int &size) {
	//Declare All Basic Histograms
	histo_["jet_pt1"]			=  new TH1D("jet_pt1","p_{T} of the first Leading Jet; Leading Jet p_{T}, [GeV]",size,0.,1500.);
	histo_["jet_pt2"]			=  new TH1D("jet_pt2","p_{T} of the second Leading Jet; sub-Leading Jet p_{T}, [GeV]",size,0.,1500.);
	histo_["jet_pt3"]			=  new TH1D("jet_pt3","p_{T} of the Third Leading Jet; third-Leading Jet p_{T}, [GeV]",size,0.,1500.);

	histo_["jet_eta1"]			=  new TH1D("jet_eta1","#eta of the first Leading Jet; Leading Jet #eta ",(int)size/2,-2.5,2.5);
	histo_["jet_eta2"]			=  new TH1D("jet_eta2","#eta of the second Leading Jet; sub-Leading Jet #eta ",(int)size/2,-2.5,2.5);
	histo_["jet_eta3"]			=  new TH1D("jet_eta3","#eta of the third Leading Jet; third-Leading Jet #eta ",(int)size/2,-2.5,2.5);

	histo_["jet_btag_csv1"]		=  new TH1D("jet_btag_csv1","btag_{csv} discr. first Leading Jet; Leading Jet btag_{csv} discr.",size,0.5,1.);
	histo_["jet_btag_csv2"]		=  new TH1D("jet_btag_csv2","btag_{csv} discr. first Leading Jet; Leading Jet btag_{csv} discr.",size,0.5,1.);
	histo_["jet_btag_csv3"]		=  new TH1D("jet_btag_csv3","btag_{csv} discr. first Leading Jet; Leading Jet btag_{csv} discr.",size,0.5,1.);

	histo_["jet_btag_cmva1"]	=  new TH1D("jet_btag_cmva1","btag_{cmva} discr. first Leading Jet; Leading Jet btag_{cmva} discr.",size,0.3,1.);
	histo_["jet_btag_cmva2"]	=  new TH1D("jet_btag_cmva2","btag_{cmva} discr. first Leading Jet; Leading Jet btag_{cmva} discr.",size,0.3,1.);
	histo_["jet_btag_cmva3"]	=  new TH1D("jet_btag_cmva3","btag_{cmva} discr. first Leading Jet; Leading Jet btag_{cmva} discr.",size,0.3,1.);

	histo_["jet_deta12"]		=  new TH1D("jet_deta12","#Delta #eta between Leading and sub-Leading jets;#Delta #eta_{12}",(int)size/2,-1.8,1.8);

	histo_["jet_dR12"]			=  new TH1D("jet_dR12","#Delta R between Leading and sub-Leading jets;#Delta R_{12}",size,0,5);
	histo_["jet_dR13"]			=  new TH1D("jet_dR13","#Delta R between Leading and third-Leading jets;#Delta R_{13}",size,0,5);
	histo_["jet_dR23"]			=  new TH1D("jet_dR23","#Delta R between sub-Leading and third-Leading jets;#Delta R_{23}",size,0,5);

	histo_["diJet_pt"]			=  new TH1D("diJet_pt","p_{T} of the di-Jet object;di-Jet p_{T}, [GeV]",size,0.,1500.);
	histo_["diJet_eta"]			=  new TH1D("diJet_eta","#eta of the di-Jet object; di-Jet #eta",(int)size/2,-2.5,2.5);
	histo_["diJet_phi"]			=  new TH1D("diJet_phi","#phi of the di-Jet object; di-Jet #phi",(int)size/2,0,2.*TMath::Pi());
	histo_["diJet_m"]			=  new TH1D("diJet_m","M_{12} of the di_jet object; di-Jet M_{12}, [GeV]",size,0.,1500);

	eff_["eff_2DPtTrigger_lowM_pt"]		=  new TEfficiency("eff_2DPtTrigger_lowM_pt","2D kinematic Trigger Efficiency, Low M trigger;Leading Jet p^{(1)}_{T}, [GeV]; sub-Leading Jet p^{(1)}_{T}, [GeV]; #epsilon",size,0.,500.);
	eff_["eff_2DPtTrigger_highM_pt"]	=  new TEfficiency("eff_2DPtTrigger_highM_pt","2D kinematic Trigger Efficiency, High M trigger;Leading Jet p^{(1)}_{T}, [GeV]; sub-Leading Jet p^{(1)}_{T}, [GeV]; #epsilon",size,0.,500.);
	eff_["eff_dEta_lowM"]				=  new TEfficiency("eff_dEta_lowM","#Delta #eta kinematic trigger Efficiency; |#Delta #eta|",(int)size/2,0,1.8);

}

std::map<std::string, TEfficiency* >& Histograms::getEff() {
	return eff_;
}

std::map<std::string, TH1* >& Histograms::getHisto() {
	return histo_;
}
