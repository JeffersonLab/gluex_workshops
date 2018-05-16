#include "DReaction_factory_ReactionFilter.h"

/*
 * COMMAND LINE INPUT:
 *
 * Any particles whose decays are not explicitly listed: default decay mode will be used if any. If none available, it will be assumed that the particle is to be detected.
 * This means that no default decay should be listed for the pi+, k+, etc.
 *
 * Reaction Form 1: BeamPID_TargetPID__FS1PID_FS2PID_FS3PID
 * Reaction Form 2: DecayPID_TargetPID__FS1PID_FS2PID_FS3PID
 * Reaction Form 3: DecayPID__FS1PID_FS2PID_FS3PID
 * To distinguish between forms 1 & 2: If first step, form 1 is assumed
 *
 * PID: Geant PID
 * parentheses around any means missing, Unknown means inclusive
 *
 * CONTROL MODE EXAMPLES:
 * -PReaction1=1_14__14_8_9 #g, p -> p, pi+, pi-
 * -PReaction2=1_14__14_8_9_7 #g, p -> p, pi+, pi-, pi0
 *
 * -PReaction3=1_14__14_8_9_7 #g, p -> p, pi+, pi-, pi0
 * -PReaction3:Decay1=7__2_3_1 # pi0 -> e+, e-, g
 * -PReaction3:Flags=F1_B2_T4_M7_U1  #Fit enum 1 (p4-only (includes mass)), +/- 2 RF bunches, 4 extra tracks, don't constrain mass on pi0 (can have any number), save unused tracks # Can list these in any order
 *    Fit enums are defined in DReaction.h (d_P4AndVertexFit is 4 and is the default)
 * -PReaction3:Name=my_pi0_channel
 *
 * -PReaction4=1_14__14_8_9_m7 #g, p -> p, pi+, pi-, missing pi0
 * -PReaction4=1_14__14_8_9_m0 #g, p -> p, pi+, pi-, inclusive
 *
 */

/******************************************************************************** CUSTOMIZATION FUNCTIONS ********************************************************************************/

DReactionStep* DReaction_factory_ReactionFilter::Create_DefaultDecayStep(Particle_t locPID)
{
	//DO NOT SPECIFY DEFAULT DECAYS FOR PARTICLES THAT CAN BE DETECTED!!!!!! (e.g. pion, neutron, klong)

	//MESONS
	if(locPID == Pi0)
		return (new DReactionStep(Pi0, {Gamma, Gamma}));
	else if(locPID == KShort)
		return (new DReactionStep(KShort, {PiMinus, PiPlus}));
	else if(locPID == Eta)
		return (new DReactionStep(Eta, {Gamma, Gamma}));
	else if(locPID == omega)
		return (new DReactionStep(omega, {PiMinus, PiPlus, Pi0}));
	else if(locPID == EtaPrime)
		return (new DReactionStep(EtaPrime, {PiMinus, PiPlus, Eta}));
	else if(locPID == phiMeson)
		return (new DReactionStep(phiMeson, {KMinus, KPlus}));
	else if(locPID == D0)
		return (new DReactionStep(D0, {KMinus, PiPlus}));
	else if(locPID == AntiD0)
		return (new DReactionStep(AntiD0, {KPlus, PiMinus}));
	else if(locPID == Jpsi)
		return (new DReactionStep(Jpsi, {Electron, Positron}));

	//BARYONS
	else if(locPID == Lambda)
		return (new DReactionStep(Lambda, {PiMinus, Proton}));
	else if(locPID == AntiLambda)
		return (new DReactionStep(AntiLambda, {PiPlus, AntiProton}));
	else if(locPID == SigmaMinus)
		return (new DReactionStep(SigmaMinus, {PiMinus, Neutron}));
	else if(locPID == Sigma0)
		return (new DReactionStep(Sigma0, {Gamma, Lambda}));
	else if(locPID == SigmaPlus)
		return (new DReactionStep(SigmaPlus, {Pi0, Proton}));
	else if(locPID == Xi0)
		return (new DReactionStep(Xi0, {Pi0, Lambda}));
	else if(locPID == XiMinus)
		return (new DReactionStep(XiMinus, {PiMinus, Lambda}));
	else if(locPID == OmegaMinus)
		return (new DReactionStep(OmegaMinus, {KMinus, Lambda}));
	else if(locPID == Lambda_c)
		return (new DReactionStep(Lambda_c, {PiPlus, KMinus, Proton}));

	return nullptr;
}

