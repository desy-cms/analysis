#include "TF1.h"

int SFb(){

	TF1 *central = new TF1("central","0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x)))" ,30,670);
	central->SetTitle("BTagScaleFactor;p_{T} (GeV); SFb");
	central->GetYaxis()->SetRangeUser(0.8,1.2);
	central->SetLineColor(1);

	TF1 *up1 = new TF1("up1","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.019803794100880623",30,50);
	TF1 *up2 = new TF1("up2","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.026958625763654709",50,70);
	TF1 *up3 = new TF1("up3","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.024285079911351204",70,100);
	TF1 *up4 = new TF1("up4","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.028512096032500267",100,140);
	TF1 *up5 = new TF1("up5","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.029808893799781799",140,200);
	TF1 *up6 = new TF1("up6","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.026503190398216248",200,300);
	TF1 *up7 = new TF1("up7","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))+0.042264193296432495",300,670);

	TF1 *down1 = new TF1("down1","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.019803794100880623",30,50);
	TF1 *down2 = new TF1("down2","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.026958625763654709",50,70);
	TF1 *down3 = new TF1("down3","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.024285079911351204",70,100);
	TF1 *down4 = new TF1("down4","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.028512096032500267",100,140);
	TF1 *down5 = new TF1("down5","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.029808893799781799",140,200);
	TF1 *down6 = new TF1("down6","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.026503190398216248",200,300);
	TF1 *down7 = new TF1("down7","(0.886376*((1.+(0.00250226*x))/(1.+(0.00193725*x))))-0.042264193296432495",300,670);

	central->Draw();
	up1->Draw("same");
	up2->Draw("same");
	up3->Draw("same");
	up4->Draw("same");
	up5->Draw("same");
	up6->Draw("same");
	up7->Draw("same");

	down1->Draw("same");
	down2->Draw("same");
	down3->Draw("same");
	down4->Draw("same");
	down5->Draw("same");
	down6->Draw("same");
	down7->Draw("same");

	int point = 150;

	std::cout<<"point: "<<point<<std::endl;
	std::cout<<"central:"<<central->Eval(point)<<" up: "<<up5->Eval(point)<<" down: "<<down5->Eval(point)<<std::endl;
	std::cout<<"Evaluation: "<<central->Eval(point)*central->Eval(point)<<" Error: "<<(up5->Eval(point)*up5->Eval(point)-down5->Eval(point)*down5->Eval(point))/2<<
			" "<<(up5->Eval(point)*up5->Eval(point)-down5->Eval(point)*down5->Eval(point))/2/central->Eval(point)/central->Eval(point)<<std::endl;


	return 0;
}
