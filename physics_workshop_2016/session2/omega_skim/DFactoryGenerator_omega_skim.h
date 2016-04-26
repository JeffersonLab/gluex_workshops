// $Id$
//
//    File: DFactoryGenerator_omega_skim.h
// Created: Tue Apr 26 11:29:50 EDT 2016
// Creator: pmatt (on Linux pmattdesktop.jlab.org 2.6.32-573.22.1.el6.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_omega_skim_
#define _DFactoryGenerator_omega_skim_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_omega_skim.h"

class DFactoryGenerator_omega_skim : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_omega_skim";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_omega_skim());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_omega_skim_