void DReaction_factory_ReactionFilter::Set_Flags(DReaction* locReaction, string locRemainingFlagString)
{
	//Example: -PReaction3:Flags=F1_B2_T4_M7  #Fit enum 1 (p4-only (includes mass)), +/- 2 RF bunches, 4 extra tracks, don't constrain mass on pi0 (can have any number) # Can list these in any order

	//First set defaults, then let user override them
	locReaction->Set_KinFitType(d_P4AndVertexFit);
	locReaction->Set_NumPlusMinusRFBunches(1); // +/- 1 bunch for sideband subtraction
	locReaction->Set_MaxExtraGoodTracks(3);

	bool locSaveUnusedHypotheses = false;
	string locTreeFileName = string("tree_") + locReaction->Get_ReactionName() + string(".root");

	//Parse user input
	while(locRemainingFlagString != "")
	{
		auto locUnderscoreIndex = locRemainingFlagString.find("_");
		auto locThisFlagString = (locUnderscoreIndex != string::npos) ? locRemainingFlagString.substr(0, locUnderscoreIndex) : locRemainingFlagString;

		//Get the type char
		auto locFlagTypeChar = locThisFlagString[0];

		//Get the value
		istringstream locIStream(locThisFlagString.substr(1));
		int locFlagArg;
		locIStream >> locFlagArg;
		if(locIStream.fail())
		{
			cout << "BUILDING DREACTION, FLAG " << locThisFlagString << " NOT RECOGNIZED." << endl;
			if(locUnderscoreIndex == string::npos)
				break;
			continue;
		}

		switch(locFlagTypeChar)
		{
			case 'B': //# +/- rf bunches
				locReaction->Set_NumPlusMinusRFBunches(locFlagArg);
				break;
			case 'F': //kinfit enum value
				locReaction->Set_KinFitType(DKinFitType(locFlagArg));
				break;
			case 'T': //# extra tracks
				locReaction->Set_MaxExtraGoodTracks(locFlagArg);
				break;
			case 'U': //# save unused hypotheses
				locSaveUnusedHypotheses = (locFlagArg == 0) ? false : true;
				break;
			case 'M': //pid to not constrain mass of during kinfit
				for(auto& locStep : locReaction->Get_ReactionSteps())
				{
					if(locStep->Get_InitialPID() != Particle_t(locFlagArg))
						continue;
					(const_cast<DReactionStep*>(locStep))->Set_KinFitConstrainInitMassFlag(false);
				}
				break;
			default:
				cout << "BUILDING DREACTION, FLAG " << locThisFlagString << " NOT RECOGNIZED." << endl;
				continue;
		}

		if(locUnderscoreIndex == string::npos)
			break;
		locRemainingFlagString = locRemainingFlagString.substr(locUnderscoreIndex + 1);
	}

	locReaction->Enable_TTreeOutput(locTreeFileName, locSaveUnusedHypotheses); //true/false: do/don't save unused hypotheses
}

