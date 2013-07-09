// $Id$
//
//    File: DFactoryGenerator_workshop_s4b.h
// Created: Mon Jul  8 23:41:58 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DFactoryGenerator_workshop_s4b_
#define _DFactoryGenerator_workshop_s4b_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_workshop_s4b.h"

class DFactoryGenerator_workshop_s4b : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_workshop_s4b";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_workshop_s4b());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_workshop_s4b_

