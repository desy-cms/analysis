#include "mssm_xs_tools.C"

int xsection_13TeV(){
	mssm_xs_tools tool("mhmodp_mu200_13TeV.root");
//	tool.SetInput("mhmodp_mu200_13TeV.root");
	int point[] = {100,120,160,200,250,300,350,400,500,600,700,900,1100,1300};
	int beta[] = {20};
	//A
	std::cout<<"A->bb"<<std::endl;
	for(int i = 0; i < sizeof(beta)/sizeof(int); ++i)
	{
		for(int j = 0; j < sizeof(point)/sizeof(int); ++j){
			std::cout<<"beta = "<<beta[i]<<" M = "<<point[j]<<" XSection = "<<tool.bbHSantander_A(point[j],beta[i])<<" Br = "<<tool.br_Abb(point[j],beta[i])<<std::endl;
		}
	}

	//H
	std::cout<<"H->bb"<<std::endl;
        for(int i = 0; i < sizeof(beta)/sizeof(int); ++i)
        {
                for(int j = 0; j < sizeof(point)/sizeof(int); ++j){
                        std::cout<<"beta = "<<beta[i]<<" M = "<<point[j]<<" XSection = "<<tool.bbHSantander_H(point[j],beta[i])<<" Br = "<<tool.br_Hbb(point[j],beta[i])<<std::endl;
                }
        }
	return 0;
}
