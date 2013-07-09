// $Id$
//
//    File: DReaction_factory_workshop_s4b.cc
// Created: Mon Jul  8 23:41:58 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//


#include "DReaction_factory_workshop_s4b.h"

//------------------
// init
//------------------
jerror_t DReaction_factory_workshop_s4b::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction("workshop_s4b"); //unique name, feel free to change

	/**************************************************** workshop_s4b Reaction Steps ****************************************************/

	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	//g, p -> pi+, pi+, pi-, (n)
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(Neutron, true); //true: neutron missing
	locReactionStep->Add_FinalParticleID(PiPlus);
	locReactionStep->Add_FinalParticleID(PiMinus);
	locReactionStep->Add_FinalParticleID(PiPlus);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	/**************************************************** workshop_s4b Control Variables ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DKinFitResults.h
	locReaction->Set_KinFitType(d_P4AndVertexFit); //simultaneously constrain apply four-momentum conservation, invariant masses, and common-vertex constraints

	// Optional: When generating particle combinations, reject all combinations with a combined tracking confidence level < 0.1% (or any custom number)
	locReaction->Set_MinCombinedTrackingFOM(0.001);

	// Optional: When generating particle combinations, reject all combinations with a combined PID FOM < 0.1% (or any custom number)
	locReaction->Set_MinCombinedPIDFOM(0.001);

	/**************************************************** workshop_s4b Analysis Actions ****************************************************/

	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions.h and ANALYSIS/DCutActions.h

	//Extremely Loose Mass Cuts
		//Used to speed up execution by throwing away obviously-bogus combinations early
	locReaction->Add_AnalysisAction(new DCutAction_MissingMass(locReaction, false, 0.1, 1.6, "Neutron_Loose")); //false: measured data

	// PID
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));
	locReaction->Add_AnalysisAction(new DCutAction_CombinedPIDFOM(locReaction, 0.01)); //1%
	locReaction->Add_AnalysisAction(new DHistogramAction_TruePID(locReaction)); //momentum distributions of tracks with true/false PID (if thrown data available)

	// Missing Mass & Kinematics, Pre-KinFit
		//false's: fill histograms with measured particle data
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, false, 650, 0.3, 1.6));

	// Kinematic Fit Results and Confidence Level Cut
	locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.01)); //1%

	// Missing Mass & Kinematics, Post-KinFit
		//"PostKinFitConLev"'s: unique name since action type is repeated
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "PostKinFit")); //true: kinfit data
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, false, 650, 0.3, 1.6, "PostKinFit")); //false: measured data

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_workshop_s4b::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

