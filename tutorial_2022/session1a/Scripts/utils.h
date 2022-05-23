#include "TAxis.h"
#include "TStyle.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TLine.h"

//Provided by: Daniel Lersch
//For suggestions or complaints, please write email to: dlersch@jlab.org

//A simple function to set axis properly, without having to type the same four lines over and over again:
void set_axis(TAxis *hist_axis,TString axis_title,Double_t title_offset,Double_t title_size,Double_t label_size){
    hist_axis->SetTitle(axis_title);
    hist_axis->SetTitleOffset(title_offset);
    hist_axis->SetTitleSize(title_size);
    hist_axis->SetLabelSize(label_size);
}

//-------------------------------------------------

//Set the graphics style:
void set_style(){
    gStyle->SetOptStat(0);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
}

//-------------------------------------------------

//Ideal chi^2 distribution:
Double_t ideal_chisq_dist(Double_t *x, Double_t *par){
    Double_t arg = 0.5*par[0];
    Double_t f1 = TMath::Power(2,arg)*TMath::Gamma(arg);
    Double_t f2 = TMath::Power(x[0],arg-1);
    Double_t f3 = TMath::Exp(-0.5*x[0]);
    
    return f2*f3/f1;
}

//-------------------------------------------------

//Get histogram of the ideal chi^2 distribution:
TH1F *get_ideal_chisq_dist(TH1F *measured_dist,Int_t ndf){
    Int_t nbins = measured_dist->GetNbinsX();
    Double_t min_chisq = measured_dist->GetXaxis()->GetXmin();
    Double_t max_chisq = measured_dist->GetXaxis()->GetXmax();
    
    TF1 *chisq_dist = new TF1("chisq_dist",ideal_chisq_dist,min_chisq,max_chisq,1);
    chisq_dist->FixParameter(0,ndf);
    
    
    
    TH1F *chisq_hist = new TH1F("chisq_hist","",nbins,min_chisq,max_chisq);
    //++++++++++++++++++++++++++++++++
    for(Int_t i=0;i<measured_dist->GetEntries();i++){
        Double_t val = chisq_dist->GetRandom();
        chisq_hist->Fill(val);
    }
    //++++++++++++++++++++++++++++++++
    
    chisq_hist->Scale(measured_dist->GetMaximum()*TMath::Power(chisq_hist->GetMaximum(),-1));
    return chisq_hist;
}

//-------------------------------------------------

//Get the ideal probability distribution which is just a simple line:
TLine *get_ideal_prob_dist(TH1F *measured_dist,Double_t ref_val){
    Int_t val_bin = measured_dist->GetXaxis()->FindFixBin(ref_val);
    Double_t ref_height = measured_dist->GetBinContent(val_bin);
    
    TLine *prob_line = new TLine(0.0,ref_height,1.0,ref_height);
    return prob_line;
}


//-------------------------------------------------

//Fit a gaussian function to a pull-distribution and retreive the fit parameters:
Double_t *get_pull_pars(TH1F *current_pull_dist,Double_t x_min,Double_t x_max,TString fit_options){
    Double_t *pull_pars = 0;
    pull_pars = new Double_t[3];
    
    TF1 *pull_fit = new TF1("pull_fit","gaus",x_min,x_max);
    pull_fit->SetParameter(1,0.0);//--> We assume the pull is centered at zero
    pull_fit->SetParameter(2,1.0);//--> We assume the pull has a sigma ~1.0
    
    current_pull_dist->Fit("pull_fit",fit_options);
    pull_pars[0] = pull_fit->GetParameter(0);
    pull_pars[1] = pull_fit->GetParameter(1);
    pull_pars[2] = pull_fit->GetParameter(2);
    pull_fit->Clear();
    
    return pull_pars;
}

//-------------------------------------------------

//Translate fit parameters from the function above to a histogram:
TH1F *get_fitted_pull_dist(TH1F *current_pull_dist,Double_t *pull_fit_pars){
    Int_t n_bins = current_pull_dist->GetNbinsX();
    Double_t x_min = current_pull_dist->GetXaxis()->GetXmin();
    Double_t x_max = current_pull_dist->GetXaxis()->GetXmax();
    
    TF1 *pull_fit = new TF1("pull_fit","gaus",x_min,x_max);
    pull_fit->FixParameter(0,pull_fit_pars[0]);
    pull_fit->FixParameter(1,pull_fit_pars[1]);
    pull_fit->FixParameter(2,pull_fit_pars[2]);
    
    pull_fit->SetNpx(n_bins);
    TH1F *pull_fit_hist = (TH1F*)pull_fit->GetHistogram();
    pull_fit->Clear();
    
    return pull_fit_hist;
}

