// Basic pLots for flattened DATA trees:
//TString FND("/work/halld/gluex_workshop_data/tutorial_2022/session2d/flatten/sp2017/pol*/tree_pi0eta__B4_M17_M7_FLAT_*.root");

// Basic plots for flattened and skimmed DATA trees:
TString FND("/work/halld/gluex_workshop_data/tutorial_2022/session2d/skim/tree_pi0eta__B4_M17_M7_DATA_sp17_*_GENERAL_SKIM.root");

// Basic plots for flattened MC trees:
//TString FND("/work/halld/gluex_workshop_data/tutorial_2022/session2d/flatten/flatMC/tree_pi0eta__B4_M17_M7_FLAT_MC.root");

// Basic plots for flattened BGGEN MC trees:
//TString FND("/work/halld/gluex_workshop_data/tutorial_2022/session2d/flatten/bggen/tree_pi0eta__B4_M17_M7_*.root");


TString NT("ntFSGlueX_MODECODE");

void setup(){
  if (FSModeCollection::modeVector().size() != 0) return;
  FSHistogram::readHistogramCache();
  FSModeCollection::addModeInfo("101_1")->addCategory("m101_1");

// DEFINITION OF CUTS:

  // FIXED CUTS
  FSCut::defineCut("unusedE","EnUnusedSh<0.1");
  FSCut::defineCut("unusedTracks","NumUnusedTracks<1");
  FSCut::defineCut("zProton","ProdVz>=52&&ProdVz<=78");
  FSCut::defineCut("protMom","MOMENTUM([p+])>=0.3");
  FSCut::defineCut("cet0103","OR(abs(-1*MASS2(GLUEXTARGET,-[p+])-0.2)<0.1)");    // 0.1 < t < 0.3
  FSCut::defineCut("e8288","(EnPB>8.2&&EnPB<8.8)");
  FSCut::defineCut("chi2","Chi2DOF<3.3","Chi2DOF>10.0&&Chi2DOF<15.0");
  FSCut::defineCut("photFiducialA","(acos(COSINE([eta]a))*180/3.141>2.5 && acos(COSINE([eta]a))*180/3.141<10.3) || (acos(COSINE([eta]a))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialB","(acos(COSINE([eta]b))*180/3.141>2.5 && acos(COSINE([eta]b))*180/3.141<10.3) || (acos(COSINE([eta]b))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialC","(acos(COSINE([pi0]a))*180/3.141>2.5 && acos(COSINE([pi0]a))*180/3.141<10.3) || (acos(COSINE([pi0]a))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialD","(acos(COSINE([pi0]b))*180/3.141>2.5 && acos(COSINE([pi0]b))*180/3.141<10.3) || (acos(COSINE([pi0]b))*180/3.141>11.9)");

  FSCut::defineCut("rejectOmega","!((MASS([pi0]a,[eta]a)<0.15 && MASS([pi0]b,[eta]b)<0.15) || (MASS([pi0]a,[eta]b)<0.15 && MASS([pi0]b,[eta]a)<0.15) || (MASS([pi0]a,[eta]a)<0.12 && MASS([pi0]b,[eta]a)<0.12) || (MASS([pi0]a,[eta]b)<0.12 && MASS([pi0]b,[eta]b)<0.12))");
  FSCut::defineCut("delta","MASS([p+],[pi0])>1.4");
  
  // CUTS W/ SIDEBANDS
  FSCut::defineCut("rf","OR(abs(RFDeltaT)<2.0)", "abs(RFDeltaT)>2.0",0.125);
  FSCut::defineCut("eta","abs(MASS([eta])-0.548)<0.05","(abs(MASS([eta])-0.548+0.107)<0.025 || abs(MASS([eta])-0.548-0.103)<0.025)", 1.0);
  FSCut::defineCut("pi0","abs(MASS([pi0])-0.135)<0.025","(abs(MASS([pi0])-0.135+0.0425)<0.0125 || abs(MASS([pi0])-0.135-0.0425)<0.0125)", 1.0);
}

