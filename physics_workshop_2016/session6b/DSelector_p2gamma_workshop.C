#include "DSelector_p2gamma_workshop.h"

void DSelector_p2gamma_workshop::Init(TTree *locTree)
{
	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_p2gamma.root"; //"" for none
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
	dCutPIDDeltaT->Initialize();

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_Proton_dEdx_P = new TH2I("Proton_dEdx_P", " ;p_{proton} GeV/c; dE/dx (keV/cm)", 250, 0.0, 5.0, 250, 0.0, 25.);

	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_M2gamma = new TH1I("M2gamma", ";M_{#gamma#gamma} (GeV/c^{2})", 600, 0.0, 1.5);
	dHist_phi_t = new TH2I("phi_t", ";-t (GeV/c)^{2}; #phi_{proton}", 100, 0.0, 2.0, 360, -180., 180.);
	dHist_acc_phi_t = new TH2I("acc_phi_t", ";-t (GeV/c)^{2}; #phi_{proton}", 100, 0.0, 2.0, 360, -180., 180.);

	// EXAMPLE CUT PARAMETERS:
        fMinProton_dEdx = new TF1("fMinProton_dEdx", "exp(-1.*[0]*x + [1]) + [2]", 0., 10.);
        fMinProton_dEdx->SetParameters(4.0, 2.5, 1.25);
        dMinKinFitCL = 0.001;
        dMinBeamEnergy = 8.4;
        dMaxBeamEnergy = 9.0;
        dMinPi0Mass = 0.11;
        dMaxPi0Mass = 0.16;

	/***************************************** ADVANCED: CHOOSE BRANCHES TO READ ****************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_p2gamma_workshop::Process(Long64_t locEntry)
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
	set<Int_t> locUsedSoFar_Proton;

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass, locUsedSoFar_Pi0, locUsedSoFar_Angles;

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

		// Reject true pi0 events from sim1 (ie. bggen) since adding signal from AmpTools generator
                if(Get_IsThrownTopology() && dOption.Contains("sim1")) {
                        dComboWrapper->Set_IsComboCut(true);
                        continue;
                }

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locDecayingPi0P4 = dDecayingPi0Wrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		TLorentzVector locDecayingPi0P4_Measured = locPhoton1P4_Measured + locPhoton2P4_Measured;
		TLorentzVector locDecayingPi0P4_KinFit = locPhoton1P4 + locPhoton2P4;

		/**************************************** EXAMPLE: HISTOGRAM KINEMATICS ******************************************/

		dHistComboKinematics->Perform_Action();
		dHistComboPID->Perform_Action();

		/**************************************** EXAMPLE: PID CUT ACTION ************************************************/
		if(!dCutPIDDeltaT->Perform_Action()) {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		// Proton CDC dE/dx histogram and cut 
                double locProton_dEdx_CDC = dProtonWrapper->Get_dEdx_CDC()*1e6;
		if(locUsedSoFar_Proton.find(locProtonTrackID) == locUsedSoFar_Proton.end())
		{
			dHist_Proton_dEdx_P->Fill(locProtonP4.P(), locProton_dEdx_CDC);
			locUsedSoFar_Proton.insert(locProtonTrackID);
		}
                if(locProton_dEdx_CDC < fMinProton_dEdx->Eval(locProtonP4.P())) {
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
                if(dComboWrapper->Get_ConfidenceLevel_KinFit() < dMinKinFitCL) {
                        dComboWrapper->Set_IsComboCut(true);
                        continue;
                }

                // beam energy cut for SDME
                if(locBeamP4.E() < dMinBeamEnergy || locBeamP4.E() > dMaxBeamEnergy) {
                        dComboWrapper->Set_IsComboCut(true);
                        continue;
                }

                // pi0 mass histogram and cut
                map<Particle_t, set<Int_t> > locUsedThisCombo_Pi0;
                locUsedThisCombo_Pi0[Gamma].insert(locPhoton1NeutralID);
                locUsedThisCombo_Pi0[Gamma].insert(locPhoton2NeutralID);
                if(locUsedSoFar_Pi0.find(locUsedThisCombo_Pi0) == locUsedSoFar_Pi0.end())
                {
                        dHist_M2gamma->Fill(locDecayingPi0P4_KinFit.M());
                        locUsedSoFar_Pi0.insert(locUsedThisCombo_Pi0);
                }
                if(locDecayingPi0P4_KinFit.M() < dMinPi0Mass || locDecayingPi0P4_KinFit.M() > dMaxPi0Mass) {
                        dComboWrapper->Set_IsComboCut(true);
                        continue;
                }
		
		// calculate beam RF
		TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
		double locRFTime = dComboWrapper->Get_RFTime_Measured();
		double locDeltaTRF = locBeamX4.T() - (locRFTime + (locBeamX4.Z() - dTargetCenter.Z())/29.9792458);

		// calculate kinematic and angular variables
                double t = (locProtonP4 - dTargetP4).M2();
		double phi = locProtonP4.Phi()*180/TMath::Pi();
		if(phi < -180.) phi += 2*180.;
		if(phi > 180.) phi -= 2*180.;

		map<Particle_t, set<Int_t> > locUsedThisCombo_Angles;
		locUsedThisCombo_Angles[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_Angles[Proton].insert(locProtonTrackID);
		locUsedThisCombo_Angles[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_Angles[Gamma].insert(locPhoton2NeutralID);
		if(locUsedSoFar_Angles.find(locUsedThisCombo_Angles) == locUsedSoFar_Angles.end())
		{
			if(fabs(locDeltaTRF) < 2.004) {
				dHist_phi_t->Fill(-1.*t, phi);
			}
			else 
				dHist_acc_phi_t->Fill(-1.*t, phi);
			locUsedSoFar_Angles.insert(locUsedThisCombo_Angles);
		}
	}

	return kTRUE;
}

void DSelector_p2gamma_workshop::Finalize(void)
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
