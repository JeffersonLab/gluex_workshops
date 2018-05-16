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
    
    vector<Particle_t> finalchargedPIDs,finalneutralPIDs;
    size_t nReactionSteps = locReaction->Get_NumReactionSteps();
    int nFinalChargedParticles,nFinalNeutralParticles;
    
    vector<string> pidVector = getPIDinReaction(locReaction);
    map<Particle_t,int> assignMap = getNameForParticle(locReaction,pidVector);
    
    //----------------------------------------------------------------------------------------------
    for(size_t loc_i=0;loc_i<nReactionSteps;loc_i++){
        
        //cout <<"Reaction step: "<<loc_i<< endl;
        
        //Charged Particles:
        finalchargedPIDs = locReaction->Get_ReactionStep(loc_i)->Get_FinalPIDs(true,d_Charged,true);
        nFinalChargedParticles = finalchargedPIDs.size();
        
        //----------------------------------------------------------------------------------------------
        for(int loc_j=0;loc_j<nFinalChargedParticles;loc_j++){
          //Define branches for final charged particles:
            string branchName = assignName(finalchargedPIDs.at(loc_j),assignMap);
            assignMap[finalchargedPIDs.at(loc_j)]--;
            if(branchName != "nada"){
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Px_Pull"),"NumCombos", dInitNumComboArraySize);
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Py_Pull"),"NumCombos", dInitNumComboArraySize);
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Pz_Pull"),"NumCombos", dInitNumComboArraySize);
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vx_Pull"),"NumCombos", dInitNumComboArraySize);
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vy_Pull"),"NumCombos", dInitNumComboArraySize);
                locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vz_Pull"),"NumCombos", dInitNumComboArraySize);
            }
        }
        //----------------------------------------------------------------------------------------------
        
        
        //Neutral Particles:
        finalneutralPIDs = locReaction->Get_ReactionStep(loc_i)->Get_FinalPIDs(true,d_Neutral,true);
        nFinalNeutralParticles = finalneutralPIDs.size();
        
        //----------------------------------------------------------------------------------------------
        for(int loc_j=0;loc_j<nFinalNeutralParticles;loc_j++){
            //Define branches for final neutral particles:
            string branchName = assignName(finalneutralPIDs.at(loc_j),assignMap);
            assignMap[finalneutralPIDs.at(loc_j)]--;
            if(branchName != "nada"){
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Px_Pull"),"NumCombos", dInitNumComboArraySize);
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Py_Pull"),"NumCombos", dInitNumComboArraySize);
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Pz_Pull"),"NumCombos", dInitNumComboArraySize);
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vx_Pull"),"NumCombos", dInitNumComboArraySize);
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vy_Pull"),"NumCombos", dInitNumComboArraySize);
              locBranchRegister.Register_FundamentalArray<Double_t>(Build_BranchName(branchName,"Vz_Pull"),"NumCombos", dInitNumComboArraySize);
            }
        }
        //----------------------------------------------------------------------------------------------
        
    }
    //----------------------------------------------------------------------------------------------
    
    assignMap.clear();
    abundanceMap.clear();
    
   
    
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
    int nChargedParticles,nNeutralParticles;
   
    //----------------------------------------------------------------------------
    for(int iPC=0;iPC<nParticleCombos;iPC++){
        Get_PullsFromFit(locParticleCombos.at(iPC));
        
        vector<string> pidVector = getPIDinReaction(locReaction);
        map<Particle_t,int> assignMap = getNameForParticle(locReaction,pidVector);
        
        
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
        someBeamMap.clear();
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
            
            string branchName = assignName(currentPID,assignMap);
            assignMap[currentPID]--;
            if(branchName != "nada"){
        
              //Check for final state particles only:
              if(kfitType == d_P4Fit || kfitType == d_P4AndVertexFit){
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Px_Pull"), someMap.find(d_PxPull)->second,iPC);
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Py_Pull"), someMap.find(d_PyPull)->second,iPC);
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Pz_Pull"), someMap.find(d_PzPull)->second,iPC);
              }
            
              if((kfitType == d_VertexFit || kfitType == d_P4AndVertexFit) && currentCharge != 0){
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Vx_Pull"), someMap.find(d_XxPull)->second,iPC);
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Vy_Pull"), someMap.find(d_XyPull)->second,iPC);
                locTreeFillData->Fill_Array<Double_t>(Build_BranchName(branchName, "Vz_Pull"), someMap.find(d_XzPull)->second,iPC);
              }
            }
            someMap.clear();
        }
        //----------------------------------------------------------------------------
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        
    
        
        myPullsMap.clear();
        assignMap.clear();
        abundanceMap.clear();
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


//I guess this is the most clumsiest way of assigning a few names to a tree-branch, but it is doing what it is supposed to do...
//*************************************************************************************************************************************
vector<string> DEventWriterROOT_someTest::getPIDinReaction(const DReaction* locReaction)const
{
    vector <string> myVector;
    vector<Particle_t> particle = locReaction->Get_FinalPIDs(-1,false,false,d_AllCharges,true);
    int nParticleIDs = particle.size();
    //----------------------------------------------------
    for(int k=0;k<nParticleIDs;k++){
        myVector.push_back(EnumString(particle.at(k)));
    }
    //----------------------------------------------------
    return myVector;
}

//===================================================================

map<Particle_t, int> DEventWriterROOT_someTest::getNameForParticle(const DReaction* someReaction, vector<string> someVector)const
{
    map <Particle_t,int> myMap;
    vector<Particle_t> particle = someReaction->Get_FinalPIDs(-1,false,false,d_AllCharges,true);
    int nParticleIDs = particle.size();
    int currentCounter = 0;
    int nEl = someVector.size();
    //----------------------------------------------------
    for(int k=0;k<nParticleIDs;k++){
        currentCounter = 0;
        
        //----------------------------------------------------
        for(int j=0;j<nEl;j++){
            if(EnumString(particle.at(k)) == someVector.at(j)){
                currentCounter++;
                someVector.at(j) = "";
            }
        }
        //----------------------------------------------------
        if(currentCounter > 0){
            abundanceMap[particle.at(k)] = currentCounter;
            myMap[particle.at(k)] = currentCounter;
        }
    }
    //----------------------------------------------------
    return myMap;
}

//===================================================================

string DEventWriterROOT_someTest::assignName(Particle_t someParticle, map<Particle_t,int> someMap)const
{
    string myName = "nada";
    int abundance = abundanceMap.find(someParticle)->second;
    if(abundance == 1){
        myName = EnumString(someParticle);
    }else if(abundance > 1){
        int index = abundance - someMap.find(someParticle)->second +1;
        string addName;
        ostringstream convert;
        convert << index;
        addName = convert.str();
        if(EnumString(someParticle) == "Gamma"){
            myName = "Photon" + addName;
        }else myName = EnumString(someParticle) + addName;
    }
    
    return myName;
}

//*************************************************************************************************************************************



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

map<DKinFitPullType, double> DEventWriterROOT_someTest::getPulls(const DKinematicData* data) const{
    return myPullsMap.find(data)->second;
}
//*************************************************************************************************************************************




