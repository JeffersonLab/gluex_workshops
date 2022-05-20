// Flattened DATA trees separated by polarization orientation:
TString FND0("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/sp2017/pol0/tree_pi0eta__B4_M17_M7_FLAT_*.root");
TString FND45("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/sp2017/pol45/tree_pi0eta__B4_M17_M7_FLAT_*.root");
TString FND90("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/sp2017/pol90/tree_pi0eta__B4_M17_M7_FLAT_*.root");
TString FND135("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/sp2017/pol135/tree_pi0eta__B4_M17_M7_FLAT_*.root");

// Flattened MC Trees (Note: Use phasespace distributed "flat" MC - no model!)
TString FNMC("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/flatMC/tree_pi0eta__B4_M17_M7_FLAT_MC.root");
TString FNMCGen("/work/gluex_workshop_data/tutorial_2022/session2d/flatten/flatMC_gen/tree_pi0eta__B4_M17_M7_FLAT_MCGEN.root");

TString NT("ntFSGlueX_MODECODE");

void setup(){
  if (FSModeCollection::modeVector().size() != 0) return;
  FSModeCollection::addModeInfo("101_1")->addCategory("m101_1");

// DEFINITION OF CUTS:
  // STATIC CUTS
  FSCut::defineCut("unusedE","EnUnusedSh<0.1");                               // UnusedEnergy < 0.1GeV
  FSCut::defineCut("unusedTracks","NumUnusedTracks<1");                       // No unsused tracks
  FSCut::defineCut("zProton","ProdVz>=52&&ProdVz<=78");                       // Production vertex z-position
  FSCut::defineCut("protMom","MOMENTUM([p+])>=0.3");                          // Proton momentum > 0.3GeV/c
  FSCut::defineCut("cet0103","OR(abs(-1*MASS2(GLUEXTARGET,-[p+])-0.2)<0.1)"); // 0.1 < t < 0.3
  FSCut::defineCut("e8288","(EnPB>8.2&&EnPB<8.8)");                           // 8.2 < E_beam < 8.8
  FSCut::defineCut("chi2","Chi2DOF<3.3");                                     // Chi2/ndf < 3.3
  FSCut::defineCut("photFiducialA","(acos(COSINE([eta]a))*180/3.141>2.5 && 
                                     acos(COSINE([eta]a))*180/3.141<10.3) || (acos(COSINE([eta]a))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialB","(acos(COSINE([eta]b))*180/3.141>2.5 && 
                                    acos(COSINE([eta]b))*180/3.141<10.3) || (acos(COSINE([eta]b))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialC","(acos(COSINE([pi0]a))*180/3.141>2.5 && 
                                    acos(COSINE([pi0]a))*180/3.141<10.3) || (acos(COSINE([pi0]a))*180/3.141>11.9)");
  FSCut::defineCut("photFiducialD","(acos(COSINE([pi0]b))*180/3.141>2.5 && 
                                     acos(COSINE([pi0]b))*180/3.141<10.3) || (acos(COSINE([pi0]b))*180/3.141>11.9)");
  FSCut::defineCut("rejectOmega","!((MASS([pi0]a,[eta]a)<0.15 && MASS([pi0]b,[eta]b)<0.15) || 
                                    (MASS([pi0]a,[eta]b)<0.15 && MASS([pi0]b,[eta]a)<0.15) || 
                                    (MASS([pi0]a,[eta]a)<0.12 && MASS([pi0]b,[eta]a)<0.12) || 
                                    (MASS([pi0]a,[eta]b)<0.12 && MASS([pi0]b,[eta]b)<0.12))");
  FSCut::defineCut("delta","MASS([p+],[pi0])>1.4");

  FSCut::defineCut("a2","MASS([eta],[pi0])>=1.04 && MASS([eta],[pi0])<=1.56");

  // CUTS W/ SIDEBANDS
  FSCut::defineCut("rf","OR(abs(RFDeltaT)<2.0)", "abs(RFDeltaT)>2.0",0.125);
  FSCut::defineCut("eta","abs(MASS([eta])-0.548)<0.05","(abs(MASS([eta])-0.548+0.15)<0.05 || abs(MASS([eta])-0.548-0.15)<0.05)", 0.5);
  FSCut::defineCut("pi0","abs(MASS([pi0])-0.135)<0.025","(abs(MASS([pi0])-0.135+0.05)<0.025 || abs(MASS([pi0])-0.135-0.05)<0.025)", 0.5);
}

void skim_a2(){
  setup();

  /////////////////////////
  // DATA
  /////////////////////////
  // Write out skimmed DATA trees with GENERAL CUTS applied:
  FSModeTree::skimTree(FND0,NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol0_GENERAL_SKIM_A2.root","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega,a2)");
  FSModeTree::skimTree(FND45,NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_GENERAL_SKIM_A2.root","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega,a2)");
  FSModeTree::skimTree(FND90,NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_GENERAL_SKIM_A2.root","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega,a2)");
  FSModeTree::skimTree(FND135,NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_GENERAL_SKIM_A2.root","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega,a2)");
  
  // Write out skimmed tree with GENERAL CUTS applied for SIGNAL REGION ONLY:
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_SIGNAL_SKIM_A2.root","CUT(rf,eta,pi0)");
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_SIGNAL_SKIM_A2.root","CUT(rf,eta,pi0)");
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_SIGNAL_SKIM_A2.root","CUT(rf,eta,pi0)");
  
  // Write out skimmed tree with GENERAL CUTS applied with SIDEBAND WEIGHTS:
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol0_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol0_SIDEBANDS_SKIM_A2.root","CUTSBWT(rf,eta,pi0)");
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_SIDEBANDS_SKIM_A2.root","CUTSBWT(rf,eta,pi0)");
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_SIDEBANDS_SKIM_A2.root","CUTSBWT(rf,eta,pi0)");
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_SIDEBANDS_SKIM_A2.root","CUTSBWT(rf,eta,pi0)");

  vector< pair<TString,TString> > friendTreeContents;
  friendTreeContents.push_back(pair<TString,TString>("weight","CUTSBWT(rf,eta,pi0)"));
  FSModeTree::createFriendTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol0_SIDEBANDS_SKIM_A2.root",NT,"","weight",friendTreeContents);
  FSModeTree::createFriendTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol45_SIDEBANDS_SKIM_A2.root",NT,"","weight",friendTreeContents);
  FSModeTree::createFriendTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol90_SIDEBANDS_SKIM_A2.root",NT,"","weight",friendTreeContents);
  FSModeTree::createFriendTree("tree_pi0eta__B4_M17_M7_DATA_sp17_pol135_SIDEBANDS_SKIM_A2.root",NT,"","weight",friendTreeContents);

  
  /////////////////////////
  // MC
  /////////////////////////
  
  // Write out skimmed MC trees with GENERAL CUTS applied:
  FSModeTree::skimTree(FNMC,NT,"","tree_pi0eta__B4_M17_M7_MC_sp17_pol0_GENERAL_SKIM_A2.root","CUT(unusedTracks,unusedE,zProton,chi2,cet0103,e8288,photFiducialA,photFiducialB,photFiducialC,photFiducialD,delta,protMom,rejectOmega,a2)");
  
  FSModeTree::skimTree("tree_pi0eta__B4_M17_M7_MC_sp17_pol0_GENERAL_SKIM_A2.root",NT,"","tree_pi0eta__B4_M17_M7_MC_sp17_pol0_SIGNAL_SKIM_A2.root","CUT(rf,eta,pi0)");
  vector< pair<TString,TString> > friendTreeContentsMC;
  friendTreeContentsMC.push_back(pair<TString,TString>("weight","CUTWT(rf,eta,pi0)"));
  FSModeTree::createFriendTree("tree_pi0eta__B4_M17_M7_MC_sp17_pol0_SIGNAL_SKIM_A2.root",NT,"","weight",friendTreeContentsMC);

  /////////////////////////
  // MC GEN
  /////////////////////////
  
  // Write out skimmed MC GEN tree with eBeam, t and m(etapi) cuts applied:
  FSModeTree::skimTree(FNMCGen,NT,"","tree_pi0eta__B4_M17_M7_MCGEN_sp17_pol0_GENERAL_SKIM_A2.root","OR(abs(-1*MCMASS2(GLUEXTARGET,-[p+])-0.2)<0.1) && (MCMASS([eta],[pi0])>=1.04 && MCMASS([eta],[pi0])<=1.56)  && (MCEnPB>8.2&&MCEnPB<8.8)");
}
