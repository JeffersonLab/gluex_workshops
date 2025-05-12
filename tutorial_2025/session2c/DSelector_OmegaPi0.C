#include "DSelector_OmegaPi0.h"

void DSelector_OmegaPi0::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
        dOutputFileName = "OmegaPi0.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "OmegaPi0_tree.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kin"; //if blank, default name will be chosen
	dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

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
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, UnknownParticle: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	// dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	// dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	// dHist_BeamEnergy_BestChiSq = new TH1I("BeamEnergy_BestChiSq", ";Beam Energy (GeV)", 600, 0.0, 12.0);

	// Added Histograms
	// create directory and cd to it 
	gDirectory->mkdir( "Beam_and_MMS" )->cd();
	dHist_MissingMassSquared = new TH1D( "MissingMassSquared", ";Missing Mass Squared (GeV)^{2}; Combos / 2 MeV^{2}", 1000, -.1,.1);
	dHist_MissingEnergy = new TH1D( "MissingE", ";Missing Energy (GeV); Combos / 2 MeV", 3000, -3,3 );
	dHist_BeamEnergy = new TH1D( "BeamEnergy", ";Beam Energy (GeV); Combos / 20 MeV", 300, 6.0, 12.0);
	dHist_BeamTiming = new TH1D ("BeamTiming", ";Beam t_{Tagger}-t_{RF} (ns); Combos / 0.2 ns", 200, -20, 20 );
	// exit the directory
        gDirectory->cd( ".." );

	gDirectory->mkdir( "CL_chiSq" )->cd();
	dHist_ChiSq = new TH1D( "ChiSq", ";#chi^{2}/ndfd; Combos / 10", 100, 0, 1000 );
	dHist_ChiSq_z = new TH1D( "ChiSq_z", ";#chi^{2}/ndf; Combos / 1", 100, 0, 100 );		
       	int locNumBins = 0;
	double* locConLevLogBinning = dAnalysisUtilities.Generate_LogBinning(-50, 0, 5, locNumBins); 
	if(locConLevLogBinning != NULL){
	  dHist_ConfidenceLevel_logX = new TH1D( "CL", ";Confidence Level", locNumBins, locConLevLogBinning );	
	  dHist_ChiSqVsConfidenceLevel_logX = new TH2D( "ChiSqVsCL_logX", ";Confidence Level; #chi^{2}", locNumBins, locConLevLogBinning, 100, 0, 1000 );
	}
	else{
	  dHist_ConfidenceLevel_logX = NULL;
	  dHist_ChiSqVsConfidenceLevel_logX = NULL;
	}
	gDirectory->cd( "..");

	gDirectory->mkdir( "PID_Proton" )->cd();
	gDirectory->mkdir( "Delta" )->cd();
	//DeltaT vs P
	dHist_DeltaTVsP_Proton_BCAL = new TH2D( "DeltaTVsP_Proton_BCAL", ";Momentum Proton (GeV); #DeltaT_{Measured - RF} BCAL (ns)", 250, 0, 10, 50 , -1, 1 );
	dHist_DeltaTVsP_Proton_TOF = new TH2D( "DeltaTVsP_Proton_TOF", ";Momentum Proton (GeV); #DeltaT_{Measured - RF} TOF (ns)", 250, 0, 10, 25, -.5, .5 );
	gDirectory->cd( ".." );
	gDirectory->mkdir( "dEdx" )->cd();
	//dEdx Plots
	dHist_dEdxVsP_Proton_CDC = new TH2D( "dEdxvsP_Proton_CDC", ";Momentum Proton (GeV); CDC dE/dx", 250, 0, 10, 250, 0, 25 );
	gDirectory->cd( ".." );
	gDirectory->mkdir( "Vertex" )->cd();
	dHist_VertexProtonZ = new TH1D( "VertexProtonZ_M", ";Z Vertex Proton (cm); Combos / .05 cm", 1000, 45, 95 );
	dHist_VertexProtonXY = new TH2D( "VertexProtonXY_M", ";X Vertex Proton (cm); Y Vertex Proton (cm)", 160, -4, 4, 160, -4, 4 );
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "PID_PiP" )->cd();
	gDirectory->mkdir( "Delta" )->cd();
	//DeltaT vs P
	dHist_DeltaTVsP_PiP_BCAL = new TH2D( "DeltaTVsP_PiP_BCAL", ";Momentum #pi^{+} (GeV); #DeltaT_{Measured - RF} BCAL (ns)", 250, 0, 10, 50 , -1, 1 );
	dHist_DeltaTVsP_PiP_TOF = new TH2D( "DeltaTVsP_PiP_TOF", ";Momentum #pi^{+} (GeV); #DeltaT_{Measured - RF} TOF (ns)", 250, 0, 10, 25, -.5, .5 );
	gDirectory->cd( ".." );
	gDirectory->mkdir( "dEdx" )->cd();
	//dEdx Plots
	dHist_dEdxVsP_PiP_CDC = new TH2D( "dEdxvsP_PiP_CDC", ";Momentum #pi^{+} (GeV); CDC dE/dx", 250, 0, 10, 250, 0, 25);
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "PID_PiM" )->cd();
	gDirectory->mkdir( "Delta" )->cd();
	//DeltaT vs P
	dHist_DeltaTVsP_PiM_BCAL = new TH2D( "DeltaTVsP_PiM_BCAL", ";Momentum #pi^{-} (GeV); #DeltaT_{Measured - RF} BCAL (ns)", 250, 0, 10, 50, -1, 1 );
	dHist_DeltaTVsP_PiM_TOF = new TH2D( "DeltaTVsP_PiM_TOF", ";Momentum #pi^{-} (GeV); #DeltaT_{Measured - RF} TOF (ns)", 250, 0, 10, 25, -.5, .5 );
	gDirectory->cd( ".." );
	gDirectory->mkdir( "dEdx" )->cd();
	//dEdx Plots
	dHist_dEdxVsP_PiM_CDC = new TH2D( "dEdxvsP_PiM_CDC", ";Momentum #pi^{-} (GeV); CDC dE/dx", 250, 0, 10, 250, 0, 25);
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );
	
	gDirectory->mkdir( "PID_Photon" )->cd();
	gDirectory->mkdir( "Delta" )->cd();
	//Delta T vs P
	dHist_DeltaTVsP_Photon_FCAL = new TH2D( "DeltaTVsP_Photon_FCAL", ";Momentum Photon (GeV); #DeltaT_{Measured - RF} FCAL (ns)", 250, 0, 10, 100, -2, 2 );
	dHist_DeltaTVsP_Photon_BCAL = new TH2D( "DeltaTVsP_Photon_BCAL", ";Momentum Photon (GeV); #DeltaT_{Measured - RF} BCAL (ns)", 250, 0, 10, 50, -1, 1 );
	gDirectory->cd( ".." );
	gDirectory->mkdir( "Showers" )->cd();
	dHist_ShowerQuality1 = new TH1D( "ShowerQuality1", ";#gamma_{1} Shower Quality; Combos / .01", 100, 0, 1 );
	dHist_ShowerQuality2 = new TH1D( "ShowerQuality2", ";#gamma_{2} Shower Quality; Combos / .01", 100, 0, 1 );
	dHist_ShowerQuality3 = new TH1D( "ShowerQuality3", ";#gamma_{3} Shower Quality; Combos / .01", 100, 0, 1 );
	dHist_ShowerQuality4 = new TH1D( "ShowerQuality4", ";#gamma_{4} Shower Quality; Combos / .01", 100, 0, 1 );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "Unused_Showers" )->cd();
	dHist_UnusedShowerEnergy = new TH1D( "UnusedShowerEnergy", ";Unused Shower Energy (GeV); Combos / 10 MeV", 500, 0 , 5 );
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );
	
	gDirectory->mkdir( "Photon_Mass" )->cd();
	gDirectory->mkdir( "Measured" )->cd();
	dHist_Mass_g1_g2_M = new TH1D( "Mass_g1_g2_M", ";Mass[#gamma_{1}#gamma_{2}] (GeV); Combos / 1.1 MeV", 100, 0.08, .19 );
	dHist_Mass_g1_g3_M = new TH1D( "Mass_g1_g3_M", ";Mass[#gamma_{1}#gamma_{3}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g1_g4_M = new TH1D( "Mass_g1_g4_M", ";Mass[#gamma_{1}#gamma_{4}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g2_g3_M = new TH1D( "Mass_g2_g3_M", ";Mass[#gamma_{2}#gamma_{3}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g2_g4_M = new TH1D( "Mass_g2_g4_M", ";Mass[#gamma_{2}#gamma_{4}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g3_g4_M = new TH1D( "Mass_g3_g4_M", ";Mass[#gamma_{3}#gamma_{4}] (GeV); Combos / 1.1/ MeV", 100, 0.08, .19 );
	gDirectory->cd( ".." );	
	gDirectory->mkdir( "KinFit" )->cd();
	dHist_Mass_g1_g2 = new TH1D( "Mass_g1_g2", ";Mass[#gamma_{1}#gamma_{2}] (GeV); Combos / 1 MeV", 200, 0.05, .25 );
	dHist_Mass_g1_g3 = new TH1D( "Mass_g1_g3", ";Mass[#gamma_{1}#gamma_{3}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g1_g4 = new TH1D( "Mass_g1_g4", ";Mass[#gamma_{1}#gamma_{4}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g2_g3 = new TH1D( "Mass_g2_g3", ";Mass[#gamma_{2}#gamma_{3}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g2_g4 = new TH1D( "Mass_g2_g4", ";Mass[#gamma_{2}#gamma_{4}] (GeV); Combos / 5 MeV", 200, 0.0, 1.0 );
	dHist_Mass_g3_g4 = new TH1D( "Mass_g3_g4", ";Mass[#gamma_{3}#gamma_{4}] (GeV); Combos / 1 MeV", 200, 0.05, .25 );
	gDirectory->cd( ".." );	
	gDirectory->mkdir( "3Photon" )->cd();
	dHist_Mass_g1_g2_g3 = new TH1D( "Mass_g1_g2_g3", ";Mass[#gamma_{1}#gamma_{2}#gamma_{3}] (GeV); Combos / 5 MeV", 500, 0.0, 2.5 );
	dHist_Mass_g1_g2_g4 = new TH1D( "Mass_g1_g2_g4", ";Mass[#gamma_{1}#gamma_{2}#gamma_{4}] (GeV); Combos / 5 MeV", 500, 0.0, 2.5 );
	dHist_Mass_g1_g3_g4 = new TH1D( "Mass_g1_g3_g4", ";Mass[#gamma_{1}#gamma_{3}#gamma_{4}] (GeV); Combos / 5 MeV", 500, 0.0, 2.5 );
	dHist_Mass_g2_g3_g4 = new TH1D( "Mass_g2_g3_g4", ";Mass[#gamma_{2}#gamma_{3}#gamma_{4}] (GeV); Combos / 5 MeV", 500, 0.0, 2.5);
	gDirectory->cd( ".." );	
	gDirectory->mkdir( "4Photon" )->cd();
	dHist_Mass_g1_g2_g3_g4 = new TH1D( "Mass_g1_g2_g3_g4", ";Mass[#gamma_{1}#gamma_{2}#gamma_{3}#gamma_{4}] (GeV); Combos / 5 MeV", 500, 0., 2.5 );
	gDirectory->cd( ".." );	
	gDirectory->mkdir( "2D_Plots_Measured" )->cd();
	dHist_g1g2vsg3g4_M = new TH2D( "g1g2vsg3g4_M", ";Mass[#gamma_{3}#gamma_{4}] (GeV); Mass[#gamma_{1}#gamma_{2}] (GeV)", 100, 0.08, .19, 100, 0.08, .19 );
        dHist_g1g3vsg2g4_M = new TH2D( "g1g3vsg2g4_M", ";Mass[#gamma_{2}#gamma_{4}] (GeV); Mass[#gamma_{1}#gamma_{3}] (GeV)", 200, 0, 1., 200, 0, 1.);
        dHist_g1g4vsg2g3_M = new TH2D( "g1g4vsg2g3_M", ";Mass[#gamma_{2}#gamma_{3}] (GeV); Mass[#gamma_{1}#gamma_{4}] (GeV)", 200, 0, 1., 200, 0, 1.);
	gDirectory->cd( ".." );	
	gDirectory->mkdir( "2D_Plots_KinFit" )->cd();
	dHist_g1g2vsg3g4 = new TH2D( "g1g2vsg3g4", ";Mass[#gamma_{3}#gamma_{4}] (GeV); Mass[#gamma_{1}#gamma_{2}] (GeV)", 100, 0.08, .19, 100, 0.08, .19 );
        dHist_g1g3vsg2g4 = new TH2D( "g1g3vsg2g4", ";Mass[#gamma_{2}#gamma_{4}] (GeV); Mass[#gamma_{1}#gamma_{3}] (GeV)", 200, 0, 1., 200, 0, 1.);
        dHist_g1g4vsg2g3 = new TH2D( "g1g4vsg2g3", ";Mass[#gamma_{2}#gamma_{3}] (GeV); Mass[#gamma_{1}#gamma_{4}] (GeV)",  200, 0, 1., 200, 0, 1.);
	gDirectory->cd( ".." );	
	gDirectory->cd( ".." );

	gDirectory->mkdir( "3particles_Mass" )->cd();
	dHist_Mass3pi1 = new TH1D( "Mass3pi1", ";Mass[3#pi_{1}] (GeV); Combos / 3 MeV ", 1000, 0.3, 3.3 );
	dHist_Mass3pi2 = new TH1D( "Mass3pi2", ";Mass[3#pi_{2}] (GeV); Combos / 3 MeV ", 1000, 0.5, 3.5 );
	dHist_Mass3pi2vs3pi1 = new TH2D( "Mass3pi2vs3pi1", ";Mass[3#pi_{1}] (GeV);Mass[3#pi_{2}] (GeV)", 1000, 0.5, 3.5, 1000, 0.5, 3.5 );
	gDirectory->mkdir( "omega_selected" )->cd();
	dHist_3piMassCorr = new TH2D("3piPiMassCorr", ";Mass[3#pi] (GeV); ;Mass[3#pi_{alt}] (GeV)", 100, 0.5, 1.5, 100, 0.5, 1.5);
	dHist_3piMassCorr2DWeight = new TH2D("3piPiMassCorr2DWeight", ";Mass[3#pi] (GeV); ;Mass[3#pi_{alt}] (GeV)", 100, 0.5, 1.5, 100, 0.5, 1.5);
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "Baryons" )->cd();
	dHist_MassPipP = new TH1D( "MassPipP", "; Mass[#pi^{+}p] (GeV); Combos / 5 MeV", 500, 1, 3.5 );
	dHist_MassPimP = new TH1D( "MassPimP", "; Mass[#pi^{-}p] (GeV); Combos / 5 MeV", 500, 1, 3.5 );
	dHist_MassPi01P = new TH1D( "MassPi01P", "; Mass[#pi^{0}_{1}p] (GeV); Combos / 5 MeV", 500, 1, 3.5 );
	dHist_MassPi02P = new TH1D( "MassPi02P", "; Mass[#pi^{0}_{2}p] (GeV); Combos / 5 MeV", 500, 1, 3.5 );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "Mesons" )->cd();
	dHist_MassPipPim =  new TH1D( "MassPipPim", ";Mass[#pi^{+}#pi^{-}] (GeV); Combos / 5 MeV", 400, .2, 2.2 );
	dHist_MassPipPi01 =  new TH1D( "MassPipPi01", ";Mass[#pi^{+}#pi^{0}_{1}] (GeV); Combos / 5 MeV", 400, .2, 2.2 );
	dHist_MassPipPi02 =  new TH1D( "MassPipPi02", ";Mass[#pi^{+}#pi^{0}_{2}] (GeV); Combos / 5 MeV", 400, .2, 2.2 );
	dHist_MassPimPi01 =  new TH1D( "MassPimPi01", ";Mass[#pi^{-}#pi^{0}_{1}] (GeV); Combos / 5 MeV", 400, .2, 2.2 );
	dHist_MassPimPi02 =  new TH1D( "MassPimPi02", ";Mass[#pi^{-}#pi^{0}_{2}] (GeV); Combos / 5 MeV", 400, .2, 2.2 );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "Baryon_Meson" )->cd();
	dHist_pipPvspimPi01 = new TH2D( "pipPvspimPi01", ";Mass[#pi^{-}#pi^{0}_{1}] (GeV); Mass[#pi^{+}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pipPvspimPi02 = new TH2D( "pipPvspimPi02", ";Mass[#pi^{-}#pi0_{2}] (GeV); Mass[#pi^{+}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pipPvspi0Pi02 = new TH2D( "pipPvspi0Pi02", ";Mass[#pi^{0}_{1}#pi0_{2}] (GeV); Mass[#pi^{+}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );

	dHist_pimPvspipPi01 = new TH2D( "pimPvspipPi01", ";Mass[#pi^{+}#pi^{0}_{1}] (GeV); Mass[#pi^{-}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pimPvspipPi02 = new TH2D( "pimPvspipPi02", ";Mass[#pi^{-}#pi0_{2}] (GeV); Mass[#pi^{-}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pimPvspi0Pi02 = new TH2D( "pimPvspi0Pi02", ";Mass[#pi^{0}_{1}#pi0_{2}] (GeV); Mass[#pi^{-}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );

	dHist_pi01PvspipPim = new TH2D( "pi01PvspipPim", ";Mass[#pi^{+}#pi^{-}] (GeV); Mass[#pi^{0}_{1}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pi01PvspipPi02 = new TH2D( "pi01PvspipPi02", ";Mass[#pi^{+}#pi0_{2}] (GeV); Mass[#pi^{0}_{1}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pi01PvspimPi02 = new TH2D( "pi01PvspimPi02", ";Mass[#pi^{-}#pi0_{2}] (GeV); Mass[#pi^{0}_{1}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );

	dHist_pi02PvspipPim = new TH2D( "pi02PvspipPim", ";Mass[#pi^{+}#pi^{-}] (GeV); Mass[#pi0_{2}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pi02PvspipPi01 = new TH2D( "pi02PvspipPi01", ";Mass[#pi^{+}#pi^{0}_{1}] (GeV); Mass[#pi0_{2}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	dHist_pi02PvspimPi01 = new TH2D( "pi02PvspimPi01", ";Mass[#pi^{-}#pi^{0}_{1}] (GeV); Mass[#pi0_{2}p] (GeV)", 230, .2, 2.5, 250, 1, 3.5 );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "MesonsVsMesons" )->cd();
	dHist_pipPimvspi01Pi02 = new TH2D( "pipPimvspi01Pi02", ";Mass[#pi^{0}_{1}#pi0_{2}] (GeV); Mass[#pi^{+}#pi^{-}] (GeV)", 230, .2, 2.5, 230, .2, 2.5 );
	dHist_pipPi01vspimPi02 = new TH2D( "pipPi01vspimPi02", ";Mass[#pi^{-}#pi0_{2}] (GeV); Mass[#pi^{+}#pi^{0}_{1}] (GeV)", 230, .2, 2.5, 230, .2, 2.5 );
	dHist_pipPi02vspimPi01 = new TH2D( "pipPi02vspimPi01", ";Mass[#pi^{+}#pi0_{2}] (GeV); Mass[#pi^{-}#pi^{0}_{1}] (GeV)", 230, .2, 2.5, 230, .2, 2.5 );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "4pi_Mass" )->cd();
	dHist_Mass4p = new TH1D( "Mass4pi", ";Mass[4#pi] (GeV); Combos / 5 MeV ", 600, 0.7, 3.7 );
	gDirectory->mkdir( "omega_selected" )->cd();
	dHist_4piMassSum = new TH1D( "4piMassSum", ";Mass[4#pi] (GeV); Combos / 5 MeV ", 600, 0.7, 3.7 );
	gDirectory->cd( ".." );
	gDirectory->cd( ".." );

	gDirectory->mkdir( "t_4pi" )->cd();	
	dHist_t = new TH1D( "t", ";|-t_{4#pi}| (GeV^{2}); Combos/ 20 MeV^{2}", 150, 0, 3 );
	dHist_tVs4piMass = new TH2D( "tVs4piMass", ";Mass[4#pi] (GeV); |-t_{4#pi}| (GeV^{2})", 250, 0.7, 3.7, 150, 0, 3);

	dHist_tprime = new TH1D( "tprime", ";|-t'_{4#pi}| (GeV^{2}); Combos/ 20 MeV^{2}", 150, 0, 3 );
	dHist_tprimeVs4piMass = new TH2D( "tprimeVs4piMass", ";Mass[4#pi] (GeV); |-t'_{4#pi}| (GeV^{2})", 250, 0.7, 3.7, 150, 0, 3);
		
	gDirectory->cd( ".." );

	
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

	// RECOMMENDED: CREATE ACCIDENTAL WEIGHT BRANCH
	// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("accidweight");

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

	SetupAmpTools_FlatTree();

	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("t");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("M3Pi");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("M4Pi");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("MRecoilPi");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("Phi_Prod");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("cosTheta");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("phi");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("cosThetaH");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("phiH");
	dFlatTreeInterface->Create_Branch_Fundamental<Float_t>("lambda");


	//Fully flat P4
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_photon1");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_photon1");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_photon1");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_photon1");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_photon2");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_photon2");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_photon2");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_photon2");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_photon3");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_photon3");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_photon3");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_photon3");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_photon4");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_photon4");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_photon4");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_photon4");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_pip");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_pip");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_pim");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_pim");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_beam");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_beam");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_beam");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_beam");

	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("e_recoil");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("px_recoil");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("py_recoil");
	dFlatTreeInterface->Create_Branch_Fundamental<double_t>("pz_recoil");



	
	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

}

