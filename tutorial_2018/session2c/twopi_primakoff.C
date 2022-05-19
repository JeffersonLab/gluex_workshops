void twopi_primakoff(TString filename, Int_t maxev=100000)
{
// File: twopi_primakoff.C
    // Output histograms and fits generated from amp fitting of parameters.
//

  gStyle->SetPalette(1,0);
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);

  char string[256];
    vector <TString> sdme;
    
    /*const Int_t nparms=8;
    Double_t parms[nparms];
    Double_t parms_err[nparms];*/
    vector <double> parms;
    vector <double> parms_err;
    
    bool setscale(true);
    
    // TString filename = "twopi_primakoff_gen";
    // Double_t scale_factor=0.5;       // divide ymax/scale_factor
    // TString filename = "twopi_primakoff_DSelect";
    // TString filename = "twopi_primakoff_DSelect_thrown_mod";
    // TString filename = "twopi_primakoff_DSelect_thrown_mod2";
    Double_t scale_factor=400000/(float)maxev;       // divide ymax/scale_factor
    cout << "Use scale_factor=" << scale_factor << " maxev=" << maxev << endl;
    
    TString infile = filename+".fit2";   // file with parameters
    TFile *f = new TFile(filename+".root","read");
    
    cout << "Opening parameters file: " << infile.Data() << endl;
    cout << "Opening root file: " << (filename+".root").Data() << endl;
    
    
    TH1F *M2pigen = (TH1F*)f->Get("M2pigen");
    TH1F *M2piacc = (TH1F*)f->Get("M2piacc");
    TH1F *M2pidat = (TH1F*)f->Get("M2pidat");
    TH1F *M2pibkgnd = (TH1F*)f->Get("M2pibkgnd");
    TH1F *M2pidatsub = (TH1F*)M2pidat->Clone("M2pidatsub");
    M2pidatsub->Add(M2pibkgnd,-1);
    
    TH1F *cosThetagen = (TH1F*)f->Get("cosThetagen");
    TH1F *cosThetaacc = (TH1F*)f->Get("cosThetaacc");
    TH1F *cosThetadat = (TH1F*)f->Get("cosThetadat");
    TH1F *cosThetabkgnd = (TH1F*)f->Get("cosThetabkgnd");
    TH1F *cosThetadatsub = (TH1F*)cosThetadat->Clone("cosThetadatsub");
    cosThetadatsub->Add(cosThetabkgnd,-1);
    
    TH1F *psigen = (TH1F*)f->Get("psigen");
    TH1F *psiacc = (TH1F*)f->Get("psiacc");
    TH1F *psidat = (TH1F*)f->Get("psidat");
    TH1F *psibkgnd = (TH1F*)f->Get("psibkgnd");
    TH1F *psidatsub = (TH1F*)psidat->Clone("psidatsub");
    psidatsub->Add(psibkgnd,-1);
    
    TH1F *Phigen = (TH1F*)f->Get("Phigen");
    TH1F *Phiacc = (TH1F*)f->Get("Phiacc");
    TH1F *Phidat = (TH1F*)f->Get("Phidat");
    TH1F *Phibkgnd = (TH1F*)f->Get("Phibkgnd");
    TH1F *Phidatsub = (TH1F*)Phidat->Clone("Phidatsub");
    Phidatsub->Add(Phibkgnd,-1);
    
    TH1F *phigen = (TH1F*)f->Get("phigen");
    TH1F *phiacc = (TH1F*)f->Get("phiacc");
    TH1F *phidat = (TH1F*)f->Get("phidat");
    TH1F *phibkgnd = (TH1F*)f->Get("phibkgnd");
    TH1F *phidatsub = (TH1F*)phidat->Clone("phidatsub");
    phidatsub->Add(phibkgnd,-1);
    
    TH1F *tgen = (TH1F*)f->Get("tgen");
    TH1F *tacc = (TH1F*)f->Get("tacc");
    TH1F *tdat = (TH1F*)f->Get("tdat");
    TH1F *tbkgnd = (TH1F*)f->Get("tbkgnd");
    TH1F *tdatsub = (TH1F*)tdat->Clone("tdatsub");
    tdatsub->Add(tbkgnd,-1);
    
    
   TCanvas *c0 = new TCanvas("c0", "c0",200,10,1000,700);

   c0->Divide(3,2);
    c0->cd(1);
    // gPad->SetLogy();
    Double_t xmin = 0.2;
    Double_t xmax = 0.8;
    Double_t ymin = 0/scale_factor;
    Double_t ymax = 4000/scale_factor;
        
    M2pigen->SetTitle(filename);
    M2pigen->GetXaxis()->SetRangeUser(xmin,xmax);
    M2pigen->GetYaxis()->SetRangeUser(ymin,ymax);
    M2pigen->GetXaxis()->SetTitleSize(0.05);
    M2pigen->GetYaxis()->SetTitleSize(0.05);
    M2pigen->GetXaxis()->SetTitle("M(#pi^{+}#pi^{-})");
    M2pigen->SetMarkerColor(4);
    // M2piacc->Draw("samep");
    M2pidat->SetMarkerColor(2);
    M2pidat->SetLineColor(2);
    M2pidat->SetMarkerStyle(20);
    M2pidat->SetMarkerSize(0.1);
    M2pibkgnd->SetMarkerStyle(20);
    M2pibkgnd->SetMarkerSize(0.1);
    M2pibkgnd->SetMarkerColor(1);
    M2pibkgnd->SetLineColor(1);
    M2pigen->Draw("p");
    M2pidat->Draw("samep");
    M2pibkgnd->Draw("samep");
    
    TLegend *leg = new TLegend(0.6,0.3,0.8,0.5);
    leg->AddEntry(M2pigen,"Gen","lp");
    leg->AddEntry(M2pibkgnd,"Bkgnd","lp");
    leg->AddEntry(M2pidat,"Data","lp");
    leg->Draw();
    
    c0->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 10000/scale_factor;
    
    cosThetagen->SetTitle(filename);
    // cosThetagen->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) cosThetagen->GetYaxis()->SetRangeUser(ymin,ymax);
    cosThetagen->GetXaxis()->SetTitleSize(0.05);
    cosThetagen->GetYaxis()->SetTitleSize(0.05);
    cosThetagen->GetXaxis()->SetTitle("cos(#theta)");
    cosThetagen->SetLineColor(4);
    cosThetagen->Draw("p");
    // cosThetaacc->Draw("samep");
    cosThetadat->SetMarkerColor(2);
    cosThetadat->SetLineColor(2);
    cosThetadat->SetMarkerStyle(20);
    cosThetadat->SetMarkerSize(0.1);
    cosThetabkgnd->SetMarkerStyle(20);
    cosThetabkgnd->SetMarkerSize(0.1);
    cosThetabkgnd->SetMarkerColor(1);
    cosThetabkgnd->SetLineColor(1);
    cosThetadat->Draw("samep");
    cosThetabkgnd->Draw("samep");
    
    c0->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 10000/scale_factor;
    
    TF1 *cos2phi = new TF1("cos2phi","[0]*(1+[1]*cos(2*x))",-3.14159,3.14159);
    TF1 *cosphi = new TF1("cosphi","[0]+[1]*cos(x)",-3.14159,3.14159);
    
    psigen->SetTitle(filename);
    // psigen->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) psigen->GetYaxis()->SetRangeUser(ymin,ymax);
    psigen->GetXaxis()->SetTitleSize(0.05);
    psigen->GetYaxis()->SetTitleSize(0.05);
    psigen->GetXaxis()->SetTitle("#psi");
    psigen->SetMarkerColor(4);
    psigen->Fit(cos2phi);
    psigen->Draw("p");
    // psiacc->Draw("samep");
    psidat->SetMarkerColor(2);
    psidat->SetLineColor(2);
    psidat->SetMarkerStyle(20);
    psidat->SetMarkerSize(0.1);
    psibkgnd->SetMarkerStyle(20);
    psibkgnd->SetMarkerSize(0.1);
    psibkgnd->SetMarkerColor(1);
    psibkgnd->SetLineColor(1);
    psidat->Draw("samep");
    psibkgnd->Draw("samep");
    
    c0->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 8000/scale_factor;
    
    Phigen->SetTitle(filename);
    // Phigen->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) Phigen->GetYaxis()->SetRangeUser(ymin,ymax);
    Phigen->GetXaxis()->SetTitleSize(0.05);
    Phigen->GetYaxis()->SetTitleSize(0.05);
    Phigen->GetXaxis()->SetTitle("#Phi");
    Phigen->SetMarkerColor(4);
    Phigen->Fit(cos2phi);
    Phigen->Draw("p");
    // Phiacc->Draw("samep");
    Phidat->SetMarkerColor(2);
    Phidat->SetLineColor(2);
    Phidat->SetMarkerStyle(20);
    Phidat->SetMarkerSize(0.1);
    Phibkgnd->SetMarkerStyle(20);
    Phibkgnd->SetMarkerSize(0.1);
    Phibkgnd->SetMarkerColor(1);
    Phibkgnd->SetLineColor(1);
    Phidat->Draw("samep");
    Phibkgnd->Draw("samep");
    
    c0->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 8000/scale_factor;
    
    phigen->SetTitle(filename);
    // phigen->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) phigen->GetYaxis()->SetRangeUser(ymin,ymax);
    phigen->GetXaxis()->SetTitleSize(0.05);
    phigen->GetYaxis()->SetTitleSize(0.05);
    phigen->GetXaxis()->SetTitle("#phi");
    phigen->SetMarkerColor(4);
    phigen->Fit(cosphi);
    phigen->Draw("p");
    // phiacc->Draw("samep");
    phidat->SetMarkerColor(2);
    phidat->SetLineColor(2);
    phidat->SetMarkerStyle(20);
    phidat->SetMarkerSize(0.1);
    phibkgnd->SetMarkerStyle(20);
    phibkgnd->SetMarkerSize(0.1);
    phibkgnd->SetMarkerColor(1);
    phibkgnd->SetLineColor(1);
    phidat->Draw("samep");
    phibkgnd->Draw("samep");
    
    c0->cd(6);
    gPad->SetLogy(); // use  gPad->SetLogy();
    xmin = 0;
    xmax = 0.012;
    ymin = 100/scale_factor;
    ymax = 400000/scale_factor;
    
    tdat->SetTitle(filename);
    tdat->GetXaxis()->SetTitleSize(0.05);
    tdat->GetYaxis()->SetTitleSize(0.05);
    tdat->GetXaxis()->SetTitle("-t");
    // tacc->Draw("samep");
    tdat->GetXaxis()->SetRangeUser(xmin,xmax);
    tdat->GetYaxis()->SetRangeUser(ymin,ymax);
    tdat->SetMarkerColor(2);
    tdat->SetLineColor(2);
    tdat->SetMarkerStyle(20);
    tdat->SetMarkerSize(0.1);
    tdat->Fit("expo","","",0.002,0.01);
    tdat->Draw("p");
    tgen->SetMarkerColor(4);
    tbkgnd->SetMarkerStyle(20);
    tbkgnd->SetMarkerSize(0.1);
    tbkgnd->SetMarkerColor(1);
    tbkgnd->SetLineColor(1);
    tgen->Draw("samep");
    tbkgnd->Draw("samep");
    
    
    TCanvas *c2 = new TCanvas("c2", "c2",200,10,1000,700);
    
    c2->Divide(3,2);
    c2->cd(1);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 1.2;
    
    TH1F *M2piAcceptance = (TH1F*)M2piacc->Clone("M2piAcceptance");
    M2piAcceptance->SetTitle("Acceptance");
    // M2piAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) M2piAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    M2piAcceptance->Divide(M2pigen);
    M2piAcceptance->GetXaxis()->SetTitleSize(0.05);
    M2piAcceptance->GetYaxis()->SetTitleSize(0.05);
    M2piAcceptance->GetXaxis()->SetTitle("M(#pi^{+}#pi^{-})");
    M2piAcceptance->SetMarkerColor(4);
    M2piAcceptance->Draw("p");
    
    c2->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    // ymax = 1.2;
    
    TH1F *cosThetaAcceptance = (TH1F*)cosThetaacc->Clone("cosThetaAcceptance");
    cosThetaAcceptance->SetTitle("Acceptance");
    // cosThetaAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) cosThetaAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    cosThetaAcceptance->Divide(cosThetagen);
    cosThetaAcceptance->GetXaxis()->SetTitleSize(0.05);
    cosThetaAcceptance->GetYaxis()->SetTitleSize(0.05);
    cosThetaAcceptance->GetXaxis()->SetTitle("cos(#theta)");
    cosThetaAcceptance->SetMarkerColor(4);
    cosThetaAcceptance->Draw("p");
    
    c2->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    // ymax = 1.2;
    
    TH1F *psiAcceptance = (TH1F*)psiacc->Clone("psiAcceptance");
    psiAcceptance->SetTitle("Acceptance");
    // psiAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) psiAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    psiAcceptance->Divide(psigen);
    psiAcceptance->GetXaxis()->SetTitleSize(0.05);
    psiAcceptance->GetYaxis()->SetTitleSize(0.05);
    psiAcceptance->GetXaxis()->SetTitle("#psi");
    psiAcceptance->SetMarkerColor(4);
    psiAcceptance->Draw("p");
    
    c2->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    // ymax = 1.2;
    
    TH1F *PhiAcceptance = (TH1F*)Phiacc->Clone("PhiAcceptance");
    PhiAcceptance->SetTitle("Acceptance");
    // PhiAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) PhiAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    PhiAcceptance->Divide(Phigen);
    PhiAcceptance->GetXaxis()->SetTitleSize(0.05);
    PhiAcceptance->GetYaxis()->SetTitleSize(0.05);
    PhiAcceptance->GetXaxis()->SetTitle("#Phi");
    PhiAcceptance->SetMarkerColor(4);
    PhiAcceptance->Draw("p");
    
    c2->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    // ymax = 1.2;
    
    TH1F *phiAcceptance = (TH1F*)phiacc->Clone("phiAcceptance");
    phiAcceptance->SetTitle("Acceptance");
    // phiAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) phiAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    phiAcceptance->Divide(phigen);
    phiAcceptance->GetXaxis()->SetTitleSize(0.05);
    phiAcceptance->GetYaxis()->SetTitleSize(0.05);
    phiAcceptance->GetXaxis()->SetTitle("#phi");
    phiAcceptance->SetMarkerColor(4);
    phiAcceptance->Draw("p");
    
    c2->cd(6);
    // gPad->SetLogy();
    ymin = 0;
    // ymax = 1.2;
    xmin = 0;
    xmax = 0.012;
    
    TH1F *tAcceptance = (TH1F*)tacc->Clone("tAcceptance");
    tAcceptance->SetTitle("Acceptance");
    tAcceptance->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) tAcceptance->GetYaxis()->SetRangeUser(ymin,ymax);
    tAcceptance->Divide(tgen);
    tAcceptance->GetXaxis()->SetTitleSize(0.05);
    tAcceptance->GetYaxis()->SetTitleSize(0.05);
    tAcceptance->GetXaxis()->SetTitle("-t");
    tAcceptance->SetMarkerColor(4);
    tAcceptance->Draw("p");
    
    TCanvas *c1 = new TCanvas("c1", "c1",200,10,1000,700);
    
    c1->Divide(3,2);
    c1->cd(1);
    // gPad->SetLogy();
    ymin = 0/scale_factor;
    ymax = 2000/scale_factor;
    
    M2piacc->SetTitle(filename);
    M2piacc->GetXaxis()->SetRangeUser(xmin,xmax);
    M2piacc->GetYaxis()->SetRangeUser(ymin,ymax);
    M2piacc->GetXaxis()->SetTitleSize(0.05);
    M2piacc->GetYaxis()->SetTitleSize(0.05);
    M2piacc->GetXaxis()->SetTitle("M(#pi^{+}#pi^{-})");
    M2piacc->SetMarkerColor(1);
    M2piacc->SetLineColor(1);
    // M2piacc->Draw("samep");
    M2pidat->SetMarkerColor(2);
    M2pidat->SetLineColor(2);
    M2pidat->SetMarkerStyle(20);
    M2pidat->SetMarkerSize(0.1);
    M2pibkgnd->SetMarkerColor(4);
    M2pibkgnd->SetLineColor(4);
    M2pibkgnd->SetMarkerStyle(20);
    M2pibkgnd->SetMarkerSize(0.1);
    M2piacc->Draw("p");
    M2pidat->Draw("samep");
    M2pibkgnd->Draw("samep");
    
    TLegend *leg1 = new TLegend(0.6,0.3,0.8,0.5);
    // leg1->AddEntry(M2pigen,"Gen","lp");
    leg1->AddEntry(M2piacc,"Acc","lp");
    leg1->AddEntry(M2pidat,"Data","lp");
    leg1->Draw();
    
    c1->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    cosThetaacc->SetTitle(filename);
    // cosThetaacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) cosThetaacc->GetYaxis()->SetRangeUser(ymin,ymax);
    cosThetaacc->GetXaxis()->SetTitleSize(0.05);
    cosThetaacc->GetYaxis()->SetTitleSize(0.05);
    cosThetaacc->GetXaxis()->SetTitle("cos(#theta)");
    cosThetaacc->SetLineColor(1);
    cosThetaacc->SetMarkerColor(1);
    cosThetaacc->Draw("p");
    // cosThetaacc->Draw("samep");
    cosThetadat->SetMarkerColor(2);
    cosThetadat->SetLineColor(2);
    cosThetadat->SetMarkerStyle(20);
    cosThetadat->SetMarkerSize(0.1);
    cosThetabkgnd->SetMarkerColor(4);
    cosThetabkgnd->SetLineColor(4);
    cosThetabkgnd->SetMarkerStyle(20);
    cosThetabkgnd->SetMarkerSize(0.1);
    cosThetadat->Draw("samep");
    cosThetabkgnd->Draw("samep");
    
    c1->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    psiacc->SetTitle(filename);
    // psiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) psiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    psiacc->GetXaxis()->SetTitleSize(0.05);
    psiacc->GetYaxis()->SetTitleSize(0.05);
    psiacc->GetXaxis()->SetTitle("#psi");
    psiacc->SetMarkerColor(1);
    psiacc->SetLineColor(1);
    psiacc->Fit(cos2phi);
    psiacc->Draw("p");
    // psiacc->Draw("samep");
    psidat->SetMarkerColor(2);
    psidat->SetLineColor(2);
    psidat->SetMarkerStyle(20);
    psidat->SetMarkerSize(0.1);
    psibkgnd->SetMarkerColor(4);
    psibkgnd->SetLineColor(4);
    psibkgnd->SetMarkerStyle(20);
    psibkgnd->SetMarkerSize(0.1);
    psidat->Draw("samep");
    psibkgnd->Draw("samep");
    
    c1->cd(4);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    Phiacc->SetTitle(filename);
    // Phiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) Phiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    Phiacc->GetXaxis()->SetTitleSize(0.05);
    Phiacc->GetYaxis()->SetTitleSize(0.05);
    Phiacc->GetXaxis()->SetTitle("#Phi");
    Phiacc->SetMarkerColor(1);
    Phiacc->SetLineColor(1);
    Phiacc->Fit(cos2phi);
    Phiacc->Draw("p");
    // Phiacc->Draw("samep");
    Phidat->SetMarkerColor(2);
    Phidat->SetLineColor(2);
    Phidat->SetMarkerStyle(20);
    Phidat->SetMarkerSize(0.1);
    Phibkgnd->SetMarkerColor(4);
    Phibkgnd->SetLineColor(4);
    Phibkgnd->SetMarkerStyle(20);
    Phibkgnd->SetMarkerSize(0.1);
    Phidat->Draw("samep");
    Phibkgnd->Draw("samep");
    
    
    c1->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    phiacc->SetTitle(filename);
    // phiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) phiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    phiacc->GetXaxis()->SetTitleSize(0.05);
    phiacc->GetYaxis()->SetTitleSize(0.05);
    phiacc->GetXaxis()->SetTitle("#phi");
    phiacc->SetMarkerColor(1);
    phiacc->SetLineColor(1);
    phiacc->Fit(cosphi);
    phiacc->Draw("p");
    // phiacc->Draw("samep");
    phidat->SetMarkerColor(2);
    phidat->SetLineColor(2);
    phidat->SetMarkerStyle(20);
    phidat->SetMarkerSize(0.1);
    phibkgnd->SetMarkerColor(4);
    phibkgnd->SetLineColor(4);
    phibkgnd->SetMarkerStyle(20);
    phibkgnd->SetMarkerSize(0.1);
    phidat->Draw("samep");
    phibkgnd->Draw("samep");
    
    c1->cd(6);
    gPad->SetLogy();  // use  gPad->SetLogy();
    xmin = 0;
    xmax = 0.012;
    ymin = 10/scale_factor;
    ymax = 400000/scale_factor;
    
    tdat->SetTitle(filename);
    tdat->GetXaxis()->SetTitleSize(0.05);
    tdat->GetYaxis()->SetTitleSize(0.05);
    tdat->GetXaxis()->SetTitle("-t");
    // tacc->Draw("samep");
    tdat->GetXaxis()->SetRangeUser(xmin,xmax);
    tdat->GetYaxis()->SetRangeUser(ymin,ymax);
    tdat->SetMarkerColor(2);
    tdat->SetLineColor(2);
    tdat->SetMarkerStyle(20);
    tdat->SetMarkerSize(0.1);
    tdat->Draw("p");
    tacc->SetMarkerColor(1);
    tacc->SetLineColor(1);
    tbkgnd->SetMarkerColor(4);
    tbkgnd->SetLineColor(4);
    tbkgnd->SetMarkerStyle(20);
    tbkgnd->SetMarkerSize(0.1);
    tacc->Draw("samep");
    tbkgnd->Draw("samep");
    
    TCanvas *c3 = new TCanvas("c3", "c3",200,10,1000,700);
    
    c3->Divide(3,2);
    c3->cd(1);
    // gPad->SetLogy();
    ymin = 100;
    ymax = 10000/scale_factor;
    
    M2piacc->SetTitle(filename);
    // M2piacc->GetXaxis()->SetRangeUser(xmin,xmax);
    // M2piacc->GetYaxis()->SetRangeUser(ymin,ymax);
    M2piacc->GetXaxis()->SetTitleSize(0.05);
    M2piacc->GetYaxis()->SetTitleSize(0.05);
    M2piacc->GetXaxis()->SetTitle("M(#pi^{+}#pi^{-})");
    M2piacc->SetMarkerColor(1);
    // M2piacc->Draw("samep");
    M2pidatsub->SetMarkerColor(2);
    M2pidatsub->SetLineColor(2);
    M2pidatsub->SetMarkerStyle(20);
    M2pidatsub->SetMarkerSize(0.1);
    M2piacc->Draw("p");
    M2pidatsub->Draw("samep");
    
    c3->cd(2);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    cosThetaacc->SetTitle(filename);
    // cosThetaacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) cosThetaacc->GetYaxis()->SetRangeUser(ymin,ymax);
    cosThetaacc->GetXaxis()->SetTitleSize(0.05);
    cosThetaacc->GetYaxis()->SetTitleSize(0.05);
    cosThetaacc->GetXaxis()->SetTitle("cos(#theta)");
    cosThetaacc->SetLineColor(1);
    cosThetaacc->Draw("p");
    // cosThetaacc->Draw("samep");
    cosThetadatsub->SetMarkerColor(2);
    cosThetadatsub->SetLineColor(2);
    cosThetadatsub->SetMarkerStyle(20);
    cosThetadatsub->SetMarkerSize(0.1);
    cosThetadatsub->Draw("samep");
    
    c3->cd(3);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    // TF1 *cos2phi = new TF1("cos2phi","[0]*(1+[1]*cos(2*x))",-3.14159,3.14159);
    
    Phiacc->SetTitle(filename);
    // Phiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) Phiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    Phiacc->GetXaxis()->SetTitleSize(0.05);
    Phiacc->GetYaxis()->SetTitleSize(0.05);
    Phiacc->GetXaxis()->SetTitle("#Phi");
    Phiacc->SetMarkerColor(1);
    Phiacc->Fit(cos2phi);
    Phiacc->Draw("p");
    // Phiacc->Draw("samep");
    Phidatsub->SetMarkerColor(2);
    Phidatsub->SetLineColor(2);
    Phidatsub->SetMarkerStyle(20);
    Phidatsub->SetMarkerSize(0.1);
    Phidatsub->Draw("samep");
    
    
    c3->cd(4);


    ymin = 0;
    ymax = 4000/scale_factor;
    
    psiacc->SetTitle(filename);
    // psiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) psiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    psiacc->GetXaxis()->SetTitleSize(0.05);
    psiacc->GetYaxis()->SetTitleSize(0.05);
    psiacc->GetXaxis()->SetTitle("#psi");
    psiacc->SetMarkerColor(1);
    psiacc->Fit(cos2phi);
    psiacc->Draw("p");
    // psiacc->Draw("samep");
    psidatsub->SetMarkerColor(2);
    psidatsub->SetLineColor(2);
    psidatsub->SetMarkerStyle(20);
    psidatsub->SetMarkerSize(0.1);
    psidatsub->Draw("samep");

    
    c3->cd(5);
    // gPad->SetLogy();
    ymin = 0;
    ymax = 4000/scale_factor;
    
    phiacc->SetTitle(filename);
    // phiacc->GetXaxis()->SetRangeUser(xmin,xmax);
    if (setscale) phiacc->GetYaxis()->SetRangeUser(ymin,ymax);
    phiacc->GetXaxis()->SetTitleSize(0.05);
    phiacc->GetYaxis()->SetTitleSize(0.05);
    phiacc->GetXaxis()->SetTitle("#phi");
    phiacc->SetMarkerColor(1);
    phiacc->SetLineColor(1);
    phiacc->Fit(cosphi);
    phiacc->Draw("p");
    // phiacc->Draw("samep");
    phidatsub->SetMarkerColor(2);
    phidatsub->SetLineColor(2);
    phidatsub->SetMarkerStyle(20);
    phidatsub->SetMarkerSize(0.1);
    phidatsub->Draw("samep");

    c3->cd(6);
    
    // now read and print fitted values
    
    ifstream parameters;
    parameters.open (infile.Data());
    if (!parameters) {
        cout << "ERROR: Failed to open data file= " << infile.Data() << endl;
        return;
    }
    
    TString line;
    while (line.ReadLine(parameters)){
        
        TObjArray *tokens = line.Tokenize("\t");
        Int_t ntokens = tokens->GetEntries();
        
        cout << " ntokens=" << ntokens << " line=" << line.Data() << endl;
	Int_t jmax=ntokens/3;
        for (Int_t j=0; j<jmax; j++){
	  sdme.push_back( (((TObjString*)tokens->At(3*j))->GetString()) );
          parms.push_back( (((TObjString*)tokens->At(3*j+1))->GetString()).Atof() );
          parms_err.push_back( (((TObjString*)tokens->At(3*j+2))->GetString()).Atof());
        }
        
    }   // end loop over lines
    
        sprintf (string,"AmpTool Fit\n");
        printf("string=%s",string);
        TLatex *t1 = new TLatex(0.2,0.85,string);
        t1->SetNDC();
        t1->SetTextSize(0.04);
        t1->Draw();
        
        for (Int_t j=0; j<sdme.size()-2; j++) {     // -2 to eliminate Sigma and P
            cout << sdme[j] << "=" << parms[j] << " err=" << parms_err[j] << endl;
        
            TString sdmename;
            sdmename = sdme[j];
            sprintf (string,"%s = \t%.3f #pm %.3f\n",sdmename.Data(),parms[j],parms_err[j]);
        	printf("string=%s",string);
        	TLatex *t1 = new TLatex(0.2,0.75 - 0.05*j,string);
        	t1->SetNDC();
        	t1->SetTextSize(0.04);
        	t1->Draw();
        }
        
    
    parameters.close();
    

    c0->SaveAs(filename+".pdf(");
    c1->SaveAs(filename+".pdf");
    c2->SaveAs(filename+".pdf");
    c3->SaveAs(filename+".pdf)");
}
