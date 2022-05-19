#include <iostream> 
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"

R__LOAD_LIBRARY(libDSelector.so)
void runDSelector() 
{
	int proof_Nthreads = 26; // number of threads to process the DSelector with
	gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");

	TString nameOfTree = "pi0eta__B4_M17_M7_Tree"; 
	//TString nameOfTree = "pi0eta__B4_M7_M17_Tree"; 
	TChain *chain = new TChain(nameOfTree);

	///// Load all the trees you want to run over
	// - MC simulated flat in mass to determine the acceptance
	//chain->Add("/work/halld/lng/tSlopeSampler/2017_130M/tree_builder/merged/tree_pi0eta__B4_M17_M7_merged*");
	// - Data from the spring 2017 run period
	chain->Add("/cache/halld/RunPeriod-2017-01/analysis/ver52/tree_pi0eta__B4_M17_M7/merged/tree_pi0eta__B4_M17_M7_03*");
	//chain->Add("/work/halld2/home/lng/MC/malte_kmatrix/tree_pi0eta__B4_M7_M17_gen_amp*");
        
	string options = "";
	string outputHistFileName="output_hists.root"; // name of the output root file that contains the histograms
        string outputTreeFileName="output_tree.root"; // name of the output root file that contains the tree

	//// Choice 1: run with proof with your desired number of threads
	DPROOFLiteManager::Process_Chain(chain, "DSelector_etapi.C+",  proof_Nthreads, outputHistFileName, outputTreeFileName, options);
	//// Choice 2: run interactively - useful for debugging sometimes when running over small (sub)samples
	//chain->Process("DSelector_etapi.C+", options.data());

	return;
}