Bool_t DSelector_OmegaPi0::Process(Long64_t locEntry)
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

	//EXAMPLE 0: Event-specific info:
	Bool_t locUsedSoFar_Event = false; // Flag used to mark if the best chi-squared combo is filled in the histogram

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search. This container is used for the "hybrid" method dealing with combinatorics.

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE
	// Single Particle
	set<Int_t> locUsedSoFar_Proton;
	set<Int_t> locUsedSoFar_PiPlus;
	set<Int_t> locUsedSoFar_PiMinus;
	set<Int_t> locUsedSoFar_Photon1;
	set<Int_t> locUsedSoFar_Photon2;
	set<Int_t> locUsedSoFar_Photon3;
	set<Int_t> locUsedSoFar_Photon4;

	// Multiple Particles
	// Photon Combos
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_1_2;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_1_3;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_1_4;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_2_3;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_2_4;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Photons_3_4;

 	// 3 Photons
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3Photons1;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3Photons2;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3Photons3;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3Photons4;

 	// 4 Photons
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_4Photons;

	// 3 Particle Combos
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3pi1Mass;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_3pi2Mass;

	// Baryons
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPipP;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPimP;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPi01P;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPi02P;

	// Mesons
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPipPim;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPipPi01;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPipPi02;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPimPi01;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MassPimPi02;

	// 4Pi
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_4piMass;

	// All particles without beam
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_5Particles;


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

	// Vector to store combo information
	std::vector<std::pair<UInt_t, Double_t>> loc_combos;

	// Pre-loop to gather kinfit ComboIndex-chiSq pairing and sort by chiSq value ascendingly
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		dComboWrapper->Set_ComboIndex(loc_i);
		Double_t locChiSq = dComboWrapper->Get_ChiSq_KinFit("");
		loc_combos.push_back(std::make_pair(loc_i, locChiSq));
	}
	// Sort the combos by ChiSq
	std::sort(loc_combos.begin(), loc_combos.end(), [](const std::pair<UInt_t, Double_t>& a, const std::pair<UInt_t, Double_t>& b) {
		return a.second < b.second;
	});

	//Loop over combos
	for(const auto& loc_combo : loc_combos)
	{
		UInt_t loc_i = loc_combo.first;
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

		//Step 2
		Int_t locPhoton3NeutralID = dPhoton3Wrapper->Get_NeutralID();
		Int_t locPhoton4NeutralID = dPhoton4Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locDecayingPi01P4 = dDecayingPi01Wrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();
		//Step 2
		TLorentzVector locDecayingPi02P4 = dDecayingPi02Wrapper->Get_P4();
		TLorentzVector locPhoton3P4 = dPhoton3Wrapper->Get_P4();
		TLorentzVector locPhoton4P4 = dPhoton4Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();
		//Step 2
		TLorentzVector locPhoton3P4_Measured = dPhoton3Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton4P4_Measured = dPhoton4Wrapper->Get_P4_Measured();


		/*********************************************** GET X4-VECTOR ************************************************/

		//Step 0
		TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
		TLorentzVector locPiPlusX4 = dPiPlusWrapper->Get_X4();
		TLorentzVector locPiMinusX4 = dPiMinusWrapper->Get_X4();
		TLorentzVector locProtonX4 = dProtonWrapper->Get_X4();
		//Step 1
		TLorentzVector locPhoton1X4 = dPhoton1Wrapper->Get_X4();
		TLorentzVector locPhoton2X4 = dPhoton2Wrapper->Get_X4();
		//Step 1
		TLorentzVector locPhoton3X4 = dPhoton3Wrapper->Get_X4();
		TLorentzVector locPhoton4X4 = dPhoton4Wrapper->Get_X4();

		// Get Measured X4's:
		//Step 0
		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		TLorentzVector locPiPlusX4_Measured = dPiPlusWrapper->Get_X4_Measured();
		TLorentzVector locPiMinusX4_Measured = dPiMinusWrapper->Get_X4_Measured();
		TLorentzVector locProtonX4_Measured = dProtonWrapper->Get_X4_Measured();

		//Step 1
		TLorentzVector locPhoton1X4_Measured = dPhoton1Wrapper->Get_X4_Measured();
		TLorentzVector locPhoton2X4_Measured = dPhoton2Wrapper->Get_X4_Measured();

		//Step 2
		TLorentzVector locPhoton3X4_Measured = dPhoton3Wrapper->Get_X4_Measured();
		TLorentzVector locPhoton4X4_Measured = dPhoton4Wrapper->Get_X4_Measured();

		
		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		Int_t locNumOutOfTimeBunchesInTree = 4; //YOU need to specify this number
		//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
			dComboWrapper->Set_IsComboCut(true); 
			continue; 
		} 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured + locPhoton3P4_Measured + locPhoton4P4_Measured;


		//////////////// Define Particle P4 Combinations //////////////////
		
		//Photons Combos
		TLorentzVector locPhotons_1_2_Measured = locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locPhotons_1_3_Measured = locPhoton1P4_Measured + locPhoton3P4_Measured;
		TLorentzVector locPhotons_1_4_Measured = locPhoton1P4_Measured + locPhoton4P4_Measured;
		TLorentzVector locPhotons_2_3_Measured = locPhoton2P4_Measured + locPhoton3P4_Measured;
		TLorentzVector locPhotons_2_4_Measured = locPhoton2P4_Measured + locPhoton4P4_Measured;
		TLorentzVector locPhotons_3_4_Measured = locPhoton3P4_Measured + locPhoton4P4_Measured;

		TLorentzVector locPhotons_1_2 = locPhoton1P4 + locPhoton2P4; //Pi01 hypothesis
		TLorentzVector locPhotons_1_3 = locPhoton1P4 + locPhoton3P4;
		TLorentzVector locPhotons_1_4 = locPhoton1P4 + locPhoton4P4;
		TLorentzVector locPhotons_2_3 = locPhoton2P4 + locPhoton3P4;
		TLorentzVector locPhotons_2_4 = locPhoton2P4 + locPhoton4P4;
		TLorentzVector locPhotons_3_4 = locPhoton3P4 + locPhoton4P4; //Pi02 hypothesis

		//3 Photons
		TLorentzVector locPhotons_1_2_3 = locPhoton1P4 + locPhoton2P4 + locPhoton3P4;
		TLorentzVector locPhotons_1_2_4 = locPhoton1P4 + locPhoton2P4 + locPhoton4P4;
		TLorentzVector locPhotons_1_3_4 = locPhoton1P4 + locPhoton3P4 + locPhoton4P4;
		TLorentzVector locPhotons_2_3_4 = locPhoton2P4 + locPhoton3P4 + locPhoton4P4;

		//4Photons
		TLorentzVector locPhotons_1_2_3_4 = locPhoton1P4 + locPhoton2P4 + locPhoton3P4 + locPhoton4P4;

		//3Pi combos
		TLorentzVector loc3Pi1 = locPiPlusP4 + locPiMinusP4 + locPhotons_1_2;
		TLorentzVector loc3Pi2 = locPiPlusP4 + locPiMinusP4 + locPhotons_3_4;

		//Baryons
		TLorentzVector locPipP = locPiPlusP4 + locProtonP4;
		TLorentzVector locPimP = locPiMinusP4 + locProtonP4;
		TLorentzVector locPi01P = locPhotons_1_2 + locProtonP4;
		TLorentzVector locPi02P = locPhotons_3_4 + locProtonP4;

		//Meson 
		TLorentzVector locPipPim = locPiPlusP4 + locPiMinusP4;
		TLorentzVector locPipPi01 = locPiPlusP4 + locPhotons_1_2;
		TLorentzVector locPipPi02 = locPiPlusP4 + locPhotons_3_4;
		TLorentzVector locPimPi01 = locPiMinusP4 + locPhotons_1_2;
		TLorentzVector locPimPi02 = locPiMinusP4 + locPhotons_3_4;

		//4 Particle
		TLorentzVector loc4pi = locPiPlusP4 + locPiMinusP4 + locPhoton1P4 + locPhoton2P4 + locPhoton3P4 + locPhoton4P4;


		/////////////////////////// Caclculate DeltaT for Particles /////////////////////////////////////////////////////

		//Calculating Delta T (using Measured data)
		double locRFTime = dComboWrapper->Get_RFTime();
		//Beam ( Calculated above )
		// double locPropagatedRFTimeBeam =  locRFTime + ( locBeamX4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		// double locBeamDeltaT = locBeamX4_Measured.T() - locPropagatedRFTimeBeam;
		//Proton
		double locPropagatedRFTimeProton =  locRFTime + ( locProtonX4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locProtonDeltaT = locProtonX4_Measured.T() - locPropagatedRFTimeProton;		
		//PiP
		double locPropagatedRFTimePiPlus = locRFTime + ( locPiPlusX4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPiPlusDeltaT = locPiPlusX4_Measured.T() - locPropagatedRFTimePiPlus;		
		//PiM
		double locPropagatedRFTimePiMinus = locRFTime + ( locPiMinusX4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPiMinusDeltaT = locPiMinusX4_Measured.T() - locPropagatedRFTimePiMinus;
		//Photon1
		double locPropagatedRFTimePhoton1 = locRFTime + ( locPhoton1X4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPhoton1DeltaT = locPhoton1X4_Measured.T() - locPropagatedRFTimePhoton1;
		//Photon2
		double locPropagatedRFTimePhoton2 = locRFTime + ( locPhoton2X4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPhoton2DeltaT = locPhoton2X4_Measured.T() - locPropagatedRFTimePhoton2;
		//Photon3
		double locPropagatedRFTimePhoton3 = locRFTime + ( locPhoton3X4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPhoton3DeltaT = locPhoton3X4_Measured.T() - locPropagatedRFTimePhoton3;
		//Photon4
		double locPropagatedRFTimePhoton4 = locRFTime + ( locPhoton4X4_Measured.Z() - dTargetCenter.Z() )/29.9792458;
		double locPhoton4DeltaT = locPhoton4X4_Measured.T() - locPropagatedRFTimePhoton4;

		//////////////////////////////////// Calculating t and t' //////////////////////////////////////////////

		//Calculating t min for omega eta system
		TLorentzVector locMomentumTransfer = locBeamP4  - loc4pi;//[needs to be squared to be t [.M2()]
		TLorentzVector sMandelstam = locBeamP4 + dTargetP4; //This is not s until .M2() [it needs to be squared]
		// Look at [https://pdg.lbl.gov/2024/reviews/rpp2024-rev-kinematics.pdf] for equations
		double E3CM = ( sMandelstam.M2() + loc4pi.M2() - locProtonP4.M2() ) / (2*sMandelstam.M());// Eq. 49.36  
		double p3CM = sqrt( E3CM*E3CM - loc4pi.M2()); // Eq. 49.37
		double p1CM = ( locBeamP4.Vect() ).Mag() * dTargetP4.M() / sMandelstam.M() ; // Eq. 49.37
		double t_1term =  ( locBeamP4.M2() - loc4pi.M2() - dTargetP4.M2() + locProtonP4.M2() )/ (2*sMandelstam.M()); //  First term in equation 49.35
		double tmin = (t_1term)*(t_1term) - (p1CM - p3CM)*(p1CM - p3CM); // Eq. 49.35
		double tprime = (locMomentumTransfer.M2() - tmin); 
		tprime = -1 * tprime;


		
		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** CUTS  *****************************************/
		
		//Defining booleans for cuts
		// bool confidenceLevel =  dComboWrapper->Get_ConfidenceLevel_KinFit( "" ) < 1e-2;
		double reducedChiSq = dComboWrapper->Get_ChiSq_KinFit("") / dComboWrapper->Get_NDF_KinFit("");
		bool redChiSq = reducedChiSq > 2.5;

		// bool showerQuality_Photon1 = false;
		// bool showerQuality_Photon2 = false;
		// bool showerQuality_Photon3 = false;
		// bool showerQuality_Photon4 = false;

		// //// suggested in the FCAL quality paper
		// if( dPhoton1Wrapper->Get_Detector_System_Timing() == SYS_FCAL )
		//   showerQuality_Photon1 = dPhoton1Wrapper->Get_Shower_Quality() < .5; 
		
		// if( dPhoton2Wrapper->Get_Detector_System_Timing() == SYS_FCAL )
		//   showerQuality_Photon2 = dPhoton2Wrapper->Get_Shower_Quality() < .5;
	        
		// if( dPhoton3Wrapper->Get_Detector_System_Timing() == SYS_FCAL )
		//   showerQuality_Photon3 = dPhoton3Wrapper->Get_Shower_Quality() < .5;
	       
		// if( dPhoton4Wrapper->Get_Detector_System_Timing() == SYS_FCAL )
		//   showerQuality_Photon4 = dPhoton4Wrapper->Get_Shower_Quality() < .5;

		// bool zVertex =  locProtonX4_Measured.Z()  <  52 || locProtonX4_Measured.Z() > 78; // Standard target cut

		bool beamEnergy = locBeamP4_Measured.E() < 8.2 || locBeamP4_Measured.E() > 8.8; // Coherent Peak
		// bool missingEnergy =  abs(locMissingP4_Measured.E()) > .2;
		bool missingMassSquared = abs(locMissingP4_Measured.M2()) > 0.05;
		
		bool tOmegaEtaCut = abs(locMomentumTransfer.M2()) > 1;

		bool activeCuts = redChiSq || beamEnergy || missingMassSquared || tOmegaEtaCut;
		// if( activeCuts ){
		//   dComboWrapper->Set_IsComboCut(true);
		//   continue;
		// }


		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: BEST chi2 METHOD *****************************************/

		// Need to uncomment the section computing combo timing info before running this block of code
		//if(locUsedSoFar_Event == false)
		//{
			// Fill the histogram only when the beam bunch is in-time. 
			//if(!locRelBeamBucket)
			//{
			//	dHist_BeamEnergy_BestChiSq->Fill(locBeamP4.E());
			//	locUsedSoFar_Event = true;
			//}
		//}

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction
			dHist_BeamTiming->Fill( locDeltaT_RF );

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		///////////////////////// Single Particle Hist ///////////////////////////////////////////
		//Proton
		if(locUsedSoFar_Proton.find( locProtonTrackID ) == locUsedSoFar_Proton.end() ){
		  //Timing
		  if( dProtonWrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_Proton_BCAL->Fill( locProtonP4_Measured.P(), locProtonDeltaT );  
		  		  
		  if( dProtonWrapper->Get_Detector_System_Timing() == SYS_TOF )
		    dHist_DeltaTVsP_Proton_TOF->Fill( locProtonP4_Measured.P(), locProtonDeltaT );  
		  
		  //CDC dE/dx
		  if( dProtonWrapper->Get_dEdx_CDC() > 0. )
		    dHist_dEdxVsP_Proton_CDC->Fill( locProtonP4_Measured.P(), dProtonWrapper->Get_dEdx_CDC()*1e6 );

		  //Vertex
		  dHist_VertexProtonZ->Fill( locProtonX4_Measured.Z() );
		  dHist_VertexProtonXY->Fill( locProtonX4_Measured.X(), locProtonX4_Measured.Y() );

		  locUsedSoFar_Proton.insert( locProtonTrackID );
		}

		//PiPlus
		if(locUsedSoFar_PiPlus.find( locPiPlusTrackID ) == locUsedSoFar_PiPlus.end() ){

		  if( dPiPlusWrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_PiP_BCAL->Fill( locPiPlusP4_Measured.P(), locPiPlusDeltaT );  
		  		  
		  if( dPiPlusWrapper->Get_Detector_System_Timing() == SYS_TOF )
		    dHist_DeltaTVsP_PiP_TOF->Fill( locPiPlusP4_Measured.P(), locPiPlusDeltaT );  
		  		  
		  if( dPiPlusWrapper->Get_dEdx_CDC() > 0. )
		    dHist_dEdxVsP_PiP_CDC->Fill( locPiPlusP4_Measured.P(), dPiPlusWrapper->Get_dEdx_CDC()*1e6 );

		  locUsedSoFar_PiPlus.insert( locPiPlusTrackID );
		}

		//PiMinus
		if(locUsedSoFar_PiMinus.find( locPiMinusTrackID ) == locUsedSoFar_PiMinus.end() ){

		  if( dPiMinusWrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_PiM_BCAL->Fill( locPiMinusP4_Measured.P(), locPiMinusDeltaT );  
		  
		  if( dPiMinusWrapper->Get_Detector_System_Timing() == SYS_TOF )
		    dHist_DeltaTVsP_PiM_TOF->Fill( locPiMinusP4_Measured.P(), locPiMinusDeltaT );  
		  		  
		  if( dPiMinusWrapper->Get_dEdx_CDC() > 0. )
		    dHist_dEdxVsP_PiM_CDC->Fill( locPiMinusP4_Measured.P(), dPiMinusWrapper->Get_dEdx_CDC()*1e6 );

		  locUsedSoFar_PiMinus.insert( locPiMinusTrackID );
		}

		//Photon1
		if( locUsedSoFar_Photon1.find( locPhoton1NeutralID ) == locUsedSoFar_Photon1.end() ){
		 
		  //Timing
		  if( dPhoton1Wrapper->Get_Detector_System_Timing() == SYS_FCAL ){
		    dHist_DeltaTVsP_Photon_FCAL->Fill( locPhoton1P4_Measured.P(), locPhoton1DeltaT );

		    //Shower Quality
		    dHist_ShowerQuality1->Fill( dPhoton1Wrapper->Get_Shower_Quality() );		  
		  }

		  if( dPhoton1Wrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_Photon_BCAL->Fill( locPhoton1P4_Measured.P(), locPhoton1DeltaT );
		  

		  
		  locUsedSoFar_Photon1.insert( locPhoton1NeutralID );
		}

		//Photon2
		if( locUsedSoFar_Photon2.find( locPhoton2NeutralID ) == locUsedSoFar_Photon2.end() ){

		  //Timing
		  if( dPhoton2Wrapper->Get_Detector_System_Timing() == SYS_FCAL ){
		    dHist_DeltaTVsP_Photon_FCAL->Fill( locPhoton2P4_Measured.P(), locPhoton2DeltaT );
		      
		    //Shower Quality
		    dHist_ShowerQuality2->Fill( dPhoton2Wrapper->Get_Shower_Quality() );
		  }
		    
		  if( dPhoton2Wrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_Photon_BCAL->Fill( locPhoton2P4_Measured.P(), locPhoton2DeltaT );
		    


		  locUsedSoFar_Photon2.insert( locPhoton2NeutralID );
		}

		//Photon3
		if( locUsedSoFar_Photon3.find( locPhoton3NeutralID ) == locUsedSoFar_Photon3.end() ){
		    
		  //Timing
		  if( dPhoton3Wrapper->Get_Detector_System_Timing() == SYS_FCAL ){
		    dHist_DeltaTVsP_Photon_FCAL->Fill( locPhoton3P4_Measured.P(), locPhoton3DeltaT );

		    //Shower Quality
		    dHist_ShowerQuality3->Fill( dPhoton3Wrapper->Get_Shower_Quality() ); 
		  }
		    
		  if( dPhoton3Wrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_Photon_BCAL->Fill( locPhoton3P4_Measured.P(), locPhoton3DeltaT );  
		    
		  locUsedSoFar_Photon3.insert( locPhoton3NeutralID );
		}

		//Photon4
		if( locUsedSoFar_Photon4.find( locPhoton4NeutralID ) == locUsedSoFar_Photon4.end() ){

		  //Timing
		  if( dPhoton4Wrapper->Get_Detector_System_Timing() == SYS_FCAL ){
		    dHist_DeltaTVsP_Photon_FCAL->Fill( locPhoton4P4_Measured.P(), locPhoton4DeltaT );
		    
		    //Shower Quality
		    dHist_ShowerQuality4->Fill( dPhoton4Wrapper->Get_Shower_Quality() );      
		  }

		  if( dPhoton4Wrapper->Get_Detector_System_Timing() == SYS_BCAL )
		    dHist_DeltaTVsP_Photon_BCAL->Fill( locPhoton4P4_Measured.P(), locPhoton4DeltaT );
		    
		  locUsedSoFar_Photon4.insert( locPhoton4NeutralID );
		}


		
		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "UnknownParticle" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[UnknownParticle].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton3NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton4NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared); // Fills in-time and out-of-time beam photon combos
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			dHist_MissingEnergy->Fill( locMissingP4_Measured.E() );
					  
			/// t plots
			dHist_t->Fill( abs(locMomentumTransfer.M2()) );
			dHist_tprime->Fill( abs(tprime) );		  
			dHist_tVs4piMass->Fill( loc4pi.M(), abs(locMomentumTransfer.M2()) );
			dHist_tprimeVs4piMass->Fill( loc4pi.M(), abs(tprime) );


			// CL and ChiSq
			dHist_ConfidenceLevel_logX->Fill( dComboWrapper->Get_ConfidenceLevel_KinFit( "" ) );
			dHist_ChiSq->Fill(  dComboWrapper->Get_ChiSq_KinFit("") / dComboWrapper->Get_NDF_KinFit("") );
			dHist_ChiSq_z->Fill(  dComboWrapper->Get_ChiSq_KinFit("") / dComboWrapper->Get_NDF_KinFit("") );
			dHist_ChiSqVsConfidenceLevel_logX->Fill( dComboWrapper->Get_ConfidenceLevel_KinFit( "" ) , dComboWrapper->Get_ChiSq_KinFit( "" ) );

			//Unused Shower Energy
			dHist_UnusedShowerEnergy->Fill( dComboWrapper->Get_Energy_UnusedShowers() );

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}


		/////////////////// Photon Plots /////////////
		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_1_2;
 		locUsedThisCombo_Photons_1_2[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_Photons_1_2[Gamma].insert(locPhoton2NeutralID);
 		if(locUsedSoFar_Photons_1_2.find(locUsedThisCombo_Photons_1_2) == locUsedSoFar_Photons_1_2.end()){
 		  //Mass
 		  dHist_Mass_g1_g2_M->Fill( locPhotons_1_2_Measured.M() );
 		  dHist_Mass_g1_g2->Fill( locPhotons_1_2.M() );
 		  locUsedSoFar_Photons_1_2.insert(locUsedThisCombo_Photons_1_2);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_1_3;
 		locUsedThisCombo_Photons_1_3[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_Photons_1_3[Gamma].insert(locPhoton3NeutralID);
 		if(locUsedSoFar_Photons_1_3.find(locUsedThisCombo_Photons_1_3) == locUsedSoFar_Photons_1_3.end()){
 		  //Mass
 		  dHist_Mass_g1_g3_M->Fill( locPhotons_1_3_Measured.M() );
 		  dHist_Mass_g1_g3->Fill( locPhotons_1_3.M() );
 		  locUsedSoFar_Photons_1_3.insert(locUsedThisCombo_Photons_1_3);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_1_4;
 		locUsedThisCombo_Photons_1_4[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_Photons_1_4[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_Photons_1_4.find(locUsedThisCombo_Photons_1_4) == locUsedSoFar_Photons_1_4.end()){
 		  //Mass
 		  dHist_Mass_g1_g4_M->Fill( locPhotons_1_4_Measured.M() );
 		  dHist_Mass_g1_g4->Fill( locPhotons_1_4.M() );
 		  locUsedSoFar_Photons_1_4.insert(locUsedThisCombo_Photons_1_4);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_2_3;
 		locUsedThisCombo_Photons_2_3[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_Photons_2_3[Gamma].insert(locPhoton3NeutralID);
 		if(locUsedSoFar_Photons_2_3.find(locUsedThisCombo_Photons_2_3) == locUsedSoFar_Photons_2_3.end()){
 		  //Mass
 		  dHist_Mass_g2_g3_M->Fill( locPhotons_2_3_Measured.M() );
 		  dHist_Mass_g2_g3->Fill( locPhotons_2_3.M() );
 		  locUsedSoFar_Photons_2_3.insert(locUsedThisCombo_Photons_2_3);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_2_4;
 		locUsedThisCombo_Photons_2_4[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_Photons_2_4[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_Photons_2_4.find(locUsedThisCombo_Photons_2_4) == locUsedSoFar_Photons_2_4.end()){
 		  //Mass
 		  dHist_Mass_g2_g4_M->Fill( locPhotons_2_4_Measured.M() );
 		  dHist_Mass_g2_g4->Fill( locPhotons_2_4.M() );
 		  locUsedSoFar_Photons_2_4.insert(locUsedThisCombo_Photons_2_4);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_Photons_3_4;
 		locUsedThisCombo_Photons_3_4[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_Photons_3_4[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_Photons_3_4.find(locUsedThisCombo_Photons_3_4) == locUsedSoFar_Photons_3_4.end()){
 		  //Mass
 		  dHist_Mass_g3_g4_M->Fill( locPhotons_3_4_Measured.M() );
 		  dHist_Mass_g3_g4->Fill( locPhotons_3_4.M() );
 		  locUsedSoFar_Photons_3_4.insert(locUsedThisCombo_Photons_3_4);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_3Photons1;
 		locUsedThisCombo_3Photons1[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_3Photons1[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_3Photons1[Gamma].insert(locPhoton3NeutralID);
 		if(locUsedSoFar_3Photons1.find(locUsedThisCombo_3Photons1) == locUsedSoFar_3Photons1.end()){
 		  //3 Photons
 		  dHist_Mass_g1_g2_g3->Fill(locPhotons_1_2_3.M());
 		  locUsedSoFar_3Photons1.insert(locUsedThisCombo_3Photons1);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_3Photons2;
 		locUsedThisCombo_3Photons2[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_3Photons2[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_3Photons2[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_3Photons2.find(locUsedThisCombo_3Photons2) == locUsedSoFar_3Photons2.end()){
 		  //3 Photons
 		  dHist_Mass_g1_g2_g4->Fill(locPhotons_1_2_4.M());		  
 		  locUsedSoFar_3Photons2.insert(locUsedThisCombo_3Photons2);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_3Photons3;
 		locUsedThisCombo_3Photons3[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_3Photons3[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_3Photons3[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_3Photons3.find(locUsedThisCombo_3Photons3) == locUsedSoFar_3Photons3.end()){
 		  //3 Photons
 		  dHist_Mass_g1_g3_g4->Fill(locPhotons_1_3_4.M()); 
 		  locUsedSoFar_3Photons3.insert(locUsedThisCombo_3Photons3);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_3Photons4;
 		locUsedThisCombo_3Photons4[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_3Photons4[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_3Photons4[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_3Photons4.find(locUsedThisCombo_3Photons4) == locUsedSoFar_3Photons4.end()){
 		  //3 Photons
 		  dHist_Mass_g2_g3_g4->Fill(locPhotons_2_3_4.M());
 		  locUsedSoFar_3Photons4.insert(locUsedThisCombo_3Photons4);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_4Photons;
 		locUsedThisCombo_4Photons[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_4Photons[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_4Photons[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_4Photons[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_4Photons.find(locUsedThisCombo_4Photons) == locUsedSoFar_4Photons.end()){
 		  dHist_g1g2vsg3g4_M->Fill( locPhotons_3_4_Measured.M(), locPhotons_1_2_Measured.M() );
 		  dHist_g1g3vsg2g4_M->Fill( locPhotons_2_4_Measured.M(), locPhotons_1_3_Measured.M() );
 		  dHist_g1g4vsg2g3_M->Fill( locPhotons_2_3_Measured.M(), locPhotons_1_4_Measured.M() );

 		  dHist_g1g2vsg3g4->Fill( locPhotons_3_4.M(), locPhotons_1_2.M() );
 		  dHist_g1g3vsg2g4->Fill( locPhotons_2_4.M(), locPhotons_1_3.M() );
 		  dHist_g1g4vsg2g3->Fill( locPhotons_2_3.M(), locPhotons_1_4.M() );

 		  dHist_Mass_g1_g2_g3_g4->Fill(locPhotons_1_2_3_4.M());
 	       
 		  locUsedSoFar_4Photons.insert(locUsedThisCombo_4Photons);
 		}


		///////////////////////// Baryons ///////////////////////////////

		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPipP;
 		locUsedThisCombo_MassPipP[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPipP[Proton].insert(locProtonTrackID);
       		if(locUsedSoFar_MassPipP.find(locUsedThisCombo_MassPipP) == locUsedSoFar_MassPipP.end()){
		  dHist_MassPipP->Fill( locPipP.M());
 		  locUsedSoFar_MassPipP.insert(locUsedThisCombo_MassPipP);
 		}
	
 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPimP;
 		locUsedThisCombo_MassPimP[PiMinus].insert(locPiMinusTrackID);
 		locUsedThisCombo_MassPimP[Proton].insert(locProtonTrackID);
       		if(locUsedSoFar_MassPimP.find(locUsedThisCombo_MassPimP) == locUsedSoFar_MassPimP.end()){
		  dHist_MassPimP->Fill( locPimP.M());
 		  locUsedSoFar_MassPimP.insert(locUsedThisCombo_MassPimP);
 		}
	
 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPi01P;
 		locUsedThisCombo_MassPi01P[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_MassPi01P[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_MassPi01P[Proton].insert(locProtonTrackID);
       		if(locUsedSoFar_MassPi01P.find(locUsedThisCombo_MassPi01P) == locUsedSoFar_MassPi01P.end()){
		  dHist_MassPi01P->Fill( locPi01P.M());
 		  locUsedSoFar_MassPi01P.insert(locUsedThisCombo_MassPi01P);
 		}

		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPi02P;
 		locUsedThisCombo_MassPi02P[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_MassPi02P[Gamma].insert(locPhoton4NeutralID);
 		locUsedThisCombo_MassPi02P[Proton].insert(locProtonTrackID);
       		if(locUsedSoFar_MassPi02P.find(locUsedThisCombo_MassPi02P) == locUsedSoFar_MassPi02P.end()){
 		  dHist_MassPi02P->Fill( locPi02P.M());
 		  locUsedSoFar_MassPi02P.insert(locUsedThisCombo_MassPi02P);
 		}

		///////////////////////// Mesons ///////////////////////////////
		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPipPim;
 		locUsedThisCombo_MassPipPim[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPipPim[PiMinus].insert(locPiMinusTrackID);
       		if(locUsedSoFar_MassPipPim.find(locUsedThisCombo_MassPipPim) == locUsedSoFar_MassPipPim.end()){
 		   dHist_MassPipPim->Fill( locPipPim.M());
 		  locUsedSoFar_MassPipPim.insert(locUsedThisCombo_MassPipPim);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPipPi01;
 		locUsedThisCombo_MassPipPi01[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPipPi01[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_MassPipPi01[Gamma].insert(locPhoton2NeutralID);
 		if(locUsedSoFar_MassPipPi01.find(locUsedThisCombo_MassPipPi01) == locUsedSoFar_MassPipPi01.end()){
 		  dHist_MassPipPi01->Fill( locPipPi01.M());
 		  locUsedSoFar_MassPipPi01.insert(locUsedThisCombo_MassPipPi01);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPipPi02;
 		locUsedThisCombo_MassPipPi02[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPipPi02[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_MassPipPi02[Gamma].insert(locPhoton4NeutralID);	
    		if(locUsedSoFar_MassPipPi02.find(locUsedThisCombo_MassPipPi02) == locUsedSoFar_MassPipPi02.end()){
 		  dHist_MassPipPi02->Fill( locPipPi02.M());
 		  locUsedSoFar_MassPipPi02.insert(locUsedThisCombo_MassPipPi02);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPimPi01;
 		locUsedThisCombo_MassPimPi01[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPimPi01[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_MassPimPi01[Gamma].insert(locPhoton2NeutralID);      
  		if(locUsedSoFar_MassPimPi01.find(locUsedThisCombo_MassPimPi01) == locUsedSoFar_MassPimPi01.end()){
 		  dHist_MassPimPi01->Fill( locPimPi01.M());
 		  locUsedSoFar_MassPimPi01.insert(locUsedThisCombo_MassPimPi01);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_MassPimPi02;
 		locUsedThisCombo_MassPimPi02[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_MassPimPi02[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_MassPimPi02[Gamma].insert(locPhoton4NeutralID);
 		if(locUsedSoFar_MassPimPi02.find(locUsedThisCombo_MassPimPi02) == locUsedSoFar_MassPimPi02.end()){
 		  dHist_MassPimPi02->Fill( locPimPi02.M());
 		  locUsedSoFar_MassPimPi02.insert(locUsedThisCombo_MassPimPi02);
 		}

 		//3pi
 		map<Particle_t, set<Int_t> > locUsedThisCombo_3pi1Mass;
 		locUsedThisCombo_3pi1Mass[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_3pi1Mass[PiMinus].insert(locPiMinusTrackID);
 		locUsedThisCombo_3pi1Mass[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_3pi1Mass[Gamma].insert(locPhoton2NeutralID);
       		if(locUsedSoFar_3pi1Mass.find(locUsedThisCombo_3pi1Mass) == locUsedSoFar_3pi1Mass.end()){
 		  dHist_Mass3pi1->Fill( loc3Pi1.M());
 		  locUsedSoFar_3pi1Mass.insert(locUsedThisCombo_3pi1Mass);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_3pi2Mass;
 		locUsedThisCombo_3pi2Mass[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_3pi2Mass[PiMinus].insert(locPiMinusTrackID);
 		locUsedThisCombo_3pi2Mass[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_3pi2Mass[Gamma].insert(locPhoton4NeutralID);
       		if(locUsedSoFar_3pi2Mass.find(locUsedThisCombo_3pi2Mass) == locUsedSoFar_3pi2Mass.end()){
 		  dHist_Mass3pi2->Fill( loc3Pi2.M());
 		  locUsedSoFar_3pi2Mass.insert(locUsedThisCombo_3pi2Mass);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_4piMass;
 		locUsedThisCombo_4piMass[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_4piMass[PiMinus].insert(locPiMinusTrackID);
 		locUsedThisCombo_4piMass[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_4piMass[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_4piMass[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_4piMass[Gamma].insert(locPhoton4NeutralID);
       		if(locUsedSoFar_4piMass.find(locUsedThisCombo_4piMass) == locUsedSoFar_4piMass.end()){

		  dHist_Mass3pi2vs3pi1->Fill( loc3Pi1.M(), loc3Pi2.M() );

 		  //Meson vs Meson
 		  dHist_pipPimvspi01Pi02->Fill( locPhotons_1_2_3_4.M(), locPipPim.M() );
 		  dHist_pipPi01vspimPi02->Fill( locPimPi02.M(), locPipPi01.M() );
 		  dHist_pipPi02vspimPi01->Fill( locPimPi01.M(), locPipPi02.M() );
		  
 		  locUsedSoFar_4piMass.insert(locUsedThisCombo_4piMass);
 		}

 		map<Particle_t, set<Int_t> > locUsedThisCombo_5Particles;
 		locUsedThisCombo_5Particles[PiPlus].insert(locPiPlusTrackID);
 		locUsedThisCombo_5Particles[PiMinus].insert(locPiMinusTrackID);
 		locUsedThisCombo_5Particles[Proton].insert(locProtonTrackID);
 		locUsedThisCombo_5Particles[Gamma].insert(locPhoton1NeutralID);
 		locUsedThisCombo_5Particles[Gamma].insert(locPhoton2NeutralID);
 		locUsedThisCombo_5Particles[Gamma].insert(locPhoton3NeutralID);
 		locUsedThisCombo_5Particles[Gamma].insert(locPhoton4NeutralID);
 		if( locUsedSoFar_5Particles.find( locUsedThisCombo_5Particles ) == locUsedSoFar_5Particles.end() ){

 		  dHist_pipPvspimPi01->Fill( locPimPi01.M(), locPipP.M() );
 		  dHist_pipPvspimPi02->Fill( locPimPi02.M(), locPipP.M() );
 		  dHist_pipPvspi0Pi02->Fill( locPhotons_1_2_3_4.M(), locPipP.M() );

 		  dHist_pimPvspipPi01->Fill( locPipPi01.M(), locPimP.M() );
 		  dHist_pimPvspipPi02->Fill( locPipPi02.M(), locPimP.M() );
 		  dHist_pimPvspi0Pi02->Fill( locPhotons_1_2_3_4.M(), locPimP.M() );

 		  dHist_pi01PvspipPim->Fill( locPipPim.M(), locPi01P.M() );
 		  dHist_pi01PvspipPi02->Fill( locPipPi02.M(), locPi01P.M() );
 		  dHist_pi01PvspimPi02->Fill( locPimPi02.M(), locPi01P.M() );

 		  dHist_pi02PvspipPim->Fill( locPipPim.M(), locPi02P.M() );
 		  dHist_pi02PvspipPi01->Fill( locPipPi01.M(), locPi02P.M() );
 		  dHist_pi02PvspimPi01->Fill( locPimPi01.M(), locPi02P.M() );

 		  locUsedSoFar_5Particles.insert( locUsedThisCombo_5Particles );
 		}


		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		// RECOMMENDED: FILL ACCIDENTAL WEIGHT
		// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		vector<TLorentzVector> locPi0P4;
		locPi0P4.push_back(locDecayingPi01P4);
		locPi0P4.push_back(locDecayingPi02P4);
		for (int i = 0; i < (int)locPi0P4.size(); i++){

		  // omega mass cut and sideband weight
		  TLorentzVector loc3PiP4 = locPi0P4[i] + locPiPlusP4 + locPiMinusP4;
		  double loc3PiMass = loc3PiP4.M();
		  TLorentzVector loc3PiP4_alt = locPi0P4[abs(i - 1)] + locPiPlusP4 + locPiMinusP4;
		  double loc3PiMass_alt = loc3PiP4_alt.M();

		  if( i==0)
		    dHist_3piMassCorr->Fill(loc3PiMass, loc3PiMass_alt);
		  else
		    dHist_3piMassCorr->Fill(loc3PiMass, loc3PiMass_alt);
		  
		  
		  //duble loc2Dweight;
		  double loc2Dweight=1;
		  double locLmin = 0.690;
		  double locLmax = 0.735;
		  double locomegamin = 0.760;
		  double locomegamax = 0.805;
		  double locHmin = 0.830;
		  double locHmax = 0.875;

		  if ((loc3PiMass > locomegamin && loc3PiMass < locomegamax) && (loc3PiMass_alt > locomegamin && loc3PiMass_alt < locomegamax))
		    {
		      // "double-omega" events only used once, closest to true omega mass
		      if (fabs(loc3PiMass - 0.783) < fabs(loc3PiMass_alt - 0.783))
			loc2Dweight = 1.0;
		      else
			continue;
		    }
		  else if ((loc3PiMass > locomegamin && loc3PiMass < locomegamax))
		    loc2Dweight = 1.0;
		  else if ((loc3PiMass_alt > locomegamin && loc3PiMass_alt < locomegamax))
		    continue;
		  // contol region overlap (cross hatched) are counted twice with a weight of -5/8
		  else if ((loc3PiMass > locLmin && loc3PiMass < locLmax) && (loc3PiMass_alt > locLmin && loc3PiMass_alt < locLmax))
		    loc2Dweight = -0.625;
		  else if ((loc3PiMass > locLmin && loc3PiMass < locLmax) && (loc3PiMass_alt > locHmin && loc3PiMass_alt < locHmax))
		    loc2Dweight = -0.625;
		  else if ((loc3PiMass > locHmin && loc3PiMass < locHmax) && (loc3PiMass_alt > locLmin && loc3PiMass_alt < locLmax))
		    loc2Dweight = -0.625;
		  else if ((loc3PiMass > locHmin && loc3PiMass < locHmax) && (loc3PiMass_alt > locHmin && loc3PiMass_alt < locHmax))
		    loc2Dweight = -0.625;
		  // control regions included once with weight of -0.5
		  else if ((loc3PiMass > locLmin && loc3PiMass < locLmax) || (loc3PiMass > locHmin && loc3PiMass < locHmax))
		    loc2Dweight = -0.5;
		  else
		    continue;

		  TLorentzVector locProtonPi0P4 = locProtonP4 + locPi0P4[abs(i - 1)];
		  TLorentzVector loc4PiP4 = loc3PiP4 + locPi0P4[abs(i - 1)];
		  double loc4PiMass = loc4PiP4.M();
		  dHist_4piMassSum->Fill(loc4PiMass, loc2Dweight);

			
		  if (i == 0)
		    dHist_3piMassCorr2DWeight->Fill(loc3PiMass, loc3PiMass_alt, loc2Dweight);	  
		  else
		    dHist_3piMassCorr2DWeight->Fill(loc3PiMass_alt, loc3PiMass, loc2Dweight);

		  // some plots of angular distributions
		  TLorentzVector locGammapP4 = locBeamP4 + dTargetP4;
		  TVector3 locGammapBoost = locGammapP4.BoostVector();
		  TLorentzVector locBeamP4_gpRest = locBeamP4;
		  locBeamP4_gpRest.Boost(-1.0 * locGammapBoost);
		  TLorentzVector locOmegaPi0P4_gpRest = loc4PiP4;
		  locOmegaPi0P4_gpRest.Boost(-1.0 * locGammapBoost);
		  TLorentzVector locOmegaP4_gpRest = loc3PiP4;
		  locOmegaP4_gpRest.Boost(-1.0 * locGammapBoost);
		  TLorentzVector locProtonP4_gpRest = locProtonP4;
		  locProtonP4_gpRest.Boost(-1.0 * locGammapBoost);

		  TVector3 locOmegapiBoost = locOmegaPi0P4_gpRest.BoostVector();
		  TLorentzVector locOmegaP4_opiRest = locOmegaP4_gpRest;
		  locOmegaP4_opiRest.Boost(-1.0 * locOmegapiBoost);
		  TVector3 locOmegaP3_opiRest = locOmegaP4_opiRest.Vect();
		  TLorentzVector locProtonP4_opiRest = locProtonP4_gpRest;
		  locProtonP4_opiRest.Boost(-1.0 * locOmegapiBoost);
		  TVector3 locProtonP3_opiRest = locProtonP4_opiRest.Vect();

		  TVector3 locz = -1.0 * locProtonP3_opiRest.Unit();
		  TVector3 locy = locBeamP4_gpRest.Vect().Cross(locProtonP3_opiRest).Unit();
		  TVector3 locx = locy.Cross(locz).Unit();
		  TVector3 locOmegaP3_Angles(locOmegaP3_opiRest.Dot(locx), locOmegaP3_opiRest.Dot(locy), locOmegaP3_opiRest.Dot(locz));

		  double loccostheta = locOmegaP3_Angles.CosTheta();
		  double locphi = locOmegaP3_Angles.Phi();

		  // omega decay angles
		  TVector3 locOmegaBoost = locOmegaP4_opiRest.BoostVector();
		  TLorentzVector locPiPlusP4_oRest = locPiPlusP4;
		  locPiPlusP4_oRest.Boost(-1.0 * locGammapBoost);
		  locPiPlusP4_oRest.Boost(-1.0 * locOmegapiBoost);
		  locPiPlusP4_oRest.Boost(-1.0 * locOmegaBoost);
		  TLorentzVector locPiMinusP4_oRest = locPiMinusP4;
		  locPiMinusP4_oRest.Boost(-1.0 * locGammapBoost);
		  locPiMinusP4_oRest.Boost(-1.0 * locOmegapiBoost);
		  locPiMinusP4_oRest.Boost(-1.0 * locOmegaBoost);

		  TVector3 locPlusCrossMinus = locPiPlusP4_oRest.Vect().Cross(locPiMinusP4_oRest.Vect());
		  TVector3 locNormalP3 = locPlusCrossMinus.Unit();

		  TVector3 loczH = locOmegaP3_opiRest.Unit();
		  TVector3 locyH = locz.Cross(loczH).Unit();
		  TVector3 locxH = locyH.Cross(loczH).Unit();
		  TVector3 locNormalP3_Angles(locNormalP3.Dot(locxH), locNormalP3.Dot(locyH), locNormalP3.Dot(loczH));
		  double loccosthetaH = locNormalP3_Angles.CosTheta();
		  double locphiH = locNormalP3_Angles.Phi();

		  // lambda matrix element
		  double loclambda = 4 / 3. * fabs(locPlusCrossMinus.Dot(locPlusCrossMinus)) / TMath::Power((1 / 9. * loc3PiP4.M2() - locPi0P4[i].M2()), 2.);

		  double loct = -1. * (locProtonP4 - dTargetP4).M2();
			
		

		  // set ordering of pions for amplitude analysis
		  vector<TLorentzVector> locFinalStateP4;
		  locFinalStateP4.push_back(locProtonP4);
		  locFinalStateP4.push_back(locPi0P4[abs(i - 1)]); // Bachelor Pi0
		  locFinalStateP4.push_back(locPi0P4[i]);
		  locFinalStateP4.push_back(locPiPlusP4);
		  locFinalStateP4.push_back(locPiMinusP4);

		  dFlatTreeInterface->Fill_Fundamental<Float_t>("M3Pi", loc3PiMass);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("M4Pi", loc4PiMass);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("MRecoilPi", locProtonPi0P4.M());
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("Phi_Prod", locProtonP4.Phi());
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("t", loct);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("cosTheta", loccostheta);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("phi", locphi);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("cosThetaH", loccosthetaH);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("phiH", locphiH);
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("lambda", loclambda);

		  /// full flat P4 (filled twice if not in 'if' statement)
		  if (i == 0){
		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_photon1",locPhoton1P4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_photon1",locPhoton1P4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_photon1",locPhoton1P4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_photon1",locPhoton1P4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_photon2",locPhoton2P4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_photon2",locPhoton2P4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_photon2",locPhoton2P4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_photon2",locPhoton2P4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_photon3",locPhoton3P4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_photon3",locPhoton3P4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_photon3",locPhoton3P4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_photon3",locPhoton3P4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_photon4",locPhoton4P4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_photon4",locPhoton4P4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_photon4",locPhoton4P4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_photon4",locPhoton4P4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_pip",locPiPlusP4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_pip",locPiPlusP4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_pip",locPiPlusP4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_pip",locPiPlusP4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_pim",locPiMinusP4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_pim",locPiMinusP4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_pim",locPiMinusP4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_pim",locPiMinusP4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_beam",locBeamP4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_beam",locBeamP4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_beam",locBeamP4.Py());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_beam",locBeamP4.Pz());

		    dFlatTreeInterface->Fill_Fundamental<double_t>("e_recoil",locProtonP4.E());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("px_recoil",locProtonP4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("py_recoil",locProtonP4.Px());
		    dFlatTreeInterface->Fill_Fundamental<double_t>("pz_recoil",locProtonP4.Px());
		  }
			
		  // Set weight from the 2D sidebands and accidentals
		  dFlatTreeInterface->Fill_Fundamental<Float_t>("Weight", loc2Dweight * locHistAccidWeightFactor);
		  FillAmpTools_FlatTree(locBeamP4, locFinalStateP4);

		
		  //FILL FLAT TREE
		  Fill_FlatTree(); //for the active combo
		}
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

void DSelector_OmegaPi0::Finalize(void)
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