//------------------
// evnt
//------------------
jerror_t DReaction_factory_ReactionFilter::evnt(JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	//INIT
	dSourceComboP4Handler = new DSourceComboP4Handler(nullptr, false);
	dSourceComboTimeHandler = new DSourceComboTimeHandler(nullptr, nullptr, nullptr);

	auto locInputTuple = Parse_Input();
	auto locReactions = Create_Reactions(locInputTuple);

	// Loop over reactions
	for(auto locReaction : locReactions)
	{
		auto locKinFitType = locReaction->Get_KinFitType();
		auto locKinFitPerformedFlag = (locKinFitType != d_NoFit);
		auto locP4Fit = ((locKinFitType != d_NoFit) && (locKinFitType != d_VertexFit) && (locKinFitType != d_SpacetimeFit));

		/**************************************************** Analysis Actions ****************************************************/

		// HISTOGRAM MASSES
		Add_MassHistograms(locReaction, false, "PreKinFit"); //false: measured

		// IF NO KINFIT
		if(!locKinFitPerformedFlag)
		{
			locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction, false));
			locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false));
			_data.push_back(locReaction); //Register the DReaction with the factory
			continue;
		}

		// KINEMATIC FIT
		locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only

		//POST-KINFIT PID CUTS
		Add_PostKinfitTimingCuts(locReaction);

		// HISTOGRAM MASSES, POST-KINFIT
		if(locP4Fit)
		{
			Add_MassHistograms(locReaction, false, "PostKinFit"); //false: measured
			if(locKinFitPerformedFlag)
				Add_MassHistograms(locReaction, true, "PostKinFit_KinFit"); //true: kinfit
		}

		// KINEMATICS
		locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true));

		_data.push_back(locReaction); //Register the DReaction with the factory
	}

	return NOERROR;
}

/********************************************************************************** CREATION FUNCTIONS ***********************************************************************************/

DReactionStep* DReaction_factory_ReactionFilter::Create_ReactionStep(const DReactionStepTuple& locStepTuple)
{
	auto locInclusiveFlag = (std::get<4>(locStepTuple) == DReactionStep::Get_ParticleIndex_Inclusive());
	auto locBeamMissingFlag = (std::get<4>(locStepTuple) == DReactionStep::Get_ParticleIndex_Initial());
//	auto locSecondBeamMissingFlag = (std::get<4>(locStepTuple) == DReactionStep::Get_ParticleIndex_SecondBeam());

	if(std::get<1>(locStepTuple) == Unknown) //no target or 2nd beam: pure decay
		return (new DReactionStep(std::get<0>(locStepTuple), std::get<2>(locStepTuple), std::get<3>(locStepTuple), locInclusiveFlag));
	else //rescattering decay //2nd beam is not allowed for now (and in fact, is undistinguishable in input scheme)
		return (new DReactionStep(std::get<0>(locStepTuple), std::get<1>(locStepTuple), std::get<2>(locStepTuple), std::get<3>(locStepTuple), locInclusiveFlag, locBeamMissingFlag));
}

void DReaction_factory_ReactionFilter::Create_Steps(DReaction* locReaction, DReactionStep* locCurrentStep, vector<DReactionStepTuple>& locDecayStepTuples)
{
	//loop over final state particles of the current step. if the user has specified a decay for any of them, use that one. else generate decays as needed
	for(auto& locFinalPID : locCurrentStep->Get_FinalPIDs(false)) //false: exclude missing
	{
		auto Find_DecayStep = [&locFinalPID](const DReactionStepTuple& locDecayStepTuple) -> bool{return (std::get<0>(locDecayStepTuple) == locFinalPID);};
		auto locStepTupleIterator = std::find_if(locDecayStepTuples.begin(), locDecayStepTuples.end(), Find_DecayStep);

		//If not found, create default decay chain (if it's a detected particle (e.g. pion), this won't do anything)
		DReactionStep* locDecayStep = nullptr;
		if(locStepTupleIterator == locDecayStepTuples.end())
			locDecayStep = Create_DefaultDecayStep(locFinalPID);
		else //use user-specified decay, and erase it from the vector
		{
			locDecayStep = Create_ReactionStep(*locStepTupleIterator);
			locDecayStepTuples.erase(locStepTupleIterator);
		}
		if(locDecayStep == nullptr)
			continue; //must be a final-state particle

		//and, call this function again to see if we need to create decays for THOSE particles
		locReaction->Add_ReactionStep(locDecayStep);
		Create_Steps(locReaction, locDecayStep, locDecayStepTuples);
	}
}