//-------------------------------------------------

//A bit more elaborate: Fit a gaussian function + pol(3) to a histogram
//To make things easier, a pre-fit for the signal region is used. All fit parameters summarized and returned within an array:
Double_t *get_hist_fit_pars(TH1F *invariant_mass_hist,Double_t x_min,Double_t x_max,Double_t pre_fit_min,Double_t pre_fit_right,TString fit_options){
    Double_t *hist_fit_pars = 0;
    hist_fit_pars = new Double_t[7];
    
    //We run a pre-fit here first:
    TF1 *pre_fit = new TF1("pre_fit","gaus",pre_fit_min,pre_fit_right);
    invariant_mass_hist->Fit("pre_fit","RQN");
    
    TF1 *hist_fit = new TF1("hist_fit","gaus(0)+pol3(3)",x_min,x_max);
    hist_fit->SetParameter(0,pre_fit->GetParameter(0));
    hist_fit->FixParameter(1,pre_fit->GetParameter(1));//Roughly the phi-mass...
    hist_fit->SetParameter(2,pre_fit->GetParameter(2));
    hist_fit->SetParameter(3,1.0);
    hist_fit->SetParameter(4,1.0);
    hist_fit->SetParameter(5,1.0);
    hist_fit->SetParameter(6,1.0);
    
    invariant_mass_hist->Fit("hist_fit",fit_options);
    hist_fit_pars[0] = hist_fit->GetParameter(0);
    hist_fit_pars[1] = hist_fit->GetParameter(1);
    hist_fit_pars[2] = hist_fit->GetParameter(2);
    hist_fit_pars[3] = hist_fit->GetParameter(3);
    hist_fit_pars[4] = hist_fit->GetParameter(4);
    hist_fit_pars[5] = hist_fit->GetParameter(5);
    hist_fit_pars[6] = hist_fit->GetParameter(6);
    
    hist_fit->Clear();
    
    return hist_fit_pars;
}

//-------------------------------------------------

//Get histogram from the elaborate fit:
TH1F *get_fit_dist(TH1F *invariant_mass_hist,Double_t *hist_fit_pars,Double_t x_min_fit,Double_t x_max_fit,Bool_t show_bkg){
    Int_t n_bins = invariant_mass_hist->GetNbinsX();
    Double_t x_min = invariant_mass_hist->GetXaxis()->GetXmin();
    Double_t x_max = invariant_mass_hist->GetXaxis()->GetXmax();
    
    TF1 *hist_fit;
    //--------------------------------------------------------------
    if(show_bkg){
        hist_fit = new TF1("hist_fit","gaus(0)+pol3(3)",x_min,x_max);
        hist_fit->FixParameter(0,hist_fit_pars[0]);
        hist_fit->FixParameter(1,hist_fit_pars[1]);
        hist_fit->FixParameter(2,hist_fit_pars[2]);
        hist_fit->FixParameter(3,hist_fit_pars[3]);
        hist_fit->FixParameter(4,hist_fit_pars[4]);
        hist_fit->FixParameter(5,hist_fit_pars[5]);
        hist_fit->FixParameter(6,hist_fit_pars[6]);
    }else{
        hist_fit = new TF1("hist_fit","gaus",x_min,x_max);
        hist_fit->FixParameter(0,hist_fit_pars[0]);
        hist_fit->FixParameter(1,hist_fit_pars[1]);
        hist_fit->FixParameter(2,hist_fit_pars[2]);
    }
    //--------------------------------------------------------------
    
    hist_fit->SetNpx(n_bins);
    TH1F *fit_dist = (TH1F*)hist_fit->GetHistogram();
    hist_fit->Clear();
    
    //Do some cosmetics here, because we do not want / need to see the entire distribution because it does
    //not make any sense outside a certain region:
    //+++++++++++++++++++++++++++++++
    for(Int_t b=1;b<=n_bins;b++){
        Double_t bin_value = fit_dist->GetXaxis()->GetBinCenter(b);
        if(bin_value < x_min_fit || bin_value > x_max_fit)fit_dist->SetBinContent(b,0.0);
    }
    //+++++++++++++++++++++++++++++++
    
    return fit_dist;
}

//-------------------------------------------------


