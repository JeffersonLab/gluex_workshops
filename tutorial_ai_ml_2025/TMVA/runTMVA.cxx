#include <TFile.h>
#include <TTree.h>
#include <TMVA/Factory.h>
#include <TMVA/DataLoader.h>
#include <TMVA/Tools.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    // Initialize TMVA
    TMVA::Tools::Instance();
    
    // Create a ROOT output file where TMVA will store results
    //TFile* outputFile = TFile::Open("TMVA_BDT-v-07092025.root", "RECREATE");
    TFile* outputFile = TFile::Open("TMVA_MLP-v-07092025.root", "RECREATE");

    // Create a TMVA factory
    TMVA::Factory factory("TMVAClassification", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D");

    // Create a DataLoader object
    TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset-v-07092025");

    // Load the data from a ROOT file
    TFile* inputFile_SIG = TFile::Open("../FromAndrewToMe/p2k/flat_kpkm.root");
    TFile* inputFile_BKG[6];
    
    inputFile_BKG[0] = TFile::Open("../FromAndrewToMe/p2k/flat_pippim.root");
    //inputFile_BKG[1] = TFile::Open("hists-gluex-2018-08-bkg-rdm-gen-EtaRegge-eta-to-pi0pi0pi0.root");
    TTree* signalTree_SIG = (TTree*) inputFile_SIG->Get("flat_kpkm");
    TTree* backgroundTree[6];
    int nbkg = 1;
    for (int i = 0; i < nbkg; i ++)
      backgroundTree[i] = (TTree*) inputFile_BKG[i]->Get("flat_pippim");

    vector <TString> s_features1;
    vector <TString> s_features2;
    
    s_features1 = {"P4_meas_E", "P4_meas_X", "P4_meas_Y", "P4_meas_Z"};
    s_features2 = {
      "dEdx_FDC",
      //"dEdx_CDC",
      "TrackFCAL_DOCA",
      //"TrackBCAL_DeltaPhi", "TrackBCAL_DeltaZ",
      //"Energy_BCAL",
      "Energy_FCAL",
      "E1E9_FCAL", "E9E25_FCAL",
      "SumU_FCAL", "SumV_FCAL"
      //"SigLong_BCAL", "SigTheta_BCAL", "SigTrans_BCAL"
    };
    /*
    // Add variables (features) to the DataLoader
    for (int i = 0; i < (int) s_features1.size(); i ++) {
      dataloader->AddVariable("Positive" + s_features1[i], 'F');
      dataloader->AddVariable("Negative" + s_features1[i], 'F');
      dataloader->AddVariable("Recoil" + s_features1[i], 'F');
    }
    */
    for (int i = 0; i < (int) s_features2.size(); i ++) {
      dataloader->AddVariable("Positive_" + s_features2[i], 'F');
      dataloader->AddVariable("Negative_" + s_features2[i], 'F');
      //dataloader->AddVariable("Recoil_" + s_features2[i], 'F');
    }
    //dataloader->AddVariable("BeamX4_meas_T", 'F');
    //dataloader->AddVariable("BeamP4_meas_E", 'F');
    //dataloader->AddVariable("BeamX4_meas_Z", 'F');
    //dataloader->AddVariable("sqrt(pow(PositiveP4_kin_X, 2.) + pow(PositiveP4_kin_Y, 2.) + pow(PositiveP4_kin_Z, 2.))", 'F');
    //dataloader->AddVariable("sqrt(pow(NegativeP4_kin_X, 2.) + pow(NegativeP4_kin_Y, 2.) + pow(NegativeP4_kin_Z, 2.))", 'F');
    
    // Add the signal and background trees to the DataLoader
    dataloader->AddSignalTree(signalTree_SIG, 1.0);
    dataloader->AddBackgroundTree(backgroundTree[0], 1.0);
    //dataloader->AddBackgroundTree(backgroundTree[1], 0.14);
    
    // Define cut
    TCut mycut_beam = "8.2 < BeamP4_meas_E && BeamP4_meas_E < 8.8";
    TCut mycut_pp = "2 < sqrt(pow(PositiveP4_kin_X, 2.) + pow(PositiveP4_kin_Y, 2.) + pow(PositiveP4_kin_Z, 2.)) && sqrt(pow(PositiveP4_kin_X, 2.) + pow(PositiveP4_kin_Y, 2.) + pow(PositiveP4_kin_Z, 2.)) < 6";
    TCut mycut_np = "2 < sqrt(pow(NegativeP4_kin_X, 2.) + pow(NegativeP4_kin_Y, 2.) + pow(NegativeP4_kin_Z, 2.)) && sqrt(pow(NegativeP4_kin_X, 2.) + pow(NegativeP4_kin_Y, 2.) + pow(NegativeP4_kin_Z, 2.)) < 6";
    TCut mycut = mycut_beam + mycut_pp + mycut_np;
    dataloader->PrepareTrainingAndTestTree(mycut, "SplitMode=Random:NormMode=NumEvents:!V");

    // Book a BDT method
    //factory.BookMethod(dataloader, TMVA::Types::kBDT, "BDT","!H:!V:NTrees=50:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20");
    // Book MLP method for multi-class classification
    factory.BookMethod(dataloader, TMVA::Types::kMLP, "MLP","H:!V:VarTransform=N:NeuronType=tanh:NCycles=3000:HiddenLayers=N+5:TestRate=5:EstimatorType=MSE");
    //Train, test, and evaluate the classifier
    factory.TrainAllMethods();
    factory.TestAllMethods();
    factory.EvaluateAllMethods();
    
    // Save the output
    outputFile->Close();
    inputFile_SIG->Close();
    for (int i = 0; i < nbkg; i ++) 
      inputFile_BKG[i]->Close();
    // Clean up
    delete dataloader;
    delete outputFile;
    delete inputFile_SIG;
    for (int i = 0; i < nbkg; i ++) 
      delete inputFile_BKG[i];

    return 0;
}
