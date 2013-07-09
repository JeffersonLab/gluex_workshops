// $Id$
//
//    File: DFactoryGenerator_workshop_s4a.h
// Created: Mon Jul  8 23:19:34 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DFactoryGenerator_workshop_s4a_
#define _DFactoryGenerator_workshop_s4a_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_workshop_s4a.h"

class DFactoryGenerator_workshop_s4a : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_workshop_s4a";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_workshop_s4a());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_workshop_s4a_