vector<DReaction*> DReaction_factory_ReactionFilter::Create_Reactions(const map<size_t, tuple<string, string, string, vector<string>>>& locInputStrings)
{
	//loop through channels, setting up the reactions
	vector<DReaction*> locReactions;
	for(auto& locReactionPair : locInputStrings)
	{
		auto& locNameString = std::get<0>(locReactionPair.second);
		auto& locFirstStepString = std::get<1>(locReactionPair.second);
		auto& locFlagString = std::get<2>(locReactionPair.second);
		if(dDebugFlag)
			cout << "name string, first step string, flag string, #decay strings: " << locNameString << ", " << locFirstStepString << ", " << locFlagString << ", " << std::get<3>(locReactionPair.second).size() << endl;

		DReactionStepTuple locFirstStepTuple;
		if(!Parse_StepPIDString(locFirstStepString, locFirstStepTuple))
		{
			cout << "BUILDING DREACTIONS, INVALID PID STRING: " << locFirstStepString << endl;
			continue;
		}

		//see if we need to make our tree/reaction name
		//automatic reaction naming scheme (if not manually specified):
		//FirstStep__SpecifiedDecayStep1__SpecifiedDecayStep2_..._FlagString
		//within a step: InitName1InitName2_FinalName1FinalName2... //name is from ShortName()
		//also put "miss" in front of missing particles
		auto locReactionName = (locNameString != "") ? locNameString : Create_StepNameString(locFirstStepTuple, true); //will continue below if needed

		//loop over steps
		vector<DReactionStepTuple> locDecayStepTuples;
		for(auto& locDecayStepString : std::get<3>(locReactionPair.second))
		{
			DReactionStepTuple locDecayStepTuple;
			if(!Parse_StepPIDString(locDecayStepString, locDecayStepTuple))
			{
				cout << "BUILDING DREACTIONS, INVALID DECAY PID STRING: " << locDecayStepString << endl;
				continue;
			}
			locDecayStepTuples.push_back(locDecayStepTuple);

			//expand name if needed
			if(locNameString == "")
				locReactionName += string("__") + Create_StepNameString(locDecayStepTuple, false);
		}

		if((locNameString == "") && (locFlagString != "")) //add flags to name
			locReactionName += string("__") + locFlagString;

		if(dDebugFlag)
			cout << "ReactionFilter: reaction name: " << locReactionName << endl;

		//create dreaction & first step
		auto locReaction = new DReaction(locReactionName);
		auto locPrimaryStep = Create_ReactionStep(locFirstStepTuple);
		locReaction->Add_ReactionStep(locPrimaryStep);

		//create following steps
		Create_Steps(locReaction, locPrimaryStep, locDecayStepTuples);

		//Set flags
		Set_Flags(locReaction, locFlagString);

		//save reaction
		cout << "ReactionFilter: Reaction: " << locReactionName << endl;
		DAnalysis::Print_Reaction(locReaction);

		locReactions.push_back(locReaction);
	}

	return locReactions;
}

/************************************************************** ACTIONS AND CUTS **************************************************************/

