// $Id$
//
//    File: DReaction_factory_workshop_s3a.cc
// Created: Mon Jul  8 21:53:19 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//


#include "DReaction_factory_workshop_s3a.h"

//------------------
// init
//------------------
jerror_t DReaction_factory_workshop_s3a::init(void)
{
	// Make as many DReaction objects as desired
	DReactionStep* locReactionStep = NULL;
	DReaction* locReaction = new DReaction("workshop_s3a");

	/**************************************************** workshop_s3a Reaction Steps ****************************************************/

	/* 
	//Required: DReactionSteps to specify the channel and decay chain you want to study
		//Particles are of type Particle_t, an enum defined in sim-recon/src/libraries/include/particleType.h

	//Example: g, p -> pi+, pi-, pi0, (p)
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Gamma);
	locReactionStep->Set_TargetParticleID(Proton);
	locReactionStep->Add_FinalParticleID(PiPlus);
	locReactionStep->Add_FinalParticleID(PiMinus);
	locReactionStep->Add_FinalParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Proton, true); //true: proton missing
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak

	//Example: pi0 -> g, g
	locReactionStep = new DReactionStep();
	locReactionStep->Set_InitialParticleID(Pi0);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReactionStep->Add_FinalParticleID(Gamma);
	locReaction->Add_ReactionStep(locReactionStep);
	dReactionStepPool.push_back(locReactionStep); //register so will be deleted later: prevent memory leak
	*/

	_data.push_back(locReaction); //Register the DReaction with the factory

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DReaction_factory_workshop_s3a::fini(void)
{
	for(size_t loc_i = 0; loc_i < dReactionStepPool.size(); ++loc_i)
		delete dReactionStepPool[loc_i]; //cleanup memory
	return NOERROR;
}

