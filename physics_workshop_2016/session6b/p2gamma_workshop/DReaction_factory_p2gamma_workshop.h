// $Id$
//
//    File: DReaction_factory_p2gamma_workshop.h
// Created: Fri Apr 29 13:18:03 EDT 2016
// Creator: jrsteven (on Linux halldw1.jlab.org 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _DReaction_factory_p2gamma_workshop_
#define _DReaction_factory_p2gamma_workshop_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_p2gamma_workshop : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_p2gamma_workshop()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "p2gamma_workshop";}

	private:
		jerror_t brun(JEventLoop* locEventLoop, int32_t locRunNumber);
		jerror_t evnt(JEventLoop* locEventLoop, uint64_t locEventNumber);
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		double dBeamBunchPeriod;
		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_p2gamma_workshop_