void DReaction_factory_ReactionFilter::Add_MassHistograms(DReaction* locReaction, bool locUseKinFitResultsFlag, string locBaseUniqueName)
{
	auto locKinFitType = locReaction->Get_KinFitType();
	auto locP4Fit = ((locKinFitType != d_NoFit) && (locKinFitType != d_VertexFit) && (locKinFitType != d_SpacetimeFit));
	auto locNumMissingParticles = locReaction->Get_MissingPIDs().size();

	size_t locNumInclusiveSteps = 0;
	for(auto locReactionStep : locReaction->Get_ReactionSteps())
	{
		if(locReactionStep->Get_IsInclusiveFlag())
			++locNumInclusiveSteps;
	}

	//invariant mass
	set<Particle_t> locDecayPIDsUsed;
	for(size_t loc_i = 1; loc_i < locReaction->Get_NumReactionSteps(); ++loc_i)
	{
		//do missing mass squared hists for every decaying and missing particle
		auto locReactionStep = locReaction->Get_ReactionStep(loc_i);

		if(locP4Fit && locUseKinFitResultsFlag && locReactionStep->Get_KinFitConstrainInitMassFlag())
			continue;

		auto locDecayPID = locReactionStep->Get_InitialPID();
		if(locDecayPIDsUsed.find(locDecayPID) != locDecayPIDsUsed.end())
			continue; //already done!
		if(DAnalysis::Check_IfMissingDecayProduct(locReaction, loc_i))
			continue;

		Create_InvariantMassHistogram(locReaction, locDecayPID, locUseKinFitResultsFlag, locBaseUniqueName);
		locDecayPIDsUsed.insert(locDecayPID);
	}

	//missing mass
	for(size_t loc_i = 0; loc_i < locReaction->Get_NumReactionSteps(); ++loc_i)
	{
		auto locReactionStep = locReaction->Get_ReactionStep(loc_i);
		set<Particle_t> locMissingDecayPIDsUsed;
		for(size_t loc_j = 0; loc_j < locReactionStep->Get_NumFinalPIDs(); ++loc_j)
		{
			auto locPID = locReactionStep->Get_FinalPID(loc_j);
//cout << "i, j, pid, missing index: " << loc_i << ", " << loc_j << ", " << locPID  << ", " << locReactionStep->Get_MissingParticleIndex() << endl;
			if(locMissingDecayPIDsUsed.find(locPID) != locMissingDecayPIDsUsed.end())
				continue;

			//check if missing particle
			if(int(loc_j) == locReactionStep->Get_MissingParticleIndex())
			{
				if((locNumMissingParticles > 1) || (locNumInclusiveSteps > 0))
					continue;
				if(locUseKinFitResultsFlag && locP4Fit)
					continue; //mass is constrained, will be a spike
				Create_MissingMassSquaredHistogram(locReaction, locPID, locUseKinFitResultsFlag, locBaseUniqueName, 0, {});
			}

			//check if decaying particle
			auto locDecayStepIndex = DAnalysis::Get_DecayStepIndex(locReaction, loc_i, loc_j);
//cout << "decay step index: " << locDecayStepIndex << endl;
			if(locDecayStepIndex <= 0)
				continue; //nope

			//nothing can be missing anywhere, except in it's decay products
			auto locMissingDecayProducts = DAnalysis::Get_MissingDecayProductIndices(locReaction, locDecayStepIndex);
//cout << "num missing total/decay: " << locNumMissingParticles << ", " << locMissingDecayProducts.size() << endl;
			if((locNumMissingParticles - locMissingDecayProducts.size()) > 0)
				continue; //nope

			//check inclusives, same thing
			size_t locNumInclusiveDecayProductSteps = 0;
			for(auto& locParticlePair : locMissingDecayProducts)
			{
				if(locParticlePair.second == DReactionStep::Get_ParticleIndex_Inclusive())
					++locNumInclusiveDecayProductSteps;
			}
//cout << "num inclusives total/decay: " << locNumInclusiveSteps << ", " << locNumInclusiveDecayProductSteps << endl;
			if((locNumInclusiveSteps - locNumInclusiveDecayProductSteps) > 0)
				continue; //nope

//cout << "p4 fit, use kinfit, constrain flag: " << locP4Fit << ", " << locUseKinFitResultsFlag << ", " << locReaction->Get_ReactionStep(locDecayStepIndex)->Get_KinFitConstrainInitMassFlag() << endl;
			if(locP4Fit && locUseKinFitResultsFlag && locReaction->Get_ReactionStep(locDecayStepIndex)->Get_KinFitConstrainInitMassFlag())
				continue; //constrained, will be a spike

			auto locFinalPIDs = locReactionStep->Get_FinalPIDs();
			locFinalPIDs.erase(locFinalPIDs.begin() + loc_j);
			deque<Particle_t> locMissingMassOffOfPIDs(locFinalPIDs.begin(), locFinalPIDs.end());
			Create_MissingMassSquaredHistogram(locReaction, locPID, locUseKinFitResultsFlag, locBaseUniqueName, loc_i, locMissingMassOffOfPIDs);

			locMissingDecayPIDsUsed.insert(locPID);
		}
	}

	//if nothing missing, overall missing mass squared
	if((locNumMissingParticles == 0) && (locNumInclusiveSteps == 0) && (!locUseKinFitResultsFlag || !locP4Fit))
		Create_MissingMassSquaredHistogram(locReaction, Unknown, locUseKinFitResultsFlag, locBaseUniqueName, 0, {});
}

