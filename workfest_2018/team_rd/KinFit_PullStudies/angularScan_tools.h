#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"
#include "TLorentzVector.h"

//Get some angles here:
//*******************************************************************************************
Double_t getTheta(TLorentzVector particle, TString option){
    Double_t theta = particle.Theta();
    if(option == "deg"){
        theta *= TMath::RadToDeg();
    }
    return theta;
}

//=============================================

Double_t getTheta(TLorentzVector *particle, TString option){
    Double_t theta = particle->Theta();
    if(option == "deg"){
        theta *= TMath::RadToDeg();
    }
    return theta;
}

//=============================================

Double_t getPhi(TLorentzVector particle, TString option){
    Double_t phi = particle.Phi();
    if(option == "deg"){
        phi *= TMath::RadToDeg();
    }
    return phi;
}

//=============================================

Double_t getPhi(TLorentzVector *particle, TString option){
    Double_t phi = particle->Phi();
    if(option == "deg"){
        phi *= TMath::RadToDeg();
    }
    return phi;
}

//=============================================

void fillAngularHists(TLorentzVector PiPlus, TLorentzVector PiMinus, TLorentzVector Prot,TH2I *Theta_vs_Phi_PiPlus, TH2I *Theta_vs_Phi_PiMinus, TH2I *Theta_vs_Phi_Prot){
    Theta_vs_Phi_PiPlus->Fill(getPhi(PiPlus,"deg"),getTheta(PiPlus,"deg"));
    Theta_vs_Phi_PiMinus->Fill(getPhi(PiMinus,"deg"),getTheta(PiMinus,"deg"));
    Theta_vs_Phi_Prot->Fill(getPhi(Prot,"deg"),getTheta(Prot,"deg"));
}

//=============================================

void fillAngularHists(TLorentzVector *PiPlus, TLorentzVector *PiMinus, TLorentzVector *Prot,TH2I *Theta_vs_Phi_PiPlus, TH2I *Theta_vs_Phi_PiMinus, TH2I*Theta_vs_Phi_Prot){
    Theta_vs_Phi_PiPlus->Fill(getPhi(PiPlus,"deg"),getTheta(PiPlus,"deg"));
    Theta_vs_Phi_PiMinus->Fill(getPhi(PiMinus,"deg"),getTheta(PiMinus,"deg"));
    Theta_vs_Phi_Prot->Fill(getPhi(Prot,"deg"),getTheta(Prot,"deg"));
}
//*******************************************************************************************

//Set some improtant variables:
//*******************************************************************************************
const int NThetaScans = 5;
const int NPhiScans = 5;

//=============================================

Double_t thetaMin_pip,thetaMax_pip,phiMin_pip,phiMax_pip;
void setThetaMin_pip(Double_t yourVal){
    thetaMin_pip = yourVal;
}

//=============================================

void setThetaMax_pip(Double_t yourVal){
    thetaMax_pip = yourVal;
}

//=============================================

void setPhiMin_pip(Double_t yourVal){
    phiMin_pip = yourVal;
}

//=============================================

void setPhiMax_pip(Double_t yourVal){
    phiMax_pip = yourVal;
}

//=============================================

Double_t thetaMin_pim,thetaMax_pim,phiMin_pim,phiMax_pim;
void setThetaMin_pim(Double_t yourVal){
    thetaMin_pim = yourVal;
}

//=============================================

void setThetaMax_pim(Double_t yourVal){
    thetaMax_pim = yourVal;
}

//=============================================

void setPhiMin_pim(Double_t yourVal){
    phiMin_pim = yourVal;
}

//=============================================

void setPhiMax_pim(Double_t yourVal){
    phiMax_pim = yourVal;
}

//=============================================

Double_t thetaMin_p,thetaMax_p,phiMin_p,phiMax_p;
void setThetaMin_p(Double_t yourVal){
    thetaMin_p = yourVal;
}

//=============================================

void setThetaMax_p(Double_t yourVal){
    thetaMax_p = yourVal;
}

//=============================================

