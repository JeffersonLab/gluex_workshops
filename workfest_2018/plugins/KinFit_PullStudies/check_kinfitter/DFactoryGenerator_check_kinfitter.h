#ifndef _DFactoryGenerator_check_kinfitter_
#define _DFactoryGenerator_check_kinfitter_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "DReaction_factory_check_kinfitter.h"
#include "DEventWriterROOT_factory_someTest.h"

class DFactoryGenerator_check_kinfitter: public jana::JFactoryGenerator
{
	public:
		DFactoryGenerator_check_kinfitter(){}
		virtual ~DFactoryGenerator_check_kinfitter(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "DFactoryGenerator_check_kinfitter";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop)
		{
			loop->AddFactory(new DReaction_factory_check_kinfitter());
            loop->AddFactory(new DEventWriterROOT_factory_someTest());
			return NOERROR;
		}
};

#endif // _DFactoryGenerator_b1pi_hists_

