// macro to process analysis TTree with TSelector which cuts on BDT response through TMVA::Reader class
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void postTrain_n3pi(TString myBdtCut = "0.1", TString myPath = "data/") 
{

  // select options for TSelector to evaluate the BDT response for each combo
  // combos which pass a (user defined) cut on the BDT classifier are written to a TTree to be used in AmpTools fit
  TString options = "EvalTMVA:BdtCut=";
  options+=myBdtCut;
    
  // process signal
  TString fnameSignal = myPath+"tree_n3pi_signal_train.root";
  TFile *inputSignal = new TFile(fnameSignal);  
  TTree *signal     = (TTree*)inputSignal->Get("n3pi_Tree"); 
  signal->Process("selector_tree_n3pi.C+",options);
  gSystem->Exec(Form("mv tree_n3piBdtCut.root tree_n3piBdtCut%s_signal_train.root",myBdtCut.Data()));
  
  // process background
  options+=":bggenData"; //excludes trueSignal evnes
  TString fnameBackground = myPath+"tree_n3pi_bggen_train.root";
  TFile *inputBackground = new TFile(fnameBackground);  
  TTree *background = (TTree*)inputBackground->Get("n3pi_Tree");
  background->Process("selector_tree_n3pi.C+",options);
  gSystem->Exec(Form("mv tree_n3piBdtCut.root tree_n3piBdtCut%s_bggen_train.root",myBdtCut.Data()));

  // similar processors can be added for the evaluation samples
  // tree_n3pi_bggen_eval.root
  // tree_n3pi_signal_eval.root
  // tree_n3pi_signal_acc_eval.root

  return;
}
