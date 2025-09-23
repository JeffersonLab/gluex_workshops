#include <TFile.h>
#include <TTree.h>
#include <TMVA/Reader.h>
#include <TMVA/Tools.h>
#include <iostream>
#include <vector>
#include <string>

void ApplyModel(TString input_file = "", TString output_file = "", TString xml_bdt_file = "", TString xml_mlp_file = "") {
    // Initialize TMVA
    TMVA::Tools::Instance();

    // Load the trained model
    TMVA::Reader *reader = new TMVA::Reader("!Color:!Silent");
    
    //int var_nb = 26;
    //TString s_features[] = {"thrust_perp","beam_e","pt_gg","pt_pi0","pt_eta","chi2","cos_gg","cos_eta","mk_gpi0_min","mk_gpi0_max","mk_ggpi0", "ang_gg_pi0","mma","mmp","g_pl1", "g_pl2", "g_pl3", "g_pl4", "g_th1", "g_th2", "g_th3", "g_th4", "g_pt1", "g_pt2", "g_pt3", "g_pt4"};
    //Float_t var[26];
    
    //int var_nb = 25;
    //TString s_features[] = {"thrust_perp","beam_e","pt_gg","pt_pi0","pt_eta","cos_gg","cos_eta","mk_gpi0_min","mk_gpi0_max","mk_ggpi0", "ang_gg_pi0","mma","mmp","g_pl1", "g_pl2", "g_pl3", "g_pl4", "g_th1", "g_th2", "g_th3", "g_th4", "g_pt1", "g_pt2", "g_pt3", "g_pt4"};
    //Float_t var[var_nb];

    //int var_nb = 22;
    //TString s_features[] = {"thrust_perp","beam_e","pt_gg","pt_pi0","pt_eta","cos_gg","cos_eta","ang_gg_pi0","mma","mmp","g_pl1", "g_pl2", "g_pl3", "g_pl4", "g_th1", "g_th2", "g_th3", "g_th4", "g_pt1", "g_pt2", "g_pt3", "g_pt4"};

    int var_nb = 7;
    TString s_features[] = {"thrust_perp","pt_gg","pt_pi0","pt_eta","cos_gg","cos_eta","ang_gg_pi0"};
    Float_t var[var_nb];

    for (int i = 0; i < var_nb; i ++)
      reader->AddVariable(s_features[i], &var[i]);
    
    // Book the methods
    //reader->BookMVA("BDT", "dataset/weights/TMVAClassification_BDT.weights.xml");
    reader->BookMVA("BDT", xml_bdt_file);
    //reader->BookMVA("MLP", xml_mlp_file);
    
    // Load the new data
    TFile *input = TFile::Open(input_file);
    TTree *tree = (TTree*)input->Get("pinn");
    
    // Set branch addresses
    for (int i = 0; i < var_nb; i ++)
      tree->SetBranchAddress(s_features[i], &var[i]);
    
    // Create a new file and tree to store the results
    TFile *outputFile = TFile::Open(output_file, "RECREATE");
    TTree *outputTree = tree->CloneTree(0);
    Float_t BDT_response, MLP_response;
    outputTree->Branch("BDT_response", &BDT_response, "BDT_response/F");
    //outputTree->Branch("MLP_response", &MLP_response, "MLP_response/F");
    
    // Loop over the entries and classify
    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; ++i) {
      tree->GetEntry(i);
      BDT_response = reader->EvaluateMVA("BDT");
      //MLP_response = reader->EvaluateMVA("MLP");
      outputTree->Fill();
    }
    
    // Save the results
    outputFile->Write();
    outputFile->Close();
    input->Close();
    delete reader;
}

int main(int argc, char* argv[]) {
  
  TString input_file = std::string(argv[1]);
  TString output_file = std::string(argv[2]);
  TString xml_bdt_file = std::string(argv[3]);
  TString xml_mlp_file = std::string(argv[4]);
  
  ApplyModel(input_file, output_file, xml_bdt_file, xml_mlp_file);
  return 0;
}
