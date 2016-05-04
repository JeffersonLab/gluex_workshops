// $Id$
//
//    File: DEventProcessor_omega_skim_tree.h
// Created: Tue Apr 26 11:29:50 EDT 2016
// Creator: pmatt (on Linux pmattdesktop.jlab.org 2.6.32-573.22.1.el6.x86_64 x86_64)
//

#ifndef _DEventProcessor_omega_skim_tree_
#define _DEventProcessor_omega_skim_tree_

#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>

#include <ANALYSIS/DEventWriterROOT.h>
#include <HDDM/DEventWriterREST.h>
#include <ANALYSIS/DHistogramActions.h>

#include "DFactoryGenerator_omega_skim_tree.h"

using namespace jana;
using namespace std;

class DEventProcessor_omega_skim_tree : public jana::JEventProcessor
{
	public:
		const char* className(void){return "DEventProcessor_omega_skim_tree";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber);	///< Called every time a new run number is detected.
		jerror_t evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber);	///< Called every event.
		jerror_t erun(void);						///< Called every time run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		//For non-custom reaction-independent histograms, it is recommended that you simultaneously run the monitoring_hists plugin instead of defining them here

		//For making EventStore skim_trees
		int Get_FileNumber(JEventLoop* locEventLoop) const;
		ofstream dEventStoreSkimStream;
};

#endif // _DEventProcessor_omega_skim_tree_

