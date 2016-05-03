// $Id$
//
//    File: DEventProcessor_omega_ref.cc
// Created: Mon May  2 12:05:11 EDT 2016
// Creator: pmatt (on Linux pmattdesktop.jlab.org 2.6.32-573.22.1.el6.x86_64 x86_64)
//

#include "DEventProcessor_omega_ref.h"

// Routine used to create our DEventProcessor

extern "C"
{
	void InitPlugin(JApplication *locApplication)
	{
		InitJANAPlugin(locApplication);
		locApplication->AddProcessor(new DEventProcessor_omega_ref()); //register this plugin
		locApplication->AddFactoryGenerator(new DFactoryGenerator_omega_ref()); //register the factory generator
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_omega_ref::init(void)
{
	// This is called once at program startup.

	/*
	//OPTIONAL: Create an EventStore skim.  
	string locSkimFileName = "omega.idxa";
	dEventStoreSkimStream.open(locSkimFileName.c_str());
	dEventStoreSkimStream << "IDXA" << endl;
	*/

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_omega_ref::brun(jana::JEventLoop* locEventLoop, int32_t locRunNumber)
{
	// This is called whenever the run number changes

	//Recommended: Create output ROOT TTrees (nothing is done if already created)
	const DEventWriterROOT* locEventWriterROOT = NULL;
	locEventLoop->GetSingle(locEventWriterROOT);
	locEventWriterROOT->Create_DataTrees(locEventLoop);

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_omega_ref::evnt(jana::JEventLoop* locEventLoop, uint64_t locEventNumber)
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
	// japp->RootFillLock(this);
	//  ... fill historgrams or trees ...
	// japp->RootFillUnLock(this);

	// DOCUMENTATION:
	// ANALYSIS library: https://halldweb1.jlab.org/wiki/index.php/GlueX_Analysis_Software

	/*********************************************************** REQUIRED ***********************************************************/

	//REQUIRED: To run an analysis, You MUST call one at least of the below code fragments. 
		//JANA is on-demand, so if you don't call one of these, then your analysis won't run. 

	//Recommended: Write surviving particle combinations (if any) to output ROOT TTree
		//If no cuts are performed by the analysis actions added to a DReaction, then this saves all of its particle combinations. 
		//The event writer gets the DAnalysisResults objects from JANA, performing the analysis. 
	// string is DReaction factory tag: will fill trees for all DReactions that are defined in the specified factory
	const DEventWriterROOT* locEventWriterROOT = NULL;
	locEventLoop->GetSingle(locEventWriterROOT);
	locEventWriterROOT->Fill_DataTrees(locEventLoop, "omega");

	/*
	//Optional: Get the analysis results for all DReactions. 
		//Getting these objects triggers the analysis, if it wasn't performed already. 
		//These objects contain the DParticleCombo objects that survived the DAnalysisAction cuts that were added to the DReactions
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);
	*/

	/************************************************** OPTIONAL: FURTHER ANALYSIS **************************************************/
	/*
	//Optional: Get all particle combinations for all DReactions. 
		//If kinematic fits were requested, these contain both the measured and kinematic-fit track parameters
		//No cuts from DAnalysisActions are placed on these combos
	vector<const DParticleCombo*> locParticleCombos;
	locEventLoop->Get(locParticleCombos);
	for(size_t loc_i = 0; loc_i < locParticleCombos.size(); ++loc_i)
	{
		const DParticleCombo* locParticleCombo = locParticleCombos[loc_i];
		if(locParticleCombo->Get_Reaction()->Get_ReactionName() != "omega")
			continue; // particle combination was for a different reaction
		//perform further analysis steps here...
	}
	*/

	/*
	//Optional: Perform further cuts on the particle combos in the analysis results. 
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "omega")
			continue; // analysis results were for a different reaction
		//get the DParticleCombo objects for this DReaction that survived all of the DAnalysisAction cuts
		deque<const DParticleCombo*> locPassedParticleCombos;
		locAnalysisResults->Get_PassedParticleCombos(locPassedParticleCombos);
		for(size_t loc_j = 0; loc_j < locPassedParticleCombos.size(); ++loc_j)
		{
			const DParticleCombo* locPassedParticleCombo = locPassedParticleCombos[loc_j];
			//perform further analysis steps here...
		}
	}
	*/

	/******************************************************** OPTIONAL: SKIMS *******************************************************/

	/*
	//Optional: Save event to output REST file. Use this to create physical skims.
	const DEventWriterREST* locEventWriterREST = NULL;
	locEventLoop->GetSingle(locEventWriterREST);
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "omega")
			continue; // analysis results were for a different reaction

		//get the DParticleCombo objects for this DReaction that survived all of the DAnalysisAction cuts
		deque<const DParticleCombo*> locPassedParticleCombos;
		locAnalysisResults->Get_PassedParticleCombos(locPassedParticleCombos);

		if(!locPassedParticleCombos.empty())
			locEventWriterREST->Write_RESTEvent(locEventLoop, "omega"); //string is part of output file name
	}
	*/

	/*
	//Optional: Create an EventStore skim. 

	// See whether this is MC data or real data
	vector<const DMCThrown*> locMCThrowns;
	locEventLoop->Get(locMCThrowns);

	unsigned int locRunNumber = locEventLoop->GetJEvent().GetRunNumber();
	unsigned int locUniqueID = locMCThrowns.empty() ? 1 : Get_FileNumber(locEventLoop);

	// If a particle combo passed the cuts, save the event info in the output file
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "omega")
			continue; // analysis results were for a different reaction
		if(locAnalysisResults->Get_NumPassedParticleCombos() == 0)
			continue; // no combos passed

		//MUST LOCK AROUND MODIFICATION OF MEMBER VARIABLES IN brun() or evnt().
		japp->WriteLock("omega.idxa"); //Lock is unique to this output file
		{
			dEventStoreSkimStream << locRunNumber << " " << locEventNumber << " " << locUniqueID << endl;
		}
		japp->Unlock("omega.idxa");
	}
	*/

	return NOERROR;
}

int DEventProcessor_omega_ref::Get_FileNumber(JEventLoop* locEventLoop) const
{
	//Assume that the file name is in the format: *_X.ext, where:
		//X is the file number (a string of numbers of any length)
		//ext is the file extension (probably .evio or .hddm)

	//get the event source
	JEventSource* locEventSource = locEventLoop->GetJEvent().GetJEventSource();
	if(locEventSource == NULL)
		return -1;

	//get the source file name (strip the path)
	string locSourceFileName = locEventSource->GetSourceName();

	//find the last "_" & "." indices
	size_t locUnderscoreIndex = locSourceFileName.rfind("_");
	size_t locDotIndex = locSourceFileName.rfind(".");
	if((locUnderscoreIndex == string::npos) || (locDotIndex == string::npos))
		return -1;

	size_t locNumberLength = locDotIndex - locUnderscoreIndex - 1;
	string locFileNumberString = locSourceFileName.substr(locUnderscoreIndex + 1, locNumberLength);

	int locFileNumber = -1;
	istringstream locFileNumberStream(locFileNumberString);
	locFileNumberStream >> locFileNumber;

	return locFileNumber;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_omega_ref::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_omega_ref::fini(void)
{
	// Called before program exit after event processing is finished.
	if(dEventStoreSkimStream.is_open())
		dEventStoreSkimStream.close();
	return NOERROR;
}

