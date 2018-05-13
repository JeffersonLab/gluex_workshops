// $Id$
//
//    File: DReaction_factory_p3pi_FCALStudy_P4V.h
// Created: Wed May  2 14:02:29 EDT 2018
// Creator: jzarling (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DReaction_factory_p3pi_FCALStudy_P4V_
#define _DReaction_factory_p3pi_FCALStudy_P4V_

#include <iostream>
#include <iomanip>

#include <JANA/JFactory.h>
#include <ANALYSIS/DReaction.h>
#include <ANALYSIS/DHistogramActions.h>
#include <ANALYSIS/DCutActions.h>

using namespace std;
using namespace jana;

class DReaction_factory_p3pi_FCALStudy_P4V : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_p3pi_FCALStudy_P4V()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "p3pi_FCALStudy_P4V";}
		string m_treeno;

	private:
		jerror_t brun(JEventLoop* locEventLoop, int32_t locRunNumber);
		jerror_t evnt(JEventLoop* locEventLoop, uint64_t locEventNumber);
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_p3pi_FCALStudy_P4V_

