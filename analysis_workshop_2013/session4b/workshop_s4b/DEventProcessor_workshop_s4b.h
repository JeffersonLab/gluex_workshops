// $Id$
//
//    File: DEventProcessor_workshop_s4b.h
// Created: Mon Jul  8 23:41:58 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DEventProcessor_workshop_s4b_
#define _DEventProcessor_workshop_s4b_

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DAnalysisResults.h>

#include "DFactoryGenerator_workshop_s4b.h"

using namespace jana;
using namespace std;

class DEventProcessor_workshop_s4b : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_workshop_s4b";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, int locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//Recommended: Manually executed analysis actions that histogram useful reaction-independent quantities
		DHistogramAction_TrackMultiplicity dHistogramAction_TrackMultiplicity;
		DHistogramAction_ThrownParticleKinematics dHistogramAction_ThrownParticleKinematics;
		DHistogramAction_DetectedParticleKinematics dHistogramAction_DetectedParticleKinematics;
		DHistogramAction_GenReconTrackComparison dHistogramAction_GenReconTrackComparison;
};

#endif // _DEventProcessor_workshop_s4b_

