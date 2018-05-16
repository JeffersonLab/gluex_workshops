// $Id$
//
//    File: DFactoryGenerator_ReactionFilter.h
// Created: Mon Nov 21 17:54:40 EST 2016
// Creator: pmatt (on Darwin Pauls-MacBook-Pro-2.local 13.4.0 i386)
//

#ifndef _DFactoryGenerator_ReactionFilter_
#define _DFactoryGenerator_ReactionFilter_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_ReactionFilter.h"
#include "DEventWriterROOT_factory_someTest.h"

class DFactoryGenerator_ReactionFilter : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_ReactionFilter";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_ReactionFilter());
            locEventLoop->AddFactory(new DEventWriterROOT_factory_someTest());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_ReactionFilter_

