// Basic pLots for flattened DATA trees:
TString FN("/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/dataTrees/RunPeriod-2018-08/tree_pi0pip0pippim__B4_flat_*.root");

// Basic plots for flattened MC trees:
TString FNMC("/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/mcTrees/RunPeriod-2018-08/tree_pi0pip0pippim__B4_flat_*.root");

TString NT("ntFSGlueX_100_112");

  // cuts for the signal region
TString SIG4("(M234>0.760&&M234<0.805)");
TString SIG5("(M235>0.760&&M235<0.805)");
TString SIG4orSIG5(  "(("+SIG4+")||("+SIG5+"))");

  // cuts for the sideband regions
TString SB4("(((M234>0.690&&M234<0.735)||"
              "(M234>0.830&&M234<0.875))&&!("+SIG4+")&&!("+SIG5+"))");
TString SB5("(((M235>0.690&&M235<0.735)||"
              "(M235>0.830&&M235<0.875))&&!("+SIG4+")&&!("+SIG5+"))");
TString SB4andSB5( "(("+SB4+")&&("+SB5+"))");
TString SB4orSB5(  "(("+SB4+")||("+SB5+"))");
TString SB4xorSB5("((("+SB4+")||("+SB5+"))&&!("+SB4andSB5+"))");

  // event weights
TString OMEGAWT("(1.00*("+SIG4orSIG5+")-0.50*("+SB4xorSB5+")-1.25*("+SB4andSB5+"))");

void setup(){
    if (FSModeCollection::modeVector().size() != 0) return;
    FSHistogram::readHistogramCache();
    FSModeCollection::addModeInfo("100_112")->addCategory("m100_112");

// DEFINITION OF CUTS:

    // SIMPLE CUTS
    FSCut::defineCut("chi2", "Chi2DOF<2.5");                                      // chi^2 / DoF < 2.5
    FSCut::defineCut("cohPeak", "(EnPB>8.2&&EnPB<8.8)");                          // 8.2 < E_beam < 8.8 GeV
    FSCut::defineCut("tCut", "OR(abs(-1*MASS2(GLUEXTARGET,-[p+]))<1.0)");         // -t < 1.0 GeV^2
    FSCut::defineCut("mm2", "abs(RMASS2(B,GLUEXTARGET,-[p+],-[pi01],-[pi02],-[pi+],-[pi-])) < 0.05");  // missing mass squared < 0.05 GeV^2

    // CUTS W/ SIDEBANDS
    FSCut::defineCut( "rf", "OR(abs(RFDeltaT)<2.0)", "abs(RFDeltaT)>2.0", 0.125 );
    
    // FRIEND TREE FOR 2D OMEGA SUBTRACTION
    FSTree::addFriendTree("M3PI");
}

void saveM3PI(){ //only needs to be run ONCE, sets up friend trees for convenience with the 2D sideband subtraction
    vector<pair<TString,TString> > friendTreeContents;
    friendTreeContents.push_back(pair<TString,TString>("M234","MASS(2,3,4)"));
    friendTreeContents.push_back(pair<TString,TString>("M235","MASS(2,3,5)"));
    FSTree::createFriendTree(FN,NT,"M3PI",friendTreeContents);
    FSTree::createFriendTree(FNMC,NT,"M3PI",friendTreeContents);
}

