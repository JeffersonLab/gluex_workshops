
#ifndef _DEventProcessor_check_kinfitter_
#define _DEventProcessor_check_kinfitter_

#include "TFile.h"
#include "TROOT.h"

#include "JANA/JEventProcessor.h"

#include "DANA/DApplication.h"
#include "ANALYSIS/DAnalysisResults.h"
#include "ANALYSIS/DEventWriterROOT.h"
#include "DEventWriterROOT_someTest.h"
#include "HDDM/DEventWriterREST.h"

#include "DFactoryGenerator_check_kinfitter.h"

using namespace jana;

class DEventProcessor_check_kinfitter : public JEventProcessor
{
	public:
		DEventProcessor_check_kinfitter(){};
		~DEventProcessor_check_kinfitter(){};
		const char* className(void){return "DEventProcessor_check_kinfitter";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};


#endif // _DEventProcessor_b1pi_hists_

