#include "DSelector_etapi.h"
#include "TLorentzRotation.h"

void DSelector_etapi::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "output_dselector.root"; //"" for none
	dOutputTreeFileName = "output_tree.root"; //"" for none
	dFlatTreeFileName = "output_flat.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
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
	//std::deque<Particle_t> MyPhi;
	//MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

//	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
//	//false/true below: use measured/kinfit data
//
//	//PID
//	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
//	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
//	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));
//
//	//PIDFOM (for charged tracks)
//	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
//	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
//	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));
//
//	//MASSES
//	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
//	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));
//
//	//KINFIT RESULTS
//	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));
//
//	//CUT MISSING MASS
//	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));
//
//	//CUT ON SHOWER QUALITY
//	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));
//
//	//BEAM ENERGY
//	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
//	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999
//
//	////KINEMATICS
//	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// Change MyPhi to match reaction
	//dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	//dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	// Best practices is to include the bin width in the axis labels
	//dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 200, -0.2, 0.2);
	dHist_BeamEnergy = new TH1F("BeamEnergy", ";Beam Energy (GeV);Entries / 0.1 GeV", 90, 3.0, 12.0);
	dHist_Metapi_tot = new TH1F("Metapi_tot",";M(4#gamma) (GeV);Entries / 0.04 GeV",50,0.5,2.5);
	dHist_Metapi_sig = new TH1F("Metapi_sig",";M(4#gamma) (GeV);Entries / 0.04 GeV",50,0.5,2.5);
	dHist_Metapi_sig_zoomed = new TH1F("Metapi_sig_zoom",";M(4#gamma) (GeV);Entries / 0.04 GeV",13,1.04,1.56); // zooms into the a2(1320) mass region
	dHist_Metapi_bkg = new TH1F("Metapi_bkg",";M(4#gamma) (GeV);Entries / 0.04 GeV",50,0.5,2.5);
	dHist_Mpi0p = new TH1F("Mpi0p",";M(#gamma_{1}#gamma_{2}p) (GeV);Entries / 0.04 GeV",100,0,4);
	dHist_Metap = new TH1F("Metap",";M(#gamma_{3}#gamma_{4}p) (GeV);Entries / 0.04 GeV",100,0,4);
	dHist_Meta = new TH1F("Meta",";M(#gamma_{3}#gamma_{4}) (GeV);Entries / 0.05 GeV",100,0.3,0.8);
	dHist_Mpi0 = new TH1F("Mpi0",";M(#gamma_{1}#gamma_{2}) (GeV);Entries / 0.0012 GeV",100,0.08,0.2);
	dHist_t = new TH1F("mandelstam_t",";-t GeV^{2};Entries / 0.05 GeV^{2}",60,0,3);
	dHist_rf = new TH1F("rftime",";RF (ns);Entries / 0.4 ns",120,-24,24);
	dHist_mmsq = new TH1F("mmsq",";MMsq;Entries / 0.002 GeV",100,-0.1,0.1);
	dHist_chiSq = new TH1F("chiSq",";#chi^{2};Entries / 2",50,0,100);  
	dHist_photonThetaPi0 = new TH1F("photonThetaPi0",";#theta of #gamma_{1}(#gamma_{2}) GeV;Entries / 0.05 GeV^{2} ",80,0,40); 
	dHist_photonThetaEta = new TH1F("photonThetaEta",";#theta of #gamma_{3}(#gamma_{4}) GeV;Entries / 0.05 GeV^{2} ",80,0,40); 
	dHist_dEdx_momentum = new TH2F("dEdx_momentum",";Proton Momentum Entries / 0.04 GeV/c;dEdx_{CDC} Entries / 3E-7 GeV/cm",100,0,4,100,0,0.00003);
	dHist_protonZ = new TH1F("proton_z",";Proton z (cm);Entries / 1 cm", 50, 40,90); 
	dHist_combosRemaining = new TH1F("combosRemaining",";# Combos / Event Passed Selections",7,0,7);
	dHist_cosThetaVsMpi0eta = new TH2F("cosThetaVsMpi0eta",";M(#eta#pi^{0}) Entries / 0.02 GeV;cos_{hel}(#theta) #eta Entries / 0.04",100,0.5,2.5,50,-1,1);

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

	// This branch is needed for tree_to_amptools to get the right weights
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("DataWeight","NumCombos"); 

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

