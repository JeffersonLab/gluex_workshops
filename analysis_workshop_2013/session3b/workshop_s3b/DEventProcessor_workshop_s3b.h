// $Id$
//
//    File: DEventProcessor_workshop_s3b.h
// Created: Mon Jul  8 22:52:23 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DEventProcessor_workshop_s3b_
#define _DEventProcessor_workshop_s3b_

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <ANALYSIS/DParticleCombo.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DFactoryGenerator_workshop_s3b.h"

using namespace jana;
using namespace std;

class DEventProcessor_workshop_s3b : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_workshop_s3b";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, int locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _DEventProcessor_workshop_s3b_

