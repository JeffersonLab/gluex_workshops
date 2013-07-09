// $Id$
//
//    File: DReaction_factory_workshop_s3b.h
// Created: Mon Jul  8 22:52:22 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DReaction_factory_workshop_s3b_
#define _DReaction_factory_workshop_s3b_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_workshop_s3b : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_workshop_s3b()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "workshop_s3b";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_workshop_s3b_

