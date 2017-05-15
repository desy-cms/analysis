void addBackgroundTemplate(const std::string & signal_template, const std::string & bg_template, const int & nevents_to_scale = -1, const std::string & bgHisto = "QCD_Mbb");

using namespace std;
const auto cmsswBase = static_cast<std::string>(gSystem->Getenv("CMSSW_BASE"));

int AddBackgroundTemplates(){

    int obs_sub_r1 = 259399;//358932;//
    int obs_sub_r2 = 105053;//147630;//
    int obs_sub_r3 = 26760;//38906;//

    std::string samples_name = "ReReco_newBGTemplates_20170126";//"ReReco_oldWP";//"ReReco_PU_test_subrange_presc";//
	vector<string> sig_templates {cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-300_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-350_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-400_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-500_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-600_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-700_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-900_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1100_TuneCUETP8M1_13TeV-pythia8.root",
								  cmsswBase + "/src/Analysis/MssmHbb/output/MssmHbbSignal_" + samples_name + "_lowM_SUSYGluGluToBBHToBB_NarrowWidth_M-1300_TuneCUETP8M1_13TeV-pythia8.root",};
//	string path_to_bg = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/Rereco2016_Templates/QCD_Templates_Full.root";
	string path_to_bg_sub_r1 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/Rereco2016_Prescale_BG20170126/QCD_Templates_SR1_NovoEff.root";
	string path_to_bg_sub_r2 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/Rereco2016_Prescale_BG20170126/QCD_Templates_SR2.root";
	string path_to_bg_sub_r3 = cmsswBase + "/src/Analysis/MssmHbb/bin/input_corrections/Rereco2016_Prescale_BG20170126/QCD_Templates_SR3.root";
	for(const auto& templ : sig_templates){
		if(templ.find("M-300") != string::npos || templ.find("M-350") != string::npos || templ.find("M-400") != string::npos || templ.find("M-500") != string::npos){
			addBackgroundTemplate(templ,path_to_bg_sub_r1,obs_sub_r1);
		}
		else if (templ.find("M-600") != string::npos || templ.find("M-700") != string::npos || templ.find("M-900") != string::npos){
			addBackgroundTemplate(templ,path_to_bg_sub_r2,obs_sub_r2);
		}
		else if (templ.find("M-1100") != string::npos || templ.find("M-1300") != string::npos){
			addBackgroundTemplate(templ,path_to_bg_sub_r3,obs_sub_r3);
		}
//		addBackgroundTemplate(templ,path_to_bg,19251);
	}

	return 0;
}

void addBackgroundTemplate(const std::string & signal_template, const std::string & bg_template, const int & nevents_to_scale, const std::string & bgHisto){

	TFile fsignal(signal_template.c_str(),"UPDATE");	// Opening file for writing;
	if(fsignal.IsZombie()){
		std::cerr<<"Error opening file: "<<signal_template<<" for merging"<<std::endl;
		exit(0);
	}
	TFile fbg(bg_template.c_str(),"READ");
	if(fbg.IsZombie()){
		std::cerr<<"Error opening file: "<<bg_template<<" for merging"<<std::endl;
		exit(1);
	}
	//Check whether
	if(!fbg.GetListOfKeys()->Contains(bgHisto.c_str())){
		std::cerr<<"No histo: "<<bgHisto.c_str()<<" in file: "<<bg_template<<std::endl;
		exit(2);
	}

	//Read all objects from Bg templates file
	TIter next(fbg.GetListOfKeys());
	TKey *key;
	TIter next_sg(fsignal.GetListOfKeys());
	TKey *key_sg;

	fsignal.cd();
	while ((key_sg = (TKey*)next_sg())) {
		TClass *cl_sg = gROOT->GetClass(key_sg->GetClassName());
		if (!cl_sg->InheritsFrom("TH1")) continue;
		TH1 *h_sg = (TH1*)key_sg->ReadObj();
		std::string name = h_sg->GetName();
		string title = h_sg->GetTitle();
		if(name.std::string::find(bgHisto) != std::string::npos || title.find("Templates") != std::string::npos){
			std::cout<<"TO DELETE: "<<name<<std::endl;
			gDirectory->Delete( (name + ";1").c_str() );
		}
		gDirectory->Delete("data_obs;1");
	}

	std::string objName;
	while ((key = (TKey*)next())) {
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if (!cl->InheritsFrom("TH1")) continue;
		TH1 *h = (TH1*)key->ReadObj();
		objName = h->GetName();
		if(nevents_to_scale != -1){
			std::cout<<"Scale Bg histograms to: "<<nevents_to_scale/h->Integral()<<std::endl;
			h->Scale(nevents_to_scale/h->Integral());
		}
		if( objName.find(bgHisto) == std::string::npos) continue;
		std::cout<<h->GetName()<<std::endl;

		fsignal.cd();
		h->Write(h->GetName());
		if( objName == bgHisto){
			h->SetName("data_obs");
			h->Write("data_obs");
		}
	}

}
