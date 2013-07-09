// $Id$
//
//    File: DCustomAction_InvariantMass_n3pi.h
// Created: Tue Jul  2 21:57:46 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DCustomAction_InvariantMass_n3pi_
#define _DCustomAction_InvariantMass_n3pi_

#include <string>
#include <iostream>

#include "JANA/JEventLoop.h"
#include "JANA/JApplication.h"

#include "ANALYSIS/DAnalysisAction.h"
#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DParticleCombo.h"
#include "ANALYSIS/DAnalysisUtilities.h"

using namespace std;
using namespace jana;

class DCustomAction_InvariantMass_n3pi : public DAnalysisAction
{
	public:

		DCustomAction_InvariantMass_n3pi(const DReaction* locReaction, bool locUseKinFitResultsFlag, string locActionUniqueString = "") : 
		DAnalysisAction(locReaction, "Custom_InvariantMass_n3pi", locUseKinFitResultsFlag, locActionUniqueString) {}

		void Initialize(JEventLoop* locEventLoop);

	private:

		bool Perform_Action(JEventLoop* locEventLoop, const DParticleCombo* locParticleCombo);

		//Store any histograms as member variables here
		TH1D* dInvariantMassHist;
};

#endif // _DCustomAction_InvariantMass_n3pi_

