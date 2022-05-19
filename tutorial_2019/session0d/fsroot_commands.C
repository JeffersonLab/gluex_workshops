{

  // draw RF
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "RFDeltaT", "(1000,-20,20)", "" )->Draw();

  TString cutRF( "abs(RFDeltaT)<2" );
  TString cutRFSB( "abs(RFDeltaT)>2" );
  FSCut::defineCut( "rf", cutRF, cutRFSB, 0.125 );

  // draw eta, constrained and unconstrained photons
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "MASS(2)", "(1000,0.3,0.8)", "CUT(rf)" )->Draw();
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "RMASS(2)", "(1000,0.3,0.8)", "CUT(rf)" )->Draw();
  
  TString cutEta( "abs(MASS(2)-0.546) < 0.03" );
  TString cutEtaSB( "(abs(MASS(2)-0.546) > 0.06) && (abs(MASS(2)-0.546) < 0.12)" );
  FSCut::defineCut( "eta", cutEta, cutEtaSB, 0.5 );

  // draw the chi^2 plot for the signal and background based on 2D sidebands
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "Chi2DOF", "(1000,0,50)", "CUT(rf,eta)" )->Draw();
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "Chi2DOF", "(1000,0,50)", "CUTSB(rf,eta)" )->Draw( "same" );

  // define a chi^2 cut
  TString cutChi2( "Chi2DOF < 5" );
  FSCut::defineCut( "chi2", cutChi2 );

  // draw eta again with the chi^2 cut -- also for RF sidebands
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "MASS(2)", "(1000,0.3,0.8)", "CUT(rf,chi2)" )->Draw();
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "RMASS(2)", "(1000,0.3,0.8)", "CUT(rf,chi2)" )->Draw();
  
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "MASS(2)", "(1000,0.3,0.8)", "CUT(chi2)&&CUTSB(rf)" )->Draw();
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "RMASS(2)", "(1000,0.3,0.8)", "CUT(chi2)&&CUTSB(rf)" )->Draw();

  
  // draw the eta pi pi mass plot for signal region and 2D sidebands, both with a chi^2 cut
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "MASS(2,3,4)", "(300,0.5,4.0)", "CUT(eta,rf,chi2)" )->Draw();
  FSHistogram::getTH1F( "etapipi_flat.root", "ntFSGlueX", "MASS(2,3,4)", "(300,0.5,4.0)", "CUTSB(eta,rf)&&CUT(chi2)" )->Draw();

  // define a few interesting regions of mass
  TString cutf1( "abs(MASS(2,3,4)-1.285)<0.03" );
  TString cutRhop( "abs(MASS(2,3,4)-1.6)<0.1" );

  FSCut::defineCut( "f1", cutf1 );
  FSCut::defineCut( "rhop", cutRhop );

  // Dalitz plot in the rho prime region (no sideband subtraction due to low stats):
  FSHistogram::getTH2F( "etapipi_flat.root", "ntFSGlueX", "MASS2(2,3):MASS2(3,4)", "(100,0,1.5,100,0.4,2.5)", "CUT(eta,rf,chi2,rhop)" )->Draw( "colz" );

}
