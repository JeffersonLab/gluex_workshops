
{
  
  TString f( "res.root" );
  TString tr( "nt" );
  
  TH1F* phiHel = FSHistogram::getTH1F( f, tr, "HELPHI(2;3;1;B)", "(100,-3.14,3.14)", "" );
  phiHel->SetTitle( "Helicity Frame;#phi [rad];Candidates/31.4 mrad");

  TH1F* cosThHel = FSHistogram::getTH1F( f, tr, "HELCOSTHETA(2;3;1;B)", "(100,-1,1)", "" );
  cosThHel->SetTitle( "Helicity Frame;cos(#theta);Candidates/0.01");
  
  TH1F* bigPhi = FSHistogram::getTH1F( f, tr, "TMath::ATan2(PyP1,PxP1)", "(100,-3.14,3.14)", "" );
  bigPhi->SetTitle( "P_{#gamma} = 1;#Phi [rad];Candidates/31.4 mrad");
  
  TH1F* mass = FSHistogram::getTH1F( f, tr, "MASS(2,3)", "(100, 0.0, 3.0)", "" );
  mass->SetTitle( "M(#eta#pi);M(#eta#pi) [GeV];Candidates/30 MeV" );
  
  TH2F* phiVsCosThHel = FSHistogram::getTH2F( f, tr, "HELPHI(2;3;1;B):HELCOSTHETA(2;3;1;B)",
					      "(100,-1,1,100,-3.14,3.14)", "" );
  phiVsCosThHel->SetTitle( "Helicity Frame;cos(#theta);#phi" );

  TH2F* phiHelVsBigPhi = FSHistogram::getTH2F( f, tr, "HELPHI(2;3;1;B):TMath::ATan2(PyP1,PxP1)",
						"(100,-3.14,3.14,100,-3.14,3.14)", "" );
  phiHelVsBigPhi->SetTitle( "P_{#gamma} = 1;#Phi [rad];#phi_{helicity} [rad]" );
  
  TH2F* cosThHelVsBigPhi = FSHistogram::getTH2F( f, tr, "HELCOSTHETA(2;3;1;B):TMath::ATan2(PyP1,PxP1)",
						"(100,-3.14,3.14,100,-1,1)", "" );
  cosThHelVsBigPhi->SetTitle( "P_{#gamma} = 1;#Phi [rad];cos(#theta_{helicity})" );

  TCanvas* can = new TCanvas( "can", "Resonance Angles", 1600, 800 );
  can->Divide( 4, 2 );

  can->cd( 1 );
  mass->Draw();

  can->cd( 2 );
  cosThHel->Draw();

  can->cd( 5 );
  phiHel->Draw();
  
  can->cd( 4 );
  bigPhi->Draw();

  can->cd( 6 );
  phiVsCosThHel->Draw();

  can->cd( 7 );
  cosThHelVsBigPhi->Draw();
  
  can->cd( 8 );
  phiHelVsBigPhi->Draw();

  can->Print( "plotAngles.pdf" );
}
