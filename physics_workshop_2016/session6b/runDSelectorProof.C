// macro to process analysis TTree with DSelector
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runDSelectorProof(TString sample = "sim1_10000_0", TString path = "/work/halld/home/jrsteven/farmOutput/2016-workshop/bggen/tree/") 
{
	// Load DSelector library
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
	int locProof_Nthreads = 4;
	
	TString fileName = path;
        fileName += Form("tree_p2gamma_%s.root", sample.Data());
	TChain *chain = new TChain("p2gamma_workshop_Tree");
	chain->Add(fileName);
	
	cout<<"total entries in TChain = "<<chain->GetEntries()<<endl;
	DPROOFLiteManager::Process_Chain(chain, "DSelector_p2gamma_workshop.C+", Form("hist_p2gamma_%s.acc.root", sample.Data()), "", sample.Data(), locProof_Nthreads);
	
	return;
}
