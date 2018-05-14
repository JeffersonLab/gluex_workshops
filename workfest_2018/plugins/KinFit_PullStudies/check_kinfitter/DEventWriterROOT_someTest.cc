// $Id$
//
//    File: DEventWriterROOT_someTest.cc
// Created: Sa 14. Apr 18:30:24 EDT 2018
// Creator: dlersch (on Linux ifarm1402.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#include "DEventWriterROOT_someTest.h"

//GLUEX TTREE DOCUMENTATION: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat

void DEventWriterROOT_someTest::Create_CustomBranches_DataTree(DTreeBranchRegister& locBranchRegister, JEventLoop* locEventLoop, const DReaction* locReaction, bool locIsMCDataFlag) const
{
    
    //Get Pulls for the beam:
    Particle_t beamPID = locReaction->Get_ReactionStep(0)->Get_InitialPID();
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Px_Pull","NumCombos", dInitNumComboArraySize);
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Py_Pull","NumCombos", dInitNumComboArraySize);
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Pz_Pull","NumCombos", dInitNumComboArraySize);
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Vx_Pull","NumCombos", dInitNumComboArraySize);
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Vy_Pull","NumCombos", dInitNumComboArraySize);
    locBranchRegister.Register_FundamentalArray<Double_t>("ComboBeam__Vz_Pull","NumCombos", dInitNumComboArraySize);
    
    
    //Try to get the pull-information for all final state particles:
    string locArraySizeString = "NumCombos";
    
    vector<Particle_t> finalPIDs;
    size_t nReactionSteps = locReaction->Get_NumReactionSteps();
    int nFinalParticles;
    
    //----------------------------------------------------------------------------------------------
    for(size_t loc_i=0;loc_i<nReactionSteps;loc_i++){
        finalPIDs = locReaction->Get_ReactionStep(loc_i)->Get_FinalPIDs(true,d_Charged,true);
        nFinalParticles = finalPIDs.size();
        
        //----------------------------------------------------------------------------------------------
        for(int loc_j=0;loc_j<nFinalParticles;loc_j++){
          //Define branches for final particles:
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Px_Pull"),"NumCombos", dInitNumComboArraySize);
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Py_Pull"),"NumCombos", dInitNumComboArraySize);
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Pz_Pull"),"NumCombos", dInitNumComboArraySize);
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Vx_Pull"),"NumCombos", dInitNumComboArraySize);
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Vy_Pull"),"NumCombos", dInitNumComboArraySize);
          locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(EnumString(finalPIDs.at(loc_j)),"Vz_Pull"),"NumCombos", dInitNumComboArraySize);
        }
        //----------------------------------------------------------------------------------------------
    }
    //----------------------------------------------------------------------------------------------
    
   
    
/*
	//EXAMPLES: Create a branch for a single object (e.g. Int_t, Float_t, TVector3):
	//If filling for a specific particle, the branch name should match the particle branch name
	locBranchRegister.Register_Single<UInt_t>("DummyUInt");
	locBranchRegister.Register_Single<Float_t>("PiPlus__DummyFloat");
	locBranchRegister.Register_Single<TVector3>("Dummy3Vector");
	locBranchRegister.Register_Single<TLorentzVector>("PiPlus__Dummy4Vector");
*/

/*
	//EXAMPLES: Create a branch to hold an array of fundamental type:
	//If filling for a specific particle, the branch name should match the particle branch name
	//locArraySizeString is the name of the branch whose variable that contains the size of the array for that tree entry
		//To match the default TTree branches, use either: 'NumThrown', 'NumBeam', 'NumChargedHypos', 'NumNeutralHypos', or 'NumCombos', as appropriate
	unsigned int locInitArraySize = 10; //if too small, will auto-increase as needed, but requires new calls //if too large, uses more memory than needed
	locBranchRegister.Register_Single<UInt_t>( "DummyArraySize"); //you must store the size of the fundamental array for each entry!!
	locBranchRegister.Register_FundamentalArray<Int_t>("PiPlus__DummyIntArray", "DummyArraySize", locInitArraySize);
	locBranchRegister.Register_FundamentalArray<Float_t>("DummyFloatArray", "DummyArraySize", locInitArraySize);

*/