void setPhiMin_p(Double_t yourVal){
    phiMin_p = yourVal;
}

//=============================================

void setPhiMax_p(Double_t yourVal){
    phiMax_p = yourVal;
}

//=============================================

TH2I *PiPlus_PullPx_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiPlus_PullPy_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiPlus_PullPz_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiPlus_PullVx_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiPlus_PullVy_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiPlus_PullVz_vs_Prob[NThetaScans][NPhiScans];

TH2I *PiMinus_PullPx_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiMinus_PullPy_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiMinus_PullPz_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiMinus_PullVx_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiMinus_PullVy_vs_Prob[NThetaScans][NPhiScans];
TH2I *PiMinus_PullVz_vs_Prob[NThetaScans][NPhiScans];

TH2I *Prot_PullPx_vs_Prob[NThetaScans][NPhiScans];
TH2I *Prot_PullPy_vs_Prob[NThetaScans][NPhiScans];
TH2I *Prot_PullPz_vs_Prob[NThetaScans][NPhiScans];
TH2I *Prot_PullVx_vs_Prob[NThetaScans][NPhiScans];
TH2I *Prot_PullVy_vs_Prob[NThetaScans][NPhiScans];
TH2I *Prot_PullVz_vs_Prob[NThetaScans][NPhiScans];

//=============================================

void readInHists(TH2I *hist, Int_t ind1,Int_t ind2, TString histType){
    if(histType == "pipPx"){
        PiPlus_PullPx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pipPy"){
        PiPlus_PullPy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pipPz"){
        PiPlus_PullPz_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pipVx"){
        PiPlus_PullVx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pipVy"){
        PiPlus_PullVy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pipVz"){
        PiPlus_PullVz_vs_Prob[ind1][ind2] = hist;
    }
    
    //++++++++++++++++++++++++++++++++++
    
    if(histType == "pimPx"){
        PiMinus_PullPx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pimPy"){
        PiMinus_PullPy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pimPz"){
        PiMinus_PullPz_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pimVx"){
        PiMinus_PullVx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pimVy"){
        PiMinus_PullVy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pimVz"){
        PiMinus_PullVz_vs_Prob[ind1][ind2] = hist;
    }
    
    //++++++++++++++++++++++++++++++++++
    
    if(histType == "pPx"){
        Prot_PullPx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pPy"){
        Prot_PullPy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pPz"){
        Prot_PullPz_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pVx"){
        Prot_PullVx_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pVy"){
        Prot_PullVy_vs_Prob[ind1][ind2] = hist;
    }else if(histType == "pVz"){
        Prot_PullVz_vs_Prob[ind1][ind2] = hist;
    }
}
//*******************************************************************************************


