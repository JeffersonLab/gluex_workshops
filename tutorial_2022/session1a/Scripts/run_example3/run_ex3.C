#include "../utils.h"
#include "../data_reader.h"
#include "TCanvas.h"

void run_ex3(Int_t fitter_type){
    set_style();
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    
    Double_t title_offset_x = 0.85;
    Double_t title_offset_y = 1.1;
    Double_t title_size = 0.075;
    Double_t label_size = 0.05;
    
    TString file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4_Fit.root";
    TString tree_name = "3Pi_P4_Fit_Tree";
    TString hist_title = "P4-Fit";
    TString out_add_name = "_p4_fit.pdf";
    
    if(fitter_type == 1){
        file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4Vertex_Fit.root";
        tree_name = "3Pi_P4Vertex_Fit_Tree";
        hist_title = "P4-Vertex-Fit";
        TString out_add_name = "_p4vertex_fit.pdf";
    }
    
    TTree *tree = get_tree(file_name,tree_name);
    load_pulls(tree,fitter_type);
    load_particle_4vectors(tree);
    Int_t n_entries = tree->GetEntries();
    
    TH1F *h_pim_pull_pz[4];
    TH1F *h_pip_pull_pz[4];
    TString add_cut_name[4] = {"_no_cut","_0001_cut","_001_cut","_01_cut"};
    Double_t prob_cut[4] = {0.0,0.001,0.01,0.1};
    Int_t pull_colors[4] = {1,6,4,2};
    TString x_axis_title[2] = {"P_{z}-Pull (#pi^{-})","P_{z}-Pull (#pi^{+})"};
    Double_t pull_min = -5.0;
    Double_t pull_max = 5.0;
    
    TString leg_entries[4] = {"No Cut","Prob. #geq 0.001","Prob. #geq 0.01","Prob. #geq 0.1"};
    TLegend *leg_pip_pull = new TLegend(0.6,0.6,0.9,0.9);
    leg_pip_pull->SetFillColor(0);
    TLegend *leg_pim_pull = new TLegend(0.6,0.6,0.9,0.9);
    leg_pim_pull->SetFillColor(0);
    
    TH1F *fitted_pim_pull_pz[4];
    TH1F *fitted_pip_pull_pz[4];
    
    TH2F *h_pim_pull_vs_phi[4];
    TProfile *t_pim_pull_vs_phi[4];
    //++++++++++++++++++++++++++++++++++++++++++
    for(Int_t h=0;h<4;h++){
        h_pim_pull_pz[h] = new TH1F("h_pim_pull_pz" + add_cut_name[h],"",200,pull_min,pull_max);
        h_pim_pull_pz[h]->SetLineWidth(3.0);
        h_pim_pull_pz[h]->SetLineColor(pull_colors[h]);
        set_axis(h_pim_pull_pz[h]->GetXaxis(),x_axis_title[0],title_offset_x,title_size,label_size);
        set_axis(h_pim_pull_pz[h]->GetYaxis(),"Entries",title_offset_y,title_size,label_size);
        h_pim_pull_pz[h]->SetTitle(hist_title);
        leg_pim_pull->AddEntry(h_pim_pull_pz[h],leg_entries[h]);
        
        h_pip_pull_pz[h] = new TH1F("h_pip_pull_pz" + add_cut_name[h],"",200,pull_min,pull_max);
        h_pip_pull_pz[h]->SetLineWidth(3.0);
        h_pip_pull_pz[h]->SetLineColor(pull_colors[h]);
        set_axis(h_pip_pull_pz[h]->GetXaxis(),x_axis_title[1],title_offset_x,title_size,label_size);
        set_axis(h_pip_pull_pz[h]->GetYaxis(),"Entries",title_offset_y,title_size,label_size);
        h_pip_pull_pz[h]->SetTitle(hist_title);
        leg_pip_pull->AddEntry(h_pim_pull_pz[h],leg_entries[h]);
        
        h_pim_pull_vs_phi[h] = new TH2F("h_pim_pull_vs_phi" + add_cut_name[h],"",36,-180,180,200,pull_min,pull_max);
        set_axis(h_pim_pull_vs_phi[h]->GetXaxis(),"#phi [deg]",title_offset_x,title_size,label_size);
        set_axis(h_pim_pull_vs_phi[h]->GetYaxis(),x_axis_title[0],0.65,title_size,label_size);
        h_pim_pull_vs_phi[h]->SetTitle(leg_entries[h]);
        
        t_pim_pull_vs_phi[h] = new TProfile("t_pim_pull_vs_phi" + add_cut_name[h],"",36,-180,180);
        t_pim_pull_vs_phi[h]->SetLineWidth(3.0);
        t_pim_pull_vs_phi[h]->SetLineColor(kRed);
    }
    //++++++++++++++++++++++++++++++++++++++++++
    
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t ev=0;ev<n_entries;ev++){
        tree->GetEntry(ev);
        
        //+++++++++++++++++++++++
        for(Int_t h=0;h<4;h++){
            if(KFit_Prob >= prob_cut[h]){
                h_pim_pull_pz[h]->Fill(pim_pull_pz);
                h_pip_pull_pz[h]->Fill(pip_pull_pz);
                
                h_pim_pull_vs_phi[h]->Fill(pim_p4_meas->Phi()*TMath::RadToDeg(),pim_pull_pz);
                t_pim_pull_vs_phi[h]->Fill(pim_p4_meas->Phi()*TMath::RadToDeg(),pim_pull_pz);
            }
        }
        //+++++++++++++++++++++++
        
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    
    TGraph *gr_mean_pim_pull_pz = new TGraph(4);
    TGraph *gr_mean_pip_pull_pz = new TGraph(4);
    TGraph *gr_sigma_pim_pull_pz = new TGraph(4);
    TGraph *gr_sigma_pip_pull_pz = new TGraph(4);
    
    gr_mean_pim_pull_pz->SetMarkerStyle(20);
    gr_mean_pim_pull_pz->SetMarkerColor(kBlack);
    gr_mean_pim_pull_pz->SetLineWidth(2.0);
    gr_mean_pim_pull_pz->SetLineColor(kBlack);
    set_axis(gr_mean_pim_pull_pz->GetXaxis(),"Probability Cut",title_offset_x,title_size,label_size);
    set_axis(gr_mean_pim_pull_pz->GetYaxis(),"Mean #mu from P_{z}-Pull",title_offset_y,title_size,label_size);
    
    gr_sigma_pim_pull_pz->SetMarkerStyle(20);
    gr_sigma_pim_pull_pz->SetMarkerColor(kBlack);
    gr_sigma_pim_pull_pz->SetLineWidth(2.0);
    gr_sigma_pim_pull_pz->SetLineColor(kBlack);
    set_axis(gr_sigma_pim_pull_pz->GetXaxis(),"Probability Cut",title_offset_x,title_size,label_size);
    set_axis(gr_sigma_pim_pull_pz->GetYaxis(),"Width #sigma from P_{z}-Pull",title_offset_y,title_size,label_size);
    
    gr_mean_pip_pull_pz->SetMarkerStyle(21);
    gr_mean_pip_pull_pz->SetMarkerColor(kRed);
    gr_mean_pip_pull_pz->SetLineWidth(2.0);
    gr_mean_pip_pull_pz->SetLineColor(kRed);
    
    gr_sigma_pip_pull_pz->SetMarkerStyle(21);
    gr_sigma_pip_pull_pz->SetMarkerColor(kRed);
    gr_sigma_pip_pull_pz->SetLineWidth(2.0);
    gr_sigma_pip_pull_pz->SetLineColor(kRed);
    
    
    std::cout<<" "<< std::endl;
    //+++++++++++++++++++++++
    for(Int_t h=0;h<4;h++){
        std::cout<<"Fit pi- pz-pull "<<h<<" "<< std::endl;
        Double_t *current_pim_pull_pars = get_pull_pars(h_pim_pull_pz[h],-2.0,2.0,"RN");
        fitted_pim_pull_pz[h] = get_fitted_pull_dist(h_pim_pull_pz[h],current_pim_pull_pars);
        fitted_pim_pull_pz[h]->SetLineWidth(3);
        fitted_pim_pull_pz[h]->SetLineColor(pull_colors[h]);
        fitted_pim_pull_pz[h]->SetLineStyle(2);
        set_axis(fitted_pim_pull_pz[h]->GetXaxis(),"Fitted " + x_axis_title[0],title_offset_x,title_size,label_size);
        set_axis(fitted_pim_pull_pz[h]->GetYaxis(),"Entries",title_offset_y,title_size,label_size);
        
        gr_mean_pim_pull_pz->SetPoint(h,prob_cut[h],current_pim_pull_pars[1]);
        gr_sigma_pim_pull_pz->SetPoint(h,prob_cut[h],current_pim_pull_pars[2]);
        
        std::cout<<" "<< std::endl;
        
        std::cout<<"Fit pi+ pz-pull "<<h<<" "<< std::endl;
        Double_t *current_pip_pull_pars = get_pull_pars(h_pip_pull_pz[h],-2.0,2.0,"RN");
        fitted_pip_pull_pz[h] = get_fitted_pull_dist(h_pip_pull_pz[h],current_pip_pull_pars);
        fitted_pip_pull_pz[h]->SetLineWidth(3);
        fitted_pip_pull_pz[h]->SetLineColor(pull_colors[h]);
        fitted_pip_pull_pz[h]->SetLineStyle(2);
        set_axis(fitted_pip_pull_pz[h]->GetXaxis(),"Fitted " + x_axis_title[1],title_offset_x,title_size,label_size);
        set_axis(fitted_pip_pull_pz[h]->GetYaxis(),"Entries",title_offset_y,title_size,label_size);
        
        gr_mean_pip_pull_pz->SetPoint(h,prob_cut[h],current_pip_pull_pars[1]);
        gr_sigma_pip_pull_pz->SetPoint(h,prob_cut[h],current_pip_pull_pars[2]);
        
        std::cout<<" "<< std::endl;
    }
    //+++++++++++++++++++++++
    
    
    TCanvas *c_0 = new TCanvas("c_0","",1);
    h_pim_pull_pz[0]->Draw();
    c_0->cd();

    TCanvas *c_1 = new TCanvas("c_1","",1);
    h_pim_pull_pz[0]->Draw();
    h_pim_pull_pz[1]->Draw("same");
    h_pim_pull_pz[2]->Draw("same");
    h_pim_pull_pz[3]->Draw("same");
    leg_pim_pull->Draw("same");
    fitted_pim_pull_pz[0]->Draw("same");
    fitted_pim_pull_pz[1]->Draw("same");
    fitted_pim_pull_pz[2]->Draw("same");
    fitted_pim_pull_pz[3]->Draw("same");
    c_1->cd();
    c_1->SaveAs("Pim_Pz_Pulls"+out_add_name);
    
    TCanvas *c_2 = new TCanvas("c_2","",1200,500);
    c_2->Divide(2);
    c_2->cd(1);
    h_pim_pull_pz[0]->Draw();
    h_pim_pull_pz[1]->Draw("same");
    h_pim_pull_pz[2]->Draw("same");
    h_pim_pull_pz[3]->Draw("same");
    leg_pim_pull->Draw("same");
    fitted_pim_pull_pz[0]->Draw("same");
    fitted_pim_pull_pz[1]->Draw("same");
    fitted_pim_pull_pz[2]->Draw("same");
    fitted_pim_pull_pz[3]->Draw("same");
    c_2->cd(2);
    h_pip_pull_pz[0]->Draw();
    h_pip_pull_pz[1]->Draw("same");
    h_pip_pull_pz[2]->Draw("same");
    h_pip_pull_pz[3]->Draw("same");
    leg_pip_pull->Draw("same");
    fitted_pip_pull_pz[0]->Draw("same");
    fitted_pip_pull_pz[1]->Draw("same");
    fitted_pip_pull_pz[2]->Draw("same");
    fitted_pip_pull_pz[3]->Draw("same");
    c_2->cd();
    c_2->SaveAs("Pip_vs_Pim_Pz_Pulls"+out_add_name);
    
    TLegend *leg_sum = new TLegend(0.6,0.6,0.9,0.9);
    leg_sum->SetFillColor(0);
    leg_sum->AddEntry(gr_sigma_pim_pull_pz,"#pi^{-}-Pulls");
    leg_sum->AddEntry(gr_sigma_pip_pull_pz,"#pi^{+}-Pulls");
    
    TCanvas *c_3 = new TCanvas("c_3","",1200,500);
    c_3->Divide(2);
    c_3->cd(1);
    gr_mean_pim_pull_pz->Draw("APL");
    gr_mean_pip_pull_pz->Draw("PL");
    c_3->cd(2);
    gr_sigma_pim_pull_pz->Draw("APL");
    gr_sigma_pip_pull_pz->Draw("PL");
    leg_sum->Draw("same");
    c_3->cd();
    c_3->SaveAs("pull_comparison"+out_add_name);
    
    TCanvas *c_4 = new TCanvas("c_4","",1600,1600);
    c_4->Divide(2,2);
    c_4->cd(1);
    h_pim_pull_vs_phi[0]->Draw("COLZ");
    t_pim_pull_vs_phi[0]->Draw("same");
    c_4->cd(2);
    h_pim_pull_vs_phi[1]->Draw("COLZ");
    t_pim_pull_vs_phi[1]->Draw("same");
    c_4->cd(3);
    h_pim_pull_vs_phi[2]->Draw("COLZ");
    t_pim_pull_vs_phi[2]->Draw("same");
    c_4->cd(4);
    h_pim_pull_vs_phi[3]->Draw("COLZ");
    t_pim_pull_vs_phi[3]->Draw("same");
    c_4->cd();
    c_4->SaveAs("Pim_Pz_Pull_vs_Phi"+out_add_name);
    
    
}
