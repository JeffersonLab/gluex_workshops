// $Id$
//
//    File: DFactoryGenerator_workshop_s3a.h
// Created: Mon Jul  8 21:53:19 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DFactoryGenerator_workshop_s3a_
#define _DFactoryGenerator_workshop_s3a_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_workshop_s3a.h"

class DFactoryGenerator_workshop_s3a : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_workshop_s3a";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_workshop_s3a());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_workshop_s3a_

