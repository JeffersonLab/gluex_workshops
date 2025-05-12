#include "DSelector_Thrown.h"

void DSelector_Thrown::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "Thrown.root"; //"" for none
	dFlatTreeFileName = "omegapi_tree.root"; // output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = "kin";						  // if blank, default name will be chosen
	// dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.

	//USERS: SET OUTPUT TREE FILES/NAMES //e.g. binning into separate files for AmpTools
	//dOutputTreeFileNameMap["Bin1"] = "mcgen_bin1.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin2"] = "mcgen_bin2.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin3"] = "mcgen_bin3.root"; //key is user-defined, value is output file name

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	dPreviousRunNumber = 0;

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want



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

	
}

Bool_t DSelector_Thrown::Process(Long64_t locEntry)
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

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/******************************************* LOOP OVER THROWN DATA ***************************************/

	//Thrown beam: just use directly
	double locBeamEnergyUsedForBinning = 0.0;
	if(dThrownBeam != NULL)
		locBeamEnergyUsedForBinning = dThrownBeam->Get_P4().E();

	//Loop over throwns
	// for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	// {
	// 	//Set branch array indices corresponding to this particle
	// 	dThrownWrapper->Set_ArrayIndex(loc_i);

	// 	//Do stuff with the wrapper here ...
	// 	Particle_t locPID = dThrownWrapper->Get_PID();
	// 	TLorentzVector locThrownP4 = dThrownWrapper->Get_P4();
	// 	//cout << "Thrown " << loc_i << ": " << locPID << ", " << locThrownP4.Px() << ", " << locThrownP4.Py() << ", " << locThrownP4.Pz() << ", " << locThrownP4.E() << endl;
	// }

	// //OR Manually:
	// //BEWARE: Do not expect the particles to be at the same array indices from one event to the next!!!!
	// //Why? Because while your channel may be the same, the pions/kaons/etc. will decay differently each event.

	// //BRANCHES: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat#TTree_Format:_Simulated_Data
	// TClonesArray** locP4Array = dTreeInterface->Get_PointerToPointerTo_TClonesArray("Thrown__P4");
	// TBranch* locPIDBranch = dTreeInterface->Get_Branch("Thrown__PID");
/*
	Particle_t locThrown1PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[0]);
	TLorentzVector locThrown1P4 = *((TLorentzVector*)(*locP4Array)->At(0));
	cout << "Particle 1: " << locThrown1PID << ", " << locThrown1P4.Px() << ", " << locThrown1P4.Py() << ", " << locThrown1P4.Pz() << ", " << locThrown1P4.E() << endl;
	Particle_t locThrown2PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[1]);
	TLorentzVector locThrown2P4 = *((TLorentzVector*)(*locP4Array)->At(1));
	cout << "Particle 2: " << locThrown2PID << ", " << locThrown2P4.Px() << ", " << locThrown2P4.Py() << ", " << locThrown2P4.Pz() << ", " << locThrown2P4.E() << endl;
*/


	/******************************************* BIN THROWN DATA INTO SEPARATE TREES FOR AMPTOOLS ***************************************/

/*
	//THESE KEYS MUST BE DEFINED IN THE INIT SECTION (along with the output file names)
	if((locBeamEnergyUsedForBinning >= 8.0) && (locBeamEnergyUsedForBinning < 9.0))
		Fill_OutputTree("Bin1"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 9.0) && (locBeamEnergyUsedForBinning < 10.0))
		Fill_OutputTree("Bin2"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 10.0) && (locBeamEnergyUsedForBinning < 11.0))
		Fill_OutputTree("Bin3"); //your user-defined key
*/


	vector<TLorentzVector> locFinalStateThrownP4;
	vector<TLorentzVector> locPi0ThrownP4;
	TLorentzVector loc3piThrownP4;
	TLorentzVector loc4piThrownP4;
	TLorentzVector locProtonPiThrownP4;
	double loctThrown = 0;
	int locPi0OmegaIndex = -1;
	if (Get_NumThrown() > 0)
	{
		for (UInt_t i = 0; i < Get_NumThrown(); i++)
		{
			dThrownWrapper->Set_ArrayIndex(i);
			Int_t locThrownPID = dThrownWrapper->Get_PID();
			// get Pi0s first to set proper order for AmpTools
			if (locThrownPID == 7)
			{
				locPi0ThrownP4.push_back(dThrownWrapper->Get_P4());
				if (locPi0OmegaIndex < 0)
					locPi0OmegaIndex = i;
			}
		}

		for (UInt_t i = 0; i < Get_NumThrown(); i++)
		{
			// Set branch array indices corresponding to this particle
			dThrownWrapper->Set_ArrayIndex(i);
			Int_t locThrownPID = dThrownWrapper->Get_PID();
			if (locThrownPID == 14)
			{
				locFinalStateThrownP4.push_back(dThrownWrapper->Get_P4());
				loctThrown = -1. * (dThrownWrapper->Get_P4() - dTargetP4).M2();
				locProtonPiThrownP4 += dThrownWrapper->Get_P4();
			}
			if (locThrownPID == 8 || locThrownPID == 9)
			{
				locFinalStateThrownP4.push_back(dThrownWrapper->Get_P4());
				loc3piThrownP4 += dThrownWrapper->Get_P4();
				loc4piThrownP4 += dThrownWrapper->Get_P4();
			}

			if (locFinalStateThrownP4.size() == 1)
			{
				if (locPi0ThrownP4.size() > 0)
				{
					locFinalStateThrownP4.push_back(locPi0ThrownP4[1]);
					loc4piThrownP4 += locPi0ThrownP4[1];
					locProtonPiThrownP4 += locPi0ThrownP4[1];

					locFinalStateThrownP4.push_back(locPi0ThrownP4[0]);
					loc3piThrownP4 += locPi0ThrownP4[0];
					loc4piThrownP4 += locPi0ThrownP4[0];
				}
			}
			if (locFinalStateThrownP4.size() == 5)
				break;
		}

		TLorentzVector locBeamP4 = dThrownBeam->Get_P4();
		bool beamSel = locBeamP4.E() >= 8.2 && locBeamP4.E() <= 8.8;
		double loct = (locBeamP4 - loc4piThrownP4).M2();
		bool tSel = abs(loct) <= 1;
		if( tSel && beamSel ){
		dFlatTreeInterface->Fill_Fundamental<Float_t>("M3Pi", loc3piThrownP4.M());
		dFlatTreeInterface->Fill_Fundamental<Float_t>("M4Pi", loc4piThrownP4.M());
		dFlatTreeInterface->Fill_Fundamental<Float_t>("t", loctThrown);
		dFlatTreeInterface->Fill_Fundamental<Float_t>("MRecoilPi", locProtonPiThrownP4.M());
		dFlatTreeInterface->Fill_Fundamental<Float_t>("Weight", 1.0);
		
		// for(uint i=0; i<locFinalStateThrownP4.size(); i++)
		//	cout<<i<<" "<<locFinalStateThrownP4[i].M()<<endl;
		FillAmpTools_FlatTree(locBeamP4, locFinalStateThrownP4);
		Fill_FlatTree();
		}
	}

	return kTRUE;
}

void DSelector_Thrown::Finalize(void)
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
