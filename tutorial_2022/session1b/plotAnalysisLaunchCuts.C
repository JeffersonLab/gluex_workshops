
void plotAnalysisLaunchCuts() {

	gStyle->SetOptStat("");
	gStyle->SetPaintTextFormat("4.3f"); // set precision for histogram text
	gStyle->SetLegendBorderSize(0);

	// better with data
	TFile *f = TFile::Open("hist_etapi_data.acc.root");

	TH2F *hDeltaTVsP_BCAL = (TH2F*)f->Get("Hist_ParticleID/Step1__Pi0_->_Photon_Photon/Photon/DeltaTVsP_BCAL");
	TH2F *hDeltaTVsP_FCAL = (TH2F*)f->Get("Hist_ParticleID/Step1__Pi0_->_Photon_Photon/Photon/DeltaTVsP_FCAL");

	TH2F *h2DInvariantMass = (TH2F*)f->Get("h2DInvariantMass_ThrownTopology_0");
	TH2F *h2DInvariantMassMeasured = (TH2F*)f->Get("h2DInvariantMass_Measured_ThrownTopology_0");

	TCanvas *aa = new TCanvas("aa","aa",600,400);
	h2DInvariantMass->Draw("colz");

	TCanvas *bb = new TCanvas("bb","bb",600,400);
	h2DInvariantMassMeasured->Draw("colz");

	TCanvas *cc = new TCanvas("cc","cc",800,400);
	cc->Divide(2,1);
	cc->cd(1);
	hDeltaTVsP_BCAL->Draw("colz");
	cc->cd(2);
	hDeltaTVsP_FCAL->Draw("colz");

	cc->Print("plots/dataPID.pdf");
	aa->Print("plots/dataMassCorrPID.pdf");
	bb->Print("plots/dataMassMeasuredCorrPID.pdf");

	TH1F *hInvariantMass = (TH1F*)h2DInvariantMass->ProjectionY()->Clone();
	TH1F *hRFDeltaT = (TH1F*)f->Get("hRFDeltaT");

	aa->cd();
	hInvariantMass->Draw();
	aa->Print("plots/sidebandMass.pdf");
	
	aa->cd();
	hRFDeltaT->Draw();
	aa->Print("plots/rfDeltaT.pdf");

	return;
}
