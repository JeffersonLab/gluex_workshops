// $Id$
//
//    File: DCustomAction_dEdxCut.cc
// Created: Thu Oct  1 11:18:05 EDT 2015
// Creator: pmatt (on Darwin Pauls-MacBook-Pro-2.local 13.4.0 i386)
//

#include "DCustomAction_dEdxCut.h"

void DCustomAction_dEdxCut::Initialize(JEventLoop* locEventLoop)
{
	//Optional: Create histograms and/or modify member variables.
	//Create any histograms/trees/etc. within a ROOT lock. 
		//This is so that when running multithreaded, only one thread is writing to the ROOT file at a time. 

	//CREATE THE FUNCTIONS
	//Since we are creating functions, the contents of gDirectory will be modified: must use JANA-wide ROOT lock
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		string locFuncName = "df_dEdxCut_SelectHeavy"; //e.g. proton
		if(gDirectory->Get(locFuncName.c_str()) != NULL) //already created by another thread
			dFunc_dEdxCut_SelectHeavy = static_cast<TF1*>(gDirectory->Get(locFuncName.c_str()));
		else
		{
			dFunc_dEdxCut_SelectHeavy = new TF1(locFuncName.c_str(), "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 12.0);
			dFunc_dEdxCut_SelectHeavy->SetParameters(4.0, 2.5, 1.25);
		}

		locFuncName = "df_dEdxCut_SelectLight"; //e.g. pions, kaons
		if(gDirectory->Get(locFuncName.c_str()) != NULL) //already created by another thread
			dFunc_dEdxCut_SelectLight = static_cast<TF1*>(gDirectory->Get(locFuncName.c_str()));
		else
		{
			dFunc_dEdxCut_SelectLight = new TF1(locFuncName.c_str(), "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 12.0);
			dFunc_dEdxCut_SelectLight->SetParameters(4.0, 2.0, 2.5);
		}
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
}

bool DCustomAction_dEdxCut::Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo)
{
	//Write custom code to perform an action on the INPUT DParticleCombo (DParticleCombo)
	//NEVER: Grab DParticleCombo or DAnalysisResults objects (of any tag!) from the JEventLoop within this function
	//NEVER: Grab objects that are created post-kinfit (e.g. DKinFitResults, etc.) from the JEventLoop if Get_UseKinFitResultsFlag() == false: CAN CAUSE INFINITE DEPENDENCY LOOP

	deque<const DKinematicData*> locParticles;
	locParticleCombo->Get_DetectedFinalChargedParticles_Measured(locParticles);
	for(size_t loc_i = 0; loc_i < locParticles.size(); ++loc_i)
	{
		const DChargedTrackHypothesis* locChargedTrackHypothesis = static_cast<const DChargedTrackHypothesis*>(locParticles[loc_i]);
		if(!Cut_dEdx(locChargedTrackHypothesis))
			return false;
	}

	return true; //return false if you want to use this action to apply a cut (and it fails the cut!)
}

bool DCustomAction_dEdxCut::Cut_dEdx(const DChargedTrackHypothesis* locChargedTrackHypothesis) const
{
	Particle_t locPID = locChargedTrackHypothesis->PID();

	const DTrackTimeBased* locTrackTimeBased = NULL;
	locChargedTrackHypothesis->GetSingle(locTrackTimeBased);

	double locP = locTrackTimeBased->momentum().Mag();

	//if requested max rejection, only do so if no timing information
		//assume time resolution good enough to separate protons and pions
	bool locHasNoTimeInfoFlag = (locChargedTrackHypothesis->dNDF_Timing == 0);

	if(!Cut_dEdx(locPID, locP, locTrackTimeBased->ddEdx_CDC*1.0E6, locHasNoTimeInfoFlag))
		return false;
//	if(!Cut_dEdx(locPID, locP, locTrackTimeBased->ddEdx_FDC*1.0E6, locHasNoTimeInfoFlag))
//		return false;

	return true;
}

bool DCustomAction_dEdxCut::Cut_dEdx(Particle_t locPID, double locP, double locdEdx, bool locHasNoTimeInfoFlag) const
{
	if(ParticleCharge(locPID) < 0)
		return true; //only need to separate q+

	if((ParticleMass(locPID) + 0.0001) >= ParticleMass(Proton))
	{
		//protons
		if(dMaxRejectionFlag && locHasNoTimeInfoFlag) //focus on rejecting background pions
		{
			if(locdEdx < dFunc_dEdxCut_SelectLight->Eval(locP))
				return false;
		}
		else //focus on keeping signal protons
		{
			if(locdEdx < dFunc_dEdxCut_SelectHeavy->Eval(locP))
				return false;
		}
	}
	else
	{
		//pions
		if(dMaxRejectionFlag && locHasNoTimeInfoFlag) //focus on rejecting background protons
		{
			if(locdEdx > dFunc_dEdxCut_SelectHeavy->Eval(locP))
				return false;
		}
		else //focus on keeping signal pions
		{
			if(locdEdx > dFunc_dEdxCut_SelectLight->Eval(locP))
				return false;
		}
	}

	return true;
}

