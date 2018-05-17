// $Id$
//
//    File: DReaction_factory_ReactionFilter.h
// Created: Mon Nov 21 17:54:40 EST 2016
// Creator: pmatt (on Darwin Pauls-MacBook-Pro-2.local 13.4.0 i386)
//

#ifndef _DReaction_factory_ReactionFilter_
#define _DReaction_factory_ReactionFilter_

#include <iostream>
#include <iomanip>
#include <sstream>

#include "JANA/JFactory.h"
#include "ANALYSIS/DReaction.h"
#include "ANALYSIS/DHistogramActions.h"
#include "ANALYSIS/DCutActions.h"
#include "ANALYSIS/DSourceComboP4Handler.h"
#include "ANALYSIS/DSourceComboTimeHandler.h"

using namespace std;
using namespace jana;

class DReaction_factory_ReactionFilter : public jana::JFactory<DReaction>
{
	//return tuple: initial pid, target/2nd-beam pid, detected final pids, missing final pid (if any), missing particle index
	using DReactionStepTuple = tuple<Particle_t, Particle_t, vector<Particle_t>, Particle_t, int>;

	public:
		DReaction_factory_ReactionFilter()
		{
			// This is so that the created DReaction objects persist throughout the life of the program instead of being cleared each event. 
			SetFactoryFlag(PERSISTANT);
		}
		const char* Tag(void){return "ReactionFilter";}

	private:
		bool dDebugFlag = false;

		jerror_t evnt(JEventLoop* locEventLoop, uint64_t locEventNumber);

		//UTILITY FUNCTIONS
		map<size_t, tuple<string, string, string, vector<string>>> Parse_Input(void);
		bool Convert_StringToPID(string locString, Particle_t& locPID, bool& locIsMissingFlag);
		bool Parse_StepPIDString(string locStepString, DReactionStepTuple& locStepTuple);
		string Create_StepNameString(const DReactionStepTuple& locStepTuple, bool locFirstStepFlag);

		//CUSTOMIZATION FUNCTIONS
		void Set_Flags(DReaction* locReaction, string locRemainingFlagString);
		DReactionStep* Create_DefaultDecayStep(Particle_t locPID);

		//CREATION FUNCTIONS
		void Create_Steps(DReaction* locReaction, DReactionStep* locCurrentStep, vector<DReactionStepTuple>& locDecayStepTuples);
		vector<DReaction*> Create_Reactions(const map<size_t, tuple<string, string, string, vector<string>>>& locInputStrings);
		DReactionStep* Create_ReactionStep(const DReactionStepTuple& locStepTuple);

		//ACTIONS
		void Add_MassHistograms(DReaction* locReaction, bool locUseKinFitResultsFlag, string locBaseUniqueName = "");
		void Create_InvariantMassHistogram(DReaction* locReaction, Particle_t locPID, bool locUseKinFitResultsFlag, string locBaseUniqueName);
		void Create_MissingMassSquaredHistogram(DReaction* locReaction, Particle_t locPID, bool locUseKinFitResultsFlag, string locBaseUniqueName, int locMissingMassOffOfStepIndex, const deque<Particle_t>& locMissingMassOffOfPIDs);
		void Add_PostKinfitTimingCuts(DReaction* locReaction);

		DSourceComboP4Handler* dSourceComboP4Handler = nullptr;
		DSourceComboTimeHandler* dSourceComboTimeHandler = nullptr;
		deque<DReactionStep*> dReactionStepPool; //to prevent memory leaks
};

#endif // _DReaction_factory_ReactionFilter_

