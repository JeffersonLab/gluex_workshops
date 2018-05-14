//Copied from Pauls b1pi-example


#include "DEventProcessor_check_kinfitter.h"

// The executable should define the ROOTfile global variable. It will
// be automatically linked when dlopen is called.
extern TFile *ROOTfile;

// Routine used to create our DEventProcessor
extern "C"
{
	void InitPlugin(JApplication *app)
	{
		InitJANAPlugin(app);
		app->AddProcessor(new DEventProcessor_check_kinfitter());
		app->AddFactoryGenerator(new DFactoryGenerator_check_kinfitter());
	}
} // "C"

//------------------
// init
//------------------
jerror_t DEventProcessor_check_kinfitter::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t DEventProcessor_check_kinfitter::brun(JEventLoop *locEventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t DEventProcessor_check_kinfitter::evnt(JEventLoop *locEventLoop, uint64_t eventnumber)
{
	//Triggers the analysis (is also automatically called by DEventWriterROOT::Fill_Trees())
	vector<const DAnalysisResults*> locAnalysisResultsVector;
	locEventLoop->Get(locAnalysisResultsVector);

    const DEventWriterROOT_someTest* locEventWriterROOT = NULL;
    locEventLoop->GetSingle(locEventWriterROOT,"someTest");
    locEventWriterROOT->Fill_DataTrees(locEventLoop, "check_kinfitter");
    
	//Do Miscellaneous Cuts
    /*
	bool locSaveEventFlag = false;
	for(size_t loc_i = 0; loc_i < locAnalysisResultsVector.size(); ++loc_i)
	{
		const DAnalysisResults* locAnalysisResults = locAnalysisResultsVector[loc_i];
        
		if(locAnalysisResults->Get_Reaction()->Get_ReactionName() != "kpkm_reaction")
			continue;
		if(locAnalysisResults->Get_NumPassedParticleCombos() == 0)
			continue;
		locSaveEventFlag = true;
		break;
	}

	//Output REST File
	if(locSaveEventFlag)
	{
        
        
		vector<const DEventWriterREST*> locEventWriterRESTVector;
		locEventLoop->Get(locEventWriterRESTVector);
		locEventWriterRESTVector[0]->Write_RESTEvent(locEventLoop, "kpkm_reaction");
      //  locEventWriterRESTVector[0]->Write_RESTEvent(locEventLoop, "phikpkm_reaction");
       // locEventWriterRESTVector[0]->Write_RESTEvent(locEventLoop, "pippipim_reaction");
       // locEventWriterRESTVector[0]->Write_RESTEvent(locEventLoop, "rhopippipim_reaction");
	}
*/
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t DEventProcessor_check_kinfitter::erun(void)
{
	// Any final calculations on histograms (like dividing them)
	// should be done here. This may get called more than once.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t DEventProcessor_check_kinfitter::fini(void)
{
	return NOERROR;
}

