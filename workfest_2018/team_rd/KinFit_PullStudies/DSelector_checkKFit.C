#include "DSelector_checkKFit.h"
#include "cut_tools.h"
#include "angularScan_tools.h"

void DSelector_checkKFit::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "checkKFitVtx.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = ""; //output flat tree (one combo per tree entry), "" for none
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

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	//dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	//dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//BEAM ENERGY
	//dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.4, 9.05));

	//KINEMATICS
	//dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	missingP_vs_missingE = new TH2I("missingP_vs_missingE","",1000,-3,3,1000,0,3);
	EM_balance = new TH1I("EM_balance","",1000,-1,1);
	
	TString pName[3]  = {"PiPlus", "PMinus", "Proton"};
	TString pullName[6] = {"_PxPull_vs_Prob", "_PyPull_vs_Prob", "_PzPull_vs_Prob", "_VxPull_vs_Prob", "_VyPull_vs_Prob", "_VzPull_vs_Prob"};
	TString histname;
	
	for(Int_t a=0;a<3;a++){
		for(Int_t b=0;b<6;b++){
		    histname = pName[a] + pullName[b];
		    
		    Pulls_vs_Prob[a][b] = new TH2I(histname,"",1000,0,1.01,1000,-25,25);	
		}
	}
	
	MM_vs_IM = new TH2I("MM_vs_IM","",1000,0,2,1000,0,2);
	
	probDist = new TH1I("probDist","",1000,0,1.01);
	MMALL = new TH1I("MMALL","",1000,-0.07,0.07);
	
	
	probDistCut = new TH1I("probDistCut","",1000,0,1.01);
	missingP_vs_missingE_Cut = new TH2I("missingP_vs_missingE_Cut","",1000,-3,3,1000,0,3);
	MM_vs_IM_Cut = new TH2I("MM_vs_IM_Cut","",1000,0,2,1000,0,2);
	EM_balance_Cut = new TH1I("EM_balance_Cut","",1000,-1,1);
	MMALL_Cut = new TH1I("MMALL_Cut","",1000,-0.07,0.07);
	
	probDistCut2 = new TH1I("probDistCut2","",1000,0,1.01);
	missingP_vs_missingE_Cut2 = new TH2I("missingP_vs_missingE_Cut2","",1000,-3,3,1000,0,3);
	MM_vs_IM_Cut2 = new TH2I("MM_vs_IM_Cut2","",1000,0,2,1000,0,2);
	EM_balance_Cut2 = new TH1I("EM_balance_Cut2","",1000,-1,1);
	MMALL_Cut2 = new TH1I("MMALL_Cut2","",1000,-0.07,0.07);
	
	Theta_vs_phi_PiPlus = new TH2I("Theta_vs_Phi_PiPlus","",1000,-180,180,1000,0,180);
	Theta_vs_phi_PiMinus = new TH2I("Theta_vs_Phi_PiMinus","",1000,-180,180,1000,0,180);
	Theta_vs_phi_Prot = new TH2I("Theta_vs_Phi_Prot","",1000,-180,180,1000,0,180);
	
	TString histName;
	 
	 for(Int_t i=0;i<NThetaScans;i++){
        for(Int_t j=0;j<NPhiScans;j++){
			//Pulls for pi+:
            histName = Form("PiPlus_PullsPx_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullPx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullPx_vs_Prob[i][j],i,j,"pipPx");
            histName = Form("PiPlus_PullsPy_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullPy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullPy_vs_Prob[i][j],i,j,"pipPy");
            histName = Form("PiPlus_PullsPz_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullPz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullPz_vs_Prob[i][j],i,j,"pipPz");
            histName = Form("PiPlus_PullsVx_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullVx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullVx_vs_Prob[i][j],i,j,"pipVx");
            histName = Form("PiPlus_PullsVy_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullVy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullVy_vs_Prob[i][j],i,j,"pipVy");
            histName = Form("PiPlus_PullsVz_vs_Prob_Theta%i_Phi%i",i,j);
            PiPlus_PullVz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiPlus_PullVz_vs_Prob[i][j],i,j,"pipVz");
            
            //Pulls for pi-:
            histName = Form("PiMinus_PullsPx_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullPx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiMinus_PullPx_vs_Prob[i][j],i,j,"pimPx");
            histName = Form("PiMinus_PullsPy_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullPy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
             readInHists(PiMinus_PullPy_vs_Prob[i][j],i,j,"pimPy");
            histName = Form("PiMinus_PullsPz_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullPz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiMinus_PullPz_vs_Prob[i][j],i,j,"pimPz");
            histName = Form("PiMinus_PullsVx_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullVx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
             readInHists(PiMinus_PullVx_vs_Prob[i][j],i,j,"pimVx");
            histName = Form("PiMinus_PullsVy_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullVy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiMinus_PullVy_vs_Prob[i][j],i,j,"pimVy");
            histName = Form("PiMinus_PullsVz_vs_Prob_Theta%i_Phi%i",i,j);
            PiMinus_PullVz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(PiMinus_PullVz_vs_Prob[i][j],i,j,"pimVz");
            
            //Pulls for p:
            histName = Form("Prot_PullsPx_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullPx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullPx_vs_Prob[i][j],i,j,"pPx");
            histName = Form("Prot_PullsPy_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullPy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullPy_vs_Prob[i][j],i,j,"pPy");
            histName = Form("Prot_PullsPz_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullPz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullPz_vs_Prob[i][j],i,j,"pPz");
            histName = Form("Prot_PullsVx_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullVx_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullVx_vs_Prob[i][j],i,j,"pVx");
            histName = Form("Prot_PullsVy_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullVy_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullVy_vs_Prob[i][j],i,j,"pVy");
            histName = Form("Prot_PullsVz_vs_Prob_Theta%i_Phi%i",i,j);
            Prot_PullVz_vs_Prob[i][j] = new TH2I(histName,"",1000,0,1.01,1000,-20,20);
            readInHists(Prot_PullVz_vs_Prob[i][j],i,j,"pVz");
	}
}
	
	
	setThetaMin_pip(2.0);
	setThetaMax_pip(12.0);
	setPhiMin_pip(-180.0);
	setPhiMax_pip(180.0);
	
	setThetaMin_pim(2.0);
	setThetaMax_pim(12.0);
	setPhiMin_pim(-180.0);
	setPhiMax_pim(180.0);

    setThetaMin_p(65.0);
	setThetaMax_p(75.0);
	setPhiMin_p(-180.0);
	setPhiMax_p(180.0);
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

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
       
     // dTreeInterface->Get_Fundamental<Float_t>("PiPlus__Px_Pull",100);
}

Bool_t DSelector_checkKFit::Process(Long64_t locEntry)
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
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_Topology;
	bool usedTopology = false;

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
		TLorentzVector locPiPlusP4 = dPiPlusWrapper->Get_P4();
		TLorentzVector locPiMinusP4 = dPiMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlusP4_Measured = dPiPlusWrapper->Get_P4_Measured();
		TLorentzVector locPiMinusP4_Measured = dPiMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		
		//chi2PiPlus = dPiPlusWrapper->Get_Beta_Timing_Measured();
		//chi2Prot = dProtonWrapper->Get_Beta_Timing_Measured();
		
		inVector = locBeamP4_Measured + dTargetP4;
		outVector = locPiPlusP4_Measured + locPiMinusP4_Measured + locProtonP4_Measured;
		
		missingP = (inVector - outVector).P();
		missingE = (inVector - outVector).E();
		mimass_all = (inVector - outVector).M2();
		
		mimassVec = inVector - locProtonP4_Measured;
		imassVec = locPiPlusP4_Measured + locPiMinusP4_Measured;
		
		mimassPipPimVec = inVector - locPiPlusP4_Measured - locPiMinusP4_Measured;
	    mimassPipPim = mimassPipPimVec.M2();
		mimassPipPVec = inVector - locPiPlusP4_Measured - locProtonP4_Measured;
		mimassPipP = mimassPipPVec.M2();
		mimassPimPVec = inVector - locPiMinusP4_Measured - locProtonP4_Measured;
		mimassPimP = mimassPimPVec.M2();
		
		mimass = mimassVec.M();
		imass = imassVec.M();
		
		kfitProb = TMath::Prob(dComboWrapper->Get_ChiSq_KinFit(),dComboWrapper->Get_NDF_KinFit());
		
		pullCollection[0][0] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Px_Pull");
		pullCollection[0][1] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Py_Pull");
		pullCollection[0][2] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Pz_Pull");
		pullCollection[0][3] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Vx_Pull");
		pullCollection[0][4] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Vy_Pull");
		pullCollection[0][5] = dComboWrapper->Get_Fundamental<Double_t>("PiPlus__Vz_Pull");
		
		pullCollection[1][0] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Px_Pull");
		pullCollection[1][1] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Py_Pull");
		pullCollection[1][2] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Pz_Pull");
		pullCollection[1][3] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Vx_Pull");
		pullCollection[1][4] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Vy_Pull");
		pullCollection[1][5] = dComboWrapper->Get_Fundamental<Double_t>("PiMinus__Vz_Pull");
		
		pullCollection[2][0] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Px_Pull");
		pullCollection[2][1] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Py_Pull");
		pullCollection[2][2] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Pz_Pull");
		pullCollection[2][3] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Vx_Pull");
		pullCollection[2][4] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Vy_Pull");
		pullCollection[2][5] = dComboWrapper->Get_Fundamental<Double_t>("Proton__Vz_Pull");
		
		//setPullElements(0,0,pullCollection[0][0]);

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
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_Topology;
		locUsedThisCombo_Topology[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_Topology[PiPlus].insert(locPiPlusTrackID);
		locUsedThisCombo_Topology[PiMinus].insert(locPiMinusTrackID);
		locUsedThisCombo_Topology[Proton].insert(locProtonTrackID);
		
		if(locUsedSoFar_Topology.find(locUsedThisCombo_Topology) == locUsedSoFar_Topology.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			//dHist_MissingMassSquared->Fill(locMissingMassSquared);
			
			locUsedSoFar_Topology.insert(locUsedThisCombo_Topology);
			usedTopology = true;
		}else usedTopology = false;
		
        if(usedTopology){
		    missingP_vs_missingE->Fill(missingE,missingP);
		    EM_balance->Fill(missingP-TMath::Abs(missingE));
		    MMALL->Fill(mimass_all);
            MM_vs_IM->Fill(imass,mimass);
            probDist->Fill(kfitProb);
            
            //-----------------------------------------------------------------------------
            if(isInsideDM(missingP-TMath::Abs(missingE),0.0016,0.0106,2)){ //1st cut to reconstruct: gp->pi+pi- p: missing energy and  missing momentum
				probDistCut->Fill(kfitProb);
				missingP_vs_missingE_Cut->Fill(missingE,missingP);
				MM_vs_IM_Cut->Fill(imass,mimass);
				EM_balance_Cut->Fill(missingP-TMath::Abs(missingE));
				MMALL_Cut->Fill(mimass_all);
				
				//-----------------------------------------------------------------------------
				if(mimass_all > -0.005 && mimass_all < 0.005){ //2nd cut to reconstruct: gp->pi+pi- p: overall missing mass --> should be 0
					probDistCut2->Fill(kfitProb);
				    missingP_vs_missingE_Cut2->Fill(missingE,missingP);
				    MM_vs_IM_Cut2->Fill(imass,mimass);
				    EM_balance_Cut2->Fill(missingP-TMath::Abs(missingE));
				    MMALL_Cut2->Fill(mimass_all);	
				     
				     
				     //Angular INDEPENDENT investigation of pulls:
				     //------------------------------------------------------------
		             for(Int_t a=0;a<3;a++){
				        for(Int_t b=0;b<6;b++){
					       Pulls_vs_Prob[a][b]->Fill(kfitProb,pullCollection[a][b]); 
				        }
			         }
			         //------------------------------------------------------------ 
				     
				     
				    fillAngularHists(locPiPlusP4_Measured,locPiMinusP4_Measured,locProtonP4_Measured,Theta_vs_phi_PiPlus,Theta_vs_phi_PiMinus,Theta_vs_phi_Prot);
				       
			         //Angular DEPENDENT investigation of pulls:
				    fillPullHistsPiPlus(getTheta(locPiPlusP4,"deg"),getPhi(locPiPlusP4,"deg"),kfitProb);
				    fillPullHistsPiMinus(getTheta(locPiMinusP4,"deg"),getPhi(locPiMinusP4,"deg"),kfitProb);
				    fillPullHistsProt(getTheta(locProtonP4,"deg"),getPhi(locProtonP4,"deg"),kfitProb);
				}
				//-----------------------------------------------------------------------------
			}
            //-----------------------------------------------------------------------------
		}
    


		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

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

void DSelector_checkKFit::Finalize(void)
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
