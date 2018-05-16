#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "TLatex.h"
#include "TPaveStats.h"
#include "TGraphPainter.h"
#include "TString.h"
#include "TCollection.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2I.h"
#include "TF1.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMinuit.h"
#include "TKey.h"
#include "TCut.h"
#include "TDatime.h"
#include "TAxis.h"
#include "TLine.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "TString.h"
#include "TLorentzVector.h"

// TString config_file = "/home/fs11/jzarling/ChargedAsymSyst/deltapp/NominalSettings.config";

Int_t NBINS_INVMASS = 1000;

Double_t RF_PERIOD = 4.008;
Int_t NBEAM_BUNCHES_HISTOGRAMMING = 4; //Number of out-of-time bunches contained in input file

//Cuts
Double_t PIPL_PIM_M_CUT_LO_HISTOGRAMMING = 0.; //Wide enough to accept all
Double_t PIPL_PIM_M_CUT_HI_HISTOGRAMMING = 1000.; //Wide enough to accept all

//Fitting inputs
Int_t POLY_ORDER = 4;
Int_t POLY_BKG = 1; //Old fashioned polynomials recentered at threshold
Int_t BERN_BKG = 0; //Use bernstein polynomials
Double_t FIT_RANGE_LO = 1.1; //Range to fit in
Double_t FIT_RANGE_HI = 3.5; //Range to fit in
Double_t FIT_RANGE_EXCLUDE_LO = 1.5; //If you want to exclude around a certain region (e.g. Delta*)
Double_t FIT_RANGE_EXCLUDE_HI = 2.2; //If you want to exclude around a certain region (e.g. Delta*)
Double_t YIELD_MASS_CUTOFF = 1.6; //BW has really, really long right tail. Report yields integrated from threshold to this value.

