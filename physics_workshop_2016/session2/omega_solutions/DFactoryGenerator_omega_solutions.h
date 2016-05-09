// $Id$
//
//    File: DFactoryGenerator_omega_solutions.h
// Created: Thu May  5 14:02:58 EDT 2016
// Creator: pmatt (on Linux pmattdesktop.jlab.org 2.6.32-573.22.1.el6.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_omega_solutions_
#define _DFactoryGenerator_omega_solutions_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_omega.h"

class DFactoryGenerator_omega_solutions : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_omega_solutions";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_omega());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_omega_solutions_

