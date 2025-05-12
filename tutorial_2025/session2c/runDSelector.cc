#include <iostream> 
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"

R__LOAD_LIBRARY(libDSelector.so)
void runDSelector(){
  //Make a directory for the log output of the DSelector in local direcotory
  TString currentDir = gSystem->pwd();  // Gets current working directory
  TString sandboxPath = currentDir + "/tmp";
  gEnv->SetValue("ProofLite.Sandbox", sandboxPath);
  int proof_Nthreads = 6; // number of threads to process the DSelector with
  gROOT->ProcessLine(".x $(ROOT_ANALYSIS_HOME)/scripts/Load_DSelector.C");

  // Name of the Data and MC accepted tree
  TString nameOfTree = "pi0pi0pippim__B4_Tree";
  // Name of the MC thrown tree
  // TString nameOfTree = "Thrown_Tree"; 
  TChain *chain = new TChain(nameOfTree);

  ///// Load all the trees you want to run over /////
  // If the trees are not in '/cache' use the following command to load
  // them from tape ('/mss'):
  // jache -get "path_to_the_tree/files*" -e email
  // Set of trees for Data (9 files)
  chain->Add( "/cache/halld/RunPeriod-2018-08/analysis/ver16/tree_pi0pi0pippim__B4/merged/tree_pi0pi0pippim__B4_05163*");
  // Set of trees for MC accpeted
  // chain->Add("/cache/halld/home/jrsteven/REQUESTED_MC/omegapi_phasespace_2018_08_ver03/tree_pi0pi0pippim__B4/merged/tree_pi0pi0pippim__B4_0516*");
  // Set of trees for MC thrown
  //chain->Add("/cache/halld/home/jrsteven/REQUESTED_MC/omegapi_phasespace_2018_08_ver03/tree_thrown/merged/tree_thrown_0516*");

 
  string options = "";
  string outputHistFileName="OmegaPi_hists.root"; // name of the root file with the histograms
  string outputTreeFileName="OmegaPi_tree.root"; // name of the root file with the tree

  //// Choice 1: run with proof with your desired number of threads /////
  DPROOFLiteManager::Process_Chain(chain, "DSelector_OmegaPi0.C+",  proof_Nthreads, outputHistFileName, outputTreeFileName, options);
  //// Choice 2: run interactively - useful for debugging sometimes when running over small (sub)samples /////
  //chain->Process("DSelector_etapi.C+", options.data());

  // Note: The '+' in "DSelector_OmegaPi0.C+" compiles if needed. To force compilation ues '++'
  return;
}
