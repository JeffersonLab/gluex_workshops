void plot_Z2pi_trees(TString filename)
{
// File: Z2pi_trees.C
//

  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
    
  // TString filename = "DSelector_Z2pi_trees_flat";
  // TString filename = "DSelector_Z2pi_trees_signal";

    
    TFile *f = new TFile(filename+".root","read");
    
    cout << "Opening root file: " << (filename+".root").Data() << endl;
    
    TH1F *NumEventsSurvivedAction = (TH1F*)f->Get("NumEventsSurvivedAction");
    TH2F *NumCombosSurvivedAction = (TH2F*)f->Get("NumCombosSurvivedAction");
    TH1F *BeamEnergy = (TH1F*)f->Get("BeamEnergy");
    // TH1F *pMomentumMeasured = (TH1F*)f->Get("pMomentumMeasured");
    TH1F *piplusMomentumMeasured = (TH1F*)f->Get("piplusMomentumMeasured");
    TH1F *piminusMomentumMeasured = (TH1F*)f->Get("piminusMomentumMeasured");
    
    TH1F *KinFitChiSq = (TH1F*)f->Get("KinFitChiSq");
    TH1F *KinFitCL = (TH1F*)f->Get("KinFitCL");
    TH1F *MissingMassSquared = (TH1F*)f->Get("MissingMassSquared");
    TH1F *M2pikin = (TH1F*)f->Get("M2pikin");
    TH1F *tkin = (TH1F*)f->Get("tkin");
    TH1F *psikin = (TH1F*)f->Get("psikin");
    TH2F *CosTheta_Psi = (TH2F*)f->Get("CosTheta_Psi");
    
    TH1F *M2pidiff = (TH1F*)f->Get("M2pidiff");
    TH1F *tdiff = (TH1F*)f->Get("tdiff");
    TH1F *psidiff = (TH1F*)f->Get("psidiff");
    TH1F *pipDeltap_Measured = (TH1F*)f->Get("pipDeltap_Measured");
    TH1F *pimDeltap_Measured = (TH1F*)f->Get("pimDeltap_Measured");
    // TH1F *pDeltap = (TH1F*)f->Get("pDeltap");
    TH1F *pipDeltap = (TH1F*)f->Get("pipDeltap");
    TH1F *pimDeltap = (TH1F*)f->Get("pimDeltap");

    TH1F *Phikin = (TH1F*)f->Get("Phikin");
    TH1F *phikin = (TH1F*)f->Get("phikin");
    TH1F *CosTheta = (TH1F*)f->Get("CosTheta");
    TH1F *Phidiff = (TH1F*)f->Get("Phidiff");
    TH1F *phidiff = (TH1F*)f->Get("phidiff");
    TH1F *CosThetadiff = (TH1F*)f->Get("CosThetadiff");

    TH1F *dHist_TaggerAccidentals = (TH1F*)f->Get("dHist_TaggerAccidentals");
    
    TCanvas *c0 = new TCanvas("c0", "c0",200,10,1000,700);

   c0->Divide(3,2);
    c0->cd(1);
    // gPad->SetLogy();
    Double_t xmin = 0;
    Double_t xmax = 2;
    Double_t ymin = 100;
    Double_t ymax = 10000;
        
    NumEventsSurvivedAction->SetTitle(filename);
    // NumEventsSurvivedAction->GetXaxis()->SetRangeUser(xmin,xmax);
    // NumEventsSurvivedAction->GetYaxis()->SetRangeUser(ymin,ymax);
    NumEventsSurvivedAction->GetXaxis()->SetTitleSize(0.05);
    NumEventsSurvivedAction->GetYaxis()->SetTitleSize(0.05);
    // NumEventsSurvivedAction->GetXaxis()->SetTitle("Events");
    NumEventsSurvivedAction->SetMarkerColor(4);
    NumEventsSurvivedAction->Draw();
    
    c0->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 5000;
    
    NumCombosSurvivedAction->SetTitle(filename);
    // NumCombosSurvivedAction->GetXaxis()->SetRangeUser(xmin,xmax);
    // NumCombosSurvivedAction->GetYaxis()->SetRangeUser(ymin,ymax);
    NumCombosSurvivedAction->GetXaxis()->SetTitleSize(0.05);
    NumCombosSurvivedAction->GetYaxis()->SetTitleSize(0.05);
    // NumCombosSurvivedAction->GetXaxis()->SetTitle("Events");
    // NumCombosSurvivedAction->SetMarkerColor(4);
    NumCombosSurvivedAction->Draw("colz");
    
    c0->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    BeamEnergy->SetTitle(filename);
    // BeamEnergy->GetXaxis()->SetRangeUser(xmin,xmax);
    // BeamEnergy->GetYaxis()->SetRangeUser(ymin,ymax);
    BeamEnergy->GetXaxis()->SetTitleSize(0.05);
    BeamEnergy->GetYaxis()->SetTitleSize(0.05);
    BeamEnergy->GetXaxis()->SetTitle("Energy (GeV)");
    BeamEnergy->SetMarkerColor(4);
    BeamEnergy->Draw();
    
    c0->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    CosTheta->SetTitle(filename);
    // CosTheta->GetXaxis()->SetRangeUser(xmin,xmax);
    // CosTheta->GetYaxis()->SetRangeUser(ymin,ymax);
    CosTheta->GetXaxis()->SetTitleSize(0.05);
    CosTheta->GetYaxis()->SetTitleSize(0.05);
    CosTheta->GetXaxis()->SetTitle("Cos#Theta");
    CosTheta->SetMarkerColor(4);
    CosTheta->Draw();
    
    c0->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    piplusMomentumMeasured->SetTitle(filename);
    // piplusMomentumMeasured->GetXaxis()->SetRangeUser(xmin,xmax);
    // piplusMomentumMeasured->GetYaxis()->SetRangeUser(ymin,ymax);
    piplusMomentumMeasured->GetXaxis()->SetTitleSize(0.05);
    piplusMomentumMeasured->GetYaxis()->SetTitleSize(0.05);
    piplusMomentumMeasured->GetXaxis()->SetTitle("PiPlus Momentum (GeV)");
    piplusMomentumMeasured->SetMarkerColor(4);
    piplusMomentumMeasured->Draw();
    
    c0->cd(6);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    piminusMomentumMeasured->SetTitle(filename);
    // piminusMomentumMeasured->GetXaxis()->SetRangeUser(xmin,xmax);
    // piminusMomentumMeasured->GetYaxis()->SetRangeUser(ymin,ymax);
    piminusMomentumMeasured->GetXaxis()->SetTitleSize(0.05);
    piminusMomentumMeasured->GetYaxis()->SetTitleSize(0.05);
    piminusMomentumMeasured->GetXaxis()->SetTitle("PiMinus Momentum (GeV)");
    piminusMomentumMeasured->SetMarkerColor(4);
    piminusMomentumMeasured->Draw();
    
    
    TCanvas *c1 = new TCanvas("c1", "c1",200,10,1000,700);
    
    c1->Divide(3,2);
    
    
    c1->cd(1);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    KinFitChiSq->SetTitle(filename);
    // KinFitChiSq->GetXaxis()->SetRangeUser(xmin,xmax);
    // KinFitChiSq->GetYaxis()->SetRangeUser(ymin,ymax);
    KinFitChiSq->GetXaxis()->SetTitleSize(0.05);
    KinFitChiSq->GetYaxis()->SetTitleSize(0.05);
    KinFitChiSq->GetXaxis()->SetTitle("#chi^{2}");
    KinFitChiSq->SetMarkerColor(4);
    KinFitChiSq->Draw();
    
    c1->cd(2);
    gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    KinFitCL->SetTitle(filename);
    // KinFitCL->GetXaxis()->SetRangeUser(xmin,xmax);
    // KinFitCL->GetYaxis()->SetRangeUser(ymin,ymax);
    KinFitCL->GetXaxis()->SetTitleSize(0.05);
    KinFitCL->GetYaxis()->SetTitleSize(0.05);
    KinFitCL->GetXaxis()->SetTitle("Probability");
    KinFitCL->SetMarkerColor(4);
    KinFitCL->Draw();
    
    c1->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    MissingMassSquared->SetTitle(filename);
    // MissingMassSquared->GetXaxis()->SetRangeUser(xmin,xmax);
    // MissingMassSquared->GetYaxis()->SetRangeUser(ymin,ymax);
    MissingMassSquared->GetXaxis()->SetTitleSize(0.05);
    MissingMassSquared->GetYaxis()->SetTitleSize(0.05);
    MissingMassSquared->GetXaxis()->SetTitle("Missing Mass (GeV2)");
    MissingMassSquared->SetMarkerColor(4);
    MissingMassSquared->Draw();
    
    c1->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    M2pikin->SetTitle(filename);
    // M2pikin->GetXaxis()->SetRangeUser(xmin,xmax);
    // M2pikin->GetYaxis()->SetRangeUser(ymin,ymax);
    M2pikin->GetXaxis()->SetTitleSize(0.05);
    M2pikin->GetYaxis()->SetTitleSize(0.05);
    M2pikin->GetXaxis()->SetTitle("Mass 2pi Kin (GeV)");
    M2pikin->SetMarkerColor(4);
    M2pikin->Draw();
    
    c1->cd(5);
    gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    tkin->SetTitle(filename);
    // tkin->GetXaxis()->SetRangeUser(xmin,xmax);
    // tkin->GetYaxis()->SetRangeUser(ymin,ymax);
    tkin->GetXaxis()->SetTitleSize(0.05);
    tkin->GetYaxis()->SetTitleSize(0.05);
    tkin->GetXaxis()->SetTitle("-t Kin (GeV2)");
    tkin->GetXaxis()->SetNdivisions(505);
    tkin->SetMarkerColor(4);
    tkin->Draw();
    
    c1->cd(6);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    CosThetadiff->SetTitle(filename);
    // CosThetadiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // CosThetadiff->GetYaxis()->SetRangeUser(ymin,ymax);
    CosThetadiff->GetXaxis()->SetTitleSize(0.05);
    CosThetadiff->GetYaxis()->SetTitleSize(0.05);
    // CosThetadiff->GetYaxis()->SetTitle("CosTheta");
    CosThetadiff->GetXaxis()->SetTitle("Cos#Theta diff Kin-Gen");
    CosThetadiff->SetMarkerColor(4);
    CosThetadiff->Draw();
    
    
    TCanvas *c2 = new TCanvas("c2", "c2",200,10,1000,700);
    
    c2->Divide(3,2);
    
    c2->cd(1);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    M2pidiff->SetTitle(filename);
    // M2pidiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // M2pidiff->GetYaxis()->SetRangeUser(ymin,ymax);
    M2pidiff->GetXaxis()->SetTitleSize(0.05);
    M2pidiff->GetYaxis()->SetTitleSize(0.05);
    // M2pidiff->GetXaxis()->SetTitle("");
    M2pidiff->SetMarkerColor(4);
    M2pidiff->GetXaxis()->SetNdivisions(505);
    M2pidiff->Fit("gaus");
    M2pidiff->Draw();
    
    c2->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    pipDeltap_Measured->SetTitle(filename);
    // pipDeltap_Measured->GetXaxis()->SetRangeUser(xmin,xmax);
    // pipDeltap_Measured->GetYaxis()->SetRangeUser(ymin,ymax);
    pipDeltap_Measured->GetXaxis()->SetTitleSize(0.05);
    pipDeltap_Measured->GetYaxis()->SetTitleSize(0.05);
    // pipDeltap_Measured->GetXaxis()->SetTitle("#chi^{2}");
    pipDeltap_Measured->SetMarkerColor(4);
    pipDeltap_Measured->Draw();
    
    c2->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    pimDeltap_Measured->SetTitle(filename);
    // pimDeltap_Measured->GetXaxis()->SetRangeUser(xmin,xmax);
    // pimDeltap_Measured->GetYaxis()->SetRangeUser(ymin,ymax);
    pimDeltap_Measured->GetXaxis()->SetTitleSize(0.05);
    pimDeltap_Measured->GetYaxis()->SetTitleSize(0.05);
    // pimDeltap_Measured->GetXaxis()->SetTitle("#chi^{2}");
    pimDeltap_Measured->SetMarkerColor(4);
    pimDeltap_Measured->Draw();
    
    c2->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    tdiff->SetTitle(filename);
    // tdiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // tdiff->GetYaxis()->SetRangeUser(ymin,ymax);
    tdiff->GetXaxis()->SetTitleSize(0.05);
    tdiff->GetYaxis()->SetTitleSize(0.05);
    // tdiff->GetXaxis()->SetTitle("");
    tdiff->SetMarkerColor(4);
    tdiff->GetXaxis()->SetNdivisions(505);
    tdiff->Fit("gaus");
    tdiff->Draw();
    
    c2->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    pipDeltap->SetTitle(filename);
    // pipDeltap->GetXaxis()->SetRangeUser(xmin,xmax);
    // pipDeltap->GetYaxis()->SetRangeUser(ymin,ymax);
    pipDeltap->GetXaxis()->SetTitleSize(0.05);
    pipDeltap->GetYaxis()->SetTitleSize(0.05);
    // pipDeltap->GetXaxis()->SetTitle("#chi^{2}");
    pipDeltap->SetMarkerColor(4);
    pipDeltap->Draw();
    
    c2->cd(6);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    pimDeltap->SetTitle(filename);
    // pimDeltap->GetXaxis()->SetRangeUser(xmin,xmax);
    // pimDeltap->GetYaxis()->SetRangeUser(ymin,ymax);
    pimDeltap->GetXaxis()->SetTitleSize(0.05);
    pimDeltap->GetYaxis()->SetTitleSize(0.05);
    // pimDeltap->GetXaxis()->SetTitle("#chi^{2}");
    pimDeltap->SetMarkerColor(4);
    pimDeltap->Draw();

    
    TCanvas *c3 = new TCanvas("c3", "c3",200,10,1000,700);
    
    c3->Divide(3,2);

    c3->cd(1);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    Phikin->SetTitle(filename);
    // Phikin->GetXaxis()->SetRangeUser(xmin,xmax);
    // Phikin->GetYaxis()->SetRangeUser(ymin,ymax);
    Phikin->GetXaxis()->SetTitleSize(0.05);
    Phikin->GetYaxis()->SetTitleSize(0.05);
    Phikin->GetXaxis()->SetTitle("#Phi Kin (Degrees)");
    Phikin->SetMarkerColor(4);
    Phikin->Draw();

    c3->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    phikin->SetTitle(filename);
    // phikin->GetXaxis()->SetRangeUser(xmin,xmax);
    // phikin->GetYaxis()->SetRangeUser(ymin,ymax);
    phikin->GetXaxis()->SetTitleSize(0.05);
    phikin->GetYaxis()->SetTitleSize(0.05);
    phikin->GetXaxis()->SetTitle("#phi Kin (Degrees)");
    phikin->SetMarkerColor(4);
    phikin->Draw();

    c3->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    psikin->SetTitle(filename);
    // psikin->GetXaxis()->SetRangeUser(xmin,xmax);
    // psikin->GetYaxis()->SetRangeUser(ymin,ymax);
    psikin->GetXaxis()->SetTitleSize(0.05);
    psikin->GetYaxis()->SetTitleSize(0.05);
    psikin->GetXaxis()->SetTitle("#psii Kin (Degrees)");
    psikin->SetMarkerColor(4);
    psikin->Draw();

    c3->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    Phidiff->SetTitle(filename);
    // Phidiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // Phidiff->GetYaxis()->SetRangeUser(ymin,ymax);
    Phidiff->GetXaxis()->SetTitleSize(0.05);
    Phidiff->GetYaxis()->SetTitleSize(0.05);
    Phidiff->GetXaxis()->SetTitle("#Phi Kin-Gen (Degrees)");
    Phidiff->SetMarkerColor(4);
    Phidiff->Draw();

    c3->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    phidiff->SetTitle(filename);
    // phidiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // phidiff->GetYaxis()->SetRangeUser(ymin,ymax);
    phidiff->GetXaxis()->SetTitleSize(0.05);
    phidiff->GetYaxis()->SetTitleSize(0.05);
    phidiff->GetXaxis()->SetTitle("#phi Kin-Gen (Degrees)");
    phidiff->SetMarkerColor(4);
    phidiff->Draw();

    c3->cd(6);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    psidiff->SetTitle(filename);
    // psidiff->GetXaxis()->SetRangeUser(xmin,xmax);
    // psidiff->GetYaxis()->SetRangeUser(ymin,ymax);
    psidiff->GetXaxis()->SetTitleSize(0.05);
    psidiff->GetYaxis()->SetTitleSize(0.05);
    psidiff->GetXaxis()->SetTitle("#psi Kin-Gen (Degrees)");
    psidiff->SetMarkerColor(4);
    psidiff->Draw();

    TCanvas *c4 = new TCanvas("c4", "c4",200,10,1000,700);
    
    c4->Divide(3,2);

    c4->cd(1);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000;
    
    dHist_TaggerAccidentals->SetTitle(filename);
    // dHist_TaggerAccidentals->GetXaxis()->SetRangeUser(xmin,xmax);
    // dHist_TaggerAccidentals->GetYaxis()->SetRangeUser(ymin,ymax);
    dHist_TaggerAccidentals->GetXaxis()->SetTitleSize(0.05);
    dHist_TaggerAccidentals->GetYaxis()->SetTitleSize(0.05);
    dHist_TaggerAccidentals->GetXaxis()->SetTitle("Vertex Time - RF (ns)");
    dHist_TaggerAccidentals->SetMarkerColor(4);
    dHist_TaggerAccidentals->Draw();
    
    c0->SaveAs(filename+".pdf(");
    c1->SaveAs(filename+".pdf");
    c2->SaveAs(filename+".pdf");
    c3->SaveAs(filename+".pdf");
    c4->SaveAs(filename+".pdf)");
}
