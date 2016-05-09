// $Id$
//
//    File: DFactoryGenerator_p2gamma_workshop.h
// Created: Fri Apr 29 13:18:03 EDT 2016
// Creator: jrsteven (on Linux halldw1.jlab.org 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_p2gamma_workshop_
#define _DFactoryGenerator_p2gamma_workshop_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_p2gamma_workshop.h"

class DFactoryGenerator_p2gamma_workshop : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_p2gamma_workshop";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_p2gamma_workshop());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_p2gamma_workshop_

