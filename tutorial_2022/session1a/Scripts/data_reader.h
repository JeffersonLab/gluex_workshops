#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <iostream>

//Provided by: Daniel Lersch
//For suggestions or complaints, please write email to: dlersch@jlab.org

//Define all important variables here:

//Kinematic fit chi^2 and probability:
Double_t KFit_Prob = 0.0;
Float_t kin_chisq = 0.0;

//Reconstructed particle 4-Vectors:
TLorentzVector *p_p4_meas = 0;//proton
TLorentzVector *pip_p4_meas = 0;//pi+
TLorentzVector *pim_p4_meas = 0;//pi-
TLorentzVector *g1_p4_meas = 0;//photon 1
TLorentzVector *g2_p4_meas = 0;//photon 2

//Kinematically fitted particle 4-Vectors:
TLorentzVector *p_p4_kin = 0;//proton
TLorentzVector *pip_p4_kin = 0;//pi+
TLorentzVector *pim_p4_kin = 0;//pi-
TLorentzVector *g1_p4_kin = 0;//photon 1
TLorentzVector *g2_p4_kin = 0;//photon 2

//Reconstructed masses, momenta and energies:
Double_t Mx_p = 0.0;
Double_t Mx_pgg = 0.0;
Double_t Mx_ppippim = 0.0;
Double_t Mx_ppippimgg = 0.0;
Double_t IM_pippim = 0.0;
Double_t IM_gg = 0.0;
Double_t IM_pippimgg = 0.0;
Double_t Px = 0.0;
Double_t Ex = 0.0;

//Fitted masses, momenta and energies:
Double_t Mx_p_fit = 0.0;
Double_t Mx_pgg_fit = 0.0;
Double_t Mx_ppippim_fit = 0.0;
Double_t Mx_ppippimgg_fit = 0.0;
Double_t IM_pippim_fit = 0.0;
Double_t IM_gg_fit = 0.0;
Double_t IM_pippimgg_fit = 0.0;
Double_t Px_fit = 0.0;
Double_t Ex_fit = 0.0;

//Pulls, more pulls!
//P4-Pulls:
Double_t p_pull_px = 0.0;
Double_t p_pull_py = 0.0;
Double_t p_pull_pz = 0.0;

Double_t pip_pull_px = 0.0;
Double_t pip_pull_py = 0.0;
Double_t pip_pull_pz = 0.0;

Double_t pim_pull_px = 0.0;
Double_t pim_pull_py = 0.0;
Double_t pim_pull_pz = 0.0;

Double_t g1_pull_px = 0.0;
Double_t g1_pull_py = 0.0;
Double_t g1_pull_pz = 0.0;

Double_t g2_pull_px = 0.0;
Double_t g2_pull_py = 0.0;
Double_t g2_pull_pz = 0.0;

//Vertex pulls:
Double_t p_pull_xx = 0.0;
Double_t p_pull_xy = 0.0;
Double_t p_pull_xz = 0.0;

Double_t pip_pull_xx = 0.0;
Double_t pip_pull_xy = 0.0;
Double_t pip_pull_xz = 0.0;

Double_t pim_pull_xx = 0.0;
Double_t pim_pull_xy = 0.0;
Double_t pim_pull_xz = 0.0;

Double_t g1_pull_e = 0.0;
Double_t g1_pull_xx = 0.0;
Double_t g1_pull_xy = 0.0;
Double_t g1_pull_xz = 0.0;

Double_t g2_pull_e = 0.0;
Double_t g2_pull_xx = 0.0;
Double_t g2_pull_xy = 0.0;
Double_t g2_pull_xz = 0.0;

//-------------------------------------------------

