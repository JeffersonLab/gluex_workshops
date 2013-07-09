// $Id$
//
//    File: DEventProcessor_workshop_s2a.cc
// Created: Mon Jul  8 21:19:49 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#include "DEventProcessor_workshop_s2a.h"

#include <PID/DChargedTrack.h>
#include <PID/DChargedTrackHypothesis.h>
#include <PID/DNeutralParticle.h>
#include <PID/DNeutralParticleHypothesis.h>
#include <PID/DNeutralShower.h>
#include <TRACKING/DMCThrown.h>
#include <PID/DBeamPhoton.h>

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_workshop_s2a()); //register this plugin
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_workshop_s2a::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... create historgrams or trees ...
	// japp->RootUnLock();
	//

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_workshop_s2a::brun(jana::JEventLoop* locEventLoop, int locRunNumber)
{
	// This is called whenever the run number changes

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_workshop_s2a::evnt(jana::JEventLoop* locEventLoop, int locEventNumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// locEventLoop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	//
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// locEventLoop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_workshop_s2a::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_workshop_s2a::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