void plots(bool bggen=false){
  setup();
  system("rm -rf plots");  system("mkdir plots");

  TCanvas* c1 = new TCanvas("c1","c1",1200,800);
  c1->Divide(3,2);
  c1->cd(1);
  TH1F* hM1 = FSModeHistogram::getTH1F(FND,NT,"m101_1","EnUnusedSh","(100,0.0,1.0)","CUT(unusedTracks,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM1->SetTitle("MC: E_{unused} for -t in (0.1,0.3)");
  hM1->SetXTitle("E_{unused}  [GeV/c^{2}]");
  hM1->SetYTitle("Events");
  hM1->Draw();
  TLine* cutUnusedE = new TLine(0.1,0,0.1,hM1->GetMaximum());
  cutUnusedE->SetLineColor(kRed);
  cutUnusedE->Draw("same");
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","EnUnusedSh","(100,0.0,1.0)","CUT(unusedTracks,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  c1->cd(2);
  TH1F* hM6 = FSModeHistogram::getTH1F(FND,NT,"m101_1","ProdVz","(100,0.,100.0)","CUT(unusedTracks,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM6->SetTitle("MC: ProdVz for -t in (0.1,0.3)");
  hM6->SetXTitle("ProdVz  [GeV/c^{2}]");
  hM6->SetYTitle("Events");
  hM6->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","ProdVz","(100,0.,100.0)","CUT(unusedTracks,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  TLine* cutVz_low = new TLine(52,0,52,hM6->GetMaximum());
  cutVz_low->SetLineColor(kRed);
  cutVz_low->Draw("same");
  TLine* cutVz_hi = new TLine(78,0,78,hM6->GetMaximum());
  cutVz_hi->SetLineColor(kRed);
  cutVz_hi->Draw("same");
  c1->cd(3);
  TH1F* hM4a = FSModeHistogram::getTH1F(FND,NT,"m101_1","abs(-1*MASS2(GLUEXTARGET,-[p+]))","(100,0,1)","CUT(unusedTracks,zProton,chi2,unusedE,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM4a->SetTitle("MC: |-t|");
  hM4a->SetXTitle("|-t| [GeV^{2}]");
  hM4a->SetYTitle("Entries");
  TLine* cutt_low = new TLine(0.1,0,0.1,hM4a->GetMaximum());
  cutt_low->SetLineColor(kRed);
  hM4a->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","abs(-1*MASS2(GLUEXTARGET,-[p+]))","(100,0,1)","CUT(unusedTracks,zProton,chi2,unusedE,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  cutt_low->Draw("same");
  TLine* cutt_hi = new TLine(0.3,0,0.3,hM4a->GetMaximum());
  cutt_hi->SetLineColor(kRed);
  cutt_hi->Draw("same");
  c1->cd(4);
  TH1F* hM4b = FSModeHistogram::getTH1F(FND,NT,"m101_1","EnPB","(125,5,12)","CUT(unusedTracks,zProton,chi2,unusedE,cet0103,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM4b->SetTitle("MC: E_{beam} for -t in (0.1,0.3)");
  hM4b->SetXTitle("E_{beam} [GeV]");
  hM4b->SetYTitle("Entries");
  TLine* cutEb_low = new TLine(8.2,0,8.2,hM4b->GetMaximum());
  cutEb_low->SetLineColor(kRed);
  hM4b->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","EnPB","(125,5,12)","CUT(unusedTracks,zProton,chi2,unusedE,cet0103,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  cutEb_low->Draw("same");
  TLine* cutEb_hi = new TLine(8.8,0,8.8,hM4b->GetMaximum());
  cutEb_hi->SetLineColor(kRed);
  cutEb_hi->Draw("same");
  c1->cd(5);
  TH1F* hM4 = FSModeHistogram::getTH1F(FND,NT,"m101_1","Chi2DOF","(80,0,20)","CUT(unusedTracks,zProton,unusedE,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM4->SetTitle("MC: #chi^{2}/dof for -t in (0.1,0.3)");
  hM4->SetXTitle("#chi^{2}/dof");
  hM4->SetYTitle("Events");
  TLine* cutChi2 = new TLine(3.3,0,3.3,hM4->GetMaximum());
  cutChi2->SetLineColor(kRed);
  hM4->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","Chi2DOF","(80,0,20)","CUT(unusedTracks,zProton,unusedE,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  cutChi2->Draw("same");
  c1->cd(6);
  TH1F* hM8 = FSModeHistogram::getTH1F(FND,NT,"m101_1","acos(COSINE([eta]a))*180/3.141","(240,0.,60)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,delta,rf,eta,pi0,rejectOmega,protMom)");
  hM8->SetTitle("MC: #vartheta_{#gamma} for -t in (0.1,0.3)");
  hM8->SetXTitle("#vartheta_{#gamma}  [#circ]");
  hM8->SetYTitle("Entries");
  hM8->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","acos(COSINE([eta]a))*180/3.141","(240,0.,60)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,delta,rf,eta,pi0,rejectOmega,protMom)");
  TLine* cutFidu1 = new TLine(2.5,0,2.5,hM8->GetMaximum());
  cutFidu1->SetLineColor(kRed);
  cutFidu1->Draw("same");
  TLine* cutFidu2 = new TLine(10.3,0,10.3,hM8->GetMaximum());
  cutFidu2->SetLineColor(kRed);
  cutFidu2->Draw("same");
  TLine* cutFidu3 = new TLine(11.9,0,11.9,hM8->GetMaximum());
  cutFidu3->SetLineColor(kRed);
  cutFidu3->Draw("same");

  TCanvas* c11 = new TCanvas("c11","c11",800,800);
  c11->Divide(2,2);
  c11->cd(1);
  TH1F* hM2 = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([eta])","(60,0.2,0.8)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,rejectOmega,protMom)");
  hM2->SetTitle("MC: M(#gamma#gamma) for -t in (0.1,0.3)");
  hM2->SetXTitle("M(#gamma#gamma)  [GeV/c^{2}]");
  hM2->SetYTitle("Events / 10 MeV/c^{2}");
  hM2->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([eta])","(60,0.2,0.8)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,rejectOmega,protMom)");
  TLine* cutEtaSigL = new TLine(0.548-0.05,0,0.548-0.05,hM2->GetMaximum());
  TLine* cutEtaSigR = new TLine(0.548+0.05,0,0.548+0.05,hM2->GetMaximum());
  TLine* cutEtaSBLowL = new TLine(0.42,0,0.42,hM2->GetMaximum());
  TLine* cutEtaSBLowR = new TLine(0.47,0,0.47,hM2->GetMaximum());
  TLine* cutEtaSBHiL = new TLine(0.63,0,0.63,hM2->GetMaximum());
  TLine* cutEtaSBHiR = new TLine(0.68,0,0.68,hM2->GetMaximum());
  cutEtaSigL->SetLineColor(kRed);
  cutEtaSigR->SetLineColor(kRed);
  cutEtaSBLowL->SetLineColor(kRed);
  cutEtaSBLowR->SetLineColor(kRed);
  cutEtaSBHiL->SetLineColor(kRed);
  cutEtaSBHiR->SetLineColor(kRed);
  cutEtaSigL->Draw("same");
  cutEtaSigR->Draw("same");
  cutEtaSBLowL->Draw("same");
  cutEtaSBLowR->Draw("same");
  cutEtaSBHiL->Draw("same");
  cutEtaSBHiR->Draw("same");
  c11->cd(2);
  TH1F* hM9 = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([pi0])","(60,0.0,0.3)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,rejectOmega,protMom)");
  hM9->SetTitle("MC: M(#gamma#gamma) for -t in (0.1,0.3)");
  hM9->SetXTitle("M(#gamma#gamma)  [GeV/c^{2}]");
  hM9->SetYTitle("Events / 10 MeV/c^{2}");
  hM9->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([pi0])","(60,0.0,0.3)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,rejectOmega,protMom)");
  hM9->SetTitle("MC: M(#gamma#gamma) for -t in (0.1,0.3)");
  TLine* cutPi0SigL = new TLine(0.135-0.025,0,0.135-0.025,hM9->GetMaximum());
  TLine* cutPi0SigR = new TLine(0.135+0.025,0,0.135+0.025,hM9->GetMaximum());
  TLine* cutPi0SBLowL = new TLine(0.08,0,0.08,hM9->GetMaximum());
  TLine* cutPi0SBLowR = new TLine(0.105,0,0.105,hM9->GetMaximum());
  TLine* cutPi0SBHiL = new TLine(0.165,0,0.165,hM9->GetMaximum());
  TLine* cutPi0SBHiR = new TLine(0.19,0,0.19,hM9->GetMaximum());
  cutPi0SigL->SetLineColor(kRed);
  cutPi0SigR->SetLineColor(kRed);
  cutPi0SBLowL->SetLineColor(kRed);
  cutPi0SBLowR->SetLineColor(kRed);
  cutPi0SBHiL->SetLineColor(kRed);
  cutPi0SBHiR->SetLineColor(kRed);
  cutPi0SigL->Draw("same");
  cutPi0SigR->Draw("same");
  cutPi0SBLowL->Draw("same");
  cutPi0SBLowR->Draw("same");
  cutPi0SBHiL->Draw("same");
  cutPi0SBHiR->Draw("same");
  c11->cd(3);
  TH1F* hM3 = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([p+],[pi0])","(100,0.8,1.8)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,rf,eta,pi0,rejectOmega,protMom)");
  hM3->SetTitle("MC: M(p#pi^{0}) for -t in (0.1,0.3)");
  hM3->SetXTitle("M(#p#pi^{0})  [GeV/c^{2}]");
  hM3->SetYTitle("Events / 10 MeV/c^{2}");
  TLine* cutDelta = new TLine(1.4,0,1.4,hM3->GetMaximum());
  cutDelta->SetLineColor(kRed);
  hM3->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([p+],[pi0])","(100,0.8,1.8)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,rf,eta,pi0,rejectOmega,protMom)");
  cutDelta->Draw("same");
  c11->cd(4);
  TH1F* hM5 = FSModeHistogram::getTH1F(FND,NT,"m101_1","RFDeltaT","(400,-20,20)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,eta,pi0,rejectOmega,protMom)");
  hM5->SetTitle("MC: #Delta t_{RF} for -t in (0.1,0.3)");
  hM5->SetXTitle("#Delta t_{RF}");
  hM5->SetYTitle("Events");
  TLine* cutRFSigL = new TLine(-2.0,0,-2.0,hM5->GetMaximum());
  TLine* cutRFSigR = new TLine(2.0,0,2.0,hM5->GetMaximum());
  cutRFSigL->SetLineColor(kRed);
  cutRFSigR->SetLineColor(kRed);
  hM5->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","RFDeltaT","(400,-20,20)","CUT(unusedTracks,zProton,unusedE,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,eta,pi0,rejectOmega,protMom)");
  cutRFSigL->Draw("same");
  cutRFSigR->Draw("same");
  
  c1->Print("plots/p001_etapi.pdf");

  TCanvas* c2 = new TCanvas("c2","c2",1200,400);
  c2->Divide(3,1);
  c2->cd(1);
  TH1F* hMetapi = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([eta],[pi0])","(100,0.5,2.5)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rf,eta,pi0,rejectOmega,protMom)");
  TH1F* hMetapiSig = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([eta],[pi0])","(100,0.5,2.5)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rejectOmega,protMom)*CUTWT(rf,eta,pi0)");
  TH1F* hMetapiBg = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([eta],[pi0])","(100,0.5,2.5)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rejectOmega,protMom)*CUTSBWT(rf,eta,pi0)*(-1.0)");
  hMetapiSig->SetFillColor(kBlue);
  hMetapiBg->SetFillColor(kRed);
  hMetapi->SetTitle("MC: M(#eta#pi^{0}) for -t in (0.1,0.3)");
  hMetapi->SetXTitle("M(#eta#pi^{0}) [GeV/c^{2}]");
  hMetapi->SetYTitle("Events");
  hMetapi->Draw();
  hMetapiSig->Draw("hist,same");
  hMetapiBg->Draw("hist,same");

  c2->cd(2);
  hMetapiSig->SetTitle("MC: M(#eta#pi^{0}) for -t in (0.1,0.3)");
  hMetapiSig->SetXTitle("M(#eta#pi^{0}) [GeV/c^{2}]");
  hMetapiSig->SetYTitle("Events");
  hMetapiSig->DrawClone();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([eta],[pi0])","(100,0.5,2.5)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom)*CUTWT(rf,eta,pi0)");
  cout << hMetapiSig->Integral() << endl;

  FSFitUtilities::createFunction(new FSFitPOLY("p",1.04,1.56,1,0.0),1600.0,-900);
  FSFitUtilities::createFunction(new FSFitGAUS("g",1.04,1.56),500.0,1.32,0.05);
  FSFitUtilities::createFunction("pg","p+g");
  FSFitUtilities::fixParameters("p");
  FSFitUtilities::migrad(hMetapiSig,"pg");
  TF1* fpg = FSFitUtilities::getTF1("pg");
  fpg->SetLineColor(kRed); fpg->SetLineStyle(kSolid);
  fpg->Draw("same");
  TF1* fg = FSFitUtilities::getTF1("g");
  fg->SetLineColor(kBlue); fg->SetLineStyle(kSolid);
  fg->Draw("same");
  cout << "fg Integral: " << fg->Integral(1.04,1.56) << endl;
  
  c2->cd(3);
  TH2F* hMetapiVsGJCosTheta = FSModeHistogram::getTH2F(FND,NT,"m101_1","GJCOSTHETA([eta];[pi0];GLUEXBEAM):MASS([eta],[pi0])","(100,0.7,2.7,50,-1.,1.)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,rejectOmega,protMom)*CUTWT(rf,eta,pi0)");
  hMetapiVsGJCosTheta->SetXTitle("M(#eta#pi^{0}) [GeV/c^{2}]");
  hMetapiVsGJCosTheta->SetYTitle("Events");
  hMetapiVsGJCosTheta->Draw("colz");


  TCanvas* c3 = new TCanvas("c3","c3",800,800);
  c3->Divide(2,2);
  c3->cd(1);
  hMetapiSig->DrawClone();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([eta],[pi0])","(100,0.5,2.5)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  
  c3->cd(2);
  TH1F* hMpiproton = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([p+],[pi0])","(100,0.9,3.9)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  hMpiproton->SetTitle("MC: M(p#pi^{0}) for -t in (0.1,0.3)");
  hMpiproton->SetXTitle("M(p#pi^{0}) [GeV/c^{2}]");
  hMpiproton->SetYTitle("Events");
  hMpiproton->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([p+],[pi0])","(100,0.9,3.9)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  
  c3->cd(3);
  TH1F* hMetaproton = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([p+],[eta])","(100,1.4,3.9)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  hMetaproton->SetTitle("MC: M(p#eta) for -t in (0.1,0.3)");
  hMetaproton->SetXTitle("M(p#eta) [GeV/c^{2}]");
  hMetaproton->SetYTitle("Events");
  hMetaproton->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([p+],[eta])","(100,1.4,3.9)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");

  c3->cd(4);
  TH1F* hMpi0g = FSModeHistogram::getTH1F(FND,NT,"m101_1","MASS([pi0],[eta]b)","(100,0.,2.)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  hMpi0g->SetTitle("MC: M(#pi^{0}#gamma) for -t in (0.1,0.3)");
  hMpi0g->SetXTitle("M(#pi^{0}#gamma) [GeV/c^{2}]");
  hMpi0g->SetYTitle("Events");
  hMpi0g->Draw();
  if(bggen) FSModeHistogram::drawMCComponentsSame(FND,NT,"m101_1","MASS([pi0],[eta]b)","(100,0.,2.)","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega)*CUTWT(rf,eta,pi0)");
  FSHistogram::dumpHistogramCache();
}
