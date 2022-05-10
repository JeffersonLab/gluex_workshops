#include "DSelector_pippimeta__B4_M17_ver25.h"

void DSelector_pippimeta__B4_M17_ver25::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "pippimeta__B4_M17_ver25.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "pippimetaFlat_ver25.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
    std::deque<Particle_t> MyDelta;
    MyDelta.push_back(Proton); MyDelta.push_back(PiPlus);
    
    std::deque<Particle_t> MyEtaPiMinus;
    MyEtaPiMinus.push_back(Eta); MyEtaPiMinus.push_back(PiMinus);
    
    //ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
    //false/true below: use measured/kinfit data
    
    //PID
    dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
    //below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, PiPlus, SYS_TOF));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 1.0, PiPlus, SYS_BCAL));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 2.0, PiPlus, SYS_FCAL));
    
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, PiMinus, SYS_TOF));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 1.0, PiMinus, SYS_BCAL));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 2.0, PiMinus, SYS_FCAL));
    
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.6, Proton, SYS_TOF));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 1, Proton, SYS_BCAL));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 2, Proton, SYS_FCAL));
    
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 1.0, Gamma, SYS_BCAL));
    dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 1.0, Gamma, SYS_FCAL));
    
    //dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.3, Gamma, SYS_BCAL));
    //dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.6, Gamma, SYS_FCAL));
    
    
    //MASSES
    //dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
    //dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));
    
    //KINFIT RESULTS
    dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));
    
    //CUT MISSING MASS
    dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.05, 0.05));
    
    //BEAM ENERGY
    dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
    dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8, 12));
    
    //KINEMATICS
    dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));
    
    //CUT Unused Energy
    dAnalysisActions.push_back(new DCutAction_Energy_UnusedShowers(dComboWrapper, 0.1));
    
    // ANALYZE CUT ACTIONS
    // // Change MyPhi to match reaction
    dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyDelta, 1000, 0.9, 3.0, "Delta" );
    //dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyEtaPiMinus, 1000, 0.6, 3.0, "EtaPi" );
    
    dAnalysisActions.push_back(new DHistogramAction_vanHoveFour(dComboWrapper, false, Eta, PiMinus, PiPlus, Proton, ""));    // ANALYZE CUT ACTIONS

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
    dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
    dHist_MissingMassSquared_allcuts = new TH1I("MissingMassSquared_allcuts", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
    
    dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
    dHist_Chi2=new TH1I("Chi2",";#chi^{2}/NDF",100,0,30);
    dHist_ShowerQuality=new TH1I("ShowerQuality",";Shower Quality",200,0,1);
    dHist_ShowerQualityPostCuts=new TH1I("dHist_ShowerQualityPostCuts",";Shower Quality",200,0,1);

    ostringstream histname, histtitle;
    
    
    for (int i=0; i<8; i++) {
        histname.str("");
        histname<<"EtaPiMinusMass_Chi2_bins_"<<i;
        histtitle.str("");
        histtitle<<";M(#etaEpi^{-}) chi^{2}<"<<(i+1)*2;
        dHist_EtaPiMinusMass_Chi2_bins[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),200,0.6,3);
        
        histname.str("");
        histname<<"CosTheta_vs_EtaPiMinusMass_Chi2_bins_"<<i;
        histtitle.str("");
        histtitle<<";M(#etaEpi^{-}) chi^{2}<"<<(i+1)*2<<";cos#theta_{GJ}";
        dHist_CosTheta_vs_EtaPiMinusMass_Chi2_bins[i]=new TH2I(histname.str().c_str(),histtitle.str().c_str(),200,0.6,3,200,-1,1);
        
    }
    
    
    dHist_ProtonZ = new TH1I("ProtonZ_postCut",";proton Z (cm)",500, 0, 200);
    dHist_ProtonR = new TH2I("ProtonR _postCut",";proton Z (cm)",500, -5, 5,200, -5, 5);
    
    //Masses//
    dHist_EtaMass=new TH1I("EtaMass","#eta Mass",500,0.3,0.8);
    dHist_EtaMassPostDeltaPP=new TH1I("EtaMassPostDeltaPP","#eta Mass",500,0.3,0.8);
    
    dHist_DeltaPPMass=new TH1I("DeltaPPMass","#Delta^{++} Mass",500,0.9,4.0);
    dHist_EtaPiPlusPiMinusMass=new TH1I("EtaPiPlusPiMinusMass","",500,0.85,4.0);
    
    dHist_PiPlusPiMinusMass=new TH1I("PiPlusPiMinusMass","#pi^{+}#pi^{-} Mass",500,0.2,2.5);
    dHist_PiPlusPiMinusMassPostDeltaPP=new TH1I("PiPlusPiMinusMassPostDeltaPP","#pi^{+}#pi^{-} Mass",500,0.2,2.5);
    
    dHist_ProtonPiMinusMass=new TH1I("ProtonPiMinusMass","p#pi^{-} Mass",500,0.9,4.0);
    dHist_ProtonPiMinusMassPostDeltaPP=new TH1I("ProtonPiMinusMassPostDeltaPP","p#pi^{-} Mass",500,0.9,4.0);
    
    dHist_EtaPrimeMass=new TH1I("EtaPrimeMass",";M(#eta#pi^{+}#pi^{-})",500,0.5,4);
    dHist_EtaPrimeMassPostDeltaPP=new TH1I("EtaPrimeMassPostDeltaPP",";M(#eta#pi^{+}#pi^{-})",500,0.5,4);
    
    dHist_EtaPiMinusMass=new TH1I("EtaPiMinusMass","",500,0.6,3);
    dHist_EtaPiMinusMassPostDeltaPP=new TH1I("EtaPiMinusMassPostDeltaPP","",500,0.6,3);
    dHist_EtaPiMinusMassPostDeltaPP_lowt=new TH1I("EtaPiMinusMassPostDeltaPP_lowT","",500,0.6,3);
    dHist_EtaPiMinusMassPostDeltaPP_lowt_vH=new TH1I("EtaPiMinusMassPostDeltaPP_lowT_vH","M(#eta#pi^{-}) -t<0.6 && vH cut",500,0.6,3);
    dHist_EtaPiMinusMassShowerCut=new TH1I("EtaPiMinusMassShowerCut",";M(#eta#pi^{-})",500,0.6,3);
    dHist_EtaPiMinusMassShowerCut_lowt=new TH1I("EtaPiMinusMassShowerCut_lowt",";M(#eta#pi^{-})",500,0.6,3);

    for (int i=0; i<5; i++) {
        histname.str("");
        histname<<"EtaPiMinusMass_DeltaPP_"<<i;
        histtitle.str("");
        histtitle<<";M(#eta#pi^{-}) M(p#pi^{+}) bins";
        dHist_EtaPiMinusMass_DeltaPP[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),200,0.6,3);
        
        histname.str("");
        histname<<"CosTheta_vs_EtaPiMinusMass_DeltaPP_bins_"<<i;
        histtitle.str("");
        histtitle<<";M(#etaEpi^{-}) #Delta^{++} bins;cos#theta_{GJ}";
        dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[i]=new TH2I(histname.str().c_str(),histtitle.str().c_str(),200,0.6,3,200,-1,1);
    }
    
    dHist_EtaPiPlusMass=new TH1I("EtaPiPlusMass","",500,0.6,3);
    dHist_EtaPiPlusMassPostDeltaPP=new TH1I("EtaPiPlusMassPostDeltaPP","",500,0.6,3);
    
    dHist_EtaProtonMass=new TH1I("EtaProtonMass","",500,1.3,5);
    dHist_EtaProtonMassPostDeltaPP=new TH1I("dHist_EtaProtonMassPostDeltaPP",";p#eta Mass",500,1.3,5);
    
    dHist_ProtonPiMinusMass_vs_DeltaPPMass=new TH2I("ProtonPiMinusMass_vs_DeltaPPMass",";#Delta^{++} Mass^{2};p#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_PiPlusPiMinusMass_vs_DeltaPPMass=new TH2I("PiPlusPiMinusMass_vs_DeltaPPMass",";#Delta^{++} Mass;#pi^{+}#pi^{-} Mass",200,0.9,4.0,200,0.2,2.5);
    dHist_PiPlusPiMinusMass2_vs_DeltaPPMass2=new TH2I("PiPlusPiMinusMass2_vs_DeltaPPMass2",";#Delta^{++} Mass^{2};#pi^{+}#pi^{-} Mass^{2}",200,1,14.0,200,0,6);
    dHist_EtaPiMinusMass_vs_EtaPiPlusMass=new TH2I("EtaPiMinusMass_vs_EtaPiPlusMass",";#eta#pi^{+} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,10);
    dHist_EtaPiMinusMass_vs_ProtonPiMinusMass=new TH2I("EtaPiMinusMass_vs_ProtonPiMinusMass",";p#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiPlusMass_vs_ProtonPiPlusMass=new TH2I("EtaPiPlusMass_vs_ProtonPiPlusMass",";p#pi^{+} Mass^{2};#eta#pi^{+} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass=new TH2I("EtaPiMinusMass_vs_PiPlusPiMinusMass",";#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiPlususMass_vs_PiPlusPiMinusMass=new TH2I("EtaPiPlusMass_vs_PiPlusPiMinusMass",";#pi^{+}#pi^{-} Mass^{2};#eta#pi^{+} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass=new TH2I("EtaPiMinusMass_vs_EtaPiPlusPiMinusMass",";#eta#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,6);
    
    dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt=new TH2I("EtaPiMinusMass_vs_EtaPiPlusMass_lowt",";#eta#pi^{+} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,10);
    dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt=new TH2I("EtaPiMinusMass_vs_ProtonPiMinusMass_lowt",";p#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt=new TH2I("EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt",";#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt=new TH2I("EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt",";#eta#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,14);
    
    dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt_vH=new TH2I("EtaPiMinusMass_vs_EtaPiPlusMass_lowt_vH",";#eta#pi^{+} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,10);
    dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt_vH=new TH2I("EtaPiMinusMass_vs_ProtonPiMinusMass_lowt_vH",";p#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt_vH=new TH2I("EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt_vH",";#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,14.0,200,0,10);
    dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt_vH=new TH2I("EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt_vH",";#eta#pi^{+}#pi^{-} Mass^{2};#eta#pi^{-} Mass^{2}",200,0,10.0,200,0,14);
    
    
    dHist_CosTheta_vs_EtaPiMinusMass=new TH2I("CosTheta_vs_EtaPiMinusMass",";M(#pi^{-}#eta); cos#theta",200,0.6,3,200,-1,1);
    dHist_CosTheta_vs_EtaPiMinusMass_lowt=new TH2I("CosTheta_vs_EtaPiMinusMass_lowt",";M(#pi^{-}#eta); cos#theta",200,0.6,3,200,-1,1);
    dHist_CosTheta_vs_EtaPiMinusMass_lowt_vH=new TH2I("CosTheta_vs_EtaPiMinusMass_lowt_vH",";M(#pi^{-}#eta); cos#theta",200,0.6,3,200,-1,1);
    
    dHist_Mandlestam_t=new TH1I("Mandlestam_t",";-t (GeV^{2})",200,0,6);
    
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_back_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins_back[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_DeltaSub_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins_DeltaSub[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_ProbWeight_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins_ProbWeight[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
    }
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"DeltaPPMass_tBins_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_DeltaPPMass_tBins[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.9,4.0);
    }
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"PiPlusPiMinusMass_tBins_sig_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_PiPlusPiMinusMass_tBins_sig[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.2,2.5);
        
        histname.str("");
        histname<<"PiPlusPiMinusMass_tBins_back_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_PiPlusPiMinusMass_tBins_back[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.2,2.5);
        
        histname.str("");
        histname<<"PiPlusPiMinusMass_tBins_DeltaSub_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_PiPlusPiMinusMass_tBins_DeltaSub[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.2,2.5);
        
    }
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"EtaPiPlusMass_tBins_sig_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPiPlusMass_tBins_sig[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        histname.str("");
        histname<<"EtaPiPlusMass_tBins_back_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPiPlusMass_tBins_back[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        histname.str("");
        histname<<"EtaPiPlusMass_tBins_DeltaSub_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPiPlusMass_tBins_DeltaSub[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
    }
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"ProtonPiMinusMass_tBins_sig_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_ProtonPiMinusMass_tBins_sig[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.9,4.0);
        
        histname.str("");
        histname<<"ProtonPiMinusMass_tBins_back_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_ProtonPiMinusMass_tBins_back[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.9,4.0);
        
        histname.str("");
        histname<<"ProtonPiMinusMass_tBins_DeltaSub_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_ProtonPiMinusMass_tBins_DeltaSub[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.9,4.0);
    }
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"EtaPrimeMass_tBins_sig_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPrimeMass_tBins_sig[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.5,4);
        
        histname.str("");
        histname<<"EtaPrimeMass_tBins_back_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPrimeMass_tBins_back[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.5,4);
        
        histname.str("");
        histname<<"EtaPrimeMass_tBins_DeltaSub_"<<i;
        histtitle.str("");
        histtitle<<0.0+(i/10.)<<"<t<"<<0.1+(i/10.)<<";M(p#pi^{+})";
        dHist_EtaPrimeMass_tBins_DeltaSub[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.5,4);
        
    }
    
    for (int i=0; i<21; i++) {
        histname.str("");
        histname<<"CosThetaEta_GJ_vs_EtaPiMinusMass_tBins"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-});cos#theta";
        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins[i]=new TH2I(histname.str().c_str(),histtitle.str().c_str(),50,0.6,3,50,-1,1);
    }
    
    dHist_VanHove=new TH2I("VanHove",";X;Y",100,-3,3,100,-3,3);
    dHist_VanHove_positiveq1=new TH2I("VanHove_positiveq1",";X;Y",100,-3,3,100,-3,3);
    dHist_VanHove_positiveq2=new TH2I("VanHove_positiveq2",";X;Y",100,-3,3,100,-3,3);
    dHist_VanHove_positiveq3=new TH2I("VanHove_positiveq3",";X;Y",100,-3,3,100,-3,3);
    dHist_VanHove_positiveq23=new TH2I("VanHove_positiveq23",";X;Y",100,-3,3,100,-3,3);
    dHist_DeltaPPMass_vH=new TH1I("DeltaPPMass_vH",";M(#pi^{-}p)",200,1,4);
    dHist_EtaPiMinusMass_vH=new TH1I("EtaPiMinusMass_vH",";M(#eta#pi^{-})",500,0.6,3);
    dHist_EtaPiPlusMass_vH=new TH1I("EtaPiPlusMass_vH",";M(#eta#pi^{+})",500,0.6,3);
    dHist_EtaPrimeMass_vH=new TH1I("EtaPrimeMass_vH",";M(#eta#pi^{+}#pi^{-})",500,0.5,4);
    dHist_ProtonPiMinusMass_vH=new TH1I("ProtonPiMinusMass_vH","p#pi^{-} Mass",500,0.9,4.0);
    dHist_PiPlusPiMinusMass_vH=new TH1I("PiPlusPiMinusMass_vH","#pi^{+}#pi^{-} Mass",500,0.2,2.5);
    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_vH=new TH2I("CosThetaEta_GJ_vs_EtaPiMinusMass_vH",";M(#pi^{-}#eta); cos#theta_{#eta}",200,0.6,3,200,-1,1);
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_vH_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins_vH[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
        
        
    }
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_vH_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-});cos#theta";
        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_vH[i]=new TH2I(histname.str().c_str(),histtitle.str().c_str(),50,0.6,3,50,-1,1);
        
        
    }
    
    dHist_VanHove4d=new TH2I("VanHove_4d",";Phi; Theta",200,0,6.284,200,0,3.142);
    dHist_VanHove4d_Actions=new TH2I("VanHove_4d_Actions",";Phi; Theta",200,0,6.284,200,0,3.142);
    dHist_VanHove4d_Theta=new TH2I("VanHove_4d_Theta",";Phi; Theta",200,0,6.284,200,0,3.142);
    
    
    dHist_VanHove4d_q1q2cut=new TH2I("VanHove_4d_q1q2cut",";Phi; Theta",200,0,6.284,200,0,3.142);
    
    dHist_EtaPiPlusMass_VanHove4d=new TH1I("EtaPiPlusMass_VanHove4d","",500,0.6,3);
    dHist_EtaPiMinusMass_VanHove4d=new TH1I("EtaPiMinusMass_VanHove4d","",500,0.6,3);
    dHist_EtaPiPlusPiMinusMass_VanHove4d=new TH1I("EtaPiPlusPiMinusMass_VanHove4d","M(#eta#pi^{+}#pi^{-})",500,0.5,4);
    dHist_PiPlusPiMinusMass_VanHove4d=new TH1I("PiPlusPiMinusMass_VanHove4d","#pi^{+}#pi^{-} Mass",500,0.2,2.5);
    dHist_ProtonPiMinusMass_VanHove4d=new TH1I("ProtonPiMinusMass_VanHove4d","p#pi^{-} Mass",500,0.9,4.0);
    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_VanHove4d=new TH2I("CosThetaEta_GJ_vs_EtaPiMinusMass_VanHove4d",";M(#pi^{-}#eta); cos#theta_{#eta}",200,0.6,3,200,-1,1);
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"EtaPiMinusMass_tBins_VanHove_4d_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiMinusMass_tBins_VanHove_4d[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
    }
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"PiPlusPiMinusMass_tBins_VanHove_4d_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_PiPlusPiMinusMass_tBins_VanHove_4d[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.2,2.5);
    }
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"EtaPiPlusMass_tBins_VanHove_4d_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_EtaPiPlusMass_tBins_VanHove_4d[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3);
    }
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"ProtonPiMinusMass_tBins_VanHove_4d_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_ProtonPiMinusMass_tBins_VanHove_4d[i]=new TH1I(histname.str().c_str(),histtitle.str().c_str(),100,1.0,4.);
    }
    
    for (int i=0; i<20; i++) {
        histname.str("");
        histname<<"CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_VanHove4d_"<<i;
        histtitle.str("");
        histtitle<<0.1+(i/10.)<<"<t<"<<0.2+(i/10.)<<";M(#eta#pi^{-})";
        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_VanHove_4d[i]=new TH2I(histname.str().c_str(),histtitle.str().c_str(),100,0.6,3,100,-1,1);
    }
    
    
    
    dHist_EtaPiMinusMass_RhoSelection=new TH1I("EtaPiMinusMass_RhoSelection",";M(#eta#pi^{-}), #rho Selected",200,0.6,3);
    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection=new TH2I("CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection",";M(#eta#pi^{-}); cos#theta",100,0.6,3,100,-1,1);
    
    dHist_CosThetaEta_RhoGJ=new TH1I("CosThetaEta_RhoGJ","",100,-1.2,1.2);
    dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ=new TH2I("CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ",";M(#eta#rho); cos#theta",100,1.0,4,100,-1,1);
    dHist_EtaPiPlusPiMinusMass_RhoGJ=new TH1I("EtaPiPlusPiMinusMass_RhoGJ",";M(#eta#pi^{+}#pi^{-})",500,0.9,4);
    
    dHist_PiPlusPiMinusMass_CutDelta=new TH1I("PiPlusPiMinusMass_CutDelta","#pi^{+}#pi^{-} Mass",500,0.2,2.5);
    dHist_EtaPiMinusMass_RhoSelection_CutDelta=new TH1I("EtaPiMinusMass_RhoSelection_CutDelta",";M(#eta#pi^{-}), #rho Selected",500,0.6,3);
    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection_CutDelta=new TH2I("CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection_CutDelta",";M(#eta#pi^{-}); cos#theta",100,0.6,3,100,-1,1);
    
    dHist_EtaPiPlusPiMinusMass_RhoGJ_CutDelta=new TH1I("EtaPiPlusPiMinusMass_RhoGJ_CutDelta",";M(#eta#pi^{+}#pi^{-})",200,0.9,4);
    dHist_CosThetaEta_RhoGJ_CutDelta=new TH1I("CosThetaEta_RhoGJ_CutDelta","",100,-1.,1.);
    dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ_CutDelta=new TH2I("CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ_CutDelta",";M(#eta#rho); cos#theta",100,1.0,4,100,-1,1);
    
	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("MissingMassSquared");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KinFitChiSqPerNDF");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_E");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_E");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_E");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_E");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_E");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Eta_px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Eta_py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Eta_pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Eta_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("EtaPiMinusMass"); //fundamental = char, int, float, double, etc.
    dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("DeltaPPMass"); //fundamental = char, int, float, double, etc.
    dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("Mandlestam_t"); //fundamental = char, int, float, double, etc.
    dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("tagWeight"); //fundamental = char, int, float, double, etc.
    dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("sidebandWeight"); //fundamental = char, int, float, double, etc.
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHq1");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHq2");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHq3");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHx");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHr");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHy");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vHomega");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("q1_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("q2_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("q3_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("q4_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("theta_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("phi_4d");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("r_4d");


	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_pippimeta__B4_M17_ver25::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	//TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;
    set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMassb;
    set<map<Particle_t, set<Int_t> > >  locUsedSoFar_Vertexing;
	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();
		Int_t locPiMinusTrackID = dPiMinusWrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E());
			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();
        double ShowerQuality1 = dPhoton1Wrapper->Get_Shower_Quality();
        double ShowerQuality2 = dPhoton2Wrapper->Get_Shower_Quality();
		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles

			dHist_MissingMassSquared->Fill(locMissingMassSquared);
			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		
        TLorentzVector locGammaGamma=locPhoton1P4+locPhoton2P4;
        TLorentzVector locEtaP4=locPhoton1P4+locPhoton2P4;
        double locGammaGammaMass=locGammaGamma.M();
        
        TLorentzVector locPiPlusProton=locPiPlusP4+locProtonP4;
        double locPiPlusProtonMass=locPiPlusProton.M();
        double locPiPlusProtonMassSq=locPiPlusProton.Mag2();
        
        TLorentzVector locEtaPiMinus=locGammaGamma+locPiMinusP4;
        double locEtaPiMinusMass=locEtaPiMinus.M();
        double locEtaPiMinusMassSq=locEtaPiMinus.Mag2();
        
        TLorentzVector locEtaProton=locGammaGamma+locProtonP4;
        double locEtaProtonMass=locEtaProton.M();
        
        TLorentzVector locPiMinusProton=locPiMinusP4+locProtonP4;
        double locPiMinusProtonMass=locPiMinusProton.M();
        double locPiMinusProtonMassSq=locPiMinusProton.Mag2();
        
        TLorentzVector locEtaPiPlus=locGammaGamma+locPiPlusP4;
        double locEtaPiPlusMass=locEtaPiPlus.M();
        double locEtaPiPlusMassSq=locEtaPiPlus.Mag2();
        
        TLorentzVector locPiPlusPiMinus=locPiMinusP4+locPiPlusP4;
        double locPiPlusPiMinusMass=locPiPlusPiMinus.M();
        double locPiPlusPiMinusMassSq=locPiPlusPiMinus.Mag2();
        
        TLorentzVector locEtaPiPlusPiMinus=locEtaP4+locPiMinusP4+locPiPlusP4;
        double locEtaPiPlusPiMinusMass=locEtaPiPlusPiMinus.M();
        double locEtaPiPlusPiMinusMasSq=locEtaPiPlusPiMinus.Mag2();
        
        //t with delta++
        TLorentzVector Mandlestam_t=locPiPlusProton-dTargetP4;
        double locMandlestam_t=Mandlestam_t.Mag2();
        
        double locConfidenceLevel = dComboWrapper->Get_ConfidenceLevel_KinFit("");
        double locKinFitChiSqPerNDF = dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit("");
        
        ///CoM Boost
        TLorentzVector locCoMP4=locBeamP4 + dTargetP4;
        TVector3 boostCoM=-(locCoMP4.Vect())*(1.0/locCoMP4.E());
        //Boost in CoM
        TLorentzVector locProtonP4_CM=locProtonP4;
        TLorentzVector locBeamP4_CM=locBeamP4;
        TLorentzVector locEtaP4_CM=locGammaGamma;
        TLorentzVector locPiMinusP4_CM=locPiMinusP4;
        TLorentzVector locEtaPiMinusP4_CM=locEtaPiMinus;
        TLorentzVector locDeltaPPP4_CM=locPiPlusProton;
        TLorentzVector locPiPlusP4_CM=locPiPlusP4;
        TLorentzVector locEtaPiPlusPiMinus_CM=locEtaPiPlusPiMinus;
        TLorentzVector locPiPlusPiMinus_CM=locPiPlusPiMinus;
        
        locProtonP4_CM.Boost(boostCoM);
        locDeltaPPP4_CM.Boost(boostCoM);
        locEtaPiMinusP4_CM.Boost(boostCoM);
        locBeamP4_CM.Boost(boostCoM);
        locEtaP4_CM.Boost(boostCoM);
        locPiMinusP4_CM.Boost(boostCoM);
        locPiPlusP4_CM.Boost(boostCoM);
        locEtaPiPlusPiMinus_CM.Boost(boostCoM);
        locPiPlusPiMinus_CM.Boost(boostCoM);
        
        //GJ Boost
        TVector3 boostGJ=-(locEtaPiMinusP4_CM.Vect())*(1.0/locEtaPiMinusP4_CM.E());
        TVector3 boostGJ_EtaRho=-(locEtaPiPlusPiMinus_CM.Vect())*(1.0/locEtaPiPlusPiMinus_CM.E());
        
        //Define GJ frame vectors
        TLorentzVector locEtaPiMinus_GJ=locEtaPiMinusP4_CM;
        TLorentzVector locEtaP4_GJ=locEtaP4_CM;
        TLorentzVector locPiminusP4_GJ=locPiMinusP4_CM;
        TLorentzVector locBeamP4GJ=locBeamP4_CM;
        
        TLorentzVector locEtaP4_RhoGJ=locEtaP4_CM;
        TLorentzVector locRhoP4_RhoGJ=locPiPlusPiMinus_CM;
        TLorentzVector locBeamP4_RhoGJ=locBeamP4_CM;
        
        //Boost in GJ
        locEtaPiMinus_GJ.Boost(boostGJ);
        locBeamP4GJ.Boost(boostGJ);
        locEtaP4_GJ.Boost(boostGJ);
        locPiminusP4_GJ.Boost(boostGJ);
        
        locEtaP4_RhoGJ.Boost(boostGJ_EtaRho);
        locRhoP4_RhoGJ.Boost(boostGJ_EtaRho);
        locBeamP4_RhoGJ.Boost(boostGJ_EtaRho);
        
        TVector3 z_GJ;
        z_GJ.SetXYZ(locBeamP4GJ.X(),locBeamP4GJ.Y(),locBeamP4GJ.Z());//z GJ
        TVector3 z_hat_GJ=z_GJ.Unit();
        TVector3 y_GJ=locBeamP4_CM.Vect().Cross(locEtaPiMinusP4_CM.Vect());
        TVector3 y_hat_GJ=y_GJ.Unit();
        TVector3 x_hat_GJ=y_hat_GJ.Cross(z_hat_GJ);//x hat GJ
        
        TVector3 v(locEtaP4_GJ.Vect()*x_hat_GJ, locEtaP4_GJ.Vect()*y_hat_GJ,
                   locEtaP4_GJ.Vect()*z_hat_GJ);
        double cosTheta = v.CosTheta();
        double theta = v.Theta();
        double phi = v.Phi()*180./TMath::Pi();
        
        double locCosThetaEtaGJ = (locBeamP4GJ.Vect().Unit()).Dot(locEtaP4_GJ.Vect().Unit());
        double locCosThetaPiMinusGJ = (locBeamP4GJ.Vect().Unit()).Dot(locPiminusP4_GJ.Vect().Unit());
        
        //RhoEta GJ
        TVector3 z_RhoGJ;
        z_RhoGJ.SetXYZ(locBeamP4_RhoGJ.X(),locBeamP4_RhoGJ.Y(),locBeamP4_RhoGJ.Z());//z GJ
        TVector3 z_hat_RhoGJ=z_GJ.Unit();
        TVector3 y_RhoGJ=locBeamP4_CM.Vect().Cross(locEtaPiPlusPiMinus_CM.Vect());
        TVector3 y_hat_RhoGJ=y_RhoGJ.Unit();
        TVector3 x_hat_RhoGJ=y_hat_RhoGJ.Cross(z_hat_RhoGJ);//x hat GJ
        
        TVector3 v_RhoGJ(locEtaP4_RhoGJ.Vect()*x_hat_RhoGJ, locEtaP4_RhoGJ.Vect()*y_hat_RhoGJ,
                         locEtaP4_RhoGJ.Vect()*z_hat_RhoGJ);
        double cosTheta_RhoGJ = v_RhoGJ.CosTheta();
        double theta_RhoGJ = v_RhoGJ.Theta();
        double phi_RhoGJ = v_RhoGJ.Phi()*180./TMath::Pi();
        
        double locCosThetaEta_RhoGJ = (locBeamP4_RhoGJ.Vect().Unit()).Dot(locEtaP4_RhoGJ.Vect().Unit());
        
        //VanHove
        TVector3 vHz = locBeamP4_CM.Vect().Unit();
        double q1 = locDeltaPPP4_CM.Vect().Dot(vHz);
        double q2 = locEtaP4_CM.Vect().Dot(vHz);
        double q3 = -q1-q2; //piminus
        
        double omega = TMath::ATan2(-1.*sqrt(3.)*q1, 2.*q2+q1);
        double r = q1 / TMath::Sin(omega);
        
        double vHx = r * TMath::Cos(omega);
        double vHy = q1;
        
        //Eta Rho VAn Hove
        
        //4d Van Hove
        TVector3 vHz_4d = locBeamP4_CM.Vect().Unit();
        //TVector3 vHz_4d = locEtaP4_CM.Vect().Unit();
        
        double q1_4d=locEtaP4_CM.Vect().Dot(vHz_4d);
        double q2_4d=locPiMinusP4_CM.Vect().Dot(vHz_4d);
        double q3_4d=locPiPlusP4_CM.Vect().Dot(vHz_4d);
        double q4_4d=-q1_4d-q2_4d-q3_4d;
        
        double x_4d=sqrt(3./8.)*(q3_4d-q2_4d);
        double y_4d=sqrt(1./8.)*(2.*q1_4d+3.*q2_4d+3.*q3_4d);
        double z_4d=q1_4d;
        
        double r_4d=sqrt(pow(x_4d,2)+pow(y_4d,2)+pow(z_4d,2));
        //double theta_4d=TMath::ASin(sqrt(1.-pow(z_4d/sqrt(pow(x_4d,2)+pow(y_4d,2)),2)));
        //inverse cosine calculation from the ATan2 expression, via trig identies
        double theta_4d = 2*(TMath::ATan2((TMath::Sqrt(1- ((z_4d/(TMath::Sqrt(x_4d*x_4d + y_4d*y_4d))) * (z_4d/(TMath::Sqrt(x_4d*x_4d + y_4d*y_4d)))))), (1 + (z_4d/(TMath::Sqrt(x_4d*x_4d + y_4d*y_4d))))));
        double phi_4d=TMath::ATan2(y_4d,x_4d) + TMath::Pi();
        
        dHist_VanHove4d_Actions->Fill(phi_4d,theta_4d);
        
        
        TVector3 locPhoton1P3=locPhoton1P4.Vect();
        TVector3 locPhoton2P3=locPhoton2P4.Vect();
        
        
        double ThetaPhoton1=locPhoton1P3.Theta()*180.0/TMath::Pi();
        double ThetaPhoton2=locPhoton2P3.Theta()*180.0/TMath::Pi();
        
        
        double ThetaMin=11.;
        double ThetaMax=12.5;
        
        
        if(ThetaPhoton1 < ThetaMax && ThetaPhoton1 > ThetaMin){
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        if ( ThetaPhoton2 < ThetaMax && ThetaPhoton2 > ThetaMin )
        {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        dHist_VanHove4d_Theta->Fill(phi_4d,theta_4d);
        
        double locRFTime = dComboWrapper->Get_RFTime();
        TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
        
        double locBeamDeltaT = locBeamX4.T() - (locRFTime + (locBeamX4.Z() - dTargetCenter.Z())/29.9792458);
        
        double tagWeight=0;
        if  (fabs(locBeamDeltaT) > 4.008) tagWeight=-1./8.;
        else if(fabs(locBeamDeltaT) < 2.004) tagWeight=1;
        
        TLorentzVector locProtonX4_Measured = dComboBeamWrapper->Get_X4_Measured();
        
        double locProtonX = locProtonX4_Measured.X();
        double locProtonY = locProtonX4_Measured.Y();
        double locProtonZ = locProtonX4_Measured.Z();
        
        double dMinProtonZ=51;
        double dMaxProtonZ=78;
        
        if ( locProtonX4_Measured.Z() < dMinProtonZ || locProtonX4_Measured.Z() > dMaxProtonZ )
        {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        if (sqrt(locProtonX*locProtonX+locProtonY*locProtonY) > 1.0){
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        
        
        double xm4s=0.496069;
        double xm8s=0.444706;
        double xp4s=0.598796;
        double xp8s=0.650159;
        double xp12s=0.701523;
        double xm12s=0.393343;
        
        
        double xm3s=0.441373;
        double xm6s=0.335223;
        double xp3s=0.653671;
        double xp6s=0.759821;
        double xp9s=0.86597;
        double xm9s=0.229074;
        
        double sidebandWeight=0;
        if (locGammaGammaMass>xm3s && locGammaGammaMass<xp3s) sidebandWeight=1;
        else if (locGammaGammaMass<=xm3s && locGammaGammaMass>xm6s) sidebandWeight=-1/2.;
        else if (locGammaGammaMass<=xp6s && locGammaGammaMass>=xp3s) sidebandWeight=-1/2.;
        
        
        map<Particle_t, set<Int_t> > locUsedThisCombo_Vertexing;
        locUsedThisCombo_Vertexing[Unknown].insert(locBeamID); //beam
        locUsedThisCombo_Vertexing[PiPlus].insert(locPiPlusTrackID);
        locUsedThisCombo_Vertexing[PiMinus].insert(locPiMinusTrackID);
        locUsedThisCombo_Vertexing[Proton].insert(locProtonTrackID);
        locUsedThisCombo_Vertexing[Gamma].insert(locPhoton1NeutralID);
        locUsedThisCombo_Vertexing[Gamma].insert(locPhoton2NeutralID);
        
        
        
        
        double SignalWeight[21][25];
        
        for (int i=0; i<21; i++) {
            for (int j=0; j<25; j++) {
                if (j<=5) {
                    SignalWeight[i][j]=1.;
                }
                else if(j<=10 && j>5){
                    SignalWeight[i][j]=0.5;
                    
                }
                else if(j<=15 && j>10){
                    SignalWeight[i][j]=0.25;
                    
                }
                else if(j<=20 && j>15){
                    SignalWeight[i][j]=0.1;
                    
                }
                else{
                    SignalWeight[i][j]=0.01;
                    
                }
            }
        }
        
        double MassBins[26]={1,1.04,1.08,1.12,1.16,1.2,1.24,1.28,1.32,1.36,1.4,1.44,1.48,1.52,1.56,1.6,1.64,1.68,1.72,1.76,1.8,1.84,1.88,1.92,1.96,2.0};
        
        double weight[21]={0,0,-0.616604,-0.573294,-0.484101,-0.472415,-0.443785,-0.423821,-0.436791,-0.486954,-0.492946,-0.520727,-0.505529,-0.532109,-0.539272,-0.519781,-0.54981,-0.580814,-0.525578,-0.566559,-1.3307};
        double weight_Delta[21];//={0,-2.31488,-0.616604,-0.573294,-0.484101,-0.472415,-0.443785,-0.423821,-0.436791,-0.486954,-0.492946,-0.520727,-0.505529,-0.532109,-0.539272,-0.519781,-0.54981,-0.580814,-0.525578,-0.566559,-1.3307};
        
        
        //compare to what's been used so far
        if(locUsedSoFar_Vertexing.find(locUsedThisCombo_Vertexing) == locUsedSoFar_Vertexing.end())
        {
            dHist_ShowerQuality->Fill(ShowerQuality1,tagWeight*sidebandWeight);
            dHist_ShowerQuality->Fill(ShowerQuality2,tagWeight*sidebandWeight);
            if (locPiPlusProtonMass<1.5) {
                dHist_Chi2->Fill(locKinFitChiSqPerNDF,tagWeight*sidebandWeight);
            }
            
            if (locKinFitChiSqPerNDF>16) {
                dComboWrapper->Set_IsComboCut(true);
                continue;
            }
            
            if(locPiPlusProtonMass>1.5){
                dHist_EtaPiPlusPiMinusMass->Fill(locEtaPiPlusPiMinusMass,tagWeight*sidebandWeight);
                
            }
            dHist_EtaMass->Fill(locGammaGammaMass, tagWeight);
            dHist_DeltaPPMass->Fill(locPiPlusProtonMass, tagWeight*sidebandWeight);
            dHist_PiPlusPiMinusMass->Fill(locPiPlusPiMinusMass, tagWeight*sidebandWeight);
            dHist_ProtonPiMinusMass->Fill(locPiMinusProtonMass, tagWeight*sidebandWeight);
            dHist_EtaPrimeMass->Fill(locEtaPiPlusPiMinusMass, tagWeight*sidebandWeight);
            dHist_EtaPiMinusMass->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
            dHist_EtaPiPlusMass->Fill(locEtaPiPlusMass, tagWeight*sidebandWeight);
            dHist_EtaProtonMass->Fill(locEtaProtonMass,tagWeight*sidebandWeight);
            
            dHist_PiPlusPiMinusMass_vs_DeltaPPMass->Fill(locPiPlusProtonMassSq,locPiPlusPiMinusMassSq, tagWeight*sidebandWeight);
            
            
            if(locPiPlusProtonMass>1.6){
                dHist_PiPlusPiMinusMass_CutDelta->Fill(locPiPlusPiMinusMass, tagWeight*sidebandWeight);
                
                if(locPiPlusPiMinusMass>0.6 && locPiPlusPiMinusMass<0.9){
                    
                    dHist_EtaPiMinusMass_RhoSelection_CutDelta->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection_CutDelta->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                    
                    dHist_EtaPiPlusPiMinusMass_RhoGJ_CutDelta->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosThetaEta_RhoGJ_CutDelta->Fill(locCosThetaEta_RhoGJ,sidebandWeight*tagWeight);
                    dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ_CutDelta->Fill(locEtaPiPlusPiMinusMass,locCosThetaEta_RhoGJ,sidebandWeight*tagWeight);
                    
                }
            }
            if (locEtaPiPlusPiMinusMass<1.05) {
                dComboWrapper->Set_IsComboCut(true);
                continue;
            }
            
            if(locPiPlusProtonMass>1.5){
                dHist_EtaPiMinusMass_DeltaPP[0]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[0]->Fill(locEtaPiMinusMass,cosTheta,sidebandWeight*tagWeight);
                
                if (locPiPlusProtonMass>1.5 && locPiPlusProtonMass<2.0) {
                    dHist_EtaPiMinusMass_DeltaPP[1]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[1]->Fill(locEtaPiMinusMass,cosTheta,sidebandWeight*tagWeight);
                    
                }
                else if (locPiPlusProtonMass>2.0 && locPiPlusProtonMass<2.5) {
                    dHist_EtaPiMinusMass_DeltaPP[2]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[2]->Fill(locEtaPiMinusMass,cosTheta,sidebandWeight*tagWeight);
                    
                }
                else if (locPiPlusProtonMass>2.5 && locPiPlusProtonMass<3.0) {
                    dHist_EtaPiMinusMass_DeltaPP[3]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[3]->Fill(locEtaPiMinusMass,cosTheta,sidebandWeight*tagWeight);
                    
                }
                else if (locPiPlusProtonMass>3.0) {
                    dHist_EtaPiMinusMass_DeltaPP[4]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[4]->Fill(locEtaPiMinusMass,cosTheta,sidebandWeight*tagWeight);
                    
                }
                
            }
            if(q2>0 && q3>0){
                dHist_DeltaPPMass_vH->Fill(locPiPlusProtonMass,sidebandWeight*tagWeight);
            }
            
            dHist_ProtonPiMinusMass_vs_DeltaPPMass->Fill(locPiPlusProtonMassSq, locPiMinusProtonMassSq,tagWeight*sidebandWeight);
            
            if (locPiPlusProtonMass>2.0) {
                dComboWrapper->Set_IsComboCut(true);
                continue;
            }
            
            for (int i=0; i<21; i++) {
                if(fabs(locMandlestam_t)>=0.0+(i/10.) && fabs(locMandlestam_t)<0.1+(i/10.) && i<20){
                    dHist_DeltaPPMass_tBins[i]->Fill(locPiPlusProtonMass,sidebandWeight*tagWeight);
                    
                    if(locPiPlusProtonMass>1.102 && locPiPlusProtonMass<1.4){
                        weight_Delta[i]=1.;
                    }
                    else if(locPiPlusProtonMass>1.6 && locPiPlusProtonMass<1.898){
                        weight_Delta[i]=weight[i];
                    }
                    else weight_Delta[i]=0;
                    
                    dHist_EtaPiMinusMass_tBins_DeltaSub[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    dHist_PiPlusPiMinusMass_tBins_DeltaSub[i]->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    dHist_EtaPiPlusMass_tBins_DeltaSub[i]->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    dHist_ProtonPiMinusMass_tBins_DeltaSub[i]->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    dHist_EtaPrimeMass_tBins_DeltaSub[i]->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    
                    if (locPiPlusProtonMass>1.6 && locPiPlusProtonMass<1.898) {
                        dHist_EtaPiMinusMass_tBins_back[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        
                        dHist_PiPlusPiMinusMass_tBins_back[i]->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight);
                        dHist_EtaPiPlusMass_tBins_back[i]->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight);
                        dHist_ProtonPiMinusMass_tBins_back[i]->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight);
                        dHist_EtaPrimeMass_tBins_back[i]->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                    }
                    for (int j=0; j<25; j++) {
                        if(locPiPlusProtonMass>1.0+(j*0.04) && locPiPlusProtonMass<=1.04+(j*0.04)){
                            dHist_EtaPiMinusMass_tBins_ProbWeight[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight*SignalWeight[i][j]);
                        }
                    }
                    
                    
                }
                else if(i==20 && fabs(locMandlestam_t)>=2.0){
                    dHist_DeltaPPMass_tBins[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    
                    if(locPiPlusProtonMass>1.102 && locPiPlusProtonMass<1.4){
                        weight_Delta[i]=1.;
                    }
                    else if(locPiPlusProtonMass>1.6 && locPiPlusProtonMass<1.6+0.298){
                        weight_Delta[i]=weight[i];
                    }
                    else weight_Delta[i]=0;
                    dHist_EtaPiMinusMass_tBins_DeltaSub[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight*weight_Delta[i]);
                    
                    if (locPiPlusProtonMass>1.6 && locPiPlusProtonMass<1.898) {
                        dHist_EtaPiMinusMass_tBins_back[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        
                        dHist_PiPlusPiMinusMass_tBins_back[i]->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight);
                        dHist_EtaPiPlusMass_tBins_back[i]->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight);
                        dHist_ProtonPiMinusMass_tBins_back[i]->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight);
                        dHist_EtaPrimeMass_tBins_back[i]->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                        
                        
                    }
                    
                    for (int j=0; j<25; j++) {
                        if(locPiPlusProtonMass>1.0+(j*0.04) && locPiPlusProtonMass<=1.04+(j*0.04)){
                            dHist_EtaPiMinusMass_tBins_ProbWeight[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight*SignalWeight[i][j]);
                        }
                    }
                }
            }
            
            
            
            if (locPiPlusProtonMass>1.4) {
                dComboWrapper->Set_IsComboCut(true);
                continue;
            }
            
            dHist_VanHove4d->Fill(phi_4d,theta_4d,tagWeight*sidebandWeight);
            
            for (int i=0; i<8; i++) {
                if (locKinFitChiSqPerNDF<(i+1)*2) {
                    dHist_EtaPiMinusMass_Chi2_bins[i]->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_Chi2_bins[i]->Fill(locEtaPiMinusMass, cosTheta, tagWeight*sidebandWeight);
                }
                
            }
            
            map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMassb;
            locUsedThisCombo_MissingMassb[Unknown].insert(locBeamID); //beam
            locUsedThisCombo_MissingMassb[PiPlus].insert(locPiPlusTrackID);
            locUsedThisCombo_MissingMassb[PiMinus].insert(locPiMinusTrackID);
            locUsedThisCombo_MissingMassb[Proton].insert(locProtonTrackID);
            locUsedThisCombo_MissingMassb[Gamma].insert(locPhoton1NeutralID);
            locUsedThisCombo_MissingMassb[Gamma].insert(locPhoton2NeutralID);
            
            
            //compare to what's been used so far
            if(locUsedSoFar_MissingMassb.find(locUsedThisCombo_MissingMassb) == locUsedSoFar_MissingMassb.end())
            {
                //unique missing mass combo: histogram it, and register this combo of particles
                dHist_MissingMassSquared_allcuts->Fill(locMissingMassSquared, sidebandWeight*tagWeight);
                locUsedSoFar_MissingMassb.insert(locUsedThisCombo_MissingMassb);
            }
            
            dHist_EtaMassPostDeltaPP->Fill(locGammaGammaMass, tagWeight);
            dHist_PiPlusPiMinusMassPostDeltaPP->Fill(locPiPlusPiMinusMass, tagWeight*sidebandWeight);
            dHist_ProtonPiMinusMassPostDeltaPP->Fill(locPiMinusProtonMass, tagWeight*sidebandWeight);
            dHist_EtaPrimeMassPostDeltaPP->Fill(locEtaPiPlusPiMinusMass, tagWeight*sidebandWeight);
            dHist_EtaPiMinusMassPostDeltaPP->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
            dHist_ShowerQualityPostCuts->Fill(ShowerQuality1,tagWeight*sidebandWeight);
            dHist_ShowerQualityPostCuts->Fill(ShowerQuality2,tagWeight*sidebandWeight);
            
            if(ShowerQuality1>0.5 && ShowerQuality2>0.5){
                dHist_EtaPiMinusMassShowerCut->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
            }
            dHist_EtaPiPlusMassPostDeltaPP->Fill(locEtaPiPlusMass, tagWeight*sidebandWeight);
            dHist_EtaProtonMassPostDeltaPP->Fill(locEtaProtonMass,tagWeight*sidebandWeight);
            
            if (-locMandlestam_t>0.1 && -locMandlestam_t<0.6) {
                dHist_EtaPiMinusMassPostDeltaPP_lowt->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
                
                if(ShowerQuality1>0.5 && ShowerQuality2>0.5){
                    dHist_EtaPiMinusMassShowerCut_lowt->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
                }
            }
            
            dHist_EtaPiMinusMass_vs_EtaPiPlusMass->Fill(locEtaPiPlusMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
            dHist_EtaPiMinusMass_vs_ProtonPiMinusMass->Fill(locPiMinusProtonMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
            dHist_EtaPiPlusMass_vs_ProtonPiPlusMass->Fill(locPiPlusProtonMassSq,locEtaPiPlusMassSq,tagWeight*sidebandWeight);
            dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass->Fill(locPiPlusPiMinusMassSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
            dHist_EtaPiPlususMass_vs_PiPlusPiMinusMass->Fill(locPiPlusPiMinusMassSq,locEtaPiPlusMassSq,tagWeight*sidebandWeight);
            dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass->Fill(locEtaPiPlusPiMinusMasSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
            
            dHist_CosTheta_vs_EtaPiMinusMass->Fill(locEtaPiMinusMass, cosTheta, tagWeight*sidebandWeight);
            if (-locMandlestam_t>0.1 && -locMandlestam_t<0.6) {
                dHist_CosTheta_vs_EtaPiMinusMass_lowt->Fill(locEtaPiMinusMass, cosTheta, tagWeight*sidebandWeight);
                
                dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt->Fill(locEtaPiPlusMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
                dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt->Fill(locPiMinusProtonMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
                dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt->Fill(locPiPlusPiMinusMassSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
                dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt->Fill(locEtaPiPlusPiMinusMasSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
                
                if(q2>0 && q3>0){
                    dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt_vH->Fill(locEtaPiPlusMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
                    dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt_vH->Fill(locPiMinusProtonMassSq,locEtaPiMinusMassSq, tagWeight*sidebandWeight);
                    dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt_vH->Fill(locPiPlusPiMinusMassSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
                    dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt_vH->Fill(locEtaPiPlusPiMinusMasSq,locEtaPiMinusMassSq,tagWeight*sidebandWeight);
                }
                
            }
            //unique missing mass combo: histogram it, and register this combo of particles
            dHist_ProtonZ->Fill(locProtonX4_Measured.Z(), sidebandWeight*tagWeight);
            dHist_ProtonR->Fill(locProtonX,locProtonY, sidebandWeight*tagWeight);
            
            dHist_Mandlestam_t->Fill(-locMandlestam_t, sidebandWeight*tagWeight);
            
            for (int i=0; i<21; i++) {
                if(fabs(locMandlestam_t)>=0.1+(i/10.) && fabs(locMandlestam_t)<0.2+(i/10.) && i<20){
                    dHist_EtaPiMinusMass_tBins[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                    
                    dHist_PiPlusPiMinusMass_tBins_sig[i]->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight);
                    dHist_EtaPiPlusMass_tBins_sig[i]->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight);
                    dHist_ProtonPiMinusMass_tBins_sig[i]->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight);
                    dHist_EtaPrimeMass_tBins_sig[i]->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                }
                else if(i==20 && fabs(locMandlestam_t)>=2.0){
                    dHist_EtaPiMinusMass_tBins[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                    dHist_PiPlusPiMinusMass_tBins_sig[i]->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight);
                    dHist_EtaPiPlusMass_tBins_sig[i]->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight);
                    dHist_ProtonPiMinusMass_tBins_sig[i]->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight);
                    dHist_EtaPrimeMass_tBins_sig[i]->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                    
                }
            }
            
            dHist_VanHove->Fill(vHx,vHy,sidebandWeight*tagWeight);
            
            if(q1>0) dHist_VanHove_positiveq1->Fill(vHx,vHy,sidebandWeight*tagWeight);
            else if(q2>0) dHist_VanHove_positiveq2->Fill(vHx,vHy,sidebandWeight*tagWeight);
            else if (q3>0) dHist_VanHove_positiveq3->Fill(vHx,vHy,sidebandWeight*tagWeight);
            
            if(q2>0 && q3>0){
                dHist_VanHove_positiveq23->Fill(vHx,vHy,sidebandWeight*tagWeight);
                dHist_EtaPiMinusMass_vH->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                dHist_EtaPiPlusMass_vH->Fill(locEtaPiPlusMass,sidebandWeight*tagWeight);
                dHist_PiPlusPiMinusMass_vH->Fill(locPiPlusPiMinusMass,sidebandWeight*tagWeight);
                dHist_ProtonPiMinusMass_vH->Fill(locPiMinusProtonMass,sidebandWeight*tagWeight);
                dHist_EtaPrimeMass_vH->Fill(locEtaPiPlusPiMinusMass,sidebandWeight*tagWeight);
                dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_vH->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                
                if (-locMandlestam_t>0.1 && -locMandlestam_t<0.6) {
                    dHist_EtaPiMinusMassPostDeltaPP_lowt_vH->Fill(locEtaPiMinusMass, tagWeight*sidebandWeight);
                    dHist_CosTheta_vs_EtaPiMinusMass_lowt_vH->Fill(locEtaPiMinusMass, cosTheta, tagWeight*sidebandWeight);
                    
                }
                
                for (int i=0; i<20; i++) {
                    if(fabs(locMandlestam_t)>=0.1+(i/10.) && fabs(locMandlestam_t)<0.2+(i/10.) && i<19){
                        dHist_EtaPiMinusMass_tBins_vH[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_vH[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                    }
                    else if(i==19 && fabs(locMandlestam_t)>=0.2+(i/10.)){
                        dHist_EtaPiMinusMass_tBins_vH[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_vH[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                    }
                }
            }
            
            if(q1_4d>0 && q2_4d>0 && q3_4d<0 && q4_4d<0){
                dHist_EtaPiMinusMass_VanHove4d->Fill(locEtaPiMinusMass,tagWeight*sidebandWeight);
                dHist_EtaPiPlusMass_VanHove4d->Fill(locEtaPiPlusMass,tagWeight*sidebandWeight);
                dHist_EtaPiPlusPiMinusMass_VanHove4d->Fill(locEtaPiPlusPiMinusMass,tagWeight*sidebandWeight);
                dHist_PiPlusPiMinusMass_VanHove4d->Fill(locPiPlusPiMinusMass,tagWeight*sidebandWeight);
                dHist_ProtonPiMinusMass_VanHove4d->Fill(locPiMinusProtonMass,tagWeight*sidebandWeight);
                dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_VanHove4d->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,tagWeight*sidebandWeight);
                dHist_VanHove4d_q1q2cut->Fill(phi_4d,theta_4d,tagWeight*sidebandWeight);
                
                for (int i=0; i<20; i++) {
                    if(fabs(locMandlestam_t)>=0.1+(i/10.) && fabs(locMandlestam_t)<0.2+(i/10.) && i<19){
                        dHist_EtaPiMinusMass_tBins_VanHove_4d[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_VanHove_4d[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                        dHist_EtaPiPlusMass_tBins_VanHove_4d[i]->Fill(locEtaPiPlusMass,tagWeight*sidebandWeight);
                        dHist_PiPlusPiMinusMass_tBins_VanHove_4d[i]->Fill(locPiPlusPiMinusMass,tagWeight*sidebandWeight);
                        dHist_ProtonPiMinusMass_tBins_VanHove_4d[i]->Fill(locPiMinusProtonMass,tagWeight*sidebandWeight);
                        
                        
                    }
                    else if(i==19 && fabs(locMandlestam_t)>=0.2+(i/10.)){
                        dHist_EtaPiMinusMass_tBins_VanHove_4d[i]->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                        dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_VanHove_4d[i]->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                        dHist_EtaPiPlusMass_tBins_VanHove_4d[i]->Fill(locEtaPiPlusMass,tagWeight*sidebandWeight);
                        dHist_PiPlusPiMinusMass_tBins_VanHove_4d[i]->Fill(locPiPlusPiMinusMass,tagWeight*sidebandWeight);
                        dHist_ProtonPiMinusMass_tBins_VanHove_4d[i]->Fill(locPiMinusProtonMass,tagWeight*sidebandWeight);
                    }
                }
                
            }
            
            //select rho
            if(locPiPlusPiMinusMass>0.6 && locPiPlusPiMinusMass<0.9){
                dHist_EtaPiPlusPiMinusMass_RhoGJ->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                dHist_EtaPiMinusMass_RhoSelection->Fill(locEtaPiMinusMass,sidebandWeight*tagWeight);
                dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection->Fill(locEtaPiMinusMass,locCosThetaEtaGJ,sidebandWeight*tagWeight);
                
                dHist_CosThetaEta_RhoGJ->Fill(locCosThetaEta_RhoGJ,sidebandWeight*tagWeight);
                dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ->Fill(locEtaPiPlusPiMinusMass,locCosThetaEta_RhoGJ,sidebandWeight*tagWeight);
                
            }
            
            locUsedSoFar_Vertexing.insert(locUsedThisCombo_Vertexing);
        }
        
        if (locKinFitChiSqPerNDF>20) {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        if (locPiPlusProtonMass>2.5) {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        
        dHist_VanHove4d->Fill(phi_4d,theta_4d,tagWeight*sidebandWeight);
        
        Double_t locVHq1_Flat=q1;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHq1", locVHq1_Flat);
        Double_t locVHq2_Flat=q2;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHq2", locVHq2_Flat);
        Double_t locVHq3_Flat=q3;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHq3", locVHq3_Flat);
        Double_t locvHx_Flat=vHx;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHx", locvHx_Flat);
        Double_t locvHy_Flat=vHy;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHy", locvHy_Flat);
        Double_t locvHr_Flat=r;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHr", locvHr_Flat);
        Double_t locvHomega_Flat=omega;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("vHomega", locvHomega_Flat);
        
        Double_t loc_q1_4d_Flat=q1_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("q1_4d", loc_q1_4d_Flat);
        Double_t loc_q2_4d_Flat=q2_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("q2_4d", loc_q2_4d_Flat);
        Double_t loc_q3_4d_Flat=q3_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("q3_4d", loc_q3_4d_Flat);
        Double_t loc_q4_4d_Flat=q4_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("q4_4d", loc_q4_4d_Flat);
        
        Double_t loc_theta_4d_Flat=theta_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("theta_4d", loc_theta_4d_Flat);
        Double_t loc_phi_4d_Flat=phi_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("phi_4d", loc_phi_4d_Flat);
        Double_t loc_r_4d_Flat=r_4d;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("r_4d", loc_r_4d_Flat);
        
        Double_t locMissingMassSquared_Flat=locMissingMassSquared;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("MissingMassSquared", locMissingMassSquared_Flat);
        Double_t locKinFitChiSqPerNDF_Flat=locKinFitChiSqPerNDF;
        dFlatTreeInterface->Fill_Fundamental<Double_t>("KinFitChiSqPerNDF", locKinFitChiSqPerNDF_Flat);
        Double_t locTarget_px_Flat = dTargetP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_px", locTarget_px_Flat);
        Double_t locTarget_py_Flat = dTargetP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_py", locTarget_py_Flat);
        Double_t locTarget_pz_Flat = dTargetP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_pz", locTarget_pz_Flat);
        Double_t locTarget_E_Flat = dTargetP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_E", locTarget_E_Flat);
        
        Double_t locBeam_px_Flat = locBeamP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_px", locBeam_px_Flat);
        Double_t locBeam_py_Flat = locBeamP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_py", locBeam_py_Flat);
        Double_t locBeam_pz_Flat = locBeamP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_pz", locBeam_pz_Flat);
        Double_t locBeam_E_Flat = locBeamP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_E", locBeam_E_Flat);
        
        Double_t locPiPlus_px_Flat = locPiPlusP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_px", locPiPlus_px_Flat);
        Double_t locPiPlus_py_Flat = locPiPlusP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_py", locPiPlus_py_Flat);
        Double_t locPiPlus_pz_Flat = locPiPlusP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_pz", locPiPlus_pz_Flat);
        Double_t locPiPlus_E_Flat = locPiPlusP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_E", locPiPlus_E_Flat);
        
        Double_t locPiMinus_px_Flat = locPiMinusP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_px", locPiMinus_px_Flat);
        Double_t locPiMinus_py_Flat = locPiMinusP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_py", locPiMinus_py_Flat);
        Double_t locPiMinus_pz_Flat = locPiMinusP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_pz", locPiMinus_pz_Flat);
        Double_t locPiMinus_E_Flat = locPiMinusP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_E", locPiMinus_E_Flat);
        
        Double_t locProton_px_Flat = locProtonP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_px", locProton_px_Flat);
        Double_t locProton_py_Flat = locProtonP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_py", locProton_py_Flat);
        Double_t locProton_pz_Flat = locProtonP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_pz", locProton_pz_Flat);
        Double_t locProton_E_Flat = locProtonP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_E", locProton_E_Flat);
        
        Double_t locEta_px_Flat = locEtaP4.Px();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Eta_px", locEta_px_Flat);
        Double_t locEta_py_Flat = locEtaP4.Py();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Eta_py", locEta_py_Flat);
        Double_t locEta_pz_Flat = locEtaP4.Pz();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Eta_pz", locEta_pz_Flat);
        Double_t locEta_E_Flat = locEtaP4.E();
        dFlatTreeInterface->Fill_Fundamental<Double_t>("Eta_E", locEta_E_Flat);
        
        Float_t locEtaPiMinusMass_Flat = locEtaPiMinusMass;
        dFlatTreeInterface->Fill_Fundamental<Float_t>("EtaPiMinusMass", locEtaPiMinusMass_Flat);
        Float_t locPiPlusProtonMass_Flat = locPiPlusProtonMass;
        dFlatTreeInterface->Fill_Fundamental<Float_t>("DeltaPPMass", locPiPlusProtonMass_Flat);
        Float_t locMandlestam_t_Flat = locMandlestam_t;
        dFlatTreeInterface->Fill_Fundamental<Float_t>("Mandlestam_t", locMandlestam_t_Flat);
        
        Float_t locsidebandWeight_Flat = sidebandWeight;
        dFlatTreeInterface->Fill_Fundamental<Float_t>("sidebandWeight", locsidebandWeight_Flat);
        Float_t tagWeight_Flat = tagWeight;
        dFlatTreeInterface->Fill_Fundamental<Float_t>("tagWeight", tagWeight_Flat);
        
        //FILL FLAT TREE
        Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
/*
	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();
*/

	return kTRUE;
}

void DSelector_pippimeta__B4_M17_ver25::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
