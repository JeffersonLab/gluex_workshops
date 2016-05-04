// macro to process analysis TTree with DSelector
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runDSelector(TString sample = "sim1", TString path = "/work/halld2/home/pmatt/workshop/session3/") 
{
	// Load DSelector library
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
	int locProof_Nthreads = 4;
	
	TString fileName = path;
        fileName += Form("tree_omega_%s_10000_0.root", sample.Data());
	TChain *chain = new TChain("omega_skim_Tree");
	chain->Add(fileName);
	
	cout<<"total entries in TChain = "<<chain->GetEntries()<<endl;
	DPROOFLiteManager::Process_Chain(chain, "DSelector_p3pi_workshop.C+", Form("hist_p3pi_%s.acc.root", sample.Data()), Form("tree_p3pi_%s.acc.root", sample.Data()), locProof_Nthreads);
	
	return;
}
