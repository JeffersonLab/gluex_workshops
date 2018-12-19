#include "DSelector_omega_3pi.h"

void DSelector_omega_3pi::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_omega_3pi.root"; //"" for none
	dOutputTreeFileName = "tree_omega_3pi_accepted.root"; //"" for none
	dFlatTreeFileName = "tree_omega_3pi_accepted_flat.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "pi0pippim__B4_FlatTree"; //if blank, default name will be chosen

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
	// // For histogramming the omega mass in omega -> pi+ pi- pi0
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyOmega;
	MyOmega.push_back(PiPlus); MyOmega.push_back(PiMinus); MyOmega.push_back(Pi0); 

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.2, PiPlus, SYS_TOF));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.2, PiMinus, SYS_TOF));

	//MASSES
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Pi0, 1100, 0.08, 0.19, "Pi0"));
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, 0, MyOmega, 1000, 0.4, 1.4, "Omega"));
	dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));
	dAnalysisActions.push_back(new DCutAction_KinFitFOM(dComboWrapper, 1e-5));

	//MASSES
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Pi0, 1100, 0.08, 0.19, "Pi0_KinFit"));
	dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, true, 0, MyOmega, 1000, 0.4, 1.4, "Omega_KinFit"));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyOmega to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyOmega, 1000, 0.4, 1.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	// timing distribution to see prompt and accidental peaks in DeltaT distribution
	dHist_BeamDeltaT = new TH1F("BeamDeltaT", "; t_{Tagger} - t_{RF} (ns)", 200, -20., 20.);

	// beam energy distribution for omega events
	dHist_BeamE = new TH1F("BeamE", "; E_{#gamma} (GeV)", 600, 6.0, 12.0);

	// IMPORTANT: For weighted histograms DON'T USE TH1I!  Instead use TH1F or TH1D to store non-integer weights
	dHist_MissingMassSquared_weighted = new TH1F("MissingMassSquared_weighted", "; Missing Mass Squared (GeV/c^{2})^{2}", 300, -0.1, 0.1);
	dHist_MissingMassSquared_prompt = new TH1F("MissingMassSquared_prompt", "; Missing Mass Squared (GeV/c^{2})^{2}", 300, -0.1, 0.1);
	dHist_MissingMassSquared_accid = new TH1F("MissingMassSquared_accid", "; Missing Mass Squared (GeV/c^{2})^{2}", 300, -0.1, 0.1);
	
	// IMPORTANT: Need to set Sumw2 to use weight squared in histogram for subtraction later.  Can either do this here at histogram definition, or in macro for plotting.
	dHist_MissingMassSquared_prompt->Sumw2();
	dHist_MissingMassSquared_accid->Sumw2();

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
 
	dTreeInterface->Create_Branch_Fundamental<Double_t>("BeamDeltaT"); //fundamental = char, int, float, double, etc.

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)

	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("BeamDeltaT"); //fundamental = char, int, float, double, etc.
}

Bool_t DSelector_omega_3pi::Process(Long64_t locEntry)
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

	//EXAMPLE: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMassSquared;

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
		TLorentzVector locDecayingPi0P4 = dDecayingPi0Wrapper->Get_P4();
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
		TLorentzVector loc3piP4 = locPiPlusP4 + locPiMinusP4 + locDecayingPi0P4;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
		dComboWrapper->Set_IsComboCut(true);


		/******************************************** ACCIDENTAL SUBRACTION INFO *******************************************/
		
		// measured tagger time for combo
		TLorentzVector locBeam_X4_Measured = dComboBeamWrapper->Get_X4_Measured(); 

		// measured RF time for combo
		double locRFTime = dComboWrapper->Get_RFTime_Measured(); 

		// time difference between tagger and RF (corrected for production vertex position relative to target center)
		double locBeamDeltaT = locBeam_X4_Measured.T() - (locRFTime + (locBeam_X4_Measured.Z() - dTargetCenter.Z())/29.9792458); 
		dHist_BeamDeltaT->Fill(locBeamDeltaT);

		// calculate accidental subtraction weight based on time difference 
		double locWeight = 0.; // weight to accidentally subtracted histgorams
		bool locAccid = false; // flag to fill separate prompt and accidental histograms for later subtraction

		if(fabs(locBeamDeltaT) < 0.5*4.008) { // prompt signal recieves a weight of 1
			locWeight = 1.;
			locAccid = false;
		}
                else { // accidentals recieve a weight of 1/# RF bunches included in TTree (8 in this case)
			locWeight = -1./8.;
			locAccid = true;
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Motivation for uniqueness tracking
		//  * Since all 6 measured particles enter in the calculation of the MM^2 they should be tracked uniquely
		//  * All beam photons are uniquely tracked, so both prompt and accidental photons are used to fill the histograms below.
		
		//Uniqueness tracking: Build the map of particles used for the missing mass
		//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMassSquared;
		locUsedThisCombo_MissingMassSquared[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMassSquared[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_MissingMassSquared[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_MissingMassSquared[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMassSquared[Gamma].insert(locPhoton2NeutralID);
		locUsedThisCombo_MissingMassSquared[Proton].insert(locProtonTrackID);
		
		//compare to what's been used so far
		if(locUsedSoFar_MissingMassSquared.find(locUsedThisCombo_MissingMassSquared) == locUsedSoFar_MissingMassSquared.end())
		{
			// fill weighted histogram with ALL beam photons! (resulting histogram is accidental subtracted)
			dHist_MissingMassSquared_weighted->Fill(locMissingMassSquared, locWeight);

			// fill separate histograms for subtracting later
			if(locAccid) dHist_MissingMassSquared_accid->Fill(locMissingMassSquared);
			else dHist_MissingMassSquared_prompt->Fill(locMissingMassSquared);
			
			// register unique combo
			locUsedSoFar_MissingMassSquared.insert(locUsedThisCombo_MissingMassSquared);
		}

		// Simple mass cut to obtain energy distribution (in reality need to fit omega mass for accurate yield)
		if(loc3piP4.M() > 0.7 && loc3piP4.M() < 0.85)
			dHist_BeamE->Fill(locBeamP4.E(), locWeight);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/


		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Double_t>("BeamDeltaT", locBeamDeltaT);

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		//FILL ANY CUSTOM BRANCHES FIRST!!
		dFlatTreeInterface->Fill_Fundamental<Double_t>("BeamDeltaT", locBeamDeltaT);

		//FILL FLAT TREE
		Fill_FlatTree(); //for the active combo

	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

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

void DSelector_omega_3pi::Finalize(void)
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