//Fill the histograms:
//*******************************************************************************************
Double_t pullCollection[3][6];
void fillPullHistsPiPlus(Double_t currentTheta, Double_t currentPhi, Double_t prob){
    Double_t thetaStep = (thetaMax_pip - thetaMin_pip)/NThetaScans;
    Double_t phiStep = (phiMax_pip - phiMin_pip)/NPhiScans;
    
    Bool_t isInPhiBin = false;
    Bool_t isInThetaBin = false;
    
    //--------------------------------------------
    for(Int_t i=0;i<NThetaScans;i++){
        //--------------------------------------------
        for(Int_t j=0;j<NPhiScans;j++){
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(currentTheta >= thetaMin_pip + thetaStep*i &&  currentTheta < thetaMin_pip + thetaStep*(i+1)){
                isInThetaBin = true;
            }else isInThetaBin = false;
            
            if(currentPhi >= phiMin_pip + phiStep*j &&  currentPhi < phiMin_pip + phiStep*(j+1)){
                isInPhiBin = true;
            }else isInPhiBin = false;
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(isInThetaBin && isInPhiBin){
                PiPlus_PullPx_vs_Prob[i][j]->Fill(prob,pullCollection[0][0]);
                PiPlus_PullPy_vs_Prob[i][j]->Fill(prob,pullCollection[0][1]);
                PiPlus_PullPz_vs_Prob[i][j]->Fill(prob,pullCollection[0][2]);
                PiPlus_PullVx_vs_Prob[i][j]->Fill(prob,pullCollection[0][3]);
                PiPlus_PullVy_vs_Prob[i][j]->Fill(prob,pullCollection[0][4]);
                PiPlus_PullVz_vs_Prob[i][j]->Fill(prob,pullCollection[0][5]);
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
        }
        //--------------------------------------------
    }
    //--------------------------------------------
}

//=============================================

void fillPullHistsPiMinus(Double_t currentTheta, Double_t currentPhi, Double_t prob){
    Double_t thetaStep = (thetaMax_pim - thetaMin_pim)/NThetaScans;
    Double_t phiStep = (phiMax_pim - phiMin_pim)/NPhiScans;
    
    Bool_t isInPhiBin = false;
    Bool_t isInThetaBin = false;
    
    //--------------------------------------------
    for(Int_t i=0;i<NThetaScans;i++){
        //--------------------------------------------
        for(Int_t j=0;j<NPhiScans;j++){
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(currentTheta >= thetaMin_pim  + thetaStep*i &&  currentTheta < thetaMin_pim  + thetaStep*(i+1)){
                isInThetaBin = true;
            }else isInThetaBin = false;
            
            if(currentPhi >= phiMin_pim  + phiStep*j &&  currentPhi < phiMin_pim  + phiStep*(j+1)){
                isInPhiBin = true;
            }else isInPhiBin = false;
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(isInThetaBin && isInPhiBin){
                PiMinus_PullPx_vs_Prob[i][j]->Fill(prob,pullCollection[1][0]);
                PiMinus_PullPy_vs_Prob[i][j]->Fill(prob,pullCollection[1][1]);
                PiMinus_PullPz_vs_Prob[i][j]->Fill(prob,pullCollection[1][2]);
                PiMinus_PullVx_vs_Prob[i][j]->Fill(prob,pullCollection[1][3]);
                PiMinus_PullVy_vs_Prob[i][j]->Fill(prob,pullCollection[1][4]);
                PiMinus_PullVz_vs_Prob[i][j]->Fill(prob,pullCollection[1][5]);
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
        }
        //--------------------------------------------
    }
    //--------------------------------------------
}

//=============================================

void fillPullHistsProt(Double_t currentTheta, Double_t currentPhi, Double_t prob){
    Double_t thetaStep = (thetaMax_p - thetaMin_p)/NThetaScans;
    Double_t phiStep = (phiMax_p - phiMin_p)/NPhiScans;
    
    Bool_t isInPhiBin = false;
    Bool_t isInThetaBin = false;
    
    //--------------------------------------------
    for(Int_t i=0;i<NThetaScans;i++){
        //--------------------------------------------
        for(Int_t j=0;j<NPhiScans;j++){
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(currentTheta >= thetaMin_p + thetaStep*i &&  currentTheta < thetaMin_p + thetaStep*(i+1)){
                isInThetaBin = true;
            }else isInThetaBin = false;
            
            if(currentPhi >= phiMin_p + phiStep*j &&  currentPhi < phiMin_p + phiStep*(j+1)){
                isInPhiBin = true;
            }else isInPhiBin = false;
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
            if(isInThetaBin && isInPhiBin){
                Prot_PullPx_vs_Prob[i][j]->Fill(prob,pullCollection[2][0]);
                Prot_PullPy_vs_Prob[i][j]->Fill(prob,pullCollection[2][1]);
                Prot_PullPz_vs_Prob[i][j]->Fill(prob,pullCollection[2][2]);
                Prot_PullVx_vs_Prob[i][j]->Fill(prob,pullCollection[2][3]);
                Prot_PullVy_vs_Prob[i][j]->Fill(prob,pullCollection[2][4]);
                Prot_PullVz_vs_Prob[i][j]->Fill(prob,pullCollection[2][5]);
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++
        }
        //--------------------------------------------
    }
    //--------------------------------------------
}
//*******************************************************************************************




























