/*
	//EXAMPLES: Create a branch to hold a TClonesArray of TObject type:
	//If filling for a specific particle, the branch name should match the particle branch name
	unsigned int locInitObjectArraySize = 10; //if too small, will auto-increase as needed, but requires new calls //if too large, uses more memory than needed
	locBranchRegister.Register_ClonesArray<TLorentzVector>("PiPlus__Dummy4VectorArray", locInitObjectArraySize);
	locBranchRegister.Register_ClonesArray<TVector3>("Dummy3VectorArray", locInitObjectArraySize);
*/
}

void DEventWriterROOT_someTest::Create_CustomBranches_ThrownTree(DTreeBranchRegister& locBranchRegister, JEventLoop* locEventLoop) const
{
	//EXAMPLES: See Create_CustomBranches_DataTree
}

void DEventWriterROOT_someTest::Fill_CustomBranches_DataTree(DTreeFillData* locTreeFillData, JEventLoop* locEventLoop, const DReaction* locReaction, const DMCReaction* locMCReaction, const vector<const DMCThrown*>& locMCThrowns,
	const DMCThrownMatching* locMCThrownMatching, const DDetectorMatches* locDetectorMatches,
	const vector<const DBeamPhoton*>& locBeamPhotons, const vector<const DChargedTrackHypothesis*>& locChargedHypos,
	const vector<const DNeutralParticleHypothesis*>& locNeutralHypos, const deque<const DParticleCombo*>& locParticleCombos) const
{
    DKinFitType kfitType = locReaction->Get_KinFitType();

    //Now fill the pulls for all particles:
    int nParticleCombos = locParticleCombos.size();
    
    int nMeasuredFinalParticles;
    double currentCharge;
    Particle_t currentPID;
   
    //----------------------------------------------------------------------------
    for(int iPC=0;iPC<nParticleCombos;iPC++){
        Get_PullsFromFit(locParticleCombos.at(iPC));
        
        //Look at beam photons:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        const DParticleComboStep* locParticleComboStep = locParticleCombos[iPC]->Get_ParticleComboStep(0);
        const DKinematicData* beamPhoton = locParticleComboStep->Get_InitialParticle_Measured();
        
        map<DKinFitPullType, double> someBeamMap = getPulls(beamPhoton);
        
        
        
        //Check for final state particles only:
        if(kfitType == d_P4Fit || kfitType == d_P4AndVertexFit){
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Px_Pull", someBeamMap.find(d_PxPull)->second,iPC);
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Py_Pull", someBeamMap.find(d_PyPull)->second,iPC);
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Pz_Pull", someBeamMap.find(d_PzPull)->second,iPC);
        }
        
        if(kfitType == d_VertexFit || kfitType == d_P4AndVertexFit){
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Vx_Pull", someBeamMap.find(d_XxPull)->second,iPC);
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Vy_Pull", someBeamMap.find(d_XyPull)->second,iPC);
          locTreeFillData->Fill_Array<Double_t>("ComboBeam__Vz_Pull", someBeamMap.find(d_XzPull)->second,iPC);
        }
        someBeamMap.empty();
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        //Look at the decay products:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        vector<const DKinematicData*> finalParticlesMeasured = locParticleCombos.at(iPC)->Get_FinalParticles_Measured(locReaction, d_AllCharges);
        nMeasuredFinalParticles = finalParticlesMeasured.size();
        
        //----------------------------------------------------------------------------
        for(int iFP=0;iFP<nMeasuredFinalParticles;iFP++){
            map<DKinFitPullType, double> someMap = getPulls(finalParticlesMeasured.at(iFP));
            currentCharge = finalParticlesMeasured.at(iFP)->charge();
            currentPID = finalParticlesMeasured.at(iFP)->PID();
        
            //Check for final state particles only:
            if(kfitType == d_P4Fit || kfitType == d_P4AndVertexFit){
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Px_Pull"), someMap.find(d_PxPull)->second,iPC);
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Py_Pull"), someMap.find(d_PyPull)->second,iPC);
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Pz_Pull"), someMap.find(d_PzPull)->second,iPC);
            }
            
            if(kfitType == d_VertexFit || kfitType == d_P4AndVertexFit){
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Vx_Pull"), someMap.find(d_XxPull)->second,iPC);
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Vy_Pull"), someMap.find(d_XyPull)->second,iPC);
              locTreeFillData->Fill_Array<Double_t>(Build_BranchName(EnumString(currentPID), "Vz_Pull"), someMap.find(d_XzPull)->second,iPC);
            }
            someMap.empty();
        }
        //----------------------------------------------------------------------------
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     
        myPullsMap.empty();
    }
    //----------------------------------------------------------------------------
    
    
    
    //Fill the array for charged tracks:
    
    
	//The array indices of the particles/combos in the main TTree branches match the vectors of objects passed into this function
		//So if you want to add custom data for each (e.g.) charged track, the correspondence to the main arrays is 1 <--> 1

/*
	//EXAMPLES: Fill a branch for a fundamental data type (e.g. Int_t, Float_t):
	//!!!!! YOU MUST BE SURE THAT template type matches the type you used to create the branch
	locTreeFillData->Fill_Single<UInt_t>("DummyUInt", 14); //14: dummy value
	locTreeFillData->Fill_Single<Float_t>("PiPlus__DummyFloat", TMath::Pi()); //pi: dummy value
*/

/*
	//EXAMPLES: Fill a branch for a TObject data type (e.g. TVector3, TLorentzVector):
	//!!!!! YOU MUST BE SURE THAT template type matches the type you used to create the branch
	TVector3 locPosition(0.0, 0.0, 65.0);
	locTreeFillData->Fill_Single<TVector3>("Dummy3Vector", locPosition);
	TLorentzVector locP4(1.0, 2.0, 3.0, 4.0);
	locTreeFillData->Fill_Single<TLorentzVector>("PiPlus__Dummy4Vector", locP4);
*/

/*
	//EXAMPLES: Fill a branch with an array of fundamental type:
	//!!!!! YOU MUST BE SURE THAT template type matches the type you used to create the branch
	Int_t locOutputArraySize = 7;
	locTreeFillData->Fill_Single<UInt_t>("DummyArraySize", locOutputArraySize);
	for(Int_t loc_i = 0; loc_i < locOutputArraySize; ++loc_i)
	{
		Int_t locValue = loc_i - 14; //dummy number
		locTreeFillData->Fill_Array<Int_t>("PiPlus__DummyIntArray", locValue, loc_i);
		locTreeFillData->Fill_Array<Float_t>("DummyFloatArray", locValue + 9999, loc_i);
	}
*/

/*
	//EXAMPLES: Fill a branch with a TClonesArray of TObject type:
	//!!!!! YOU MUST BE SURE THAT template type matches the type you used to create the branch
	for(Int_t loc_i = 0; loc_i < 15; ++loc_i)
	{
		TLorentzVector locNewP4(99.0, 2.0, 3.0, 4.0);
		locTreeFillData->Fill_Array<TLorentzVector>("PiPlus__Dummy4VectorArray", locNewP4, loc_i);
		TVector3 locNewPosition(100.0, 0.0, 65.0);
		locTreeFillData->Fill_Array<TVector3>("Dummy3VectorArray", locNewPosition, loc_i);
	}
*/
}

