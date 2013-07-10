// Simplified version of $ROOTSYS/tmva/test/TMVAClassificationApplication.C to illustrate TMVA evaluation procedure
void TMVAClassificationApplication( TString myMethodList = "" ) 
{   
   TMVA::Tools::Instance();

   // --- Create the Reader object
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   Float_t var1, var2;
   Float_t var3, var4;
   reader->AddVariable( "myvar1 := var1+var2", &var1 );
   reader->AddVariable( "myvar2 := var1-var2", &var2 );
   reader->AddVariable( "var3",                &var3 );
   reader->AddVariable( "var4",                &var4 );

   // Book the MVA methods
   reader->BookMVA( "BDT method", "weights/TMVAClassification_BDT.weights.xml" ); 
   
   // Book output histograms
   TH1F* histBdt = new TH1F( "MVA_BDT", "MVA_BDT", 100, -0.8, 0.8 );
   
   // Inptut file
   TFile *input = new TFile("./tmva_class_example.root");

   // Initialize TTree
   TTree* theTree = (TTree*)input->Get("TreeS");
   Float_t userVar1, userVar2;
   theTree->SetBranchAddress( "var1", &userVar1 );
   theTree->SetBranchAddress( "var2", &userVar2 );
   theTree->SetBranchAddress( "var3", &var3 );
   theTree->SetBranchAddress( "var4", &var4 );

   // Event loop
   for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
      if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      theTree->GetEntry(ievt);
      var1 = userVar1 + userVar2;
      var2 = userVar1 - userVar2;

      histBdt->Fill( reader->EvaluateMVA( "BDT method" ) );
   }

   // Write histograms
   TFile *target  = new TFile( "TMVApp.root","RECREATE" );
   histBdt->Write();
   
   target->Close();
   delete reader;
} 
