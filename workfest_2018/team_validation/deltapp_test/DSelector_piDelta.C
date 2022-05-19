#include "DSelector_piDelta.h"

void DSelector_piDelta::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	// dOutputFileName = "hello.root"; //"" for none
	// dOutputTreeFileName = "asdf.root"; //"" for none
	dFlatTreeFileName = "PiDeltaDSelectorOut.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "pipl_pim_Tree"; //if blank, default name will be chosen

	//JZEDIT: FLAGS n stuff
	jz_output_4vectors = true;
	jz_use_measured = true;
	jz_get_thrown_info = false;
	jz_counter = 0;
	
	
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

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.4, 9.05));

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);

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
	
		//JZ EDIT: Register 4-vector branches
	if(jz_output_4vectors) {
		
		// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("test");
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
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("PiPlus_PIDhit");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("PiMinus_PIDhit");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("Proton_PIDhit");
		
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("pipl_pim_m");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("pipl_p_m");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("MM2");
		
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("is_perp");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("is_para");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("is_45");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("is_135");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("is_amo");
		
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("rf_deltaT");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("beamE");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("kinfit_CL");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("kinfit_chi2_ndf");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vertex_z");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("vertex_r");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("NExtraTracks");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("ExtraShowerE");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("NCombosPluginLvl");
		
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("Run");
		dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("Event");
	
	if(jz_get_thrown_info) {
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_px_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_py_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_pz_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_E_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_px_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_py_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_pz_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_E_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_px_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_py_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_pz_th");
		dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_E_th");
	}
	
	
	
	}
	
	
	
	
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
}

