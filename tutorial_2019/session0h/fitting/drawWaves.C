
{
    
    ifstream in;
    in.open( "etapi_fit.txt" );
    
    enum { kMaxPoints = 100 };
    
    double ll = 0.7;
    double ul = 2.0;
    
    double eventCounter = 0;
    
    double mass[kMaxPoints];
    double masse[kMaxPoints];
    double etaPiSWave[kMaxPoints];
    double etaPiSWavee[kMaxPoints];
    double etaPiP0Wave[kMaxPoints];
    double etaPiP0Wavee[kMaxPoints];
    double etaPiP1pWave[kMaxPoints];
    double etaPiP1pWavee[kMaxPoints];
    double etaPiP1mWave[kMaxPoints];
    double etaPiP1mWavee[kMaxPoints];
    double etaPiD0Wave[kMaxPoints];
    double etaPiD0Wavee[kMaxPoints];
    double etaPiD1Wave[kMaxPoints];
    double etaPiD1Wavee[kMaxPoints];
    double etaPiDpWave[kMaxPoints];
    double etaPiDpWavee[kMaxPoints];
    //double etaPiPXWave[kMaxPoints];
    //double etaPiPXWavee[kMaxPoints];
    //double f2PiSWave[kMaxPoints];
    //double f2PiSWavee[kMaxPoints];
    //double etaPiPWave[kMaxPoints];
    //double etaPiPWavee[kMaxPoints];
    double all[kMaxPoints];
    double alle[kMaxPoints];
    double phaseP0D0[kMaxPoints];
    double phaseP0D0e[kMaxPoints];
    double phaseSD0[kMaxPoints];
    double phaseSD0e[kMaxPoints];
    double phaseSP0[kMaxPoints];
    double phaseSP0e[kMaxPoints];
    double phaseSP1m[kMaxPoints];
    double phaseSP1me[kMaxPoints];
    double phaseD1[kMaxPoints];
    double phaseD1e[kMaxPoints];
    double phaseP1D1[kMaxPoints];
    double phaseP1D1e[kMaxPoints];
    double phaseP1D1p[kMaxPoints];
    double phaseP1D1pe[kMaxPoints];

    int line = 0;
    while( ! in.eof() ){
        
        in >> mass[line]
        >> etaPiSWave[line] >> etaPiSWavee[line]
        >> etaPiP0Wave[line] >> etaPiP0Wavee[line]
        >> etaPiP1mWave[line] >> etaPiP1mWavee[line]
        >> etaPiD0Wave[line] >> etaPiD0Wavee[line]
        >> etaPiD1Wave[line] >> etaPiD1Wavee[line]
        >> etaPiP1pWave[line] >> etaPiP1pWavee[line]
        >> etaPiDpWave[line] >> etaPiDpWavee[line]
        >> all[line] >> alle[line]
        >> phaseSD0[line] >> phaseSD0e[line]
        >> phaseSP0[line] >> phaseSP0e[line]
        >> phaseSP1m[line] >> phaseSP1me[line]
        >> phaseD1[line] >> phaseD1e[line]
        >> phaseP0D0[line] >> phaseP0D0e[line]
        >> phaseP1D1[line] >> phaseP1D1e[line]
        >> phaseP1D1p[line] >> phaseP1D1pe[line];

        eventCounter += all[line];
        
        line++;

	if(line>=65)break;
    }
    
    TGraphErrors etaPiSWaveGraph( line, mass, etaPiSWave, masse, etaPiSWavee );
    etaPiSWaveGraph.SetMarkerStyle( 20 );
    etaPiSWaveGraph.SetMarkerSize( .5 );
    TGraphErrors etaPiD0WaveGraph( line, mass, etaPiD0Wave, masse, etaPiD0Wavee );
    etaPiD0WaveGraph.SetMarkerStyle( 20 );
    etaPiD0WaveGraph.SetMarkerSize( 0.5 );
    TGraphErrors etaPiD1WaveGraph( line, mass, etaPiD1Wave, masse, etaPiD1Wavee );
    etaPiD1WaveGraph.SetMarkerStyle( 20 );
    etaPiD1WaveGraph.SetMarkerSize( 0.5 );
    TGraphErrors etaPiDpWaveGraph( line, mass, etaPiDpWave, masse, etaPiDpWavee );
    etaPiDpWaveGraph.SetMarkerStyle( 20 );
    etaPiDpWaveGraph.SetMarkerSize( 0.5 );
    
    TGraphErrors etaPiP0WaveGraph( line, mass, etaPiP0Wave, masse, etaPiP0Wavee );
    etaPiP0WaveGraph.SetMarkerStyle( 20 );
    etaPiP0WaveGraph.SetMarkerSize( 0.5 );
    
    TGraphErrors etaPiP1mWaveGraph( line, mass, etaPiP1mWave, masse, etaPiP1mWavee );
    etaPiP1mWaveGraph.SetMarkerStyle( 20 );
    etaPiP1mWaveGraph.SetMarkerSize( 0.5 );
    
    TGraphErrors etaPiP1pWaveGraph( line, mass, etaPiP1pWave, masse, etaPiP1pWavee );
    etaPiP1pWaveGraph.SetMarkerStyle( 20 );
    etaPiP1pWaveGraph.SetMarkerSize( 0.5 );
    
    TGraphErrors allGraph( line, mass, all, masse, alle );
    allGraph.SetMarkerStyle( 20 );
    allGraph.SetMarkerSize( 0.5 );
    
    TGraphErrors phaseD0Graph( line, mass, phaseP0D0, masse, phaseP0D0e );
    phaseD0Graph.SetMarkerStyle( 20 );
    phaseD0Graph.SetMarkerSize( 0.5 );
    TGraphErrors phaseD1Graph( line, mass, phaseD1, masse, phaseD1e );
    phaseD1Graph.SetMarkerStyle( 20 );
    phaseD1Graph.SetMarkerSize( 0.5 );
    
    int ymax=50000;
 
    TCanvas* can = new TCanvas( "can", "Amplitude Analysis Plots", 800, 800 );
    can->Divide( 3, 3 );
    
    can->cd( 1 );
    TH1F h1( "h1", "#eta#pi S0", 1, ll, ul );
    h1.SetMaximum( ymax );
    h1.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h1.SetStats( 0 );
    h1.Draw();
    etaPiSWaveGraph.Draw( "P" );
    
    can->cd( 2 );
    TH1F h2( "h2", "#eta#pi P0", 1, ll, ul );
    h2.SetMaximum( ymax );
    h2.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h2.SetStats( 0 );
    h2.Draw();
    etaPiP0WaveGraph.Draw( "P" );
    
    can->cd( 3 );
    TH1F h3( "h3", "#eta#pi P1-", 1, ll, ul );
    h3.SetMaximum( ymax );
    h3.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h3.SetStats( 0 );
    h3.Draw();
    etaPiP1mWaveGraph.Draw( "P" );
    
    can->cd( 4 );
    TH1F h4( "h4", "#eta#pi P1+", 1, ll, ul );
    h4.SetMaximum( ymax );
    h4.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h4.SetStats( 0 );
    h4.Draw();
    etaPiP1pWaveGraph.Draw( "P" );
    
    can->cd( 5 );
    TH1F h5( "h5", "#eta#pi D0", 1, ll, ul );
    h5.SetMaximum( ymax );
    h5.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h5.SetStats( 0 );
    h5.Draw();
    etaPiD0WaveGraph.Draw( "P" );
    
    can->cd( 6 );
    TH1F h6( "h6", "#eta#pi D1-", 1, ll, ul );
    h6.SetMaximum( ymax );
    h6.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h6.SetStats( 0 );
    h6.Draw();
    etaPiD1WaveGraph.Draw( "P" );
    
    can->cd( 7 );
    TH1F h7( "h7", "#eta#pi D1+", 1, ll, ul );
    h7.SetMaximum( ymax );
    h7.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h7.SetStats( 0 );
    h7.Draw();
    etaPiDpWaveGraph.Draw( "P" );
    
    can->cd( 8 );
    TH1F h8( "h8", "#eta#pi All Waves", 1, ll, ul );
    h8.SetMaximum( ymax );
    h8.GetXaxis()->SetTitle( "#eta#pi Invariant Mass [GeV/c^{2}]" );
    h8.SetStats( 0 );
    h8.Draw();
    allGraph.Draw( "P" );
    
    can->SaveAs("CombinedFit.pdf");
    cout << "Total number of events:  " << eventCounter << endl;
}