Bool_t DSelector_etapi::Process(Long64_t locEntry)
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
	        hasPolarizationAngle = dAnalysisUtilities.Get_PolarizationAngle(locRunNumber, locPolarizationAngle);
		if (!hasPolarizationAngle) return kTRUE;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	//dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

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
	int combos_remaining=0;
	float combo_weight=0;
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
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		TLorentzVector locProtonX4 = dProtonWrapper->Get_X4();
		//Step 1
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();
		//Step 2
		TLorentzVector locPhoton3P4 = dPhoton3Wrapper->Get_P4();
		TLorentzVector locPhoton4P4 = dPhoton4Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();
		//Step 2
		TLorentzVector locPhoton3P4_Measured = dPhoton3Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton4P4_Measured = dPhoton4Wrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		// 0 for in-time events, non-zero integer for out-of-time photons
		Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); 
		Int_t locNumOutOfTimeBunchesInTree = 4; //YOU need to specify this number
		//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times 
		//	this number for left + right bunches) 
		Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		// Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); 
		//locAccidentalScalingFactor=1; // Lawrence - testing something
		Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); 
		// Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; 
		if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
			dComboWrapper->Set_IsComboCut(true); 
			continue; 
		} 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE
		//// CONSTRUCT SIDEBAND WEIGHTS
		// A hidden step here that required fitting M(pi0) and M(eta) to extract 
		// 	the peak and widths and associated weightings. Take these number
		// 	as given for now
		float Mpi0=(locPhoton1P4+locPhoton2P4).M();
		float Meta=(locPhoton3P4+locPhoton4P4).M();
		float pi0Mean=0.135881;
		float etaMean=0.548625;
		float pi0Std=0.0076;
		float etaStd=0.0191;
		float pi0_sbweight;
		float eta_sbweight;
		// The signal regions are both +/- 3 sigmas around the peak the left and right sidebands 
		// 	are both 2 sigmas wide a 1 sigma "skip" region is included between the signal
		// 	and sideband regions
		if (Mpi0 > pi0Mean-3*pi0Std && Mpi0 < pi0Mean+3*pi0Std){ pi0_sbweight=1; }
		else if (Mpi0 > pi0Mean+4*pi0Std && Mpi0 < pi0Mean+5.5*pi0Std){ pi0_sbweight=-2.0; }
		else if (Mpi0 > pi0Mean-5.5*pi0Std && Mpi0 < pi0Mean-4*pi0Std){ pi0_sbweight=-2.0; }
		else { pi0_sbweight=0; }
		if (Meta > etaMean-3*etaStd && Meta < etaMean+3*etaStd){ eta_sbweight=1; }
		else if (Meta > etaMean+4*etaStd && Meta < etaMean+6*etaStd){ eta_sbweight=-1.5; }
		else if (Meta > etaMean-6*etaStd && Meta < etaMean-4*etaStd){ eta_sbweight=-1.5; }
		else { eta_sbweight=0; }
		//if ( abs(Mpi0-0.135)<0.025){ pi0_sbweight=1; }
		//else if ( abs(Mpi0-0.135+0.05)<0.025 ){ pi0_sbweight=-0.5; }
		//else if ( abs(Mpi0-0.135-0.05)<0.025 ){ pi0_sbweight=-0.5; }
		//else { pi0_sbweight=0; }
		//if ( abs(Meta-0.548)<0.05 ){ eta_sbweight=1; }
		//else if ( abs(Meta-0.548+0.15)<0.05 ){ eta_sbweight=-0.5; }
		//else if ( abs(Meta-0.548-0.15)<0.05 ){ eta_sbweight=-0.5; }
		//else { eta_sbweight=0; }
		float sbweight=pi0_sbweight*eta_sbweight;
		float weight=sbweight*locHistAccidWeightFactor;
		// Reject combinations with zero weights. Zero weights take up space and do nothing. 
		// 	Worse, it might cause the amptools unbinned likelihood fits to break
		bool bWeight=(weight==0) ? false : true; 

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured + locPhoton3P4_Measured + locPhoton4P4_Measured;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		//dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		//if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
		//	continue;

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

		// fill our weight branch that will be used for tree_to_amptools
		dTreeInterface->Fill_Fundamental<Float_t>("DataWeight", weight, loc_i);

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton3NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton4NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			//dHist_MissingMassSquared->Fill(locMissingMassSquared); // Fills in-time and out-of-time beam photon combos
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		//////////////////////////////////////////////////////////////////
		//  DEFINING SELECTIONS AND INTERESTING VARIABLES
		//////////////////////////////////////////////////////////////////
		//// 1. NEUTRAL SHOWER RELATED (selecting good photons)
		// Low energy photons are more likely to be spurious, require a minimum E
		bool bPhotonE=(locPhoton1P4.E()>0.1)*(locPhoton2P4.E()>0.1)*(locPhoton3P4.E()>0.1)*(locPhoton4P4.E()>0.1); 
		// Working in degrees instead of radians, we remove photons near the beamline (<~2.5) and near the BCAL/FCAL transition (<~11.9, >~10.3)
		float radToDeg=180/3.14159;
		bool bPhotonTheta=
			((locPhoton1P4.Theta()*radToDeg>=2.5 && locPhoton1P4.Theta()*radToDeg<=10.3) || locPhoton1P4.Theta()*radToDeg>=11.9)*
			((locPhoton2P4.Theta()*radToDeg>=2.5 && locPhoton2P4.Theta()*radToDeg<=10.3) || locPhoton2P4.Theta()*radToDeg>=11.9)*
			((locPhoton3P4.Theta()*radToDeg>=2.5 && locPhoton3P4.Theta()*radToDeg<=10.3) || locPhoton3P4.Theta()*radToDeg>=11.9)*
			((locPhoton4P4.Theta()*radToDeg>=2.5 && locPhoton4P4.Theta()*radToDeg<=10.3) || locPhoton4P4.Theta()*radToDeg>=11.9);

		//// 2. CHARGED TRACK RELATED (selecting good protons)
		// protons need some momentum be reconstructed properly
		bool bProtonMomentum=locProtonP4.Vect().Mag()>0.3; 
		// separate proton/pi+ based on energy loss in CDC
		bool bProton_dEdx=dProtonWrapper->Get_dEdx_CDC()>=TMath::Power(10,-6)*(0.9+TMath::Exp(3.0-3.5*(locProtonP4.Vect().Mag()+0.05)/.93827)); 
		//bProton_dEdx=true; // Lawrence - testing something
		// require proton to come from ~ the target region [52,78]centimeters
		bool bProtonZ = 52 <= locProtonX4.Z() && locProtonX4.Z() <= 78;

		//// 3. EXCLUSIVITY RELATED (ensure we select exclusive gp->4gp reaction)
		// Kinematic fit for this tree only attempts to quantify how well conservation of 4-momentum is maintained
		// 	4 NDF in this fit where 13.277 corresponds to a p=0.01
		bool bChiSq=dComboWrapper->Get_ChiSq_KinFit("")<13.277;
		// Unused energy is the sum of unused neutral shower energy not used by this current combo
		// 	require no unused energy meaning the event only has 4 neutral shower hypotheses to limit final state combinatorics
		bool bUnusedEnergy=dComboWrapper->Get_Energy_UnusedShowers()<0.05;
		// No missing particles are expected = no missing mass
		bool bMMsq=abs(locMissingMassSquared)<0.05;
		
		//// 4. KINEMATICS RELATED (extra selections related to kinematics)
		// Select on coherent peak for region of high polarization. The AMPTOOLS fit using Zlm amplitudes will use the polarization
		// 	for extra separation power (will tell us something about the production mechanism)
		bool bBeamEnergy=(locBeamP4.E()>8.2)*(locBeamP4.E()<8.8); 
		float mandelstam_t=-(dTargetP4-locProtonP4).M2();		
		float Mpi0p=(locPhoton1P4+locPhoton2P4+locProtonP4).M();
		float Metap=(locPhoton3P4+locPhoton4P4+locProtonP4).M();
		float Metapi0=(locPhoton1P4+locPhoton2P4+locPhoton3P4+locPhoton4P4).M();
		// Meson production occurs with small-t whereas baryon production occurs with large-t. This analysis cares about mesons
		bool bmandelstamt=(mandelstam_t<0.3)*(mandelstam_t>0.1); 
		// There are a few baryon resonances, the Delta+(1232) being the largest. We can reject it 
		bool bMpi0p=Mpi0p>1.4; 
		// We are interested in the eta+pi0 system where we can see multiple resonances
		// 	in the mass spectra. The spin of the resonance has a direct influence
		// 	on the decay angular distributions of the daughter particles (eta/pi)
		// 	The helicity frame and Gottfried-Jackson frame are both in the rest 
		// 	frame of the etapi system and requires a redefinition of the axes. 
		// 	For this analysis, the helicity frame will be used as the amplitudes 
		// 	that you will use are based on this frame
		TLorentzVector pi0 = locPhoton1P4+locPhoton2P4;
		TLorentzVector eta = locPhoton3P4+locPhoton4P4;
		// First we need to boost to center of mass frame
		TLorentzRotation cmRestBoost( -(locBeamP4+dTargetP4).BoostVector() );
		TLorentzVector pi0_cm = cmRestBoost * pi0; 
		TLorentzVector eta_cm = cmRestBoost * eta; 
		TLorentzVector beam_cm = cmRestBoost * locBeamP4;
		TLorentzVector recoil_cm = cmRestBoost * locProtonP4;
   		TLorentzVector resonance = pi0_cm + eta_cm;
		// We boost again, now to the resonances rest frame
   		TLorentzRotation resRestBoost( -resonance.BoostVector() );
   		TLorentzVector beam_res   = resRestBoost * beam_cm;
   		TLorentzVector recoil_res = resRestBoost * recoil_cm;
   		TLorentzVector eta_res = resRestBoost * eta_cm;
		//// Redefinition of the axes
   		TVector3 z = -1. * recoil_res.Vect().Unit(); // Helicity frame
   		// TVector3 z = beam_res.Vect().Unit(); // Gottfried-Jackson frame
   		// normal to the production plane
   		TVector3 y = (beam_cm.Vect().Unit().Cross(-recoil_cm.Vect().Unit())).Unit();
   		TVector3 x = y.Cross(z);
   		TVector3 angles( (eta_res.Vect()).Dot(x),
   		      (eta_res.Vect()).Dot(y),
   		      (eta_res.Vect()).Dot(z) );
   		float cosTheta = angles.CosTheta();
   		float phi = angles.Phi();
		
		// 5. With the above selections and sidebands stucture, the subtraction near threshold has problems
		// 	The backgrounds that populate the near threshold region are pi0pi0->4g and omega->3gamma
		// 	These backgrounds tend to populate the lower masses in the alternative (not {g1g2~pi0,g3g4~eta}) photon pairs
		float Mg1g3=(locPhoton1P4+locPhoton3P4).M();
		float Mg1g4=(locPhoton1P4+locPhoton4P4).M();
		float Mg2g3=(locPhoton2P4+locPhoton3P4).M();
		float Mg2g4=(locPhoton2P4+locPhoton4P4).M();
		bool bLowMassAltCombo=
		    !(((Mg1g3<0.15)*(Mg2g4<0.15)) || ((Mg1g4<0.15)*(Mg2g3<0.15)) ||
		    ((Mg1g3<0.12)*(Mg2g3<0.12)) || ((Mg1g4<0.12)*(Mg2g4<0.12)));

		//// We can finally multiply all of our selections together to define our final selection criteria. 
		//	Since we have defined SELECTIONS we have to flip the boolean to get a CUT since the 
		//	FLAG used asks if the combo should be cut
		bool selection=bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bProtonZ*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*
				bmandelstamt*bMpi0p*bLowMassAltCombo;

		// We generally do not want to apply a cut on a histogram we are trying to view. To this extent,
		// 	we will just apply all other selections that are not used in the current plot
		// 	i.e. if we want to plot MMSq we will apply all selections EXCEPT for bMMsq
		if (selection){ 
			dHist_Mpi0->Fill((locPhoton1P4+locPhoton2P4).M(),locHistAccidWeightFactor);
			dHist_Meta->Fill((locPhoton3P4+locPhoton4P4).M(),locHistAccidWeightFactor);
		}
		if (!bWeight){ // We do not want to keep any combos with weight 0. Not just a waste of space, can cause problems during fitting
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		if(bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bProtonZ*bChiSq*bUnusedEnergy*bBeamEnergy*bmandelstamt*bMpi0p*bLowMassAltCombo){
			dHist_mmsq->Fill(locMissingMassSquared,weight);}
		if(bPhotonE*bProtonMomentum*bProton_dEdx*bProtonZ*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*bmandelstamt*bMpi0p*bLowMassAltCombo){
			dHist_photonThetaPi0->Fill(locPhoton1P4.Theta()*radToDeg,weight);
			dHist_photonThetaPi0->Fill(locPhoton2P4.Theta()*radToDeg,weight);
			dHist_photonThetaEta->Fill(locPhoton3P4.Theta()*radToDeg,weight);
			dHist_photonThetaEta->Fill(locPhoton4P4.Theta()*radToDeg,weight);}
		if(bPhotonE*bPhotonTheta*bProtonZ*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*bmandelstamt*bMpi0p*bLowMassAltCombo){
			dHist_dEdx_momentum->Fill(locProtonP4.Vect().Mag(),dProtonWrapper->Get_dEdx_CDC(),weight);}
		if(bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*bmandelstamt*bMpi0p*bLowMassAltCombo){
			dHist_protonZ->Fill(locProtonX4.Z(),weight);}
		if(bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bProtonZ*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*bMpi0p*bLowMassAltCombo){
			dHist_t->Fill(mandelstam_t,weight);}
		if(bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bProtonZ*bChiSq*bUnusedEnergy*bMMsq*bBeamEnergy*bmandelstamt*bLowMassAltCombo){
			dHist_Mpi0p->Fill(Mpi0p,weight);}
		if(bPhotonE*bPhotonTheta*bProtonMomentum*bProton_dEdx*bProtonZ*bUnusedEnergy*bMMsq*bBeamEnergy*bmandelstamt*bMpi0p*bLowMassAltCombo){
			dHist_chiSq->Fill(dComboWrapper->Get_ChiSq_KinFit(""),weight);}
		if(!selection)
		{
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		++combos_remaining;
		combo_weight+=weight;
		dHist_rf->Fill(locDeltaT_RF);
		dHist_Metapi_sig->Fill(Metapi0,weight);
		dHist_Metap->Fill(Metap,weight);
		dHist_cosThetaVsMpi0eta->Fill(Metapi0,cosTheta,weight);
		if ( (pi0_sbweight==1)*(eta_sbweight==1) )
			dHist_Metapi_tot->Fill(Metapi0,locHistAccidWeightFactor);
		else
			dHist_Metapi_bkg->Fill(Metapi0,locHistAccidWeightFactor*sbweight);
		if ( (Metapi0<1.56)*(Metapi0>1.04) )
			dHist_Metapi_sig_zoomed->Fill(Metapi0,weight);
		// Since the sideband weights are dependent on the invariant mass distribution of the 
		// 	pi0 and eta candidates we do now want to weight these histograms with 
		// 	the sideband weights.  Resulting histogram would look very weird

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

		//FILL FLAT TREE
		//Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	//Fill_NumCombosSurvivedHists();
	
	// We can manually fill any histograms here that have a single value per event
	// 	where we are now outside the combo loop
	// 	i.e. # combos per event that passed our event selections
	dHist_combosRemaining->Fill(combos_remaining);

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

	return kTRUE;
}

void DSelector_etapi::Finalize(void)
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
