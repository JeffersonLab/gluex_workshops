// $Id$
//
//    File: DReaction_factory_p3pi_PiPlusMiss_P4Vpi0.cc
// Created: Wed May  2 14:03:24 EDT 2018
// Creator: jzarling (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//


#include "DReaction_factory_p3pi_PiPlusMiss_P4Vpi0.h"
//------------------
// brun
//------------------
jerror_t DReaction_factory_p3pi_PiPlusMiss_P4Vpi0::brun(JEventLoop* locEventLoop, int32_t locRunNumber)
{
	// vector<double> locBeamPeriodVector;
	// locEventLoop->GetCalib("PHOTON_BEAM/RF/beam_period", locBeamPeriodVector);
	// dBeamBunchPeriod = locBeamPeriodVector[0];

	m_treeno = "";
	gPARMS->SetDefaultParameter("TREENO",m_treeno);
	
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DReaction_factory_p3pi_PiPlusMiss_P4Vpi0::evnt(JEventLoop* locEventLoop, uint64_t locEventNumber)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = NULL; //create with a unique name for each DReaction object. CANNOT (!) be "Thrown"

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software
	// DReaction factory: https://halldweb1.jlab.org/wiki/index.php/Analysis_DReaction

	/************************************************** p3pi_PiPlusMiss_P4Vpi0 Reaction Definition *************************************************/

	locReaction = new DReaction("p3pi_PiPlusMiss_P4Vpi0");

	// gamma p -> omega p
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(omega);
	locReactionStep->Add_FinalParticleID(Proton);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//omega -> pi0 pi+ pi- 
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(omega);
	locReactionStep->Add_FinalParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(PiPlus,true);
	locReactionStep->Add_FinalParticleID(PiMinus);
	//locReactionStep->Set_ApplyKinFitMassConstraintOnInitialParticleFlag(false); //Paul commented this out for me
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

    //pi0 -> g + g
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Pi0);
	// locReactionStep->Add_FinalParticleID(Gamma,true);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Set_KinFitConstrainInitMassFlag(true);

	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	/**************************************************** p3pi_PiPlusMiss_P4Vpi0 Control Settings ****************************************************/

	// Highly Recommended: Set EventStore skim query (use with "eventstore" source)
		// This will skip creating particle combos for events that aren't in the skims you list
		// Query should be comma-separated list of skims to boolean-AND together
	//locReaction->Set_EventStoreSkims("myskim1,myskim2,myskim3"); //boolean-AND of skims

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are of type DKinFitType, an enum defined in sim-recon/src/libraries/ANALYSIS/DReaction.h
		//Options: d_NoFit (default), d_P4Fit, d_VertexFit, d_P4AndVertexFit
		//P4 fits automatically constrain decaying particle masses, unless they are manually disabled
	locReaction->Set_KinFitType(d_P4AndVertexFit);

	// Highly Recommended: When generating particle combinations, reject all beam photons that match to a different RF bunch
	locReaction->Set_NumPlusMinusRFBunches(2); //1: 3 bunches, -1, 0, 1

	// Highly Recommended: Cut on number of extra "good" tracks. "Good" tracks are ones that survive the "PreSelect" (or user custom) factory.
		// Important: Keep cut large: Can have many ghost and accidental tracks that look "good"
	locReaction->Set_MaxExtraGoodTracks(6);

	// Highly Recommended: Enable ROOT TTree output for this DReaction
	// string is file name (must end in ".root"!!): doen't need to be unique, feel free to change
	string root_out_name = "tree_p3pi_PiPlusMiss_P4Vpi0_"+m_treeno+".root";
	locReaction->Enable_TTreeOutput(root_out_name, true); //true/false: do/don't save unused hypotheses

	/**************************************************** p3pi_PiPlusMiss_P4Vpi0 Analysis Actions ****************************************************/

	locReaction->Add_AnalysisAction(new DCutAction_AllVertexZ(locReaction, 48., 78.));
	locReaction->Add_AnalysisAction(new DCutAction_MaxTrackDOCA(locReaction, Proton, 1,"proton_DOCA"));
	locReaction->Add_AnalysisAction(new DCutAction_MaxTrackDOCA(locReaction, PiPlus, 1,"PiPlus_DOCA"));
	locReaction->Add_AnalysisAction(new DCutAction_MaxTrackDOCA(locReaction, PiMinus, 1,"PiMinus_DOCA"));
	locReaction->Add_AnalysisAction(new DCutAction_MissingMassSquared(locReaction, false, -0.1,0.15,"MeasuredMMCut")); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_MissingMass(locReaction, 0 , Proton, false, 0.2, 2, "MissingOmegaMass"));

	//Temporary, probably
	locReaction->Add_AnalysisAction(new DCutAction_BeamEnergy(locReaction,false,8.1,9.01,"cohE_cut"));  //For testing: only consider coh. beam E
	
	locReaction->Add_AnalysisAction(new DCutAction_InvariantMass(locReaction,Pi0,false,0.07,0.2,"Pi0_masscut"));  //Pi0 measured mass cut
	
	
	//locReaction->Add_AnalysisAction(new DCutAction_NoPIDHit(locReaction, Proton)); //for p candidates, cut tracks with no PID hit
	//locReaction->Add_AnalysisAction(new DCutAction_NoPIDHit(locReaction, PiPlus)); //for p candidates, cut tracks with no PID hit
	//locReaction->Add_AnalysisAction(new DCutAction_NoPIDHit(locReaction, PiMinus)); //for p candidates, cut tracks with no PID hit	
	// Time of Flight: All charged 1 ns 
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 0.6, Proton, SYS_TOF)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 0.5, PiPlus, SYS_TOF)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 0.5, PiMinus, SYS_TOF)); //cut at delta-t +/- 1.0 //false: measured data
	// BCAL: 3 ns all
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, Proton, SYS_BCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, PiPlus, SYS_BCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, PiMinus, SYS_BCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, Gamma, SYS_BCAL)); //cut at delta-t +/- 1.0 //false: measured data
	// FCAL: 4 ns all (may want to play with)
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 2.0, Proton, SYS_FCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 2.0, PiPlus, SYS_FCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 2.0, PiMinus, SYS_FCAL)); //cut at delta-t +/- 1.0 //false: measured data
	locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 2.0, Gamma, SYS_FCAL)); //cut at delta-t +/- 1.0 //false: measured data

	locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 1e-50)); //5% confidence level cut //require kinematic fit converges
	
	deque<Particle_t> pipl_pim_deque;
	pipl_pim_deque.push_back(PiPlus);
	pipl_pim_deque.push_back(PiMinus);
	
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, 0, Proton, false, 800,0.,2,"Missing_omega_measured"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, 0, Proton, true, 800,0.,2,"Missing_omega_KF"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, 2, Gamma, false, 1000,-0.5,0.5,"MissingGammaMeasuredM2"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMassSquared(locReaction, 1, pipl_pim_deque, false,800,-0.5,0.5,"Missing_pi0__m2_measured"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, 1, pipl_pim_deque, false,800,0,0.5,"Missing_pi0_measured"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, 1, pipl_pim_deque, true,800,0,0.5,"Missing_pi0"));
	
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));
	locReaction->Add_AnalysisAction(new DHistogramAction_TruePID(locReaction)); //momentum distributions of tracks with true/false PID (if thrown data available)

	// Transverse Momentum //Recommended for no-missing-particle reactions only!
	// locReaction->Add_AnalysisAction(new DHistogramAction_MissingTransverseMomentum(locReaction, false, 1000, 0.0, 1.0)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DCutAction_TransverseMomentum(locReaction, 0.4)); //Max Missing Pt of 0.4 GeV

	// Kinematic Fit Results
	// locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	// locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); //0% confidence level cut //require kinematic fit converges

	// Kinematics
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: fill histograms with measured particle data
	// locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: fill histograms with kinematic-fit particle data //"KinFit": unique name since action type is repeated
	locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
		
		
		
		
		
	/*
	// Recommended: Analysis actions automatically performed by the DAnalysisResults factories to histogram useful quantities.
		//These actions are executed sequentially, and are executed on each surviving (non-cut) particle combination 
		//Pre-defined actions can be found in ANALYSIS/DHistogramActions_*.h and ANALYSIS/DCutActions.h
		//If a histogram action is repeated, it should be created with a unique name (string) to distinguish them

	// HISTOGRAM PID
	locReaction->Add_AnalysisAction(new DHistogramAction_PID(locReaction));

	// CUT PID
	// SYS_TOF, SYS_BCAL, SYS_FCAL, ...: DetectorSystem_t: Defined in libraries/include/GlueX.h
	// locReaction->Add_AnalysisAction(new DCutAction_EachPIDFOM(locReaction, 5.73303E-7));
	// locReaction->Add_AnalysisAction(new DCutAction_PIDDeltaT(locReaction, false, 1.0, Proton, SYS_TOF)); //cut at delta-t +/- 1.0 //false: measured data
	// locReaction->Add_AnalysisAction(new DCutAction_PIDTimingBeta(locReaction, 0.0, 0.9, Neutron, SYS_BCAL)); //min/max beta cut for neutrons
	// locReaction->Add_AnalysisAction(new DCutAction_NoPIDHit(locReaction, KPlus)); //for K+ candidates, cut tracks with no PID hit

	// HISTOGRAM MASSES //false/true: measured/kinfit data
	locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 600, 0.0, 0.3, "Pi0_PreKinFit"));
	locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, false, 1000, 0.7, 1.2, "PreKinFit"));

	// KINEMATIC FIT
	// locReaction->Add_AnalysisAction(new DHistogramAction_KinFitResults(locReaction, 0.05)); //5% confidence level cut on pull histograms only
	// locReaction->Add_AnalysisAction(new DCutAction_KinFitFOM(locReaction, 0.0)); //0% confidence level cut //require kinematic fit converges

	// HISTOGRAM MASSES //false/true: measured/kinfit data
	//locReaction->Add_AnalysisAction(new DHistogramAction_InvariantMass(locReaction, Pi0, false, 600, 0.0, 0.3, "Pi0_PostKinFit"));
	//locReaction->Add_AnalysisAction(new DHistogramAction_MissingMass(locReaction, false, 1000, 0.7, 1.2, "PostKinFit"));

	// Kinematics
	locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, false)); //false: measured data
	// locReaction->Add_AnalysisAction(new DHistogramAction_ParticleComboKinematics(locReaction, true, "KinFit")); //true: kinematic-fit data
	locReaction->Add_AnalysisAction(new DHistogramAction_TrackVertexComparison(locReaction));
	*/

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_p3pi_PiPlusMiss_P4Vpi0::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

