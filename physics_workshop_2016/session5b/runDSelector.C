// macro to process analysis TTree with DSelector
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void runDSelector(string sample = "sim1", bool proof = 1, string path = "$DATA/session5b/") 
{
	// Load DSelector library
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");
	int proof_Nthreads = 1;

	// open ROOT file and TTree
	TString fileName = path;
	fileName += Form("tree_p2gamma_%s.root", sample.data());

	string options = sample;
	if(proof) { // add TTree to chain and use PROOFLiteManager
		TChain *chain = new TChain("p2gamma_workshop_Tree");
		chain->Add(fileName);
		string outputHistFileName = Form("hist_p2gamma_%s.acc.root", sample.data());
		string outputTreeFileName = ""; // don't need output tree
		DPROOFLiteManager::Process_Chain(chain, "DSelector_p2gamma_workshop.C+", outputHistFileName, outputTreeFileName, options, proof_Nthreads);
	}
	else { // get TTree and use standard TTree::Process
		TFile *f = TFile::Open(fileName);
		TTree *tree = (TTree*)f->Get("p2gamma_workshop_Tree");
		tree->Process("DSelector_p2gamma_workshop.C+", options.data());
	}
		
	return;
}
