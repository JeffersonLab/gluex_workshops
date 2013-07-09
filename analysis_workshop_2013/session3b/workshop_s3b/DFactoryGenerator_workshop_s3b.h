// $Id$
//
//    File: DFactoryGenerator_workshop_s3b.h
// Created: Mon Jul  8 22:52:23 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#ifndef _DFactoryGenerator_workshop_s3b_
#define _DFactoryGenerator_workshop_s3b_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_workshop_s3b.h"

class DFactoryGenerator_workshop_s3b : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_workshop_s3b";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_workshop_s3b());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_workshop_s3b_

