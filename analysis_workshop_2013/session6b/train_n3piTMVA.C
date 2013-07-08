#include <cstdlib>
#include <iostream> 

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif
   
void train_n3piTMVA(TString myPath = "data/") 
{

   // this loads the library
   TMVA::Tools::Instance();

   // Create a new root output file
   TString baseName = "n3pi";
   TString outputFileName = baseName+"_trainTMVA.root";
   TFile* outputFile = TFile::Open(outputFileName, "RECREATE");
   TMVA::Factory *factory = new TMVA::Factory(baseName, outputFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I");

   // 3 pi PID variables 
   factory->AddVariable("(PiPlus1__ChiSq_Timing_KinFit==PiPlus1__ChiSq_Timing_KinFit) ? TMath::Prob(PiPlus1__ChiSq_Timing_KinFit,PiPlus1__NDF_Timing) : 0.","Timing FOM: #pi+ 1","",'F');
   factory->AddVariable("TMath::Prob(PiPlus1__ChiSq_DCdEdx,PiPlus1__NDF_DCdEdx)","dE/dx FOM: #pi+ 1","",'F');
   factory->AddVariable("PiPlus1__NDF_Tracking","Track Fit NDF: #pi+ 1","",'I');
   factory->AddVariable("(PiPlus2__ChiSq_Timing_KinFit==PiPlus2__ChiSq_Timing_KinFit) ? TMath::Prob(PiPlus2__ChiSq_Timing_KinFit,PiPlus2__NDF_Timing) : 0.","Timing FOM: #pi+ 1","",'F');
   factory->AddVariable("TMath::Prob(PiPlus2__ChiSq_DCdEdx,PiPlus2__NDF_DCdEdx)","dE/dx FOM: #pi+ 2","",'F');
   factory->AddVariable("PiPlus2__NDF_Tracking","Track Fit NDF: #pi+ 2","",'I');
   factory->AddVariable("(PiMinus__ChiSq_Timing_KinFit==PiMinus__ChiSq_Timing_KinFit) ? TMath::Prob(PiMinus__ChiSq_Timing_KinFit,PiMinus__NDF_Timing) : 0.","Timing FOM: #pi+ 1","",'F');
   factory->AddVariable("TMath::Prob(PiMinus__ChiSq_DCdEdx,PiMinus__NDF_DCdEdx)","dE/dx FOM: #pi-","",'F');
   factory->AddVariable("PiMinus__NDF_Tracking","Track Fit NDF: #pi-","",'I');

   // Kinematic fit variables
   factory->AddVariable("TMath::Prob(ChiSq_KinFit,NDF_KinFit)","Kinematic Fit Confidence Level","",'F');
   factory->AddVariable("MissingNeutron__X4.Perp()","Primary Vertex Radius","cm",'F');
   factory->AddVariable("MissingNeutron__P4.Perp()","Missing Neutron Transverse Momentum","GeV/c",'F');
   
   // Computed variables from friend tree
   factory->AddVariable("Unused__Energy_FCAL_Total","Isolated FCAL Sum Energy","GeV",'F');
   factory->AddVariable("Unused__Energy_BCAL_Total","Isolated BCAL Sum Energy","GeV",'F');
   factory->AddVariable("Unused__Max_Proton_FOM","Unused Proton hypothesis track with largest FOM","",'F');
   factory->AddVariable("Unused__Max_KPlus_FOM","Unused K+ hypothesis track with largest FOM","",'F');
   factory->AddVariable("Unused__Max_KMinus_FOM","Unused K- hypothesis track with largest FOM","",'F');

   // load the signal and background event samples from ROOT trees
   TString fnameSignal = "tree_n3pi_signal_train.root";
   TString fnameBackground = "tree_n3pi_bggen_train.root";
   TFile *inputSignal = new TFile(myPath+fnameSignal);  
   TFile *inputBackground = new TFile(myPath+fnameBackground);  
   
   TTree *signal     = (TTree*)inputSignal->Get("n3pi_Tree"); 
   TTree *background_combinatoric = (TTree*)inputSignal->Get("n3pi_Tree");
   TTree *background = (TTree*)inputBackground->Get("n3pi_Tree");
   
   // friend tree with trueSignal and other computed variables
   signal->AddFriend("n3piFriend_Tree",fnameSignal.ReplaceAll("n3pi","n3piFriend"));
   background_combinatoric->AddFriend("n3piFriend_Tree",fnameSignal);
   background->AddFriend("n3piFriend_Tree",fnameBackground.ReplaceAll("n3pi","n3piFriend"));
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0; 
   Double_t backgroundWeight = 1.0;
   
   factory->AddSignalTree    ( signal,     signalWeight     );
   factory->AddBackgroundTree( background_combinatoric, signalWeight );
   factory->AddBackgroundTree( background, backgroundWeight );

   TString bothCut = " && PiPlus1__NDF_Tracking>5 && PiPlus2__NDF_Tracking>5 && PiMinus__NDF_Tracking>5";
   TString signalCut = "trueSignal"; 
   TString backgroundCut = "!trueSignal"; 
   signalCut+=bothCut; backgroundCut+=bothCut;

   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = signalCut;
   TCut mycutb = backgroundCut;

   // tell the factory to use all remaining events in the trees after training for testing:
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // book MVA method
   factory->BookMethod( TMVA::Types::kBDT, "BDT","!H:!V:NTrees=100:nEventsMin=100:MaxDepth=5:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=200:PruneMethod=NoPruning" );

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();
   delete factory;

}
