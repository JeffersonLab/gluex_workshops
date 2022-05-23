
void plotMassCorr() {

	gStyle->SetOptStat("");
	gStyle->SetPaintTextFormat("4.3f"); // set precision for histogram text
	gStyle->SetLegendBorderSize(0);

	// better with data
	TFile *f = TFile::Open("hist_etapi_bggen.acc.root");

	int topologies[5] = {0,1,2,3,4};
	for(int itop=0; itop<5; itop++) {
		TH2F *h2DInvariantMass = (TH2F*)f->Get(Form("h2DInvariantMass_ThrownTopology_%d",topologies[itop]));
		TH2F *h2DInvariantMassMeasured = (TH2F*)f->Get(Form("h2DInvariantMassMeasured_ThrownTopology_%d",topologies[itop]));
		TH2F *h2DInvariantMassShuffled = (TH2F*)f->Get(Form("h2DInvariantMassShuffled_ThrownTopology_%d",topologies[itop]));
		
		TCanvas *bb = new TCanvas("bb","bb",600,400);
		h2DInvariantMass->Draw("colz");
		bb->Print(Form("plots/2DInvariantMass_%d.pdf",topologies[itop]));
		
		TCanvas *cc = new TCanvas("cc","cc",600,400);
		h2DInvariantMassShuffled->Draw("colz");
		cc->Print(Form("plots/2DInvariantMassShuffled_%d.pdf",topologies[itop]));
	}

	return;
}