void DReaction_factory_ReactionFilter::Add_PostKinfitTimingCuts(DReaction* locReaction)
{
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction, true));

	//Get, loop over detected PIDs in reaction
	//Will have little effect except for on particles at detached vertices
	auto locFinalPIDs = locReaction->Get_FinalPIDs(-1, false, false, d_AllCharges, false);
	for(auto locPID : locFinalPIDs)
	{
		//Add timing cuts //false: measured data
		auto locTimeCuts = dSourceComboTimeHandler->Get_TimeCuts(locPID);
		for(auto& locSystemPair : locTimeCuts)
			locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, true, locSystemPair.second->Eval(12.0), locPID, locSystemPair.first)); //true: kinfit results
	}
}

void DReaction_factory_ReactionFilter::Create_InvariantMassHistogram(DReaction* locReaction, Particle_t locPID, bool locUseKinFitResultsFlag, string locBaseUniqueName)
{
	pair<float, float> locCutPair;
	if(!dSourceComboP4Handler->Get_InvariantMassCuts(locPID, locCutPair))
		return;

	//determine #bins
	int locNumBins = int((locCutPair.second - locCutPair.first)*1000.0 + 0.001);
	if(locNumBins < 200)
		locNumBins *= 5; //get close to 1000 bins
	if(locNumBins < 500)
		locNumBins *= 2; //get close to 1000 bins

	//build name string
	string locActionUniqueName = string(ParticleType(locPID)) + string("_") + locBaseUniqueName;

	//add histogram action
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, locPID, locUseKinFitResultsFlag, locNumBins, locCutPair.first, locCutPair.second, locActionUniqueName));
}

void DReaction_factory_ReactionFilter::Create_MissingMassSquaredHistogram(DReaction* locReaction, Particle_t locPID, bool locUseKinFitResultsFlag, string locBaseUniqueName, int locMissingMassOffOfStepIndex, const deque<Particle_t>& locMissingMassOffOfPIDs)
{
	pair<TF1*, TF1*> locFuncPair;
	if(!dSourceComboP4Handler->Get_MissingMassSquaredCuts(locPID, locFuncPair))
		return;
	auto locCutPair = std::make_pair(locFuncPair.first->Eval(12.0), locFuncPair.second->Eval(12.0)); //where it's likely widest

	//determine #bins
	int locNumBins = int((locCutPair.second - locCutPair.first)*1000.0 + 0.001);
	if(locNumBins < 200)
		locNumBins *= 5; //get close to 1000 bins
	if(locNumBins < 500)
		locNumBins *= 2; //get close to 1000 bins

	//build name string
	ostringstream locActionUniqueNameStream;
	if((locPID == Unknown) && (locMissingMassOffOfStepIndex == 0))
		locActionUniqueNameStream << locBaseUniqueName;
	else if(locMissingMassOffOfStepIndex == 0)
		locActionUniqueNameStream << ParticleType(locPID) << "_" << locBaseUniqueName;
	else if(locPID == Unknown)
		locActionUniqueNameStream << "Step" << locMissingMassOffOfStepIndex << "_" << locBaseUniqueName;
	else
		locActionUniqueNameStream << ParticleType(locPID) << "_Step" << locMissingMassOffOfStepIndex << "_" << locBaseUniqueName;

	if(dDebugFlag)
	{
		cout << "create miss mass squared action: off step index, kinfit flag, off pids: " << locMissingMassOffOfStepIndex << ", " << locUseKinFitResultsFlag;
		for(auto& locPID : locMissingMassOffOfPIDs)
			cout << ", " << locPID;
		cout << endl;
	}
	//add histogram action
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, locMissingMassOffOfStepIndex, locMissingMassOffOfPIDs, locUseKinFitResultsFlag, locNumBins, locCutPair.first, locCutPair.second, locActionUniqueNameStream.str()));
}