void plots(){
    setup();

    TCanvas* c1 = new TCanvas("c1","c1",1200,800);
    c1->Divide(3,2);
    c1->cd(1);
    TH1F* hChi2 = FSModeHistogram::getTH1F( FN, NT, "m100_112", "Chi2DOF", "(100,0,20)", OMEGAWT+"*CUT(cohPeak,tCut,mm2,rf)" );
    hChi2->SetTitle( "Data: KinFit #chi^{2} / DoF;#chi^{2} / DoF;Events" );
    hChi2->SetMinimum(0);
    hChi2->Draw();
    TLine* chi2Cut = new TLine( 2.5, 0, 2.5, hChi2->GetBinContent( hChi2->FindBin( 2.5 ) ) );
    chi2Cut->SetLineColor( kRed+2 );
    chi2Cut->SetLineWidth( 2 );
    chi2Cut->Draw( "same" );
    c1->cd(2);
    TH1F* hEBeam = FSModeHistogram::getTH1F( FN, NT, "m100_112", "EnPB", "(100,6,12)", OMEGAWT+"*CUT(chi2,tCut,mm2,rf)" );
    hEBeam->SetTitle("Data: Beam Energy;E_{#gamma} (GeV);Events / 60 MeV");
    hEBeam->SetMinimum(0);
    hEBeam->Draw();
    TLine* eBeamCut_low = new TLine( 8.2, 0, 8.2, hEBeam->GetBinContent( hEBeam->FindBin( 8.2 ) ) );
    eBeamCut_low->SetLineColor( kRed+2 );
    eBeamCut_low->SetLineWidth( 2 );
    eBeamCut_low->Draw( "same" );
    TLine* eBeamCut_high = new TLine( 8.8, 0, 8.8, hEBeam->GetBinContent( hEBeam->FindBin( 8.8 ) ) );
    eBeamCut_high->SetLineColor( kRed+2 );
    eBeamCut_high->SetLineWidth( 2 );
    eBeamCut_high->Draw( "same" );
    c1->cd(3);
    TH1F* ht = FSModeHistogram::getTH1F( FN, NT, "m100_112", "abs(-1*MASS2(GLUEXTARGET,-[p+]))", "(100,0,2)", OMEGAWT+"*CUT(chi2,cohPeak,mm2,rf)" );
    ht->SetTitle( "Data: Momentum Transfer;-t (GeV^{2});Events / 0.02 GeV^{2}" );
    ht->SetMinimum(0);
    ht->Draw();
    TLine* tcut = new TLine( 1.0, 0, 1.0, ht->GetBinContent( ht->FindBin( 1.0 ) ) );
    tcut->SetLineColor( kRed+2 );
    tcut->SetLineWidth( 2 );
    tcut->Draw( "same" );
    c1->cd(4);
    TH1F* hmm2 = FSModeHistogram::getTH1F( FN, NT, "m100_112", "RMASS2(B,GLUEXTARGET,-[p+],-[pi01],-[pi02],-[pi+],-[pi-])", "(100, -0.1, 0.1)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,rf)" );
    hmm2->SetTitle("Data: Missing Mass Squared;MM^{2} (GeV^{2});Events / 0.002 GeV^{2}");
    hmm2->SetMinimum(0);
    hmm2->Draw();
    TLine* mm2Cut_low = new TLine( -0.05, 0, -0.05, hmm2->GetMaximum() );
    mm2Cut_low->SetLineColor( kRed+2 );
    mm2Cut_low->SetLineWidth( 2 );
    mm2Cut_low->Draw( "same" );
    TLine* mm2Cut_high = new TLine( 0.05, 0, 0.05, hmm2->GetMaximum() );
    mm2Cut_high->SetLineColor( kRed+2 );
    mm2Cut_high->SetLineWidth( 2 );
    mm2Cut_high->Draw( "same" );
    c1->cd(5);
    TH1F* hrf = FSModeHistogram::getTH1F( FN, NT, "m100_112", "RFDeltaT", "(400,-20,20)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,mm2)" );
    hrf->SetTitle( "Data: RF #Delta t;#Delta t_{RF} (ns);Events / 0.1 ns" );
    hrf->SetMinimum(0);
    hrf->Draw();
    TLine* rfcut_low = new TLine( -2, 0, -2, hrf->GetMaximum() );
    rfcut_low->SetLineColor( kRed+2 );
    rfcut_low->SetLineWidth( 2 );
    rfcut_low->Draw( "same" );
    TLine* rfcut_high = new TLine( 2, 0, 2, hrf->GetMaximum() );
    rfcut_high->SetLineColor( kRed+2 );
    rfcut_high->SetLineWidth( 2 );
    rfcut_high->Draw( "same" );
    c1->cd(6);
    TH1F* h3pi = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi+],[pi-])", "(100,0.6,1.0)", "CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h3pi->SetTitle( "Data: 3#pi Invariant Mass;M(#pi^{+}#pi^{-}#pi^{0}) (GeV);Combos / 4 MeV" );
    h3pi->Draw();
    TLine* l3pi1 = new TLine( 0.69, 0, 0.69, h3pi->GetBinContent( h3pi->FindBin( 0.69 ) ) );
    TLine* l3pi2 = new TLine( 0.735, 0, 0.735, h3pi->GetBinContent( h3pi->FindBin( 0.735 ) ) );
    TLine* l3pi3 = new TLine( 0.76, 0, 0.76, h3pi->GetBinContent( h3pi->FindBin( 0.76 ) ) );
    TLine* l3pi4 = new TLine( 0.805, 0, 0.805, h3pi->GetBinContent( h3pi->FindBin( 0.805 ) ) );
    TLine* l3pi5 = new TLine( 0.83, 0, 0.83, h3pi->GetBinContent( h3pi->FindBin( 0.83 ) ) );
    TLine* l3pi6 = new TLine( 0.875, 0, 0.875, h3pi->GetBinContent( h3pi->FindBin( 0.875 ) ) );
    l3pi1->SetLineColor( kRed+2 );
    l3pi1->SetLineWidth( 2 );
    l3pi1->Draw();
    l3pi2->SetLineColor( kRed+2 );
    l3pi2->SetLineWidth( 2 );
    l3pi2->Draw();
    l3pi3->SetLineColor( kRed+2 );
    l3pi3->SetLineWidth( 2 );
    l3pi3->Draw();
    l3pi4->SetLineColor( kRed+2 );
    l3pi4->SetLineWidth( 2 );
    l3pi4->Draw();
    l3pi5->SetLineColor( kRed+2 );
    l3pi5->SetLineWidth( 2 );
    l3pi5->Draw();
    l3pi6->SetLineColor( kRed+2 );
    l3pi6->SetLineWidth( 2 );
    l3pi6->Draw();

    TCanvas* c2 = new TCanvas("c2","c2",800,800);
    c2->cd();
    TH1F* h4pi_nocut = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "" );
    h4pi_nocut->SetTitle( "Data: No Cuts;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_nocut->Draw();

    TCanvas* c3 = new TCanvas("c3","c3",1200,800);
    c3->Divide(3,2);
    c3->cd(1);
    TH1F* h4pi_chi2 = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2)" );
    h4pi_chi2->SetTitle( "Data: #chi^{2} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_chi2->Draw();
    c3->cd(2);
    TH1F* h4pi_eBeam = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak)" );
    h4pi_eBeam->SetTitle( "Data: Add E_{#gamma} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_eBeam->Draw();
    c3->cd(3);
    TH1F* h4pi_t = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut)" );
    h4pi_t->SetTitle( "Data: Add -t Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_t->Draw();
    c3->cd(4);
    TH1F* h4pi_mm2 = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut,mm2)" );
    h4pi_mm2->SetTitle( "Data: Add MM^{2} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_mm2->Draw();
    c3->cd(5);
    TH1F* h4pi_rf = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h4pi_rf->SetTitle( "Data: Add RF SB Subtraction;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_rf->Draw();
    c3->cd(6);
    TH1F* h4pi_o = FSModeHistogram::getTH1F( FN, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h4pi_o->SetTitle( "Data: Add #omega SB Subtraction;M(#omega#pi^{0}) (GeV);Events / 8 MeV" );
    h4pi_o->SetMinimum(0);
    h4pi_o->Draw();

    TCanvas* c4 = new TCanvas("c4","c4",1200,800);
    c4->Divide(3,2);
    c4->cd(1);
    TH1F* hChi2_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "Chi2DOF", "(100,0,20)", OMEGAWT+"*CUT(cohPeak,tCut,mm2,rf)" );
    hChi2_mc->SetTitle( "MC: KinFit #chi^{2} / DoF;#chi^{2} / DoF;Events" );
    hChi2_mc->SetMinimum(0);
    hChi2_mc->Draw();
    TLine* chi2Cut_mc = new TLine( 2.5, 0, 2.5, hChi2_mc->GetBinContent( hChi2_mc->FindBin( 2.5 ) ) );
    chi2Cut_mc->SetLineColor( kRed+2 );
    chi2Cut_mc->SetLineWidth( 2 );
    chi2Cut_mc->Draw( "same" );
    c4->cd(2);
    TH1F* hEBeam_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "EnPB", "(100,6,12)", OMEGAWT+"*CUT(chi2,tCut,mm2,rf)" );
    hEBeam_mc->SetTitle("MC: Beam Energy;E_{#gamma} (GeV);Events / 60 MeV");
    hEBeam_mc->SetMinimum(0);
    hEBeam_mc->Draw();
    TLine* eBeamCut_low_mc = new TLine( 8.2, 0, 8.2, hEBeam_mc->GetBinContent( hEBeam_mc->FindBin( 8.2 ) ) );
    eBeamCut_low_mc->SetLineColor( kRed+2 );
    eBeamCut_low_mc->SetLineWidth( 2 );
    eBeamCut_low_mc->Draw( "same" );
    TLine* eBeamCut_high_mc = new TLine( 8.8, 0, 8.8, hEBeam_mc->GetBinContent( hEBeam_mc->FindBin( 8.8 ) ) );
    eBeamCut_high_mc->SetLineColor( kRed+2 );
    eBeamCut_high_mc->SetLineWidth( 2 );
    eBeamCut_high_mc->Draw( "same" );
    c4->cd(3);
    TH1F* ht_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "abs(-1*MASS2(GLUEXTARGET,-[p+]))", "(100,0,2)", OMEGAWT+"*CUT(chi2,cohPeak,mm2,rf)" );
    ht_mc->SetTitle( "MC: Momentum Transfer;-t (GeV^{2});Events / 0.02 GeV^{2}" );
    ht_mc->SetMinimum(0);
    ht_mc->Draw();
    TLine* tcut_mc = new TLine( 1.0, 0, 1.0, ht_mc->GetBinContent( ht_mc->FindBin( 1.0 ) ) );
    tcut_mc->SetLineColor( kRed+2 );
    tcut_mc->SetLineWidth( 2 );
    tcut_mc->Draw( "same" );
    c4->cd(4);
    TH1F* hmm2_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "RMASS2(B,GLUEXTARGET,-[p+],-[pi01],-[pi02],-[pi+],-[pi-])", "(100, -0.1, 0.1)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,rf)" );
    hmm2_mc->SetTitle("MC: Missing Mass Squared;MM^{2} (GeV^{2});Events / 0.002 GeV^{2}");
    hmm2_mc->SetMinimum(0);
    hmm2_mc->Draw();
    TLine* mm2Cut_low_mc = new TLine( -0.05, 0, -0.05, hmm2_mc->GetMaximum() );
    mm2Cut_low_mc->SetLineColor( kRed+2 );
    mm2Cut_low_mc->SetLineWidth( 2 );
    mm2Cut_low_mc->Draw( "same" );
    TLine* mm2Cut_high_mc = new TLine( 0.05, 0, 0.05, hmm2_mc->GetMaximum() );
    mm2Cut_high_mc->SetLineColor( kRed+2 );
    mm2Cut_high_mc->SetLineWidth( 2 );
    mm2Cut_high_mc->Draw( "same" );
    c4->cd(5);
    TH1F* hrf_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "RFDeltaT", "(400,-20,20)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,mm2)" );
    hrf_mc->SetTitle( "MC: RF #Delta t;#Delta t_{RF} (ns);Events / 0.1 ns" );
    hrf_mc->SetMinimum(0);
    hrf_mc->Draw();
    TLine* rfcut_low_mc = new TLine( -2, 0, -2, hrf_mc->GetMaximum() );
    rfcut_low_mc->SetLineColor( kRed+2 );
    rfcut_low_mc->SetLineWidth( 2 );
    rfcut_low_mc->Draw( "same" );
    TLine* rfcut_high_mc = new TLine( 2, 0, 2, hrf_mc->GetMaximum() );
    rfcut_high_mc->SetLineColor( kRed+2 );
    rfcut_high_mc->SetLineWidth( 2 );
    rfcut_high_mc->Draw( "same" );
    c4->cd(6);
    TH1F* h3pi_mc = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi+],[pi-])", "(100,0.6,1.0)", "CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h3pi_mc->SetTitle( "MC: 3#pi Invariant Mass;M(#pi^{+}#pi^{-}#pi^{0}) (GeV);Combos / 4 MeV" );
    h3pi_mc->Draw();
    TLine* l3pi1_mc = new TLine( 0.69, 0, 0.69, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.69 ) ) );
    TLine* l3pi2_mc = new TLine( 0.735, 0, 0.735, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.735 ) ) );
    TLine* l3pi3_mc = new TLine( 0.76, 0, 0.76, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.76 ) ) );
    TLine* l3pi4_mc = new TLine( 0.805, 0, 0.805, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.805 ) ) );
    TLine* l3pi5_mc = new TLine( 0.83, 0, 0.83, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.83 ) ) );
    TLine* l3pi6_mc = new TLine( 0.875, 0, 0.875, h3pi_mc->GetBinContent( h3pi_mc->FindBin( 0.875 ) ) );
    l3pi1_mc->SetLineColor( kRed+2 );
    l3pi1_mc->SetLineWidth( 2 );
    l3pi1_mc->Draw();
    l3pi2_mc->SetLineColor( kRed+2 );
    l3pi2_mc->SetLineWidth( 2 );
    l3pi2_mc->Draw();
    l3pi3_mc->SetLineColor( kRed+2 );
    l3pi3_mc->SetLineWidth( 2 );
    l3pi3_mc->Draw();
    l3pi4_mc->SetLineColor( kRed+2 );
    l3pi4_mc->SetLineWidth( 2 );
    l3pi4_mc->Draw();
    l3pi5_mc->SetLineColor( kRed+2 );
    l3pi5_mc->SetLineWidth( 2 );
    l3pi5_mc->Draw();
    l3pi6_mc->SetLineColor( kRed+2 );
    l3pi6_mc->SetLineWidth( 2 );
    l3pi6_mc->Draw();


    TCanvas* c5 = new TCanvas("c5","c5",1200,800);
    c5->Divide(3,2);
    c5->cd(1);
    TH1F* h4piMC_chi2 = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2)" );
    h4piMC_chi2->SetTitle( "MC: #chi^{2} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_chi2->Draw();
    c5->cd(2);
    TH1F* h4piMC_eBeam = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak)" );
    h4piMC_eBeam->SetTitle( "MC: Add E_{#gamma} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_eBeam->Draw();
    c5->cd(3);
    TH1F* h4piMC_t = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut)" );
    h4piMC_t->SetTitle( "MC: Add -t Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_t->Draw();
    c5->cd(4);
    TH1F* h4piMC_mm2 = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut,mm2)" );
    h4piMC_mm2->SetTitle( "MC: Add MM^{2} Cut;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_mm2->Draw();
    c5->cd(5);
    TH1F* h4piMC_rf = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", "CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h4piMC_rf->SetTitle( "MC: Add RF SB Subtraction;M(#pi^{+}#pi^{-}#pi^{0}#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_rf->Draw();
    c5->cd(6);
    TH1F* h4piMC_o = FSModeHistogram::getTH1F( FNMC, NT, "m100_112", "MASS([pi01],[pi02],[pi+],[pi-])", "(100,0.8,1.6)", OMEGAWT+"*CUT(chi2,cohPeak,tCut,mm2,rf)" );
    h4piMC_o->SetTitle( "MC: Add #omega SB Subtraction;M(#omega#pi^{0}) (GeV);Events / 8 MeV" );
    h4piMC_o->SetMinimum(0);
    h4piMC_o->Draw();

    c1->Print( "plots/dataCuts.pdf" );
    c2->Print( "plots/m4piNoCuts.pdf" );
    c3->Print( "plots/m4piDataCuts.pdf" );
    c4->Print( "plots/mcCuts.pdf" );
    c5->Print( "plots/m4piMCCuts.pdf" );

}
