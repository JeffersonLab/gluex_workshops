// $Id$
//
//    File: DCustomAction_dEdxCut.h
// Created: Thu Oct  1 11:18:05 EDT 2015
// Creator: pmatt (on Darwin Pauls-MacBook-Pro-2.local 13.4.0 i386)
//

#ifndef _DCustomAction_dEdxCut_
#define _DCustomAction_dEdxCut_

#include <string>
#include <iostream>

#include "TF1.h"

#include "JANA/JEventLoop.h"
#include "JANA/JApplication.h"

#include "ANALYSIS/DAnalysisAction.h"
#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DParticleCombo.h"
#include "ANALYSIS/DAnalysisUtilities.h"

using namespace std;
using namespace jana;

class DCustomAction_dEdxCut : public DAnalysisAction
{
	public:

		DCustomAction_dEdxCut(const DReaction* locReaction, bool locMaxRejectionFlag = false, string locActionUniqueString = "") :
		DAnalysisAction(locReaction, "Custom_dEdxCut", false, locActionUniqueString),
		dMaxRejectionFlag(locMaxRejectionFlag) {}

		void Initialize(JEventLoop* locEventLoop);

		bool Cut_dEdx(const DChargedTrackHypothesis* locChargedTrackHypothesis) const;
		bool Cut_dEdx(Particle_t locPID, double locP, double locdEdx, bool locHasNoTimeInfoFlag) const;

	private:

		bool Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo);

		// Optional: Useful utility functions.
		// const DAnalysisUtilities* dAnalysisUtilities;

		//specify whether to focus on keeping signal or rejecting background
		//however, if the track has timing information, always try to keep as many good tracks as possible
			//assume background can be rejected by timing PID cut
			//otherwise, would cut almost all tracks above p = 1 GeV/c
		bool dMaxRejectionFlag; // if false, keep as many good tracks as possible. if true, reject as many wrong tracks as possible

		//Store any histograms as member variables here
		TF1* dFunc_dEdxCut_SelectHeavy; //e.g. proton //if dMaxRejectionFlag = true, then used to cut all heavy
		TF1* dFunc_dEdxCut_SelectLight; //e.g. pion, kaon //if dMaxRejectionFlag = true, then used to cut all light
};

#endif // _DCustomAction_dEdxCut_