Int_t MakeDeltappHists(TString str_infile, TString str_outfile) {
	TFile* f_in = new TFile(str_infile);
	TTree* t_in = (TTree*)f_in->FindObjectAny("pipl_pim_Tree");
	
	Double_t PiPlus_px;  t_in->SetBranchAddress("PiPlus_px",&PiPlus_px);
	Double_t PiPlus_py;  t_in->SetBranchAddress("PiPlus_py",&PiPlus_py);
	Double_t PiPlus_pz;  t_in->SetBranchAddress("PiPlus_pz",&PiPlus_pz);
	Double_t PiPlus_E;   t_in->SetBranchAddress("PiPlus_E",&PiPlus_E);
	Double_t PiMinus_px; t_in->SetBranchAddress("PiMinus_px",&PiMinus_px);
	Double_t PiMinus_py; t_in->SetBranchAddress("PiMinus_py",&PiMinus_py);
	Double_t PiMinus_pz; t_in->SetBranchAddress("PiMinus_pz",&PiMinus_pz);
	Double_t PiMinus_E;  t_in->SetBranchAddress("PiMinus_E",&PiMinus_E);
	Double_t Proton_px;  t_in->SetBranchAddress("Proton_px",&Proton_px);
	Double_t Proton_py;  t_in->SetBranchAddress("Proton_py",&Proton_py);
	Double_t Proton_pz;  t_in->SetBranchAddress("Proton_pz",&Proton_pz);
	Double_t Proton_E;   t_in->SetBranchAddress("Proton_E",&Proton_E);
	Double_t pipl_pim_m; t_in->SetBranchAddress("pipl_pim_m",&pipl_pim_m);
	Int_t is_perp;       t_in->SetBranchAddress("is_perp",&is_perp);
	Int_t is_para;       t_in->SetBranchAddress("is_para",&is_para);
	Int_t is_45;         t_in->SetBranchAddress("is_45",&is_45);
	Int_t is_135;        t_in->SetBranchAddress("is_135",&is_135);
	Int_t is_amo;        t_in->SetBranchAddress("is_amo",&is_amo);
	Double_t rf_deltaT;  t_in->SetBranchAddress("rf_deltaT",&rf_deltaT);
	Double_t beamE;      t_in->SetBranchAddress("beamE",&beamE);
	Double_t kinfit_CL;  t_in->SetBranchAddress("kinfit_CL",&kinfit_CL);
	Double_t MM2;        t_in->SetBranchAddress("MM2",&MM2);
	Int_t Run;           t_in->SetBranchAddress("Run",&Run);
	Int_t Event;         t_in->SetBranchAddress("Event",&Event);
	
	TH1F* h_massdist_intime = new TH1F("h_massdist_intime","Pi^{+} Proton Mass Spectrum",NBINS_INVMASS,1.,4.5);
	TH1F* h_massdist_outoftime = new TH1F("h_massdist_outoftime","Pi^{+} Proton Mass Spectrum",NBINS_INVMASS,1.,4.5);
	TH1F* h_massdist_accidsub = new TH1F("h_massdist_accidsub","Pi^{+} Proton Mass Spectrum",NBINS_INVMASS,1.,4.5);

    TH2F* h_proton_p_theta_intime = new TH2F("h_proton_p_theta_intime","Proton Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_proton_p_theta_outoftime = new TH2F("h_proton_p_theta_outoftime","Proton Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_proton_p_theta_accidsub = new TH2F("h_proton_p_theta_accidsub","Proton Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);

    TH2F* h_pip_p_theta_intime = new TH2F("h_pip_p_theta_intime","Pi^{+} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_pip_p_theta_outoftime = new TH2F("h_pip_p_theta_outoftime","Pi^{+} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_pip_p_theta_accidsub = new TH2F("h_pip_p_theta_accidsub","Pi^{+} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
	
    TH2F* h_pim_p_theta_intime = new TH2F("h_pim_p_theta_intime","Pi^{-} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_pim_p_theta_outoftime = new TH2F("h_pim_p_theta_outoftime","Pi^{-} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);
    TH2F* h_pim_p_theta_accidsub = new TH2F("h_pim_p_theta_accidsub","Pi^{-} Momentum v. Theta; #theta (degress); p (GeV/c)", 70, 0, 140, 50, 0, 10);

	
	Long64_t nentries = t_in->GetEntries();
	cout << "Number of entries to get through: " << nentries << endl;
	for(size_t i =0; i<nentries; ++i) {
		t_in->GetEntry(i);
		if(i%100000==0) cout << "i: " << i << endl;
	
		TLorentzVector p4_PiPlus = TLorentzVector(PiPlus_px,PiPlus_py,PiPlus_pz,PiPlus_E);
		TLorentzVector p4_PiMinus = TLorentzVector(PiMinus_px,PiMinus_py,PiMinus_pz,PiMinus_E);
		TLorentzVector p4_Proton = TLorentzVector(Proton_px,Proton_py,Proton_pz,Proton_E);
	
		Double_t pipl_pim_mass = (p4_PiPlus+p4_PiMinus).M();
		Double_t pipl_proton_mass = (p4_PiPlus+p4_Proton).M();
	
		//All cuts applied here
		if(pipl_pim_mass<PIPL_PIM_M_CUT_LO_HISTOGRAMMING) continue;
		if(pipl_pim_mass>PIPL_PIM_M_CUT_HI_HISTOGRAMMING) continue;
	
		if(fabs(rf_deltaT)<RF_PERIOD*0.5) {
			h_massdist_intime->Fill(pipl_proton_mass);
            if(pipl_proton_mass < 1.3) {
                h_proton_p_theta_intime->Fill(p4_Proton.Vect().Theta()*180./TMath::Pi(), p4_Proton.Vect().Mag());
                h_pip_p_theta_intime->Fill(p4_PiPlus.Vect().Theta()*180./TMath::Pi(), p4_PiPlus.Vect().Mag());
                h_pim_p_theta_intime->Fill(p4_PiMinus.Vect().Theta()*180./TMath::Pi(), p4_PiMinus.Vect().Mag());
            }
		}
		if(fabs(rf_deltaT)>RF_PERIOD*0.5) {
			h_massdist_outoftime->Fill(pipl_proton_mass);
            if(pipl_proton_mass < 1.3) {
                h_proton_p_theta_outoftime->Fill(p4_Proton.Vect().Theta()*180./TMath::Pi(), p4_Proton.Vect().Mag());
                h_pip_p_theta_outoftime->Fill(p4_PiPlus.Vect().Theta()*180./TMath::Pi(), p4_PiPlus.Vect().Mag());
                h_pim_p_theta_outoftime->Fill(p4_PiMinus.Vect().Theta()*180./TMath::Pi(), p4_PiMinus.Vect().Mag());
            }
		}
	
	}
	
	//Accidental subtraction
	h_massdist_intime->Sumw2();
	h_massdist_outoftime->Sumw2();
	Double_t Delta_t_scalefactor = 1. / (2*NBEAM_BUNCHES_HISTOGRAMMING);
	h_massdist_accidsub->Add(h_massdist_intime,h_massdist_outoftime,1,-1.*Delta_t_scalefactor);	
    h_massdist_accidsub->Rebin(5);

	h_massdist_accidsub->GetXaxis()->SetTitle("#pi^{+} proton inv. mass (GeV)");
	h_massdist_accidsub->GetYaxis()->SetTitle("Counts");
	
    h_proton_p_theta_intime->Sumw2();
    h_proton_p_theta_outoftime->Sumw2();
    h_proton_p_theta_accidsub->Add(h_proton_p_theta_intime,h_proton_p_theta_outoftime,1,-1.*Delta_t_scalefactor);
    h_pip_p_theta_intime->Sumw2();
    h_pip_p_theta_outoftime->Sumw2();
    h_pip_p_theta_accidsub->Add(h_pip_p_theta_intime,h_pip_p_theta_outoftime,1,-1.*Delta_t_scalefactor);
    h_pim_p_theta_intime->Sumw2();
    h_pim_p_theta_outoftime->Sumw2();
    h_pim_p_theta_accidsub->Add(h_pim_p_theta_intime,h_pim_p_theta_outoftime,1,-1.*Delta_t_scalefactor);

	TFile* outfile = new TFile( str_outfile, "RECREATE" );
	outfile->cd();
	h_massdist_intime->Write();
	h_massdist_outoftime->Write();
	h_massdist_accidsub->Write();
    h_proton_p_theta_intime->Write();
    h_proton_p_theta_outoftime->Write();
    h_proton_p_theta_accidsub->Write();
    h_pip_p_theta_intime->Write();
    h_pip_p_theta_outoftime->Write();
    h_pip_p_theta_accidsub->Write();
    h_pim_p_theta_intime->Write();
    h_pim_p_theta_outoftime->Write();
    h_pim_p_theta_accidsub->Write();
	outfile->Close();
	
	cout << "Done with histogram generation" << endl;
	
	return 0;
}

Double_t jz_Lambda(Double_t x, Double_t y, Double_t z) {
	return x*x+y*y+z*z-2*x*y-2*x*z-2*y*z;
}

//Rescaled such that polynomials range from a to b
//Parms max index 12 
Double_t jz_BernsteinPoly(Double_t* unnorm_x, Double_t* parms) {
	Int_t degree = TMath::Floor(parms[0]);
	Double_t a = parms[1];
	Double_t b = parms[2];

	
	if(degree<0 || degree > 9) {
		cout << "ERROR! Unexpected bernstein polynomial order encountered!" << endl;
		cout << "Degree: " << degree << endl;
		return -1;
	}
	
	Double_t x = (unnorm_x[0]-a)/(b-a);
	Double_t s = (1-x);
	Double_t result = 0;
	
	//Recursive definition
	//Refer to http://mathworld.wolfram.com/BernsteinPolynomial.html
	//and https://root.cern.ch/doc/master/RooBernstein_8cxx_source.html 
	for(Int_t i =0; i <= degree; i++) result += parms[i+3]*TMath::Binomial(degree,i)*TMath::Power(x,i)*TMath::Power(s,degree-i);

	return result;
	
}


Double_t jz_BkgOnly(Double_t* x, Double_t* parms) {

	//Naming for polynomial only, ignore for bernstein
	Double_t poly0 = parms[0];
	Double_t poly1 = parms[1];
	Double_t poly2 = parms[2];
	Double_t poly3 = parms[3];
	Double_t poly4 = parms[4];
	Double_t poly5 = parms[5];
	Double_t poly6 = parms[6];
	Double_t poly7 = parms[7];
	Double_t poly8 = parms[8];
	Double_t poly9 = parms[9];
	Double_t poly10 = parms[10];
	Double_t thresh = parms[11];
	
	Double_t bkg_poly = 0;
	if(POLY_BKG==1) return poly0+poly1*TMath::Power(x[0]-thresh,1)+poly2*TMath::Power(x[0]-thresh,2)+poly3*TMath::Power(x[0]-thresh,3)+poly4*TMath::Power(x[0]-thresh,4)+poly5*TMath::Power(x[0]-thresh,5)+poly6*TMath::Power(x[0]-thresh,6)+poly7*TMath::Power(x[0]-thresh,7)+poly8*TMath::Power(x[0]-thresh,8)+poly9*TMath::Power(x[0]-thresh,9)+poly10*TMath::Power(x[0]-thresh,10);
	if(BERN_BKG==1) return jz_BernsteinPoly(x,parms);

   // return poly0+poly1*TMath::Power(x[0]-thresh,1)+poly2*TMath::Power(x[0]-thresh,2)+poly3*TMath::Power(x[0]-thresh,3)+poly4*TMath::Power(x[0]-thresh,4)+poly5*TMath::Power(x[0]-thresh,5)+poly6*TMath::Power(x[0]-thresh,6)+poly7*TMath::Power(x[0]-thresh,7)+poly8*TMath::Power(x[0]-thresh,8)+poly9*TMath::Power(x[0]-thresh,9)+poly10*TMath::Power(x[0]-thresh,10); 

    return 0;
}


Double_t jz_DeltaBW_JPAC(Double_t* x, Double_t* parms) {
	
	Double_t M_BW  = parms[1];
	Double_t Gamma = parms[2];
	Double_t R     = parms[3];
	
	
	Double_t mass_proton = 0.93827;
	Double_t mass_pion = 0.13957;
	Double_t m2_proton = TMath::Power(mass_proton,2);
	Double_t m2_pion = TMath::Power(mass_pion,2);
	Double_t m2_x = TMath::Power(x[0],2);
	
	//k0 numerator squared equivalent to Kaellen function with inputs: proton mass^2, pion mass^2, and m(pi+ p)^2
	Double_t k0   = TMath::Sqrt( jz_Lambda(m2_proton,m2_pion,M_BW*M_BW)) / (2 * M_BW) ;
	Double_t my_k   = TMath::Sqrt( jz_Lambda(m2_proton,m2_pion,x[0]*x[0])) / (2 * x[0]) ;
	
	Double_t my_gamma = Gamma*(M_BW/x[0])* TMath::Power( (my_k/k0) ,3) * ((1+(k0*R)*(k0*R)) / (1+(my_k*R)*(my_k*R)));
 
	Double_t h_p = my_k*R/TMath::Sqrt( 1+my_k*my_k*R*R);
 
	// Double_t BW_value = (1/(my_k*my_k)) * ((M_BW*my_gamma)*(M_BW*my_gamma) / ( TMath::Power((M_BW*M_BW-x*x),2) + (M_BW*my_gamma)*(M_BW*my_gamma)))  ;
	Double_t BW2_value = (h_p*h_p) / ( TMath::Power((M_BW*M_BW-x[0]*x[0]),2) + (M_BW*my_gamma)*(M_BW*my_gamma))  ; // Abs squared of Breit-Wigner
	// Double_t BW2_value = (h_p*h_p) / ( TMath::Power((M_BW*M_BW-x[0]*x[0]),2) + (x[0]*my_gamma)*(x[0]*my_gamma))  ; // Abs squared of Breit-Wigner
	// Double_t BW_value = ((M_BW*my_gamma)*(M_BW*my_gamma) / ( TMath::Power((M_BW*M_BW-x*x),2) + (M_BW*my_gamma)*(M_BW*my_gamma)))  ;
 
	Double_t dsigma_dtdm = 2*my_k*BW2_value;
 
 
   return parms[0]*dsigma_dtdm; 
 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   // return parms[0]*BW_value; 
}

Double_t jz_DeltaBW_PlusBkg(Double_t* x, Double_t* parms) {
	
	Double_t M_BW  = parms[1];
	Double_t Gamma = parms[2];
	Double_t R     = parms[3];
	Double_t poly0 = parms[4];
	Double_t poly1 = parms[5];
	Double_t poly2 = parms[6];
	Double_t poly3 = parms[7];
	Double_t poly4 = parms[8];
	Double_t poly5 = parms[9];
	Double_t poly6 = parms[10];
	Double_t poly7 = parms[11];
	Double_t poly8 = parms[12];
	Double_t poly9 = parms[13];
	Double_t poly10= parms[14];
	Double_t thresh= parms[15]; //Centers poly or bernstein bkg functions
	Double_t bern_max     = parms[16]; //Maximum for bernstein function
	
	Double_t mass_proton = 0.93827;
	Double_t mass_pion = 0.13957;
	Double_t m2_proton = TMath::Power(mass_proton,2);
	Double_t m2_pion = TMath::Power(mass_pion,2);
	Double_t m2_x = TMath::Power(x[0],2);
	
	//k0 numerator squared equivalent to Kaellen function with inputs: proton mass^2, pion mass^2, and m(pi+ p)^2
	Double_t k0   = TMath::Sqrt( jz_Lambda(m2_proton,m2_pion,M_BW*M_BW)) / (2 * M_BW) ;
	Double_t my_k   = TMath::Sqrt( jz_Lambda(m2_proton,m2_pion,x[0]*x[0])) / (2 * x[0]) ;
	
	
	Double_t my_gamma = Gamma*(M_BW/x[0])* TMath::Power( (my_k/k0) ,3) * ((1+(k0*R)*(k0*R)) / (1+(my_k*R)*(my_k*R)));
 
	Double_t h_p = my_k*R/TMath::Sqrt( 1+my_k*my_k*R*R);
 
	Double_t BW2_value = (h_p*h_p) / ( TMath::Power((M_BW*M_BW-x[0]*x[0]),2) + (M_BW*my_gamma)*(M_BW*my_gamma))  ; // Abs squared of Breit-Wigner
 
	Double_t dsigma_dtdm = 2*my_k*BW2_value;
	Double_t bkg_poly = 0;
	if(POLY_BKG==1) bkg_poly = poly0+poly1*TMath::Power(x[0]-thresh,1)+poly2*TMath::Power(x[0]-thresh,2)+poly3*TMath::Power(x[0]-thresh,3)+poly4*TMath::Power(x[0]-thresh,4)+poly5*TMath::Power(x[0]-thresh,5)+poly6*TMath::Power(x[0]-thresh,6)+poly7*TMath::Power(x[0]-thresh,7)+poly8*TMath::Power(x[0]-thresh,8)+poly9*TMath::Power(x[0]-thresh,9)+poly10*TMath::Power(x[0]-thresh,10);
	if(BERN_BKG==1) {
		Double_t bern_parms[13];
		bern_parms[0]=POLY_ORDER+0.0001; //Nudge factor; I take floor a little later
		bern_parms[1]=thresh;
		bern_parms[2]=bern_max;
		bern_parms[3]=poly0;
		bern_parms[4]=poly1;
		bern_parms[5]=poly2;
		bern_parms[6]=poly3;
		bern_parms[7]=poly4;
		bern_parms[8]=poly5;
		bern_parms[9]=poly6;
		bern_parms[10]=poly7;
		bern_parms[11]=poly8;
		bern_parms[12]=poly9;
		bkg_poly = jz_BernsteinPoly(x,bern_parms);
	}
 
	return parms[0]*dsigma_dtdm+bkg_poly; 
}


vector<Double_t> GetBWYieldPlusPoly(TH1F* hist,TString fname="",Int_t POLY_ORDER=3) {
	
	Double_t THRESHOLD_FITTING = 1.07784;
	
	Double_t bin_width = hist->GetBinWidth(1);
	TCanvas* c1 = new TCanvas ("c1","c1",1200*1,900*1);
	
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);
	
	TF1* BW_fit = new TF1("BW_fit",jz_DeltaBW_PlusBkg,FIT_RANGE_LO,FIT_RANGE_HI,17);
	BW_fit->SetParameters(1.,1.232,0.117,5.84,0,0,0,0,0);	
	
	BW_fit->SetParLimits(0,0.,100000); ////No negative yields!
	BW_fit->FixParameter(1,1.232); //Mas
	BW_fit->FixParameter(2,0.117); //Width
	BW_fit->FixParameter(3,5.84); //Interaction Radius
	//Threshold setup
	BW_fit->FixParameter(15,1.07784);
	
	//Bernstein polynomial constants are positive (set here)
	if(BERN_BKG==1) {
		// BW_fit->SetParLimits(4,0,10000.); //pol0
		BW_fit->FixParameter(4,0); //pol0 is 1 at range minimum. Undesirable.
		BW_fit->SetParLimits(5,0,10000.); //pol1
		BW_fit->SetParLimits(6,0,10000.); //pol2
		BW_fit->SetParLimits(7,0,10000.); //pol3
		BW_fit->SetParLimits(8,0,10000.); //pol4
		BW_fit->SetParLimits(9,0,10000.); //pol5
		BW_fit->SetParLimits(10,0,10000.); //pol6
		BW_fit->SetParLimits(11,0,10000.); //pol7
		BW_fit->SetParLimits(12,0,10000.); //pol8
		BW_fit->SetParLimits(13,0,10000.); //pol9
		BW_fit->FixParameter(14,0.); //pol10, not used
		BW_fit->SetParameter(16,4.); //Bernstein function range max
		BW_fit->SetParLimits(16,3.6,4.5); //Bernstein function maximum range parameter
	}
	
	if(POLY_BKG==1) {
		BW_fit->FixParameter(4,0); //Offset would be highly unphysical!!
	}
	
	if(POLY_ORDER<1)  BW_fit->FixParameter(5,0); //Pol1
	if(POLY_ORDER<2)  BW_fit->FixParameter(6,0); //Pol2
	if(POLY_ORDER<3)  BW_fit->FixParameter(7,0); //Pol3
	if(POLY_ORDER<4)  BW_fit->FixParameter(8,0); //Pol4
	if(POLY_ORDER<5)  BW_fit->FixParameter(9,0); //Pol5
	if(POLY_ORDER<6)  BW_fit->FixParameter(10,0); //Pol6
	if(POLY_ORDER<7)  BW_fit->FixParameter(11,0); //Pol7
	if(POLY_ORDER<8)  BW_fit->FixParameter(12,0); //Pol8
	if(POLY_ORDER<9)  BW_fit->FixParameter(13,0); //Pol9
	if(POLY_ORDER<10) BW_fit->FixParameter(14,0); //Pol10
	
	
	BW_fit->SetNpx(1000);
	
	//Fit and grab values
	hist->Fit(BW_fit,"Q","",FIT_RANGE_LO,FIT_RANGE_HI); //Option WL: weighted likelihood fit
	// hist->Fit(BW_fit,FIT_OPTION_STRING.c_str(),"",FIT_RANGE_LO,FIT_RANGE_HI); //Option WL: weighted likelihood fit
	Double_t parms[17]; 
	BW_fit->GetParameters(parms);
	const Double_t* parm_errs = BW_fit->GetParErrors();
	
	TF1* my_bkg= new TF1("my_bkg",jz_BkgOnly,1.07784,FIT_RANGE_HI,13);
	if(POLY_BKG==1) {
		my_bkg->SetParameters(parms[4],parms[5],parms[6],parms[7],parms[8],parms[9],parms[10],parms[11],parms[12],parms[13],parms[14]);	
		my_bkg->SetParameter(11,parms[15]);
	}
	if(BERN_BKG==1) {
		Double_t bern_parms[13];
		bern_parms[0]=POLY_ORDER+0.0001; //Nudge factor; I take floor a little later
		bern_parms[1]=parms[15]; // Threshold
		bern_parms[2]=parms[16]; //Max
		bern_parms[3]=parms[4]; //c0
		bern_parms[4]=parms[5]; //c1
		bern_parms[5]=parms[6]; //c2
		bern_parms[6]=parms[7]; //c3
		bern_parms[7]=parms[8]; //c4
		bern_parms[8]=parms[9]; //c5
		bern_parms[9]=parms[10]; //c6
		bern_parms[10]=parms[11]; //c7
		bern_parms[11]=parms[12]; //c8
		bern_parms[12]=parms[13]; //c9
		// bkg_poly = jz_BernsteinPoly(x,bern_parms);
		my_bkg->SetParameters(bern_parms);
		}
	my_bkg->SetLineColor(kBlue);
	my_bkg->SetLineStyle(kDashed);
	Double_t near_thresh_bkgyield = my_bkg->Integral(THRESHOLD_FITTING,1.112);
	my_bkg->SetNpx(1000);
		
	TF1* BW4Norm = new TF1("BW4Norm",jz_DeltaBW_JPAC,THRESHOLD_FITTING,1e10,4);
	BW4Norm->SetParameters(1.,BW_fit->GetParameter(1),BW_fit->GetParameter(2),BW_fit->GetParameter(3));
	Double_t BWnormalization       = (BW4Norm->Integral(THRESHOLD_FITTING,10000.) )/bin_width;
	Double_t BWfrac_in_range       = BW4Norm->Integral(THRESHOLD_FITTING,YIELD_MASS_CUTOFF) / BW4Norm->Integral(THRESHOLD_FITTING,10000.);
	Double_t BW_yield_in_range     = BWnormalization*BWfrac_in_range*parms[0];
	Double_t BW_yield_in_range_err = BWnormalization*BWfrac_in_range*parm_errs[0];
	
	
	Double_t raw_yield_wide=0; //Can't use GetEntries(), something about non-unit weights
	Double_t raw_yield_range=0;
	for(int i =0; i<hist->GetNbinsX(); ++i) {
		raw_yield_wide+=hist->GetBinContent(i+1);	
		if(hist->GetBinCenter(i+1)<YIELD_MASS_CUTOFF ) raw_yield_range+=hist->GetBinContent(i+1);	
	}
	
	TF1* my_BW = new TF1("my_bkg",jz_DeltaBW_JPAC,THRESHOLD_FITTING,FIT_RANGE_HI,4);
	my_BW->SetParameters(parms[0],parms[1],parms[2],parms[3]);	
	my_BW->SetLineColor(kMagenta);
	my_BW->SetLineStyle(kDashed);
	my_BW->SetNpx(1000);
	// my_BW->SetFillStyle(3008); // If shading of integral desired
	// my_BW->SetFillColor(kMagenta);
	
	Double_t bkg_yield = my_bkg->Integral(THRESHOLD_FITTING,YIELD_MASS_CUTOFF)/bin_width;
	Double_t bkg_yield_wide = my_bkg->Integral(THRESHOLD_FITTING,FIT_RANGE_HI)/bin_width;
	
	cout << "Histogram tag: " << fname << endl;
	cout << "Raw counts in range: " << raw_yield_range << endl;
	cout << "Delta yield in range: " << BW_yield_in_range << endl;
	cout << "Bkg yield in range: " << bkg_yield << endl;
	cout << "WIDE bkg yield: " << bkg_yield_wide << endl;
	cout << "Fractional difference: " << (BW_yield_in_range + bkg_yield - raw_yield_range)/raw_yield_range  << endl;
	cout << endl;
	
	hist->Draw();
	c1->Update();
	double hist_ymin = gPad->GetUymin();
	double hist_ymax = gPad->GetUymax();
	
	
	TLine* excl_line_lo = new TLine(FIT_RANGE_EXCLUDE_LO,hist_ymin,FIT_RANGE_EXCLUDE_LO,hist_ymax);
	excl_line_lo->SetLineColor(kGreen+4);
	excl_line_lo->SetLineStyle(kDashed);
	excl_line_lo->SetLineWidth(3);
	TLine* excl_line_hi = new TLine(FIT_RANGE_EXCLUDE_HI,hist_ymin,FIT_RANGE_EXCLUDE_HI,hist_ymax);
	excl_line_hi->SetLineColor(kGreen+4);
	excl_line_hi->SetLineStyle(kDashed);
	excl_line_hi->SetLineWidth(3);
	
	my_BW->Draw("same");
	my_bkg->Draw("same");
	excl_line_lo->Draw("same");
	excl_line_hi->Draw("same");
	c1->SaveAs(fname+"WideView.png");
	
	hist->GetXaxis()->SetRangeUser(THRESHOLD_FITTING-0.2,FIT_RANGE_EXCLUDE_LO);
	hist->Draw();
	BW_fit->Draw("same");
	my_bkg->Draw("same");
	my_BW->Draw("same");
	excl_line_lo->Draw("same");
	// if(SAVE_ZOOM==1) if(fname!="")  c1->SaveAs(".asymm_moment_plot_dump/"+fname+"ZoomedIn.png");
	
	//Save, maybe
	// if(fname!="")  jz_SaveTObject(hist,fname);
	// cout << "Unnormalized yield: " << parms[0]/bin_width  << endl;
	
	delete c1;
		
	vector<Double_t> return_vec;
	return_vec.push_back(BW_yield_in_range); //0 in vector
	return_vec.push_back(bkg_yield); //1 in vector
	return_vec.push_back(BW_fit->GetChisquare()/BW_fit->GetNDF()); //2 in vector
	return_vec.push_back((BW_yield_in_range + bkg_yield - raw_yield_range)/raw_yield_range); //3 in vector
	return_vec.push_back(bkg_yield_wide); //4 in vector
	return_vec.push_back(raw_yield_range); //5 in vector
	return_vec.push_back(raw_yield_wide); //6 in vector
		
	return return_vec;
}

Int_t FitInvHist(TString str_infile, TString png_out_tag) {

	TFile* f_in = new TFile(str_infile);	
    

	vector<Double_t> fit_results = GetBWYieldPlusPoly((TH1F*)f_in->Get("h_massdist_accidsub"),png_out_tag,POLY_ORDER);	
	cout << "Yield of total histogram: " << fit_results[0] << " +/- " << fit_results[1] << " with chi2/ndf: " << fit_results[2] << endl;

    ofstream outf("data.txt");
    outf <<  fit_results[0] << endl;
    outf.close();


	TCanvas* c1 = new TCanvas ("c1","c1",800,600);
    TH1F *h_proton_p_theta_accidsub = (TH1F*)f_in->Get("h_proton_p_theta_accidsub");
    TH1F *h_pip_p_theta_accidsub = (TH1F*)f_in->Get("h_pip_p_theta_accidsub");
    TH1F *h_pim_p_theta_accidsub = (TH1F*)f_in->Get("h_pim_p_theta_accidsub");

    h_proton_p_theta_accidsub->Rebin2D();
    h_proton_p_theta_accidsub->Draw("COLZ");
    c1->Print("deltapi_proton_p_theta.png");
    h_pip_p_theta_accidsub->Rebin2D();
    h_pip_p_theta_accidsub->Draw("COLZ");
    c1->Print("deltapi_pip_p_theta.png");
    h_pim_p_theta_accidsub->Rebin2D();
    h_pim_p_theta_accidsub->Draw("COLZ");
    c1->Print("deltapi_pim_p_theta.png");
    delete c1;
	
	return 0;
}