/*********************************************************************************** UTILITY FUNCTIONS ***********************************************************************************/

bool DReaction_factory_ReactionFilter::Convert_StringToPID(string locString, Particle_t& locPID, bool& locIsMissingFlag)
{
	if(locString[0] == 'm')
	{
		locIsMissingFlag = true;
		locString = locString.substr(1);
	}
	else
		locIsMissingFlag = false;

	istringstream locIStream(locString);
	int locPIDInt;
	locIStream >> locPIDInt;
	if(locIStream.fail())
		return false;

	locPID = (Particle_t)locPIDInt;
	return true;
}

string DReaction_factory_ReactionFilter::Create_StepNameString(const DReactionStepTuple& locStepTuple, bool locFirstStepFlag)
{
	string locNameString = "";
	auto locFirstStepTargetPID = locFirstStepFlag ? std::get<1>(locStepTuple) : Unknown;
	if(!locFirstStepFlag)
	{
		locNameString = ShortName(std::get<0>(locStepTuple));
		auto locSecondPID = std::get<1>(locStepTuple);
		if(locSecondPID != Unknown)
			locNameString += ShortName(locSecondPID);
		locNameString += "_";
	}
	for(auto& locFinalPID : std::get<2>(locStepTuple))
	{
		if(locFinalPID == locFirstStepTargetPID)
			continue; //target PID is understood
		locNameString += ShortName(locFinalPID);
	}
	auto locMissFinalPID = std::get<3>(locStepTuple);
	if(locMissFinalPID != Unknown)
		locNameString += string("miss") + ShortName(locMissFinalPID);

	if(std::get<4>(locStepTuple) == DReactionStep::Get_ParticleIndex_Inclusive())
		locNameString += "inc";
	return locNameString;
}

map<size_t, tuple<string, string, string, vector<string>>> DReaction_factory_ReactionFilter::Parse_Input(void)
{
	//Get input channel info
	//key is reaction#, 1st string: name (if any) //2nd string: value for 1st decay step //3rd string: value for flags //string vector: value for decays
	map<size_t, tuple<string, string, string, vector<string>>> locInputStrings;
	map<string, string> locParameterMap; //parameter key - filter, value
	gPARMS->GetParameters(locParameterMap, "Reaction"); //gets all parameters with this filter at the beginning of the key
	for(auto locParamPair : locParameterMap)
	{
		if(dDebugFlag)
			cout << "param pair: " << locParamPair.first << ", " << locParamPair.second << endl;

		//make sure that what follows "Reaction," up to the colon (if any) is a number
		auto locColonIndex = locParamPair.first.find(':');
		auto locPreColonName = locParamPair.first.substr(0, locColonIndex);
		if(dDebugFlag)
			cout << "colon index, pre-colon name: " << locColonIndex << ", " << locPreColonName << endl;

		istringstream locIStream(locPreColonName);
		size_t locReactionNumber;
		locIStream >> locReactionNumber;
		if(locIStream.fail())
			continue; //must be for a different use
		if(dDebugFlag)
			cout << "reaction #: " << locReactionNumber << endl;

		//hack so that don't get warning message about no default
		string locKeyValue;
		string locFullParamName = string("Reaction") + locParamPair.first; //have to add back on the filter
		gPARMS->SetDefaultParameter(locFullParamName, locKeyValue);

		//save it in the input map
		if(locColonIndex == string::npos)
			std::get<1>(locInputStrings[locReactionNumber]) = locKeyValue;
		else
		{
			auto locPostColonName = locParamPair.first.substr(locColonIndex + 1);
			if(locPostColonName.substr(0, 4) == "Name")
				std::get<0>(locInputStrings[locReactionNumber]) = locKeyValue;
			else if(locPostColonName.substr(0, 5) == "Flags")
				std::get<2>(locInputStrings[locReactionNumber]) = locKeyValue;
			else
				std::get<3>(locInputStrings[locReactionNumber]).push_back(locKeyValue);
		}

		if(dDebugFlag)
		{
			cout << "reaction #, tuple strings: " << locReactionNumber << ", " << std::get<0>(locInputStrings[locReactionNumber]) << ", " << std::get<1>(locInputStrings[locReactionNumber]) << ", " << std::get<2>(locInputStrings[locReactionNumber]);
			for(auto& locTempString : std::get<3>(locInputStrings[locReactionNumber]))
				cout << ", " << locTempString;
			cout << endl;
		}

	}
	return locInputStrings;
}

