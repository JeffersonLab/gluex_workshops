void CalculateCrossSec(TString flux_file, Double_t yield, Double_t yield_uncert)
{
	cout << "Determining an a_2(1320) example cross section extraction..." << endl;	
	cout << "Opening file: " << flux_file << endl;
	cout << "a2(1320) efficiency-corrected yield provided: " << yield << " +/- " << yield_uncert << endl;
	cout << "(note that uncertainties here are statistical only, uncertainties from branching fractions and flux have not been incorporated)" << endl << endl;
	
	TFile* f_flux = TFile::Open(flux_file);
	TH1F*  h_tagged_lumi = (TH1F*)f_flux->Get("tagged_lumi"); // in pb^-1
	Double_t tagged_lumi_inv_nb = h_tagged_lumi->Integral() * 1000; // Factor of 1000 to convert from pb^-1 to nb^-1

	Double_t a2_to_etapi0_BF = 0.145;  // Correct for MC decaying a2->eta pi0 100% of time. Obtained from Particle Data Group (https://pdglive.lbl.gov/Viewer.action)
	Double_t eta_to_gg_BF    = 0.3941; // Correct for MC decaying eta->gg     100% of time. Obtained from Particle Data Group (https://pdglive.lbl.gov/Viewer.action)
	Double_t tot_BF = a2_to_etapi0_BF*eta_to_gg_BF; // Note: we skip pi0->gg branching fraction because MC did NOT assume it decays 100% of time.
	
	Double_t xsec_nb        = yield/(tagged_lumi_inv_nb*tot_BF);
	Double_t xsec_uncert_nb = yield_uncert/(tagged_lumi_inv_nb*tot_BF);
	
	cout << "Cross section for a2(1320) example (8.2<E_beam<8.8 GeV, 0.1<|t|<0.3 GeV^2): " << endl;
	cout << " " << xsec_nb << " +/- " << xsec_uncert_nb << " (nanobarns)" << endl << endl;
	cout << "Or, as a differential cross section dsigma/dt, in |t| range 0.1-0.3: " << endl;
	Double_t Delta_t = 0.2;
	cout << " " << xsec_nb/Delta_t << " +/- " << xsec_uncert_nb/Delta_t << " (nanobarns/GeV^2)" << endl << endl;
	
	
	return;
}	