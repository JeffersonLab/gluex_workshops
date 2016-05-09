
void Hist_FitAsymmetry() {
	
	TFile *filepara = TFile::Open("hist_p2gamma_para.acc.root");
	TFile *fileperp = TFile::Open("hist_p2gamma_perp.acc.root");
	TH2F *h2para = (TH2F*)filepara->Get("phi_t"); h2para->Rebin2D(10, 5);
	TH2F *h2perp = (TH2F*)fileperp->Get("phi_t"); h2perp->Rebin2D(10, 5);

	// phi distributions integrated over |t|
	TCanvas *cc1 = new TCanvas("cc1", "cc1", 800, 600);
	cc1->Divide(2,2);

	TH1F *hpara = (TH1F*)h2para->ProjectionY("phi_para");
	TH1F *hperp = (TH1F*)h2perp->ProjectionY("phi_perp");
	TH1F *hasym = hperp->GetAsymmetry(hpara);
	TH1F *hsum = (TH1F*)hpara->Clone(); hsum->Add(hperp);
	cc1->cd(1);
	TF1* fpara = new TF1("fitpara","[0]*(1.0 + [1]*cos(2*(x + [2])/180.*3.14159))");
	fpara->SetLineColor(kRed);
	hpara->Fit(fpara, "", "");
	hpara->Draw("e");
	cc1->cd(2);
	TF1* fperp = new TF1("fitperp","[0]*(1.0 + [1]*cos(2*(x + [2])/180.*3.14159))");
	fperp->SetLineColor(kRed);
	hperp->Fit(fperp, "", "");
	hperp->Draw("e");
	cc1->cd(3);
	TF1* fitasym = new TF1("fitasym","[1]*cos(2*(x + [2])/180.*3.14159)");
	fitasym->SetLineColor(kRed);
	hasym->Fit(fitasym, "", "");
	hasym->Draw("e");
	cc1->cd(4);
	hsum->Draw();

	return;	
	
}
