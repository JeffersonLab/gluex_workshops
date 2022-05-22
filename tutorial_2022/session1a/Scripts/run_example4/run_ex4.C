#include "../utils.h"
#include "../data_reader.h"
#include "TCanvas.h"

void scale_graph(TGraph *gr, Double_t scale){
    Double_t x,y,y_new;
    Int_t nPoints = gr->GetN();
    //++++++++++++++++++++++++
    for(Int_t i=0;i<nPoints;i++){
        gr->GetPoint(i,x,y);
        y_new = y*scale;
        gr->SetPoint(i,x,y_new);
    }
    //++++++++++++++++++++++++
}

void run_ex4(){
    set_style();
    
    Double_t title_offset = 0.95;
    Double_t title_size = 0.075;
    Double_t label_size = 0.05;
    
    TString file_name = "/lustre19/expphy/volatile/halld/home/dlersch/KinFit_Workshop_2021_Data/3Pi_P4_Fit.root";
    TTree *tree = get_tree(file_name,"3Pi_P4_Fit_Tree");
    
    Int_t n_entries = tree->GetEntries();
    
    const int n_fits = 10;
    TH1F *h_M_pippimgg[n_fits];
    TH1F *h_fit[n_fits];
    TH1F *h_phi[n_fits];
    TH1F *h_bkg[n_fits];
    Double_t prob_cut[n_fits] = {0.0,0.0001,0.001,0.01,0.025,0.05,0.1,0.25,0.5,0.75};
    TString h_names[n_fits] = {"_no_cut","_00001_cut","_0001_cut","_001_cut","_0025_cut","_005_cut","_01_cut","_025_cut","_05_cut","_075_cut"};
    TString h_title[n_fits] = {"No Cut","Probability #geq 0.0001","Probability #geq 0.001","Probability #geq 0.01","Probability #geq 0.025","Probability #geq 0.05","Probability #geq 0.1","Probability #geq 0.25","Probability #geq 0.5","Probability #geq 0.75"};
    Double_t x_min = 0.3;
    Double_t x_max = 1.5;
    Double_t zoom_l = 0.9;
    Double_t zoom_r = 1.3;
    Double_t fit_l = 0.9; //0.92
    Double_t fit_r = 1.3;//1.2
    Double_t fit_sig_l = 0.98;
    Double_t fit_sig_r = 1.05;
    TLegend *fit_leg[n_fits];
    TCanvas *c_fit[n_fits];
    
    Double_t n_sigma_window = 3.0;
    
    //+++++++++++++++++++
    for(Int_t h=0;h<n_fits;h++){
        h_M_pippimgg[h] = new TH1F("h_M_pippimgg" + h_names[h],"",200,x_max,x_max);
        h_M_pippimgg[h]->SetLineWidth(3.0);
        h_M_pippimgg[h]->SetLineColor(kBlack);
        
        set_axis(h_M_pippimgg[h]->GetXaxis(),"M(#pi^{+},#pi^{-},#gamma_{1},#gamma_{2}) [GeV/c^{2}]",title_offset,title_size,label_size);
        set_axis(h_M_pippimgg[h]->GetYaxis(),"Entries",title_offset,title_size,label_size);
        h_M_pippimgg[h]->SetTitle(h_title[h]);
    }
    //+++++++++++++++++++
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    for(Int_t ev=0;ev<n_entries;ev++){
        tree->GetEntry(ev);
        
        //+++++++++++++++++++
        for(Int_t p=0;p<n_fits;p++){
            if(KFit_Prob >= prob_cut[p]){
                h_M_pippimgg[p]->Fill(IM_pippimgg_fit);
            }
        }
        //+++++++++++++++++++
        
    }
    //+++++++++++++++++++++++++++++++++++++++++++++++++
    
    TCanvas *c_init = new TCanvas("c_init","",1);
    h_M_pippimgg[0]->Draw();
    c_init->cd();
    
    TGraph *gr_sb = new TGraph(n_fits);
    gr_sb->SetLineWidth(3.0);
    gr_sb->SetLineColor(kBlack);
    gr_sb->SetMarkerStyle(20);
    set_axis(gr_sb->GetXaxis(),"Probability Cut",title_offset,title_size,label_size);
    set_axis(gr_sb->GetYaxis(),"N_{S}/N_{B}",title_offset,title_size,label_size);
    
    TGraph *gr_sign = new TGraph(n_fits);
    gr_sign->SetLineWidth(3.0);
    gr_sign->SetLineColor(kRed);
    gr_sign->SetMarkerColor(kRed);
    gr_sign->SetMarkerStyle(21);
    
    Double_t N_sig = 0.0;
    Double_t N_bkg = 0.0;
    Double_t S_B_ratio = 0.0;
    Double_t significance = 0.0;
    Double_t max_significance = 0.0;
    //+++++++++++++++++++
    for(Int_t h=0;h<n_fits;h++){
        h_M_pippimgg[h]->GetXaxis()->SetRangeUser(zoom_l,zoom_r);
        
        
        Double_t *current_fit_pars = get_hist_fit_pars(h_M_pippimgg[h],fit_l,fit_r,fit_sig_l,fit_sig_r,"RQN");
        Double_t left_int = current_fit_pars[1] - n_sigma_window*current_fit_pars[2];
        Double_t right_int = current_fit_pars[1] + n_sigma_window*current_fit_pars[2];
        Int_t left_int_bin = h_M_pippimgg[h]->GetXaxis()->FindFixBin(left_int);
        Int_t right_int_bin = h_M_pippimgg[h]->GetXaxis()->FindFixBin(right_int);
        
        TLine *left_Line = new TLine(left_int,0.0,left_int,h_M_pippimgg[h]->GetMaximum());
        left_Line->SetLineWidth(2.0);
        left_Line->SetLineColor(6);
        left_Line->SetLineStyle(2);
        
        TLine *right_Line = new TLine(right_int,0.0,right_int,h_M_pippimgg[h]->GetMaximum());
        right_Line->SetLineWidth(2.0);
        right_Line->SetLineColor(6);
        right_Line->SetLineStyle(2);
        
        h_fit[h] = get_fit_dist(h_M_pippimgg[h],current_fit_pars,fit_l,fit_r,true);
        h_fit[h]->SetLineWidth(3.0);
        h_fit[h]->SetLineColor(kRed);
    
        h_phi[h] = get_fit_dist(h_M_pippimgg[h],current_fit_pars,fit_l,fit_r,false);
        h_phi[h]->SetLineWidth(3.0);
        h_phi[h]->SetLineColor(8);
        
        h_bkg[h] = (TH1F*)h_fit[h]->Clone();
        h_bkg[h]->SetName("Background_" + h_names[h]);
        h_bkg[h]->Add(h_phi[h],-1);
        h_bkg[h]->SetLineWidth(3.0);
        h_bkg[h]->SetLineColor(kBlue);
        
        fit_leg[h] = new TLegend(0.6,0.3,0.9,0.6);
        fit_leg[h]->SetFillColor(0);
        fit_leg[h]->AddEntry(h_M_pippimgg[h],"Data");
        fit_leg[h]->AddEntry(h_fit[h],"Gaus + Pol(3)");
        fit_leg[h]->AddEntry(h_phi[h],"Gaus");
        fit_leg[h]->AddEntry(h_bkg[h],"Pol(3)");
        fit_leg[h]->AddEntry(left_Line,"3#sigma-window","l");
        
        c_fit[h] = new TCanvas("c_fit" + h_names[h],"",1);
        h_M_pippimgg[h]->GetYaxis()->SetRangeUser(0.0,h_M_pippimgg[h]->GetMaximum()*1.01);
        h_M_pippimgg[h]->Draw();
        
        h_fit[h]->Draw("same");
        h_phi[h]->Draw("same");
        h_bkg[h]->Draw("same");
        fit_leg[h]->Draw("same");
        
        left_Line->Draw("same");
        right_Line->Draw("same");
        
        c_fit[h]->cd();
        c_fit[h]->SaveAs("Phi_Fit" + h_names[h] + ".pdf");
        
        
        N_sig = h_phi[h]->Integral(left_int_bin,right_int_bin);
        N_bkg = h_bkg[h]->Integral(left_int_bin,right_int_bin);
        S_B_ratio = N_sig / N_bkg;
        significance = N_sig / TMath::Sqrt(N_sig + N_bkg);
        
        if(significance > max_significance)max_significance = significance;
        
        gr_sb->SetPoint(h,prob_cut[h],S_B_ratio);
        gr_sign->SetPoint(h,prob_cut[h],significance);
    }
    //+++++++++++++++++++
    
    TCanvas *cf = new TCanvas("cf","",1);
    gr_sb->GetXaxis()->SetLimits(-0.01,1.0);
    gr_sb->Draw("APL");
    cf->Update();
    
    Double_t sign_scale = gPad->GetUymax() / (1.1*max_significance);
    scale_graph(gr_sign,sign_scale);
    
    gr_sign->Draw("PL");
    TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
                              gPad->GetUxmax(), gPad->GetUymax(),0,1.1*max_significance,510,"+L");
    axis->SetLabelSize(label_size);
    
    axis->SetLineColor(kRed);
    axis->SetLabelColor(kRed);
    axis->SetTitle("#color[2]{N_{S}/#sqrt{N_{S}+N_{B}}}");
    axis->SetTitleOffset(title_offset);
    axis->SetTitleSize(title_size);
    
    axis->Draw("same");
    cf->cd();
    cf->SaveAs("Prob_Scan_Results.pdf");
    
}
