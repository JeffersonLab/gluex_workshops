// $Id$
//
//    File: DCustomAction_Dalitz_n3pi.cc
// Created: Tue Jul  2 21:43:19 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#include "DCustomAction_Dalitz_n3pi.h"

void DCustomAction_Dalitz_n3pi::Initialize(JEventLoop* locEventLoop)
{
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
		locHistTitle += ";#it{#pi}^{+}_{1}#it{#pi}^{-} Invariant Mass Squared (GeV/c^{2})^{2}";
		locHistTitle += ";#it{#pi}^{+}_{2}#it{#pi}^{-} Invariant Mass Squared (GeV/c^{2})^{2}";
		if(gDirectory->Get("Dalitz") == NULL) //check to see if already created by another thread
			dDalitzHist = new TH2D("Dalitz", locHistTitle.c_str(), 250, 0.0, 2.5, 250, 0.0, 2.5);
		else //already created by another thread
			dDalitzHist = static_cast<TH2D*>(gDirectory->Get("Dalitz"));
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!
}

bool DCustomAction_Dalitz_n3pi::Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo)
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

	//Calculate the 4-Vectors
	DLorentzVector locP4Sum1 = locPiPlus1->lorentzMomentum() + locPiMinus->lorentzMomentum();
	DLorentzVector locP4Sum2 = locPiPlus2->lorentzMomentum() + locPiMinus->lorentzMomentum();

	//Fill the histogram
	japp->RootWriteLock(); //ACQUIRE ROOT LOCK!!
	{
		dDalitzHist->Fill(locP4Sum1.M2(), locP4Sum2.M2());
	}
	japp->RootUnLock(); //RELEASE ROOT LOCK!!

	return true; //return false if you want to use this action to apply a cut
}

