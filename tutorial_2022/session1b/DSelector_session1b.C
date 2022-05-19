#include "DSelector_session1b.h"

void DSelector_session1b::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_etapi.acc.root"; //"" for none
	dOutputTreeFileName = "tree_etapi.acc.root"; //"" for none
	dFlatTreeFileName = "tree_etapi_flat.acc.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen
	dSaveDefaultFlatBranches = false; // false: don't save default branches, reduce disk footprint.

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

	std::deque<Particle_t> MyEta;
        MyEta.push_back(Gamma); MyEta.push_back(Gamma);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, Proton, SYS_BCAL));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 2, MyEta, 100, 0.5, 0.6, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	dHistThrownTopologies = new TH1F("hThrownTopologies","hThrownTopologies", 10, -0.5, 9.5);

	vector<TString> locThrownTopologies;
	locThrownTopologies.push_back("data");
	locThrownTopologies.push_back("4#gammap[#pi^{0},#eta]");
	locThrownTopologies.push_back("6#gammap[3#pi^{0}]");		
	locThrownTopologies.push_back("5#gammap[2#pi^{0},#omega]");
	locThrownTopologies.push_back("6#gammap[2#pi^{0},#eta]");
	locThrownTopologies.push_back("4#gammap[2#pi^{0}]");
	locThrownTopologies.push_back("8#gammap[4#pi^{0},#eta]");
	locThrownTopologies.push_back("6#gamma#pi^{#plus}#pi^{#minus}p[3#pi^{0}]");		
	locThrownTopologies.push_back("4#gamma#pi^{#plus}#pi^{#minus}p[2#pi^{0}]");
	locThrownTopologies.push_back("4#gamma#pi^{#plus}#pi^{#minus}p[#pi^{0},#eta]");		
	locThrownTopologies.push_back("8#gammap[3#pi^{0},#eta]");
	locThrownTopologies.push_back("3#gammap[#pi^{0},#omega]");
	for(uint i=0; i<locThrownTopologies.size(); i++) {
		dHistInvariantMass_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hInvariantMass_ThrownTopology_%d", i),Form("Invariant Mass Topology: %s; M_{#eta#pi^{0}} (GeV)", locThrownTopologies[i].Data()), 200, 0.5, 2.0);
		dHist2DInvariantMass_ThrownTopology[locThrownTopologies[i]] = new TH2I(Form("h2DInvariantMass_ThrownTopology_%d", i),Form("2D Invariant Mass: Topology %s; M_{#gamma#gamma} (GeV); M_{#gamma#gamma} (GeV)", locThrownTopologies[i].Data()), 200, 0.0, 0.7, 200, 0.0, 0.7);
		dHist2DInvariantMassShuffled_ThrownTopology[locThrownTopologies[i]] = new TH2I(Form("h2DInvariantMassShuffled_ThrownTopology_%d", i),Form("2D Invariant Mass Shuffled: Topology %s; M_{#gamma#gamma} (GeV); M_{#gamma#gamma} (GeV)", locThrownTopologies[i].Data()), 200, 0.0, 0.7, 200, 0.0, 0.7);

		dHistProtonPi0Mass_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hProtonPi0Mass_ThrownTopology_%d", i),Form("p#pi^{0} Mass: Topology %s; M_{p#pi^{0}} (GeV)", locThrownTopologies[i].Data()), 100, 1.0, 3.0);
		dHistProtonEtaMass_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hProtonEtaMass_ThrownTopology_%d", i),Form("p#eta Mass: Topology %s; M_{p#eta} (GeV)", locThrownTopologies[i].Data()), 100, 1.0, 3.0);

		dHistKinFitChi2_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hKinFitChi2_ThrownTopology_%d", i),Form("Kinematic Fit #Chi^{2}/NDF: Topology %s; #Chi^{2}/NDF", locThrownTopologies[i].Data()), 200, 0.0, 100.0);
		dHistUnusedShowers_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hUnusedShowers_ThrownTopology_%d", i),Form("Number of unused showers: Topology %s; # unused showers", locThrownTopologies[i].Data()), 10, 0, 10);
		dHistUnusedShowerEnergy_ThrownTopology[locThrownTopologies[i]] = new TH1I(Form("hUnusedShowerEnergy_ThrownTopology_%d", i),Form("Unused shower energy: Topology %s; Unused shower energy (GeV)", locThrownTopologies[i].Data()), 100, 0.0, 10.0);
		
	}

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("test"); //fundamental = char, int, float, double, etc.
	//dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	//dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	//dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

}

