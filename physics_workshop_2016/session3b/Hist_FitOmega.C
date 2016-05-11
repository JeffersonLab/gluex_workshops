
void Hist_FitOmega() {

	TFile *fsim1 = TFile::Open("hist_omega_sim1.acc.root");
	if(!fsim1) return;
	
	TFile *fangles = TFile::Open("hist_omega_angles.acc.root");
	if(!fangles) return;

	int rebin = 5;
	TH1I* locHist_M3pi_sim1 = (TH1I*)fsim1->Get("M3pi"); 
	locHist_M3pi_sim1->SetLineColor(kRed); locHist_M3pi_sim1->Rebin(rebin);
	TH1I* locHist_M3pi_angles = (TH1I*)fangles->Get("M3pi");
	locHist_M3pi_angles->SetLineColor(kBlue); locHist_M3pi_angles->Rebin(rebin);

	// "data" sample with signal and background
	TH1I* locHist_M3pi_data = (TH1I*)locHist_M3pi_sim1->Clone();
	locHist_M3pi_data->Add(locHist_M3pi_angles);
	locHist_M3pi_data->SetLineColor(kBlack);

	int lowBin = locHist_M3pi_data->FindBin(0.757);
	int highBin = locHist_M3pi_data->FindBin(0.807);
	double dataYieldOmega = locHist_M3pi_data->Integral(lowBin, highBin);
	double sim1YieldOmega = locHist_M3pi_sim1->Integral(lowBin, highBin);
	double anglesYieldOmega = locHist_M3pi_angles->Integral(lowBin, highBin);
	cout<<"Total yield = "<<dataYieldOmega<<", signal = "<<anglesYieldOmega<<", bkgd = "<<sim1YieldOmega<<endl;
	cout<<"Purity = "<<anglesYieldOmega/dataYieldOmega<<endl;

	// fit to determine yield and purity
	double low = 0.65;
	double high = 0.91;
	TF1* f = new TF1("f","[0]*TMath::Voigt(x - [1], [2], [3]) + [4]*x + [5]", low, high);
	f->SetParameters(300, 0.782, 0.01, 0.01);
	f->SetLineColor(kBlack);

	TCanvas *cc = new TCanvas("cc", "cc", 800, 600);
	locHist_M3pi_data->Draw();
	locHist_M3pi_data->Fit(f, "", "", low, high);
	locHist_M3pi_sim1->Draw("same");
	locHist_M3pi_angles->Draw("same");

	TF1* fVoigt = new TF1("fVoigt","[0]*TMath::Voigt(x - [1], [2], [3])", low, high);
	fVoigt->SetParameters(f->GetParameter(0), f->GetParameter(1),f->GetParameter(2), f->GetParameter(3));
	fVoigt->SetLineColor(kBlue);
	TF1* fPol = new TF1("fPol","[0]*x + [1]", low, high);
	fPol->SetParameters(f->GetParameter(4), f->GetParameter(5));
	fPol->SetLineColor(kRed);

	fVoigt->Draw("same");
	fPol->Draw("same");

	return;
}
