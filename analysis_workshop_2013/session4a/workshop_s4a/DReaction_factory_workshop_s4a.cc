// $Id$
//
//    File: DReaction_factory_workshop_s4a.cc
// Created: Mon Jul  8 23:19:34 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//


#include "DReaction_factory_workshop_s4a.h"

//------------------
// init
//------------------
jerror_t DReaction_factory_workshop_s4a::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction("workshop_s4a"); //unique name, feel free to change

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/**************************************************** workshop_s4a Reaction Steps ****************************************************/

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

	/**************************************************** workshop_s4a Control Variables ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DKinFitResults.h
	locReaction->Set_KinFitType(d_P4AndVertexFit); //simultaneously constrain apply four-momentum conservation, invariant masses, and common-vertex constraints

	// Optional: When generating particle combinations, reject all combinations with a combined tracking confidence level < 0.1% (or any custom number)
	locReaction->Set_MinCombinedTrackingFOM(0.001);

	// Optional: When generating particle combinations, reject all combinations with a combined PID FOM < 0.1% (or any custom number)
	locReaction->Set_MinCombinedPIDFOM(0.001);

	/**************************************************** workshop_s4a Analysis Actions ****************************************************/

	/*
	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions.h and ANALYSIS/DCutActions.h

	// PID
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));
	locReaction->Add_AnalysisAction(new DHistogramAction_TruePID(locReaction)); //momentum distributions of tracks with true/false PID (if thrown data available)

	// Kinematic Fit Results
	locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only

	// Kinematics
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: fill histograms with measured particle data
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: fill histograms with kinematic-fit particle data //"KinFit": unique name since action type is repeated
	locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
	*/

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_workshop_s4a::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

