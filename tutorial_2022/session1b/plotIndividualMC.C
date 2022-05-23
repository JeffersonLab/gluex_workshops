
void plotIndividualMC() {

	gStyle->SetOptStat("");
	gStyle->SetPaintTextFormat("4.3f"); // set precision for histogram text
	gStyle->SetLegendBorderSize(0);

	// better with data
	TFile *f = TFile::Open("hist_etapi_bggen.acc.root");
	TFile *fdata = TFile::Open("hist_etapi_bggen.acc.root");
	TFile *fDeltaPlusEta = TFile::Open("hist_etapi_DeltaPlusEta.acc.root");
	TFile *fb1 = TFile::Open("hist_etapi_b1.acc.root");

	TH2F *hPi0ProtonMass_data = (TH2F*)fdata->Get("hPi0ProtonMass_ThrownTopology_0");
	TH2F *hEtaPiMass_data = (TH2F*)fdata->Get("hEtaPiMass_ThrownTopology_0");

	TH2F *hPi0ProtonMass_DeltaPlusEta = (TH2F*)fDeltaPlusEta->Get("hPi0ProtonMass_ThrownTopology_1");
	TH2F *hPi0ProtonMass_bggen = (TH2F*)f->Get("hPi0ProtonMass_ThrownTopology_1");
	
	TH2F *hEtaPiMass_b1 = (TH2F*)fb1->Get("hEtaPiMass_ThrownTopology_3");
	TH2F *hEtaPiMass_bggen = (TH2F*)f->Get("hEtaPiMass_ThrownTopology_3");
				
	TCanvas *bb = new TCanvas("bb","bb",600,400);
	bb->Divide(2,1);
	bb->cd(1);
	hPi0ProtonMass_data->Draw();
	bb->cd(2);
	hPi0ProtonMass_bggen->Draw();
	bb->Print("plots/Pi0ProtonMass_bggen.pdf");

	bb->cd(1);
	hPi0ProtonMass_data->Draw();
	bb->cd(2);
	hPi0ProtonMass_DeltaPlusEta->Draw();
	bb->Print("plots/Pi0ProtonMass_DeltaPlusEta.pdf");
	
	TCanvas *cc = new TCanvas("cc","cc",600,400);
	cc->Divide(2,1);
	cc->cd(1);
	hEtaPiMass_data->Draw();
	cc->cd(2);
	hEtaPiMass_bggen->Draw();
	cc->Print("plots/EtaPiMass_bggen.pdf");
	
	cc->cd(1);
	hEtaPiMass_data->Draw();
	cc->cd(2);
	hEtaPiMass_b1->Draw();
	cc->Print("plots/EtaPiMass_bq.pdf");

	return;
}
