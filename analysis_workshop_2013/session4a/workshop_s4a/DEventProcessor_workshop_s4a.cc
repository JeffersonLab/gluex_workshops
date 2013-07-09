// $Id$
//
//    File: DEventProcessor_workshop_s4a.cc
// Created: Mon Jul  8 23:19:34 EDT 2013
// Creator: pmatt (on Darwin pmattLaptop 10.8.0 i386)
//

#include "DEventProcessor_workshop_s4a.h"

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_workshop_s4a()); //register this plugin
		locApplication->AddFactoryGenerator(new DFactoryGenerator_workshop_s4a()); //register the factory generator
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_workshop_s4a::init(void)
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
jerror_t DEventProcessor_workshop_s4a::brun(jana::JEventLoop* locEventLoop, int locRunNumber)
{
	// This is called whenever the run number changes

	/*
	//Recommeded: Initialize reaction-independent analysis actions (nothing should be done if already initialized)
	dHistogramAction_TrackMultiplicity.Initialize(locEventLoop);
	dHistogramAction_ThrownParticleKinematics.Initialize(locEventLoop);
	dHistogramAction_DetectedParticleKinematics.Initialize(locEventLoop);
	dHistogramAction_GenReconTrackComparison.Initialize(locEventLoop);
	*/

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_workshop_s4a::evnt(jana::JEventLoop* locEventLoop, int locEventNumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	//
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();

	/*
	//Recommended: Execute reaction-independent actions (fill histograms)
	dHistogramAction_TrackMultiplicity(locEventLoop);
	dHistogramAction_ThrownParticleKinematics(locEventLoop);
	dHistogramAction_DetectedParticleKinematics(locEventLoop);
	dHistogramAction_GenReconTrackComparison(locEventLoop);
	*/

	//Get the analysis results for all DReactions. 
		//Getting these objects triggers the analysis
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_workshop_s4a::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_workshop_s4a::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

