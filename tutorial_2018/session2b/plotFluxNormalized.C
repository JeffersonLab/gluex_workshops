
void plotFluxNormalized(int rebin = 5) {

	gStyle->SetOptStat(0);
	TString locDataDir = "/work/halld2/home/jrsteven/tutorial2018/data/";

	// get histograms for data
	TFile *fFlux = TFile::Open(locDataDir+"flux_30730.root");
	TH1F *locHist_UntaggedFlux = (TH1F*)fFlux->Get("PS_UntaggedFlux");
	TH1F *locHist_TaggedFlux = (TH1F*)fFlux->Get("PS_TaggedFlux");
	locHist_UntaggedFlux->Rebin(rebin); locHist_UntaggedFlux->SetMarkerStyle(20);
	locHist_TaggedFlux->Rebin(rebin); locHist_TaggedFlux->SetMarkerStyle(20);

	TCanvas *aa = new TCanvas("aa", "aa", 700, 400);
	aa->Divide(2,1);
	aa->cd(1);
	locHist_UntaggedFlux->Draw("p");
	aa->cd(2);
	locHist_TaggedFlux->Draw("p");

	TFile *fObservedData = TFile::Open("../session1c/hist_omega_3pi.root");
	TH1F *locHist_OmegaObservedData = (TH1F*)fObservedData->Get("BeamE");
	locHist_OmegaObservedData->Rebin(2*rebin);

	// get histograms for MC
	TFile *fGenerated = TFile::Open("hist_generated.root");
	TH1F *locHist_OmegaGenerated = (TH1F*)fGenerated->Get("hEgamma");
	TFile *fObservedMC = TFile::Open("../session1c/hist_omega_3pi_gen_omega_3pi.root");
	TH1F *locHist_OmegaObservedMC = (TH1F*)fObservedMC->Get("BeamE");
	locHist_OmegaGenerated->Rebin(2*rebin);
	locHist_OmegaObservedMC->Rebin(2*rebin);

	locHist_OmegaGenerated->SetMarkerStyle(20);
	locHist_OmegaObservedMC->SetMarkerStyle(20);
	locHist_OmegaObservedData->SetMarkerStyle(20);

	TH1F *locHist_OmegaFluxNormalized = (TH1F*)locHist_OmegaObservedData->Clone("fluxNormalized");
	locHist_OmegaFluxNormalized->Divide(locHist_TaggedFlux);

	TH1F *locHist_Effic = (TH1F*)locHist_OmegaObservedMC->Clone("efficiency");
	locHist_Effic->Divide(locHist_OmegaGenerated);
	locHist_Effic->SetTitle("; E_{#gamma} (GeV); efficiency");
       
	// calculate cross section
	TH1F *locHist_OmegaCrossSection = (TH1F*)locHist_OmegaFluxNormalized->Clone("sigma");
	locHist_OmegaCrossSection->Divide(locHist_Effic);
	locHist_OmegaCrossSection->SetTitle("; E_{#gamma} (GeV); #omega cross section (nb)");
	locHist_OmegaCrossSection->Scale(1/1.2e-9);

	TCanvas *bb = new TCanvas("bb", "bb", 1000, 400);
	bb->Divide(3,1);

	bb->cd(1);
	locHist_OmegaGenerated->Draw("p");
	
	bb->cd(2);
	locHist_OmegaObservedMC->Draw("p");

	// draw efficiency vs Egamma
	bb->cd(3);
	locHist_Effic->Draw("p");

	TCanvas *cc = new TCanvas("cc", "cc", 1000, 400);
	cc->Divide(3,1);

	// draw efficiency vs Egamma
	cc->cd(1);
	locHist_Effic->Draw("p");

	// Draw flux normalized yield vs Egamma
	cc->cd(2);
	locHist_OmegaObservedData->Draw("p");

	// draw "cross section" vs Egamma
	cc->cd(3);
	locHist_OmegaCrossSection->Draw("p");


	return;
}
