#include "../utils.h"
#include "../data_reader.h"
#include "TCanvas.h"

void run_ex2(Int_t fit_type){
    set_style();
    
    Double_t title_offset = 0.85;
    Double_t title_size = 0.075;
    Double_t label_size = 0.05;
    
    TString file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4_Fit.root";
    TString tree_name = "3Pi_P4_Fit_Tree";
    TString out_add_name = "_p4_fit.pdf";
    TString hist_title = "P4-Fit";
    Int_t ndf = 4;
    
    if(fit_type == 1){
        file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4Vertex_Fit.root";
        tree_name = "3Pi_P4Vertex_Fit_Tree";
        out_add_name = "_p4vertex_fit.pdf";
        hist_title = "P4-Vertex-Fit";
        ndf = 7;
    }
    
    TTree *tree = get_tree(file_name,tree_name);
    Int_t n_entries = tree->GetEntries();
    
    Double_t min_mass_cut[4] = {0.0,0.5,0.7,0.92};
    Double_t max_mass_cut[4] = {2.0,0.58,0.85,1.06};
    Double_t prob_cuts[4] = {0.0,0.01,0.1,0.5};
    
    TH1F *chisq_dist[4];
    TH1F *ideal_chisq_dist[4];
    TH1F *prob_dist[4];
    TLine *ideal_prob[4];
    TString add_name[4] = {"_no_cut","_eta_cut","_omega_cut","_phi_cut"};
    TString add_mass_name[4] = {"_no_cut","_001_cut","_01_cut","_05_cut"};
    TH1F *IM_hist[4];
    TString add_prob_ax[2] = {"NDF=4)","NDF=7)"};
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t i=0;i<4;i++){
        chisq_dist[i] = new TH1F("chisq_dist" + add_name[i],"",100,0.0,30.0);
        chisq_dist[i]->SetLineWidth(3);
        chisq_dist[i]->SetLineColor(kBlack);
        set_axis(chisq_dist[i]->GetXaxis(),"#chi^{2}",title_offset,title_size,label_size);
        set_axis(chisq_dist[i]->GetYaxis(),"Entries",1.2,title_size,label_size);
        chisq_dist[i]->SetTitle(hist_title);
        
        prob_dist[i] = new TH1F("prob_dist" + add_name[i],"",100,0.0,1.01);
        prob_dist[i]->SetLineWidth(3);
        prob_dist[i]->SetLineColor(kBlack);
        set_axis(prob_dist[i]->GetXaxis(),"Probability(#chi^{2},"+add_prob_ax[fit_type],title_offset,title_size,label_size);
        set_axis(prob_dist[i]->GetYaxis(),"Entries",title_offset,title_size,label_size);
        prob_dist[i]->SetTitle(hist_title);
        
        IM_hist[i] = new TH1F("IM_pippimgg" + add_mass_name[i],"",500,0.3,1.5);
        set_axis(IM_hist[i]->GetXaxis(),"M(#pi^{+},#pi^{-},#gamma_{1},#gamma_{2})",title_offset,title_size,label_size);
        set_axis(IM_hist[i]->GetYaxis(),"Entries",title_offset + 0.25,title_size,label_size);
        IM_hist[i]->SetLineWidth(3);
        IM_hist[i]->SetLineColor(kBlack);
        IM_hist[i]->SetTitle(hist_title);
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t ev=0;ev<n_entries;ev++){
        tree->GetEntry(ev);
        
        //+++++++++++++++++++++++
        for(Int_t m=0;m<4;m++){
            if(IM_pippimgg >= min_mass_cut[m] && IM_pippimgg <= max_mass_cut[m]){
                chisq_dist[m]->Fill(kin_chisq);
                prob_dist[m]->Fill(KFit_Prob);
            }
            
            if(KFit_Prob >= prob_cuts[m]){
                IM_hist[m]->Fill(IM_pippimgg);
            }
            
        }
        //+++++++++++++++++++++++
        
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    

    TCanvas *c_init = new TCanvas("c_ini","",1200,500);
    c_init->Divide(2);
    c_init->cd(1);
    
    ideal_chisq_dist[0] = get_ideal_chisq_dist(chisq_dist[0],ndf);
    ideal_chisq_dist[0]->SetLineWidth(3);
    ideal_chisq_dist[0]->SetLineColor(kRed);
    
    TLegend *leg_init_chisq = new TLegend(0.6,0.6,0.9,0.9);
    leg_init_chisq->SetFillColor(0);
    leg_init_chisq->AddEntry(chisq_dist[0],"From Data");
    leg_init_chisq->AddEntry(ideal_chisq_dist[0],"Theory");
    
    chisq_dist[0]->Draw();
    ideal_chisq_dist[0]->Draw("sameHIST");
    leg_init_chisq->Draw("same");
    
    c_init->cd(2);
    ideal_prob[0] = get_ideal_prob_dist(prob_dist[0],0.99);
    ideal_prob[0]->SetLineWidth(3.0);
    ideal_prob[0]->SetLineColor(kRed);
    
    TLegend *leg_init_prob = new TLegend(0.6,0.6,0.9,0.9);
    leg_init_prob->SetFillColor(0);
    leg_init_prob->AddEntry(prob_dist[0],"From Data");
    leg_init_prob->AddEntry(ideal_prob[0],"Theory","l");
    
    prob_dist[0]->Draw();
    ideal_prob[0]->Draw("same");
    leg_init_prob->Draw("same");
    
    gPad->SetLogy();
    c_init->cd();
    c_init->SaveAs("KinFit_ChiSquare_Prob_noCuts" + out_add_name);
    
    
    TLegend *leg_chisq = new TLegend(0.6,0.6,0.9,0.9);
    leg_chisq->SetFillColor(0);
    leg_chisq->AddEntry(chisq_dist[0],"No cuts");
    leg_chisq->AddEntry(ideal_chisq_dist[0],"Theory");
    leg_chisq->AddEntry(chisq_dist[1],"m_{#eta}-cut");
    leg_chisq->AddEntry(chisq_dist[2],"m_{#omega}-cut");
    leg_chisq->AddEntry(chisq_dist[3],"m_{#phi}-cut");
    
    TLegend *leg_prob = new TLegend(0.6,0.6,0.9,0.9);
    leg_prob->SetFillColor(0);
    leg_prob->AddEntry(chisq_dist[0],"No cuts");
    leg_prob->AddEntry(ideal_chisq_dist[0],"Theory");
    leg_prob->AddEntry(chisq_dist[1],"m_{#eta}-cut");
    leg_prob->AddEntry(chisq_dist[2],"m_{#omega}-cut");
    leg_prob->AddEntry(chisq_dist[3],"m_{#phi}-cut");
    
    TCanvas *c_sum = new TCanvas("c_sum","",1200,500);
    c_sum->Divide(2);
    c_sum->cd(1);
    chisq_dist[0]->Draw();
    ideal_chisq_dist[0]->Draw("sameHIST");
    
    chisq_dist[1]->SetLineColor(8);
    chisq_dist[1]->Draw("same");
    
    chisq_dist[2]->SetLineColor(4);
    chisq_dist[2]->Draw("same");
    
    chisq_dist[3]->SetLineColor(28);
    chisq_dist[3]->Draw("same");
    
    leg_chisq->Draw("same");
    gPad->SetLogy();
    c_sum->cd(2);
    prob_dist[0]->Draw();
    ideal_prob[0]->Draw("sameHIST");
    
    prob_dist[1]->SetLineColor(8);
    prob_dist[1]->Draw("same");
    
    prob_dist[2]->SetLineColor(4);
    prob_dist[2]->Draw("same");
    
    prob_dist[3]->SetLineColor(28);
    prob_dist[3]->Draw("same");
    
    leg_prob->Draw("same");
    gPad->SetLogy();
    c_sum->cd();
    c_sum->SaveAs("KinFit_ChiSquare_Prob_Cuts" + out_add_name);
    
    IM_hist[1]->SetLineColor(4);
    IM_hist[2]->SetLineColor(8);
    IM_hist[3]->SetLineColor(kRed);
    
    TLegend *leg_m = new TLegend(0.6,0.6,0.9,0.9);
    leg_m->SetFillColor(0);
    leg_m->AddEntry(IM_hist[0],"No cuts");
    leg_m->AddEntry(IM_hist[1],"Prob. #geq 0.01");
    leg_m->AddEntry(IM_hist[2],"Prob. #geq 0.1");
    leg_m->AddEntry(IM_hist[3],"Prob. #geq 0.5");
    
    
    TCanvas *c_res = new TCanvas("c_res","",1);
    IM_hist[0]->Draw();
    IM_hist[1]->Draw("same");
    IM_hist[2]->Draw("same");
    IM_hist[3]->Draw("same");
    leg_m->Draw("same");
    c_res->cd();
    c_res->SaveAs("M_pippimgg_Cuts" + out_add_name);
   

}
