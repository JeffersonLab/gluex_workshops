// $Id$
//
//    File: DCustomAction_InvariantMass_n3pi.cc
// Created: Tue Jul  2 21:57:46 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#include "DCustomAction_InvariantMass_n3pi.h"

void DCustomAction_InvariantMass_n3pi::Initialize(JEventLoop* locEventLoop)
{
	//Optional: Create histograms and/or modify member variables.
	//Create any histograms/trees/etc. within a ROOT lock. 
		//This is so that when running multithreaded, only one thread is writing to the ROOT file at a time. 
	//When creating a reaction-independent action, only modify member variables within a ROOT lock. 
		//Objects created within a plugin (such as reaction-independent actions) can be accessed by many threads simultaneously. 
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		//Create a folder in the ROOT output file that will contain all of the output ROOT objects (if any) for this action.
			//If another thread has already created the folder, it just changes to it. 
		CreateAndChangeTo_ActionDirectory();

		//	Create a histogram. 
		string locHistTitle = "#it{#gamma}#it{p}#rightarrow#it{#pi}^{+}#it{#pi}^{+}#it{#pi}^{-}(#it{n})";
		locHistTitle += ";#it{#pi}^{+}#it{#pi}^{+}#it{#pi}^{-} Invariant Mass (GeV/c^{2})";
		locHistTitle += ";# Combos / 2 MeV/c^{2}";
		if(gDirectory->Get("InvariantMass") == NULL) //check to see if already created by another thread
			dInvariantMassHist = new TH1D("InvariantMass", locHistTitle.c_str(), 1000, 0.5, 2.5);
		else //already created by another thread
			dInvariantMassHist = static_cast<TH1D*>(gDirectory->Get("InvariantMass"));
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
}

bool DCustomAction_InvariantMass_n3pi::Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo)
{
	//Check whether the user wanted to use the kinematic fit results when performing this action
	bool locUseKinFitResultsFlag = Get_UseKinFitResultsFlag();

	//Get the DParticleCombo objects for which this action has been previously executed.
		//This is useful for determining whether filling a histogram will result in double-counting. 
	deque<pair<const DParticleCombo*, bool> > locPreviousParticleCombos;
	Get_PreviousParticleCombos(locPreviousParticleCombos);

	//Get the step containing the particles
	const DParticleComboStep* locParticleComboStep = locParticleCombo->Get_ParticleComboStep(0);
	//Get the particles, using the kinematic-fit or measured data as requested
	const DKinematicData* locPiPlus1 = locUseKinFitResultsFlag ? locParticleComboStep->Get_FinalParticle(1) : locParticleComboStep->Get_FinalParticle_Measured(1);
	const DKinematicData* locPiPlus2 = locUseKinFitResultsFlag ? locParticleComboStep->Get_FinalParticle(3) : locParticleComboStep->Get_FinalParticle_Measured(3);
	const DKinematicData* locPiMinus = locUseKinFitResultsFlag ? locParticleComboStep->Get_FinalParticle(2) : locParticleComboStep->Get_FinalParticle_Measured(2);

	//Calculate the 4-Vector
	DLorentzVector locP4Sum = locPiPlus1->lorentzMomentum() + locPiPlus2->lorentzMomentum() + locPiMinus->lorentzMomentum();

	//Optional: Fill histograms
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		// Fill any histograms here
		dInvariantMassHist->Fill(locP4Sum.M());
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!

	return true; //return false if you want to use this action to apply a cut
}
