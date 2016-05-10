#include "mssm_xs_tools.C"

int xsection_8TeV(){
	mssm_xs_tools tool;
	tool.SetInput("mhmodp_mu-200_8TeV.root");
	int point[] = {100,120,160,200,250,300,350,400,500,600,700,900,1100,1300};
	int beta[] = {20};

	std::cout<<"A->bb"<<std::endl;
	for(int i = 0; i < sizeof(beta)/sizeof(int); ++i)
	{
		for(int j = 0; j < sizeof(point)/sizeof(int); ++j){
			std::cout<<"beta = "<<beta[i]<<" M = "<<point[j]<<" XSection = "<<tool.GiveXsec_Santander_A(point[j],beta[i])<<" Br = "<<tool.Give_BR_A_bb(point[j],beta[i])<<std::endl;
		}
	}

	std::cout<<"H->bb"<<std::endl;
        for(int i = 0; i < sizeof(beta)/sizeof(int); ++i)
        {
                for(int j = 0; j < sizeof(point)/sizeof(int); ++j){
                        std::cout<<"beta = "<<beta[i]<<" M = "<<point[j]<<" XSection = "<<tool.GiveXsec_Santander_H(point[j],beta[i])<<" Br = "<<tool.Give_BR_H_bb(point[j],beta[i])<<std::endl;
                }
        }
	return 0;
}
