
{

  ifstream in;
  in.open( "threepi_fit.txt" );

  enum { kMaxPoints = 100 };

  double ll = 0.7;
  double ul = 2.0;
  
  double eventCounter = 0;

  double mass[kMaxPoints];
  double masse[kMaxPoints];
  double rhoPiSWave[kMaxPoints];
  double rhoPiSWavee[kMaxPoints];
  double rhoPiDWave[kMaxPoints];
  double rhoPiDWavee[kMaxPoints];
  double rhoPiPXWave[kMaxPoints];
  double rhoPiPXWavee[kMaxPoints];
  double f2PiSWave[kMaxPoints];
  double f2PiSWavee[kMaxPoints];
  double rhoPiPWave[kMaxPoints];
  double rhoPiPWavee[kMaxPoints];
  double all[kMaxPoints];
  double alle[kMaxPoints];
  double phaseDP[kMaxPoints];
  double phaseDPe[kMaxPoints];
  double phaseDS[kMaxPoints];
  double phaseDSe[kMaxPoints];

  int line = 0;
  while( ! in.eof() ){

    in >> mass[line]
       >> rhoPiSWave[line] >> rhoPiSWavee[line]
       >> rhoPiDWave[line] >> rhoPiDWavee[line]
       >> rhoPiPXWave[line] >> rhoPiPXWavee[line]
       >> f2PiSWave[line] >> f2PiSWavee[line]
       >> rhoPiPWave[line] >> rhoPiPWavee[line]
       >> all[line] >> alle[line]
       >> phaseDP[line] >> phaseDPe[line]
       >> phaseDS[line] >> phaseDSe[line];
    
    eventCounter += all[line];
    
    line++;
  }

  TGraphErrors rhoPiSWaveGraph( line, mass, rhoPiSWave, masse, rhoPiSWavee );
  rhoPiSWaveGraph.SetMarkerStyle( 20 );
  rhoPiSWaveGraph.SetMarkerSize( .5 );
  TGraphErrors rhoPiDWaveGraph( line, mass, rhoPiDWave, masse, rhoPiDWavee );
  rhoPiDWaveGraph.SetMarkerStyle( 20 );
  rhoPiDWaveGraph.SetMarkerSize( 0.5 );
  TGraphErrors rhoPiPXWaveGraph( line, mass, rhoPiPXWave, masse, rhoPiPXWavee );
  rhoPiPXWaveGraph.SetMarkerStyle( 20 );
  rhoPiPXWaveGraph.SetMarkerSize( 0.5 );
  TGraphErrors f2PiSWaveGraph( line, mass, f2PiSWave, masse, f2PiSWavee );
  f2PiSWaveGraph.SetMarkerStyle( 20 );
  f2PiSWaveGraph.SetMarkerSize( 0.5 );
  TGraphErrors rhoPiPWaveGraph( line, mass, rhoPiPWave, masse, rhoPiPWavee );
  rhoPiPWaveGraph.SetMarkerStyle( 20 );
  rhoPiPWaveGraph.SetMarkerSize( 0.5 );
  TGraphErrors allGraph( line, mass, all, masse, alle );
  allGraph.SetMarkerStyle( 20 );
  allGraph.SetMarkerSize( 0.5 );
  TGraphErrors phaseDPGraph( line, mass, phaseDP, masse, phaseDPe );
  phaseDPGraph.SetMarkerStyle( 20 );
  phaseDPGraph.SetMarkerSize( 0.5 );
  TGraphErrors phaseDSGraph( line, mass, phaseDS, masse, phaseDSe );
  phaseDSGraph.SetMarkerStyle( 20 );
  phaseDSGraph.SetMarkerSize( 0.5 );
  
  TCanvas* can = new TCanvas( "can", "Amplitude Analysis Plots", 800, 800 );
  can->Divide( 2, 4 );

  can->cd( 1 );
  TH1F h1( "h1", "1^{+} #rho#pi S", 1, ll, ul );
  h1.SetMaximum( 7000 );
  h1.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h1.SetStats( 0 );
  h1.Draw();
  rhoPiSWaveGraph.Draw( "P" );

  can->cd( 2 );
  TH1F h2( "h2", "1^{-} #rho#pi P", 1, ll, ul );
  h2.SetMaximum( 700 );
  h2.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h2.SetStats( 0 );
  h2.Draw();
  rhoPiPXWaveGraph.Draw( "P" );

  can->cd( 3 );
  TH1F h3( "h3", "2^{+} #rho#pi D", 1, ll, ul );
  h3.SetMaximum( 4000 );
  h3.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h3.SetStats( 0 );
  h3.Draw();
  rhoPiDWaveGraph.Draw( "P" );

  can->cd( 4 );
  TH1F h4( "h4", "2^{-} f_{2}#pi S", 1, ll, ul );
  h4.SetMaximum( 2400 );
  h4.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h4.SetStats( 0 );
  h4.Draw();
  f2PiSWaveGraph.Draw( "P" );

  can->cd( 5 );
  TH1F h5( "h5", "2^{-} #rho#pi P", 1, ll, ul );
  h5.SetMaximum( 2800 );
  h5.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h5.SetStats( 0 );
  h5.Draw();
  rhoPiPWaveGraph.Draw( "P" );

  can->cd( 6 );
  TH1F h6( "h6", "3#pi All Waves", 1, ll, ul );
  h6.SetMaximum( 9000 );
  h6.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h6.SetStats( 0 );
  h6.Draw();
  allGraph.Draw( "P" );

  can->cd( 7 );
  TH1F h7( "h7", "Phase( 2^{+} #rho#pi D ) - Phase( 1^{-} #rho#pi P )", 1, ll, ul );
  h7.SetMaximum( 6.28 );
  h7.SetMinimum( -6.28 );
  h7.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h7.SetStats( 0 );
  h7.Draw();
  phaseDPGraph.Draw( "P" );

  can->cd( 8 );
  TH1F h8( "h8", "Phase( 2^{+} #rho#pi D ) - Phase( 2^{-} f_{2}#pi S )", 1, ll, ul );
  h8.SetMaximum( 6.28 );
  h8.SetMinimum( -6.28 );
  h8.GetXaxis()->SetTitle( "3#pi Invariant Mass [GeV/c^{2}]" );
  h8.SetStats( 0 );
  h8.Draw();
  phaseDSGraph.Draw( "P" );
  
  cout << "Total number of events:  " << eventCounter << endl;
}

