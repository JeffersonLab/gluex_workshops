#include "../utils.h"
#include "../data_reader.h"
#include "TCanvas.h"

void run_ex1(Double_t prob_cut,TString add_name){

    set_style();
    
    Double_t title_offset = 0.85;
    Double_t title_size = 0.075;
    Double_t label_size = 0.05;
    
    TString file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4_Fit.root";
    TTree *tree = get_tree(file_name,"3Pi_P4_Fit_Tree");
    
    Int_t n_entries = tree->GetEntries();
    
    TH2F *Mx_p_vs_IM_pippimgg = new TH2F("Mx_p_vs_IM_pippimgg","",100,0.4,1.5,100,0.4,1.5);
    set_axis(Mx_p_vs_IM_pippimgg->GetXaxis(),"M(#pi^{+},#pi^{-},#gamma_{1},#gamma_{2}) [GeV/c^{2}]",title_offset,title_size,label_size);
    set_axis(Mx_p_vs_IM_pippimgg->GetYaxis(),"M_{x}(p) [GeV/c^{2}]",title_offset,title_size,label_size);
    Mx_p_vs_IM_pippimgg->SetTitle("Reconstructed Values");
    
    TH2F *Mx_p_vs_IM_pippimgg_fit = new TH2F("Mx_p_vs_IM_pippimgg_fit","",100,0.4,1.5,100,0.4,1.5);
    set_axis(Mx_p_vs_IM_pippimgg_fit->GetXaxis(),"M(#pi^{+},#pi^{-},#gamma_{1},#gamma_{2}) [GeV/c^{2}]",title_offset,title_size,label_size);
    set_axis(Mx_p_vs_IM_pippimgg_fit->GetYaxis(),"M_{x}(p) [GeV/c^{2}]",title_offset,title_size,label_size);
    Mx_p_vs_IM_pippimgg_fit->SetTitle("Fitted Values");
    
    TH2F *Px_vs_Ex = new TH2F("Px_vs_Ex","",100,-2.0,2.0,100,0.0,2.0);
    set_axis(Px_vs_Ex->GetXaxis(),"Missing Energy [GeV]",title_offset,title_size,label_size);
    set_axis(Px_vs_Ex->GetYaxis(),"Missing Momentum [GeV/c]",title_offset,title_size,label_size);
    
    TH2F *Px_vs_Ex_fit = new TH2F("Px_vs_Ex_fit","",100,-2.0,2.0,100,0.0,2.0);
    set_axis(Px_vs_Ex_fit->GetXaxis(),"Missing Energy [GeV]",title_offset,title_size,label_size);
    set_axis(Px_vs_Ex_fit->GetYaxis(),"Missing Momentum [GeV/c]",title_offset,title_size,label_size);
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t ev=0;ev<n_entries;ev++){
        tree->GetEntry(ev);
        
        if(KFit_Prob >= prob_cut){
           Mx_p_vs_IM_pippimgg->Fill(IM_pippimgg,Mx_p);
           Mx_p_vs_IM_pippimgg_fit->Fill(IM_pippimgg_fit,Mx_p_fit);
        
           Px_vs_Ex->Fill(Ex,Px);
           Px_vs_Ex_fit->Fill(Ex_fit,Px_fit);
        }
        
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    
    TCanvas *c_M_rec = new TCanvas("c_M_rec","",1);
    Mx_p_vs_IM_pippimgg->Draw("COLZ");
    gPad->SetLogz();
    c_M_rec->SaveAs("Mx_p_vs_IM_pippimgg_rec_" + add_name + ".png");
    
    TCanvas *c_M_fit = new TCanvas("c_M_fit","",1);
    Mx_p_vs_IM_pippimgg_fit->Draw("COLZ");
    c_M_fit->SaveAs("Mx_p_vs_IM_pippimgg_fit_" + add_name + ".png");
    
    TCanvas *c_em_rec = new TCanvas("c_em_rec","",1);
    Px_vs_Ex->Draw("COLZ");
    c_em_rec->SaveAs("Px_vs_Ex_rec_" + add_name + ".png");
    
    TCanvas *c_em_fit = new TCanvas("c_em_fit","",1);
    Px_vs_Ex_fit->Draw("COLZ");
    c_em_fit->SaveAs("Px_vs_Ex_fit_" + add_name + ".png");

}