bool DReaction_factory_ReactionFilter::Parse_StepPIDString(string locStepString, DReactionStepTuple& locStepTuple)
{
	//return tuple: initial pid, target/2nd-beam pid, detected final pids, missing final pid (if any), missing particle index
	locStepTuple = std::make_tuple(Unknown, Unknown, vector<Particle_t>{}, Unknown, DReactionStep::Get_ParticleIndex_None());

	//find separator
	auto locStateSeparationIndex = locStepString.find("__");
	if(locStateSeparationIndex == string::npos)
		return false;

	//start with initial state
	{
		auto locInitStateString = locStepString.substr(0, locStateSeparationIndex);
		auto locUnderscoreIndex = locInitStateString.find("_");
		auto locInitParticleString = (locUnderscoreIndex != string::npos) ? locInitStateString.substr(0, locUnderscoreIndex) : locInitStateString;

		Particle_t locPID;
		bool locIsMissingFlag;
		if(!Convert_StringToPID(locInitParticleString, locPID, locIsMissingFlag))
			return false;
		std::get<0>(locStepTuple) = locPID;
		if(locIsMissingFlag)
			std::get<4>(locStepTuple) = DReactionStep::Get_ParticleIndex_Initial();

		if(locUnderscoreIndex != string::npos)
		{
			locInitStateString = locInitStateString.substr(locUnderscoreIndex + 1);
			if(!Convert_StringToPID(locInitStateString, locPID, locIsMissingFlag))
				return false;
			std::get<1>(locStepTuple) = locPID;
			if(locIsMissingFlag)
				std::get<4>(locStepTuple) = DReactionStep::Get_ParticleIndex_SecondBeam();
		}
	}

	string locRemainingStepString = locStepString.substr(locStateSeparationIndex + 2);
	while(true)
	{
		auto locUnderscoreIndex = locRemainingStepString.find("_");
		auto locParticleString = (locUnderscoreIndex != string::npos) ? locRemainingStepString.substr(0, locUnderscoreIndex) : locRemainingStepString;
		if(dDebugFlag)
			cout << "remaining string, underscore index, particle string: " << locRemainingStepString << ", " << locUnderscoreIndex << ", " << locParticleString << endl;

		Particle_t locPID = Unknown;
		bool locIsMissingFlag = false;
		if(!Convert_StringToPID(locParticleString, locPID, locIsMissingFlag))
		{
			cout << "BUILDING DREACTION, STEP PID STRING " << locStepString << " NOT RECOGNIZED." << endl;
			return false;
		}
		if(locIsMissingFlag)
		{
			if(locPID != Unknown)
				std::get<3>(locStepTuple) = locPID;
			else
				std::get<4>(locStepTuple) = DReactionStep::Get_ParticleIndex_Inclusive();
		}
		else
			std::get<2>(locStepTuple).push_back(locPID);

		if(locUnderscoreIndex == string::npos)
			break;
		locRemainingStepString = locRemainingStepString.substr(locUnderscoreIndex + 1);
	}

	if(std::get<3>(locStepTuple) != Unknown)
		std::get<4>(locStepTuple) = std::get<2>(locStepTuple).size();

	if(dDebugFlag)
	{
		cout << "step tuple: init pid, 2nd init pid, #final pids, missing pid, missing index: " << std::get<0>(locStepTuple) << ", " << std::get<1>(locStepTuple);
		cout << ", " << std::get<2>(locStepTuple).size() << ", " << std::get<3>(locStepTuple) << ", " << std::get<4>(locStepTuple) << endl;
	}
	return true;
}