Bool_t DSelector_session1b::Process(Long64_t locEntry)
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

	/********************************************* RESET ANALYSIS ACTIONS ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	/************************************************* PARSE THROWN TOPOLOGY ***************************************/
	TString locThrownTopology = Get_ThrownTopologyString();
	if(!dIsMC) locThrownTopology = "data";

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

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locProtonP4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured + locPhoton3P4_Measured + locPhoton4P4_Measured;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		// Fill histogram of thrown topologies
		dHistThrownTopologies->Fill(locThrownTopology.Data(),1);

		TLorentzVector locPi0P4 = locPhoton1P4 + locPhoton2P4;
		TLorentzVector locEtaP4 = locPhoton3P4 + locPhoton4P4;
		TLorentzVector locPi0EtaP4 = locPi0P4 + locEtaP4;
		TLorentzVector locPi0ProtonP4 = locPi0P4 + locProtonP4;
		TLorentzVector locEtaProtonP4 = locEtaP4 + locProtonP4;
		pair<TLorentzVector,TLorentzVector> locShuffledP4[2];
		locShuffledP4[0] = make_pair(locPhoton1P4 + locPhoton3P4, locPhoton2P4 + locPhoton4P4);
		locShuffledP4[1] = make_pair(locPhoton1P4 + locPhoton4P4, locPhoton2P4 + locPhoton3P4);

		double reducedChiSq = dComboWrapper->Get_ChiSq_KinFit()/dComboWrapper->Get_NDF_KinFit();
		double energyUnusedShowers = dComboWrapper->Get_Energy_UnusedShowers();
		double numUnusedShowers = dComboWrapper->Get_NumUnusedShowers();
		
		if(dHistInvariantMass_ThrownTopology.find(locThrownTopology) != dHistInvariantMass_ThrownTopology.end()) {
			dHistKinFitChi2_ThrownTopology[locThrownTopology]->Fill(reducedChiSq,locHistAccidWeightFactor);
			if(reducedChiSq > 5.0) {
				dComboWrapper->Set_IsComboCut(true); 
				continue;
			}

			dHistUnusedShowers_ThrownTopology[locThrownTopology]->Fill(numUnusedShowers,locHistAccidWeightFactor);
			dHistUnusedShowerEnergy_ThrownTopology[locThrownTopology]->Fill(energyUnusedShowers,locHistAccidWeightFactor);

			dHist2DInvariantMass_ThrownTopology[locThrownTopology]->Fill(locPi0P4.M(),locEtaP4.M(),locHistAccidWeightFactor);
			dHist2DInvariantMassShuffled_ThrownTopology[locThrownTopology]->Fill(locShuffledP4[0].first.M(), locShuffledP4[0].second.M(),locHistAccidWeightFactor);
			dHist2DInvariantMassShuffled_ThrownTopology[locThrownTopology]->Fill(locShuffledP4[1].first.M(), locShuffledP4[1].second.M(),locHistAccidWeightFactor);
			
			dHistProtonPi0Mass_ThrownTopology[locThrownTopology]->Fill(locPi0ProtonP4.M(),locHistAccidWeightFactor);
			dHistProtonEtaMass_ThrownTopology[locThrownTopology]->Fill(locEtaProtonP4.M(),locHistAccidWeightFactor);

			dHistInvariantMass_ThrownTopology[locThrownTopology]->Fill(locPi0EtaP4.M(),locHistAccidWeightFactor);	
		}

		

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		//FILL ANY CUSTOM BRANCHES FIRST!!
		Double_t test = locPi0EtaP4.M();
		dFlatTreeInterface->Fill_Fundamental<Double_t>("test", test);

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

void DSelector_session1b::Finalize(void)
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
