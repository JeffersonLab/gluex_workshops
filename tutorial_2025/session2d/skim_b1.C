TString inDataDir( "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/dataTrees/RunPeriod-2018-08/" );
TString inMCDir( "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/mcTrees/RunPeriod-2018-08/" );
TString outDir( "/work/halld2/home/aschertz/gluex_workshops/tutorial_2025/session2d/trees4AmpTools/" );

// Flattened DATA trees:
TString FND( inDataDir + "tree_pi0pip0pippim__B4_flat_*.root" );

// Flattened MC Trees (Note: Use phasespace distributed "flat" MC - no model!)
TString FNMC( inMCDir + "tree_pi0pip0pippim__B4_flat_*.root" );

// Flattened Thrown MC Trees (from same sample as MC trees above)
TString FNMCGen( inMCDir + "thrown/tree_thrown_flat_*.root" );

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
TString OMEGASIG("(1.00*("+SIG4orSIG5+"))");
TString OMEGASB("(-0.50*("+SB4xorSB5+")-1.25*("+SB4andSB5+"))");


void setup(){
    if (FSModeCollection::modeVector().size() != 0) return;
    FSModeCollection::addModeInfo("100_112")->addCategory("m100_112");

// DEFINITION OF CUTS:
    // GENERAL CUTS
    FSCut::defineCut("chi2", "Chi2DOF<2.5");                                      // chi^2 / DoF < 2.5
    FSCut::defineCut("cohPeak", "(EnPB>8.2&&EnPB<8.8)");                          // 8.2 < E_beam < 8.8 GeV
    FSCut::defineCut("tCut", "OR(abs(-1*MASS2(GLUEXTARGET,-[p+]))<1.0)");         // -t < 1.0 GeV^2
    FSCut::defineCut("mm2", "abs(RMASS2(B,GLUEXTARGET,-[p+],-[pi01],-[pi02],-[pi+],-[pi-])) < 0.05");  // missing mass squared < 0.05 GeV^2

    // CUTS W/ SIDEBANDS
    FSCut::defineCut( "rf", "OR(abs(RFDeltaT)<2.0)", "abs(RFDeltaT)>2.0", 0.125 );

    // FRIEND TREE FOR 2D OMEGA SUBTRACTION
    FSTree::addFriendTree("M3PI");
}

void saveM3PI(){ //only needs to be run ONCE, sets up friend trees for convenience with the 2D sideband subtraction. If you've already run the plotting script, you do not need to worry about using this
    vector<pair<TString,TString> > friendTreeContents;
    friendTreeContents.push_back(pair<TString,TString>("M234","MASS(2,3,4)"));
    friendTreeContents.push_back(pair<TString,TString>("M235","MASS(2,3,5)"));
    FSTree::createFriendTree(FND,NT,"M3PI",friendTreeContents);
    FSTree::createFriendTree(FNMC,NT,"M3PI",friendTreeContents);
}

void skim_b1(){
  setup();

  /////////////////////////
  // DATA
  /////////////////////////
  // Write out skimmed DATA trees with GENERAL CUTS applied:
  FSModeTree::skimTree(FND,NT,"",outDir+"tree_pi0pi0pippim__B4_DATA_GENERAL_SKIM.root","CUT(chi2,cohPeak,tCut,mm2)");
  
  // Write out skimmed tree with GENERAL CUTS applied for SIGNAL REGION ONLY:
  FSModeTree::skimTree(outDir+"tree_pi0pi0pippim__B4_DATA_GENERAL_SKIM.root",NT,"",outDir+"tree_pi0pi0pippim__B4_DATA_SIGNAL.root",OMEGASIG+"*CUT(rf)");
  
  // Write out skimmed tree with GENERAL CUTS applied with SIDEBAND WEIGHTS:
  FSModeTree::skimTree(outDir+"tree_pi0pi0pippim__B4_DATA_GENERAL_SKIM.root",NT,"",outDir+"tree_pi0pi0pippim__B4_DATA_SIDEBANDS.root",OMEGASB+"*CUTSBWT(rf)");

  vector< pair<TString,TString> > friendTreeContents;
  friendTreeContents.push_back(pair<TString,TString>("weight","CUTSBWT(rf)"));
  FSModeTree::createFriendTree(outDir+"tree_pi0pi0pippim__B4_DATA_SIDEBANDS.root",NT,"","weight",friendTreeContents);

  
  /////////////////////////
  // MC
  /////////////////////////
  
  // Write out skimmed MC trees with GENERAL CUTS applied:
  FSModeTree::skimTree(FNMC,NT,"",outDir+"tree_pi0pi0pippim__B4_MC_GENERAL_SKIM.root","CUT(chi2,cohPeak,tCut,mm2)");
  
  // Write out skimmed tree with GENERAL CUTS applied for SIGNAL REGION ONLY:
  FSModeTree::skimTree(outDir+"tree_pi0pi0pippim__B4_MC_GENERAL_SKIM.root",NT,"",outDir+"tree_pi0pi0pippim__B4_MC_SIGNAL.root",OMEGASIG+"*CUT(rf)");

  vector< pair<TString,TString> > friendTreeContentsMC;
  friendTreeContentsMC.push_back(pair<TString,TString>("weight","CUTWT(rf)"));
  FSModeTree::createFriendTree(outDir+"tree_pi0pi0pippim__B4_MC_SIGNAL.root",NT,"","weight",friendTreeContentsMC);

  /////////////////////////
  // MC GEN
  /////////////////////////
  
  // Write out skimmed MC GEN tree with eBeam, t and m(omegapi) cuts applied:
  FSModeTree::skimTree(FNMCGen,NT,"",outDir+"tree_pi0pi0pippim__B4_MCGEN_GENERAL_SKIM.root","OR(abs(-1*MCMASS2(GLUEXTARGET,-[p+]))<0.1) && (MCMASS([pi01],[pi02],[pi+],[pi-])>=1.15 && MCMASS([pi01],[pi02],[pi+],[pi-])<=1.30)  && (MCEnPB>8.2&&MCEnPB<8.8)");
}