Bool_t DSelector_piDelta::Process(Long64_t locEntry)
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
	// cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	//TLorentzVector locProductionX4 = Get_X4_Production();
	if(jz_counter%5000000==0) cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl; // JZEDIT
	
	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
		jz_PolDirection = -1;
		dAnalysisUtilities.Get_PolarizationAngle(locRunNumber,jz_PolDirection);
	}

	
	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();

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

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured(); //JZEDIT: get beam photon timing info
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
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
		dTreeInterface->Fill_Fundamental<Double_t>("my_combo_array", -2*loc_i, loc_i);
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

		//enums for detector systems
		// SYS_NULL       = 0x0000,
		// SYS_CDC        = 0x0001,
		// SYS_FDC        = 0x0002,
		// SYS_BCAL       = 0x0004,
		// SYS_TOF        = 0x0008,
		// SYS_CHERENKOV  = 0x0010,
		// SYS_FCAL       = 0x0020,
		// SYS_UPV        = 0x0040,
		// SYS_TAGM       = 0x0080,
		// SYS_START      = 0x0100,
		// SYS_DIRC       = 0x0200,
		// SYS_CCAL       = 0x0400,
		// SYS_TAGH       = 0x0800,
		// SYS_RF         = 0x1000,
		// SYS_PS         = 0x2000,
		// SYS_PSC        = 0x4000,
		// SYS_FMWPC      = 0x8000,
		// SYS_TPOL       = 0x10000,
		// SYS_TAC        = 0x20000
		
		
		//Loop over throwns
		TLorentzVector my_p4_pipl_th;
		TLorentzVector my_p4_pim_th;
		TLorentzVector my_p4_proton_th;
		if(jz_get_thrown_info) {
			for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
			{
				//Appears to match the order of the ascii file made by genr8
				// i= 0 pi-
				// i = 1 proton
				// i = 2 pi+
				dThrownWrapper->Set_ArrayIndex(loc_i);
				if(loc_i==0) my_p4_pim_th  = dThrownWrapper->Get_P4();
				if(loc_i==1) my_p4_proton_th  = dThrownWrapper->Get_P4();
				if(loc_i==2) my_p4_pipl_th  = dThrownWrapper->Get_P4();
				//Do stuff with the wrapper here ...
			}
		}
		
		DetectorSystem_t locSystemPiPlus = dPiPlusWrapper->Get_Detector_System_Timing();
		DetectorSystem_t locSystemPiMinus = dPiMinusWrapper->Get_Detector_System_Timing();
		DetectorSystem_t locSystemProton = dProtonWrapper->Get_Detector_System_Timing();
		
		
		// cout << "Number of combos: " << Get_NumCombos() << endl;
		// cout << "Extra shower E: " << dComboWrapper->Get_Energy_UnusedShowers() << endl;
		// cout << "Unused track hypotheses: " << Get_NumChargedHypos()-3 << endl;
		// cout << "Polarized flag: " <<  dIsPolarizedFlag << endl;
		// cout << "Polarization direction: " << jz_PolDirection << endl;
		// cout << "Pi+ detector system: " << locSystemPiPlus << endl;
		// cout << "Pi- detector system: " << locSystemPiMinus << endl;
		// cout << "Proton detector system: " << locSystemProton << endl;
		// cout << endl;
		
		
		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared);
			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		//JZEDIT
		// const DChargedTrackHypothesis* locProtonHypothesis = static_cast<const DChargedTrackHypothesis*>(locParticles[loc_i]);

		
		
		Double_t kinfit_CL = dComboWrapper->Get_ConfidenceLevel_KinFit();
		Double_t kinfit_chi2 = dComboWrapper->Get_ChiSq_KinFit();
		Double_t kinfit_ndf = dComboWrapper->Get_NDF_KinFit();
		Double_t kinfit_chi2_ndf = kinfit_chi2/kinfit_ndf;
		// cout << "kinfit CL: " << kinfit_CL << endl;
		// cout << "kinfit kinfit_chi2: " << kinfit_chi2 << endl;
		// cout << "kinfit kinfit_ndf: " << kinfit_ndf << endl;
		// cout << "kinfit kinfit_chi2_ndf: " << kinfit_chi2_ndf << endl;
		Double_t rf_timing = locBeamX4_Measured.T() - (dComboWrapper->Get_RFTime_Measured() + (locBeamX4_Measured.Z()- dComboWrapper->Get_TargetCenter().Z())/29.9792458 );
		Int_t is_para = 0;
		Int_t is_perp = 0;
		Int_t is_45 = 0;
		Int_t is_135 = 0;
		Int_t is_amo  = 0;
		
		TLorentzVector my_p4_pipl;
		TLorentzVector my_p4_pim;
		TLorentzVector my_p4_proton;
		
		if(jz_use_measured) {
			my_p4_pipl = locPiPlusP4_Measured;
			my_p4_pim = locPiMinusP4_Measured;
			my_p4_proton = locProtonP4_Measured;
		}
		else {
			my_p4_pipl = locPiPlusP4;
			my_p4_pim = locPiMinusP4;
			my_p4_proton = locProtonP4;
		}
		
		
		//Get Polarization info
		if(!dIsPolarizedFlag) is_amo=1;
		Int_t most_significant_run_digit = TMath::Floor( locRunNumber /10000.);
		if(dIsPolarizedFlag&&dIsPARAFlag && most_significant_run_digit==1 ) is_para=1; //Spring 2016 data
		if(dIsPolarizedFlag&&!dIsPARAFlag && most_significant_run_digit==1 ) is_perp=1; //Spring 2016 data
		if(most_significant_run_digit==3) { //Spring 2017 data
			if(!dIsPolarizedFlag) is_amo=1;
			if(dIsPolarizedFlag &&  jz_PolDirection==0) is_para=1;
			if(dIsPolarizedFlag && jz_PolDirection==90) is_perp=1;
			if(dIsPolarizedFlag &&  jz_PolDirection==45) is_45=1;
			if(dIsPolarizedFlag &&  jz_PolDirection==135) is_135=1;
		}

		
		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		//If passing cuts, then fill
		// if(8.15<locBeamP4.E() && locBeamP4.E()<8.85 && (my_p4_pipl+my_p4_pim).M()>0.9 && jz_output_4vectors) {
        if( (my_p4_pipl+my_p4_pim).M()>0.9 && jz_output_4vectors) {
		//if(jz_output_4vectors) {
		// if(jz_output_4vectors) {			
			
			// dFlatTreeInterface->Fill_Fundamental<Double_t>("test",0.0);
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_px",my_p4_pipl.Px());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_py",my_p4_pipl.Py());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_pz",my_p4_pipl.Pz());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_E",my_p4_pipl.E());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_px",my_p4_pim.Px());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_py",my_p4_pim.Py());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_pz",my_p4_pim.Pz());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_E",my_p4_pim.E());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_px",my_p4_proton.Px());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_py",my_p4_proton.Py());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_pz",my_p4_proton.Pz());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_E",my_p4_proton.E());
			dFlatTreeInterface->Fill_Fundamental<Int_t>("PiPlus_PIDhit",locSystemPiPlus);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("PiMinus_PIDhit",locSystemPiMinus);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("Proton_PIDhit",locSystemProton);
						
			dFlatTreeInterface->Fill_Fundamental<Double_t>("pipl_pim_m",(my_p4_pipl+my_p4_pim).M());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("pipl_p_m",(my_p4_pipl+my_p4_proton).M());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("MM2",locMissingMassSquared);
			
			dFlatTreeInterface->Fill_Fundamental<Int_t>("is_perp",is_perp);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("is_para",is_para);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("is_45",is_45);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("is_135",is_135);
			dFlatTreeInterface->Fill_Fundamental<Int_t>("is_amo",is_amo);
			
			dFlatTreeInterface->Fill_Fundamental<Double_t>("rf_deltaT",rf_timing);
			dFlatTreeInterface->Fill_Fundamental<Double_t>("beamE",locBeamP4.E());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("kinfit_CL",kinfit_CL);
			dFlatTreeInterface->Fill_Fundamental<Double_t>("kinfit_chi2_ndf",kinfit_chi2_ndf);
			dFlatTreeInterface->Fill_Fundamental<Double_t>("vertex_z",locBeamX4_Measured.Z());
			dFlatTreeInterface->Fill_Fundamental<Double_t>("vertex_r",locBeamX4_Measured.Pt());
			dFlatTreeInterface->Fill_Fundamental<Int_t>("NExtraTracks",Get_NumChargedHypos()-3);
			dFlatTreeInterface->Fill_Fundamental<Double_t>("ExtraShowerE",dComboWrapper->Get_Energy_UnusedShowers());
			dFlatTreeInterface->Fill_Fundamental<Int_t>("NCombosPluginLvl",Get_NumCombos());
			
			dFlatTreeInterface->Fill_Fundamental<Int_t>("Run",Get_RunNumber());
			dFlatTreeInterface->Fill_Fundamental<Int_t>("Event",Get_EventNumber());
			
			if(jz_get_thrown_info) {
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_px_th",my_p4_pipl_th.Px());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_py_th",my_p4_pipl_th.Py());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_pz_th",my_p4_pipl_th.Pz());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiPlus_E_th",my_p4_pipl_th.E());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_px_th",my_p4_pim_th.Px());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_py_th",my_p4_pim_th.Py());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_pz_th",my_p4_pim_th.Pz());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("PiMinus_E_th",my_p4_pim_th.E());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_px_th",my_p4_proton_th.Px());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_py_th",my_p4_proton_th.Py());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_pz_th",my_p4_proton_th.Pz());
				dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_E_th",my_p4_proton_th.E());
			}
			
			// cout << "PiPlus_px: " << my_p4_pipl.Px() << endl;
			// cout << "PiPlus_py: " << my_p4_pipl.Py() << endl;
			// cout << "PiPlus_pz: " << my_p4_pipl.Pz() << endl;
			// cout << "PiPlus_E: " << my_p4_pipl.E() << endl;
			// cout << "PiMinus_px: " << my_p4_pim.Px() << endl;
			// cout << "PiMinus_py: " << my_p4_pim.Py() << endl;
			// cout << "PiMinus_pz: " << my_p4_pim.Pz() << endl;
			// cout << "PiMinus_E: " << my_p4_pim.E() << endl;
			// cout << "Proton_px: " << my_p4_proton.Px() << endl;
			// cout << "Proton_py: " << my_p4_proton.Py() << endl;
			// cout << "Proton_pz: " << my_p4_proton.Pz() << endl;
			// cout << "Proton_E: " << my_p4_proton.E() << endl;
			
			// cout << "is_perp: " << is_perp << endl;
			// cout << "is_para: " << is_para << endl;
			// cout << "is_45: " << is_45 << endl;
			// cout << "is_135: " << is_135 << endl;
			// cout << "is_amo: " << is_amo << endl;
			
			//FILL FLAT TREE
			Fill_FlatTree(); //for the active combo
			jz_counter++;
			// cout << "GOT HERE " << endl;
			// cout << "count is at: " << jz_counter << endl;
		}
		
		
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
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/

	//Thrown beam: just use directly
	// if(dThrownBeam != NULL)
		// double locEnergy = dThrownBeam->Get_P4().E();


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

void DSelector_piDelta::Finalize(void)
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
