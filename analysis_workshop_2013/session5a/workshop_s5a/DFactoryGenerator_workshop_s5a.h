// $Id$
//
//    File: DFactoryGenerator_workshop_s5a.h
// Created: Tue Jul  9 00:09:03 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DFactoryGenerator_workshop_s5a_
#define _DFactoryGenerator_workshop_s5a_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_workshop_s5a.h"

class DFactoryGenerator_workshop_s5a : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_workshop_s5a";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_workshop_s5a());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_workshop_s5a_

