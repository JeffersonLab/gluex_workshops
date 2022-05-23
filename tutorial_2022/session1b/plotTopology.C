
void plotTopology() {

	gStyle->SetOptStat("");
	gStyle->SetPaintTextFormat("4.3f"); // set precision for histogram text
	gStyle->SetLegendBorderSize(0);

	TFile *f = TFile::Open("hist_etapi_bggen.acc.root");

	// plot percentages for different thrown topologies
	TCanvas *bb = new TCanvas("bb","bb",600,400);
	TH1F *hThrownTopologies = (TH1F*)f->Get("hThrownTopologies");
	hThrownTopologies->GetXaxis()->LabelsOption(">"); // order by most common topology
	hThrownTopologies->GetXaxis()->SetRangeUser(0, 10); // only plot first 10 topologies
	hThrownTopologies->Scale(100./hThrownTopologies->GetEntries()); // turn histogram into percentage
	hThrownTopologies->GetYaxis()->SetTitle("Thrown Topology %");
	hThrownTopologies->Draw("htext");

	// draw invariant mass histograms for different thrown topologies
	TCanvas *cc = new TCanvas("cc","cc",600,400);
	TLegend *leg = new TLegend(0.5,0.6,0.89,0.89);
	leg->SetNColumns(2);
	THStack *hStack = new THStack("hstack","");

	int locNumTopologies = 10;
	for(int i=1; i<locNumTopologies; i++) {
		TH1I *hMassThrownTopology = (TH1I*)f->Get(Form("hInvariantMass_ThrownTopology_%d", i));
		//hMassThrownTopology = (TH1I*)f->Get(Form("hKinFitChi2_ThrownTopology_%d", i));
		//hMassThrownTopology = (TH1I*)f->Get(Form("hUnusedShowerEnergy_ThrownTopology_%d", i));
		if(!hMassThrownTopology) break;
		hMassThrownTopology->SetLineColor(kBlack+i-1);
		hMassThrownTopology->SetFillColor(kBlack+i-1);
		hMassThrownTopology->SetMarkerColor(kBlack+i-1);
		//hMassThrownTopology->Rebin(10);

		TString locLegendTitle = hMassThrownTopology->GetTitle();
		locLegendTitle.ReplaceAll("Invariant Mass Topology:","");
		//locLegendTitle.ReplaceAll("Kinematic Fit #Chi^{2}/NDF: Topology","");
		//locLegendTitle.ReplaceAll("Unused shower energy: Topology","");
		leg->AddEntry(hMassThrownTopology,locLegendTitle,"f");

		//hMassThrownTopology->GetXaxis()->SetRangeUser(0, 1.0);
		hMassThrownTopology->SetMarkerStyle(20);
		

		if(i==1) {
			//hMassThrownTopology->SetTitle("#eta#pi^{0} Invariant Mass; M_{#eta#pi^{0}} (GeV)");
			hMassThrownTopology->Draw("p");
		}
		else 
			hMassThrownTopology->Draw("psame");
		hStack->Add(hMassThrownTopology);
	}

	leg->Draw("same");

	TCanvas *dd = new TCanvas("dd","dd",600,400);
	hStack->Draw();
	leg->Draw("same");

	bb->Print("TopologyPercentage.pdf");

	//cc->SetLogy();
	cc->Print("TopologyInvariantMass.pdf");

	return;
}
