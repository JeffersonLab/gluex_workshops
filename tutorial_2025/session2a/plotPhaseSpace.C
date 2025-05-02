{
    TString FN("/w/halld-scshelf2101/home/ksaldan/environment_builds/gluex_workshops/tutorial_2025/session2a/b1_omegapi_ps.root");
    TString TN(FSTree::getTreeNameFromFile(FN));

    // Define histograms coming from generated file
    FSHistogram::enableRDataFrame(false);
    TH1F* tslope = FSHistogram::getTH1F(FN,TN,"-1.*MASS2(GLUEXTARGET,-1)","(100,-0.01,4)","");
    TH1F* uvMass = FSHistogram::getTH1F(FN,TN,"MASS(2,3)","(100,0.8,4)","");
    TH1F* lvMass = FSHistogram::getTH1F(FN,TN,"MASS(1)","(100,0.7,1.1)","");
    TH1F* pBeam  = FSHistogram::getTH1F(FN,TN,"EnPB","(100,6,11)","");
    TH2F* aVSm   = FSHistogram::getTH2F(FN,TN,"GJCOSTHETA(3;2;B):MASS(2,3)","(75,0.8,4,75,-1.1,1.1)","");
    FSHistogram::executeRDataFrame();

    // Place a title for each histogram
    tslope->SetTitle("; Momentum Transfer -t (GeV)^{2}; Events/ 0.03 (GeV)^{2};");
    uvMass->SetTitle("; M[#omega #pi^{0}] GeV; Events/ 22 MeV; ");
    lvMass->SetTitle("; M[p] GeV; Events/ 4 MeV;");
    pBeam->SetTitle("; E_{#gamma} GeV; Events/ 40;");
    aVSm->SetTitle(";M[#omega #pi^{0}] GeV; COS(#theta_{GJ})^{#omega}");

    bool toggleCanvas = true;

    // Keep toggleCanvas = true, if you want all plots in one canvas
    if(toggleCanvas){
      TCanvas* c1 = new TCanvas("c1","c1");
      c1->Divide(2,3);

      c1->cd(1); tslope->Draw();
      c1->cd(2); aVSm->Draw("colz");
      c1->cd(3); pBeam->Draw();
      c1->cd(4); uvMass->Draw();
      c1->cd(6); lvMass->Draw();
    }
    // Place toggleCanvas = false, if you want all plot on a seperate canvas
    if(!toggleCanvas){
      TCanvas* c1 = new TCanvas("c1","c1");
      TCanvas* c2 = new TCanvas("c2","c2");
      TCanvas* c3 = new TCanvas("c3","c3");
      TCanvas* c4 = new TCanvas("c4","c4");
      TCanvas* c5 = new TCanvas("c5","c5");
        
  
      c1->cd(); tslope->Draw();
      c2->cd(); aVSm->Draw("colz");
      c3->cd(); pBeam->Draw();
      c4->cd(); uvMass->Draw();
      c5->cd(); lvMass->Draw();   
    }






}