//Load the ROOT-file, get the ROOT-tree and set the variables:
TTree *get_tree(TString file_name, TString tree_name){
    TFile *data_file = TFile::Open(file_name);
    TTree *data_tree = (TTree*)data_file->Get(tree_name);
    
    data_tree->SetBranchStatus("*",0);
    data_tree->SetBranchStatus("KFit_Prob",1);
    data_tree->SetBranchStatus("kin_chisq",1);
    
    data_tree->SetBranchStatus("Mx_p",1);
    data_tree->SetBranchStatus("Mx_pgg",1);
    data_tree->SetBranchStatus("Mx_ppippim",1);
    data_tree->SetBranchStatus("Mx_ppippimgg",1);
    data_tree->SetBranchStatus("Px",1);
    data_tree->SetBranchStatus("Ex",1);
    data_tree->SetBranchStatus("IM_pippim",1);
    data_tree->SetBranchStatus("IM_pippimgg",1);
    data_tree->SetBranchStatus("IM_gg",1);
    
    data_tree->SetBranchStatus("Mx_p_fit",1);
    data_tree->SetBranchStatus("Mx_pgg_fit",1);
    data_tree->SetBranchStatus("Mx_ppippim_fit",1);
    data_tree->SetBranchStatus("Mx_ppippimgg_fit",1);
    data_tree->SetBranchStatus("Px_fit",1);
    data_tree->SetBranchStatus("Ex_fit",1);
    data_tree->SetBranchStatus("IM_pippim_fit",1);
    data_tree->SetBranchStatus("IM_pippimgg_fit",1);
    data_tree->SetBranchStatus("IM_gg_fit",1);
    
    data_tree->SetBranchAddress("KFit_Prob",&KFit_Prob);
    data_tree->SetBranchAddress("kin_chisq",&kin_chisq);
    
    data_tree->SetBranchAddress("Mx_p",&Mx_p);
    data_tree->SetBranchAddress("Mx_pgg",&Mx_pgg);
    data_tree->SetBranchAddress("Mx_ppippim",&Mx_ppippim);
    data_tree->SetBranchAddress("Mx_ppippimgg",&Mx_ppippimgg);
    data_tree->SetBranchAddress("IM_pippim",&IM_pippim);
    data_tree->SetBranchAddress("IM_pippimgg",&IM_pippimgg);
    data_tree->SetBranchAddress("IM_gg",&IM_gg);
    data_tree->SetBranchAddress("Px",&Px);
    data_tree->SetBranchAddress("Ex",&Ex);
    
    data_tree->SetBranchAddress("Mx_p_fit",&Mx_p_fit);
    data_tree->SetBranchAddress("Mx_pgg_fit",&Mx_pgg_fit);
    data_tree->SetBranchAddress("Mx_ppippim_fit",&Mx_ppippim_fit);
    data_tree->SetBranchAddress("Mx_ppippimgg_fit",&Mx_ppippimgg_fit);
    data_tree->SetBranchAddress("IM_pippim_fit",&IM_pippim_fit);
    data_tree->SetBranchAddress("IM_pippimgg_fit",&IM_pippimgg_fit);
    data_tree->SetBranchAddress("IM_gg_fit",&IM_gg_fit);
    data_tree->SetBranchAddress("Px_fit",&Px_fit);
    data_tree->SetBranchAddress("Ex_fit",&Ex_fit);
    
    return data_tree;
}

//-------------------------------------------------

