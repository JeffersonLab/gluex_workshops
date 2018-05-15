#ifndef _DReaction_factory_check_kinfitter_
#define _DReaction_factory_check_kinfitter_

#include "JANA/JFactory.h"
#include "JANA/JEventLoop.h"

#include "particleType.h"

#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DReactionStep.h"
#include "ANALYSIS/DHistogramActions.h"
#include "ANALYSIS/DCutActions.h"
#include "reactionSpecifier.h"

using namespace std;
using namespace jana;

class DReaction_factory_check_kinfitter : public jana::JFactory<DReaction>
{
	public:
		DReaction_factory_check_kinfitter(){SetFactoryFlag(PERSISTANT);}; // Setting the PERSISTANT prevents JANA from deleting the objects every event so we only create them once.
		~DReaction_factory_check_kinfitter(){};
		const char* Tag(void){return "check_kinfitter";}
    
    reactionSpecifier *rs;

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks!
};

#endif // _DReaction_factory_b1pi_hists_

