// macro to process analysis TTree with TSelector which computes new variables from to be used in BDT training
#include <iostream> 

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
   
void preTrain_n3pi(TString myPath = "data/") 
{
   // select options for TSelector to write output fried TTree
   TString options = "FriendOutput";

   // process signal 
   TString fnameSignal = myPath+"tree_n3pi_signal_train.root";
   TFile *inputSignal = new TFile(fnameSignal);  
   TTree *signal     = (TTree*)inputSignal->Get("n3pi_Tree"); 
   signal->Process("selector_tree_n3pi.C+",options);
   gSystem->Exec("mv tree_n3piFriend.root tree_n3piFriend_signal_train.root");
   
   // process background
   TString fnameBackground = myPath+"tree_n3pi_bggen_train.root";
   TFile *inputBackground = new TFile(fnameBackground);  
   TTree *background = (TTree*)inputBackground->Get("n3pi_Tree");
   background->Process("selector_tree_n3pi.C+",options);
   gSystem->Exec("mv tree_n3piFriend.root tree_n3piFriend_bggen_train.root");

   return;
}
