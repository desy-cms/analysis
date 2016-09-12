
bool comp(std::pair<std::string,double> j1, std::pair<std::string,double> j2) {return j1.second > j2.second;}

int test_punzi(){

	std::string fullPath = "/afs/desy.de/user/s/shevchen/cms/cmssw-analysis/CMSSW_7_6_3_patch2/src/Analysis/MssmHbb/";
	std::string name = fullPath + "output/CutOptimisation_lowM_SUSYGluGluToBBHToBB_M-350_TuneCUETP8M1_13TeV-pythia8.root";//"output/CutOptimisation_lowM_clange-SUSYGluGluToBBHToBB_M-500_cfg_GEN_DIGI76X_RECO76X_MiniAODv2_76X-17d438ff51ec6b3cada9e499a5a978e0.root";

	TFile *mc   = new TFile(name.c_str(),"READ");
	name = fullPath + "output/CutOptimisation_lowM_Run2015.root";
	TFile *data = new TFile(name.c_str(),"READ");

	std::vector<std::string> pt_cuts = {"40","50","60","70","80","90","100","120","140","160","180","200"};
	std::vector<std::string> d_csv	 = {"0.46","0.8","0.935"};
	std::vector<std::string> d_cmva	 = {"-0.715","0.185","0.875"};
	std::vector<std::string> temp;
	std::vector<std::string> vars	 = {"d_csv","d_cmva"};
	std::array<std::string,3> jets = {"jet1","jet2","jet3"};
	std::vector<std::pair<std::string,double> > sens;

	TH1D *h_csv_mc, *h_csv_data;
	for(const auto & var : vars){
		std::cout<<"\n"<<var<<" scan"<<std::endl;
		if(var == "d_csv") temp = d_csv;
		else temp = d_cmva;
			for(const auto & s1 : temp){
				for(const auto & s2 : temp){
					for(const auto & s3 : temp){
						std::string name = "jet1_m12_" + var + ">" +s1+"_j2>"+s2+"_j3>"+s3;
//						std::cout<<name<<std::endl;
						h_csv_mc = (TH1D*) mc -> Get( ( name ).c_str() );
						h_csv_data = (TH1D*) data -> Get( ( name ).c_str() );
						if(!h_csv_mc) exit(-1);
						std::cout<<name<<" S = "<< h_csv_mc->Integral()<<" Bg = "<<h_csv_data->Integral()<<" S/sqrt(Bg) = "<<h_csv_mc->Integral()/sqrt(h_csv_data->Integral())<<std::endl;
						sens.push_back( std::pair<std::string,double>(name,h_csv_mc->Integral()/sqrt(h_csv_data->Integral())) );
					}
				}
			}
		}
	std::cout<<"\n\n Sensitivity of CSV and CMVA in Descending Order"<<std::endl;
	std::sort(sens.begin(),sens.end(),comp);
	for(const auto & s : sens){
		std::cout<<s.first<<" "<<s.second<<std::endl;
	}

	std::cout<<"pt3 scan"<<std::endl;
	for(const auto & jet : jets){
		for(const auto & s1 : pt_cuts){
			std::string name = jet + "_m12_pt>" +s1;
			h_csv_mc = (TH1D*) mc -> Get( ( name ).c_str() );
			h_csv_data = (TH1D*) data -> Get( ( name ).c_str() );
			if(!h_csv_mc) exit(-1);
			std::cout<<name<<" S = "<< h_csv_mc->Integral()<<" Bg = "<<h_csv_data->Integral()<<" S/sqrt(Bg) = "<<h_csv_mc->Integral()/sqrt(h_csv_data->Integral())<<std::endl;
		}
	}


	return 0;
}
