// $Id$
//
//    File: DFactoryGenerator_ReactionPlugin.h
// Created: Tue Jun 19 13:47:31 EDT 2018
// Creator: aaustreg (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_ReactionPlugin_
#define _DFactoryGenerator_ReactionPlugin_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_ReactionPlugin.h"

class DFactoryGenerator_ReactionPlugin : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_ReactionPlugin";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_ReactionPlugin());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_ReactionPlugin_

