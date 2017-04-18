int peak(){

	auto can1 = new TCanvas("can1");
	gPad->SetLogy();
	//Generate toy sample with expo shape of Bg and gaus for the signal
	auto toy = new TF1("toy","expo(0)+0.2*gaus(2)",200,900);
	toy->SetParameters(1,-0.001,1,750,15);
	toy->SetLineWidth(10);
	toy->SetLineColor(1);
	toy->Draw();


//	TCanvas *can2;
//	auto p = new TF1("p","gaus",-1,1);
//	p->SetParameters(1,0,0.1);
//	p->SetMinimum(-1);
//	p->SetLineWidth(10);
//	p->SetLineColor(1);
//	p->Draw();

	return 0;
}