void DEventWriterROOT_someTest::Fill_CustomBranches_ThrownTree(DTreeFillData* locTreeFillData, JEventLoop* locEventLoop, const DMCReaction* locMCReaction, const vector<const DMCThrown*>& locMCThrowns) const
{
	//EXAMPLES: See Fill_CustomBranches_DataTree
}




//*************************************************************************************************************************************
void DEventWriterROOT_someTest::Get_PullsFromFit(const DParticleCombo* particleCombos) const
{
    //-----------------------------------------------------------------------------
    const DKinFitResults* fitResults = particleCombos->Get_KinFitResults();
    if(fitResults != NULL){
        fitResults->Get_Pulls(myPullsMap);
    }
    //-----------------------------------------------------------------------------
}

//===================================================================

map<DKinFitPullType, double> DEventWriterROOT_someTest::getBeamPulls(const DBeamPhoton* beamPhoton) const
{
    return myBeamPullsMap.find((DKinematicData*)beamPhoton)->second;
}

//===================================================================

map<DKinFitPullType, double> DEventWriterROOT_someTest::getChargedPulls(const DChargedTrackHypothesis* chargedTrack) const
{
    return myChargedPullsMap.find((DKinematicData*)chargedTrack)->second;
}

//===================================================================

map<DKinFitPullType, double> DEventWriterROOT_someTest::getNeutralPulls(const DNeutralParticleHypothesis* neutralTrack) const
{
    return myNeutralPullsMap.find((DKinematicData*)neutralTrack)->second;
}


map<DKinFitPullType, double> DEventWriterROOT_someTest::getPulls(const DKinematicData* data) const{
    return myPullsMap.find(data)->second;
}
//*************************************************************************************************************************************




