#ifndef DSelector_pippimeta__B4_M17_ver25_h
#define DSelector_pippimeta__B4_M17_ver25_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_pippimeta__B4_M17_ver25 : public DSelector
{
	public:

		DSelector_pippimeta__B4_M17_ver25(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_pippimeta__B4_M17_ver25(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
    TH1I* dHist_MissingMassSquared;
    TH1I* dHist_MissingMassSquared_allcuts;
    
    TH1I* dHist_Chi2;
    
    TH1I* dHist_BeamEnergy;
    
    TH1I* dHist_Mandlestam_t;
    TH1I* dHist_ShowerQuality;
    TH1I* dHist_ShowerQualityPostCuts;
    
    TH1I* dHist_ProtonZ;
    TH2I* dHist_ProtonR;
    TH1I* dHist_EtaMass;
    TH1I* dHist_EtaMassPostDeltaPP;
    
    
    TH1I* dHist_DeltaPPMass;
    TH1I* dHist_EtaPiPlusPiMinusMass;
    TH1I* dHist_PiPlusPiMinusMass;
    TH1I* dHist_PiPlusPiMinusMassPostDeltaPP;
    
    TH1I* dHist_ProtonPiMinusMass;
    TH1I* dHist_ProtonPiMinusMassPostDeltaPP;
    
    TH1I* dHist_EtaPrimeMass;
    TH1I* dHist_EtaPrimeMassPostDeltaPP;
    
    TH1I* dHist_EtaPiMinusMass;
    TH1I* dHist_EtaPiMinusMassPostDeltaPP;
    TH1I* dHist_EtaPiMinusMassPostDeltaPP_lowt;
    TH1I* dHist_EtaPiMinusMassPostDeltaPP_lowt_vH;
    TH1I* dHist_EtaPiMinusMassShowerCut;
    TH1I* dHist_EtaPiMinusMassShowerCut_lowt;
    
    TH1I* dHist_EtaPiPlusMass;
    TH1I* dHist_EtaPiPlusMassPostDeltaPP;
    
    TH1I* dHist_EtaProtonMass;
    TH1I* dHist_EtaProtonMassPostDeltaPP;
    
    TH2I* dHist_ProtonPiMinusMass_vs_DeltaPPMass;
    TH2I* dHist_PiPlusPiMinusMass_vs_DeltaPPMass;
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusMass;
    TH2I* dHist_PiPlusPiMinusMass2_vs_DeltaPPMass2;
    TH2I* dHist_EtaPiMinusMass_vs_ProtonPiMinusMass;
    TH2I* dHist_EtaPiPlusMass_vs_ProtonPiPlusMass;
    TH2I* dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass;
    TH2I* dHist_EtaPiPlususMass_vs_PiPlusPiMinusMass;
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass;
    
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt;
    TH2I* dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt;
    TH2I* dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt;
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt;
    
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusMass_lowt_vH;
    TH2I* dHist_EtaPiMinusMass_vs_ProtonPiMinusMass_lowt_vH;
    TH2I* dHist_EtaPiMinusMass_vs_PiPlusPiMinusMass_lowt_vH;
    TH2I* dHist_EtaPiMinusMass_vs_EtaPiPlusPiMinusMass_lowt_vH;
    
    TH2I* dHist_CosTheta_vs_EtaPiMinusMass;
    TH2I* dHist_CosTheta_vs_EtaPiMinusMass_lowt;
    TH2I* dHist_CosTheta_vs_EtaPiMinusMass_lowt_vH;
    
    TH2I* dHist_VanHove;
    TH2I* dHist_VanHove_positiveq1;
    TH2I* dHist_VanHove_positiveq2;
    TH2I* dHist_VanHove_positiveq3;
    TH2I* dHist_VanHove_positiveq23;
    TH1I* dHist_DeltaPPMass_vH;
    TH1I* dHist_EtaPiMinusMass_vH;
    TH1I* dHist_EtaPiPlusMass_vH;
    TH1I* dHist_PiPlusPiMinusMass_vH;
    TH1I* dHist_ProtonPiMinusMass_vH;
    TH1I* dHist_EtaPrimeMass_vH;
    
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_vH;
    
    TH2I* dHist_VanHove4d;
    TH2I* dHist_VanHove4d_Actions;
    TH2I* dHist_VanHove4d_Theta;
    TH2I* dHist_VanHove4d_BeamE;
    TH2I* dHist_VanHove4d_Vertex;
    
    TH2I* dHist_VanHove4d_q1q2cut;
    TH1I* dHist_EtaPiPlusMass_VanHove4d;
    TH1I* dHist_EtaPiMinusMass_VanHove4d;
    TH1I* dHist_EtaPiPlusPiMinusMass_VanHove4d;
    TH1I* dHist_PiPlusPiMinusMass_VanHove4d;
    TH1I* dHist_ProtonPiMinusMass_VanHove4d;
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_VanHove4d;
    
    TH1I* dHist_EtaPiMinusMass_Chi2_bins[8];
    TH2I* dHist_CosTheta_vs_EtaPiMinusMass_Chi2_bins[8];
    
    TH2I* dHist_CosTheta_vs_EtaPiMinusMass_DeltaPP_bins[5];
    TH1I* dHist_EtaPiMinusMass_DeltaPP[5];
    
    TH1I* dHist_EtaPiMinusMass_tBins[21];
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins[21];
    
    TH1I* dHist_DeltaPPMass_tBins[21];
    TH1I* dHist_EtaPiPlusMass_tBins_sig[21];
    TH1I* dHist_PiPlusPiMinusMass_tBins_sig[21];
    TH1I* dHist_ProtonPiMinusMass_tBins_sig[21];
    TH1I* dHist_EtaPrimeMass_tBins_sig[21];
    
    TH1I* dHist_EtaPiMinusMass_tBins_back[21];
    TH1I* dHist_EtaPiPlusMass_tBins_back[21];
    TH1I* dHist_PiPlusPiMinusMass_tBins_back[21];
    TH1I* dHist_ProtonPiMinusMass_tBins_back[21];
    TH1I* dHist_EtaPrimeMass_tBins_back[21];
    
    TH1I* dHist_EtaPiPlusMass_tBins_DeltaSub[21];
    TH1I* dHist_PiPlusPiMinusMass_tBins_DeltaSub[21];
    TH1I* dHist_ProtonPiMinusMass_tBins_DeltaSub[21];
    TH1I* dHist_EtaPrimeMass_tBins_DeltaSub[21];
    
    TH1I* dHist_EtaPiMinusMass_tBins_DeltaSub[21];
    TH1I* dHist_EtaPiMinusMass_tBins_ProbWeight[21];
    
    TH1I* dHist_EtaPiMinusMass_tBins_vH[21];
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_vH[21];
    
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_tBins_VanHove_4d[20];
    TH1I* dHist_ProtonPiMinusMass_tBins_VanHove_4d[20];
    TH1I* dHist_PiPlusPiMinusMass_tBins_VanHove_4d[20];
    TH1I* dHist_EtaPiMinusMass_tBins_VanHove_4d[20];
    TH1I* dHist_EtaPiPlusMass_tBins_VanHove_4d[20];
    
    TH1I* dHist_EtaPiMinusMass_RhoSelection;
    TH1I* dHist_CosThetaEta_RhoGJ;
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection;
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ;
    
    TH1I* dHist_PiPlusPiMinusMass_CutDelta;
    TH1I* dHist_EtaPiMinusMass_RhoSelection_CutDelta;
    TH1I* dHist_CosThetaEta_RhoGJ_CutDelta;
    TH1I* dHist_EtaPiPlusPiMinusMass_RhoGJ;
    TH1I* dHist_EtaPiPlusPiMinusMass_RhoGJ_CutDelta;
    
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_RhoSelection_CutDelta;
    TH2I* dHist_CosThetaEta_GJ_vs_EtaPiPlusPiMinusMass_RhoGJ_CutDelta;

	ClassDef(DSelector_pippimeta__B4_M17_ver25, 0);
};

void DSelector_pippimeta__B4_M17_ver25::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_pippimeta__B4_M17_ver25_h