//Add pulls, if necessary:
void load_pulls(TTree *tree, Int_t fitter_type){
    //0: P4-Fit, 1: P4-Vertex-Fit
    tree->SetBranchStatus("p_pull_px",1);
    tree->SetBranchStatus("p_pull_py",1);
    tree->SetBranchStatus("p_pull_pz",1);
    
    tree->SetBranchStatus("pip_pull_px",1);
    tree->SetBranchStatus("pip_pull_py",1);
    tree->SetBranchStatus("pip_pull_pz",1);
    
    tree->SetBranchStatus("pim_pull_px",1);
    tree->SetBranchStatus("pim_pull_py",1);
    tree->SetBranchStatus("pim_pull_pz",1);
    
    tree->SetBranchAddress("p_pull_px",&p_pull_px);
    tree->SetBranchAddress("p_pull_py",&p_pull_py);
    tree->SetBranchAddress("p_pull_pz",&p_pull_pz);
    
    tree->SetBranchAddress("pip_pull_px",&pip_pull_px);
    tree->SetBranchAddress("pip_pull_py",&pip_pull_py);
    tree->SetBranchAddress("pip_pull_pz",&pip_pull_pz);
    
    tree->SetBranchAddress("pim_pull_px",&pim_pull_px);
    tree->SetBranchAddress("pim_pull_py",&pim_pull_py);
    tree->SetBranchAddress("pim_pull_pz",&pim_pull_pz);
    
    if(fitter_type == 0){//P4-Fit for photons:
        
        tree->SetBranchStatus("g1_pull_px",1);
        tree->SetBranchStatus("g1_pull_py",1);
        tree->SetBranchStatus("g1_pull_pz",1);
        
        tree->SetBranchStatus("g2_pull_px",1);
        tree->SetBranchStatus("g2_pull_py",1);
        tree->SetBranchStatus("g2_pull_pz",1);
        
        tree->SetBranchAddress("g1_pull_px",&g1_pull_px);
        tree->SetBranchAddress("g1_pull_py",&g1_pull_py);
        tree->SetBranchAddress("g1_pull_pz",&g1_pull_pz);
        
        tree->SetBranchAddress("g2_pull_px",&g2_pull_px);
        tree->SetBranchAddress("g2_pull_py",&g2_pull_py);
        tree->SetBranchAddress("g2_pull_pz",&g2_pull_pz);
        
    }else if(fitter_type == 1){//P4-Vertex-Fit for all particles:
        
        tree->SetBranchStatus("p_pull_xx",1);
        tree->SetBranchStatus("p_pull_xy",1);
        tree->SetBranchStatus("p_pull_xz",1);
        
        tree->SetBranchStatus("pip_pull_xx",1);
        tree->SetBranchStatus("pip_pull_xy",1);
        tree->SetBranchStatus("pip_pull_xz",1);
        
        tree->SetBranchStatus("pim_pull_xx",1);
        tree->SetBranchStatus("pim_pull_xy",1);
        tree->SetBranchStatus("pim_pull_xz",1);
        
        tree->SetBranchAddress("p_pull_xx",&p_pull_xx);
        tree->SetBranchAddress("p_pull_xy",&p_pull_xy);
        tree->SetBranchAddress("p_pull_xz",&p_pull_xz);
        
        tree->SetBranchAddress("pip_pull_xx",&pip_pull_xx);
        tree->SetBranchAddress("pip_pull_xy",&pip_pull_xy);
        tree->SetBranchAddress("pip_pull_xz",&pip_pull_xz);
        
        tree->SetBranchAddress("pim_pull_xx",&pim_pull_xx);
        tree->SetBranchAddress("pim_pull_xy",&pim_pull_xy);
        tree->SetBranchAddress("pim_pull_xz",&pim_pull_xz);
        
        tree->SetBranchStatus("g1_pull_e",1);
        tree->SetBranchStatus("g1_pull_xx",1);
        tree->SetBranchStatus("g1_pull_xy",1);
        tree->SetBranchStatus("g1_pull_xz",1);
        
        tree->SetBranchStatus("g2_pull_e",1);
        tree->SetBranchStatus("g2_pull_xx",1);
        tree->SetBranchStatus("g2_pull_xy",1);
        tree->SetBranchStatus("g2_pull_xz",1);
        
        tree->SetBranchAddress("g1_pull_e",&g1_pull_e);
        tree->SetBranchAddress("g1_pull_xx",&g1_pull_xx);
        tree->SetBranchAddress("g1_pull_xy",&g1_pull_xy);
        tree->SetBranchAddress("g1_pull_xz",&g1_pull_xz);
        
        tree->SetBranchAddress("g2_pull_e",&g2_pull_e);
        tree->SetBranchAddress("g2_pull_xx",&g2_pull_xx);
        tree->SetBranchAddress("g2_pull_xy",&g2_pull_xy);
        tree->SetBranchAddress("g2_pull_xz",&g2_pull_xz);
        
    }else{
        std::cout <<"  "<< std::endl;
        std::cout <<"Invalid input. Please enter either 0 (P4-Fit) or 1 (P4-Vertex-Fit). Have a great day."<< std::endl;
        std::cout <<"  "<< std::endl;
    }
}

//-------------------------------------------------

//Add reconstructed and fitted particle 4-vectors, if necessary:
void load_particle_4vectors(TTree *tree){
    //Reconstructed:
    tree->SetBranchStatus("p_p4_meas",1);
    tree->SetBranchStatus("pip_p4_meas",1);
    tree->SetBranchStatus("pim_p4_meas",1);
    tree->SetBranchStatus("g1_p4_meas",1);
    tree->SetBranchStatus("g2_p4_meas",1);
    
    tree->SetBranchAddress("p_p4_meas",&p_p4_meas);
    tree->SetBranchAddress("pip_p4_meas",&pip_p4_meas);
    tree->SetBranchAddress("pim_p4_meas",&pim_p4_meas);
    tree->SetBranchAddress("g1_p4_meas",&g1_p4_meas);
    tree->SetBranchAddress("g2_p4_meas",&g2_p4_meas);
    
    //Fitted:
    tree->SetBranchStatus("p_p4_kin",1);
    tree->SetBranchStatus("pip_p4_kin",1);
    tree->SetBranchStatus("pim_p4_kin",1);
    tree->SetBranchStatus("g1_p4_kin",1);
    tree->SetBranchStatus("g2_p4_kin",1);
    
    tree->SetBranchAddress("p_p4_kin",&p_p4_kin);
    tree->SetBranchAddress("pip_p4_kin",&pip_p4_kin);
    tree->SetBranchAddress("pim_p4_kin",&pim_p4_kin);
    tree->SetBranchAddress("g1_p4_kin",&g1_p4_kin);
    tree->SetBranchAddress("g2_p4_kin",&g2_p4_kin);
}

//-------------------------------------------------

//Create a .root file, where you might want to store your results:
TFile *create_output_file(TString file_name){
    TFile *outfile = new TFile(file_name + ".root","RECREATE");
    return outfile;
}

//-------------------------------------------------

