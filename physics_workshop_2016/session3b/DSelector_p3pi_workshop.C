#include "DSelector_p3pi_workshop.h"

void DSelector_p3pi_workshop::Init(TTree *locTree)
{
	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_p3pi_omega.root"; //"" for none
	dOutputTreeFileName = "tree_p3pi_omega.root"; //"" for none
	
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

	//EXAMPLE HISTOGRAM ACTIONS
	dHistComboKinematics = new DHistogramAction_ParticleComboKinematics(dComboWrapper, dTargetCenter.Z(), true); //true: use measured data
	dHistComboPID = new DHistogramAction_ParticleID(dComboWrapper, true); //true: use measured data
	dHistComboPID_KinFit = new DHistogramAction_ParticleID(dComboWrapper, false, "KinFit"); //true: use measured data
	//change binning here
	dHistComboKinematics->Initialize();
	dHistComboPID->Initialize();
	dHistComboPID_KinFit->Initialize();

	//EXAMPLE CUT ACTIONS
	dCutPIDDeltaT = new DCutAction_PIDDeltaT(dComboWrapper, true, 2, Unknown, SYS_NULL); //true: use measured data

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 400, -0.08, 0.08);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_M2gamma = new TH1I("M2gamma", ";M_{2#gamma} (GeV/c^{2})", 400, 0.05, 0.22);
	dHist_M3pi = new TH1I("M3pi", ";M_{#pi^{+}#pi^{-}#pi^{0}} (GeV/c^{2})", 600, 0.5, 1.1);

	/***************************************** ADVANCED: CHOOSE BRANCHES TO READ ****************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_p3pi_workshop::Process(Long64_t locEntry)
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
	dHistComboPID_KinFit->Reset_NewEvent();

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

	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Pi0;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Omega;

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

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPiPlusP4_Measured + locPiMinusP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locPi0P4 = locPhoton1P4 + locPhoton2P4;
		TLorentzVector locOmegaP4 = locPiPlusP4 + locPiMinusP4 + locPhoton1P4 + locPhoton2P4;

		/**************************************** EXAMPLE: HISTOGRAM KINEMATICS ******************************************/

		dHistComboKinematics->Perform_Action();
		dHistComboPID->Perform_Action();
		dHistComboPID_KinFit->Perform_Action();

		// veto combos based on cut action
		if(!dCutPIDDeltaT->Perform_Action()) {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

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

		// kinematic fit CL cut
		if(dComboWrapper->Get_ConfidenceLevel_KinFit() < 0.1) {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		// pi0 mass histogram and cut
		map<Particle_t, set<Int_t> > locUsedThisCombo_Pi0Mass;
		locUsedThisCombo_Pi0Mass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_Pi0Mass[Gamma].insert(locPhoton2NeutralID);
		if(locUsedSoFar_Pi0.find(locUsedThisCombo_Pi0Mass) == locUsedSoFar_Pi0.end())
		{
			dHist_M2gamma->Fill(locPi0P4.M());
			locUsedSoFar_Pi0.insert(locUsedThisCombo_Pi0Mass);
		}

		if(locPi0P4.M() < 0.11 || locPi0P4.M() > 0.16) {
			dComboWrapper->Set_IsComboCut(true);
			continue; 
		}

		// pi0 mass histogram and cut
		map<Particle_t, set<Int_t> > locUsedThisCombo_OmegaMass;
		locUsedThisCombo_OmegaMass[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_OmegaMass[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_OmegaMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_OmegaMass[Gamma].insert(locPhoton2NeutralID);
		if(locUsedSoFar_Omega.find(locUsedThisCombo_Pi0Mass) == locUsedSoFar_Omega.end())
		{
			dHist_M3pi->Fill(locOmegaP4.M());
			locUsedSoFar_Omega.insert(locUsedThisCombo_OmegaMass);
		}
	}


        /************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
        Bool_t locIsEventCut = true;
        for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
                //Set branch array indices for combo and all combo particles
                dComboWrapper->Set_ComboIndex(loc_i);
                
                // Is used to indicate when combos have been cut
                if(!dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
                        locIsEventCut = false; // Combo has been cut previously
        }
        if(!locIsEventCut && dOutputTreeFileName != "") 
                FillOutputTree();
	
	return kTRUE;
}

void DSelector_p3pi_workshop::Finalize(void)
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
