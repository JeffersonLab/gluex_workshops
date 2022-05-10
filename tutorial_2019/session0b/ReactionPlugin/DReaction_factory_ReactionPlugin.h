// $Id$
//
//    File: DReaction_factory_ReactionPlugin.h
// Created: Tue Jun 19 13:47:31 EDT 2018
// Creator: aaustreg (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DReaction_factory_ReactionPlugin_
#define _DReaction_factory_ReactionPlugin_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_ReactionPlugin : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_ReactionPlugin()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "ReactionPlugin";}

	private:
		jerror_t evnt(JEventLoop* locEventLoop, uint64_t locEventNumber);
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_ReactionPlugin_

