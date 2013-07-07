{

  enum { kMaxParticles = 10 };

  TFile* f = new TFile( "threepi_data_gen.root" );
  TTree* tr = (TTree*)f->Get( "kin" );

  // this block sets up the read from the tree

  int nPart;
  float e[kMaxParticles];
  float px[kMaxParticles];
  float py[kMaxParticles];
  float pz[kMaxParticles];
  float eBeam;
  float pxBeam;
  float pyBeam;
  float pzBeam;

  tr->SetBranchAddress( "NumFinalState", &nPart );
  tr->SetBranchAddress( "E_FinalState", e );
  tr->SetBranchAddress( "Px_FinalState", px );
  tr->SetBranchAddress( "Py_FinalState", py );
  tr->SetBranchAddress( "Pz_FinalState", pz );
  tr->SetBranchAddress( "E_Beam", &eBeam );
  tr->SetBranchAddress( "Px_Beam", &pxBeam );
  tr->SetBranchAddress( "Py_Beam", &pyBeam );
  tr->SetBranchAddress( "Pz_Beam", &pzBeam );

  // book some histograms:

  TH2F* dalitz = new TH2F( "dalitz", "#pi^{+}#pi^{-}#pi^{+} Dalitz Plot", 100, 0.0, 3.5, 100, 0.0, 3.5 );
  TH1F* pip1pim = new TH1F( "pip1pim", "#pi^{+}_{1}#pi^{-} Invariant Mass", 100, 0.0, 2.0 );
  TH1F* pip2pim = new TH1F( "pip2pim", "#pi^{+}_{2}#pi^{-} Invariant Mass", 100, 0.0, 2.0 );
  TH1F* threepi = new TH1F( "threepi", "#pi^{+}#pi^{-}#pi^{+} Invariant Mass", 100, 0.7, 2.0 );

  // now loop over the tree entries:

  for( int i = 0; i < tr->GetEntries(); ++i ){

    tr->GetEntry( i );

    // neeed to know how the final state particles are packed in the tree
    // neutron, pi+1, pi-, pi+2

    TLorentzVector pip1( px[1], py[1], pz[1], e[1] );
    TLorentzVector pim( px[2], py[2], pz[2], e[2] );
    TLorentzVector pip2( px[3], py[3], pz[3], e[3] );

    dalitz->Fill( ( pip1 + pim ).M2(), ( pip2 + pim ).M2() );
    pip1pim->Fill( ( pip1 + pim ).M() );
    pip2pim->Fill( ( pip2 + pim ).M() );
    threepi->Fill( ( pip1 + pip2 + pim ).M() );
  }

  // now draw the histograms
  
  TCanvas can( "canvas", "Plot Canvas", 800, 800 );
  can.Divide( 2, 2 );

  can.cd( 1 );
  threepi->Draw();
  can.cd( 2 );
  dalitz->Draw( "colz" );
  can.cd( 3 );
  pip1pim->Draw();
  can.cd( 4 );
  pip2pim->Draw();
}
