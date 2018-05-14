// $Id$
//
//    File: DFactoryGenerator_p3pi_PiMinusMiss_P4V.h
// Created: Wed May  2 14:03:24 EDT 2018
// Creator: jzarling (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _DFactoryGenerator_p3pi_PiMinusMiss_P4V_
#define _DFactoryGenerator_p3pi_PiMinusMiss_P4V_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_p3pi_PiMinusMiss_P4V.h"

class DFactoryGenerator_p3pi_PiMinusMiss_P4V : public jana::JFactoryGenerator
{
	public:
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_p3pi_PiMinusMiss_P4V";}
		
		jerror_t GenerateFactories(jana::JEventLoop* locEventLoop)
		{
			locEventLoop->AddFactory(new DReaction_factory_p3pi_PiMinusMiss_P4V());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_p3pi_PiMinusMiss_P4V_

