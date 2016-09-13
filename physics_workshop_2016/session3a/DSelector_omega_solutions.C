#include "DSelector_omega_solutions.h"

void DSelector_omega_solutions::Init(TTree *locTree)
{
	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "myfile.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none

	//DO THIS NEXT
	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	//THEN THIS
	Get_ComboWrappers();

	/******************************************** EXAMPLE USER INITIALIZATION *******************************************/

	//DO WHATEVER YOU WANT HERE

	//EXAMPLE HISTOGRAM ACTIONS:
	dHistComboKinematics = new DHistogramAction_ParticleComboKinematics(dComboWrapper, false); //false: use measured data
	dHistComboPID = new DHistogramAction_ParticleID(dComboWrapper, false); //false: use measured data
	//change binning here
	dHistComboKinematics->Initialize();
	dHistComboPID->Initialize();

	//EXAMPLE CUT ACTIONS:
	//below: false: measured data, value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	dCutPIDDeltaT = new DCutAction_PIDDeltaT(dComboWrapper, false, 2.0, Unknown, SYS_NULL);

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 800, -0.08, 0.08);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_KinFitConLev = new TH1I("KinFitConLev", ";Kinematic Fit Confidence Level", 500, 0.0, 1.0);
	dHist_Pi0Mass = new TH1I("Pi0Mass", ";#gamma#gamma Invariant Mass", 680, 0.05, 0.22);
	dHist_OmegaMass_Measured = new TH1I("OmegaMass_Measured", ";#pi^{#plus}#pi^{#minus}#gamma#gamma Invariant Mass", 300, 0.5, 1.1);
	dHist_OmegaMass_KinFit = new TH1I("OmegaMass_KinFit", ";#pi^{#plus}#pi^{#minus}#pi^{0} Invariant Mass", 300, 0.5, 1.1);

	/***************************************** ADVANCED: CHOOSE BRANCHES TO READ ****************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_omega_solutions::Process(Long64_t locEntry)
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

	/**************************************** SETUP AUTOMATIC UNIQUENESS TRACKING ***************************************/

	//Reset uniqueness tracking for each action
	dHistComboKinematics->Reset_NewEvent();
	dHistComboPID->Reset_NewEvent();

	//INSERT OTHER USER ACTIONS HERE

	/***************************************** SETUP MANUAL UNIQUENESS TRACKING *****************************************/

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
	set<set<Int_t> > locUsedSoFar_Pi0Mass;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_OmegaMass;
	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

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
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPiPlusTrackID = dPiPlusWrapper->Get_TrackID();
		Int_t locPiMinusTrackID = dPiMinusWrapper->Get_TrackID();

		//Step 2
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		//Step 2
		TLorentzVector locDecayingPi0P4 = dDecayingPi0Wrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		//Step 2
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPiPlusP4_Measured + locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		// DO YOUR STUFF HERE
		TLorentzVector locPi0P4_Measured = locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locOmegaP4_Measured = locPi0P4_Measured + locPiPlusP4_Measured + locPiMinusP4_Measured;
		TLorentzVector locOmegaP4 = locDecayingPi0P4 + locPiPlusP4 + locPiMinusP4;

		/**************************************** EXAMPLE: HISTOGRAM KINEMATICS ******************************************/

		dHistComboKinematics->Perform_Action();
		dHistComboPID->Perform_Action();

		/**************************************** EXAMPLE: PID CUT ACTION ************************************************/
/*
		if(!dCutPIDDeltaT->Perform_Action()) {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
*/
		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E());
			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************** HIST, CUT KINFIT CONFIDENCE LEVEL ****************************************/

		double locKinFitConLev = dComboWrapper->Get_ConfidenceLevel_KinFit();
		if(locKinFitConLev < 5.73303E-7)
			continue; //could also mark combo as cut, then save cut results to a new TTree
		dHist_KinFitConLev->Fill(locKinFitConLev); //no need to track uniquness: unique for each combo (uses all particles)

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared);
			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//	continue; //could also mark combo as cut, then save cut results to a new TTree

		/***************************************** HISTOGRAM Pi0 INVARIANT MASS ******************************************/

		double locPi0Mass = locPi0P4_Measured.M();

		//Uniqueness tracking:
		set<Int_t> locUsedThisCombo_Pi0Mass;
		locUsedThisCombo_Pi0Mass.insert(locPhoton1NeutralID);
		locUsedThisCombo_Pi0Mass.insert(locPhoton2NeutralID);

		//compare to what's been used so far
		if(locUsedSoFar_Pi0Mass.find(locUsedThisCombo_Pi0Mass) == locUsedSoFar_Pi0Mass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_Pi0Mass->Fill(locPi0Mass);
			locUsedSoFar_Pi0Mass.insert(locUsedThisCombo_Pi0Mass);
		}

		/**************************************** HISTOGRAM OMEGA INVARIANT MASS *****************************************/

		double locOmegaMass_Measured = locOmegaP4_Measured.M();
		double locOmegaMass_KinFit = locOmegaP4.M();

		//Uniqueness tracking:
		map<Particle_t, set<Int_t> > locUsedThisCombo_OmegaMass;
		locUsedThisCombo_OmegaMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_OmegaMass[Gamma].insert(locPhoton2NeutralID);
		locUsedThisCombo_OmegaMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_OmegaMass[PiMinus].insert(locPiMinusTrackID);

		//compare to what's been used so far
		if(locUsedSoFar_OmegaMass.find(locUsedThisCombo_OmegaMass) == locUsedSoFar_OmegaMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_OmegaMass_Measured->Fill(locOmegaMass_Measured);
			dHist_OmegaMass_KinFit->Fill(locOmegaMass_KinFit);
			locUsedSoFar_OmegaMass.insert(locUsedThisCombo_OmegaMass);
		}
	}

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

	//Loop over charged track hypotheses (all are present, even those not in any combos)
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses (all are present, even those not in any combos)
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
		FillOutputTree();
*/

	return kTRUE;
}

void DSelector_omega_solutions::Finalize(void)
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
