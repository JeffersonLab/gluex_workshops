// Simplified version of $ROOTSYS/tmva/test/TMVAClassification.C to illustrate TMVA training procedure
void TMVAClassification()
{
   TMVA::Tools::Instance();
   TFile* outputFile = TFile::Open( "TMVA.root" , "RECREATE" );

   // Initialize factory and add variables to be used by MVA
   TMVA::Factory *factory = new TMVA::Factory("TMVAClassification",outputFile,"!V:!Silent:Color");
   factory->AddVariable( "myvar1 := var1+var2", "Expression 1", "", 'F' );
   factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
   factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
   factory->AddVariable( "var4",                "Variable 4", "units", 'F' );
   
   // Get ROOT file if doesn't exist
   TString fname = "./tmva_class_example.root";
   if (gSystem->AccessPathName( fname ))  // file does not exist in local directory
     gSystem->Exec("wget http://root.cern.ch/files/tmva_class_example.root");

   // Load TTrees for signal and background and add to factory for processing
   TFile *input = TFile::Open( fname );
   TTree *signal     = (TTree*)input->Get("TreeS");
   TTree *background = (TTree*)input->Get("TreeB");
   factory->AddSignalTree    ( signal,     1.0 );
   factory->AddBackgroundTree( background, 1.0 );
   
   // Choose how to split data on training and testing 
   factory->PrepareTrainingAndTestTree("","","nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:!V" );

   // Book MVA method and select options for that method (can book many at once)
   factory->BookMethod( TMVA::Types::kBDT, "BDT", "!H:!V:NTrees=850:nEventsMin=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   // Train MVA methods and create weight files using subset of data for training
   factory->TrainAllMethods();
   // Independent subset of data reserved for testing is evaluated with the weight files created above
   factory->TestAllMethods();
   // Evaluate performace of MVA method and compare performance if multiple methods are trained/tested
   factory->EvaluateAllMethods();

   outputFile->Close();
   delete factory;
}
