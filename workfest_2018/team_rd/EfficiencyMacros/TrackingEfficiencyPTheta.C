void TrackingEfficiencyPTheta(int myparticle,double sigcut){
  TCut c1="TMath::Prob(KinFitChiSq,KinFitNDF)>0.1";
  TCut c2="fabs(BeamRFDeltaT)<2.";
  char mycut[200];
  sprintf(mycut,"fabs((MissingP3.Pz()-ReconP3.Pz())/sqrt(MissingP3_CovPzPz))<%f",sigcut);
  TCut cPz=mycut; 
  sprintf(mycut,"fabs((MissingP3.Py()-ReconP3.Py())/sqrt(MissingP3_CovPyPy))<%f",sigcut);
  TCut cPy=mycut;
  sprintf(mycut,"fabs((MissingP3.Px()-ReconP3.Px())/sqrt(MissingP3_CovPxPx))<%f",sigcut);
  TCut cPx=mycut;

  TTree *mytree;
  char mytitle[80];
  switch(myparticle){
  case 8:
    sprintf(mytitle,"Efficiency for #pi^{+}");
    mytree=TrackEff_MissingPiPlus;
    break;
  case 9: 
    sprintf(mytitle,"Efficiency for #pi^{-}");
    mytree=TrackEff_MissingPiMinus;
    break;
  case 14:
    sprintf(mytitle,"Efficiency for proton");
    mytree=TrackEff_MissingProton;
    break;
  default:
    break;    
  }


  TH2F *h1=new TH2F("h1","h1",100,0,10,120,0,120);
  mytree->Project("h1","MissingP3.Theta()*180./TMath::Pi():MissingP3.Mag()",c1&&c2&&"NumExtraTracks<2" ); 
  h1->Sumw2();
  TH2F *h1b=new TH2F("h1b","h1b",100,0,10,120,0,120);
  mytree->Project("h1b","MissingP3.Theta()*180./TMath::Pi():MissingP3.Mag()",c1&&!c2&&"NumExtraTracks<2" );
  h1b->Sumw2();
  h1->Add(h1b,-0.5);
  TH2F *h2=new TH2F("h2","h2",100,0,10,120,0,120);
  mytree->Project("h2","MissingP3.Theta()*180./TMath::Pi():MissingP3.Mag()",c1&&c2&&"NumExtraTracks==1"&&cPx&&cPy&&cPz );  
  h2->Sumw2();
  TH2F *h2b=new TH2F("h2b","h2b",100,0,10,120,0,120);
  mytree->Project("h2b","MissingP3.Theta()*180./TMath::Pi():MissingP3.Mag()",c1&&(!c2)&&"NumExtraTracks==1"&&cPx&&cPy&&cPz);
  h2b->Sumw2();
  h2->Add(h2b,-0.5);
  h2->Divide(h1);
  h2->SetMaximum(2.);
  h2->SetMinimum(0.);
  h2->SetXTitle("p [GeV/c]");
  h2->SetYTitle("#theta [degrees]");
  h2->SetTitle(mytitle);
  h2->Draw("colz");


}
