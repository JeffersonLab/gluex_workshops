
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TStyle.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLine.h"
#include <TRandom3.h>
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"
#include <numeric>

void MakeFlat()
{
    TFile *fileIn=new TFile("flat_data.root");
    TTree *tree=(TTree*)fileIn->Get("pippimeta__B4_M17");
    
    double KinFitChiSqPerNDF,Z_ProtonX4_Measured;
    float sidebandWeight,tagWeight;
    double Beam_px,Beam_py,Beam_pz,Beam_E;
    double Target_px,Target_py,Target_pz,Target_E;
    double PiPlus_px,PiPlus_py,PiPlus_pz,PiPlus_E;
    double PiMinus_px,PiMinus_py,PiMinus_pz,PiMinus_E;
    double Proton_px,Proton_py,Proton_pz,Proton_E;
    double Eta_px,Eta_py,Eta_pz,Eta_E;
    double Mx_Measured;
    float EtaPiMinusMass, DeltaPPMass, Mandlestam_t;

    tree->SetBranchAddress("KinFitChiSqPerNDF",&KinFitChiSqPerNDF);
    tree->SetBranchAddress("Beam_px",&Beam_px);
    tree->SetBranchAddress("Beam_py",&Beam_py);
    tree->SetBranchAddress("Beam_pz",&Beam_pz);
    tree->SetBranchAddress("Beam_E",&Beam_E);
    tree->SetBranchAddress("Target_px",&Target_px);
    tree->SetBranchAddress("Target_py",&Target_py);
    tree->SetBranchAddress("Target_pz",&Target_pz);
    tree->SetBranchAddress("Target_E",&Target_E);
    tree->SetBranchAddress("PiPlus_px",&PiPlus_px);
    tree->SetBranchAddress("PiPlus_py",&PiPlus_py);
    tree->SetBranchAddress("PiPlus_pz",&PiPlus_pz);
    tree->SetBranchAddress("PiPlus_E",&PiPlus_E);
    tree->SetBranchAddress("PiMinus_px",&PiMinus_px);
    tree->SetBranchAddress("PiMinus_py",&PiMinus_py);
    tree->SetBranchAddress("PiMinus_pz",&PiMinus_pz);
    tree->SetBranchAddress("PiMinus_E",&PiMinus_E);
    tree->SetBranchAddress("Eta_px",&Eta_px);
    tree->SetBranchAddress("Eta_py",&Eta_py);
    tree->SetBranchAddress("Eta_pz",&Eta_pz);
    tree->SetBranchAddress("Eta_E",&Eta_E);
    tree->SetBranchAddress("Proton_px",&Proton_px);
    tree->SetBranchAddress("Proton_py",&Proton_py);
    tree->SetBranchAddress("Proton_pz",&Proton_pz);
    tree->SetBranchAddress("Proton_E",&Proton_E);
    tree->SetBranchAddress("sidebandWeight",&sidebandWeight);
    tree->SetBranchAddress("tagWeight",&tagWeight);
    tree->SetBranchAddress("Z_ProtonX4_Measured",&Z_ProtonX4_Measured);
    tree->SetBranchAddress("Mx_Measured",&Mx_Measured);

    tree->SetBranchAddress("EtaPiMinusMass",&EtaPiMinusMass); //fundamental = char, int, float, double, etc.
    tree->SetBranchAddress("Mandlestam_t",&Mandlestam_t); //fundamental = char, int, float, double,
    
    TLorentzVector BeamP4, TargetP4, PiPlusP4, ProtonP4, PiMinusP4, EtaP4;
    
    double xm3s=0.455484;
    double xm6s=0.370737;
    double xp3s=0.624979;
    double xp6s=0.709726;

    Double_t fgID;

    TFile *outFile2=new TFile("Accepted_Data.root","RECREATE");
    TTree *m_OutTree2= new TTree("kin", "kin");
    
    int m_nPart;
    int m_PID[4];
    float m_e[4];
    float m_px[4];
    float m_py[4];
    float m_pz[4];
    float m_eBeam;
    float m_pxBeam;
    float m_pyBeam;
    float m_pzBeam;
    float m_weight;
    float m_TargetMass;
    
    int locNumFinalStateParticles = 4;

    m_OutTree2->Branch("E_Beam", &m_eBeam, "E_Beam/F");
    m_OutTree2->Branch("Px_Beam", &m_pxBeam, "Px_Beam/F");
    m_OutTree2->Branch("Py_Beam", &m_pyBeam, "Py_Beam/F");
    m_OutTree2->Branch("Pz_Beam", &m_pzBeam, "Pz_Beam/F");
    m_OutTree2->Branch("Weight", &m_weight, "Weight/F");
    
    m_OutTree2->Branch("E_FinalState",m_e, "E_FinalState[4]/F");
    m_OutTree2->Branch("Px_FinalState", m_px, "Px_FinalState[4]/F");
    m_OutTree2->Branch("Py_FinalState", m_py, "Py_FinalState[4]/F");
    m_OutTree2->Branch("Pz_FinalState", m_pz, "Pz_FinalState[4]/F");
    
    m_OutTree2->Branch("NumFinalState", &m_nPart,"NumFinalState/I");
    m_nPart = 4;
    
    m_OutTree2->Branch("Target_Mass", &m_TargetMass, "Target_Mass/F");
    m_TargetMass = 0.938272;          // Proton mass in GeV.
    
    m_OutTree2->Branch("PID_FinalState", m_PID, "PID_FinalState[4]/I");
    m_PID[0] = 14;//proton
    m_PID[1] = 17;//eta
    m_PID[2] = 9;//pi-
    m_PID[3] = 8;//pi+


    Long64_t NEntries=tree->GetEntries();
    cout<<"There are "<<NEntries<<" entries."<<endl;
    //NEntries=2500000;

    for (Long64_t i_entry=0; i_entry<NEntries; i_entry++) {
        tree->GetEntry(i_entry);
        if (i_entry%2500000==0)                            //outputs every 500,000 events processed
            cout<<i_entry<<" events processed"<<endl;
        
        BeamP4.SetPxPyPzE(Beam_px,Beam_py,Beam_pz,Beam_E);
        TargetP4.SetPxPyPzE(Target_px,Target_py,Target_pz,Target_E);
        PiPlusP4.SetPxPyPzE(PiPlus_px,PiPlus_py,PiPlus_pz,PiPlus_E);
        PiMinusP4.SetPxPyPzE(PiMinus_px,PiMinus_py,PiMinus_pz,PiMinus_E);
        ProtonP4.SetPxPyPzE(Proton_px,Proton_py,Proton_pz,Proton_E);
        EtaP4.SetPxPyPzE(Eta_px,Eta_py,Eta_pz,Eta_E);
        
        if(BeamP4.E()>9)
            continue;
        
        TLorentzVector DeltaPPP4=ProtonP4+PiPlusP4;
        double DeltaPPMass=DeltaPPP4.M();
        TLorentzVector EtaPiMinusP4=EtaP4+PiMinusP4;
        double EtaPiMinusMass=EtaPiMinusP4.M();
        TLorentzVector RhoP4=PiPlusP4+PiMinusP4;
        double RhoMass=RhoP4.M();
        TLorentzVector EtaPiPlusMassP4=EtaP4+PiPlusP4;
        double EtaPiPlusMass=EtaPiPlusMassP4.M();
        TLorentzVector EtaPiPlusPiMinusMassP4=EtaP4+PiPlusP4+PiMinusP4;
        double EtaPiPlusPiMinusMass=EtaPiPlusPiMinusMassP4.M();
        
        double EtaMass=EtaP4.M();

        if(DeltaPPMass>1.4)
            continue;

            fgID=i_entry;
            
            m_e[3] = PiPlusP4.E();
            m_px[3] = PiPlusP4.Px();
            m_py[3] = PiPlusP4.Py();
            m_pz[3] = PiPlusP4.Pz();
            m_e[2] = PiMinusP4.E();
            m_px[2] = PiMinusP4.Px();
            m_py[2] = PiMinusP4.Py();
            m_pz[2] = PiMinusP4.Pz();
            m_e[1] = EtaP4.E();
            m_px[1] = EtaP4.Px();
            m_py[1] = EtaP4.Py();
            m_pz[1] = EtaP4.Pz();
            m_e[0] = ProtonP4.E();
            m_px[0] = ProtonP4.Px();
            m_py[0] = ProtonP4.Py();
            m_pz[0] = ProtonP4.Pz();
            m_eBeam = BeamP4.E();
            m_pxBeam = BeamP4.Px();
            m_pyBeam = BeamP4.Py();
            m_pzBeam = BeamP4.Pz();
            m_weight=1.;
        
            m_OutTree2->Fill();

    }
    m_OutTree2->Write();
    outFile2->Close();

}
