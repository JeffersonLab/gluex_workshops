// macro to process analysis TTree with DSelector
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runDSelector(TString sample = "workshop", TString path = "gen_pi0/") 
{
	// Load DSelector library
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
       	
	// open ROOT file and TTree
	TString fileName = path;
	fileName += Form("tree_p2gamma_%s.root", sample.Data());
	TFile *f = TFile::Open(fileName);
	TTree *tree = (TTree*)f->Get("p2gamma_workshop_Tree");

	TString options = "";
	tree->Process("DSelector_p2gamma_workshop.C+", options);

	return;
}
