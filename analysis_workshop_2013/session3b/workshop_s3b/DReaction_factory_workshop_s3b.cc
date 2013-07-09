// $Id$
//
//    File: DReaction_factory_workshop_s3b.cc
// Created: Mon Jul  8 22:52:23 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//


#include "DReaction_factory_workshop_s3b.h"

//------------------
// init
//------------------
jerror_t DReaction_factory_workshop_s3b::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction("workshop_s3b"); //unique name, feel free to change

	/**************************************************** workshop_s3b Reaction Steps ****************************************************/

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

	/**************************************************** workshop_s3b Control Variables ****************************************************/

	// Recommended: Type of kinematic fit to perform (default is d_NoFit)
		//fit types are: d_NoFit, d_P4Fit, d_VertexFit, and d_P4AndVertexFit //p4 also includes invariant mass constraints
	// locReaction->Set_KinFitType(d_P4AndVertexFit);

	// Optional: When generating particle combinations, reject all combinations with a combined tracking confidence level < 0.1% (or any custom number)
	// locReaction->Set_MinCombinedTrackingFOM(0.001);

	// Optional: When generating particle combinations, reject all combinations with a combined PID FOM < 0.1% (or any custom number)
	// locReaction->Set_MinCombinedPIDFOM(0.001);

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_workshop_s3b::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

