// $Id$
//
//    File: DEventProcessor_p3pi_PiPlusMiss_P4Vpi0.h
// Created: Wed May  2 14:03:24 EDT 2018
// Creator: jzarling (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DEventProcessor_p3pi_PiPlusMiss_P4Vpi0_
#define _DEventProcessor_p3pi_PiPlusMiss_P4Vpi0_

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DFactoryGenerator_p3pi_PiPlusMiss_P4Vpi0.h"

using namespace jana;
using namespace std;

class DEventProcessor_p3pi_PiPlusMiss_P4Vpi0 : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_p3pi_PiPlusMiss_P4Vpi0";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here

		//For making EventStore skims
		int Get_FileNumber(JEventLoop* locEventLoop) const;
		ofstream dEventStoreSkimStream;
};

#endif // _DEventProcessor_p3pi_PiPlusMiss_P4Vpi0_

