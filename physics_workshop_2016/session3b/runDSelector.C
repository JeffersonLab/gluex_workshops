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
       	
	// open ROOT file and TTree
	TString fileName = path;
	fileName += Form("tree_omega_%s_10000_0.root", sample.Data());
	TFile *f = TFile::Open(fileName);
	TTree *tree = (TTree*)f->Get("omega_skim_Tree");

	TString options = "";
	tree->Process("DSelector_p3pi_workshop.C+", options);

	return;
}
