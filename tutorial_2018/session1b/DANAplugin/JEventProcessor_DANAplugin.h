// $Id$
//
//    File: JEventProcessor_DANAplugin.h
// Created: Tue Jun 19 13:47:06 EDT 2018
// Creator: aaustreg (on Linux ifarm1401.jlab.org 3.10.0-327.el7.x86_64 x86_64)
//

#ifndef _JEventProcessor_DANAplugin_
#define _JEventProcessor_DANAplugin_

#include <JANA/JEventProcessor.h>

class JEventProcessor_DANAplugin:public jana::JEventProcessor{
	public:
		JEventProcessor_DANAplugin();
		~JEventProcessor_DANAplugin();
		const char* className(void){return "JEventProcessor_DANAplugin";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

#endif // _JEventProcessor_DANAplugin_

