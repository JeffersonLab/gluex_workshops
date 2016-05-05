// $Id$
//
//    File: DReaction_factory_omega.cc
// Created: Mon May  2 12:05:11 EDT 2016
// Creator: pmatt (on Linux pmattdesktop.jlab.org 2.6.32-573.22.1.el6.x86_64 x86_64)
//


#include "DReaction_factory_omega.h"
#include "DCustomAction_dEdxCut.h"

//------------------
// brun
//------------------
jerror_t DReaction_factory_omega::brun(JEventLoop* locEventLoop, int32_t locRunNumber)
{
	vector<double> locBeamPeriodVector;
	locEventLoop->GetCalib("PHOTON_BEAM/RF/beam_period", locBeamPeriodVector);
	dBeamBunchPeriod = locBeamPeriodVector[0];

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DReaction_factory_omega::evnt(JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = NULL; //create with a unique name for each DReaction object. CANNOT (!) be "Thrown"

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/************************************************** omega Reaction Definition *************************************************/

	locReaction = new DReaction("omega");

	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	//g, p -> omega, p
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(omega);
	locReactionStep->Add_FinalParticleID(Proton);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//omega -> pi+, pi-, pi0
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(omega);
	locReactionStep->Add_FinalParticleID(PiPlus);
	locReactionStep->Add_FinalParticleID(PiMinus);
	locReactionStep->Add_FinalParticleID(Pi0);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//pi0 -> g, g
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	/**************************************************** omega Control Settings ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DReaction.h
		//Options: d_NoFit (default), d_P4Fit, d_VertexFit, d_P4AndVertexFit
		//P4 fits automatically constrain decaying particle masses, unless they are manually disabled
	locReaction->Set_KinFitType(d_P4AndVertexFit);

	// Highly Recommended: When generating particle combinations, reject all beam photons that match to a different RF bunch (delta_t > 1.002 ns)
	locReaction->Set_MaxPhotonRFDeltaT(0.5*dBeamBunchPeriod); //should be minimum cut value

	// Highly Recommended: Cut on number of extra "good" tracks. "Good" tracks are ones that survive the "PreSelect" (or user custom) factory.
		// Current (09/26/2014): "Good" tracks have a detector-hit match, and tracking FOM > 0.0027 (+/- 3 sigma).
		// Important: Keep cut large: Can have many ghost and accidental tracks that look "good"
	locReaction->Set_MaxExtraGoodTracks(4);

	// Highly Recommended: Enable ROOT TTree output for this DReaction
	locReaction->Enable_TTreeOutput("tree_omega.root"); //string is file name (must end in ".root"!!): doen't need to be unique, feel free to change

	/************************************************** omega Pre-Combo Custom Cuts *************************************************/

	// Highly Recommended: Very loose invariant mass cuts, applied during DParticleComboBlueprint construction
	locReaction->Set_InvariantMassCut(Pi0, 0.05, 0.22);
	locReaction->Set_InvariantMassCut(omega, 0.5, 1.1);

	// Highly Recommended: Very loose DAnalysisAction cuts, applied just after creating the combination (before saving it)
	locReaction->Add_ComboPreSelectionAction(new DCutAction_MissingMassSquared(locReaction, false, -0.08, 0.08));

	/**************************************************** omega Analysis Actions ****************************************************/

	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions_*.h and ANALYSIS/DCutActions.h
		//If a histogram action is repeated, it should be created with a unique name (string) to distinguish them

	// HISTOGRAM PID
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));

	// CUT PID
	// SYS_TOF, SYS_BCAL, SYS_FCAL, ...: DetectorSystem_t: Defined in libraries/include/GlueX.h
	// SYS_NULL: All systems. Unknown: All particles
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 2.0, Unknown, SYS_NULL)); //cut at delta-t +/- 2.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCustomAction_dEdxCut(locReaction, false)); //false: focus on keeping signal

	// HISTOGRAM MASSES: PRE-KINFIT //false/true: measured/kinfit data
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 170, 0.05, 0.22, "Pi0_PreKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, omega, false, 300, 0.5, 1.1, "Omega_PreKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, false, 400, -0.08, 0.08, "PreKinFit"));
	deque<Particle_t> locXPIDs, locYPIDs;
	locXPIDs.push_back(PiPlus);  locXPIDs.push_back(PiMinus);
	locYPIDs.push_back(PiPlus);  locYPIDs.push_back(PiMinus);  locYPIDs.push_back(Pi0);
	locReaction->Add_AnalysisAction(new DHistogramAction_2DInvariantMass(locReaction, 1, locXPIDs, locYPIDs, false, 300, 0.5, 1.1, 300, 0.5, 1.1, "PreKinFit"));

	// MISSING TRANSVERSE MOMENTUM
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false, 400, 0.0, 0.4, "PreKinFit"));

	// KINEMATIC FIT
	locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); // require kinfit converges

	// HISTOGRAM MASSES: POST-KINFIT //false/true: measured/kinfit data
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 170, 0.05, 0.22, "Pi0_PostKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, false, 400, -0.08, 0.08, "PostKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, omega, false, 300, 0.5, 1.1, "Omega_PostKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, omega, true, 300, 0.5, 1.1, "Omega_KinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_2DInvariantMass(locReaction, 1, locXPIDs, locYPIDs, false, 300, 0.5, 1.1, 300, 0.5, 1.1, "PostKinFit"));

	// KINEMATIC FIT CUT
	locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 5.73303E-7)); // +/- 5 sigma confidence level cut

	// HISTOGRAM MASSES: KINFIT CUT //false/true: measured/kinfit data
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 170, 0.05, 0.22, "Pi0_KinFitCut"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, false, 400, -0.08, 0.08, "KinFitCut"));
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, omega, false, 300, 0.5, 1.1, "Omega_KinFitCut"));
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, omega, true, 300, 0.5, 1.1, "Omega_KinFit_KinFitCut"));
	locReaction->Add_AnalysisAction(new DHistogramAction_2DInvariantMass(locReaction, 1, locXPIDs, locYPIDs, false, 300, 0.5, 1.1, 300, 0.5, 1.1, "KinFitCut"));

	// MISSING TRANSVERSE MOMENTUM
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false, 400, 0.0, 0.4, "KinFitCut"));

	// KINEMATICS
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: kinematic-fit data
	locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));

	// KINEMATICS COMPARISON
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboGenReconComparison(locReaction, false, "Measured"));
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboGenReconComparison(locReaction, true, "KinFit"));

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_omega::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

